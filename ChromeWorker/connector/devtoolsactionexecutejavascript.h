#ifndef DEVTOOLSACTIONEXECUTEJAVASCRIPT_H
#define DEVTOOLSACTIONEXECUTEJAVASCRIPT_H

#include "idevtoolsaction.h"
class DevToolsActionExecuteJavascript :	public IDevToolsAction
{
    std::vector<std::pair<std::string, std::string> > ElementSelector;
    std::string Expression;
    std::string Variables;
    std::string InitialVariables;
    std::string LastMessage;
    std::string CurrentLoaderId;
    std::vector<std::pair<std::string, std::string> > CurrentPrefix;
    std::string CurrentContextId;
    std::string CurrentFrameSessionId = "CurrentTab";
    std::string NextContextId;
    std::string NextFrameSessionId = "CurrentTab";
    bool ScrollDataWasObtained = false;
    bool IsDoingScrollRequest = false;
    bool IsDoingScroll = false;
    bool UsesScrollData = false;
    bool UsesPositionData = false;
    bool DoScroll = false;
    int PositionX = 0;
    int PositionY = 0;
    std::string CurrentExecutionTabId;
    enum {
        NodeSearch,
        FrameSearchEvaluate,
        FrameSearchGetNodeId,
        FrameSearchReleaseObject,
        FrameSearchGetPosition,
        JavascriptExecution
    }RequestType;
    void Next();
    std::vector<std::pair<std::string, std::string> > ParseSelector(const std::string& SelectorString);
    std::string SerializeSelector(const std::vector<std::pair<std::string, std::string> >& SelectorData);
    std::string Javascript(const std::string& Script);
    bool Evaluate(std::map<std::string, Variant>& Params);
public:
    virtual void Run();
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
    virtual void OnWebSocketEvent(const std::string& Method, const std::string& Message);

};

#endif // DEVTOOLSACTIONEXECUTEJAVASCRIPT_H
