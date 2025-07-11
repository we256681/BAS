#include "DevToolsMultiAction.h"

void DevToolsMultiAction::RunAction(const std::string Name, const std::map<std::string, Variant>& Params)
{
	CurrentAction.reset(ActionFactory.Create(Name, GlobalState));
	CurrentAction->SetParams(Params);
	CurrentAction->SetId(Id);
	CurrentAction->SetTimeout(-1);
	CurrentAction->SetOverrideDefaultTabId(OverrideDefaultTabId);

	CurrentAction->Run();
	CheckCurrentAction();
}

void DevToolsMultiAction::CheckCurrentAction()
{
	if(!CurrentAction)
		return;

	if(CurrentAction->GetState() == IDevToolsAction::Finished)
	{
		OnActionFinished();
	}
}

void DevToolsMultiAction::RunJavascriptAction(const std::string& Script, const std::string& Variables, const std::string& ElementPath)
{
	std::map<std::string, Variant> Params;

	Params["expression"] = Variant(Script);
	Params["path"] = Variant(ElementPath);
	Params["variables"] = Variant(Variables);

	RunAction("ExecuteJavascript", Params);
}

std::vector<std::string> DevToolsMultiAction::GetSubscribbedEvents()
{
	if(!CurrentAction)
        return SubscribbedEvents;

	return CurrentAction->GetSubscribbedEvents();
}

void DevToolsMultiAction::SetOverrideDefaultTabId(const std::string& OverrideDefaultTabId)
{
	this->OverrideDefaultTabId = OverrideDefaultTabId;

	if(CurrentAction)
		CurrentAction->SetOverrideDefaultTabId(OverrideDefaultTabId);
}



void DevToolsMultiAction::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
	if(!CurrentAction)
		return;
	CurrentAction->OnWebSocketMessage(Message, Error);
	CheckCurrentAction();
}


void DevToolsMultiAction::OnWebSocketEvent(const std::string& Method, const std::string& Message)
{
	if(!CurrentAction)
		return;
	CurrentAction->OnWebSocketEvent(Method, Message);
	CheckCurrentAction();
}
