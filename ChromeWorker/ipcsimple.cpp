#include "ipcsimple.h"
#include <random>


std::string IPCSimple::GenerateRandomId()
{
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    std::string random_string;

    for (std::size_t i = 0; i < 10; ++i)
    {
        random_string += characters[distribution(generator)];
    }

    return random_string;
}

bool IPCSimple::GetIsActive()
{
    return IsActive;
}

std::string IPCSimple::GetId()
{
    return Id;
}

std::string IPCSimple::Init(const std::string& IdDefault)
{
    Stop();

    this->Id.clear();
    std::string IdLocal;

    if(IdDefault.empty())
    {
        IdLocal = GenerateRandomId();
    }else
    {
        IdLocal = IdDefault;
    }

    std::string IdMem = IdLocal + "mem";
    std::string IdMut = IdLocal + "mut";

    MappingHandler = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 32768,  IdMem.c_str());

    if(MappingHandler == NULL)
    {
        return this->Id;
    }

    Data = (unsigned char *)MapViewOfFile(MappingHandler, FILE_MAP_ALL_ACCESS,0,0,32768);

    if(Data == NULL)
    {
        CloseHandle(MappingHandler);
        return this->Id;
    }

    //Clean
    Data[0] = 0;

    MutexHandler = CreateMutexA(0,false,IdMut.c_str());

    if(MutexHandler == NULL)
    {
        UnmapViewOfFile(Data);
        CloseHandle(MappingHandler);
        return this->Id;
    }

    IsActive = true;
    this->Id = IdLocal;
    return IdLocal;
}

IPCSimple::~IPCSimple()
{
    Stop();
}

void IPCSimple::Stop()
{
    if(!IsActive)
        return;

    UnmapViewOfFile(Data);
    CloseHandle(MappingHandler);
    ReleaseMutex(MutexHandler);
    CloseHandle(MutexHandler);

    IsActive = false;
}


bool IPCSimple::Peek()
{
    if(!IsActive)
        return false;

    bool res = false;
    res = Data[0] > 0;
    return res;
}

std::vector<std::string> IPCSimple::Read()
{
    std::vector<std::string> Res;

    if(!IsActive)
        return Res;

    WaitForSingleObject(MutexHandler,INFINITE);

    int size = Data[0];
    int position = 1;
    for(int i=0;i<size;i++)
    {
        std::string NextMessage((char *)(Data + position));
        Res.push_back(NextMessage);
        position += NextMessage.size() + 1;
    }
    Data[0] = 0;

    ReleaseMutex(MutexHandler);
    return Res;
}

bool IPCSimple::Write(const std::string Id, const std::string& Message)
{
    std::string IdMem = Id + "mem";
    std::string IdMut = Id + "mut";

    //Open mutex and lock
    HANDLE MutexHandler = OpenMutexA(MUTEX_ALL_ACCESS,false,IdMut.c_str());

    if(MutexHandler == NULL)
    {
        return false;
    }

    WaitForSingleObject(MutexHandler,INFINITE);


    //Open mapping
    HANDLE MappingHandler = OpenFileMappingA(FILE_MAP_ALL_ACCESS, false,  IdMem.c_str());

    if(MappingHandler == NULL)
    {
        ReleaseMutex(MutexHandler);
        CloseHandle(MutexHandler);
        return false;
    }

    //Map
    unsigned char *Data = (unsigned char *)MapViewOfFile(MappingHandler, FILE_MAP_ALL_ACCESS,0,0,32768);

    if(Data == NULL)
    {
        CloseHandle(MappingHandler);
        ReleaseMutex(MutexHandler);
        CloseHandle(MutexHandler);
        return false;
    }

    //Do add data
    int CurrentSize = Data[0];
    if(CurrentSize < 10)
    {
        int position = 1;
        for(int i=0;i<CurrentSize;i++)
        {
            std::string NextMessage((char *)(Data + position));
            position += NextMessage.size() + 1;
        }
        std::memcpy(Data + position, Message.data(), Message.size());
        Data[position + Message.size()] = 0;
        CurrentSize++;
        Data[0] = (unsigned char)CurrentSize;
    }

    //Finally close all
    UnmapViewOfFile(Data);
    CloseHandle(MappingHandler);
    ReleaseMutex(MutexHandler);
    CloseHandle(MutexHandler);

    return true;
}

