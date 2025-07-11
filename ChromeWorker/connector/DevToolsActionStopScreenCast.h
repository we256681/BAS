#ifndef DEVTOOLSACTIONSTOPSCREENCAST_H
#define DEVTOOLSACTIONSTOPSCREENCAST_H

#include "idevtoolsaction.h"


class DevToolsActionStopScreenCast : public IDevToolsAction
{
public:
    virtual void Run();
    virtual bool FilterActionSaver(std::shared_ptr<IDevToolsAction> Action);
    virtual ActionSaverBehavior GetActionSaverBehavior();
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
};

#endif // DEVTOOLSACTIONSTOPSCREENCAST_H