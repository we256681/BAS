#include "handler.h"
#include <windows.h>
#include "include/cef_browser.h"
#include <sstream>
#include <string>
#include <fstream>
#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "include/cef_client.h"


Handler::Handler()
{

}

Handler::~Handler()
{

}



void Handler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
{
    /*if(!isLoading)
    {
        HWND cefHandle = browser->GetHost()->GetWindowHandle();
        HDC hdc = GetDC(cefHandle);
        int ppix = GetDeviceCaps(hdc, LOGPIXELSX);
        ReleaseDC(cefHandle, hdc);

        if (ppix > 96)
        {
            browser->GetHost()->SetZoomLevel((ppix - 96) / 48);
        }
    }*/
}


void Handler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    CefQuitMessageLoop();
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
    //ShellExecute(NULL, L"open", target_url.ToWString().c_str(), NULL, NULL, SW_SHOWNORMAL);
    return true;
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

bool Handler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                        CefProcessId source_process,
                                        CefRefPtr<CefProcessMessage> message)
{
    if(message->GetName() == "CloseApplication")
    {
        if(hwnd)
        {
            DestroyWindow(hwnd);
        }
        return true;
    }
    if(message->GetName() == "RestartApplication")
    {
        //ShellExecute(NULL,NULL,L"../Scheduler.exe",L"--browser",NULL,SW_RESTORE);

        int DirSize = GetCurrentDirectory(0, NULL);
        std::vector<wchar_t> DirData;
        DirData.resize(DirSize);
        GetCurrentDirectory(DirSize, DirData.data());
        std::wstring DirString(DirData.begin(),DirData.end() - 1);
        DirString = DirString.substr(0, DirString.find_last_of(L"/\\"));
        DirString = DirString.substr(0, DirString.find_last_of(L"/\\"));
        DirString += std::wstring(L"/");
        DirString += message->GetArgumentList()->GetString(0).ToWString();
        DirString += std::wstring(L"/");
        std::wstring SchedulerInputs = DirString;
        DirString += std::wstring(L"Scheduler.exe");
        SchedulerInputs += L"gui/html/js/inputs.js";

        try
        {
            std::wofstream outfile(SchedulerInputs, std::ios::binary);
            if(outfile.is_open())
            {
                outfile<<L"window.SchedulerInputs = "<<message->GetArgumentList()->GetString(1).ToWString();
            }
            outfile.flush();
            outfile.close();
        }catch(...)
        {

        }

        ShellExecute(NULL,NULL,DirString.c_str(),L"--browser --input-done",NULL,SW_RESTORE);

        if(hwnd)
        {
            DestroyWindow(hwnd);
        }
        return true;
    }
}

