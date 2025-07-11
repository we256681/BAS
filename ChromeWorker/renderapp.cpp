#include "renderapp.h"
#include "log.h"
#include "startwith.h"
#include "picojson.h"
#include "ipcsimple.h"

RenderApp::RenderApp()
{

}

CefRefPtr<CefRenderProcessHandler> RenderApp::GetRenderProcessHandler()
{
    return this;
}

void RenderApp::OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info)
{
    IsRecord = extra_info->GetBool(0);
    BrowserToolboxId = extra_info->GetInt(1);
    BrowserScenarioId = extra_info->GetInt(2);
    BrowserCentralId = extra_info->GetInt(3);
    Zoom = extra_info->GetInt(4);
    Lang = extra_info->GetString(5);
    Jscode = extra_info->GetString(6);
    UniqueProcessId = extra_info->GetString(7);
    worker_log_init_no_delete(IsRecord);
    ApplicationEngineVersion = extra_info->GetString(8);
    ScriptEngineVersion = extra_info->GetString(9);
    InterfaceState = extra_info->GetString(10);
}

bool RenderApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
    if(message->GetName().ToString() == std::string("StartupScriptUpdated"))
    {
        std::string Script = message->GetArgumentList()->GetString(0);
        //WORKER_LOG("RenderApp::OnProcessMessageReceived::StartupScriptUpdated<<" + Script);
        Jscode = Script;

        return true;
    }

    return false;
}

void RenderApp::OnBrowserCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDictionaryValue> extra_info)
{
    //Ensure, that context always exists.
    CefRefPtr<CefFrame> frame = browser->GetMainFrame();
    if(frame)
        frame->ExecuteJavaScript(";",browser->GetMainFrame()->GetURL(),0);
}

void RenderApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
    std::string url = frame->GetURL().ToString();

    //Toolbox Browser
    if(starts_with(url, "file:///html/toolbox"))
    {
        WORKER_LOG("OnContextCreated<<ToolboxBrowser");
        if(frame->IsMain())
        {
            toolboxv8handler = new InterprocessV8Handler("Toolbox");
            CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("NewToolboxBrowserContextCreated");
            frame->SendProcessMessage(PID_BROWSER, msg);
        }

        CefRefPtr<CefV8Value> object = context->GetGlobal();
        object->SetValue("BrowserAutomationStudio_SendEmbeddedData", CefV8Value::CreateFunction("BrowserAutomationStudio_SendEmbeddedData", toolboxv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_Append", CefV8Value::CreateFunction("BrowserAutomationStudio_Append", toolboxv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_Initialized", CefV8Value::CreateFunction("BrowserAutomationStudio_Initialized", toolboxv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_EditCancel", CefV8Value::CreateFunction("BrowserAutomationStudio_EditCancel", toolboxv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_Maximize", CefV8Value::CreateFunction("BrowserAutomationStudio_Maximize", toolboxv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_Minimize", CefV8Value::CreateFunction("BrowserAutomationStudio_Minimize", toolboxv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_OpenUrl", CefV8Value::CreateFunction("BrowserAutomationStudio_OpenUrl", toolboxv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_SaveInterfaceState", CefV8Value::CreateFunction("BrowserAutomationStudio_SaveInterfaceState", toolboxv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_ClearHighlight", CefV8Value::CreateFunction("BrowserAutomationStudio_ClearHighlight", toolboxv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_Interrupt", CefV8Value::CreateFunction("BrowserAutomationStudio_Interrupt", toolboxv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_Execute", CefV8Value::CreateFunction("BrowserAutomationStudio_Execute", toolboxv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_IncrementHighlightIndex", CefV8Value::CreateFunction("BrowserAutomationStudio_IncrementHighlightIndex", toolboxv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_MultiselectStateChanged", CefV8Value::CreateFunction("BrowserAutomationStudio_MultiselectStateChanged", toolboxv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_MultiselectReset", CefV8Value::CreateFunction("BrowserAutomationStudio_MultiselectReset", toolboxv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_EnableModule", CefV8Value::CreateFunction("BrowserAutomationStudio_EnableModule", toolboxv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_TriggerEvent", CefV8Value::CreateFunction("BrowserAutomationStudio_TriggerEvent", toolboxv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_HandleEvent", CefV8Value::CreateFunction("BrowserAutomationStudio_HandleEvent", toolboxv8handler), V8_PROPERTY_ATTRIBUTE_NONE);

        object->SetValue("_K", CefV8Value::CreateString(Lang), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("_Z", CefV8Value::CreateInt(Zoom), V8_PROPERTY_ATTRIBUTE_NONE);
        return;
    }

    //Scenario Browser
    if(starts_with(url, "file:///html/scenario"))
    {
        WORKER_LOG("OnContextCreated<<BrowserScenario");
        if(frame->IsMain())
        {
            scenariov8handler = new InterprocessV8Handler("Scenario");
            CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("NewScenarioBrowserContextCreated");
            frame->SendProcessMessage(PID_BROWSER, msg);
        }

        CefRefPtr<CefV8Value> object = context->GetGlobal();
        object->SetValue("BrowserAutomationStudio_UpdateEmbeddedData", CefV8Value::CreateFunction("BrowserAutomationStudio_UpdateEmbeddedData", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_SendCode", CefV8Value::CreateFunction("BrowserAutomationStudio_SendCode", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_Initialized", CefV8Value::CreateFunction("BrowserAutomationStudio_Initialized", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_Execute", CefV8Value::CreateFunction("BrowserAutomationStudio_Execute", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_Restart", CefV8Value::CreateFunction("BrowserAutomationStudio_Restart", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_EditStart", CefV8Value::CreateFunction("BrowserAutomationStudio_EditStart", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_EditSaveStart", CefV8Value::CreateFunction("BrowserAutomationStudio_EditSaveStart", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_EditEnd", CefV8Value::CreateFunction("BrowserAutomationStudio_EditEnd", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_OpenUrl", CefV8Value::CreateFunction("BrowserAutomationStudio_OpenUrl", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_ThreadNumberEdit", CefV8Value::CreateFunction("BrowserAutomationStudio_ThreadNumberEdit", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_RunFunction", CefV8Value::CreateFunction("BrowserAutomationStudio_RunFunction", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_OpenAction", CefV8Value::CreateFunction("BrowserAutomationStudio_OpenAction", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_RunFunctionSeveralThreads", CefV8Value::CreateFunction("BrowserAutomationStudio_RunFunctionSeveralThreads", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_RunFunctionAsync", CefV8Value::CreateFunction("BrowserAutomationStudio_RunFunctionAsync", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_SuccessNumberEdit", CefV8Value::CreateFunction("BrowserAutomationStudio_SuccessNumberEdit", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_FailNumberEdit", CefV8Value::CreateFunction("BrowserAutomationStudio_FailNumberEdit", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_SetClipboard", CefV8Value::CreateFunction("BrowserAutomationStudio_SetClipboard", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_GetClipboard", CefV8Value::CreateFunction("BrowserAutomationStudio_GetClipboard", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_SendDetectorBrowserData", CefV8Value::CreateFunction("BrowserAutomationStudio_SendDetectorBrowserData", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_SetCurrentFunction", CefV8Value::CreateFunction("BrowserAutomationStudio_SetCurrentFunction", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_SetWebInterfaceTaskResult", CefV8Value::CreateFunction("BrowserAutomationStudio_SetWebInterfaceTaskResult", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_PrepareFunctionResult", CefV8Value::CreateFunction("BrowserAutomationStudio_PrepareFunctionResult", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_IsInsideElementLoopChanged", CefV8Value::CreateFunction("BrowserAutomationStudio_IsInsideElementLoopChanged", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_HighlightMenuItem", CefV8Value::CreateFunction("BrowserAutomationStudio_HighlightMenuItem", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_TriggerEvent", CefV8Value::CreateFunction("BrowserAutomationStudio_TriggerEvent", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_HandleEvent", CefV8Value::CreateFunction("BrowserAutomationStudio_HandleEvent", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_StartBackup", CefV8Value::CreateFunction("BrowserAutomationStudio_StartBackup", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_SaveInterfaceJson", CefV8Value::CreateFunction("BrowserAutomationStudio_SaveInterfaceJson", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_SaveInterfaceState", CefV8Value::CreateFunction("BrowserAutomationStudio_SaveInterfaceState", scenariov8handler), V8_PROPERTY_ATTRIBUTE_NONE);

        object->SetValue("_ApplicationEngineVersion", CefV8Value::CreateString(ApplicationEngineVersion), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("_ScriptEngineVersion", CefV8Value::CreateString(ScriptEngineVersion), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("_I", CefV8Value::CreateString(InterfaceState), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("_K", CefV8Value::CreateString(Lang), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("_Z", CefV8Value::CreateInt(100), V8_PROPERTY_ATTRIBUTE_NONE);
        return;
    }

    //Central Browser
    if(starts_with(url, "file:///html/central") || starts_with(url, "file:///html/menu"))
    {

        WORKER_LOG("OnContextCreated<<BrowserCentral");
        CefRefPtr<CefV8Value> object = context->GetGlobal();
        if(!central8handler)
            central8handler = new InterprocessV8Handler("Central");

        CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("NewCentralBrowserContextCreated");
        frame->SendProcessMessage(PID_BROWSER, msg);
        object->SetValue("BrowserAutomationStudio_OpenUrl", CefV8Value::CreateFunction("BrowserAutomationStudio_OpenUrl", central8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_Close", CefV8Value::CreateFunction("BrowserAutomationStudio_Close", central8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_ManualControl", CefV8Value::CreateFunction("BrowserAutomationStudio_ManualControl", central8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_SetOptions", CefV8Value::CreateFunction("BrowserAutomationStudio_SetOptions", central8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_Menu", CefV8Value::CreateFunction("BrowserAutomationStudio_Menu", central8handler), V8_PROPERTY_ATTRIBUTE_NONE);

        object->SetValue("_K", CefV8Value::CreateString(Lang), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("_Z", CefV8Value::CreateInt(Zoom), V8_PROPERTY_ATTRIBUTE_NONE);
        return;
    }

    //Detector browser
    if(starts_with(url, "file:///html/detector"))
    {
        WORKER_LOG("OnContextCreated<<BrowserDetector");
        CefRefPtr<CefV8Value> object = context->GetGlobal();
        if(!detectorv8handler)
            detectorv8handler = new InterprocessV8Handler("Detector");

        CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("NewDetectorBrowserContextCreated");
        frame->SendProcessMessage(PID_BROWSER, msg);
        object->SetValue("BrowserAutomationStudio_OpenUrl", CefV8Value::CreateFunction("BrowserAutomationStudio_OpenUrl", detectorv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_SaveFingerprintKey", CefV8Value::CreateFunction("BrowserAutomationStudio_SaveFingerprintKey", detectorv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_ClearAll", CefV8Value::CreateFunction("BrowserAutomationStudio_ClearAll", detectorv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_Logout", CefV8Value::CreateFunction("BrowserAutomationStudio_Logout", detectorv8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        object->SetValue("BrowserAutomationStudio_Initialized", CefV8Value::CreateFunction("BrowserAutomationStudio_Initialized", detectorv8handler), V8_PROPERTY_ATTRIBUTE_NONE);

        return;
    }

    //Main Browser
    if(!starts_with(url,"chrome-devtools://"))
    {
        WORKER_LOG(std::string("OnContextCreated<<MainBrowser<<") + std::to_string(browser->GetIdentifier()) + "<<" + std::to_string(frame->IsMain()) + "<<" + url);
        CefRefPtr<CefV8Value> object = context->GetGlobal();

        CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("NewMainBrowserContextCreated");
        msg->GetArgumentList()->SetSize(2);
        msg->GetArgumentList()->SetInt(0,browser->GetIdentifier());
        msg->GetArgumentList()->SetBool(1,frame->IsMain());

        frame->SendProcessMessage(PID_BROWSER, msg);
        /*{
            IPCSimple::Write(UniqueProcessId,std::to_string(browser->GetIdentifier()) + std::string(",") + std::to_string(frame->IsMain()));
        }*/

        {
            if(!v8handler)
            {
                v8handler = new InterprocessV8Handler("Main");
            }

            object->SetValue("browser_automation_studio_result", CefV8Value::CreateFunction("browser_automation_studio_result", v8handler), V8_PROPERTY_ATTRIBUTE_NONE);
            object->SetValue("browser_automation_studio_eval", CefV8Value::CreateFunction("browser_automation_studio_eval", v8handler), V8_PROPERTY_ATTRIBUTE_NONE);
            object->SetValue("browser_automation_studio_inspect_result", CefV8Value::CreateFunction("browser_automation_studio_inspect_result", v8handler), V8_PROPERTY_ATTRIBUTE_NONE);
            object->SetValue("browser_automation_studio_highlight_result", CefV8Value::CreateFunction("browser_automation_studio_highlight_result", v8handler), V8_PROPERTY_ATTRIBUTE_NONE);
            object->SetValue("browser_automation_studio_frame_structure_query_result", CefV8Value::CreateFunction("browser_automation_studio_frame_structure_query_result", v8handler), V8_PROPERTY_ATTRIBUTE_NONE);
            object->SetValue("browser_automation_studio_multiselect_result", CefV8Value::CreateFunction("browser_automation_studio_multiselect_result", v8handler), V8_PROPERTY_ATTRIBUTE_NONE);
            object->SetValue("browser_automation_studio_multiselect_report", CefV8Value::CreateFunction("browser_automation_studio_multiselect_report", v8handler), V8_PROPERTY_ATTRIBUTE_NONE);
            object->SetValue("browser_automation_studio_frame_find_result", CefV8Value::CreateFunction("browser_automation_studio_frame_find_result", v8handler), V8_PROPERTY_ATTRIBUTE_NONE);
            //object->SetValue("BrowserAutomationStudio_SaveLocalStorage", CefV8Value::CreateFunction("BrowserAutomationStudio_SaveLocalStorage", v8handler), V8_PROPERTY_ATTRIBUTE_NONE);
            //object->SetValue("BrowserAutomationStudio_DomainDataNeedClear", CefV8Value::CreateFunction("BrowserAutomationStudio_DomainDataNeedClear", v8handler), V8_PROPERTY_ATTRIBUTE_NONE);
            object->SetValue("BrowserAutomationStudio_SaveBlob", CefV8Value::CreateFunction("BrowserAutomationStudio_SaveBlob", v8handler), V8_PROPERTY_ATTRIBUTE_NONE);
            object->SetValue("BrowserAutomationStudio_RecaptchaV3", CefV8Value::CreateFunction("BrowserAutomationStudio_RecaptchaV3", v8handler), V8_PROPERTY_ATTRIBUTE_NONE);
        }
    }
}

