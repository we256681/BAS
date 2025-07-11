#include "ipcwithfile.h"
#include <fstream>
#include <sstream>
#include <iomanip>


std::vector<unsigned char> IPCWithFile::ConvertHexToData(const std::string& Hex)
{
    std::vector<unsigned char> Res;
    int Char = 0;
    std::size_t CharNumber;

    for(unsigned int i = 0;i<Hex.length() / 2;i++)
    {
        std::string Substr = Hex.substr(i*2,2);
        Char = std::stoi(Substr,&CharNumber,16);
        Res.push_back((unsigned char)Char);
    }
    return Res;
}

std::string IPCWithFile::ConvertDataToHex(const std::vector<unsigned char>& Data)
{
    std::stringstream Res;
    for(unsigned int i = 0;i<Data.size();i++)
    {
        Res<<std::hex<<std::setfill ('0')<<std::setw(2)<<(int)Data[i];
    }
    return Res.str();
}

std::string IPCWithFile::ConvertHexToString(const std::string& Hex)
{
    std::vector<unsigned char> Data = ConvertHexToData(Hex);
    std::string Res = std::string(Data.begin(),Data.end());
    return Res;
}


IPCWithFile::LockData::LockData(HANDLE LockHandle, const std::wstring& FileNameLock)
{
    this->LockHandle = LockHandle;
    this->FileNameLock = FileNameLock;
    IsSuccess = LockHandle != INVALID_HANDLE_VALUE;
}

IPCWithFile::LockData::~LockData()
{
    Unlock();
}

void IPCWithFile::LockData::Unlock()
{
    if(!IsSuccess)
        return;

    OVERLAPPED Overlapped;
    memset(&Overlapped, 0, sizeof(Overlapped));
    UnlockFileEx(LockHandle, 0, MAXDWORD, MAXDWORD, &Overlapped);
    CloseHandle(LockHandle);
    if(!FileNameLock.empty())
        DeleteFile(FileNameLock.c_str());
}

bool IPCWithFile::LockData::GetIsSuccess()
{
    return IsSuccess;
}

std::shared_ptr<IPCWithFile::LockData> IPCWithFile::Lock()
{
    HANDLE LockHandle = CreateFile(FileNameLock.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS, 0, 0);
    if(LockHandle == INVALID_HANDLE_VALUE)
    {
        return std::make_shared<IPCWithFile::LockData>(INVALID_HANDLE_VALUE, std::wstring());
    }
    OVERLAPPED Overlapped;
    memset(&Overlapped, 0, sizeof(Overlapped));
    bool Result = LockFileEx(LockHandle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &Overlapped);
    if(!Result)
    {
        CloseHandle(LockHandle);
        return std::make_shared<IPCWithFile::LockData>(INVALID_HANDLE_VALUE, std::wstring());
    }

    return std::make_shared<IPCWithFile::LockData>(LockHandle, FileNameLock);
}


bool IPCWithFile::AppendRaw(const std::string& Data)
{
    HANDLE FileHandle = CreateFile(FileNameData.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 0, OPEN_ALWAYS, 0, 0);

    if(FileHandle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    bool Result = true;
    DWORD TotalBytesWritten = 0;
    while(TotalBytesWritten < Data.size())
    {
        DWORD ResultSetPointer = SetFilePointer(FileHandle, 0, NULL, FILE_END);
        if(ResultSetPointer == INVALID_SET_FILE_POINTER)
        {
            Result = false;
            break;
        }
        DWORD CurrentBytesWritten = 0;
        bool ResultWrite = WriteFile(FileHandle, Data.data() + TotalBytesWritten, (DWORD)Data.size() - TotalBytesWritten, &CurrentBytesWritten, NULL);
        if(!ResultWrite)
        {
            Result = false;
            break;
        }
        TotalBytesWritten += CurrentBytesWritten;
    }

    CloseHandle(FileHandle);
    return Result;
}

bool IPCWithFile::DeleteRaw()
{
    return DeleteFile(FileNameData.c_str());
}

std::string IPCWithFile::ReadRaw()
{
    HANDLE FileHandle = CreateFile(FileNameData.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

    if(FileHandle == INVALID_HANDLE_VALUE)
    {
        return std::string();
    }

    DWORD FileSize = GetFileSize(FileHandle, NULL);
    if (FileSize == INVALID_FILE_SIZE)
    {
        CloseHandle(FileHandle);
        return std::string();
    }

    std::string Result;
    Result.resize(FileSize);

    DWORD TotalBytesRead = 0;
    while(TotalBytesRead < FileSize)
    {
        DWORD CurrentBytesRead = 0;
        DWORD SizeToRead = FileSize - TotalBytesRead;
        bool ResultWrite = ReadFile(FileHandle, (void *)(Result.data() + TotalBytesRead), SizeToRead, &CurrentBytesRead, NULL);
        if(!ResultWrite)
        {
            break;
        }
        TotalBytesRead += CurrentBytesRead;
    }


    CloseHandle(FileHandle);
    return Result;
}

bool IPCWithFile::IsFileExistsRaw()
{
    DWORD dwAttrib = GetFileAttributes(FileNameData.c_str());

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
           !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void IPCWithFile::Init(const std::wstring& FileName)
{
    Stop();
    this->FileNameData = FileName;
    this->FileNameLock = FileName + std::wstring(L".lock");
    IsActive = true;
}

IPCWithFile::~IPCWithFile()
{
    Stop();
}

bool IPCWithFile::GetIsActive()
{
    return IsActive;
}

bool IPCWithFile::Peek()
{
    return IsFileExistsRaw();
}

void IPCWithFile::Stop()
{
    IsActive = false;
}

bool IPCWithFile::Write(const std::wstring& FileName, const std::string& Message)
{
    IPCWithFile Instance;
    Instance.Init(FileName);
    std::shared_ptr<IPCWithFile::LockData> LockInstance = Instance.Lock();
    if(!LockInstance->GetIsSuccess())
    {
        return false;
    }

    std::vector<unsigned char> Data(Message.begin(), Message.end());
    std::string MessagePrepared = Instance.ConvertDataToHex(Data);

    if(Instance.IsFileExistsRaw())
    {
        MessagePrepared = std::string("\n") + MessagePrepared;
    }

    return Instance.AppendRaw(MessagePrepared);
}

std::vector<std::string> IPCWithFile::Read()
{
    std::shared_ptr<IPCWithFile::LockData> LockInstance = Lock();
    if(!LockInstance->GetIsSuccess())
    {
        return std::vector<std::string>();
    }

    if(!IsFileExistsRaw())
    {
        return std::vector<std::string>();
    }

    std::string RawData = ReadRaw();
    DeleteRaw();

    std::vector<std::string> Result;
    std::stringstream ss(RawData);
    std::string item;
    while (std::getline(ss, item, '\n'))
    {
        std::vector<unsigned char> Data = ConvertHexToData(item);
        std::string String(Data.begin(), Data.end());
        Result.push_back(String);
    }

    return Result;
}

