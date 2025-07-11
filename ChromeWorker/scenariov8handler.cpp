#include "scenariov8handler.h"
#include "log.h"


ScenarioV8Handler::ScenarioV8Handler()
{
    Changed = false;
    ChangedExecute = false;
    ChangedCurrentFunction = false;
    ChangedDetectorData = false;
    ChangedWebInterfaceResult = false;
    ChangedPrepareFunctionResult = false;
    IsInitialized = false;
    NeedRestart = None;
    IsEventTrigger = false;
    IsEditStart = false;
    IsEditEnd = false;
    url_changed = false;
    IsInterfaceJson = false;
    IsInterfaceState = false;
    IsHightlightMenuItem = false;
    IsThreadNumberEditStart = false;
    IsSuccessNumberEditStart = false;
    IsFailNumberEditStart = false;
    IsClipboardGetRequest = false;
    IsClipboardSetRequest = false;
    IsUpdateEmbeddedData = false;
    IsRunFunctionStart = false;
    LastResultIsSilent = false;
    LastResultIsPlay = false;
    IsRunFunctionSeveralThreadsStart = false;
    IsRunFunctionAsync = false;
    IsOpenAction = false;
    IsEditSaveStart = false;
    ChangedIsInsideElementLoop = false;
    IsStartBackup = false;
    clipboard_encoded = false;
    clipboard_prefix = false;
}

std::pair<bool, bool> ScenarioV8Handler::GetIsInsideElementLoop()
{
    std::pair<bool, bool> r;
    r.first = IsInsideElementLoop;
    r.second = ChangedIsInsideElementLoop;

    ChangedIsInsideElementLoop = false;

    return r;
}


std::pair<std::vector<ScenarioV8Handler::LastResultStruct>, bool> ScenarioV8Handler::GetResult()
{
    std::pair<std::vector<ScenarioV8Handler::LastResultStruct>, bool> r;
    r.first = LastResult;
    r.second = Changed;

    Changed = false;

    LastResult.clear();

    return r;
}

std::pair<ScenarioV8Handler::WebInterfaceTaskResultStruct, bool> ScenarioV8Handler::GetWebInterfaceTaskResult()
{
    std::pair<WebInterfaceTaskResultStruct, bool> r;
    r.first = WebInterfaceTaskResult;
    r.second = ChangedWebInterfaceResult;

    ChangedWebInterfaceResult = false;

    WebInterfaceTaskResult.LastWebInterfaceResultData.clear();
    WebInterfaceTaskResult.LastWebInterfaceResultId = -1;

    return r;
}

std::pair<ScenarioV8Handler::PrepareFunctionResultStruct, bool> ScenarioV8Handler::GetPrepareFunctionResult()
{
    std::pair<PrepareFunctionResultStruct, bool> r;
    r.first = PrepareFunctionResult;
    r.second = ChangedPrepareFunctionResult;

    ChangedPrepareFunctionResult = false;

    PrepareFunctionResult.FunctionData.clear();
    PrepareFunctionResult.FunctionName.clear();

    return r;
}

std::pair<std::string, bool> ScenarioV8Handler::GetIsEditStart()
{
    std::pair<std::string, bool> r;
    r.first = EditStartScript;
    r.second = IsEditStart;

    IsEditStart = false;

    EditStartScript.clear();

    return r;
}

bool ScenarioV8Handler::GetIsEditEnd()
{
    bool res = IsEditEnd;
    IsEditEnd = false;
    return res;
}



std::pair< std::pair<std::string,bool>, std::pair<bool,bool> > ScenarioV8Handler::GetExecuteCode()
{
    std::pair< std::pair<std::string,bool>, std::pair<bool,bool> > r;
    r.first.first = LastResultExecute;
    r.first.second = LastResultIsPlay;
    r.second.first = ChangedExecute;
    r.second.second = LastResultIsSilent;

    ChangedExecute = false;

    LastResultExecute.clear();
    LastResultIsSilent = false;
    LastResultIsPlay = false;

    return r;
}

std::pair<std::string, bool> ScenarioV8Handler::GetCurrentFunction()
{
    std::pair<std::string, bool> r;
    r.first = LastCurrentFunction;
    r.second = ChangedCurrentFunction;

    ChangedCurrentFunction = false;

    LastCurrentFunction.clear();

    return r;
}

std::pair<std::string, bool> ScenarioV8Handler::GetLoadUrl()
{

    std::pair<std::string, bool> r;
    r.first = url;
    r.second = url_changed;

    url_changed = false;

    url.clear();

    return r;
}

std::pair<std::string, bool> ScenarioV8Handler::GetDetectorDataCode()
{
    std::pair<std::string, bool> r;
    r.first = DetectorData;
    r.second = ChangedDetectorData;

    ChangedDetectorData = false;

    DetectorData.clear();

    return r;
}

bool ScenarioV8Handler::GetIsInitialized()
{
    return IsInitialized;
}

ScenarioV8Handler::RestartType ScenarioV8Handler::GetNeedRestart()
{
    RestartType res = NeedRestart;
    NeedRestart = None;
    return res;
}


bool ScenarioV8Handler::Execute(const CefString& name, CefRefPtr<CefListValue> arguments)
{
    size_t size = arguments->GetSize();

    if(name == std::string("BrowserAutomationStudio_SendCode"))
    {
        if (arguments->GetSize() == 7 && arguments->GetType(0) == VTYPE_STRING && arguments->GetType(1) == VTYPE_STRING && arguments->GetType(2) == VTYPE_STRING && arguments->GetType(3) == VTYPE_STRING&& arguments->GetType(4) == VTYPE_STRING&& arguments->GetType(5) == VTYPE_STRING)
        {
            LastResultStruct NewElement;
            NewElement.LastResultCodeDiff = arguments->GetString(0);
            NewElement.LastResultFunctions = arguments->GetString(1);
            NewElement.LastResultResources = arguments->GetString(2);
            NewElement.LastResultVariables = arguments->GetString(3);
            NewElement.LastResultGlobalVariables = arguments->GetString(4);
            NewElement.LastResultLabels = arguments->GetString(5);
            if(arguments->GetType(6) == VTYPE_INT)
                NewElement.ExecuteNextId = arguments->GetInt(6);
            LastResult.push_back(NewElement);
            Changed = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_UpdateEmbeddedData"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            if(!UpdateEmbeddedData.empty())
                UpdateEmbeddedData += ";";
            UpdateEmbeddedData += arguments->GetString(0);
            IsUpdateEmbeddedData = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_Initialized"))
    {
        IsInitialized = true;
    }else if(name == std::string("BrowserAutomationStudio_Execute"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            LastResultExecute = arguments->GetString(0);
            LastResultIsPlay = false;
            LastResultIsSilent = false;
            ChangedExecute = true;
        }
        if (arguments->GetSize() == 2 && arguments->GetType(0) == VTYPE_STRING && arguments->GetType(1) == VTYPE_BOOL)
        {
            LastResultExecute = arguments->GetString(0);
            LastResultIsPlay = arguments->GetBool(1);
            LastResultIsSilent = false;
            ChangedExecute = true;
        }
        if (arguments->GetSize() == 3 && arguments->GetType(0) == VTYPE_STRING && arguments->GetType(1) == VTYPE_BOOL && arguments->GetType(2) == VTYPE_BOOL)
        {
            LastResultExecute = arguments->GetString(0);
            LastResultIsPlay = arguments->GetBool(1);
            LastResultIsSilent = arguments->GetBool(2);
            ChangedExecute = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_SetCurrentFunction"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            LastCurrentFunction = arguments->GetString(0);
            ChangedCurrentFunction = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_SetWebInterfaceTaskResult"))
    {
        if (arguments->GetSize() == 2 && arguments->GetType(0) == VTYPE_INT && arguments->GetType(1) == VTYPE_STRING)
        {
            WebInterfaceTaskResult.LastWebInterfaceResultData = arguments->GetString(1);
            WebInterfaceTaskResult.LastWebInterfaceResultId = arguments->GetInt(0);
            ChangedWebInterfaceResult = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_PrepareFunctionResult"))
    {
        if (arguments->GetSize() == 2 && arguments->GetType(0) == VTYPE_STRING && arguments->GetType(1) == VTYPE_STRING)
        {
            PrepareFunctionResult.FunctionName = arguments->GetString(0);
            PrepareFunctionResult.FunctionData = arguments->GetString(1);
            ChangedPrepareFunctionResult = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_IsInsideElementLoopChanged"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_BOOL)
        {
            IsInsideElementLoop = arguments->GetBool(0);
            ChangedIsInsideElementLoop = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_SendDetectorBrowserData"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            DetectorData = arguments->GetString(0);
            ChangedDetectorData = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_OpenUrl"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            url = arguments->GetString(0);
            url_changed = true;
        }
    }
    else if(name == std::string("BrowserAutomationStudio_Restart"))
    {
        if (arguments->GetSize() == 1)
        {
            WORKER_LOG(std::string("BrowserAutomationStudio_Restart<<") + std::to_string(arguments->GetBool(0)));
            if(arguments->GetBool(0))
            {
                NeedRestart = Stop;
            }else
            {
                NeedRestart = Restart;
            }
        }
    }
    else if(name == std::string("BrowserAutomationStudio_EditStart"))
    {
        if (arguments->GetSize() == 1)
        {
            EditStartScript = arguments->GetString(0);
            IsEditStart = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_EditSaveStart"))
    {
        if (arguments->GetSize() == 0)
        {
            IsEditSaveStart = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_EditEnd"))
    {
        if (arguments->GetSize() == 0)
        {
            IsEditEnd = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_ThreadNumberEdit"))
    {
        IsThreadNumberEditStart = true;
    }else if(name == std::string("BrowserAutomationStudio_SuccessNumberEdit"))
    {
        IsSuccessNumberEditStart = true;
    }else if(name == std::string("BrowserAutomationStudio_FailNumberEdit"))
    {
        IsFailNumberEditStart = true;
    }else if(name == std::string("BrowserAutomationStudio_RunFunction"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            RunFunctionName = arguments->GetString(0);
            IsRunFunctionStart = true;
        }

    }else if(name == std::string("BrowserAutomationStudio_OpenAction"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            OpenActionName = arguments->GetString(0);
            IsOpenAction = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_RunFunctionSeveralThreads"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            RunFunctionInSeveralThreadsName = arguments->GetString(0);
            IsRunFunctionSeveralThreadsStart = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_RunFunctionAsync"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            RunFunctionAsyncName = arguments->GetString(0);
            IsRunFunctionAsync = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_SaveInterfaceState"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            InterfaceState = arguments->GetString(0);
            IsInterfaceState = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_SaveInterfaceJson"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            InterfaceJson = arguments->GetString(0);
            IsInterfaceJson = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_SetClipboard"))
    {
        if (size > 0)
        {
            clipboard_set = arguments->GetString(0);
            clipboard_prefix = size > 1 ? arguments->GetBool(1) : true;
            clipboard_encoded = size > 2 ? arguments->GetBool(2) : false;
            IsClipboardSetRequest = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_GetClipboard"))
    {
        if (arguments->GetSize() == 0)
        {
            IsClipboardGetRequest = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_HighlightMenuItem"))
    {
        if (arguments->GetSize() == 1)
        {
            HighlightMenuItem = arguments->GetString(0);
            IsHightlightMenuItem = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_StartBackup"))
    {
        if (arguments->GetSize() == 0)
        {
            IsStartBackup = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_TriggerEvent"))
    {
        if (arguments->GetSize() > 0) 
        {
            if (arguments->GetSize() >= 1 && arguments->GetType(0) == VTYPE_STRING)
            {
                EventTriggerName = arguments->GetString(0);
            }
            if (arguments->GetSize() >= 2 && arguments->GetType(1) == VTYPE_STRING)
            {
                EventTriggerData = arguments->GetString(1);
            }
            IsEventTrigger = true;
        }
    }

    return true;
}

std::pair<std::pair<std::string, std::pair<bool, bool>>, bool> ScenarioV8Handler::GetClipboardSetRequest()
{
    std::pair<std::pair<std::string, std::pair<bool, bool>>, bool> r;
    r.first = std::make_pair(clipboard_set, std::make_pair(clipboard_prefix, clipboard_encoded));
    r.second = IsClipboardSetRequest;

    IsClipboardSetRequest = false;

    clipboard_encoded = false;
    clipboard_prefix = true;
    clipboard_set.clear();

    return r;
}

std::pair<std::string, bool> ScenarioV8Handler::GetUpdateEmbeddedData()
{

    std::pair<std::string, bool> r;
    r.first = UpdateEmbeddedData;
    r.second = IsUpdateEmbeddedData;

    IsUpdateEmbeddedData = false;

    UpdateEmbeddedData.clear();

    return r;
}

bool ScenarioV8Handler::GetClipboardGetRequest()
{
    bool res = IsClipboardGetRequest;
    IsClipboardGetRequest = false;
    return res;
}

bool ScenarioV8Handler::GetStartBackup()
{
    bool res = IsStartBackup;
    IsStartBackup = false;
    return res;
}


bool ScenarioV8Handler::GetIsThreadNumberEditStart()
{
    bool res = IsThreadNumberEditStart;
    IsThreadNumberEditStart = false;
    return res;
}


std::pair<std::string, bool> ScenarioV8Handler::GetIsRunFunctionStart()
{
    std::pair<std::string, bool> r;
    r.first = RunFunctionName;
    r.second = IsRunFunctionStart;

    RunFunctionName.clear();
    IsRunFunctionStart = false;
    return r;
}

bool ScenarioV8Handler::GetIsEditSaveStart()
{
    bool res = IsEditSaveStart;
    IsEditSaveStart = false;
    return res;
}

std::pair<std::string, bool> ScenarioV8Handler::GetIsRunFunctionSeveralThreadsStart()
{
    std::pair<std::string, bool> r;
    r.first = RunFunctionInSeveralThreadsName;
    r.second = IsRunFunctionSeveralThreadsStart;

    RunFunctionInSeveralThreadsName.clear();
    IsRunFunctionSeveralThreadsStart = false;
    return r;
}

std::pair<std::string, bool> ScenarioV8Handler::GetIsRunFunctionAsync()
{
    std::pair<std::string, bool> r;
    r.first = RunFunctionAsyncName;
    r.second = IsRunFunctionAsync;

    RunFunctionAsyncName.clear();
    IsRunFunctionAsync = false;
    return r;
}

std::pair<std::string, bool> ScenarioV8Handler::GetIsOpenAction()
{
    std::pair<std::string, bool> r;
    r.first = OpenActionName;
    r.second = IsOpenAction;

    OpenActionName.clear();
    IsOpenAction = false;
    return r;
}

bool ScenarioV8Handler::GetIsSuccessNumberEditStart()
{
    bool res = IsSuccessNumberEditStart;
    IsSuccessNumberEditStart = false;
    return res;

}

bool ScenarioV8Handler::GetIsFailNumberEditStart()
{
    bool res = IsFailNumberEditStart;
    IsFailNumberEditStart = false;
    return res;
}

std::pair<std::string, bool> ScenarioV8Handler::GetIsHighlightMenuItem()
{
    std::pair<std::string, bool> r;
    r.second = IsHightlightMenuItem;
    IsHightlightMenuItem = false;
    r.first = HighlightMenuItem;
    HighlightMenuItem.clear();
    return r;
}

std::pair<std::string, bool> ScenarioV8Handler::GetIsInterfaceState()
{
    std::pair<std::string, bool> r;
    r.second = IsInterfaceState;
    IsInterfaceState = false;
    r.first = InterfaceState;
    InterfaceState.clear();
    return r;
}

std::pair<std::string, bool> ScenarioV8Handler::GetIsInterfaceJson()
{
    std::pair<std::string, bool> r;
    r.second = IsInterfaceJson;
    IsInterfaceJson = false;
    r.first = InterfaceJson;
    InterfaceJson.clear();
    return r;
}

std::string ScenarioV8Handler::GetEventTriggerName()
{
    return EventTriggerName;
}

std::string ScenarioV8Handler::GetEventTriggerData()
{
    return EventTriggerData;
}

bool ScenarioV8Handler::GetIsEventTrigger()
{
    bool res = IsEventTrigger;
    IsEventTrigger = false;
    return res;
}
