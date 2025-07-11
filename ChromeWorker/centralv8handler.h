#ifndef CENTRALV8HANDLER_H
#define CENTRALV8HANDLER_H


#include "include/cef_app.h"
#include <functional>
#include <mutex>

class CentralV8Handler : public CefBaseRefCounted
{
    std::string url;
    bool url_changed;

    std::string menu;
    bool menu_changed;


    bool close;

    std::string settings;
    bool settings_changed;

    int manual_control;
    bool manual_control_changed;


public:
    CentralV8Handler();
    bool Execute(const CefString& name, CefRefPtr<CefListValue> arguments);
    std::pair<std::string, bool> GetLoadUrl();
    std::pair<int, bool> GetManualControl();
    std::pair<std::string, bool> GetSettings();
    std::pair<std::string, bool> GetMenuSelected();

    bool GetClose();
private:
    IMPLEMENT_REFCOUNTING(CentralV8Handler);
};

#endif // CENTRALV8HANDLER_H
