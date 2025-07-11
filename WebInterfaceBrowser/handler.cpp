#include "handler.h"
#include <windows.h>
#include "include/cef_browser.h"
#include <sstream>
#include <string>
#include "filesystem.h"
#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "include/cef_client.h"

Handler* g_instance = NULL;

Handler::Handler(): is_closing_(false) {
  DCHECK(!g_instance);
  g_instance = this;
}

Handler::~Handler() {
  g_instance = NULL;
}

Handler* Handler::GetInstance() {
  return g_instance;
}

void Handler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  browser_list_.push_back(browser);
}

bool Handler::DoClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  if(!_IconListener->NeedToClose)
  {
      TrayIcon->ShowBalloonTooltip(L"Application is still running.", L"Use exit menu item to shut it down.", CTrayIcon::eTI_Info);
      ShowWindow(_IconListener->hwnd,SW_HIDE);
      return true;
  }
  //return true;
  // Closing the main window requires special handling. See the DoClose()
  // documentation in the CEF header for a detailed destription of this
  // process.
  if (browser_list_.size() == 1) {
    // Set a flag to indicate that the window close should be allowed.
    is_closing_ = true;
  }

  // Allow the close. For windowed browsers this will result in the OS close
  // event being sent.
  return false;
}

void Handler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Remove from the list of existing browsers.
  BrowserList::iterator bit = browser_list_.begin();
  for (; bit != browser_list_.end(); ++bit) {
    if ((*bit)->IsSame(browser)) {
      browser_list_.erase(bit);
      break;
    }
  }

  if (browser_list_.empty()) {
    // All browser windows have closed. Quit the application message loop.
    CefQuitMessageLoop();
  }
}

void Handler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                ErrorCode errorCode,
                                const CefString& errorText,
                                const CefString& failedUrl) {
  CEF_REQUIRE_UI_THREAD();

  if (errorCode == ERR_ABORTED)
    return;

  std::wstringstream ss;
  ss << L"<html><body bgcolor=\"white\">"
        L"<h2>Failed to load application with error "
     << errorText.ToWString()
     << L" (" << errorCode << L").</h2></body></html>";
  //frame->LoadString(ss.str(), failedUrl);

  std::wstring ErrorString = ss.str();
  FileSystem _FileSystem;
  _FileSystem.WriteFileAsBinary(L"error.html",std::vector<char>(ErrorString.begin(),ErrorString.end()), false);
}

void Handler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                  bool isLoading,
                                  bool canGoBack,
                                  bool canGoForward)
{
    /*if(!isLoading)
    {
        HWND cefHandle = browser->GetHost()->GetWindowHandle();
        HDC hdc = GetDC(cefHandle);
        int ppix = GetDeviceCaps(hdc, LOGPIXELSX);
        ReleaseDC(cefHandle, hdc);

        if (ppix > 96)
        {
            browser->GetHost()->SetZoomLevel((ppix - 96) / 24);
        }
    }*/
}

void Handler::CloseAllBrowsers(bool force_close) {
  if (!CefCurrentlyOn(TID_UI)) {
    // Execute on the UI thread.
    CefPostTask(TID_UI, base::Bind(&Handler::CloseAllBrowsers, this,
                                   force_close));
    return;
  }

  if (browser_list_.empty())
    return;

  BrowserList::const_iterator it = browser_list_.begin();
  for (; it != browser_list_.end(); ++it)
    (*it)->GetHost()->CloseBrowser(force_close);
}

bool Handler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
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
                           bool* no_javascript_access)
{
    ShellExecute(NULL, L"open", target_url.ToWString().c_str(), NULL, NULL, SW_SHOWNORMAL);
    return true;
}

bool Handler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefProcessId source_process,
                                      CefRefPtr<CefProcessMessage> message)
{
    if(message->GetName() == "CloseApplication")
    {
        _IconListener->Close();
        return true;
    }
    if(message->GetName() == "MoveWindow")
    {
        int X = message->GetArgumentList()->GetInt(0);
        int Y = message->GetArgumentList()->GetInt(1);
        SetWindowPos(_IconListener->hwnd, 0, X, Y, -1, -1, SWP_NOSIZE);
        return true;
    }
    if(message->GetName() == "ResizeWindow")
    {
        int Width = message->GetArgumentList()->GetInt(0);
        int Height = message->GetArgumentList()->GetInt(1);
        SetWindowPos(_IconListener->hwnd, 0, -1, -1, Width, Height, SWP_NOMOVE);
        return true;
    }
    if(message->GetName() == "ShowNotification")
    {
        std::wstring Title = message->GetArgumentList()->GetString(0);
        std::wstring Text = message->GetArgumentList()->GetString(1);

        TrayIcon->ShowBalloonTooltip(Title, Text, CTrayIcon::eTI_Info);

        return true;
    }
    if(message->GetName() == "OpenUrl")
    {
        std::wstring Url = message->GetArgumentList()->GetString(0);
        if(message->GetArgumentList()->GetSize() > 1)
        {
            std::wstring Params = message->GetArgumentList()->GetString(1);
            ShellExecute(NULL, L"open", Url.c_str(), Params.c_str(), NULL, SW_SHOWNORMAL);
        }else
        {
            ShellExecute(NULL, L"open", Url.c_str(), NULL, NULL, SW_SHOWNORMAL);
        }

        return true;
    }
    if(message->GetName() == "ToggleVisibility")
    {
        if(IsWindowVisible(_IconListener->hwnd))
        {
            ShowWindow(_IconListener->hwnd, SW_HIDE);
        }else
        {
            _IconListener->Show();
        }
        return true;
    }
    return false;
}

void Handler::OnBeforeDownload(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefDownloadItem> download_item,
    const CefString& suggested_name,
    CefRefPtr<CefBeforeDownloadCallback> callback)
{
    callback->Continue("", true);
}


void Handler::OnDownloadUpdated(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefDownloadItem> download_item,
    CefRefPtr<CefDownloadItemCallback> callback)
{
    if(download_item->IsComplete())
    {
        ShellExecute(NULL, L"open", download_item->GetFullPath().ToWString().c_str(), NULL, NULL, SW_SHOWNORMAL);
    }
}
