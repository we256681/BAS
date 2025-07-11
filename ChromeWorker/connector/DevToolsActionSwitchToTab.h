#ifndef DEVTOOLSACTIONSWITCHTOTAB_H
#define DEVTOOLSACTIONSWITCHTOTAB_H

#include "idevtoolsaction.h"

class DevToolsActionSwitchToTab : public IDevToolsAction
{
    std::string SwitchToTabId;
    std::string SwitchToTabFrameId;

public:
    virtual void Run();
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
    virtual void OnWebSocketEvent(const std::string& Method, const std::string& Message);
};

#endif // DEVTOOLSACTIONSWITCHTOTAB_H
