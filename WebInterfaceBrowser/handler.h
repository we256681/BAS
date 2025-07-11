#ifndef HANDLER_H
#define HANDLER_H

#include "include/cef_client.h"
#include <list>
#include "TrayIcon.h"
#include "iconlistener.h"

class Handler : public CefClient, public CefDisplayHandler, public CefLifeSpanHandler, public CefLoadHandler, public CefContextMenuHandler, public CefDownloadHandler
{
 public:
  explicit Handler();
  ~Handler();

  static Handler* GetInstance();

  void SetTrayIcon(CTrayIcon *TrayIcon)
  {
      this->TrayIcon = TrayIcon;
  }
  void SetIconListener(IconListener *_IconListener)
  {
      this->_IconListener = _IconListener;
  }
  virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() OVERRIDE
  {
    return this;
  }
  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE
  {
    return this;
  }
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE
  {
    return this;
  }
  virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE
  {
    return this;
  }

  virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   CefRefPtr<CefContextMenuParams> params,
                                   CefRefPtr<CefMenuModel> model)OVERRIDE
  {
      bool IsText = params->GetTypeFlags() & CM_TYPEFLAG_SELECTION;
      bool IsEditable = params->GetTypeFlags() & CM_TYPEFLAG_EDITABLE;
      if(!IsText && !IsEditable)
      {
        model->Clear();
      }
  }


  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this; }

  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           ErrorCode errorCode,
                           const CefString& errorText,
                           const CefString& failedUrl) OVERRIDE;

  virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                    bool isLoading,
                                    bool canGoBack,
                                    bool canGoForward)OVERRIDE;

  void CloseAllBrowsers(bool force_close);
  bool IsClosing() const { return is_closing_; }

  virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             const CefString& target_url,
                             const CefString& target_frame_name,
                             WindowOpenDisposition target_disposition,
                             bool user_gesture,
                             const CefPopupFeatures& popupFeatures,
                             CefWindowInfo& windowInfo,
                             CefRefPtr<CefClient>& client,
                             CefBrowserSettings& settings,
                             CefRefPtr<CefDictionaryValue>& extra_info,
                             bool* no_javascript_access) OVERRIDE;


  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefProcessId source_process,
                                        CefRefPtr<CefProcessMessage> message) OVERRIDE;

  virtual void OnBeforeDownload(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefDownloadItem> download_item,
      const CefString& suggested_name,
      CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE;


  virtual void OnDownloadUpdated(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefDownloadItem> download_item,
      CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE;

 private:

  typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
  BrowserList browser_list_;
  bool is_closing_;
  CTrayIcon *TrayIcon = 0;
  IconListener *_IconListener;

  IMPLEMENT_REFCOUNTING(Handler);
};



#endif // HANDLER_H
