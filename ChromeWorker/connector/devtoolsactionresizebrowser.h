#ifndef DEVTOOLSACTIONRESIZEBROWSER_H
#define DEVTOOLSACTIONRESIZEBROWSER_H

#include "idevtoolsaction.h"

class DevToolsActionResizeBrowser : public IDevToolsAction
{
    int WindowId = -1;
public:
    virtual void Run();
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
};

#endif // DEVTOOLSACTIONRESIZEBROWSER_H
