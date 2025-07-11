#include "devtoolsactiontouch.h"

void DevToolsActionTouch::Run()
{
    State = Running;

    //Parse params and save to internal variables
    IsSendingTouchEvent = false;
    FrameFinderType = Params["type"].String;
    std::map<std::string, Variant>& Point = Params["touchPoints"].List[0].Map;
    X = Point["x"].Number;
    Y = Point["y"].Number;
    FrameFinderId = std::to_string(Point["id"].Number);


    if(GlobalState->Frames.empty())
    {
        //In case if there are no isolated frames or we use mouse move - send event as is
        //WORKER_LOG("{{{" + std::to_string(GetId()) + "," + FrameFinderId + "}}} DISPATCH IMMEDIATELLY, NOT IN FRAME ");
        /*if(FrameFinderType != "touchMove")
        {
            WORKER_LOG("+++++++++++++++++++ START [" + FrameFinderId + "] " + FrameFinderType + std::string(", hanging: ") + GlobalState->FindFrames.GetReport());
        }*/
        IsSendingTouchEvent = true;
        SendWebSocket("Input.dispatchTouchEvent", Params, GlobalState->TabId);
        return;
    }else
    {
        //If we do click - we need to know session id of a frame under cursor
        FindFrame();
    }

}

void DevToolsActionTouch::FindFrame()
{
    IsWaitingForFindFrame = false;
    //Instead of finding frame for each action, we find it once before touch start event and then use saved result if touch id is the same
    FrameFinder::StatusInfo FrameFindInfo = GlobalState->FindFrames.GetSessionId(FrameFinderId, FrameFinderType);


    if(FrameFindInfo.Status == FrameFinder::StatusInfo::Success)
    {
        //We can use previous data
        //WORKER_LOG("{{{" + std::to_string(GetId()) + "," + FrameFinderId + "}}} SUCCESS ");
        /*if(FrameFinderType != "touchMove")
        {
            WORKER_LOG("+++++++++++++++++++ START [" + FrameFinderId + "] " + FrameFinderType + std::string(", hanging: ") + GlobalState->FindFrames.GetReport());
        }*/
        IsSendingTouchEvent = true;
        SendWebSocket("Input.dispatchTouchEvent", Params, FrameFindInfo.SessionId);
        return;
    }else if(FrameFindInfo.Status == FrameFinder::StatusInfo::DoRequest)
    {
        //Need to find session id now, all other actions are waiting
        //WORKER_LOG("{{{" + std::to_string(GetId()) + "," + FrameFinderId + "}}} DO REQUEST ");
        std::map<std::string, Variant> CurrentParams;
        CurrentParams["x"] = Variant(X);
        CurrentParams["y"] = Variant(Y);
        CurrentParams["position"] = Variant(0);
        RunAction("Inspect", CurrentParams);
    }else if(FrameFindInfo.Status == FrameFinder::StatusInfo::Wait)
    {
        //Need to wait while other actions will find frame and later use its result
        //WORKER_LOG("{{{" + std::to_string(GetId()) + "," + FrameFinderId + "}}} WAIT ");
        IsWaitingForFindFrame = true;
        if(!IsTimerEventSubscribed)
        {
            //WORKER_LOG("{{{" + std::to_string(GetId()) + "," + FrameFinderId + "}}} SUBSCRIBE TO TIMER ");

            SubscribbedEvents.push_back("Timer");
            IsTimerEventSubscribed = true;
        }
    }else if(FrameFindInfo.Status == FrameFinder::StatusInfo::IgnoreEvent)
    {
        //Ignore this event
        //WORKER_LOG("{{{" + std::to_string(GetId()) + "," + FrameFinderId + "}}} IGNORE ");
        Result->Success(std::string(""));
        State = Finished;
    }else if(FrameFindInfo.Status == FrameFinder::StatusInfo::CloseHangingTouch)
    {
        //Close old touch

        IsSendingTouchEvent = true;
        Params["touchPoints"].List[0].Map["id"].Number = std::stoi(FrameFindInfo.ActionId);
        Params["type"].String = "touchEnd";

        FrameFinderType = Params["type"].String;
        std::map<std::string, Variant>& Point = Params["touchPoints"].List[0].Map;
        FrameFinderId = std::to_string(Point["id"].Number);

        /*if(FrameFinderType != "touchMove")
        {
            WORKER_LOG("+++++++++++++++++++ START [" + FrameFinderId + "] " + FrameFinderType + std::string(", hanging: ") + GlobalState->FindFrames.GetReport());
        }*/
        SendWebSocket("Input.dispatchTouchEvent", Params, FrameFindInfo.SessionId);
        return;
    }
}

void DevToolsActionTouch::OnWebSocketEvent(const std::string& Method, const std::string& Message)
{
    if(Method == "Timer" && IsWaitingForFindFrame)
    {
        //Check if result is obtained in other actions
        //WORKER_LOG("{{{" + std::to_string(GetId()) + "," + FrameFinderId + "}}} TIMER ");
        FindFrame();
    }
}

void DevToolsActionTouch::OnActionFinished()
{
    if(CurrentAction->GetResult()->GetIsSuccess())
    {
        //We know the frame under cursor, use it to send touch event
        std::string Message = CurrentAction->GetResult()->GetString();
        std::string TabId = Parser.GetStringFromJson(Message, "tab_id");

        //WORKER_LOG("{{{" + std::to_string(GetId()) + "," + FrameFinderId + "}}} ACTION FINISHED ");

        //Deliver results to other frames only after touch will be performed
        NeedToSetSessionId = true;
        SessionIdToSave = TabId;

        if(TabId != GlobalState->TabId)
        {
            IsInIsolatedFrame = true;
        }


        /*if(FrameFinderType != "touchMove")
        {
            WORKER_LOG("+++++++++++++++++++ START [" + FrameFinderId + "] " + FrameFinderType + std::string(", hanging: ") + GlobalState->FindFrames.GetReport());
        }*/
        SendWebSocket("Input.dispatchTouchEvent", Params, TabId);

        IsSendingTouchEvent = true;

        return;
    } else
    {
        //Failed to find frame, report about error and finish action
        //WORKER_LOG("{{{" + std::to_string(GetId()) + "," + FrameFinderId + "}}} ACTION FAILED");
        Result->Fail(CurrentAction->GetResult()->GetErrorMessage(), CurrentAction->GetResult()->GetErrorCode());
        State = Finished;
        return;
    }

}



void DevToolsActionTouch::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    if(IsSendingTouchEvent)
    {
        //If we are sending touch event - process message

        //WORKER_LOG("{{{" + std::to_string(GetId()) + "," + FrameFinderId + "}}} TOUCH FINISHED");

        //Deliver results to other frames
        if(NeedToSetSessionId)
        {
            //WORKER_LOG("{{{" + std::to_string(GetId()) + "," + FrameFinderId + "}}} RESOLVE FRAME ID");
            GlobalState->FindFrames.SetSessionId(FrameFinderId, SessionIdToSave);
        }

        /*if(FrameFinderType != "touchMove")
        {
            WORKER_LOG("---------------- END [" + FrameFinderId + "] " + FrameFinderType + std::string(", hanging: ") + GlobalState->FindFrames.GetReport());
        }*/


        std::map<std::string, Variant> Data;
        Data["is_in_isolated_frame"] = Variant(IsInIsolatedFrame);

        JsonSerializer Serializer;

        Result->Success(Serializer.SerializeObjectToString(Data));
        State = Finished;
    }else
    {
        //If we are not sending touch event - let "Inspect" action do processing
        DevToolsMultiAction::OnWebSocketMessage(Message, Error);
    }
}
