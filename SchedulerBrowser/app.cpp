#include "app.h"
#include <string>
#include <vector>
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "handler.h"


App::App() {}

void App::OnContextInitialized() {

    CEF_REQUIRE_UI_THREAD();

    CefRefPtr<CefCommandLine> command_line = CefCommandLine::GetGlobalCommandLine();

    CefRefPtr<Handler> handler(new Handler());
    CefBrowserSettings browser_settings;

    int DirSize = GetCurrentDirectory(0, NULL);
    std::vector<wchar_t> DirData;
    DirData.resize(DirSize);
    GetCurrentDirectory(DirSize, DirData.data());
    std::wstring DirString(DirData.begin(),DirData.end() - 1);
    DirString = std::wstring(L"file://") + DirString + std::wstring(L"/html/index.html?lang=");

    std::wstring Title;
    if(command_line->HasSwitch("lang-ru"))
    {
        DirString += L"ru";
        Title = L"\x041f\x043b\x0430\x043d\x0438\x0440\x043e\x0432\x0449\x0438\x043a";
    }else
    {
        DirString += L"en";
        Title = L"Scheduler";
    }

    CefWindowInfo window_info;

    window_info.SetAsPopup(NULL, Title);
    //window_info.style |= WS_MAXIMIZE;

    CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(window_info, handler, DirString, browser_settings, CefDictionaryValue::Create(), NULL);
    HWND hwnd = browser->GetHost()->GetWindowHandle();
    handler->hwnd = hwnd;

    {
        HICON hicon = (HICON) LoadImage(
            NULL,
            L"schedulericonsmall.ico",
            IMAGE_ICON,
            0,
            0,
            LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_SHARED
        );

        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hicon);
    }



    {
        HICON hicon = (HICON) LoadImage(
                NULL,
                L"schedulericonbig.ico",
                IMAGE_ICON,
                0,
                0,
                LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_SHARED
            );

            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hicon);
    }



}
