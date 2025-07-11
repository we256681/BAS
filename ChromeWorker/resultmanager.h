#ifndef RESULTMANAGER_H
#define RESULTMANAGER_H

#include "idevtoolsconnector.h"

class ResultManager
{
    IDevToolsConnector *Connector = 0;
    int LastActionUniqueId = 0;
public:
    void Init(IDevToolsConnector *Connector);
    Async ProcessResult(Async Result);
};

#endif // RESULTMANAGER_H
