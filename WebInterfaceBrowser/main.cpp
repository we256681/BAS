#include <windows.h>
#include "TrayIcon.h"
#include "include/cef_sandbox_win.h"
#include "app.h"
#include "apprender.h"
#include "fileutils.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
  CefEnableHighDPISupport();
  void* sandbox_info = NULL;
  CefMainArgs main_args(hInstance);
  CefRefPtr<AppRender> app_render(new AppRender);
  int exit_code = CefExecuteProcess(main_args, app_render, sandbox_info);
  if (exit_code >= 0)
  {
    return exit_code;
  }
  CefSettings settings;
  settings.log_severity = LOGSEVERITY_DISABLE;
  settings.persist_user_preferences = 1;
  settings.persist_session_cookies = 1;
  std::wstring wencoding = GetRelativePathToExe(L"profile");
  cef_string_utf16_set(wencoding.data(),wencoding.size(),&settings.cache_path,true);
  cef_string_utf16_set(wencoding.data(),wencoding.size(),&settings.user_data_path,true);

  settings.no_sandbox = true;
  CefRefPtr<App> app(new App);
  CTrayIcon *TrayIcon = new CTrayIcon();
  app->SetTrayIcon(TrayIcon);
  CefInitialize(main_args, settings, app.get(), sandbox_info);
  CefRunMessageLoop();
  CefShutdown();
  delete TrayIcon;
  return 0;
}
