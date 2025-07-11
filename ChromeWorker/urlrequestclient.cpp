#include "urlrequestclient.h"
#include "log.h"
#include "multithreading.h"
#include "readallfile.h"

URLRequestClient::URLRequestClient()
{
    TriedToAuth = false;
}

void URLRequestClient::Init(CefRefPtr<CefCallback> Callback,const std::string& Url, BrowserData * _BrowserData)
{
    this->Callback = Callback;
    this->Url = Url;
    this->_BrowserData = _BrowserData;
}

void URLRequestClient::OnRequestComplete(CefRefPtr<CefURLRequest> request)
{
    std::string url = request->GetRequest()->GetURL().ToString();
    WORKER_LOG(std::string("OnRequestComplete<<") + url);
    Responce = request->GetResponse();
    if(!Responce)
    {
        WORKER_LOG(std::string("OnRequestComplete>>Cancel>>No responce"));
        Callback->Cancel();
        return;
    }
    WORKER_LOG(std::string("Status<<") + std::to_string(Responce->GetStatus()));
    CefResponse::HeaderMap Headers;
    Responce->GetHeaderMap(Headers);

    for(auto f:Done)
        f();

    //Save to cache
    {
        std::pair<std::string, std::shared_ptr<BrowserData::CachedItem> > p;
        p.first = url;
        p.second = std::make_shared<BrowserData::CachedItem>();
        p.second->url = url;
        p.second->status = Responce->GetStatus();
        {
            CefResponse::HeaderMap Headers;
            Responce->GetHeaderMap(Headers);
            for(const std::pair<CefString, CefString>& Header : Headers)
            {
                std::pair<std::string, std::string> NewHeader;
                NewHeader.first = Header.first.ToString();
                NewHeader.second = Header.second.ToString();
                p.second->response_headers.push_back(NewHeader);
            }
        }
        {
            CefRequest::HeaderMap Headers;
            request->GetRequest()->GetHeaderMap(Headers);
            for(const std::pair<CefString, CefString>& Header : Headers)
            {
                std::pair<std::string, std::string> NewHeader;
                NewHeader.first = Header.first.ToString();
                NewHeader.second = Header.second.ToString();
                p.second->request_headers.push_back(NewHeader);
            }
        }

        CefRefPtr<CefPostData> PostData = request->GetRequest()->GetPostData();

        /* Generate post data */
        if(PostData && PostData->GetElementCount() > 0)
        {
            CefPostData::ElementVector Elements;
            PostData->GetElements(Elements);
            for(CefRefPtr<CefPostDataElement> Element:Elements)
            {
                if(Element->GetType() == PDE_TYPE_BYTES)
                {
                    int Count = Element->GetBytesCount();
                    std::vector<char> DataTemp;
                    DataTemp.resize(Count);
                    Element->GetBytes(Count,DataTemp.data());
                    std::string Data(DataTemp.data(),Count);
                    p.second->post_data.insert(p.second->post_data.end(), Data.begin(), Data.end());
                }
                if(Element->GetType() == PDE_TYPE_FILE)
                {
                    std::string Data = ReadAllString(Element->GetFile().ToString());
                    p.second->post_data.insert(p.second->post_data.end(), Data.begin(), Data.end());
                }
            }
        }

        p.second->body.insert(p.second->body.end(),Data.begin(),Data.end());

        p.second->is_finished = true;

        p.second->is_error = request->GetRequestStatus() != UR_SUCCESS;

        p.second->error = std::to_string((int)request->GetRequestError());

        /*{
            LOCK_BROWSER_DATA
            _BrowserData->_CachedData.push_back(p);
        }*/
    }

    if(request->GetRequestStatus() == UR_SUCCESS)
    {
        WORKER_LOG(std::string("OnRequestComplete>>"));
        Callback->Continue();
    }
    else
    {
        WORKER_LOG(std::string("OnRequestComplete>>Cancel>>") + std::to_string(request->GetRequestStatus()) + std::string(">>") + std::to_string(request->GetRequestError()) );
        Callback->Cancel();
    }


}

std::vector<char> URLRequestClient::GetData()
{
    return Data;
}
CefRefPtr<CefResponse> URLRequestClient::GetResponse()
{
    return Responce;
}

void URLRequestClient::OnUploadProgress(CefRefPtr<CefURLRequest> request, int64 current, int64 total)
{
}
void URLRequestClient::OnDownloadProgress(CefRefPtr<CefURLRequest> request, int64 current, int64 total)
{
}

void URLRequestClient::OnDownloadData(CefRefPtr<CefURLRequest> request, const void* data, size_t data_length)
{
    Data.insert(Data.end(),(char *)data,(char *)data + data_length);
}

bool URLRequestClient::GetAuthCredentials(bool isProxy, const CefString& host, int port, const CefString& realm, const CefString& scheme, CefRefPtr<CefAuthCallback> callback)
{
    return false;
}
