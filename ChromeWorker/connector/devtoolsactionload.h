#ifndef DEVTOOLSACTIONLOAD_H
#define DEVTOOLSACTIONLOAD_H

#include "idevtoolsaction.h"

class DevToolsActionLoad : public IDevToolsAction
{
    std::string FrameId;
public:
    virtual void Run();
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
    virtual void OnWebSocketEvent(const std::string& Method,const std::string& Message);
};

#endif // DEVTOOLSACTIONLOAD_H
