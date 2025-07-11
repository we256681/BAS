#ifndef DEVTOOLSACTIONSETSTARTUPSCRIPT_H
#define DEVTOOLSACTIONSETSTARTUPSCRIPT_H


#include "idevtoolsaction.h"
class DevToolsActionSetStartupScript : public IDevToolsAction
{
    bool IsWaitingForDisablePreviousScript = false;
    bool ClearStartupScripts = true;
    bool RemoveNextScript();
    public:
    virtual void Run();
    virtual ActionSaverBehavior GetActionSaverBehavior();
    bool IsNeedToRunForAllActiveTabs();
    virtual void OnTabCreation();
    virtual void OnTabSwitching();
    virtual void OnRunnningForAllRunActiveTab();
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
};

#endif // DEVTOOLSACTIONSETSTARTUPSCRIPT_H
