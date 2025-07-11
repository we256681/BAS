#ifndef DEVTOOLSACTIONRESTORECOOKIES_H
#define DEVTOOLSACTIONRESTORECOOKIES_H

#include "idevtoolsaction.h"

class DevToolsActionRestoreCookies : public IDevToolsAction
{
    bool IsDeletingCookies = false;
    bool IsSettingCookies = false;
    std::vector<Variant> Cookies;
public:
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
    virtual void Run();
};

#endif // DEVTOOLSACTIONRESTORECOOKIES_H
