#include "toolboxv8handler.h"

ToolboxV8Handler::ToolboxV8Handler()
{
    IsEventTrigger = false;
    IsInitialized = false;
    IsInterrupt = false;
    IsEditCancel = false;
    IsMaximize = false;
    IsMinimize = false;
    IsInterfaceState = false;
    url_changed = false;
    enable_module_changed = false;
    ChangedExecute = false;
    ChangedIncrementHighlightIndex = false;
    IsEmbeddedData = false;
    ChangedMultiselectState = false;
    ChangedMultiselectReset = false;
}

bool ToolboxV8Handler::GetIncrementHighlightIndex()
{
    bool Result = ChangedIncrementHighlightIndex;
    ChangedIncrementHighlightIndex = false;
    return Result;
}

std::pair<ToolboxV8Handler::ExecuteClass, bool> ToolboxV8Handler::GetExecuteCode()
{
    std::pair<ToolboxV8Handler::ExecuteClass, bool> r;
    r.first = LastExecute;
    r.second = ChangedExecute;

    ChangedExecute = false;

    LastExecute.Execute.clear();

    return r;
}

std::pair<std::string, bool> ToolboxV8Handler::GetEmbeddedData()
{
    std::pair<std::string, bool> r;
    r.first = EmbeddedData;
    r.second = IsEmbeddedData;

    IsEmbeddedData = false;

    EmbeddedData.clear();

    return r;
}



std::pair<std::string, bool> ToolboxV8Handler::GetInterfaceState()
{
    std::pair<std::string, bool> r;
    r.second = IsInterfaceState;
    if(IsInterfaceState)
    {
        r.first = interfacestate;
        interfacestate.clear();
        IsInterfaceState = false;
    }
    return r;
}


std::pair<ToolboxV8Handler::ResultClass, bool> ToolboxV8Handler::GetResult()
{
    std::pair<ResultClass, bool> r;
    r.second = !LastResult.empty();
    if(!LastResult.empty())
    {
        r.first = LastResult.at(0);
        LastResult.erase(LastResult.begin());
    }
    return r;
}

std::string ToolboxV8Handler::GetEventTriggerName()
{
    return EventTriggerName;
}

std::string ToolboxV8Handler::GetEventTriggerData()
{
    return EventTriggerData;
}

bool ToolboxV8Handler::GetIsEventTrigger()
{
    bool res = IsEventTrigger;
    IsEventTrigger = false;
    return res;
}

bool ToolboxV8Handler::GetIsInitialized()
{
    return IsInitialized;
}

bool ToolboxV8Handler::GetIsMaximize()
{
    bool res = IsMaximize;
    IsMaximize = false;
    return res;
}

bool ToolboxV8Handler::GetIsMinimize()
{
    bool res = IsMinimize;
    IsMinimize = false;
    return res;
}

bool ToolboxV8Handler::GetIsEditCancel()
{
    bool res = IsEditCancel;
    IsEditCancel = false;

    return res;
}

bool ToolboxV8Handler::GetIsInterrupt()
{
    bool res = IsInterrupt;
    IsInterrupt = false;

    return res;
}

bool ToolboxV8Handler::GetClearHighlight()
{
    bool res = ClearHighlight;
    ClearHighlight = false;

    return res;
}


bool ToolboxV8Handler::Execute(const CefString& name, CefRefPtr<CefListValue> arguments)
{
    if(name == std::string("BrowserAutomationStudio_Append"))
    {
        if (arguments->GetSize() >= 3 && arguments->GetType(0) == VTYPE_STRING && arguments->GetType(1) == VTYPE_STRING && arguments->GetType(2) == VTYPE_INT)
        {
            ResultClass res;
            res.Name = arguments->GetString(0);
            res.Code = arguments->GetString(1);
            switch(arguments->GetInt(2))
            {
                case 0: res.HowToExecute = OnlyExecute; break;
                case 1: res.HowToExecute = OnlyAdd; break;
                case 2: res.HowToExecute = AddAndExecute; break;
            }
            res.DisableIfAdd = true;
            if(arguments->GetSize() > 3)
                res.DisableIfAdd = arguments->GetBool(3);

            if(arguments->GetSize() > 4)
                res.Id = arguments->GetString(4);

            res.CanRestartBrowser = false;
            if(arguments->GetSize() > 5)
                res.CanRestartBrowser = arguments->GetBool(5);

            LastResult.push_back(res);
        }
    }else if(name == std::string("BrowserAutomationStudio_Execute"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            LastExecute.Execute = arguments->GetString(0);
            ChangedExecute = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_IncrementHighlightIndex"))
    {
        ChangedIncrementHighlightIndex = true;
    }else if(name == std::string("BrowserAutomationStudio_Initialized"))
    {
        IsInitialized = true;
    }
    else if(name == std::string("BrowserAutomationStudio_EditCancel"))
    {
        IsEditCancel = true;
    }else if(name == std::string("BrowserAutomationStudio_Maximize"))
    {
        IsMaximize = true;
    }else if(name == std::string("BrowserAutomationStudio_Minimize"))
    {
        IsMinimize = true;
    }else if(name == std::string("BrowserAutomationStudio_Interrupt"))
    {
        IsInterrupt = true;
    }else if(name == std::string("BrowserAutomationStudio_OpenUrl"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            url = arguments->GetString(0);
            url_changed = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_EnableModule"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            enable_module = arguments->GetString(0);
            enable_module_changed = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_MultiselectStateChanged"))
    {
        if (arguments->GetSize() == 2 && arguments->GetType(0) == VTYPE_INT && arguments->GetType(1) == VTYPE_STRING)
        {
            MultiselectState.IsActive = arguments->GetInt(0) == 1;
            MultiselectState.Serialized = arguments->GetString(1);
            ChangedMultiselectState = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_MultiselectReset"))
    {
        if (arguments->GetSize() == 0)
        {
            ChangedMultiselectReset = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_SaveInterfaceState"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            interfacestate = arguments->GetString(0);
            IsInterfaceState = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_SendEmbeddedData"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            /*if(!EmbeddedData.empty())
                EmbeddedData += "--BAS-PATCH--";*/
            EmbeddedData = arguments->GetString(0);
            IsEmbeddedData = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_ClearHighlight"))
    {
        if (arguments->GetSize() == 0)
        {
            ClearHighlight = true;
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

std::pair<std::string, bool> ToolboxV8Handler::GetLoadUrl()
{

    std::pair<std::string, bool> r;
    r.first = url;
    r.second = url_changed;

    url_changed = false;

    url.clear();

    return r;
}

std::pair<std::string, bool> ToolboxV8Handler::GetEnableModule()
{

    std::pair<std::string, bool> r;
    r.first = enable_module;
    r.second = enable_module_changed;

    enable_module_changed = false;

    enable_module.clear();

    return r;
}

std::pair<ToolboxV8Handler::MultiSelectStateClass, bool> ToolboxV8Handler::GetMultiselectStateChanged()
{
    std::pair<ToolboxV8Handler::MultiSelectStateClass, bool> r;
    r.first = MultiselectState;
    r.second = ChangedMultiselectState;

    ChangedMultiselectState = false;

    MultiselectState.Serialized.clear();
    MultiselectState.IsActive = false;


    return r;
}

bool ToolboxV8Handler::GetMultiselectReset()
{
    bool r = ChangedMultiselectReset;

    ChangedMultiselectReset = false;

    return r;
}
