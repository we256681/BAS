#ifndef DEVTOOLSACTIONSETPROXY_H
#define DEVTOOLSACTIONSETPROXY_H

#include "idevtoolsaction.h"

class DevToolsActionSetProxy : public IDevToolsAction
{
	long long FinishActionTime = 0;
    bool WaitingForStopNetworkActivity = false;
    bool WaitingForResetAllConnections = false;
    bool WaitingForSetProxy = false;
public:
	virtual void Run();
	virtual void OnWebSocketEvent(const std::string& Method, const std::string& Message);

};

#endif // DEVTOOLSACTIONSETPROXY_H
