#ifndef CHROMEPROCESSLAUNCHER_H
#define CHROMEPROCESSLAUNCHER_H

#include <string>
#include <windows.h>

class ChromeProcessLauncher
{
    HANDLE hProcess = 0;
    long long GetTime();
    std::wstring LastFile;
    std::wstring LastParams;
    std::wstring LastFolder;
    long long LastStarted = 0;
    bool NeedToStartProcess = false;
    int RestartNumber = 0;

    void StartInternal();
    bool IsProcessRunning();
public:
    void Stop();
    void Timer();
    void Start(const std::wstring& File, const std::wstring& Params, const std::wstring& Folder);
};

#endif // CHROMEPROCESSLAUNCHER_H
