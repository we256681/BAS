#ifndef NEWTABSCHEMEHANDLERFACTORY_H
#define NEWTABSCHEMEHANDLERFACTORY_H

#include "include/cef_client.h"
#include "include/cef_scheme.h"

class NewTabSchemeHandlerFactory: public CefSchemeHandlerFactory
{
public:

    virtual CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& scheme_name, CefRefPtr<CefRequest> request) OVERRIDE;
private:
    IMPLEMENT_REFCOUNTING(NewTabSchemeHandlerFactory);
};

#endif // NEWTABSCHEMEHANDLERFACTORY_H
