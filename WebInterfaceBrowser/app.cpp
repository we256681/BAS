#include "app.h"
#include <string>
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "handler.h"
#include "TrayIcon.h"

class WindowDelegate : public CefWindowDelegate {
 public:
  explicit WindowDelegate(CefRefPtr<CefBrowserView> browser_view)
      : browser_view_(browser_view) {}

  void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE {
    window->AddChildView(browser_view_);
    window->Show();
    browser_view_->RequestFocus();
  }

  void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE {
    browser_view_ = NULL;
  }

  bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE {
    CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
    if (browser)
      return browser->GetHost()->TryCloseBrowser();
    return true;
  }

 private:
  CefRefPtr<CefBrowserView> browser_view_;

  IMPLEMENT_REFCOUNTING(WindowDelegate);
  DISALLOW_COPY_AND_ASSIGN(WindowDelegate);
};



App::App() {}

void App::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();

  CefRefPtr<CefCommandLine> command_line =CefCommandLine::GetGlobalCommandLine();

  CefRefPtr<Handler> handler(new Handler());
  CefBrowserSettings browser_settings;
  std::string url;
  url = command_line->GetSwitchValue("url");
  if (url.empty())
    url = "http://www.google.com";


    CefWindowInfo window_info;
    std::string Title = "Application";
    if(command_line->HasSwitch("title"))
    {
        Title = command_line->GetSwitchValue("title");
    }

    if(command_line->HasSwitch("silent"))
    {
        window_info.SetAsPopup(NULL, Title);
        window_info.style &= ~WS_VISIBLE;
    }else
    {
        window_info.SetAsPopup(NULL, Title);
    }

    if(command_line->HasSwitch("x") && command_line->HasSwitch("y"))
    {
        std::string xString = command_line->GetSwitchValue("x");
        int x = 10000;
        try
        {
            x = std::stoi(xString);
        }catch(...)
        {

        }

        std::string yString = command_line->GetSwitchValue("y");
        int y = 10000;
        try
        {
            y = std::stoi(yString);
        }catch(...)
        {

        }

        if(x < 10000 && y < 10000)
        {
            window_info.x = x;
            window_info.y = y;
        }
    }

    if(command_line->HasSwitch("width") && command_line->HasSwitch("height"))
    {
        std::string widthString = command_line->GetSwitchValue("width");
        int width = 10000;
        try
        {
            width = std::stoi(widthString);
        }catch(...)
        {

        }

        std::string heightString = command_line->GetSwitchValue("height");
        int height = 10000;
        try
        {
            height = std::stoi(heightString);
        }catch(...)
        {

        }

        if(width < 10000 && height < 10000)
        {
            window_info.width = width;
            window_info.height = height;
        }
    }

    CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(window_info, handler, url, browser_settings, CefDictionaryValue::Create(), NULL);
    HWND hwnd = browser->GetHost()->GetWindowHandle();

    if(command_line->HasSwitch("debug"))
    {
        CefWindowInfo window_info;
        CefBrowserSettings browser_settings;

        window_info.SetAsPopup(NULL, "");
        browser->GetHost()->ShowDevTools(window_info,NULL,browser_settings,CefPoint(0,0));
    }

    IconListener *_IconListener = new IconListener();
    _IconListener->hwnd = hwnd;
    TrayIcon->SetListener(_IconListener);
    if(!command_line->HasSwitch("silent"))
    {
        _IconListener->Show();
    }


    if(command_line->HasSwitch("iconsmall"))
    {
        HICON hicon = (HICON) LoadImage(
          NULL,
          command_line->GetSwitchValue("iconsmall").ToWString().c_str(),
          IMAGE_ICON,
          0,
          0,
          LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_SHARED
        );
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hicon);
        //TrayIcon->SetName((const char* )L".");
        TrayIcon->SetIcon(hicon);
        TrayIcon->SetVisible(true);
    }
    if(command_line->HasSwitch("iconbig"))
    {
        HICON hicon = (HICON) LoadImage(
          NULL,
          command_line->GetSwitchValue("iconbig").ToWString().c_str(),
          IMAGE_ICON,
          0,
          0,
          LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_SHARED
        );
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hicon);
    }

    handler->SetTrayIcon(TrayIcon);
    handler->SetIconListener(_IconListener);
}
