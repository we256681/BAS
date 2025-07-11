#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include <string>
#include "include/cef_app.h"

class NotificationManager
{
    std::string UniqueId;

public:
    void Init(const std::string& UniqueId);
    void Timer(CefRefPtr<CefBrowser> BrowserToolbox);
};

#endif // NOTIFICATIONMANAGER_H
