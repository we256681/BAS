#include "DevToolsActionSetHeaders.h"
#include "base64.h"

void DevToolsActionSetHeaders::Run()
{
    if(Params["headers"].Map.size() == 0)
    {
        Result->Success();
        State = Finished;
        return;
    }
    State = Running;
    SendWebSocket("Network.setExtraHTTPHeaders", Params);
}

IDevToolsAction::ActionSaverBehavior DevToolsActionSetHeaders::GetActionSaverBehavior()
{
    return IDevToolsAction::SaveClearSameType;
}

bool DevToolsActionSetHeaders::IsNeedToRunForAllActiveTabs()
{
    return true;
}

bool DevToolsActionSetHeaders::IsNeedToRunForAllActiveFrames()
{
    return true;
}

void DevToolsActionSetHeaders::OnTabSwitching()
{

}

void DevToolsActionSetHeaders::OnTabCreation()
{
    Run();
}

void DevToolsActionSetHeaders::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    Result->Success();
    State = Finished;
}
