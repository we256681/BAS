#ifndef RESOURCEHANDLER_H
#define RESOURCEHANDLER_H

#include "include/cef_app.h"
#include "include/cef_urlrequest.h"
#include "urlrequestclient.h"
#include "browserdata.h"

class ResourceHandler : public CefResourceHandler
{
public:
    std::string url;
    CefRefPtr<CefURLRequest> urlrequest;
    CefRefPtr<URLRequestClient> Client;
    int ReadOffset;
    int id;
    BrowserData * _BrowserData;
    ResourceHandler(BrowserData * _BrowserData);

    virtual bool ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) OVERRIDE;
    virtual void GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl) OVERRIDE;
    virtual bool ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback) OVERRIDE;
    virtual void Cancel() OVERRIDE;

    void ResponceDoneCallback();
    std::vector<std::function<void(ResourceHandler*)> > Done;
private:
    IMPLEMENT_REFCOUNTING(ResourceHandler);
};

#endif // RESOURCEHANDLER_H
