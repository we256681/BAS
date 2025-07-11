#include "DevToolsActionCloseTab.h"

void DevToolsActionCloseTab::Run()
{
    State = Running;

    int Index = Params["index"].Number;
    Params.erase("index");

    int CurrentIndex = 0;
    std::shared_ptr<TabData> CurrentTab;
    for(auto it = GlobalState->Tabs.begin(); it != GlobalState->Tabs.end(); ++it)
    {
        std::shared_ptr<TabData> Tab = *it;
        if(Tab->ConnectionState == TabData::Connected || Tab->ConnectionState == TabData::Delayed)
        {
            if(CurrentIndex == Index)
            {

                //Delayed tab is virtual, so it can be erased without any issue.
                if(Tab->ConnectionState == TabData::Delayed)
                {
                    GlobalState->Tabs.erase(it);
                    Result->Success();
                    State = Finished;
                    return;
                }

                CurrentTab = Tab;
                break;
            }
            CurrentIndex++;
        }
    }

    if(!CurrentTab)
    {
        Result->Fail(std::string("Can't find tab with specified index"));
        State = Finished;
        return;
    }

    if(GlobalState->Tabs.size() <= 1 || Index == 0)
    {
        Result->Fail(std::string("Can't close last tab"));
        State = Finished;
        return;
    }

    IsCurrentTabClosing = CurrentTab->TabId == GlobalState->TabId;

    if(IsCurrentTabClosing)
        SubscribbedEvents.push_back("Target.targetActivated");


    std::map<std::string, Variant> CurrentParams;
    CurrentParams["targetId"] = Variant(CurrentTab->FrameId);
    SendWebSocket("Target.closeTarget", CurrentParams);
}

void DevToolsActionCloseTab::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    if(!IsCurrentTabClosing)
    {
        Result->Success();
        State = Finished;
    }
}

void DevToolsActionCloseTab::OnWebSocketEvent(const std::string& Method, const std::string& Message)
{
    std::string TargetIdCandidate = GetStringFromJson(Message, "targetInfo.targetId");
    Result->Success();
    State = Finished;
}
