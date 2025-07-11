#include "DevToolsActionNavigateBack.h"

void DevToolsActionNavigateBack::Run()
{
    State = Running;

    IsInstant = Params["instant"].Boolean;

    IsGettingHistoryEntries = true;

    std::map<std::string, Variant> CurrentParams;
    SendWebSocket("Page.getNavigationHistory", CurrentParams);
}

void DevToolsActionNavigateBack::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    if(IsGettingHistoryEntries)
    {
        IsGettingHistoryEntries = false;

        picojson::value AllValue;
        picojson::parse(AllValue, Message);
        picojson::array AllEntries = AllValue.get<picojson::object>()["entries"].get<picojson::array>();
        int CurrentIndex = AllValue.get<picojson::object>()["currentIndex"].get<double>();

        if(CurrentIndex <= 0)
        {
            Result->Success();
            State = Finished;
            return;
        }

        EntryId = AllEntries[CurrentIndex - 1].get<picojson::object>()["id"].get<double>();

        if(IsInstant)
        {
            State = Finished;
            Result->Success();
            std::map<std::string, Variant> CurrentParams;
            CurrentParams["entryId"] = Variant(EntryId);
            SendWebSocket("Page.navigateToHistoryEntry", CurrentParams);
        } else
        {
            IsGettingFrameTree = true;
            std::map<std::string, Variant> CurrentParams;
            SendWebSocket("Page.getFrameTree", CurrentParams);
        }

        
    }else if(IsGettingFrameTree)
    {
        IsGettingFrameTree = false;

        FrameId = GetStringFromJson(Message, "frameTree.frame.id");
        SubscribbedEvents.push_back("Page.frameStoppedLoading");

        std::map<std::string, Variant> CurrentParams;
        CurrentParams["entryId"] = Variant(EntryId);
        SendWebSocket("Page.navigateToHistoryEntry", CurrentParams);
        
    } else
    {
        std::string ErrorText = GetStringFromJson(Message, "errorText", "BAS_NO_VALUE");
        if(ErrorText == "BAS_NO_VALUE")
        {
            return;
        }


        Result->Fail(ErrorText);
        State = Finished;
    }

}

void DevToolsActionNavigateBack::OnWebSocketEvent(const std::string& Method, const std::string& Message)
{
    std::string CurrentLoaderId = GetStringFromJson(Message, "frameId");
    if(FrameId == CurrentLoaderId)
    {
        Result->Success();
        State = Finished;
    }
}
