#ifndef SCENARIOHANDLER_H
#define SCENARIOHANDLER_H

#include "include/cef_client.h"
#include <functional>


class ScenarioHandler : public CefClient, public CefLoadHandler
{
public:
    int Zoom = 100;
    ScenarioHandler();

    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;


    //CefClient
    bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) OVERRIDE;

    //Events
    std::vector<std::function<void(CefRefPtr<CefBrowser>, CefProcessId, CefRefPtr<CefProcessMessage>, bool*)> > EventProcessMessage;


    virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack,bool canGoForward) OVERRIDE;


private:
    IMPLEMENT_REFCOUNTING(ScenarioHandler);

};

#endif // SCENARIOHANDLER_H
