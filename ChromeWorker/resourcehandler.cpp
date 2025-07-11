#include "resourcehandler.h"
#include "log.h"
#include "converter.h"

ResourceHandler::ResourceHandler(BrowserData * _BrowserData)
{
    this->_BrowserData = _BrowserData;
    ReadOffset = 0;
}



bool ResourceHandler::ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback)
{
    url = request->GetURL().ToString();

    Client = new URLRequestClient();
    Client->Init(callback,request->GetURL().ToString(),_BrowserData);
    Client->Done.push_back(std::bind(&ResourceHandler::ResponceDoneCallback,this));

    //Need to copy request, because original is read only
    CefRefPtr<CefRequest> request_copy = CefRequest::Create();
    request_copy->SetFirstPartyForCookies(request->GetFirstPartyForCookies());
    request_copy->SetFlags(request->GetFlags() | UR_FLAG_ALLOW_STORED_CREDENTIALS);

    CefRequest::HeaderMap ReqestHeaderMap;
    request->GetHeaderMap(ReqestHeaderMap);
    request_copy->SetHeaderMap(ReqestHeaderMap);

    request_copy->SetMethod(request->GetMethod());
    request_copy->SetPostData(request->GetPostData());
    request_copy->SetURL(request->GetURL());

    urlrequest = CefURLRequest::Create(request_copy,Client,NULL);
    //urlrequest = CefURLRequest::Create(request,Client,NULL);
    return true;
}

void ResourceHandler::ResponceDoneCallback()
{

}

void ResourceHandler::GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl)
{
    WORKER_LOG(std::string("GetResponseHeaders<<") + url);
    CefRefPtr<CefResponse> r = Client->GetResponse();
    response->SetStatus(r->GetStatus());
    if(!r->GetStatusText().empty())
        response->SetStatusText(r->GetStatusText());
    CefResponse::HeaderMap headerMap;
    r->GetHeaderMap(headerMap);
    response->SetHeaderMap(headerMap);
    response_length = Client->GetData().size();
    if(!r->GetMimeType().empty())
        response->SetMimeType(r->GetMimeType());
    WORKER_LOG("GetResponseHeaders>>");
}

bool ResourceHandler::ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback)
{
    WORKER_LOG(std::string("ReadResponse<<") + url);
    std::vector<char> data = Client->GetData();

    if(ReadOffset < data.size())
    {
        int bytes_written = data.size() - ReadOffset;
        if(bytes_written > bytes_to_read)
            bytes_written = bytes_to_read;

        memcpy(data_out,data.data() + ReadOffset,bytes_written);
        ReadOffset += bytes_written;
        bytes_read = bytes_written;
        WORKER_LOG("ReadResponse>>");
        return true;
    }

    for(auto f:Done)
        f(this);
    WORKER_LOG("ReadResponse>> >>");
    return false;
}

void ResourceHandler::Cancel()
{
    WORKER_LOG("Cancel>>");

    urlrequest->Cancel();
    for(auto f:Done)
        f(this);

    WORKER_LOG("Cancel<<");
}
