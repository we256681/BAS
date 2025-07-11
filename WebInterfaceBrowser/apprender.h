#ifndef APPRENDER_H
#define APPRENDER_H


#include "include/cef_app.h"
#include "include/cef_parser.h"
#include "filesystem.h"
#include <boost/algorithm/string.hpp>
#include <boost/exception/diagnostic_information.hpp>

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
    if (name == "ResizeWindow" && arguments.size() == 2 && arguments.at(0)->IsInt() && arguments.at(1)->IsInt())
    {
      CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("ResizeWindow");
      msg->GetArgumentList()->SetSize(2);
      msg->GetArgumentList()->SetInt(0,arguments.at(0)->GetIntValue());
      msg->GetArgumentList()->SetInt(1,arguments.at(1)->GetIntValue());
      browser->GetMainFrame()->SendProcessMessage (PID_BROWSER, msg);
      return true;
    }
    if (name == "ReadFileAsBinaryInternal" && arguments.size() == 1 && arguments.at(0)->IsString())
    {
      try
      {
          FileSystem _FileSystem;
          std::vector<char> Result = _FileSystem.ReadFileAsBinary(arguments.at(0)->GetStringValue());
          retval = CefV8Value::CreateString(CefBase64Encode(Result.data(),Result.size()));
      }catch (const boost::exception& ex)
      {
          exception = boost::diagnostic_information(ex);
      }
      catch(const std::exception& ex)
      {
          exception = ex.what();
      }
      catch (...)
      {
          exception = "Exception during read file";
      }

      return true;
    }
    if ( (name == "WriteFileAsBinaryInternal" || name == "AppendFileAsBinaryInternal" ) && arguments.size() == 2 && arguments.at(0)->IsString() && arguments.at(1)->IsString())
    {
        try
        {
          FileSystem _FileSystem;
          CefRefPtr<CefBinaryValue> DataRef = CefBase64Decode(arguments.at(1)->GetStringValue());
          std::vector<char> Data;
          Data.resize(DataRef->GetSize());
          DataRef->GetData(Data.data(),DataRef->GetSize(),0);

          _FileSystem.WriteFileAsBinary(arguments.at(0)->GetStringValue(),Data,name == "AppendFileAsBinaryInternal");
        }catch (const boost::exception& ex)
        {
            exception = boost::diagnostic_information(ex);
        }
        catch(const std::exception& ex)
        {
            exception = ex.what();
        }
        catch (...)
        {
            exception = "Exception during write file";
        }

      return true;
    }
    if ( ( name == "WriteFileAsStringInternal" || name == "AppendFileAsStringInternal" )  && arguments.size() >= 2 && arguments.at(0)->IsString() && arguments.at(1)->IsString())
    {
        try
        {
          FileSystem _FileSystem;
          std::string Encoding = "UTF-8";
          if(arguments.size() >= 3 && arguments.at(2)->IsString())
          {
            Encoding = arguments.at(2)->GetStringValue();
          }

          _FileSystem.WriteFile(arguments.at(0)->GetStringValue(),arguments.at(1)->GetStringValue(),Encoding,name == "AppendFileAsStringInternal");
        }catch (const boost::exception& ex)
        {
            exception = boost::diagnostic_information(ex);
        }
        catch(const std::exception& ex)
        {
            exception = ex.what();
        }
        catch (...)
        {
            exception = "Exception during write file";
        }
      return true;
    }
    if (name == "ReadFile" && arguments.size() >= 1 && arguments.at(0)->IsString())
    {
        try
        {
          FileSystem _FileSystem;
          std::string Encoding = "UTF-8";
          if(arguments.size() >= 2 && arguments.at(1)->IsString())
          {
            Encoding = arguments.at(1)->GetStringValue();
          }
          std::string Result = _FileSystem.ReadFile(arguments.at(0)->GetStringValue(), Encoding);
          retval = CefV8Value::CreateString(Result);

        }catch (const boost::exception& ex)
        {
            exception = boost::diagnostic_information(ex);
        }
        catch(const std::exception& ex)
        {
            exception = ex.what();
        }
        catch (...)
        {
            exception = "Exception during read file";
        }

      return true;
    }
    if (name == "FileExists" && arguments.size() == 1 && arguments.at(0)->IsString())
    {
        try
        {
          FileSystem _FileSystem;
          bool Result = _FileSystem.FileExists(arguments.at(0)->GetStringValue());

          retval = CefV8Value::CreateBool(Result);
        }catch (const boost::exception& ex)
        {
            exception = boost::diagnostic_information(ex);
        }
        catch(const std::exception& ex)
        {
            exception = ex.what();
        }
        catch (...)
        {
            exception = "Exception during checking file stats";
        }
      return true;
    }
    if (name == "ParentFolder" && arguments.size() == 1 && arguments.at(0)->IsString())
    {
        try
        {
          FileSystem _FileSystem;
          retval = CefV8Value::CreateString(_FileSystem.ParentFolder(arguments.at(0)->GetStringValue()));
        }catch (const boost::exception& ex)
        {
            exception = boost::diagnostic_information(ex);
        }
        catch(const std::exception& ex)
        {
            exception = ex.what();
        }
        catch (...)
        {
            exception = "Exception during checking file stats";
        }
      return true;
    }
    if (name == "IsFolder" && arguments.size() == 1 && arguments.at(0)->IsString())
    {
        try
        {
          FileSystem _FileSystem;
          retval = CefV8Value::CreateBool(_FileSystem.IsFolder(arguments.at(0)->GetStringValue()));
        }catch (const boost::exception& ex)
        {
            exception = boost::diagnostic_information(ex);
        }
        catch(const std::exception& ex)
        {
            exception = ex.what();
        }
        catch (...)
        {
            exception = "Exception during checking file stats";
        }
      return true;
    }
    if (name == "CreateFolder" && arguments.size() == 1 && arguments.at(0)->IsString())
    {
        try
        {
          FileSystem _FileSystem;
          _FileSystem.CreateFolder(arguments.at(0)->GetStringValue());
        }catch (const boost::exception& ex)
        {
            exception = boost::diagnostic_information(ex);
        }
        catch(const std::exception& ex)
        {
            exception = ex.what();
        }
        catch (...)
        {
            exception = "Exception during creating folder";
        }
      return true;
    }
    if (name == "FileSizeInternal" && arguments.size() == 1 && arguments.at(0)->IsString())
    {
        try
        {
          FileSystem _FileSystem;
          auto Result = _FileSystem.FileSize(arguments.at(0)->GetStringValue());

          retval = CefV8Value::CreateString(std::to_string(Result));
        }catch (const boost::exception& ex)
        {
            exception = boost::diagnostic_information(ex);
        }
        catch(const std::exception& ex)
        {
            exception = ex.what();
        }
        catch (...)
        {
            exception = "Exception during getting file size";
        }
      return true;
    }
    if (name == "FileLastWriteTimeInternal" && arguments.size() == 1 && arguments.at(0)->IsString())
    {
        try
        {
          FileSystem _FileSystem;
          auto Result = _FileSystem.FileLastWriteTime(arguments.at(0)->GetStringValue());

          retval = CefV8Value::CreateString(std::to_string(Result));
        }catch (const boost::exception& ex)
        {
            exception = boost::diagnostic_information(ex);
        }
        catch(const std::exception& ex)
        {
            exception = ex.what();
        }
        catch (...)
        {
            exception = "Exception during getting file size";
        }
      return true;
    }
    if (name == "MoveFileOrFolder" && arguments.size() == 2 && arguments.at(0)->IsString() && arguments.at(1)->IsString())
    {
        try
        {
          FileSystem _FileSystem;
          _FileSystem.MoveFileOrFolder(arguments.at(0)->GetStringValue(), arguments.at(1)->GetStringValue());
        }catch (const boost::exception& ex)
        {
            exception = boost::diagnostic_information(ex);
        }
        catch(const std::exception& ex)
        {
            exception = ex.what();
        }
        catch (...)
        {
            exception = "Exception during move file";
        }
      return true;
    }
    if (name == "CopyFileOrFolder" && arguments.size() == 2 && arguments.at(0)->IsString() && arguments.at(1)->IsString())
    {
        try
        {
          FileSystem _FileSystem;
          _FileSystem.CopyFileOrFolder(arguments.at(0)->GetStringValue(), arguments.at(1)->GetStringValue());
        }catch (const boost::exception& ex)
        {
            exception = boost::diagnostic_information(ex);
        }
        catch(const std::exception& ex)
        {
            exception = ex.what();
        }
        catch (...)
        {
            exception = "Exception during copy file";
        }
      return true;
    }
    if (name == "RemoveFileOrFolder" && arguments.size() == 1 && arguments.at(0)->IsString())
    {
        try
        {
          FileSystem _FileSystem;
          _FileSystem.RemoveFileOrFolder(arguments.at(0)->GetStringValue());
        }catch (const boost::exception& ex)
        {
            exception = boost::diagnostic_information(ex);
        }
        catch(const std::exception& ex)
        {
            exception = ex.what();
        }
        catch (...)
        {
            exception = "Exception during deleting file";
        }
      return true;
    }
    if (name == "FolderContent" && arguments.size() == 1 && arguments.at(0)->IsString())
    {
        try
        {
          FileSystem _FileSystem;
          std::vector<std::wstring> Result = _FileSystem.FolderContent(arguments.at(0)->GetStringValue());
          CefRefPtr<CefV8Value> Value = CefV8Value::CreateArray(Result.size());
          int i = 0;
          for(std::wstring & File: Result)
          {
            Value->SetValue(i++,CefV8Value::CreateString(File));
          }
          retval = Value;
        }catch (const boost::exception& ex)
        {
            exception = boost::diagnostic_information(ex);
        }
        catch(const std::exception& ex)
        {
            exception = ex.what();
        }
        catch (...)
        {
            exception = "Exception during getting folder content";
        }
      return true;
    }
    if (name == "ShowNotification" && arguments.size() == 2 && arguments.at(0)->IsString() && arguments.at(1)->IsString())
    {
      CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("ShowNotification");
      msg->GetArgumentList()->SetSize(2);
      msg->GetArgumentList()->SetString(0,arguments.at(0)->GetStringValue());
      msg->GetArgumentList()->SetString(1,arguments.at(1)->GetStringValue());

      browser->GetMainFrame()->SendProcessMessage (PID_BROWSER, msg);
      return true;
    }
    if (name == "OpenUrl" && arguments.size() == 1 && arguments.at(0)->IsString())
    {
      CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("OpenUrl");
      msg->GetArgumentList()->SetSize(1);
      msg->GetArgumentList()->SetString(0,arguments.at(0)->GetStringValue());

      browser->GetMainFrame()->SendProcessMessage (PID_BROWSER, msg);
      return true;
    }
    if (name == "OpenUrl" && arguments.size() == 2 && arguments.at(0)->IsString() && arguments.at(1)->IsString())
    {
      CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("OpenUrl");
      msg->GetArgumentList()->SetSize(2);
      msg->GetArgumentList()->SetString(0,arguments.at(0)->GetStringValue());
      msg->GetArgumentList()->SetString(1,arguments.at(1)->GetStringValue());

      browser->GetMainFrame()->SendProcessMessage (PID_BROWSER, msg);
      return true;
    }
    if (name == "MoveWindow" && arguments.size() == 2 && arguments.at(0)->IsInt() && arguments.at(1)->IsInt())
    {
      CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("MoveWindow");
      msg->GetArgumentList()->SetSize(2);
      msg->GetArgumentList()->SetInt(0,arguments.at(0)->GetIntValue());
      msg->GetArgumentList()->SetInt(1,arguments.at(1)->GetIntValue());
      browser->GetMainFrame()->SendProcessMessage (PID_BROWSER, msg);
      return true;
    }
    if (name == "ToggleVisibility")
    {
      CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("ToggleVisibility");
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

    std::string FunctionsString(
                "CloseApplication,"
                "ToggleVisibility,"
                "MoveWindow,"
                "ResizeWindow,"
                "ShowNotification,"
                "OpenUrl,"
                "ReadFileAsBinaryInternal,"
                "ReadFile,"
                "WriteFileAsBinaryInternal,"
                "WriteFileAsStringInternal,"
                "AppendFileAsBinaryInternal,"
                "AppendFileAsStringInternal,"
                "FileExists,"
                "FileSizeInternal,"
                "FileLastWriteTimeInternal,"
                "ParentFolder,"
                "IsFolder,"
                "CreateFolder,"
                "RemoveFileOrFolder,"
                "MoveFileOrFolder,"
                "CopyFileOrFolder,"
                "FolderContent,"
    );

    std::vector<std::string> FunctionsList;
    boost::split(FunctionsList, FunctionsString, boost::is_any_of(","));

    for(std::string& FunctionString: FunctionsList)
    {
        CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(FunctionString, handler);
        object->SetValue(FunctionString,func, V8_PROPERTY_ATTRIBUTE_NONE);
    }

    CefRefPtr<CefV8Value> ApiVersionString = CefV8Value::CreateInt(1);
    object->SetValue("ApiVersion",ApiVersionString, V8_PROPERTY_ATTRIBUTE_NONE);
  }


 private:

  IMPLEMENT_REFCOUNTING(AppRender);
};
#endif // APPRENDER_H
