#ifndef DEVTOOLSACTIONGETBROWSERSIZE_H
#define DEVTOOLSACTIONGETBROWSERSIZE_H

#include "idevtoolsaction.h"

class DevToolsActionGetBrowserSize : public IDevToolsAction
{
public:
    virtual void Run();
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
};

#endif // DEVTOOLSACTIONGETBROWSERSIZE_H
