#include "chromeprocesslauncher.h"
#include <chrono>

using namespace std::chrono;

long long ChromeProcessLauncher::GetTime()
{
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

bool ChromeProcessLauncher::IsProcessRunning()
{
    //Process is not started or error during start
    if(!hProcess)
        return false;

    //Process is running
    if(WaitForSingleObject(hProcess, 0) == WAIT_TIMEOUT)
        return true;

    //Process is finished, close handle
    CloseHandle(hProcess);
    hProcess = 0;

    return false;
}


void ChromeProcessLauncher::StartInternal()
{
    LastStarted = GetTime();

    SHELLEXECUTEINFO ShExecInfo = {0};

    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = NULL;
    ShExecInfo.lpFile = LastFile.c_str();
    ShExecInfo.lpParameters = LastParams.c_str();
    ShExecInfo.lpDirectory = LastFolder.c_str();
    ShExecInfo.nShow = SW_SHOW;
    ShExecInfo.hInstApp = NULL;

    ShellExecuteEx(&ShExecInfo);

    hProcess = ShExecInfo.hProcess;
}

void ChromeProcessLauncher::Timer()
{
    if(!NeedToStartProcess)
        return;

    int RestartInterval = 30000;
    if(RestartNumber == 0)
    {
        RestartInterval = 3000;
    }else if(RestartNumber == 1)
    {
        RestartInterval = 5000;
    }else if(RestartNumber == 2)
    {
        RestartInterval = 10000;
    }

    if(!IsProcessRunning() && GetTime() - LastStarted > RestartInterval)
    {
        RestartNumber++;
        StartInternal();
    }
}

void ChromeProcessLauncher::Stop()
{
    NeedToStartProcess = false;
    if(hProcess)
    {
        CloseHandle(hProcess);
        hProcess = 0;
    }
}


void ChromeProcessLauncher::Start(const std::wstring& File, const std::wstring& Params, const std::wstring& Folder)
{
    LastFile = File;
    LastParams = Params;
    LastFolder = Folder;
    NeedToStartProcess = true;
    RestartNumber = 0;

    StartInternal();

}
