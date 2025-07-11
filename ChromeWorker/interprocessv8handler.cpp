#include "interprocessv8handler.h"
#include "log.h"

InterprocessV8Handler::InterprocessV8Handler(const std::string& prefix)
{
    this->prefix = prefix;
}

bool InterprocessV8Handler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
    if(name == std::string("browser_automation_studio_eval"))
    {
        if (arguments.size() == 1 && arguments[0]->IsString())
        {
            std::string script = arguments[0]->GetStringValue().ToString();
            CefRefPtr<CefV8Context> Context = CefV8Context::GetCurrentContext();
            if(Context)
            {
                CefRefPtr<CefFrame> Frame = Context->GetFrame();
                if(Frame)
                {
                    WORKER_LOG(std::string("EXEC<<") + script);
                    Frame->ExecuteJavaScript(script, "",1);
                }
            }

        }
        return true;
    }

    CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(std::string("V8HandlerMessage"));

    msg->GetArgumentList()->SetSize(arguments.size() + 2);
    msg->GetArgumentList()->SetString(0,prefix);
    msg->GetArgumentList()->SetString(1,name);

    //WORKER_LOG(std::string("-> OnProcessMessageReceived<<V8HandlerMessage<<") + prefix + std::string("<<") + name.ToString());

    for(int i = 0;i<arguments.size();i++)
    {
        if(arguments.at(i)->IsString())
        {
            msg->GetArgumentList()->SetString(i+2,arguments.at(i)->GetStringValue());
            //WORKER_LOG(std::string("     STRING<<") + arguments.at(i)->GetStringValue().ToString());

        }else if(arguments.at(i)->IsInt())
        {
            msg->GetArgumentList()->SetInt(i+2,arguments.at(i)->GetIntValue());
            //WORKER_LOG(std::string("     INT<<") + std::to_string(arguments.at(i)->GetIntValue()));

        }else if(arguments.at(i)->IsBool())
        {
            msg->GetArgumentList()->SetBool(i+2,arguments.at(i)->GetBoolValue());
            //WORKER_LOG(std::string("     BOOL<<") + std::to_string(arguments.at(i)->GetBoolValue()));

        }else if(arguments.at(i)->IsDouble())
        {
            msg->GetArgumentList()->SetDouble(i+2,arguments.at(i)->GetDoubleValue());
            //WORKER_LOG(std::string("     DOUBLE<<") + std::to_string(arguments.at(i)->GetDoubleValue()));

        }else if(arguments.at(i)->IsNull())
        {
            msg->GetArgumentList()->SetNull(i+2);
            //WORKER_LOG(std::string("     NULL<<"));

        }else if(arguments.at(i)->IsUndefined())
        {
            msg->GetArgumentList()->SetNull(i+2);
            //WORKER_LOG(std::string("     NULL<<"));

        }else
        {
            //WORKER_LOG(std::string("     UNKNOWN<<"));
        }

    }

    CefRefPtr<CefV8Context> Context = CefV8Context::GetEnteredContext();
    if(Context)
    {
        CefRefPtr<CefFrame> Frame = Context->GetFrame();
        if(Frame)
        {
            Frame->SendProcessMessage(PID_BROWSER, msg);
        }
    }

    return true;
}
