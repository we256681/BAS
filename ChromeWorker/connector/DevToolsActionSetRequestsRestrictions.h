#ifndef DEVTOOLSACTIONSETREQUESTSRESTRICTIONS_H
#define DEVTOOLSACTIONSETREQUESTSRESTRICTIONS_H

#include "idevtoolsaction.h"
#include "match.h"

class DevToolsActionSetRequestsRestrictions : public IDevToolsAction
{
    void RemoveDuplicates(std::vector<RequestRestriction>& Rules);
public:
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
    virtual ActionSaverBehavior GetActionSaverBehavior();
    virtual bool IsNeedToRunForAllActiveTabs();
    virtual bool IsNeedToRunForAllActiveFrames();
    virtual void OnTabSwitching();
    virtual void OnTabCreation();
    virtual void Run();
};

#endif // DEVTOOLSACTIONSETREQUESTSRESTRICTIONS_H
