#include "DevToolsActionStartScreenCast.h"

void DevToolsActionStartScreenCast::Run()
{
    State = Running;

    IsStoppingPreviousScreencast = !GlobalState->ScreenCastTabId.empty();

    if(IsStoppingPreviousScreencast && FindTabById(GlobalState->ScreenCastTabId))
    {
        std::map<std::string, Variant> CurrentParams;
        SendWebSocket("Page.stopScreencast", CurrentParams, GlobalState->ScreenCastTabId);
    } else
    {
        std::map<std::string, Variant> CurrentParams;
        CurrentParams["format"] = Variant(std::string("ipc"));
        SendWebSocket("Page.startScreencast", CurrentParams);
    }
}

bool DevToolsActionStartScreenCast::FilterActionSaver(std::shared_ptr<IDevToolsAction> Action)
{
    return Action->GetTypeName() != "StartScreencast" && Action->GetTypeName() != "StopScreencast";
}

IDevToolsAction::ActionSaverBehavior DevToolsActionStartScreenCast::GetActionSaverBehavior()
{
    return IDevToolsAction::SaveAndCustomFilterFunction;
}

bool DevToolsActionStartScreenCast::IsNeedToRunForAllActiveTabs()
{
    return false;
}

void DevToolsActionStartScreenCast::OnTabCreation()
{
    Run();
}

void DevToolsActionStartScreenCast::OnTabSwitching()
{
    Run();
}

void DevToolsActionStartScreenCast::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    if(IsStoppingPreviousScreencast)
    {
        IsStoppingPreviousScreencast = false;
        GlobalState->ScreenCastTabId.clear();
        std::map<std::string, Variant> CurrentParams;
        CurrentParams["format"] = Variant(std::string("ipc"));
        SendWebSocket("Page.startScreencast", CurrentParams);
    } else
    {
        GlobalState->ScreenCastTabId = GetActualTabId();
        Result->Success();
        State = Finished;
    }
}
