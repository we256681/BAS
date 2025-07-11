#ifndef DEVTOOLSACTIONEMPTY_H
#define DEVTOOLSACTIONEMPTY_H

#include "idevtoolsaction.h"

class DevToolsActionEmpty : public IDevToolsAction
{
public:
    virtual void Run();
};

#endif // DEVTOOLSACTIONEMPTY_H
