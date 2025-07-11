#ifndef TOOLBOXHANDLER_H
#define TOOLBOXHANDLER_H

#include "include/cef_client.h"
#include <functional>


class ToolboxHandler : public CefClient
{
public:
    ToolboxHandler();

    //CefClient
    bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) OVERRIDE;

    //Events
    std::vector<std::function<void(CefRefPtr<CefBrowser>, CefProcessId, CefRefPtr<CefProcessMessage>, bool*)> > EventProcessMessage;

private:
    IMPLEMENT_REFCOUNTING(ToolboxHandler);
};

#endif // TOOLBOXHANDLER_H
