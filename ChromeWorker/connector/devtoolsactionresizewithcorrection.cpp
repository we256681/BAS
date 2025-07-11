#include "devtoolsactionresizewithcorrection.h"

void DevToolsActionResizeWithCorrection::Run()
{
    State = Running;
    InitialWidth = Params["bounds.width"].Number;
    InitialHeight = Params["bounds.height"].Number;
    Try();
}

void DevToolsActionResizeWithCorrection::Try()
{
    TryNumber++;
    if(TryNumber > 2)
    {
        Result->Success();
        State = Finished;
        return;
    }
    IsResizing = true;
    IsCheckingSize = false;
    std::map<std::string, Variant> CurrentParams;
    CurrentParams["bounds.width"] = InitialWidth + GlobalState->WidthDifference;
    CurrentParams["bounds.height"] = InitialHeight + GlobalState->HeightDifference;

    RunAction("ResizeBrowser", CurrentParams);
}

void DevToolsActionResizeWithCorrection::OnActionFinished()
{
    if(IsResizing)
    {
        if(!CurrentAction->GetResult()->GetIsSuccess())
        {
            Result->Fail(CurrentAction->GetResult()->GetErrorMessage(), CurrentAction->GetResult()->GetErrorCode());
            State = Finished;
            return;
        }

        IsResizing = false;
        IsCheckingSize = true;
        RunJavascriptAction("[[WIDTH]] = _BAS_SAFE(Window.innerWidth);[[HEIGHT]] = _BAS_SAFE(Window.innerHeight);", std::string(), std::string("[]"));
    }else if(IsCheckingSize)
    {
        if(!CurrentAction->GetResult()->GetIsSuccess())
        {
            Result->Fail(CurrentAction->GetResult()->GetErrorMessage(), CurrentAction->GetResult()->GetErrorCode());
            State = Finished;
            return;
        }

        std::string ResultString = CurrentAction->GetResult()->GetString();


        int WidthResult = Parser.GetFloatFromJson(ResultString, "WIDTH");
        int HeightResult = Parser.GetFloatFromJson(ResultString, "HEIGHT");

        if(WidthResult == InitialWidth && HeightResult == InitialHeight)
        {
            Result->Success();
            State = Finished;
            return;
        }else
        {
            GlobalState->WidthDifference += InitialWidth - WidthResult;
            GlobalState->HeightDifference += InitialHeight - HeightResult;

            Try();
        }
    }

}

