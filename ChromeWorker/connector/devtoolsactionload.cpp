#include "devtoolsactionload.h"
#include "PrepareUrl.h"

void DevToolsActionLoad::Run()
{
    Params["url"].String = PrepareUrl(Params["url"].String);

    if(Params["instant"].Boolean)
    {
        State = Finished;
        Result->Success();
    }else
    {
        State = Running;
    }
    Params.erase("instant");
    SendWebSocket("Page.navigate",Params);
}

void DevToolsActionLoad::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    std::string ErrorText = GetStringFromJson(Message, "errorText", "BAS_NO_VALUE");
    if(ErrorText == "BAS_NO_VALUE")
        ErrorText = GetStringFromJson(Error, "message", "BAS_NO_VALUE");
    if(ErrorText == "BAS_NO_VALUE")
    {
        FrameId = GetStringFromJson(Message, "frameId");
        SubscribbedEvents.push_back("Page.frameStoppedLoading");
        return;
    }

    Result->Fail(ErrorText);
    State = Finished;
}

void DevToolsActionLoad::OnWebSocketEvent(const std::string& Method,const std::string& Message)
{
    std::string CurrentLoaderId = GetStringFromJson(Message, "frameId");
    if(FrameId == CurrentLoaderId)
    {
        Result->Success();
        State = Finished;
    }
}
