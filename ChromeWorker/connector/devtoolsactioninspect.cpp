#include "devtoolsactioninspect.h"
#include "processjavascript.h"
#include <regex>

void DevToolsActionInspect::Run()
{
    State = Running;

    X = Params["x"].Number;
    Params.erase("x");

    Y = Params["y"].Number;
    Params.erase("y");

    Position = Params["position"].Number;
    Params.erase("position");

    Next();
}

std::string DevToolsActionInspect::PrepareResult()
{
    std::map<std::string, Variant> ResultObject;
    ResultObject["x"] = Variant(x);
    ResultObject["y"] = Variant(y);
    ResultObject["mousex"] = Variant(mousex);
    ResultObject["mousey"] = Variant(mousey);
    ResultObject["width"] = Variant(width);
    ResultObject["height"] = Variant(height);
    ResultObject["label"] = Variant(label);
    ResultObject["css"] = Variant(css);
    ResultObject["css2"] = Variant(css2);
    ResultObject["css3"] = Variant(css3);
    ResultObject["xpath"] = Variant(xpath);
    ResultObject["match"] = Variant(match);
    ResultObject["active"] = Variant(active);
    ResultObject["position"] = Variant(Position);

    //Get info about element frame
    std::string ResolvedTabId = CurrentFrameSessionId;
    if(ResolvedTabId == "CurrentTab")
    {
        ResolvedTabId = GetDefaultTabId();
    }
    ResultObject["tab_id"] = Variant(ResolvedTabId);

    return Serializer.SerializeObjectToString(ResultObject);
}

std::string DevToolsActionInspect::Javascript(const std::string& Script)
{
    return ProcessJavaScript(Script, GlobalState->UniqueProcessId);
}

void DevToolsActionInspect::Next()
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
        //Save previous session id to release object on parent frame
        std::string CurrentFrameSessionIdCopy = CurrentFrameSessionId;

        //Get information about frame
        std::string CurrentFrame = GetStringFromJson(LastMessage, "node.frameId");

        for(std::shared_ptr<TabData> Frame: GlobalState->Frames)
        {
            if(Frame->FrameId == CurrentFrame)
            {
                CurrentFrameSessionId = Frame->TabId;
            }
        }

        std::string ResolvedTabId = CurrentFrameSessionId;
        if(ResolvedTabId == "CurrentTab")
        {
            ResolvedTabId = GetDefaultTabId();
        }

        CurrentContextId.clear();
        for(std::shared_ptr<ExecutionContextData> ExecutionContext: GlobalState->ExecutionContexts)
        {
            if(ExecutionContext->FrameId == CurrentFrame && ExecutionContext->TabId == ResolvedTabId)
            {
                CurrentContextId = ExecutionContext->ContextId;
            }
        }

        RequestType = FrameSearchReleaseObject;
        std::map<std::string, Variant> CurrentParams;
        CurrentParams["objectId"] = Variant(CurrentLoaderId);
        SendWebSocket("Runtime.releaseObject", CurrentParams, CurrentFrameSessionIdCopy);
        return;
    } else if(RequestType == FrameSearchReleaseObject)
    {
        RequestType = Initial;
        Next();
        return;
    } else if(RequestType == InspectPosition)
    {
        std::string Value = GetStringFromJson(LastMessage, "result.value", "BAS_NOT_FOUND");

        picojson::value v;
        picojson::parse(v, Value);
        if(!v.is<picojson::value::array>())
        {
            Result->Fail("Failed to inspect element");
            State = Finished;
            return;
        }
        picojson::array ValueList = v.get<picojson::value::array>();
        if(ValueList.size() != 18)
        {
            Result->Fail("Failed to inspect element");
            State = Finished;
            return;
        }


        int current_x = ValueList[0].get<double>();
        int current_y = ValueList[1].get<double>();
        int current_width = ValueList[2].get<double>();
        int current_height = ValueList[3].get<double>();
        std::string current_label = ValueList[4].get<std::string>();
        std::string current_css = ValueList[5].get<std::string>();
        std::string current_css2 = ValueList[6].get<std::string>();
        std::string current_css3 = ValueList[7].get<std::string>();
        std::string current_match = ValueList[8].get<std::string>();
        std::string current_xpath = ValueList[9].get<std::string>();
        int current_mousex = ValueList[10].get<double>();
        int current_mousey = ValueList[11].get<double>();
        bool current_active = ValueList[12].get<bool>();
        bool current_is_frame = ValueList[13].get<bool>();
        int current_x_with_padding = ValueList[15].get<double>();
        int current_y_with_padding = ValueList[16].get<double>();


        if(mousex < 0 && mousey < 0)
        {
            mousex = current_mousex;
            mousey = current_mousey;
        }

        active = current_active;

        if(!current_is_frame)
        {
            x = current_x + x_with_padding;
            y = current_y + y_with_padding;
            width = current_width;
            height = current_height;
            label = label + current_label;
            css = css + current_css;
            css2 = css2 + current_css2;
            css3 = css3 + current_css3;
            match = match + current_match;
            xpath = xpath + current_xpath;
            Result->Success(PrepareResult());
            State = Finished;
            return;
        }else
        {


            label = label + current_label + std::string(">FRAME>");
            css = css + current_css + std::string(">FRAME>");
            css2 = css2 + current_css2 + std::string(">FRAME>");
            css3 = css3 + current_css3 + std::string(">FRAME>");
            match = match + current_match + std::string(">FRAME>");
            xpath = xpath + current_xpath + std::string(">FRAME>");

            std::map<std::string, Variant> CurrentParams;
            RequestType = FrameSearchEvaluate;

            if(!CurrentContextId.empty())
                CurrentParams["uniqueContextId"] = Variant(CurrentContextId);

            std::string Script;

            Script += Javascript(std::string("_BAS_HIDE(BrowserAutomationStudio_InspectElement)(") + std::to_string(X - x_with_padding) + std::string(",") + std::to_string(Y - y_with_padding) + std::string(",") + std::to_string(Position) + std::string(", true)[14]"));

            x_with_padding += current_x_with_padding;
            y_with_padding += current_y_with_padding;

            CurrentParams["expression"] = Variant(Script);
            CurrentParams["replMode"] = Variant(true);

            SendWebSocket("Runtime.evaluate", CurrentParams, CurrentFrameSessionId);
        }

    }else if(RequestType == Initial)
    {
        std::map<std::string, Variant> CurrentParams;
        RequestType = InspectPosition;

        if(!CurrentContextId.empty())
            CurrentParams["uniqueContextId"] = Variant(CurrentContextId);

        std::string Script;

        Script += Javascript(std::string("_BAS_SAFE(JSON.stringify)(_BAS_HIDE(BrowserAutomationStudio_InspectElement)(") + std::to_string(X - x_with_padding) + std::string(",") + std::to_string(Y - y_with_padding) + std::string(",") + std::to_string(Position) + std::string(", false))"));

        CurrentParams["expression"] = Variant(Script);
        CurrentParams["replMode"] = Variant(true);

        SendWebSocket("Runtime.evaluate", CurrentParams, CurrentFrameSessionId);
    }



}

void DevToolsActionInspect::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    LastMessage = Message;
    Next();
}

