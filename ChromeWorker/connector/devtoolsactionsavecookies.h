#ifndef DEVTOOLSACTIONSAVECOOKIES_H
#define DEVTOOLSACTIONSAVECOOKIES_H

#include "idevtoolsaction.h"

class DevToolsActionSaveCookies : public IDevToolsAction
{
public:
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
    virtual void Run();
};

#endif // DEVTOOLSACTIONSAVECOOKIES_H
