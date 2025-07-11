#ifndef DEVTOOLSACTIONFACTORY_H
#define DEVTOOLSACTIONFACTORY_H

#include "idevtoolsaction.h"
#include "devtoolsactionwebsocketquery.h"
#include "DevToolsGlobalState.h"

class DevToolsActionFactory
{
    int Rand();
    public:
    IDevToolsAction* Create(const std::string& Name, DevToolsGlobalState *GlobalState);
    IDevToolsAction* CreateWebsocketQuery(const std::string& WebSocketMethod, DevToolsGlobalState* GlobalState, const std::string& ReturnPath, DevToolsActionWebsocketQuery::WebsocketQueryReturnType ReturnType);
};

#endif // DEVTOOLSACTIONFACTORY_H
