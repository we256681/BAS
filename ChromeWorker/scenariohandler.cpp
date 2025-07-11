#include "scenariohandler.h"
#include <cmath>

ScenarioHandler::ScenarioHandler()
{

}

CefRefPtr<CefLoadHandler> ScenarioHandler::GetLoadHandler()
{
    return this;
}



bool ScenarioHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
    bool IsProcessed = false;
    for(auto f:EventProcessMessage)
        f(browser, source_process, message, &IsProcessed);
    return IsProcessed;
}


void ScenarioHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack,bool canGoForward)
{
    if(!isLoading)
    {
        browser->GetHost()->SetZoomLevel(std::log((float)Zoom * 0.01)/std::log(1.2));
    }
}
