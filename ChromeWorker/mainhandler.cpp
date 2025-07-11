#include "mainhandler.h"
#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "match.h"
#include "multithreading.h"
#include "replaceall.h"
#include "startwith.h"
#include "randomid.h"
#include "startwith.h"
#include "picojson.h"
#include "browsereventsemulator.h"
#include "resourcehandler.h"
#include "preparestartupscript.h"
#include "languagemanager.h"
#include "browsercontextmenu.h"
#include <chrono>
#include "converter.h"
#include "fileutils.h"

using namespace std::placeholders;
using namespace std::chrono;

MainHandler::MainHandler()
{
    WORKER_LOG(std::string("MainHandlerCreaate<<") + std::to_string((int)this));

    NeedQuit = false;
    WaitForLoadEvent = false;
    Browser = 0;
    IsVisible = false;
    IsPopup = false;
    ConfirmResultTime = -1;
    ConfirmResultWait = false;
    NeedToSendFailLoadError = false;
}

bool MainHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
    bool IsProcessed = false;
    for(auto f:EventProcessMessage)
        f(browser, source_process, message, &IsProcessed);
    return IsProcessed;
}

void MainHandler::SetHandlersManager(HandlersManager *_HandlersManager)
{
    this->_HandlersManager = _HandlersManager;
}

int MainHandler::GetBrowserId()
{
    if(!Browser)
        return -1;
    return Browser->GetIdentifier();
}


void MainHandler::SetData(BrowserData *Data)
{
    this->Data = Data;
}

void MainHandler::SetDirectControl(BrowserDirectControl *DirectControl)
{
    this->DirectControl = DirectControl;
}

void MainHandler::SetPostManager(PostManager *_PostManager)
{
    this->_PostManager = _PostManager;
}

void MainHandler::SetSettings(settings *Settings)
{
    this->Settings = Settings;
}

void MainHandler::SetIsPopup()
{
    this->IsPopup = true;
}

bool MainHandler::GetIsPopup()
{
    return IsPopup;
}


CefRefPtr<CefDisplayHandler> MainHandler::GetDisplayHandler()
{
    return this;
}

CefRefPtr<CefLifeSpanHandler> MainHandler::GetLifeSpanHandler()
{
    return this;
}

CefRefPtr<CefLoadHandler> MainHandler::GetLoadHandler()
{
    return this;
}

CefRefPtr<CefRequestHandler> MainHandler::GetRequestHandler()
{
    return this;
}

CefRefPtr<CefDialogHandler> MainHandler::GetDialogHandler()
{
    return this;
}

CefRefPtr<CefKeyboardHandler> MainHandler::GetKeyboardHandler()
{
    return this;
}

CefRefPtr<CefRenderHandler> MainHandler::GetRenderHandler()
{
    return this;
}

CefRefPtr<CefJSDialogHandler> MainHandler::GetJSDialogHandler()
{
    return this;
}

CefRefPtr<CefDownloadHandler> MainHandler::GetDownloadHandler()
{
    return this;
}

CefRefPtr<CefContextMenuHandler> MainHandler::GetContextMenuHandler()
{
    return this;
}

CefRefPtr<CefResourceRequestHandler> MainHandler::GetResourceRequestHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_navigation, bool is_download, const CefString& request_initiator, bool& disable_default_handling)
{
    return this;
}

bool MainHandler::RunContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model, CefRefPtr<CefRunContextMenuCallback> callback)
{
    return true;
}



void MainHandler::OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback)
{



}

void MainHandler::OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback)
{
    /*if(download_item->IsComplete())
    {
        WORKER_LOG(std::string("OnBeforeDownloadFinished<<") + download_item->GetFullPath().ToString());
        if(!Data->IsRecord && Data->ManualControl != BrowserData::Indirect)
        {
            //Highlight file in manual control and non record mode.
            std::wstring CommandLine = L"/select,\"";
            CommandLine += download_item->GetFullPath();
            CommandLine += L"\"";
            ShellExecute(0, 0, L"explorer.exe", CommandLine.c_str(), 0 , SW_SHOW );
            return;
        }
        //Place info about downloaded file to cache
        {
            LOCK_BROWSER_DATA
            {
                std::pair<std::string, int> p;
                p.first = std::string("download://") + download_item->GetOriginalUrl().ToString();
                p.second = 200;
                Data->_LoadedUrls.push_back(p);
            }
            {
                std::pair<std::string, std::shared_ptr<BrowserData::CachedItem> > p;
                std::string url = std::string("download://") + download_item->GetOriginalUrl().ToString();
                p.first = url;
                p.second = std::make_shared<BrowserData::CachedItem>();
                std::wstring path = download_item->GetFullPath().ToWString();
                path = ExtractFilenameFromPath(path);
                p.second->body.assign(path.begin(),path.end());
                Data->_CachedData.push_back(p);
            }
        }
    }*/

}

bool MainHandler::OnJSDialog(CefRefPtr<CefBrowser> browser, const CefString& origin_url, JSDialogType dialog_type, const CefString& message_text, const CefString& default_prompt_text, CefRefPtr<CefJSDialogCallback> callback, bool& suppress_message)
{
    return false;
}

void MainHandler::Timer()
{
    if(ConfirmResult.get() && ConfirmResultWait && duration_cast< milliseconds >( system_clock::now().time_since_epoch() ).count() >= ConfirmResultTime)
    {
        WORKER_LOG("SLEEP_END");
        ConfirmResult->Continue(true,"");
        ConfirmResult = 0;
        ConfirmResultWait = false;
        ConfirmResultTime = -1;
    }


    if(OpenFilePostpond)
    {
        if(clock() > OpenFileWait)
        {
            OpenFileCallback->Continue(0,OpenFileResult);
            OpenFileResult.clear();
            OpenFileCallback = 0;
            OpenFilePostpond = false;
            OpenFileWait = 0;
        }
    }

}

bool MainHandler::OnBeforeUnloadDialog(CefRefPtr<CefBrowser> browser,const CefString& message_text, bool is_reload, CefRefPtr<CefJSDialogCallback> callback)
{
    callback->Continue(true,"");
    return true;
}


bool MainHandler::OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event)
{
    /*WORKER_LOG(std::string("CefKeyEvent<<") + std::string("type<<") + std::to_string(event.type)
                + std::string("<<modifiers<<") + std::to_string(event.modifiers)
                + std::string("<<windows_key_code<<") + std::to_string(event.windows_key_code)
                + std::string("<<native_key_code<<") + std::to_string(event.native_key_code)
                + std::string("<<character<<") + std::to_string(event.character)
                + std::string("<<unmodified_character<<") + std::to_string(event.unmodified_character)
                + std::string("<<focus_on_editable_field<<") + std::to_string(event.focus_on_editable_field)
                + std::string("<<is_system_key<<") + std::to_string(event.is_system_key)
               );*/
    return false;
}

CefRefPtr<CefResourceHandler> MainHandler::GetResourceHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request)
{
    //THREAD TID_IO


    //tunneling
    std::string method = request->GetMethod().ToString();
    std::string url = request->GetURL().ToString();

    bool UseHandler = method == std::string("GET") || method == std::string("HEAD") || method == std::string("DELETE") || method == std::string("PUT");
    if(method == std::string("POST"))
    {
        UseHandler = true;
        CefRefPtr<CefPostData> post = request->GetPostData();
        if(post)
        {
            CefPostData::ElementVector elements;
            post->GetElements(elements);

            for(CefRefPtr<CefPostDataElement> el:elements)
            {
                if(el->GetType() == PDE_TYPE_FILE)
                {
                    UseHandler = false;
                    break;
                }
            }
        }
    }

    //Don't add to cache if there is no browser which originated request
    if(!browser)
    {
        UseHandler = false;
    }

    if(UseHandler)
    {
        LOCK_BROWSER_DATA
        UseHandler = false;
        for(std::pair<bool, std::string> p:Data->_CacheMask)
        {
            if(match(p.second,url))
            {
                UseHandler = p.first;
            }
        }
    }

    if(UseHandler)
    {
        ResourceHandler* h = new ResourceHandler(Data);
        CefRefPtr<CefResourceHandler> new_handler(h);
        WORKER_LOG(std::string("use cache>>") + url);
        return new_handler;
    }
    return 0;

}

int MainHandler::GetResourceListLength()
{
    LOCK_BROWSER_DATA
    return Data->_RequestList.Size();
}

void MainHandler::CleanResourceHandlerList()
{

}


bool MainHandler::GetAuthCredentials(CefRefPtr<CefBrowser> browser, const CefString& origin_url, bool isProxy, const CefString& host, int port, const CefString& realm, const CefString& scheme, CefRefPtr<CefAuthCallback> callback)
{
    //THREAD TID_IO

    if(isProxy)
        return false;
    WORKER_LOG(std::string("GetAuthCredentials<<"));
    std::string Login,Password;
    {
        LOCK_HTTP_AUTH
        Login = Data->_HttpAuthLogin;
        Password = Data->_HttpAuthPassword;
        Data->_HttpAuthLogin.clear();
        Data->_HttpAuthPassword.clear();

    }
    if(Login.size() > 0)
    {
        callback->Continue(Login,Password);
        return true;
    }

    return false;
}

bool MainHandler::OnCertificateError(CefRefPtr<CefBrowser> browser,cef_errorcode_t cert_error,const CefString& request_url,CefRefPtr<CefSSLInfo> ssl_info,CefRefPtr<CefRequestCallback> callback)
{
    WORKER_LOG(std::string("OnCertificateError<<") + request_url.ToString());

    callback->Continue(true);
    return true;
}


bool MainHandler::OnFileDialog(CefRefPtr<CefBrowser> browser, FileDialogMode mode, const CefString& title, const CefString& default_file_path, const std::vector<CefString>& accept_filters, int selected_accept_filter, CefRefPtr<CefFileDialogCallback> callback)
{
    return false;
}

void MainHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
    WORKER_LOG("OnTitleChange");

    //SetWindowText(Data->_MainWindowHandle, std::wstring(title).c_str());
}

void MainHandler::OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url)
{
    SetWindowText(Data->UrlHandler, url.ToWString().c_str());
}

void MainHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    if(browser->IsPopup() && !IsPopup)
    {
        WORKER_LOG("OnAfterCreatedBad<<" + std::to_string((int)browser->GetHost()->GetClient().get()));
        browser->GetHost()->CloseBrowser(true);
    }
    WORKER_LOG("OnAfterCreated<<" + std::to_string(browser->IsPopup()));

    this->Browser = browser;

    if(IsPopup)
    {
        Browser->GetHost()->WasResized();
        auto EventPopupCreatedCopy = EventPopupCreated;
        for(auto f: EventPopupCreatedCopy)
            f(this,browser);
    }

    if(IsVisible)
        Show();
}

bool MainHandler::GetIsVisible()
{
    return IsVisible;
}

void MainHandler::Hide()
{
    IsVisible = false;
    //RECT rect;
    //GetWindowRect(Data->MainWindowHandle,&rect);
    ShowWindow(Data->_MainWindowHandle, SW_HIDE);
    //MoveWindow(Data->MainWindowHandle,3000,3000,rect.right-rect.left,rect.bottom-rect.top,true);
}

void MainHandler::Show()
{
    IsVisible = true;
    //RECT rect;
    //GetWindowRect(Data->MainWindowHandle,&rect);
    if(!Data->IsRecord)
        MoveWindow(Data->_MainWindowHandle, rand() % 100, rand() % 100, Data->WidthBrowser + 100, Data->HeightBrowser + 140, false);

    ShowWindow(Data->_MainWindowHandle, SW_SHOW);
    //MoveWindow(Data->MainWindowHandle,0,0,rect.right-rect.left,rect.bottom-rect.top,true);
    SetForegroundWindow(Data->_MainWindowHandle);
    if(Browser)
        Browser->GetHost()->Invalidate(PET_VIEW);
}

bool MainHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
    WORKER_LOG("DoClose");
    return false;
}

void MainHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    WORKER_LOG("OnBeforeClose");


    if(IsPopup)
    {
        auto EventPopupClosedCopy = EventPopupClosed;
        for(auto f: EventPopupClosedCopy)
            f(GetBrowserId());
    }
    Browser = 0;
}

bool MainHandler::OnOpenURLFromTab(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, CefRequestHandler::WindowOpenDisposition target_disposition, bool user_gesture)
{

    if (target_disposition == WOD_NEW_BACKGROUND_TAB || target_disposition == WOD_NEW_FOREGROUND_TAB)
    {
        std::string urlall = target_url.ToString();
        std::size_t index = urlall.find("|||");
        if(index == std::string::npos)
        {
            return false;
        }

        std::string url = urlall.substr(0,index);
        std::string referrer = urlall.substr(index + 3,urlall.size() - index - 3);

        for(auto f: EventLinkCtrlClick)
            f(referrer,url,GetBrowserId());


        return true;
    }

    return false;
}

bool MainHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, const CefString& target_frame_name, CefLifeSpanHandler::WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client, CefBrowserSettings& settings, CefRefPtr<CefDictionaryValue>& extra_info, bool* no_javascript_access)
{
    return false;
}

CefResourceRequestHandler::ReturnValue MainHandler::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback)
{
    //THREAD TID_IO
    bool Accept = true;
    /*std::string url = request->GetURL().ToString();

    try
    {
        if(starts_with(url,"http://fingerprints.bablosoft.com/client.js") || starts_with(url,"https://fingerprints.bablosoft.com/client.js"))
        {
            return RV_CANCEL;
        }
    }catch(...)
    {
    }


    {
        LOCK_BROWSER_DATA
        for(std::pair<bool, std::string> p:Data->_RequestMask)
        {
            if(match(p.second,url))
            {
                Accept = p.first;
            }
        }
    }


    CefRequest::HeaderMap ReqestHeaderMap;
    request->GetHeaderMap(ReqestHeaderMap);
    int TabNumber = 0;
    if(browser)
        TabNumber = _HandlersManager->FindTabIdByBrowserId(browser->GetIdentifier());
    {
        LOCK_BROWSER_DATA

        bool WasContentEncoding = false;

        for(std::shared_ptr<std::map<std::string,std::string> > Map: Data->_Headers.MatchAll(request->GetURL().ToString(),TabNumber))
        {

            for(const auto& Header: *Map)
            {
                if(Header.first == "Referer")
                {
                    if(Header.second == "_BAS_NO_REFERRER")
                    {
                        ReqestHeaderMap.erase("Referer");

                        //Disable set referer if we are creating new tab
                        if(url != "tab://new/" && !Data->IsCreatingNewPopup)
                            request->SetReferrer("", REFERRER_POLICY_DEFAULT);
                    }else
                    {
                        ReqestHeaderMap.erase("Referer");

                        //Disable set referer if we are creating new tab
                        if(url != "tab://new/" && !Data->IsCreatingNewPopup)
                        {
                            ReqestHeaderMap.insert(std::make_pair(Header.first, Header.second));
                            request->SetReferrer(Header.second, REFERRER_POLICY_NEVER_CLEAR_REFERRER);
                        }
                    }
                }else if(Header.first == "Accept-Language")
                {
                    AcceptLanguageCombineResult CombineAcceptLanguage = CombineAcceptLanguageWithPattern(Header.second,Data->_AcceptLanguagePattern);
                    ReqestHeaderMap.erase(Header.first);
                    ReqestHeaderMap.insert(std::make_pair(Header.first, CombineAcceptLanguage.Header));
                }else
                {
                    ReqestHeaderMap.erase(Header.first);
                    ReqestHeaderMap.insert(std::make_pair(Header.first, Header.second));
                }

                if(Header.first == "Accept-Encoding")
                    WasContentEncoding = true;

            }
        }

        //Disable delete referer if we are creating new tab
        if(url != "tab://new/" && !Data->IsCreatingNewPopup)
        {
            for(std::shared_ptr<std::map<std::string,std::string> > Map: Data->_Headers.MatchAll(request->GetURL().ToString(),TabNumber))
            {
                if(Map->count("Referer") > 0 && Map->at("Referer") != "_BAS_NO_REFERRER")
                    Map->erase("Referer");
            }
        }

        if(!WasContentEncoding)
        {
            ReqestHeaderMap.erase("Accept-Encoding");
            ReqestHeaderMap.insert(std::make_pair("Accept-Encoding", "gzip, deflate, br"));
        }


    }

    request->SetHeaderMap(ReqestHeaderMap);*/

    /*if(request->GetMethod().ToString() == std::string("POST"))
    {
        CefRefPtr<CefPostData> post = request->GetPostData();
        if(!post.get())
        {
            post = CefPostData::Create();
            request->SetPostData(post);
        }
        if(post->GetElementCount() == 0)
        {
            CefRefPtr<CefPostDataElement> el = CefPostDataElement::Create();
            std::string d("none=");
            el->SetToBytes(d.size(),d.c_str());
            post->AddElement(el);
        }
    }*/


    return RV_CONTINUE;
}

void MainHandler::OnResourceLoadComplete(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response, CefResourceRequestHandler::URLRequestStatus status, int64 received_content_length)
{
    //THREAD TID_IO

    for(auto f:EventUrlLoaded)
        f(request->GetURL().ToString(),response->GetStatus(),GetBrowserId(),request->GetResourceType());
}

void MainHandler::OnResourceRedirect(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response, CefString& new_url)
{
    //THREAD TID_IO

    if(Data->IsRecordHttp)
    {
        for(auto f:EventStartRequest)
            f(request, GetBrowserId());
    }
}

void MainHandler::OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor, CursorType type, const CefCursorInfo& custom_cursor_info)
{
    //THREAD TID_UI
    if(Data->ManualControl != BrowserData::Indirect)
    {
        for(auto f:EventCursorChanged)
            f((int)type);
    }
}


bool MainHandler::OnResourceResponse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response)
{
    //THREAD TID_IO

    if(Data->IsRecordHttp)
    {

        std::string ContentType = response->GetHeaderByName("Content-Type");
        if(ContentType.empty())
        {
            ContentType = response->GetHeaderByName("content-type");
        }
        try{
            std::transform(ContentType.begin(), ContentType.end(), ContentType.begin(), ::tolower);
        }catch(...)
        {

        }
        std::string Url = request->GetURL().ToString();

        if(
                ContentType.find("css") == std::string::npos &&
                ContentType.find("image") == std::string::npos &&
                ContentType.find("font") == std::string::npos &&
                ContentType.find("video") == std::string::npos &&
                ContentType.find("audio") == std::string::npos &&
                Url.find("chrome-extension://") == std::string::npos
          )
        {
            for(auto f:EventStartRequest)
                f(request, GetBrowserId());
        }
    }

    CefResponse::HeaderMap HeaderMap;
    response->GetHeaderMap(HeaderMap);
    for (std::multimap<CefString, CefString>::iterator it=HeaderMap.begin();it!=HeaderMap.end();it++)
    {
        std::string header = it->first.ToString();
        try{std::transform(header.begin(), header.end(), header.begin(), ::tolower);}catch(...){}
        if(header == "content-type")
        {
            std::string value = it->second.ToString();
            try{std::transform(value.begin(), value.end(), value.begin(), ::tolower);}catch(...){}
            if (value.find("event-stream") != std::string::npos)
            {
                return false;
            }
        }

    }


    return false;
}


void MainHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
{
    if(frame->IsMain())
    {
        WORKER_LOG(std::string("OnLoadError<<") + errorText.ToString() + std::string("<<") + failedUrl.ToString());
        LastLoadIsError = true;
        NeedToSendFailLoadError = true;
        SetWindowText(Data->UrlHandler, frame->GetURL().ToWString().c_str());
    }
}

void MainHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
    WORKER_LOG(std::string("OnLoadEnd ") + std::to_string(frame->IsMain()) + std::string(" ") + frame->GetURL().ToString() + std::string(" ") + std::to_string(httpStatusCode) );

    if(frame->GetURL().ToString() == "chrome-error://chromewebdata/" && frame->IsMain())
    {
        frame->LoadURL(std::string("data:text/html,"));
        return;
    }

    if(NeedToSendFailLoadError)
    {
        NeedToSendFailLoadError = false;
        SendTextResponce("<Messages><Load>1</Load></Messages>");

    }

    if(frame->GetURL().ToString() == "about:blank")
    {
        Data->IsAboutBlankLoaded = true;
        return;
    }

    if(frame->GetURL().ToString() == "tab://new/")
    {
        if(Data->IsCreatingNewPopup)
        {
            //WORKER_LOG("!!!!!!PopupCreate OnLoadEnd");
            Data->IsCreatingNewPopupIsLoaded = true;
            if(Data->IsCreatingNewPopupIsContextCreated)
            {
                Data->IsCreatingNewPopup = false;
                //WORKER_LOG("!!!!!!PopupCreate Final");
                //Popup has been created, finalize

                if(Data->IsCreatingNewPopupIsLoadAfterOpen)
                {
                    if(_HandlersManager->GetBrowser() && _HandlersManager->GetBrowser()->GetMainFrame())
                        _HandlersManager->GetBrowser()->GetMainFrame()->LoadURL(Data->IsCreatingNewPopupUrl);
                }else
                {
                    SendTextResponce("<PopupCreate></PopupCreate>");

                    if(Data->IsCreatingNewPopupIsSilent)
                    {
                        //Preserve id of new tab
                        int id = _HandlersManager->GetBrowser()->GetIdentifier();

                        //Switch to old tab
                        _HandlersManager->SwitchByIndex(Data->IsCreatingNewPopupIndexBeforeChange);
                        std::string url;
                        if(_HandlersManager->GetBrowser())
                        {
                            url = _HandlersManager->GetBrowser()->GetMainFrame()->GetURL();
                        }
                        SetWindowText(Data->UrlHandler, s2ws(url).c_str());

                        //Set url to open when tab gets active
                        _HandlersManager->SetUrlToOpenOnNextActivation(id,Data->IsCreatingNewPopupUrl);

                    }
                }
            }
        }

        return;
    }

    if(frame->GetURL().ToString() == "data:text/html,chromewebdata")
    {
        return;
    }

    if(LastLoadIsError)
    {
        return;
    }


    /*if(frame->IsMain())
    {
        NETWORK_LOG(std::to_string(httpStatusCode));
        if(httpStatusCode >= 200 && httpStatusCode < 300)
        {
           SendTextResponce("<Load>0</Load>");
           for(auto f:EventLoadSuccess)
               f(GetBrowserId());
        }else
        {

            SendTextResponce("<Load>1</Load>");
        }

        {
            LOCK_BROWSER_DATA
            Data->_NextReferrer.clear();
        }

    }*/
    WORKER_LOG("Loaded Data");
}

void MainHandler::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type)
{
    WORKER_LOG(std::string("OnLoadStart ") + std::to_string(frame->IsMain()) + std::string(" ") + frame->GetURL().ToString());

    if(frame->GetURL().ToString() != "about:blank")
        LastLoadIsError = false;
}

void MainHandler::SendTextResponce(const std::string& text)
{
    for(auto f:EventSendTextResponce)
        f(text,GetBrowserId());
}

bool MainHandler::IsNeedQuit()
{
    return NeedQuit;
}

void MainHandler::CloseLastBrowser()
{
    //WORKER_LOG(std::string("CloseLastBrowser"));
    //if(!Browser)
        //return;
    //DestroyWindow(Browser->GetHost()->GetWindowHandle());
}

//CefRenderHandler
void MainHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
    WORKER_LOG(std::string("GetViewRect<<") + std::to_string(Data->WidthBrowser) + std::string("<<") + std::to_string(Data->HeightBrowser));

    rect.x = 0;
    rect.y = 0;
    rect.width = Data->WidthBrowser;
    rect.height = Data->HeightBrowser;
}

void MainHandler::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
{
    _Popup.SetPopupVisibility(show);
    if(show)
    {
        for(auto f:EventCombobox)
        {
            f();
        }
    }
}

void MainHandler::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect)
{
    _Popup.SetPopupPosition(rect.x,rect.y);
}


void MainHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height)
{

}

bool MainHandler::StartDragging(CefRefPtr<CefBrowser> browser,CefRefPtr<CefDragData> drag_data,DragOperationsMask allowed_ops,int x, int y)
{
    WORKER_LOG("StartDragging");
    if(Data->LastClickIsFromIndirectControl)
    {
        Data->IsDrag = true;
        BrowserEventsEmulator::StartDrag(browser,drag_data,allowed_ops,x,y,Data->IsTouchScreen,Data->TouchEventId,Data->IsTouchPressedAutomation);
    }else
    {
        DirectControl->StartDrag(browser,drag_data,allowed_ops,x,y);
    }
    return true;
}

void MainHandler::OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser,double x,double y)
{
    WORKER_LOG(std::string("OFFSET<<") + std::to_string(x) + std::string("<<") + std::to_string(y));
    Data->ScrollX = x;
    Data->ScrollY = y;
}
