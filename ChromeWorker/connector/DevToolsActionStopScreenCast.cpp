#include "DevToolsActionStopScreenCast.h"

void DevToolsActionStopScreenCast::Run()
{
    State = Running;

    if(!GlobalState->ScreenCastTabId.empty() && FindTabById(GlobalState->ScreenCastTabId))
    {
        std::map<std::string, Variant> CurrentParams;
        SendWebSocket("Page.stopScreencast", CurrentParams, GlobalState->ScreenCastTabId);
    } else
    {
        Result->Success();
        State = Finished;
    }
}

bool DevToolsActionStopScreenCast::FilterActionSaver(std::shared_ptr<IDevToolsAction> Action)
{
    return Action->GetTypeName() != "StartScreencast" && Action->GetTypeName() != "StopScreencast";
}

IDevToolsAction::ActionSaverBehavior DevToolsActionStopScreenCast::GetActionSaverBehavior()
{
    return IDevToolsAction::DontSaveAndCustomFilterFunction;
}

void DevToolsActionStopScreenCast::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    GlobalState->ScreenCastTabId.clear();
    Result->Success();
    State = Finished;
}