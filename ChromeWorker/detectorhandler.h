#ifndef DETECTORHANDLER_H
#define DETECTORHANDLER_H


#include "include/cef_client.h"
#include <functional>


class DetectorHandler : public CefClient
{
public:
    DetectorHandler();

    //CefClient
    bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) OVERRIDE;

    //Events
    std::vector<std::function<void(CefRefPtr<CefBrowser>, CefProcessId, CefRefPtr<CefProcessMessage>, bool*)> > EventProcessMessage;

private:
    IMPLEMENT_REFCOUNTING(DetectorHandler);

};

#endif // DETECTORHANDLER_H
