#ifndef ACTIONSAVER_H
#define ACTIONSAVER_H

#include "idevtoolsaction.h"
#include "devtoolsactionfactory.h"

class ActionSaver
{
	DevToolsActionFactory ActionFactory;
	std::vector< std::shared_ptr<IDevToolsAction> > Actions;
	public:
		std::shared_ptr<IDevToolsAction> CloneAction(std::shared_ptr<IDevToolsAction> OriginalAction);
		void ProcessAction(std::shared_ptr<IDevToolsAction> Action);
        std::vector<std::shared_ptr<IDevToolsAction> > GetActions(bool IsFrame);

};

#endif // ACTIONSAVER_H
