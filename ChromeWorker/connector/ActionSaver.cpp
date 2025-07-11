#include "ActionSaver.h"


std::vector<std::shared_ptr<IDevToolsAction> > ActionSaver::GetActions(bool IsFrame)
{
	std::vector<std::shared_ptr<IDevToolsAction> > Result;
	for(std::shared_ptr<IDevToolsAction> Action : Actions)
	{
        bool NeedToAdd = false;
        if(IsFrame)
        {
            if(Action->IsNeedToRunForAllActiveFrames())
            {
                NeedToAdd = true;
            }
        }else
        {
            NeedToAdd = true;
        }

        if(NeedToAdd)
        {
            Result.push_back(CloneAction(Action));
        }
	}
	return Result;
}

void ActionSaver::ProcessAction(std::shared_ptr<IDevToolsAction> Action)
{
	IDevToolsAction::ActionSaverBehavior SaverBehavior = Action->GetActionSaverBehavior();

	if(SaverBehavior == IDevToolsAction::DontSave)
	{
		return;
	} else if(SaverBehavior == IDevToolsAction::SaveAccumulateSameType)
	{
		Actions.push_back(CloneAction(Action));
	} else if(SaverBehavior == IDevToolsAction::SaveClearSameType)
	{
		for(auto it = Actions.begin(); it != Actions.end(); )
		{
			if((*it)->GetTypeName() == Action->GetTypeName())
			{
				it = Actions.erase(it);
			} else
			{
				++it;
			}
		}
		Actions.push_back(CloneAction(Action));

	} else if(SaverBehavior == IDevToolsAction::SaveAndCustomFilterFunction || SaverBehavior == IDevToolsAction::DontSaveAndCustomFilterFunction)
	{
		for(auto it = Actions.begin(); it != Actions.end(); )
		{
			if(Action->FilterActionSaver(*it))
			{
				++it; 
			} else
			{
				it = Actions.erase(it);
			}
		}
		if(SaverBehavior == IDevToolsAction::SaveAndCustomFilterFunction)
			Actions.push_back(CloneAction(Action));
	}

}


std::shared_ptr<IDevToolsAction> ActionSaver::CloneAction(std::shared_ptr<IDevToolsAction> OriginalAction)
{
	IDevToolsAction* ResultAction = ActionFactory.Create(OriginalAction->GetTypeName(), OriginalAction->GetGlobalState());
	OriginalAction->CloneTo(ResultAction);
	std::shared_ptr<IDevToolsAction> Result(ResultAction);
	return Result;
}
