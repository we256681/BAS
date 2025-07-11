#include "toolboxhandler.h"

ToolboxHandler::ToolboxHandler()
{

}

bool ToolboxHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
    bool IsProcessed = false;
    for(auto f:EventProcessMessage)
        f(browser, source_process, message, &IsProcessed);
    return IsProcessed;
}

