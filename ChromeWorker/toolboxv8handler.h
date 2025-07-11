#ifndef TOOLBOXV8HANDLER_H
#define TOOLBOXV8HANDLER_H

#include "include/cef_app.h"
#include <functional>
#include <mutex>

class ToolboxV8Handler : public CefBaseRefCounted
{
public:
    enum ExecuteType{OnlyExecute, OnlyAdd, AddAndExecute};
    struct ResultClass
    {
        std::string Name;
        std::string Code;
        ExecuteType HowToExecute;
        bool DisableIfAdd;
        bool CanRestartBrowser;
        std::string Id;
    };
    struct ExecuteClass
    {
        std::string Execute;
    };
    struct MultiSelectStateClass
    {
        std::string Serialized;
        bool IsActive;
    };

private:
    std::vector<ResultClass> LastResult;

    std::string EventTriggerData;
    std::string EventTriggerName;

    bool IsEventTrigger;
    bool IsInitialized;
    bool IsEditCancel;
    bool ChangedExecute;
    bool ChangedIncrementHighlightIndex;

    bool IsMaximize;
    bool IsMinimize;
    bool IsInterrupt;
    bool IsInterfaceState;
    bool ClearHighlight;
    bool IsEmbeddedData;


    MultiSelectStateClass MultiselectState;
    bool ChangedMultiselectState;
    bool ChangedMultiselectReset;

    std::string url;
    bool url_changed;

    std::string enable_module;
    bool enable_module_changed;

    std::string interfacestate;
    ExecuteClass LastExecute;
    std::string EmbeddedData;


public:
    ToolboxV8Handler();
    bool GetIsEventTrigger();
    bool GetIsInitialized();
    bool GetIsEditCancel();
    bool GetIsMaximize();
    bool GetIsMinimize();
    bool GetIsInterrupt();
    bool GetClearHighlight();
    std::string GetEventTriggerData();
    std::string GetEventTriggerName();
    std::pair<std::string, bool> GetInterfaceState();

    std::pair<std::string, bool> GetLoadUrl();
    std::pair<std::string, bool> GetEnableModule();
    std::pair<ExecuteClass, bool> GetExecuteCode();
    bool GetIncrementHighlightIndex();
    std::pair<std::string, bool> GetEmbeddedData();
    std::pair<MultiSelectStateClass, bool> GetMultiselectStateChanged();
    bool GetMultiselectReset();

    virtual bool Execute(const CefString& name, CefRefPtr<CefListValue> arguments);
    std::pair<ResultClass, bool> GetResult();

private:
    IMPLEMENT_REFCOUNTING(ToolboxV8Handler);
};

#endif // TOOLBOXV8HANDLER_H
