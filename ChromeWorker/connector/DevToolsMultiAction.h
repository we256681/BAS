#ifndef DEVTOOLSMULTIACTION_H
#define DEVTOOLSMULTIACTION_H

#include "idevtoolsaction.h"
#include "devtoolsactionfactory.h"

class DevToolsMultiAction : public IDevToolsAction
{
	protected:
		DevToolsActionFactory ActionFactory;
		std::shared_ptr<IDevToolsAction> CurrentAction;
		void RunAction(const std::string Name, const std::map<std::string, Variant>& Params = std::map<std::string, Variant>());
		void RunJavascriptAction(const std::string& Script, const std::string& Variables = std::string(), const std::string& ElementPath = std::string());
		void CheckCurrentAction();
	public:

		virtual std::vector<std::string> GetSubscribbedEvents();
		virtual void SetOverrideDefaultTabId(const std::string& OverrideDefaultTabId);

		virtual void OnActionFinished() = 0;

		virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
		virtual void OnWebSocketEvent(const std::string& Method, const std::string& Message);

};

#endif // DEVTOOLSMULTIACTION_H