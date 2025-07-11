#ifndef MAINHANDLER_H
#define MAINHANDLER_H

#include "include/cef_client.h"
#include "include/cef_urlrequest.h"
#include "browserdata.h"
#include "browserdirectcontrol.h"
#include <functional>
#include "curlresourcehandler.h"
#include <atomic>
#include "settings.h"
#include "log.h"
#include "refcountpublic.h"
#include "handlersmanager.h"
#include "postmanager.h"
#include "popup.h"


class HandlersManager;

class MainHandler : public CefClient, public CefDownloadHandler, public CefDisplayHandler, public CefLifeSpanHandler, public CefLoadHandler, public CefResourceRequestHandler, public CefRequestHandler, public CefDialogHandler, public CefKeyboardHandler, public CefRenderHandler, public CefJSDialogHandler, public CefContextMenuHandler
{
    bool NeedQuit;
    bool WaitForLoadEvent;
    bool IsPopup;
    CefRefPtr<CefBrowser> Browser;
    bool IsVisible;
    BrowserData * Data;
    PostManager * _PostManager;
    settings * Settings;
    BrowserDirectControl *DirectControl;
    bool NeedToSendFailLoadError;

    int GetBrowserId();
    CefRefPtr<CefJSDialogCallback> ConfirmResult;
    long long ConfirmResultTime;
    bool ConfirmResultWait;
    HandlersManager *_HandlersManager = 0;

    //Open file delayed
    CefRefPtr<CefFileDialogCallback> OpenFileCallback;
    bool OpenFilePostpond = false;
    std::vector<CefString> OpenFileResult;
    clock_t OpenFileWait = 0;
    bool LastLoadIsError = false;
    Popup _Popup;


public:
    MainHandler();
    bool GetIsVisible();
    void SetData(BrowserData *Data);
    void SetDirectControl(BrowserDirectControl *DirectControl);
    void SetPostManager(PostManager *_PostManager);
    void SetSettings(settings *Settings);
    void SetIsPopup();
    bool GetIsPopup();
    void SetHandlersManager(HandlersManager *_HandlersManager);

    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE;
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE;
    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;
    virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE;
    virtual CefRefPtr<CefDialogHandler> GetDialogHandler() OVERRIDE;
    virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE;
    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE;
    virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() OVERRIDE;
    virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() OVERRIDE;
    virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE;
    virtual CefRefPtr<CefResourceRequestHandler> GetResourceRequestHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_navigation, bool is_download, const CefString& request_initiator, bool& disable_default_handling) OVERRIDE;

    //CefContextMenuHandler
    virtual bool RunContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model, CefRefPtr<CefRunContextMenuCallback> callback) OVERRIDE;


    //CefDownloadHandler
    virtual void OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE;
    virtual void OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE;

    // CefJSDialogHandler methods:
    virtual bool OnJSDialog(CefRefPtr<CefBrowser> browser, const CefString& origin_url, JSDialogType dialog_type, const CefString& message_text, const CefString& default_prompt_text, CefRefPtr<CefJSDialogCallback> callback, bool& suppress_message) OVERRIDE;
    virtual bool OnBeforeUnloadDialog(CefRefPtr<CefBrowser> browser,const CefString& message_text, bool is_reload, CefRefPtr<CefJSDialogCallback> callback) OVERRIDE;


    // CefKeyboardHandler methods:
    virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event) OVERRIDE;

    // CefDisplayHandler methods:
    virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) OVERRIDE;
    virtual void OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url) OVERRIDE;

    // CefLifeSpanHandler methods:
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
    virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
    virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, const CefString& target_frame_name, CefLifeSpanHandler::WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client, CefBrowserSettings& settings, CefRefPtr<CefDictionaryValue>& extra_info, bool* no_javascript_access) OVERRIDE;

    // CefLoadHandler methods:
    virtual void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) OVERRIDE;
    virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) OVERRIDE;
    virtual void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type) OVERRIDE;

    //CefResourceRequestHandler:
    virtual ReturnValue OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback) OVERRIDE;
    virtual void OnResourceLoadComplete(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response, CefResourceRequestHandler::URLRequestStatus status, int64 received_content_length) OVERRIDE;
    virtual CefRefPtr<CefResourceHandler> GetResourceHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request) OVERRIDE;
    virtual bool OnResourceResponse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response) OVERRIDE;
    virtual void OnResourceRedirect(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response, CefString& new_url) OVERRIDE;

    // CefRequestHandler methods:
    virtual bool GetAuthCredentials(CefRefPtr<CefBrowser> browser, const CefString& origin_url, bool isProxy, const CefString& host, int port, const CefString& realm, const CefString& scheme, CefRefPtr<CefAuthCallback> callback) OVERRIDE;
    virtual bool OnCertificateError(CefRefPtr<CefBrowser> browser,cef_errorcode_t cert_error,const CefString& request_url,CefRefPtr<CefSSLInfo> ssl_info,CefRefPtr<CefRequestCallback> callback) OVERRIDE;
    virtual bool OnOpenURLFromTab(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, CefRequestHandler::WindowOpenDisposition target_disposition, bool user_gesture) OVERRIDE;

    //CefDialogHandler
    virtual bool OnFileDialog(CefRefPtr<CefBrowser> browser, FileDialogMode mode, const CefString& title, const CefString& default_file_path, const std::vector<CefString>& accept_filters, int selected_accept_filter, CefRefPtr<CefFileDialogCallback> callback) OVERRIDE;

    //CefRenderHandler
    virtual void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE;
    virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) OVERRIDE;
    virtual void OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser,double x,double y) OVERRIDE;
    virtual bool StartDragging(CefRefPtr<CefBrowser> browser,CefRefPtr<CefDragData> drag_data,DragOperationsMask allowed_ops,int x, int y) OVERRIDE;
    virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) OVERRIDE;
    virtual void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) OVERRIDE;
    virtual void OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor, CursorType type, const CefCursorInfo& custom_cursor_info) OVERRIDE;


    //CefClient
    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) OVERRIDE;


    bool IsNeedQuit();
    void Hide();
    void Show();
    void Timer();
    void CloseLastBrowser();
    void CleanResourceHandlerList();
    int GetResourceListLength();

    //Events
    std::vector<std::function<void(const std::string&, int)> > EventSendTextResponce;
    std::vector<std::function<void(const std::string&, int, int, int)> > EventUrlLoaded;
    std::vector<std::function<void(int)> > EventLoadSuccess;
    std::vector<std::function<void(int)> > EventCursorChanged;
    std::vector<std::function<void()> > EventDownloadStart;
    std::vector<std::function<void()> > EventUploadStart;
    std::vector<std::function<void(CefRefPtr<CefRequest>,int)> > EventStartRequest;
    std::vector<std::function<void(char*,int,int,int)> > EventPaint;
    std::vector<std::function<void(CefRefPtr<MainHandler>,CefRefPtr<CefBrowser>)> > EventPopupCreated;
    std::vector<std::function<void(CefRefPtr<CefBrowser>, CefProcessId, CefRefPtr<CefProcessMessage>, bool*)> > EventProcessMessage;
    std::vector<std::function<void(int)> > EventPopupClosed;
    std::vector<std::function<void(const std::string&, const std::string&, int)> > EventLinkCtrlClick;
    std::vector<std::function<void()> > EventCombobox;
    std::vector<std::function<void(int64,int)> > EventOldestRequestTimeChanged;
    std::vector<CefRefPtr<CurlResourceHandler> > EventOnTimerCurlResources;
    std::atomic_int CurlResourcesLength;

private:
    void SendTextResponce(const std::string&);

private:
    IMPLEMENT_REFCOUNTING_NONE();
};

#endif // MAINHANDLER_H
