#include <windows.h>
#include "include/cef_sandbox_win.h"
#include "app.h"
#include <fstream>

class V8Handler : public CefV8Handler {
    CefRefPtr<CefBrowser> browser;

public:
  V8Handler(CefRefPtr<CefBrowser> browser) {this->browser = browser;}
  virtual bool Execute(const CefString& name,
                       CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& exception) OVERRIDE
  {


    if (name == "CloseApplication")
    {
      CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("CloseApplication");
      browser->GetMainFrame()->SendProcessMessage (PID_BROWSER, msg);
      return true;
    }
    if (name == "ShowApplicationLogs")
    {
        int DirSize = GetCurrentDirectory(0, NULL);
        std::vector<wchar_t> DirData;
        DirData.resize(DirSize);
        GetCurrentDirectory(DirSize, DirData.data());
        std::wstring DirString(DirData.begin(),DirData.end() - 1);
        DirString = DirString.substr(0, DirString.find_last_of(L"/\\"));
        DirString = DirString.substr(0, DirString.find_last_of(L"/\\"));
        DirString += std::wstring(L"/");
        DirString += L"scheduler_install_log.txt";


        std::string result;

        try
        {
            std::ifstream ifs(DirString, std::ios::binary|std::ios::ate);
            std::ifstream::pos_type pos = ifs.tellg();

            result.resize(pos);

            ifs.seekg(0, std::ios::beg);
            ifs.read(&result[0], pos);
        }
        catch(...)
        {

        }
        retval = CefV8Value::CreateString(result);
        return true;
    }
    if (name == "RestartApplication" && arguments.size() == 2 && arguments.at(0)->IsString() && arguments.at(1)->IsString())
    {
      CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("RestartApplication");
      msg->GetArgumentList()->SetSize(2);
      msg->GetArgumentList()->SetString(0, arguments.at(0)->GetStringValue());
      msg->GetArgumentList()->SetString(1, arguments.at(1)->GetStringValue());
      browser->GetMainFrame()->SendProcessMessage (PID_BROWSER, msg);
      return true;
    }
    return false;
  }
  IMPLEMENT_REFCOUNTING(V8Handler);
};

class AppRender : public CefApp, public CefRenderProcessHandler {
 public:
    AppRender(){}

    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE
    {
      return this;
    }

    virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE
    {
      CefRefPtr<CefV8Value> object = context->GetGlobal();

      CefRefPtr<CefV8Handler> handler = new V8Handler(browser);

      CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction("CloseApplication", handler);
      object->SetValue("CloseApplication",func, V8_PROPERTY_ATTRIBUTE_NONE);

      CefRefPtr<CefV8Value> func2 = CefV8Value::CreateFunction("RestartApplication", handler);
      object->SetValue("RestartApplication",func2, V8_PROPERTY_ATTRIBUTE_NONE);

      CefRefPtr<CefV8Value> func3 = CefV8Value::CreateFunction("ShowApplicationLogs", handler);
      object->SetValue("ShowApplicationLogs",func3, V8_PROPERTY_ATTRIBUTE_NONE);

    }

 private:

  IMPLEMENT_REFCOUNTING(AppRender);
};

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

  settings.persist_user_preferences = 0;
  settings.persist_session_cookies = 0;
  std::wstring cache = L"cache";
  cef_string_utf16_set(cache.data(),cache.size(),&settings.user_data_path,true);

  settings.no_sandbox = true;
  CefRefPtr<App> app(new App);
  CefInitialize(main_args, settings, app.get(), sandbox_info);
  CefRunMessageLoop();
  CefShutdown();
  return 0;
}
