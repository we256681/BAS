#ifndef DEVTOOLSACTIONRESETDEVICESCALEFACTOR_H
#define DEVTOOLSACTIONRESETDEVICESCALEFACTOR_H

#include "idevtoolsaction.h"

class DevToolsActionResetDeviceScaleFactor : public IDevToolsAction
{
    long long NextCheck = 0;
    std::wstring Path;

public:
    virtual void Run();
    virtual void OnWebSocketEvent(const std::string& Method, const std::string& Message);
};

#endif // DEVTOOLSACTIONRESETDEVICESCALEFACTOR_H
