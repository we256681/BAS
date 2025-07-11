#ifndef CENTRALHANDLER_H
#define CENTRALHANDLER_H

#include "include/cef_client.h"
#include <functional>


class CentralHandler : public CefClient
{
public:
    CentralHandler();

    //CefClient
    bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) OVERRIDE;

    //Events
    std::vector<std::function<void(CefRefPtr<CefBrowser>, CefProcessId, CefRefPtr<CefProcessMessage>, bool*)> > EventProcessMessage;


    IMPLEMENT_REFCOUNTING(CentralHandler);

};

#endif // CENTRALHANDLER_H
