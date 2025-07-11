#include "DevToolsActionGetCurrentUrl.h"

void DevToolsActionGetCurrentUrl::Run()
{
    State = Running;
	RunAction("GetTabsList");
}

void DevToolsActionGetCurrentUrl::OnActionFinished()
{
	if(CurrentAction->GetResult()->GetIsSuccess())
	{
        int Index = 0;
        for(auto const& Tab : GlobalState->Tabs)
        {
            if(Tab->ConnectionState == TabData::Connected || Tab->ConnectionState == TabData::Delayed)
            {
                if(Tab->TabId == GlobalState->TabId)
                {
                    break;
                }
                Index++;
            }
        }
        std::vector<std::string> Urls = CurrentAction->GetResult()->GetList();
        if(Index >= Urls.size())
        {
            Result->Fail("Current tab not found");
            State = Finished;
            return;
        }

        Result->Success(Urls.at(Index));
        State = Finished;
        return;
	} else
	{
        Result->Fail(CurrentAction->GetResult()->GetErrorMessage(), CurrentAction->GetResult()->GetErrorCode());
        State = Finished;
        return;
	}
    
}
