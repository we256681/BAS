#ifndef DEVTOOLSACTIONOPENFILE_H
#define DEVTOOLSACTIONOPENFILE_H


#include "idevtoolsaction.h"

class DevToolsActionOpenFile : public IDevToolsAction
{
    long long FinishActionTime = 0;
    double NodeId = 0.0;
    bool OpenFileDialogIsManual = false;
    std::string OpenFileDialogResult;
    std::string TargetIdForElement;
    bool IsMultiple = false;
public:
    virtual void Run();
    virtual void OnWebSocketEvent(const std::string& Method, const std::string& Message);

};

#endif // DEVTOOLSACTIONOPENFILE_H
