#ifndef ICONLISTENER_H
#define ICONLISTENER_H

#include <windows.h>
#include "TrayIcon.h"

class IconListener : public ITrayIconListener
{
public:
    HWND hwnd;
    bool NeedToClose = false;
    void Show();
    void Close();
    virtual void OnTrayIconRButtonUp(CTrayIcon* pTrayIcon);
    virtual void OnTrayIconLButtonUp(CTrayIcon* pTrayIcon);

};

#endif // ICONLISTENER_H
