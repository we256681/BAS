#ifndef SCENARIOV8HANDLER_H
#define SCENARIOV8HANDLER_H

#include "include/cef_app.h"
#include <functional>
#include <mutex>

class ScenarioV8Handler : public CefBaseRefCounted
{
public:
    enum RestartType{None, Restart, Stop};
    struct LastResultStruct
    {
        std::string LastResultCodeDiff;
        std::string LastResultVariables;
        std::string LastResultGlobalVariables;
        std::string LastResultFunctions;
        std::string LastResultResources;
        std::string LastResultLabels;
        int ExecuteNextId = -1;
    };
    struct WebInterfaceTaskResultStruct
    {
        std::string LastWebInterfaceResultData;
        int LastWebInterfaceResultId;
    };
    struct PrepareFunctionResultStruct
    {
        std::string FunctionName;
        std::string FunctionData;
    };
private:
    std::vector<LastResultStruct> LastResult;
    WebInterfaceTaskResultStruct WebInterfaceTaskResult;
    PrepareFunctionResultStruct PrepareFunctionResult;

    std::string LastResultExecute;
    bool LastResultIsSilent;
    bool LastResultIsPlay;
    std::string LastCurrentFunction;
    std::string OpenActionName;
    bool Changed;
    bool ChangedExecute;
    bool ChangedCurrentFunction;
    RestartType NeedRestart;

    bool ChangedIsInsideElementLoop;
    bool IsInsideElementLoop;

    bool IsEditStart;
    bool IsEditEnd;
    bool IsUpdateEmbeddedData;
    std::string EditStartScript;

    std::string url;
    bool url_changed;

    std::string UpdateEmbeddedData;

    std::string RunFunctionName;
    std::string RunFunctionInSeveralThreadsName;
    std::string RunFunctionAsyncName;

    bool IsThreadNumberEditStart;
    bool IsSuccessNumberEditStart;
    bool IsFailNumberEditStart;
    bool IsRunFunctionStart;
    bool IsRunFunctionSeveralThreadsStart;
    bool IsRunFunctionAsync;
    bool IsOpenAction;

    std::string EventTriggerData;
    std::string EventTriggerName;
    bool IsEventTrigger;

    std::string DetectorData;
    bool ChangedDetectorData;

    std::string HighlightMenuItem;
    bool IsHightlightMenuItem;

    std::string InterfaceState;
    bool IsInterfaceState;

    std::string InterfaceJson;
    bool IsInterfaceJson;

    std::mutex mut_threadnumbereditstart;
    std::mutex mut_successnumbereditstart;
    std::mutex mut_failnumbereditstart;

    std::mutex mut_clipboard_get;
    std::mutex mut_clipboard_set;

    bool IsClipboardGetRequest;
    bool IsClipboardSetRequest;

    bool IsStartBackup;

    std::string clipboard_set;
    bool clipboard_encoded;
    bool clipboard_prefix;

    bool ChangedWebInterfaceResult;
    bool ChangedPrepareFunctionResult;
    bool IsEditSaveStart;

    bool IsInitialized;

public:

    ScenarioV8Handler();
    bool Execute(const CefString& name, CefRefPtr<CefListValue> arguments);
    std::pair<std::vector<ScenarioV8Handler::LastResultStruct>, bool> GetResult();
    std::pair<WebInterfaceTaskResultStruct, bool> GetWebInterfaceTaskResult();
    std::pair<PrepareFunctionResultStruct, bool> GetPrepareFunctionResult();

    std::pair< std::pair<std::string,bool>, std::pair<bool,bool> > GetExecuteCode();
    RestartType GetNeedRestart();
    bool GetIsEventTrigger();
    bool GetIsInitialized();
    bool GetIsEditEnd();
    bool GetIsThreadNumberEditStart();
    bool GetIsSuccessNumberEditStart();
    bool GetIsFailNumberEditStart();
    std::pair<std::string, bool> GetIsRunFunctionStart();
    std::pair<std::string, bool> GetIsRunFunctionSeveralThreadsStart();
    std::pair<std::string, bool> GetIsRunFunctionAsync();
    std::pair<std::string, bool> GetIsOpenAction();
    std::pair<std::string, bool> GetIsEditStart();
    bool GetIsEditSaveStart();
    std::pair<std::pair<std::string, std::pair<bool,bool>>, bool> GetClipboardSetRequest();
    std::pair<std::string, bool> GetUpdateEmbeddedData();
    std::pair<std::string, bool> GetDetectorDataCode();
    std::pair<std::string, bool> GetCurrentFunction();
    std::pair<std::string, bool> GetLoadUrl();
    std::pair<bool, bool> GetIsInsideElementLoop();
    std::string GetEventTriggerData();
    std::string GetEventTriggerName();

    std::pair<std::string, bool> GetIsHighlightMenuItem();
    std::pair<std::string, bool> GetIsInterfaceState();
    std::pair<std::string, bool> GetIsInterfaceJson();

    bool GetClipboardGetRequest();
    bool GetStartBackup();

private:
    IMPLEMENT_REFCOUNTING(ScenarioV8Handler);
};

#endif // SCENARIOV8HANDLER_H
