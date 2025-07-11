#ifndef DEVTOOLSACTIONSTARTDRAGFILE_H
#define DEVTOOLSACTIONSTARTDRAGFILE_H

#include "idevtoolsaction.h"


class DevToolsActionStartDragFile : public IDevToolsAction
{
public:
    virtual void Run();
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
};

#endif // DEVTOOLSACTIONSTARTDRAGFILE_H
