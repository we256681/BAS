#ifndef DEVTOOLSACTIONSLEEP_H
#define DEVTOOLSACTIONSLEEP_H


#include "idevtoolsaction.h"

class DevToolsActionSleep : public IDevToolsAction
{
    long long FinishActionTime = 0;
public:
    virtual void Run();
    virtual void OnWebSocketEvent(const std::string& Method, const std::string& Message);

};


#endif // DEVTOOLSACTIONSLEEP_H
