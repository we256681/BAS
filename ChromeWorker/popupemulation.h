#ifndef POPUPEMULATION_H
#define POPUPEMULATION_H

#include "browserdata.h"
#include "ipcwithfile.h"
#include "mainlayout.h"
#include <windows.h>

class PopupEmulation
{
    BrowserData *Data = 0;
    IPCWithFile SelectElementIPC;
    bool IsActive = false;
    HMENU hMenu = 0;
    int FirstIndex = 0;
    bool GetMenuVisibility();
    HWND hwnd;
    std::string CurrentElementId;
    MainLayout* Layout;
    std::wstring ProxyConfigFolder;
    void ShowMenu(int X, int Y, int Height, std::vector<std::string> Options);
    void CloseMenu(bool ForceClose = false);

public:


    void Init(BrowserData *Data, int FirstIndex, HWND hwnd, MainLayout* Layout, const std::wstring& ProxyConfigFolder);
    void Timer();
    void SetIndex(int Index);

    std::vector<std::function<void()> > EventPopupShown;
};

#endif // POPUPEMULATION_H
