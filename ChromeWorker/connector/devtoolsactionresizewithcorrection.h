#ifndef DEVTOOLSACTIONRESIZEWITHCORRECTION_H
#define DEVTOOLSACTIONRESIZEWITHCORRECTION_H

#include "DevToolsMultiAction.h"


class DevToolsActionResizeWithCorrection : public DevToolsMultiAction
{
    int InitialWidth = -1;
    int InitialHeight = -1;
    bool IsResizing = false;
    bool IsCheckingSize = false;
    int TryNumber = 0;
    void Try();
public:
    virtual void Run();
    virtual void OnActionFinished();
};

#endif // DEVTOOLSACTIONRESIZEWITHCORRECTION_H
