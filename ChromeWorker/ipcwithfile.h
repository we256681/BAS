#ifndef IPCWITHFILE_H
#define IPCWITHFILE_H

#include <string>
#include <memory>
#include <vector>
#include <windows.h>


class IPCWithFile
{

    std::wstring FileNameData;
    std::wstring FileNameLock;
    bool IsActive = false;


    class LockData
    {
            HANDLE LockHandle = 0;
            std::wstring FileNameLock;
            bool IsSuccess = false;
        public:
            LockData(HANDLE LockHandle, const std::wstring& FileNameLock);
            ~LockData();

            bool GetIsSuccess();
            void Unlock();
    };
    std::shared_ptr<LockData> Lock();

    std::vector<unsigned char> ConvertHexToData(const std::string& Hex);
    std::string ConvertDataToHex(const std::vector<unsigned char>& Data);
    std::string ConvertHexToString(const std::string& Hex);


    bool AppendRaw(const std::string& Data);
    bool DeleteRaw();
    std::string ReadRaw();
    bool IsFileExistsRaw();

public:


    ~IPCWithFile();
    void Init(const std::wstring& FileName);
    void Stop();
    bool GetIsActive();
    bool Peek();
    std::vector<std::string> Read();

    static bool Write(const std::wstring& FileName, const std::string& Message);
};


#endif // IPCWITHFILE_H
