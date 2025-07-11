#include "iconlistener.h"


void IconListener::Show()
{
    ShowWindow(hwnd,SW_SHOW);

    SwitchToThisWindow(hwnd,true);
    HWND hCurWnd = ::GetForegroundWindow();
    DWORD dwMyID = ::GetCurrentThreadId();
    DWORD dwCurID = ::GetWindowThreadProcessId(hCurWnd, NULL);
    ::AttachThreadInput(dwCurID, dwMyID, TRUE);
    ::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    ::SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    ::SetForegroundWindow(hwnd);
    ::AttachThreadInput(dwCurID, dwMyID, FALSE);
    ::SetFocus(hwnd);
    ::SetActiveWindow(hwnd);
}
void IconListener::OnTrayIconRButtonUp(CTrayIcon* pTrayIcon)
{
    POINT p;
    if (GetCursorPos(&p))
    {

        HMENU hPopupMenu = CreatePopupMenu();
        InsertMenu(hPopupMenu, 1, MF_BYPOSITION | MF_STRING, 1, L"Show");
        InsertMenu(hPopupMenu, 2, MF_BYPOSITION | MF_STRING, 2, L"Close");
        int res = TrackPopupMenu(hPopupMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN | TPM_RETURNCMD, p.x, p.y, 0, hwnd, NULL);
        if(res == 1)
        {
            Show();
        }else if(res == 2)
        {
            NeedToClose = true;
            DestroyWindow(hwnd);
        }
        DestroyMenu(hPopupMenu);
    }
}

void IconListener::Close()
{
    NeedToClose = true;
    DestroyWindow(hwnd);
}


void IconListener::OnTrayIconLButtonUp(CTrayIcon* pTrayIcon)
{
    Show();
}

