#ifndef APP_H
#define APP_H

#include "TrayIcon.h"
#include "include/cef_app.h"


class App : public CefApp, public CefBrowserProcessHandler {
 public:
  App();

  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE
  {
    return this;
  }
  void SetTrayIcon(CTrayIcon *TrayIcon)
  {
      this->TrayIcon = TrayIcon;
  }

  virtual void OnContextInitialized() OVERRIDE;

 private:
  CTrayIcon *TrayIcon;

  IMPLEMENT_REFCOUNTING(App);
};

#endif // APP_H
