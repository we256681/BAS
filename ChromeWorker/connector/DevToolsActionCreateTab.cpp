#include "DevToolsActionCreateTab.h"
#include "PrepareUrl.h"

void DevToolsActionCreateTab::Run()
{
    if(!GlobalState->IsPopupsAllowed)
    {
        Result->Fail("Creating new tabs is prohibited");
        State = Finished;
        return;
    }
    State = Running;

    IsInstant = Params["instant"].Boolean;
    Params.erase("instant");

    Referrer = Params["referrer"].String;
    Params.erase("referrer");

    IsDelayed = Params["delayed"].Boolean;
    Params.erase("delayed");

    Url = PrepareUrl(Params["url"].String);
    Params["url"].String = "about:blank";

    if(IsDelayed)
    {
        std::shared_ptr<TabData> TabInfo = std::make_shared<TabData>();
        TabInfo->ConnectionState = TabData::Delayed;
        TabInfo->DelayedUrl = Url;
        GlobalState->Tabs.push_back(TabInfo);

        State = Finished;
        Result->Success();
        return;
    }

    SubscribbedEvents.push_back("Target.targetActivated");
    
    
    
    SendWebSocket("Target.createTarget", Params);
}

void DevToolsActionCreateTab::StartLoad()
{
    if(IsInstant)
    {
        State = Finished;
        Result->Success();
    }

    std::map<std::string, Variant> CurrentParams;

    CurrentParams["url"] = Variant(Url);
    if(!Referrer.empty())
    {
        CurrentParams["referrer"] = Variant(Referrer);
        CurrentParams["referrerPolicy"] = Variant(std::string("unsafeUrl"));
    }

    SendWebSocket("Page.navigate", CurrentParams);

}


void DevToolsActionCreateTab::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    if(!IsTargetCreated)
    {
        TargetId = GetStringFromJson(Message, "targetId");
        IsTargetCreated = true;
        if(IsTabConnected)
        {
            StartLoad();
            return;
        }
    } else
    {
        std::string ErrorText = GetStringFromJson(Message, "errorText", "BAS_NO_VALUE");
        if(ErrorText == "BAS_NO_VALUE")
            ErrorText = GetStringFromJson(Error, "message", "BAS_NO_VALUE");
        if(ErrorText == "BAS_NO_VALUE")
        {
            SubscribbedEvents.push_back("Page.frameStoppedLoading");
            return;
        }

        Result->Fail(ErrorText);
        State = Finished;
    }
}

void DevToolsActionCreateTab::OnWebSocketEvent(const std::string& Method, const std::string& Message)
{
    if(Method == "Target.targetActivated")
    {
        std::string TargetIdCandidate = GetStringFromJson(Message, "targetInfo.targetId");
        if(!IsTabConnected && TargetIdCandidate == TargetId)
        {
            IsTabConnected = true;
            if(IsTargetCreated)
            {
                StartLoad();
                return;
            }
        }
    }

    if(Method == "Page.frameStoppedLoading")
    {
        std::string CurrentLoaderId = GetStringFromJson(Message, "frameId");
        if(TargetId == CurrentLoaderId)
        {
            Result->Success();
            State = Finished;
        }
    }
}
