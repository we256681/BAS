#ifndef DEVTOOLSACTIONSTARTSCREENCAST_H
#define DEVTOOLSACTIONSTARTSCREENCAST_H

#include "idevtoolsaction.h"


class DevToolsActionStartScreenCast : public IDevToolsAction
{
    bool IsStoppingPreviousScreencast = false;
public:
    virtual bool FilterActionSaver(std::shared_ptr<IDevToolsAction> Action);
    virtual ActionSaverBehavior GetActionSaverBehavior();
    virtual bool IsNeedToRunForAllActiveTabs();
    virtual void OnTabCreation();
    virtual void OnTabSwitching();
    virtual void Run();
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
};

#endif // DEVTOOLSACTIONSTARTSCREENCAST_H