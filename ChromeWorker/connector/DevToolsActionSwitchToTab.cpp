#include "DevToolsActionSwitchToTab.h"

void DevToolsActionSwitchToTab::Run()
{
    State = Running;

    int Index = Params["index"].Number;
    Params.erase("index");

    int CurrentIndex = 0;
    std::shared_ptr<TabData> CurrentTab;
    for(std::shared_ptr<TabData> Tab : GlobalState->Tabs)
    {
        if(Tab->ConnectionState == TabData::Connected || Tab->ConnectionState == TabData::Delayed)
        {
            if(CurrentIndex == Index)
            {
                CurrentTab = Tab;
                break;
            }
            CurrentIndex++;
        }
    }

    if(!CurrentTab)
    {
        Result->Fail("Can't find tab with specified index");
        State = Finished;
        return;
    }

    SwitchToTabId = CurrentTab->TabId;
    SwitchToTabFrameId = CurrentTab->FrameId;

    SubscribbedEvents.push_back("Target.targetActivated");

    std::map<std::string, Variant> CurrentParams;
    CurrentParams["targetId"] = Variant(CurrentTab->FrameId);
    SendWebSocket("Target.activateTarget", CurrentParams);
}

void DevToolsActionSwitchToTab::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    GlobalState->SwitchToTabId = SwitchToTabId;
    GlobalState->SwitchToTabFrameId = SwitchToTabFrameId;
    GlobalState->SwitchToTabResetSavedActions = true;
}

void DevToolsActionSwitchToTab::OnWebSocketEvent(const std::string& Method, const std::string& Message)
{
    if(Method == "Target.targetActivated")
    {
        std::string TargetIdCandidate = GetStringFromJson(Message, "targetInfo.targetId");
        if(TargetIdCandidate == SwitchToTabFrameId)
        {
            Result->Success();
            State = Finished;
        }
    }

    
}
