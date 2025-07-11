#include "devtoolsactionexecutejavascript.h"
#include "processjavascript.h"
#include <regex>

void DevToolsActionExecuteJavascript::Run()
{
    State = Running;

    ElementSelector.clear();
    Expression.clear();
    Variables.clear();
    InitialVariables.clear();
    LastMessage.clear();
    CurrentLoaderId.clear();
    CurrentPrefix.clear();
    CurrentContextId.clear();
    CurrentFrameSessionId = "CurrentTab";
    NextContextId.clear();
    NextFrameSessionId = "CurrentTab";
    ScrollDataWasObtained = false;
    IsDoingScrollRequest = false;
    IsDoingScroll = false;
    UsesScrollData = false;
    UsesPositionData = false;
    DoScroll = false;
    PositionX = 0;
    PositionY = 0;

    ElementSelector = ParseSelector(Params["path"].String);

    Expression = Params["expression"].String;

    Variables = Params["variables"].String;

    DoScroll = Params["do_scroll"].Boolean;
    //Don't repeat scroll on next try
    Params["do_scroll"].Boolean = false;

    UsesPositionData = Expression.find("positionx") != std::string::npos || Expression.find("positiony") != std::string::npos;
    UsesScrollData = Expression.find("scrollx") != std::string::npos || Expression.find("scrolly") != std::string::npos;


    picojson::object VariablesList;

    std::regex pieces_regex("\\[\\[([A-Z0-9_]+)\\]\\]");

    while(true)
    {
        std::smatch pieces_match;

        if(!std::regex_search(Expression.cbegin(), Expression.cend(), pieces_match, pieces_regex))
            break;

        std::ssub_match sub_match = pieces_match[1];
        std::string piece = sub_match.str();

        Expression.replace(pieces_match.position(), pieces_match.length(), std::string("_BAS_VARIABLES[\"") + piece + std::string("\"]"));


        picojson::value VariableValue;
        bool VariableFound;
        Parser.GetValueFromJson(Variables, piece, VariableValue, VariableFound);

        if(VariableFound)
        {
            VariablesList[piece] = VariableValue;
        } else
        {
            VariablesList[piece] = picojson::value();
        }

    }

    InitialVariables = picojson::value(VariablesList).serialize();

    Result->SetResult(InitialVariables);

    SubscribbedEvents.push_back("Target.detachedFromTarget");

    LastMessage.clear();
    Next();

}

bool DevToolsActionExecuteJavascript::Evaluate(std::map<std::string, Variant>& Params)
{
    std::string TabId = GetDefaultTabId();
    bool TabExists = false;
    for(std::shared_ptr<TabData> Tab: GlobalState->Tabs)
    {
        if(Tab->TabId == TabId)
        {
            TabExists = true;
            break;
        }
    }

    if(!TabExists)
    {
        //In case if tab is destroyed, Runtime.evaluate request will never return.
        //Therefore timeout will occur, in order to avoid that, error will be returned immediately.
        Result->Fail("Tab is destroyed", "TabDestroyed");
        State = Finished;
        return true;
    }

    CurrentExecutionTabId = TabId;

    //Using frame in separate process, check if it still exsist
    if(CurrentFrameSessionId != "CurrentTab")
    {
        bool FrameExists = false;
        for(std::shared_ptr<TabData> Frame: GlobalState->Frames)
        {
            if(Frame->TabId == CurrentFrameSessionId)
            {
                FrameExists = true;
                break;
            }
        }

        if(!FrameExists)
        {
            //In case if frame is destroyed, Runtime.evaluate request will never return.
            //Therefore timeout will occur, in order to avoid that, error will be returned immediately.
            Result->Fail("Frame is destroyed", "FrameDestroyed");
            State = Finished;
            return true;
        }

        CurrentExecutionTabId = CurrentFrameSessionId;

    }

    SendWebSocket("Runtime.evaluate", Params, CurrentFrameSessionId);
    return false;
}

std::vector<std::pair<std::string, std::string> > DevToolsActionExecuteJavascript::ParseSelector(const std::string& SelectorString)
{
    picojson::value AllValue;
    picojson::parse(AllValue, SelectorString);
    picojson::array AllList = AllValue.get<picojson::array>();

    std::vector<std::pair<std::string, std::string> > Result;

    std::pair<std::string, std::string> Pair;

    bool IsFirst = true;

    for(picojson::value& Value: AllList)
    {
        std::string ValueString = Value.get<std::string>();
        if(IsFirst)
        {
            Pair.first = ValueString;
            IsFirst = false;
        }else
        {
            Pair.second = ValueString;
            Result.push_back(Pair);
            IsFirst = true;
        }
    }

    return Result;
}

std::string DevToolsActionExecuteJavascript::SerializeSelector(const std::vector<std::pair<std::string, std::string> >& SelectorData)
{
    picojson::array res;
    for(std::pair<std::string,std::string> p:SelectorData)
    {
        res.push_back(picojson::value(p.first));
        res.push_back(picojson::value(p.second));
    }

    return picojson::value(picojson::value(res).serialize()).serialize();
}

std::string DevToolsActionExecuteJavascript::Javascript(const std::string& Script)
{
    return ProcessJavaScript(Script, GlobalState->UniqueProcessId);
}

void DevToolsActionExecuteJavascript::Next()
{
    if(RequestType == JavascriptExecution)
    {
        if(GetStringFromJson(LastMessage, "result.subtype") == "error")
        {
            std::string ErrorString = GetStringFromJson(LastMessage, "result.description");
            picojson::object Object;
            Object["is_success"] = picojson::value(false);
            Object["error"] = picojson::value(ErrorString);
            Object["variables"] = picojson::value(InitialVariables);
            Result->SetRawData(picojson::value(Object).serialize());
            Result->Fail(ErrorString, "JsError");
            State = Finished;
            return;
        }else if(GetStringFromJson(LastMessage,"result.value","BAS_NOT_FOUND") != "BAS_NOT_FOUND")
        {
            LastMessage = GetStringFromJson(LastMessage, "result.value");
            Result->SetRawData(LastMessage);
            if(GetStringFromJson(LastMessage, "variables", "BAS_NOT_FOUND") != "BAS_NOT_FOUND")
            {
                if(GetBooleanFromJson(LastMessage, "is_success"))
                {
                    Result->Success(GetStringFromJson(LastMessage, "variables"));
                } else
                {
                    Result->Fail(GetStringFromJson(LastMessage, "error"), "JsError", GetStringFromJson(LastMessage, "variables"));
                }
                State = Finished;
                return;
            }
        }

        Result->Fail("Unknown response", "UnknownResponse");
        State = Finished;
        return;

    }

    if(!LastMessage.empty())
    {
        if(RequestType == FrameSearchEvaluate)
        {
            CurrentLoaderId = GetStringFromJson(LastMessage, "result.objectId", "BAS_NOT_FOUND");
            if(CurrentLoaderId == "BAS_NOT_FOUND")
            {
                CurrentLoaderId.clear();
                Result->Fail("Failed to find frame", "NoFrame");
                State = Finished;
                return;
            }else
            {
                std::map<std::string, Variant> CurrentParams;
                CurrentParams["objectId"] = Variant(CurrentLoaderId);
                RequestType = FrameSearchGetNodeId;
                SendWebSocket("DOM.describeNode", CurrentParams, CurrentFrameSessionId);
            }
            return;
        } else if(RequestType == FrameSearchGetNodeId)
        {
            //Get information about frame

            std::string CurrentFrame = GetStringFromJson(LastMessage, "node.frameId");
            /*WORKER_LOG(std::string("!!!!!!!!!!!!!!!!! SEARCHING FOR frame id = ") + CurrentFrame);

            for(std::shared_ptr<TabData> Frame: GlobalState->Frames)
            {
                WORKER_LOG(std::string("!!!!!!!!!!!!!!!!! INFO Frames, frame id = ") + Frame->FrameId + std::string(", session id = ") + Frame->TabId);
            }

            for(std::shared_ptr<ExecutionContextData> ExecutionContext: GlobalState->ExecutionContexts)
            {
                WORKER_LOG(std::string("!!!!!!!!!!!!!!!!! INFO ExecutionContexts, frame id = ") + ExecutionContext->FrameId + std::string(", session id = ") + ExecutionContext->TabId + std::string(", context id = ") + ExecutionContext->ContextId);
            }*/

            NextFrameSessionId = CurrentFrameSessionId;
            for(std::shared_ptr<TabData> Frame: GlobalState->Frames)
            {
                if(Frame->FrameId == CurrentFrame)
                {
                    NextFrameSessionId = Frame->TabId;
                }
            }

            std::string ResolvedTabId = NextFrameSessionId;

            if(ResolvedTabId == "CurrentTab")
            {
                ResolvedTabId = GetDefaultTabId();
            }

            //WORKER_LOG(std::string("!!!!!!!!!!!!!!!!! RESULT session id = ") + ResolvedTabId);

            NextContextId.clear();

            for(std::shared_ptr<ExecutionContextData> ExecutionContext: GlobalState->ExecutionContexts)
            {
                if(ExecutionContext->FrameId == CurrentFrame && ExecutionContext->TabId == ResolvedTabId)
                {
                    NextContextId = ExecutionContext->ContextId;
                }
            }

            //WORKER_LOG(std::string("!!!!!!!!!!!!!!!!! RESULT context id = ") + NextContextId);

            RequestType = FrameSearchReleaseObject;
            std::map<std::string, Variant> CurrentParams;
            CurrentParams["objectId"] = Variant(CurrentLoaderId);
            SendWebSocket("Runtime.releaseObject", CurrentParams, CurrentFrameSessionId);
            return;
        } else if(RequestType == FrameSearchReleaseObject)
        {
            if(UsesPositionData)
            {
                RequestType = FrameSearchGetPosition;
                std::map<std::string, Variant> CurrentParams;
                std::string Script = std::string("{_BAS_SAFE(JSON.stringify)(_BAS_HIDE(BrowserAutomationStudio_GetInternalBoundingRect)(_BAS_HIDE(BrowserAutomationStudio_FindElement)(") + SerializeSelector(CurrentPrefix) + std::string(")));}");
                CurrentParams["expression"] = Variant(Javascript(Script));
                if(!CurrentContextId.empty())
                    CurrentParams["uniqueContextId"] = Variant(CurrentContextId);

                Evaluate(CurrentParams);
                return;
            } else
            {
                RequestType = NodeSearch;
                CurrentContextId = NextContextId;
                CurrentFrameSessionId = NextFrameSessionId;
                Next();
                return;
            }
        } else if(RequestType == FrameSearchGetPosition)
        {
            std::string ResponseMessage = GetStringFromJson(LastMessage, "result.value", "BAS_NOT_FOUND");
            if(ResponseMessage == "BAS_NOT_FOUND")
            {
                Result->Fail("Failed to find frame position", "NoFramePosition");
                State = Finished;
                return;
            } else
            {
                PositionX += GetFloatFromJson(ResponseMessage, "left");
                PositionY += GetFloatFromJson(ResponseMessage, "top");
                RequestType = NodeSearch;
                CurrentContextId = NextContextId;
                CurrentFrameSessionId = NextFrameSessionId;
                Next();
            }
            return;
        }
    }
    int Index = -1;
    for(int i = 0;i<ElementSelector.size();i++)
    {
        if(ElementSelector.at(i).first == "frame_element")
        {
            Index = i;
            break;
        }
    }
    if(Index >= 0)
    {
        CurrentPrefix.assign(ElementSelector.begin(),ElementSelector.begin() + Index + 1);
        ElementSelector.erase(ElementSelector.begin(), ElementSelector.begin() + Index + 1);
        RequestType = FrameSearchEvaluate;
        std::map<std::string, Variant> CurrentParams;
        std::string Script = std::string("{_BAS_HIDE(BrowserAutomationStudio_FindElement)(") + SerializeSelector(CurrentPrefix) + std::string(");}");
        CurrentParams["expression"] = Variant(Javascript(Script));
        if(!CurrentContextId.empty())
            CurrentParams["uniqueContextId"] = Variant(CurrentContextId);
        
        Evaluate(CurrentParams);
        return;
    }

    if(DoScroll)
    {
        DoScroll = false;
        IsDoingScroll = true;
        std::map<std::string, Variant> CurrentParams;
        std::string Script = std::string("(function(){var self = null; try{ self = _BAS_HIDE(BrowserAutomationStudio_FindElement)(") + SerializeSelector(ElementSelector) + std::string(");}catch(e){};if(self)_BAS_SAFE($Element.scrollIntoViewIfNeeded)(self, true);})();");
        CurrentParams["expression"] = Variant(Javascript(Script));
        if(!CurrentContextId.empty())
            CurrentParams["uniqueContextId"] = Variant(CurrentContextId);

        Evaluate(CurrentParams);
        return;
    }

    if(IsDoingScroll && UsesPositionData)
    {
        //Restart, need to update positionx and positiony after scroll
        Run();
        return;
    }



    if(IsDoingScrollRequest)
    {
        ScrollDataWasObtained = true;
        GlobalState->ScrollX = GetFloatFromJson(LastMessage, "visualViewport.pageX");
        GlobalState->ScrollY = GetFloatFromJson(LastMessage, "visualViewport.pageY");
    }

    if(UsesScrollData && !ScrollDataWasObtained)
    {
        IsDoingScrollRequest = true;
        std::map<std::string, Variant> CurrentParams;
        SendWebSocket("Page.getLayoutMetrics", CurrentParams);
        return;
    }

    std::map<std::string, Variant> CurrentParams;

    if(!CurrentContextId.empty())
        CurrentParams["uniqueContextId"] = Variant(CurrentContextId);
    RequestType = JavascriptExecution;
    std::string Script;

    Script += std::string("await (async function(){\n");
    
    if (!ElementSelector.empty())
    {
        Script += Javascript(std::string("var self = null; try{ self = _BAS_HIDE(BrowserAutomationStudio_FindElement)(") + SerializeSelector(ElementSelector) + std::string(");}catch(e){};"));
    }else
    {
        Script += Javascript(std::string("var self = _BAS_SAFE($Document.body)(document);"));
    }
    
    Script += std::string("var _BAS_VARIABLES = ") + InitialVariables;

    Script += ";\n";

    if(UsesScrollData)
    {
        Script += std::string("var scrollx = ") + std::to_string(GlobalState->ScrollX);
        Script += ";\n";
        Script += std::string("var scrolly = ") + std::to_string(GlobalState->ScrollY);
        Script += ";\n";
    }

    if(UsesPositionData)
    {
        Script += std::string("var positionx = ") + std::to_string(PositionX);
        Script += ";\n";
        Script += std::string("var positiony = ") + std::to_string(PositionY);
        Script += ";\n";
    }

    Script += std::string("var _BAS_RESULT = {is_success: true, error: \"\", variables: _BAS_VARIABLES}");

    Script += ";\n";

    Script += "return await (async function(){";

    Script += "\n";

    Script += "try{";
    
    Script += "\n";

    Script += Javascript(Expression);

    Script += Javascript("}catch(e){_BAS_RESULT.error = _BAS_HIDE(BrowserAutomationStudio_ToString)(e);_BAS_RESULT.is_success = false;}");

    Script += "\n";

    Script += Javascript("_BAS_RESULT.variables = _BAS_SAFE(JSON.stringify)(_BAS_RESULT.variables);");

    Script += "\n";

    Script += Javascript("return _BAS_SAFE(JSON.stringify)(_BAS_RESULT)})();");

    Script += "\n";

    Script += std::string("})();");

    CurrentParams["expression"] = Variant(Script);
    CurrentParams["replMode"] = Variant(true);
    
    Evaluate(CurrentParams);
}

void DevToolsActionExecuteJavascript::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    CurrentExecutionTabId.clear();
    LastMessage = Message;
    Next();
}

void DevToolsActionExecuteJavascript::OnWebSocketEvent(const std::string& Method, const std::string& Message)
{
    if(Method == "Target.detachedFromTarget" && !CurrentExecutionTabId.empty() && CurrentExecutionTabId == GetStringFromJson(Message, "sessionId"))
    {
        //In case if tab is destroyed after Runtime.evaluate is sent and no answer has been returned yet, return error.
        Result->Fail("Tab is destroyed", "TabDestroyed");
        State = Finished;
    }
}
