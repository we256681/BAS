#ifndef SHAREDMEMORYIPC_H
#define SHAREDMEMORYIPC_H

#include <string>
#include <vector>
#include <windows.h>


#define SHARED_MEMORY_CHUNK_SIZE 1048576
#define SHARED_MEMORY_INITIAL_SIZE 3145728
#define SHARED_MEMORY_MAXIMUM_SIZE 104857600
#define SHARED_MEMORY_OFFSET_IMAGE_WIDTH 1
#define SHARED_MEMORY_OFFSET_IMAGE_HEIGHT 5
#define SHARED_MEMORY_OFFSET_IMAGE_ID 9
#define SHARED_MEMORY_OFFSET_ACTUAL_SIZE 13
#define SHARED_MEMORY_OFFSET_IMAGE_DATA 17

class SharedMemoryIPC
{
    bool IsStarted = false;
    bool IsError = false;
    std::string ErrorString;
    std::string MemoryName;
    std::string MutexName;

    HANDLE MappingHandler = 0;
    HANDLE MutexHandler = 0;

    unsigned char *Data = 0;
    void SetError(const std::string& CurrentErrorString);
    void ClearError();
    int32_t GetWord(int Offset);
    void SetWord(int Offset, int32_t Number);
    unsigned char GetByte(int Offset);
    void SetByte(int Offset, unsigned char Number);
    std::string GetString(int Offset);
    void SetString(int Offset, const std::string& Text);
public:
    SharedMemoryIPC();
    ~SharedMemoryIPC();

    bool Start(const std::string& Id);
    bool Connect(const std::string& Id);
    bool Stop();
    bool Lock();
    bool Unlock();
    int32_t GetImageWidth();
    void SetImageWidth(int32_t Value);
    int32_t GetImageHeight();
    void SetImageHeight(int32_t Value);
    int32_t GetImageId();
    void SetImageId(int32_t Value);
    int32_t GetImageSize();
    void SetImageSize(int32_t Value);
    int32_t GetSize();
    bool Resize(int32_t Value);
    unsigned char* GetImagePointer();


    bool GetIsStarted();
    bool GetIsError();
    std::string GetErrorString();
};

class SharedMemoryIPCLockGuard
{
    SharedMemoryIPC * IPC;
    public:
    SharedMemoryIPCLockGuard(SharedMemoryIPC * IPC);
    ~SharedMemoryIPCLockGuard();
};

#endif // SHAREDMEMORYIPC_H
