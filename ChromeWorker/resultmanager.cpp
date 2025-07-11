#include "resultmanager.h"
#include "log.h"

void ResultManager::Init(IDevToolsConnector *Connector)
{
    this->Connector = Connector;
}

Async ResultManager::ProcessResult(Async Result)
{
    if(LastActionUniqueId)
    {
        Connector->InterruptAction(LastActionUniqueId);
    }

    LastActionUniqueId = Result->GetActionUniqueId();

    return Result;
}
