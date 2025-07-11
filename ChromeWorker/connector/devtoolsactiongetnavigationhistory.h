#ifndef DEVTOOLSACTIONGETNAVIGATIONHISTORY_H
#define DEVTOOLSACTIONGETNAVIGATIONHISTORY_H

#include "idevtoolsaction.h"

class DevToolsActionGetNavigationHistory : public IDevToolsAction
{
public:
    virtual void Run();
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
};


#endif // DEVTOOLSACTIONGETNAVIGATIONHISTORY_H
