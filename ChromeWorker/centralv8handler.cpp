#include "centralv8handler.h"
#include "log.h"

CentralV8Handler::CentralV8Handler()
{
    url_changed = false;
    menu_changed = false;
    settings_changed = false;
    manual_control_changed = false;
    manual_control = -1;
    close = false;
}

bool CentralV8Handler::Execute(const CefString& name, CefRefPtr<CefListValue> arguments)
{
    if(name == std::string("BrowserAutomationStudio_OpenUrl"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            url = arguments->GetString(0);
            url_changed = true;
        }
    }

    if(name == std::string("BrowserAutomationStudio_Menu"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            menu = arguments->GetString(0);
            menu_changed = true;
        }
    }


    if(name == std::string("BrowserAutomationStudio_SetOptions"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            settings = arguments->GetString(0);
            settings_changed = true;
        }
    }

    if(name == std::string("BrowserAutomationStudio_ManualControl"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_INT)
        {
            manual_control = arguments->GetInt(0);
            manual_control_changed = true;
        }
    }

    if(name == std::string("BrowserAutomationStudio_Close"))
    {
        if (arguments->GetSize() == 0)
        {
            close = true;
        }
    }
    return true;
}

std::pair<std::string, bool> CentralV8Handler::GetLoadUrl()
{

    std::pair<std::string, bool> r;
    r.first = url;
    r.second = url_changed;

    url_changed = false;

    url.clear();

    return r;
}

std::pair<int, bool> CentralV8Handler::GetManualControl()
{
    std::pair<int, bool> r;
    r.first = manual_control;
    r.second = manual_control_changed;

    manual_control_changed = false;

    manual_control = -1;

    return r;
}

std::pair<std::string, bool> CentralV8Handler::GetMenuSelected()
{

    std::pair<std::string, bool> r;
    r.first = menu;
    r.second = menu_changed;

    menu_changed = false;

    menu.clear();

    return r;
}

std::pair<std::string, bool> CentralV8Handler::GetSettings()
{

    std::pair<std::string, bool> r;
    r.first = settings;
    r.second = settings_changed;

    settings_changed = false;

    settings.clear();

    return r;
}

bool CentralV8Handler::GetClose()
{

    bool r = close;
    close = false;


    return r;
}
