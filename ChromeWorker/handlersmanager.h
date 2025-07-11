#ifndef HANDLERSMANAGER_H
#define HANDLERSMANAGER_H

#include <vector>
#include <map>
#include <memory>
#include "include/cef_app.h"
#include "mainhandler.h"
#include "inspectresult.h"
#include "localstoragedata.h"

class MainHandler;

class HandlersManager
{
    struct HandlerUnitClass
    {
        CefRefPtr<MainHandler> Handler;
        CefRefPtr<CefBrowser> Browser;
        int BrowserId = -1;
        bool IsActive = true;
        bool DontUseAsActive = false;
        bool IsContextCreated = false;
        bool ForceShow = false;
    };
    using HandlerUnit = std::shared_ptr<HandlerUnitClass>;

    std::vector<HandlerUnit> HandlerUnits;
    HandlerUnit OriginalHandler;

    std::map<int,int> MapBrowserIdToTabNumber;
    std::map<int,std::string> MapBrowserIdNextActivation;

    /* Callbacks */
    void PopupCreated(CefRefPtr<MainHandler> new_handler,CefRefPtr<CefBrowser> new_browser);
    void PopupRemoved(int BrowserId);
    void LinkCtrlClick(const std::string& current_url, const std::string& target_url, int BrowserId);
    void DownloadStarted();
    void ComboboxOpened();
    void UploadStarted();
    void StartRequest(CefRefPtr<CefRequest> Request, int BrowserId);
    void SendTextResponce(const std::string& text, int BrowserId);
    void ProcessMessage(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message, bool* is_processed);
    void UrlLoaded(const std::string& url, int status, int BrowserId, int RequestResourceType);
    void LoadSuccess(int BrowserId);
    void CursorChanged(int CursorId);
    void OldestRequestTimeChanged(int64 OldestTime, int BrowserId);


    int CurrentBrowserId = -1;
    int DevToolsBorwserId = -1;


    CefRefPtr<MainHandler> Handler;
    CefRefPtr<CefBrowser> Browser;

    bool IsVisible = false;

    std::function<void(const std::string&)> SendTextResponceCallback;
    std::function<void(const std::string&, int, int)> UrlLoadedCallback;
    std::function<void()> LoadSuccessCallback;
    std::function<void(int)> CursorChangedCallback;
    std::function<void(CefRefPtr<CefRequest>)> StartRequestCallback;
    std::function<void(CefRefPtr<CefBrowser>, CefProcessId, CefRefPtr<CefProcessMessage>, bool*)> ProcessMessageCallback;
    std::function<void(char*,int,int)> PaintCallback;
    std::function<void(int64)> OldestRequestTimeChangedCallback;
    std::function<void()> DownloadStartCallback;
    std::function<void()> ComboboxCallback;
    std::function<void()> UploadStartCallback;
    std::function<void(const std::string&, const std::string&)> LinkCtrlClickCallback;
    std::function<void()> CurrentTabChangedCallback;


    std::vector<int> NewContextCreatedIds;

    void UpdateCurrent();

    bool IsClosedCurrent = false;
    bool IsWaitForClosedCurrent = false;

    void UpdateMapBrowserIdToTabNumber();

    std::string FrameStructureQueryId;
    int64 FrameStructureQueryResult;
    bool FrameStructureQueryTakesPlace = false;
    int FrameStructureQueryContinuous = 0;

    std::string UniqueProcessId;

public:

    void Reset();
    void Timer();
    MainHandler* GetHandler();
    CefBrowser* GetBrowser();
    int64 FindFrameId(const FrameInspectResult& Inspect);
    void SetFrameStructureQuery(const std::string& FrameStructureQueryId, int64 FrameStructureQueryResult);
    void NewContextCreated(int ContextId);
    void Init1(CefRefPtr<MainHandler> Handler,
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
               std::function<void()> CurrentTabChangedCallback);
    void Init2(CefRefPtr<CefBrowser> Browser);
    void SetUniqueProcessId(const std::string & UniqueProcessId);

    std::vector<std::function<void()> > EventNeedToCloseDevTools;
    void SetDevToolsBorwserId(int DevToolsBorwserId);

    std::vector<std::string> GetAllUrls();
    std::string GetTabsJson();
    int GetActiveIndex();
    void SendToAll(CefRefPtr<CefProcessMessage> Message);
    void CloseDevToolsAll();
    bool CloseByIndex(int index);
    void SwitchByIndex(int index);
    void SetUrlToOpenOnNextActivation(int BrowserId, std::string& url);
    bool CheckIsClosed();
    int FindTabIdByBrowserId(int BrowserId);
    CefResourceRequestHandler* GetHandlerForBrowserId(int BrowserId);

    void UpdateLocalStorageItem(const LocalStorageDataItem& item);
    void UpdateLocalStorageString(const std::string& data);

    void Show();
    void Hide();
    bool GetIsVisible();

};

#endif // HANDLERSMANAGER_H
