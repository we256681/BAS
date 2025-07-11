#include "devtoolsactionwebsocketquery.h"

void DevToolsActionWebsocketQuery::Run()
{
    State = Running;

    if(ReturnTypeString == "String")
    {
        ReturnType = String;
    }

    if(ReturnTypeString == "Bool")
        ReturnType = Bool;

    if(ReturnTypeString == "Number")
        ReturnType = Number;

    if(ReturnTypeString == "None")
        ReturnType = None;

    SendWebSocket(WebSocketMethod,Params);
}

void DevToolsActionWebsocketQuery::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    if(ReturnType == String)
    {
        Result->Success(GetStringFromJson(Message, ReturnPath));
    }else if(ReturnType == Bool)
    {
        Result->Success(GetBooleanFromJson(Message, ReturnPath));
    }else if(ReturnType == Number)
    {
        Result->Success((int)GetFloatFromJson(Message, ReturnPath));
    }else if(ReturnType == None)
    {
        Result->Success();
    }

    State = Finished;
}
