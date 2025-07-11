#ifndef RENDERAPP_H
#define RENDERAPP_H

#include "include/cef_app.h"
#include "interprocessv8handler.h"

class RenderApp: public CefApp, public CefRenderProcessHandler
{

    /* Input parameters */
    bool IsRecord;
    int BrowserToolboxId = -1;
    int BrowserScenarioId = -1;
    int BrowserCentralId = -1;
    int BrowserDetectorId = -1;
    int Zoom;
    std::string ApplicationEngineVersion;
    std::string ScriptEngineVersion;
    std::string InterfaceState;
    std::string Lang;
    std::string Jscode;
    std::string UniqueProcessId;


    /*Handlers*/
    CefRefPtr<InterprocessV8Handler> v8handler;
    CefRefPtr<InterprocessV8Handler> toolboxv8handler;
    CefRefPtr<InterprocessV8Handler> scenariov8handler;
    CefRefPtr<InterprocessV8Handler> central8handler;
    CefRefPtr<InterprocessV8Handler> detectorv8handler;

public:
    RenderApp();

    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE;

    //CefRenderProcessHandler
    virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDictionaryValue> extra_info) OVERRIDE;
    virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
    virtual void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) OVERRIDE;
    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) OVERRIDE;

private:
    IMPLEMENT_REFCOUNTING(RenderApp);
};

#endif // RENDERAPP_H
