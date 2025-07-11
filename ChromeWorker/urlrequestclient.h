#ifndef URLREQUESTCLIENT_H
#define URLREQUESTCLIENT_H

#include "include/cef_urlrequest.h"
#include <vector>
#include <functional>
#include "browserdata.h"

class URLRequestClient : public CefURLRequestClient
{
    CefRefPtr<CefCallback> Callback;
    CefRefPtr<CefResponse> Responce;
    std::vector<char> Data;
    std::string Url;
    BrowserData * _BrowserData;
    bool TriedToAuth;
public:
    URLRequestClient();
    void Init(CefRefPtr<CefCallback> Callback,const std::string& Url,BrowserData * _BrowserData);

    virtual void OnRequestComplete(CefRefPtr<CefURLRequest> request) OVERRIDE;
    virtual void OnUploadProgress(CefRefPtr<CefURLRequest> request, int64 current, int64 total) OVERRIDE;
    virtual void OnDownloadProgress(CefRefPtr<CefURLRequest> request, int64 current, int64 total) OVERRIDE;
    virtual void OnDownloadData(CefRefPtr<CefURLRequest> request, const void* data, size_t data_length) OVERRIDE;
    virtual bool GetAuthCredentials(bool isProxy, const CefString& host, int port, const CefString& realm, const CefString& scheme, CefRefPtr<CefAuthCallback> callback) OVERRIDE;

    std::vector<char> GetData();
    CefRefPtr<CefResponse> GetResponse();

    std::vector<std::function<void()> > Done;

private:
    IMPLEMENT_REFCOUNTING(URLRequestClient);

};

#endif // URLREQUESTCLIENT_H
