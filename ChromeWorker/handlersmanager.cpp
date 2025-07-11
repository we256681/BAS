#include "handlersmanager.h"
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"
#include "multithreading.h"
#include "javascriptextensions.h"
#include <functional>

using namespace std::placeholders;

void HandlersManager::Init1(
        CefRefPtr<MainHandler> Handler,
        std::function<void(const std::string&)> SendTextResponceCallback,
        std::function<void(const std::string&, int, int)> UrlLoadedCallback,
        std::function<void()> LoadSuccessCallback,
        std::function<void(int)> CursorChangedCallback,
        std::function<void(int64)> OldestRequestTimeChangedCallback,
        std::function<void()> DownloadStart,
        std::function<void()> UploadStart,
        std::function<void()> Combobox,
        std::function<void(CefRefPtr<CefRequest>)> StartRequestCallback,
        std::function<void(CefRefPtr<CefBrowser>, CefProcessId, CefRefPtr<CefProcessMessage>, bool*)> ProcessMessageCallback,
        std::function<void(const std::string&, const std::string&)> LinkCtrlClickCallback,
        std::function<void()> CurrentTabChangedCallback
    )
{
    this->Handler.swap(Handler);
    this->Handler->SetHandlersManager(this);
    this->SendTextResponceCallback = SendTextResponceCallback;
    this->UrlLoadedCallback = UrlLoadedCallback;
    this->LoadSuccessCallback = LoadSuccessCallback;
    this->CursorChangedCallback = CursorChangedCallback;
    this->StartRequestCallback = StartRequestCallback;
    this->OldestRequestTimeChangedCallback = OldestRequestTimeChangedCallback;
    this->DownloadStartCallback = DownloadStart;
    this->ComboboxCallback = Combobox;
    this->UploadStartCallback = UploadStart;
    this->ProcessMessageCallback = ProcessMessageCallback;
    this->LinkCtrlClickCallback = LinkCtrlClickCallback;
    this->CurrentTabChangedCallback = CurrentTabChangedCallback;

    this->Handler->EventLoadSuccess.push_back(std::bind(&HandlersManager::LoadSuccess,this,_1));
    this->Handler->EventCursorChanged.push_back(std::bind(&HandlersManager::CursorChanged,this,_1));
    this->Handler->EventStartRequest.push_back(std::bind(&HandlersManager::StartRequest,this,_1,_2));
    this->Handler->EventProcessMessage.push_back(std::bind(&HandlersManager::ProcessMessage,this,_1,_2,_3,_4));
    this->Handler->EventSendTextResponce.push_back(std::bind(&HandlersManager::SendTextResponce,this,_1,_2));
    this->Handler->EventUrlLoaded.push_back(std::bind(&HandlersManager::UrlLoaded,this,_1,_2,_3,_4));
    this->Handler->EventOldestRequestTimeChanged.push_back(std::bind(&HandlersManager::OldestRequestTimeChanged,this,_1,_2));

    this->Handler->EventPopupClosed.push_back(std::bind(&HandlersManager::PopupRemoved,this,_1));
    this->Handler->EventLinkCtrlClick.push_back(std::bind(&HandlersManager::LinkCtrlClick,this,_1,_2,_3));
    this->Handler->EventPopupCreated.push_back(std::bind(&HandlersManager::PopupCreated,this,_1,_2));

    this->Handler->EventDownloadStart.push_back(std::bind(&HandlersManager::DownloadStarted,this));
    this->Handler->EventCombobox.push_back(std::bind(&HandlersManager::ComboboxOpened,this));
    this->Handler->EventUploadStart.push_back(std::bind(&HandlersManager::UploadStarted,this));


}

void HandlersManager::Init2(CefRefPtr<CefBrowser> Browser)
{
    OriginalHandler = std::make_shared<HandlerUnitClass>();
    OriginalHandler->Handler = this->Handler;
    OriginalHandler->Browser = Browser;
    OriginalHandler->BrowserId = Browser->GetIdentifier();
    OriginalHandler->IsActive = true;
    UpdateMapBrowserIdToTabNumber();
    UpdateCurrent();
}

void HandlersManager::SetUniqueProcessId(const std::string & UniqueProcessId)
{
    this->UniqueProcessId = UniqueProcessId;
}


void HandlersManager::UpdateCurrent()
{
    int PrevBrowserId = CurrentBrowserId;
    bool IsPopupActive = false;

    if(OriginalHandler && OriginalHandler->ForceShow)
    {
        IsPopupActive = true;
        Handler = OriginalHandler->Handler;
        Browser = OriginalHandler->Browser;
    }

    if(!IsPopupActive)
    {
        for(HandlerUnit h:HandlerUnits)
        {
            if(h->ForceShow && h->IsActive && !h->DontUseAsActive && h->IsContextCreated)
            {
                IsPopupActive = true;
                Handler = h->Handler;
                Browser = h->Browser;
            }
        }
    }

    if(!IsPopupActive)
    {
        for(HandlerUnit h:HandlerUnits)
        {
            if(h->IsActive && !h->DontUseAsActive && h->IsContextCreated)
            {
                IsPopupActive = true;
                Handler = h->Handler;
                Browser = h->Browser;
            }
        }
    }

    if(!IsPopupActive && OriginalHandler)
    {
        Handler = OriginalHandler->Handler;
        Browser = OriginalHandler->Browser;
    }

    CurrentBrowserId = -1;
    if(Browser)
        CurrentBrowserId = Browser->GetIdentifier();

    if(PrevBrowserId != CurrentBrowserId && Browser)
    {
        //WORKER_LOG("!!!!!CURRENT BROWSER ID CHANGED " + std::to_string(PrevBrowserId) + " -> " + std::to_string(CurrentBrowserId));
        Browser->GetHost()->WasHidden(false);
        Browser->GetHost()->SendFocusEvent(true);
        Browser->GetHost()->Invalidate(PET_VIEW);
        auto it = MapBrowserIdNextActivation.find(CurrentBrowserId);
        if(it != MapBrowserIdNextActivation.end())
        {
            Browser->GetMainFrame()->LoadURL(it->second);
            MapBrowserIdNextActivation.erase(it);
        }
        CurrentTabChangedCallback();

    }
}

MainHandler* HandlersManager::GetHandler()
{
    return Handler.get();
}

CefBrowser* HandlersManager::GetBrowser()
{
    return Browser.get();
}

void HandlersManager::SetFrameStructureQuery(const std::string& FrameStructureQueryId, int64 FrameStructureQueryResult)
{
    //WORKER_LOG(std::string("$$$$$$$$$$$$$$ Query result, id = ") + FrameStructureQueryId + std::string(", result = ") + std::to_string(FrameStructureQueryResult));
    if(FrameStructureQueryId == this->FrameStructureQueryId)
    {
        this->FrameStructureQueryResult = FrameStructureQueryResult;
        FrameStructureQueryTakesPlace = false;
    }
}


int64 HandlersManager::FindFrameId(const FrameInspectResult& Inspect)
{
    if(!Inspect.is_frame)
        return -1;

    std::string CurrentQueryId = std::to_string(Inspect.parent_frame_id) + std::string(":") + std::to_string(Inspect.frame_index) + std::string(":") + std::to_string(Inspect.frame_depth);

    //WORKER_LOG(std::string("$$$$$$$$$$$$$$ Init query, id = ") + CurrentQueryId);

    if(FrameStructureQueryId == CurrentQueryId && !FrameStructureQueryTakesPlace)
    {
        //Result is ready
        //WORKER_LOG(std::string("$$$$$$$$$$$$$$ Query success result, id = ") + FrameStructureQueryId + std::string(", result = ") + std::to_string(FrameStructureQueryResult));
        FrameStructureQueryId.clear();
        FrameStructureQueryContinuous = 0;
        return FrameStructureQueryResult;
    }

    if(FrameStructureQueryId == CurrentQueryId && FrameStructureQueryTakesPlace)
    {
        //Result is not ready
        FrameStructureQueryContinuous ++;
        if(FrameStructureQueryContinuous > 10)
        {
            FrameStructureQueryId.clear();
            FrameStructureQueryContinuous = 0;
            //WORKER_LOG(std::string("$$$$$$$$$$$$$$ Query is working too long, id = ") + FrameStructureQueryId);
            return -1;
        }
        //WORKER_LOG(std::string("$$$$$$$$$$$$$$ Query is pending, id = ") + FrameStructureQueryId);
        return -2;
    }

    FrameStructureQueryContinuous = 0;

    //WORKER_LOG(std::string("$$$$$$$$$$$$$$ Start query, id = ") + CurrentQueryId);


    //No query, or another query takes place

    FrameStructureQueryId = CurrentQueryId;
    FrameStructureQueryTakesPlace = true;

    std::vector<int64> identifiers;
    GetBrowser()->GetFrameIdentifiers(identifiers);

    for(int64 id:identifiers)
    {
        CefRefPtr<CefFrame> Frame = GetBrowser()->GetFrame(id);

        int64 parent_id = -1;
        {
            CefRefPtr<CefFrame> Parent = GetBrowser()->GetFrame(id)->GetParent();
            if(Parent)
            {
                parent_id = Parent->GetIdentifier();
                if(Parent->GetIdentifier() == GetBrowser()->GetMainFrame()->GetIdentifier())
                    parent_id = -1;
            }
        }

        if(parent_id != Inspect.parent_frame_id)
            continue;


        //WORKER_LOG(std::string("$$$$$$$$$$$$$$ Send query to frame, id = ") + FrameStructureQueryId + std::string(" * ") + std::to_string(Frame->GetIdentifier())  + std::string(", parent_id = ") + std::to_string(Inspect.parent_frame_id) + std::string(", name = ") + Frame->GetName().ToString() + std::string(", url = ") + Frame->GetURL().ToString());


        std::string Script = std::string("(_BAS_HIDE(BrowserAutomationStudio_CurrentFrameIndex))(\"") + std::to_string(id) + std::string("\", ") + std::to_string(Inspect.frame_index) + std::string(", \"") + FrameStructureQueryId + std::string("\");");
        JavaScriptExtensions Extensions;
        Script = Extensions.ProcessJs(Script,UniqueProcessId);

        //WORKER_LOG(std::string("$$$$$$$$$$$$$$ Script = ") + Script);

        Frame->ExecuteJavaScript(Script,Frame->GetURL(),0);
    }

    return -2;
}

void HandlersManager::Timer()
{
    std::vector<int> Ids;
    {
        Ids = std::move(NewContextCreatedIds);
        NewContextCreatedIds.clear();
    }

    bool Updated = false;


    for(HandlerUnit h:HandlerUnits)
    {
        if(h->Handler.get() && h->IsActive && h->IsContextCreated)
            h->Handler->Timer();

        if(std::find(Ids.begin(), Ids.end(), h->BrowserId) != Ids.end())
        {
            h->IsContextCreated = true;
            Updated = true;
        }
    }

    if(OriginalHandler.get() && OriginalHandler->Handler.get())
        OriginalHandler->Handler->Timer();

    auto i = HandlerUnits.begin();
    while (i != HandlerUnits.end())
    {
        if(!(*i)->IsActive && (*i)->Handler->ref_count_.ref_count_ == 2 /*&& (*i)->Handler->GetResourceListLength() == 0*/)
        {
            (*i)->Browser = 0;

            MainHandler *h = (*i)->Handler.get();
            (*i)->Handler = 0;
            delete h;

            i = HandlerUnits.erase(i);

            Updated = true;

            if(IsWaitForClosedCurrent)
                IsClosedCurrent = true;
        }else
        {
            /*MainHandler *h = (*i)->Handler.get();
            CefPostTask(TID_IO, base::Bind(&MainHandler::CleanResourceHandlerList, h));*/

            ++i;
        }
    }
    if(OriginalHandler)
    {
        MainHandler *h = OriginalHandler->Handler.get();
        CefPostTask(TID_IO, base::Bind(&MainHandler::CleanResourceHandlerList, h));
    }else
    {
        MainHandler *h = Handler.get();
        CefPostTask(TID_IO, base::Bind(&MainHandler::CleanResourceHandlerList, h));
    }
    if(Updated)
    {
        UpdateMapBrowserIdToTabNumber();
        UpdateCurrent();
    }
}

void HandlersManager::Reset()
{
    for(HandlerUnit h:HandlerUnits)
    {
        h->DontUseAsActive = true;
        h->ForceShow = false;
        if(h->Browser)
        {
            h->Browser->GetHost()->CloseBrowser(true);
            //h->Browser->GetMainFrame()->ExecuteJavaScript("window.close();","",0);
        }
    }
    if(OriginalHandler)
        OriginalHandler->ForceShow = false;

    UpdateCurrent();

}

void HandlersManager::PopupCreated(CefRefPtr<MainHandler> new_handler,CefRefPtr<CefBrowser> new_browser)
{
    if(Browser)
    {
        Browser->GetHost()->SendFocusEvent(false);
        //Browser->GetHost()->WasHidden(true);
    }
    HandlerUnit p = std::make_shared<HandlerUnitClass>();
    p->Handler = new_handler;
    p->Browser = new_browser;
    p->BrowserId = new_browser->GetIdentifier();
    p->IsActive = true;
    p->DontUseAsActive = false;


    p->Handler->EventLoadSuccess.clear();
    p->Handler->EventCursorChanged.clear();
    p->Handler->EventSendTextResponce.clear();
    p->Handler->EventUrlLoaded.clear();
    p->Handler->EventPopupClosed.clear();
    p->Handler->EventLinkCtrlClick.clear();
    p->Handler->EventPopupCreated.clear();
    p->Handler->EventOldestRequestTimeChanged.clear();
    p->Handler->EventDownloadStart.clear();
    p->Handler->EventCombobox.clear();
    p->Handler->EventUploadStart.clear();
    p->Handler->EventStartRequest.clear();
    p->Handler->EventProcessMessage.clear();


    p->Handler->EventLoadSuccess.push_back(std::bind(&HandlersManager::LoadSuccess,this,_1));
    p->Handler->EventCursorChanged.push_back(std::bind(&HandlersManager::CursorChanged,this,_1));
    p->Handler->EventStartRequest.push_back(std::bind(&HandlersManager::StartRequest,this,_1,_2));
    p->Handler->EventSendTextResponce.push_back(std::bind(&HandlersManager::SendTextResponce,this,_1,_2));
    p->Handler->EventUrlLoaded.push_back(std::bind(&HandlersManager::UrlLoaded,this,_1,_2,_3,_4));
    p->Handler->EventOldestRequestTimeChanged.push_back(std::bind(&HandlersManager::OldestRequestTimeChanged,this,_1,_2));

    p->Handler->EventDownloadStart.push_back(std::bind(&HandlersManager::DownloadStarted,this));
    p->Handler->EventCombobox.push_back(std::bind(&HandlersManager::ComboboxOpened,this));
    p->Handler->EventUploadStart.push_back(std::bind(&HandlersManager::UploadStarted,this));

    p->Handler->EventPopupClosed.push_back(std::bind(&HandlersManager::PopupRemoved,this,_1));
    p->Handler->EventLinkCtrlClick.push_back(std::bind(&HandlersManager::LinkCtrlClick,this,_1,_2,_3));
    p->Handler->EventPopupCreated.push_back(std::bind(&HandlersManager::PopupCreated,this,_1,_2));
    p->Handler->EventProcessMessage.push_back(std::bind(&HandlersManager::ProcessMessage,this,_1,_2,_3,_4));


    HandlerUnits.push_back(p);

    OriginalHandler->ForceShow = false;

    for(HandlerUnit ht:HandlerUnits)
        ht->ForceShow = false;

    UpdateMapBrowserIdToTabNumber();
    /*UpdateCurrent();*/
}

void HandlersManager::LinkCtrlClick(const std::string& current_url, const std::string& target_url, int BrowserId)
{
    if(CurrentBrowserId == BrowserId)
        LinkCtrlClickCallback(current_url,target_url);
}

void HandlersManager::PopupRemoved(int BrowserId)
{
    for(HandlerUnit h:HandlerUnits)
    {
        if(h->BrowserId == BrowserId)
        {
            h->IsActive = false;

            //if(DevToolsBorwserId == h->BrowserId)
            {
                for(auto f:EventNeedToCloseDevTools)
                    f();
            }
        }
    }
    UpdateCurrent();

}
void HandlersManager::SendTextResponce(const std::string& text, int BrowserId)
{
    if(CurrentBrowserId == BrowserId)
        SendTextResponceCallback(text);
}

void HandlersManager::ProcessMessage(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message, bool* is_processed)
{
    ProcessMessageCallback(browser, source_process, message, is_processed);
}

void HandlersManager::UrlLoaded(const std::string& url, int status, int BrowserId, int RequestResourceType)
{
    if(CurrentBrowserId == BrowserId)
        UrlLoadedCallback(url, status, RequestResourceType);
}

void HandlersManager::DownloadStarted()
{
    DownloadStartCallback();
}

void HandlersManager::ComboboxOpened()
{
    ComboboxCallback();
}

void HandlersManager::UploadStarted()
{
    UploadStartCallback();
}

void HandlersManager::LoadSuccess(int BrowserId)
{
    if(CurrentBrowserId == BrowserId)
        LoadSuccessCallback();
}

void HandlersManager::CursorChanged(int CursorId)
{
    CursorChangedCallback(CursorId);
}

void HandlersManager::StartRequest(CefRefPtr<CefRequest> Request, int BrowserId)
{
    //if(CurrentBrowserId == BrowserId)
    StartRequestCallback(Request);

}

void HandlersManager::OldestRequestTimeChanged(int64 OldestTime, int BrowserId)
{
    if(CurrentBrowserId == BrowserId)
        OldestRequestTimeChangedCallback(OldestTime);
}

void HandlersManager::NewContextCreated(int ContextId)
{
    NewContextCreatedIds.push_back(ContextId);
}

void HandlersManager::SetDevToolsBorwserId(int DevToolsBorwserId)
{
    this->DevToolsBorwserId = DevToolsBorwserId;
}

std::string HandlersManager::GetTabsJson()
{
    std::vector<std::string> urls = GetAllUrls();
    picojson::array urls_json;
    for(std::string& url:urls)
    {
        urls_json.push_back(picojson::value(url));
    }
    int index = GetActiveIndex();

    picojson::object res;

    res["urls"] = picojson::value(urls_json);
    res["index"] = picojson::value((double)index);

    return picojson::value(res).serialize();
}

std::vector<std::string> HandlersManager::GetAllUrls()
{
    std::vector<std::string> res;
    if(OriginalHandler)
    {
        auto it = MapBrowserIdNextActivation.find(OriginalHandler->Browser->GetIdentifier());
        if(it != MapBrowserIdNextActivation.end())
        {
            res.push_back(it->second);
        }else
        {
            res.push_back(OriginalHandler->Browser->GetMainFrame()->GetURL().ToString());
        }


    }
    for(HandlerUnit h:HandlerUnits)
    {
        if(!h->DontUseAsActive && h->IsContextCreated)
        {
            auto it = MapBrowserIdNextActivation.find(h->Browser->GetIdentifier());
            if(it != MapBrowserIdNextActivation.end())
            {
                res.push_back(it->second);
            }else
            {
                res.push_back(h->Browser->GetMainFrame()->GetURL().ToString());
            }
        }
    }
    return res;
}

int HandlersManager::GetActiveIndex()
{
    int res = 0;
    if(OriginalHandler)
    {
         if(OriginalHandler->BrowserId == CurrentBrowserId)
         {
            return 0;
         }
         res++;
    }


    for(HandlerUnit h:HandlerUnits)
    {
        if(!h->DontUseAsActive && h->IsContextCreated)
        {
            if(h->BrowserId == CurrentBrowserId)
                return res;
            res ++;
        }
    }
    return 0;
}

void HandlersManager::SendToAll(CefRefPtr<CefProcessMessage> Message)
{
    if(OriginalHandler)
        OriginalHandler->Browser->GetMainFrame()->SendProcessMessage(PID_RENDERER,Message);
    for(HandlerUnit h:HandlerUnits)
    {
        if(!h->DontUseAsActive && h->IsContextCreated)
        {
            h->Browser->GetMainFrame()->SendProcessMessage(PID_RENDERER,Message);
        }
    }
}

void HandlersManager::CloseDevToolsAll()
{
    if(OriginalHandler)
        OriginalHandler->Browser->GetHost()->CloseDevTools();
    for(HandlerUnit h:HandlerUnits)
    {
        if(!h->DontUseAsActive && h->IsContextCreated)
        {
            h->Browser->GetHost()->CloseDevTools();
        }
    }
}

bool HandlersManager::CloseByIndex(int index)
{
    if(index <= 0 || index - 1 >= HandlerUnits.size())
        return false;

    IsWaitForClosedCurrent = true;
    IsClosedCurrent = false;

    HandlerUnit h = HandlerUnits[index - 1];

    h->DontUseAsActive = true;
    h->ForceShow = false;
    if(h->Browser)
    {
        //h->Browser->GetMainFrame()->ExecuteJavaScript("window.close();","",0);
        h->Browser->GetHost()->CloseBrowser(true);
    }

    UpdateCurrent();
    return true;
}

void HandlersManager::SetUrlToOpenOnNextActivation(int BrowserId, std::string& url)
{
    MapBrowserIdNextActivation[BrowserId] = url;
}

void HandlersManager::SwitchByIndex(int index)
{
    if(Browser)
    {
        Browser->GetHost()->SendFocusEvent(false);
        //Browser->GetHost()->WasHidden(true);
    }

    HandlerUnit h;
    if(index <= 0)
    {
        h = OriginalHandler;
    }else if(index - 1 < HandlerUnits.size())
    {
        h = HandlerUnits[index - 1];
    }
    if(OriginalHandler)
        OriginalHandler->ForceShow = false;

    for(HandlerUnit ht:HandlerUnits)
        ht->ForceShow = false;

    if(h && !h->DontUseAsActive)
    {
        h->ForceShow = true;
    }

    UpdateCurrent();

}

bool HandlersManager::CheckIsClosed()
{
    if(IsWaitForClosedCurrent && IsClosedCurrent)
    {
        IsWaitForClosedCurrent = false;
        IsClosedCurrent = false;
        return true;
    }
    return false;
}

void HandlersManager::UpdateLocalStorageItem(const LocalStorageDataItem& item)
{
    std::vector<HandlerUnit> all = HandlerUnits;
    if(OriginalHandler)
        all.push_back(OriginalHandler);

    for(HandlerUnit h:HandlerUnits)
    {
        if(!h->DontUseAsActive && h->IsContextCreated)
        {
            all.push_back(h);
        }
    }

    std::string jscode = "try{";
    jscode += "BrowserAutomationStudio_UpdateLocalStorage(";
    jscode += picojson::value(item.TypeString).serialize();
    jscode += ",";
    jscode += picojson::value(item.Key).serialize();
    jscode += ",";
    jscode += picojson::value(item.Value).serialize();
    jscode += ",";
    jscode += picojson::value(item.Domain).serialize();
    jscode += ",";
    jscode += picojson::value(double(item.FrameHash)).serialize();
    jscode += ",";
    jscode += picojson::value(item.Time).serialize();

    jscode += ");";
    jscode += "}catch(e){};";
    for(HandlerUnit h:all)
    {
        std::vector<int64> identifiers;

        h->Browser->GetFrameIdentifiers(identifiers);
        for(int64 id:identifiers)
        {
            h->Browser->GetFrame(id)->ExecuteJavaScript(jscode,"", 0);
        }
    }
}

void HandlersManager::UpdateLocalStorageString(const std::string& data)
{
    std::vector<HandlerUnit> all = HandlerUnits;
    if(OriginalHandler)
        all.push_back(OriginalHandler);

    for(HandlerUnit h:HandlerUnits)
    {
        if(!h->DontUseAsActive && h->IsContextCreated)
        {
            all.push_back(h);
        }
    }

    std::string jscode = "try{";
    jscode += "BrowserAutomationStudio_RestoreLocalStorage(";
    jscode += picojson::value(data).serialize();
    jscode += ");";
    jscode += "}catch(e){};";
    for(HandlerUnit h:all)
    {
        std::vector<int64> identifiers;

        h->Browser->GetFrameIdentifiers(identifiers);
        for(int64 id:identifiers)
        {
            h->Browser->GetFrame(id)->ExecuteJavaScript(jscode,"", 0);
        }
    }
}

void HandlersManager::UpdateMapBrowserIdToTabNumber()
{
    LOCK_MAP_BROWSER_ID_TO_TAB_NUMBER


    MapBrowserIdToTabNumber.clear();
    int index = 0;

    if(OriginalHandler)
    {
        MapBrowserIdToTabNumber[OriginalHandler->BrowserId] = 0;
        index++;
    }

    for(HandlerUnit h:HandlerUnits)
    {
        if(h->BrowserId >= 0)
            MapBrowserIdToTabNumber[h->BrowserId] = index;
        index++;
    }

    //WORKER_LOG(std::string("UpdateMapBrowserIdToTabNumber ") + std::to_string(MapBrowserIdToTabNumber.size()));

}


int HandlersManager::FindTabIdByBrowserId(int BrowserId)
{
    LOCK_MAP_BROWSER_ID_TO_TAB_NUMBER
    //WORKER_LOG(std::string("FindTabIdByBrowserIdSize ") + std::to_string(MapBrowserIdToTabNumber.size()));
    auto it = MapBrowserIdToTabNumber.find(BrowserId);
    if(it == MapBrowserIdToTabNumber.end())
        return -1;
    else
    {
        //WORKER_LOG(std::string("FindTabIdByBrowserId ") + std::to_string(it->first));
        //WORKER_LOG(std::string("FindTabIdByBrowserId ") + std::to_string(it->second));
        return it->second;
    }

}

CefResourceRequestHandler* HandlersManager::GetHandlerForBrowserId(int BrowserId)
{
    if(OriginalHandler && OriginalHandler->Browser)
    {
        if(OriginalHandler->Browser->GetIdentifier() == BrowserId)
        {
            return OriginalHandler->Handler.get();
        }
    }

    for(HandlerUnit h:HandlerUnits)
    {
        if(h->BrowserId == BrowserId && h->Handler)
        {
            return h->Handler.get();
        }
    }
    return NULL;
}


void HandlersManager::Show()
{
    this->IsVisible = true;
}
void HandlersManager::Hide()
{
    this->IsVisible = false;
}
bool HandlersManager::GetIsVisible()
{
    return IsVisible;
}
