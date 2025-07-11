#ifndef DEVTOOLSACTIONWEBSOCKETQUERY_H
#define DEVTOOLSACTIONWEBSOCKETQUERY_H

#include "idevtoolsaction.h"

class DevToolsActionWebsocketQuery : public IDevToolsAction
{
    public:
    enum WebsocketQueryReturnType{
        String,
        Bool,
        Number,
        None
    }ReturnType = None;
    std::string WebSocketMethod;
    std::string ReturnPath;
    std::string ReturnTypeString;
    virtual void Run();
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
};

#endif // DEVTOOLSACTIONWEBSOCKETQUERY_H
