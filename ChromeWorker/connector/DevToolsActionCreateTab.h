#ifndef DEVTOOLSACTIONCREATETAB_H
#define DEVTOOLSACTIONCREATETAB_H

#include "idevtoolsaction.h"

class DevToolsActionCreateTab : public IDevToolsAction
{
    std::string TargetId;
    std::string Referrer;
    std::string Url;
    bool IsInstant;
    bool IsTargetCreated = false;
    bool IsTabConnected = false;
    bool IsDelayed = false;
    
    void StartLoad();
public:
    virtual void Run();
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
    virtual void OnWebSocketEvent(const std::string& Method, const std::string& Message);
};

#endif // DEVTOOLSACTIONCREATETAB_H
