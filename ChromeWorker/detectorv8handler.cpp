#include "detectorv8handler.h"

DetectorV8Handler::DetectorV8Handler()
{
    url_changed = false;
    fingerprint_key_changed = false;
    clear_all = false;
    logout = false;
    edit_source_changed = false;
    is_initialized = false;
}

bool DetectorV8Handler::Execute(const CefString& name, CefRefPtr<CefListValue> arguments)
{
    if(name == std::string("BrowserAutomationStudio_OpenUrl"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            url = arguments->GetString(0);
            url_changed = true;
        }
    }

    if(name == std::string("BrowserAutomationStudio_SaveFingerprintKey"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            fingerprint_key = arguments->GetString(0);
            fingerprint_key_changed = true;
        }
    }

    if(name == std::string("BrowserAutomationStudio_EditSource"))
    {
        if (arguments->GetSize() == 1)
        {
            edit_source = arguments->GetString(0);
            edit_source_changed = true;
        }
    }

    if(name == std::string("BrowserAutomationStudio_ClearAll"))
    {
        if (arguments->GetSize() == 0)
        {
            clear_all = true;
        }
    }

    if(name == std::string("BrowserAutomationStudio_Logout"))
    {
        if (arguments->GetSize() == 0)
        {
            logout = true;
        }
    }

    if(name == std::string("BrowserAutomationStudio_Initialized"))
    {
        if (arguments->GetSize() == 0)
        {
            is_initialized = true;
        }
    }

    return true;
}

bool DetectorV8Handler::GetClearAll()
{
    bool r = clear_all;
    clear_all = false;
    return r;
}

bool DetectorV8Handler::GetLogout()
{
    bool r = logout;
    logout = false;
    return r;
}

bool DetectorV8Handler::GetIsInitialized()
{
    return is_initialized;
}

std::pair<std::string, bool> DetectorV8Handler::GetLoadUrl()
{
    std::pair<std::string, bool> r;
    r.first = url;
    r.second = url_changed;

    url_changed = false;

    url.clear();

    return r;
}

std::pair<std::string, bool> DetectorV8Handler::GetFingerprintKey()
{

    std::pair<std::string, bool> r;
    r.first = fingerprint_key;
    r.second = fingerprint_key_changed;

    fingerprint_key_changed = false;

    fingerprint_key.clear();

    return r;
}

std::pair<std::string, bool> DetectorV8Handler::GetEditSource()
{

    std::pair<std::string, bool> r;
    r.first = edit_source;
    r.second = edit_source_changed;

    edit_source_changed = false;

    edit_source.clear();

    return r;
}

