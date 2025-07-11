#ifndef DEVTOOLSACTIONRELOAD_H
#define DEVTOOLSACTIONRELOAD_H

#include "idevtoolsaction.h"

class DevToolsActionReload : public IDevToolsAction
{
    std::string FrameId;
public:
    virtual void Run();
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
    virtual void OnWebSocketEvent(const std::string& Method,const std::string& Message);
};

#endif // DEVTOOLSACTIONRELOAD_H
