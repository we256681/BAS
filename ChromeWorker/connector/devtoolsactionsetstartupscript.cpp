#include "devtoolsactionsetstartupscript.h"

void DevToolsActionSetStartupScript::Run()
{
    State = Running;

    if(!RemoveNextScript())
    {
        IsWaitingForDisablePreviousScript = false;
        SendWebSocket("Page.addScriptToEvaluateOnNewDocument", Params);
    }
}

bool DevToolsActionSetStartupScript::RemoveNextScript()
{
    bool IsSomethingSent = false;
    if(ClearStartupScripts)
    {
        std::vector<StartupScriptItem>::iterator it = GlobalState->StartupScriptIds.begin();
        while(it != GlobalState->StartupScriptIds.end())
        {
            if(it->GroupId == GroupId)
            {
                ++it;
                continue;
            }

            if(FindTabById(it->TabId))
            {
                IsWaitingForDisablePreviousScript = true;
                std::map<std::string, Variant> ParamsDisablePreviousScript;
                ParamsDisablePreviousScript["identifier"] = it->ScriptId;
                SendWebSocket("Page.removeScriptToEvaluateOnNewDocument", ParamsDisablePreviousScript, it->TabId);
                IsSomethingSent = true;
                it = GlobalState->StartupScriptIds.erase(it);
                break;
            }

            it = GlobalState->StartupScriptIds.erase(it);
        }
    }
    return IsSomethingSent;
}

void DevToolsActionSetStartupScript::OnTabCreation()
{
    ClearStartupScripts = false;
    Run();
}

void DevToolsActionSetStartupScript::OnTabSwitching()
{

}

void DevToolsActionSetStartupScript::OnRunnningForAllRunActiveTab()
{
    ClearStartupScripts = false;
    Run();
}

IDevToolsAction::ActionSaverBehavior DevToolsActionSetStartupScript::GetActionSaverBehavior()
{
    return IDevToolsAction::SaveClearSameType;
}

bool DevToolsActionSetStartupScript::IsNeedToRunForAllActiveTabs()
{
    return true;
}

void DevToolsActionSetStartupScript::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    if(!IsWaitingForDisablePreviousScript)
    {
        StartupScriptItem Item;
        Item.ScriptId = GetStringFromJson(Message, "identifier");
        Item.TabId = GetActualTabId();
        Item.GroupId = GroupId;
        GlobalState->StartupScriptIds.push_back(Item);
        Result->Success();
        State = Finished;
        return;
    }

    if(!RemoveNextScript())
    {
        IsWaitingForDisablePreviousScript = false;
        SendWebSocket("Page.addScriptToEvaluateOnNewDocument", Params);
    }
}
