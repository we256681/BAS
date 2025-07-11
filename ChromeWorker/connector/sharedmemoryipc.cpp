#include "sharedmemoryipc.h"

void SharedMemoryIPC::SetError(const std::string& CurrentErrorString)
{
    IsError = true;
    this->ErrorString = CurrentErrorString;
}
void SharedMemoryIPC::ClearError()
{
    IsError = false;
    ErrorString.clear();
}
int32_t SharedMemoryIPC::GetWord(int Offset)
{
    if(!Data)
        return -1;

    int32_t Res = 0;

    Res += Data[Offset];
    Res += Data[Offset + 1] << 8;
    Res += Data[Offset + 2] << 16;
    Res += Data[Offset + 3] << 24;

    return Res;
}
void SharedMemoryIPC::SetWord(int Offset, int32_t Number)
{
    if(!Data)
        return;

    Data[Offset] = (unsigned char)((Number & 0xFF));
    Data[Offset + 1] = (unsigned char)((Number >> 8) & 0xFF);
    Data[Offset + 2] = (unsigned char)((Number >> 16) & 0xFF);
    Data[Offset + 3] = (unsigned char)((Number >> 24) & 0xFF);

}

unsigned char SharedMemoryIPC::GetByte(int Offset)
{
    if(!Data)
        return 0;

    return Data[Offset];
}

void SharedMemoryIPC::SetByte(int Offset, unsigned char Number)
{
    if(!Data)
        return;

    Data[Offset] = Number;
}

std::string SharedMemoryIPC::GetString(int Offset)
{
    if(!Data)
        return std::string();

    return std::string((const char *)(Data +Offset));
}
void SharedMemoryIPC::SetString(int Offset, const std::string& Text)
{
    if(!Data)
        return;
    if(!Text.empty())
        memcpy(Data + Offset,Text.data(),Text.length());
    Data[Offset + Text.length()] = 0;
}
SharedMemoryIPC::SharedMemoryIPC()
{
}

SharedMemoryIPC::~SharedMemoryIPC()
{
    Stop();
}

bool SharedMemoryIPC::Start(const std::string& Id)
{
    #if defined(_WIN32) || defined(WIN32) || defined(OS_WIN)
        Stop();
        this->MemoryName = std::string("BASMLAMEMORY") + Id;
        this->MutexName = std::string("BASMLAMUTEX") + Id;

        MappingHandler = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_RESERVE, 0, SHARED_MEMORY_MAXIMUM_SIZE,  MemoryName.c_str());

        if(MappingHandler == NULL)
        {
            Stop();
            SetError(std::string("Error during file mapping creation: ") + std::to_string(GetLastError()));
            return false;
        }

        Data = (unsigned char *)MapViewOfFile(MappingHandler, FILE_MAP_ALL_ACCESS,0,0,SHARED_MEMORY_MAXIMUM_SIZE);

        if(Data == NULL)
        {
            Stop();
            SetError(std::string("Error during opening file mapping: ") + std::to_string(GetLastError()));
            return false;
        }

        //Allocate first portion of memory
        if(VirtualAlloc(Data, SHARED_MEMORY_INITIAL_SIZE, MEM_COMMIT, PAGE_READWRITE) == NULL)
        {
            Stop();
            SetError(std::string("Error during allocating memory: ") + std::to_string(GetLastError()));
            return false;
        }

        //Initialize
        Data[0] = 1;
        SetWord(SHARED_MEMORY_OFFSET_IMAGE_WIDTH, 0);
        SetWord(SHARED_MEMORY_OFFSET_IMAGE_HEIGHT, 0);
        SetWord(SHARED_MEMORY_OFFSET_IMAGE_ID, 0);
        SetWord(SHARED_MEMORY_OFFSET_ACTUAL_SIZE, SHARED_MEMORY_INITIAL_SIZE);


        MutexHandler = CreateMutexA(0,false,MutexName.c_str());

        if(MutexHandler == NULL)
        {
            Stop();
            SetError(std::string("Error creating mutex: ") + std::to_string(GetLastError()));
            return false;
        }

        IsStarted = true;

        return true;
    #else
        SetError("Wrong platform");
        return false;
    #endif

}

bool SharedMemoryIPC::Connect(const std::string& Id)
{
    #if defined(_WIN32) || defined(WIN32) || defined(OS_WIN)
        Stop();

        this->MemoryName = std::string("BASMLAMEMORY") + Id;
        this->MutexName = std::string("BASMLAMUTEX") + Id;

        MappingHandler = OpenFileMappingA(FILE_MAP_ALL_ACCESS, false,  MemoryName.c_str());

        if(MappingHandler == NULL)
        {
            Stop();
            SetError(std::string("Error during file mapping creation: ") + std::to_string(GetLastError()));
            return false;
        }

        Data = (unsigned char *)MapViewOfFile(MappingHandler, FILE_MAP_ALL_ACCESS,0,0,SHARED_MEMORY_MAXIMUM_SIZE);

        if(Data == NULL)
        {
            Stop();
            SetError(std::string("Error during opening file mapping: ") + std::to_string(GetLastError()));
            return false;
        }

        //Set version
        if(Data[0] != 1)
        {
            Stop();
            SetError(std::string("Wrong version of file mapper: ") + std::to_string(Data[0]));
            return false;
        }

        MutexHandler = OpenMutexA(MUTEX_ALL_ACCESS,false,MutexName.c_str());


        if(MutexHandler == NULL)
        {
            Stop();
            SetError(std::string("Error opening mutex: ") + std::to_string(GetLastError()));
            return false;
        }

        IsStarted = true;

        return true;
    #else
        SetError("Wrong platform");
        return false;
    #endif

}


bool SharedMemoryIPC::Stop()
{
    ClearError();
    MemoryName.clear();
    MutexName.clear();

    #if defined(_WIN32) || defined(WIN32) || defined(OS_WIN)
        if(Data)
            UnmapViewOfFile(Data);

        Data = 0;

        if(MappingHandler)
            CloseHandle(MappingHandler);

        MappingHandler = 0;

        if(MutexHandler)
            CloseHandle(MutexHandler);

        MutexHandler = 0;

        IsStarted = false;


    #else
        Data = 0;
        MappingHandler = 0;
        MutexHandler = 0;
        IsStarted = false;
    #endif

    return true;


}

bool SharedMemoryIPC::Lock()
{
    ClearError();

    if(!IsStarted)
    {
        SetError(std::string("Need to start first"));
        return false;
    }

    #if defined(_WIN32) || defined(WIN32) || defined(OS_WIN)
        WaitForSingleObject(MutexHandler,INFINITE);
    #endif

    return true;
}

bool SharedMemoryIPC::Unlock()
{
    ClearError();

    if(!IsStarted)
    {
        SetError(std::string("Need to start first"));
        return false;
    }
    #if defined(_WIN32) || defined(WIN32) || defined(OS_WIN)
        ReleaseMutex(MutexHandler);
    #endif
    return true;
}

unsigned char* SharedMemoryIPC::GetImagePointer()
{
    if(!Data)
        return 0;

    return Data + SHARED_MEMORY_OFFSET_IMAGE_DATA + 4;
}

int32_t SharedMemoryIPC::GetImageWidth()
{
    return GetWord(SHARED_MEMORY_OFFSET_IMAGE_WIDTH);
}

void SharedMemoryIPC::SetImageWidth(int32_t Value)
{
    SetWord(SHARED_MEMORY_OFFSET_IMAGE_WIDTH, Value);
}

int32_t SharedMemoryIPC::GetImageHeight()
{
    return GetWord(SHARED_MEMORY_OFFSET_IMAGE_HEIGHT);
}

void SharedMemoryIPC::SetImageHeight(int32_t Value)
{
    SetWord(SHARED_MEMORY_OFFSET_IMAGE_HEIGHT, Value);
}

int32_t SharedMemoryIPC::GetImageId()
{
    return GetWord(SHARED_MEMORY_OFFSET_IMAGE_ID);
}

void SharedMemoryIPC::SetImageId(int32_t Value)
{
    SetWord(SHARED_MEMORY_OFFSET_IMAGE_ID, Value);
}

int32_t SharedMemoryIPC::GetImageSize()
{
    return GetWord(SHARED_MEMORY_OFFSET_IMAGE_DATA);
}

void SharedMemoryIPC::SetImageSize(int32_t Value)
{
    SetWord(SHARED_MEMORY_OFFSET_IMAGE_DATA, Value);
}

int32_t SharedMemoryIPC::GetSize()
{
    return GetWord(SHARED_MEMORY_OFFSET_ACTUAL_SIZE) - SHARED_MEMORY_OFFSET_IMAGE_DATA - 4;
}

bool SharedMemoryIPC::Resize(int32_t Value)
{
    //Add space for information about image
    Value += SHARED_MEMORY_OFFSET_IMAGE_DATA + 4;

    if(!Data)
    {
        Stop();
        SetError(std::string("Error during growing memory: data not yet initilized."));
        return false;
    }

    if(Value > SHARED_MEMORY_MAXIMUM_SIZE)
    {
        Stop();
        SetError(std::string("Error during growing memory: size is too big."));
        return false;
    }

    int32_t ActualSize = GetWord(SHARED_MEMORY_OFFSET_ACTUAL_SIZE);
    if(Value <= ActualSize)
        return true;
    int32_t NewBlocksNumber = (Value - ActualSize) / SHARED_MEMORY_CHUNK_SIZE;
    if((Value - ActualSize) % SHARED_MEMORY_CHUNK_SIZE != 0)
        NewBlocksNumber += 1;

    if(VirtualAlloc(Data + ActualSize, NewBlocksNumber * SHARED_MEMORY_CHUNK_SIZE, MEM_COMMIT, PAGE_READWRITE) == NULL)
    {
        Stop();
        SetError(std::string("Error during growing memory: ") + std::to_string(GetLastError()));
        return false;
    }

    SetWord(SHARED_MEMORY_OFFSET_ACTUAL_SIZE, ActualSize + NewBlocksNumber * SHARED_MEMORY_CHUNK_SIZE);
    return true;
}

bool SharedMemoryIPC::GetIsStarted()
{
    return IsStarted;
}

bool SharedMemoryIPC::GetIsError()
{
    return IsError;
}

std::string SharedMemoryIPC::GetErrorString()
{
    return ErrorString;
}

SharedMemoryIPCLockGuard::SharedMemoryIPCLockGuard(SharedMemoryIPC * IPC)
{
    this->IPC = IPC;
    IPC->Lock();
}

SharedMemoryIPCLockGuard::~SharedMemoryIPCLockGuard()
{
    IPC->Unlock();
}
