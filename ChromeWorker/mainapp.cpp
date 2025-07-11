#include "mainapp.h"
#include <string>
#include <thread>
#include "log.h"
#include "picojson.h"
#include "converter.h"
#include "match.h"
#include "base64.h"
#include "javascriptextensions.h"
#include "browsereventsemulator.h"
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/cef_parser.h"
#include "include/wrapper/cef_helpers.h"
#include "xml_encoder.h"
#include "lodepng.h"
#include "multithreading.h"
#include "modulesdata.h"
#include "readallfile.h"
#include "toolboxpreprocessor.h"
#include "clipboard.h"
#include "urlnormalize.h"
#include "chromecommandlineparser.h"
#include "replaceall.h"
#include "split.h"
#include "extract_labels.h"
#include "writefile.h"
#include "preparestartupscript.h"
#include <fstream>
#include "readallfile.h"
#include "writefile.h"
#include "translate.h"
#include "newtabschemehandlerfactory.h"
#include <chrono>

using namespace std::chrono;
using namespace std::placeholders;
MainApp * App;

MainApp::MainApp()
{
    _HandlersManager = std::make_shared<HandlersManager>();
    DoNothing = new DoNothingCallback();
    _EmptyRequestContextHandler = new EmptyRequestContextHandler();

    IsLastCommandNull = true;
    TypeTextTaskIsActive = false;
    DelayClickType = 0;
    TypeTextIsFirstLetter = true;
    IsWaitingForLoad = false;
    ResourcesChanged = true;
    InspectPosition = 0;
    TypeTextLastTime = 0;
    LastMouseTrack = 0;
    ScrollStopTracking = 0;
    DoTrackScroll = false;
    ScrollStopTrackingStart = 0;
    ScrollTrackingX = 0;
    ScrollTrackingY = 0;
    LastHighlight = 0;
    LastHighlightMultiselect = 0;
    LastRecaptchaV3Check = 0;
    ParentWidth = 0;
    ParentHeight = 0;
    App = this;
    IsMouseMoveSimulation = false;
    NeedRenderNextFrame = false;
    RenderNextFrameTime = 0;
    RunElementCommandCallbackOnNextTimer = -1;
    TypeTextDelayCurrent = 0;
    ClearElementCommand();
    IsScenarioInterfaceInitialSent = false;
    IsToolboxInterfaceInitialSent = false;
    _DevToolsReqest2Action = 0;
    IsMainBrowserCreating = true;

    ReadDoTour();

}

void MainApp::DirectControlAddAction(const std::string& Script)
{
    if(BrowserScenario)
    {
        BrowserScenario->GetMainFrame()->ExecuteJavaScript(Script,BrowserScenario->GetMainFrame()->GetURL(), 0);
    }
}

BrowserDirectControl * MainApp::DirectControl()
{
    return _BrowserDirectControl.get();
}

void MainApp::UpdateManualControl(bool NoFocus)
{
    if(Data->ManualControl != BrowserData::DirectNoRecord && Data->IsRecord && DirectControl())
        DirectControl()->ClearSequence();

    Layout->HideCentralBrowser();
    if(Layout->IsManualControlAction && Data->ManualControl != BrowserData::DirectNoRecord)
    {
        SendTextResponce("<ManualBrowserControl/>");
        SendTextResponce("<ManualControlIndicatorStop/>");
    }
    Layout->ChangeManualBrowserControlType(Data->ManualControl, NoFocus);
    RECT r = Layout->GetBrowserOuterRectangle(Data->WidthBrowser,Data->HeightBrowser,Data->WidthAll,Data->HeightAll);
    InvalidateRect(Data->_MainWindowHandle,&r,true);
    Data->Connector->SetOpenFileDialogManualMode(!Data->IsRecord && Data->ManualControl != BrowserData::Indirect);
    UpdateBrowserData(Data);
}

void MainApp::WriteBrowserData()
{
    UpdateBrowserData(Data);
}

std::string MainApp::Javascript(const std::string& Script, const std::string& BrowserType)
{
    if(BrowserType != "main")
        return Script;
    JavaScriptExtensions Extensions;
    return Extensions.ProcessJs(Script,Data->_UniqueProcessId);
}

void MainApp::ReadDoTour()
{
    std::string filename = "../../not_first_run.txt";

    if(ReadAllString(filename) == "true")
    {
        DoTour = false;
    }else
    {
        DoTour = true;
        WriteStringToFile(filename,"true");
    }
}

void MainApp::SetData(BrowserData *Data)
{
    this->Data = Data;

    _BrowserDirectControl = std::make_shared<BrowserDirectControl>();
    _BrowserDirectControl->Init(Data);
    _BrowserDirectControl->EventExecuteScenarioCode.push_back(std::bind(&MainApp::DirectControlAddAction,this,_1));

    _HandlersManager->SetUniqueProcessId(Data->_UniqueProcessId);

}

void MainApp::SetPopupEmulation(PopupEmulation *_PopupEmulation)
{
    this->_PopupEmulation = _PopupEmulation;
}

void MainApp::SetPostManager(PostManager *_PostManager)
{
    this->_PostManager = _PostManager;
}

void MainApp::SetDevToolsReqest2Action(DevToolsReqest2Action *_DevToolsReqest2Action)
{
    this->_DevToolsReqest2Action = _DevToolsReqest2Action;
    this->_DevToolsReqest2Action->OnDataReady.push_back(std::bind(&MainApp::OnRecordHttpData,this, _1));
}

DevToolsReqest2Action * MainApp::GetDevToolsReqest2Action()
{
    return _DevToolsReqest2Action;
}

void MainApp::SetSettings(settings *Settings)
{
    this->Settings = Settings;
}

void MainApp::SetLayout(MainLayout *Layout)
{
    this->Layout = Layout;
}

BrowserData * MainApp::GetData()
{
    return Data;
}

void MainApp::GetAllPopupsUrls(std::function<void(const std::vector<std::string>&)> Callback)
{
    Data->Connector->GetTabsList()->Then([Callback](AsyncResult* Result)
    {
        Callback(Result->GetList());
    });
}

int MainApp::GetActivePopupIndex()
{
    return Data->Connector->GetCurrentTabIndex();
}

void MainApp::ContextMenu(int X, int Y)
{
    
    std::string Script = Javascript(std::string("[[RESULT]] = _BAS_HIDE(BrowserAutomationStudio_GenerateMenu)(") + std::to_string(X) + std::string(",") + std::to_string(Y) + std::string(");"), "main");
    Async Result = Data->Connector->ExecuteJavascript(Script, std::string(), std::string("[]"));
    Data->Results->ProcessResult(Result);
    Result->Then([this, X, Y](AsyncResult* Result)
    {
        JsonParser Parser;
        std::string TextResult = Parser.GetStringFromJson(Result->GetString(),"RESULT");

        std::string CurrentUrl = Parser.GetStringFromJson(TextResult,"current_url");
        std::string MediaUrl = Parser.GetStringFromJson(TextResult,"media_url");
        std::string LinkUrl = Parser.GetStringFromJson(TextResult,"link_url");
        std::string SelectedText = Parser.GetStringFromJson(TextResult,"selected_text");
        bool IsLink = Parser.GetBooleanFromJson(TextResult,"is_link");
        bool IsMedia = Parser.GetBooleanFromJson(TextResult,"is_media");
        bool IsEdit = Parser.GetBooleanFromJson(TextResult,"is_edit");

        Data->_BrowserContextMenu.Show(Data->_MainWindowHandle,X,Y,IsLink,IsMedia,IsEdit,LinkUrl,MediaUrl,CurrentUrl,SelectedText,true,true);
    });
}


CefRefPtr<CefBrowserProcessHandler> MainApp::GetBrowserProcessHandler()
{
    return this;
}


void MainApp::OnContextInitialized()
{
    _HandlersManager->Init1(new MainHandler(),
                            std::bind(&MainApp::SendTextResponce,this,_1),
                            std::bind(&MainApp::UrlLoaded,this,_1,_2,_3),
                            std::bind(&MainApp::LoadSuccessCallback,this),
                            std::bind(&MainApp::CursorChanged,this,_1),
                            std::bind(&MainApp::OldestRequestTimeChanged,this,_1),
                            std::bind(&MainApp::DownloadStart,this),
                            std::bind(&MainApp::UploadStart,this),
                            std::bind(&MainApp::ComboboxOpened,this),
                            std::bind(&MainApp::StartRequest,this,_1),
                            std::bind(&MainApp::ProcessMessage,this,_1,_2,_3,_4),
                            std::bind(&MainApp::LinkCtrlClick,this,_1,_2),
                            std::bind(&MainApp::CurrentTabChanged,this)
                            );

    _HandlersManager->GetHandler()->SetSettings(Settings);
    _HandlersManager->GetHandler()->SetData(Data);
    _HandlersManager->GetHandler()->SetPostManager(_PostManager);
    _HandlersManager->GetHandler()->SetDirectControl(DirectControl());

    dhandler = new DevToolsHandler();
    dhandler->SetData(Data);
    dhandler->SetLayout(Layout);
    dhandler->SetHandlersManager(_HandlersManager.get());
    cookievisitor = new CookieVisitor();



    CefRegisterSchemeHandlerFactory("tab", "new", new NewTabSchemeHandlerFactory());
}

void MainApp::OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info)
{
    //THREAD TID_IO

    extra_info->SetSize(4);
    extra_info->SetBool(0,Data->IsRecord);
    int BrowserToolboxId = -1;
    if(BrowserToolbox)
        BrowserToolboxId = BrowserToolbox->GetIdentifier();
    extra_info->SetInt(1,BrowserToolboxId);

    int BrowserScenarioId = -1;
    if(BrowserScenario)
        BrowserScenarioId = BrowserScenario->GetIdentifier();
    extra_info->SetInt(2,BrowserScenarioId);

    int BrowserCentralId = -1;
    if(BrowserCentral)
        BrowserCentralId = BrowserCentral->GetIdentifier();
    extra_info->SetInt(3,BrowserCentralId);

    extra_info->SetInt(4,Settings->Zoom());
    extra_info->SetString(5,Lang);

    int TabId = -1;
    if(_HandlersManager->GetBrowser())
        TabId = _HandlersManager->FindTabIdByBrowserId(_HandlersManager->GetBrowser()->GetIdentifier());

    std::string Script;
    extra_info->SetString(6,Script);

    extra_info->SetString(7,Data->_UniqueProcessId);


    extra_info->SetString(8,ApplicationEngineVersion);
    extra_info->SetString(9,ScriptEngineVersion);
    extra_info->SetString(10,InterfaceState);
}


void MainApp::OldestRequestTimeChanged(int64 OldestTime)
{
    Data->OldestRequestTime = OldestTime;
}

void MainApp::DownloadStart()
{
    if(Data->IsRecord && BrowserToolbox)
    {
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript("BrowserAutomationStudio_Notify('download')","toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
    }
}

void MainApp::ComboboxOpened()
{
    if(Data->IsRecord && BrowserToolbox)
    {
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript("BrowserAutomationStudio_Notify('combobox')","toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
    }
}

void MainApp::UploadStart()
{
    if(Data->IsRecord && BrowserToolbox)
    {
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript("BrowserAutomationStudio_Notify('upload')","toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
    }
}

void MainApp::StartRequest(CefRefPtr<CefRequest> Request)
{
}

void MainApp::OnRequestDataMain(std::string RequestData)
{
    if(Data->IsRecordHttp && _DevToolsReqest2Action)
    {
        _DevToolsReqest2Action->ConvertMain(RequestData);
    }
}

void MainApp::OnRequestDataAdditional(std::string RequestData)
{
    if(Data->IsRecordHttp && _DevToolsReqest2Action)
    {
        _DevToolsReqest2Action->ConvertAdditional(RequestData);
    }
}

void MainApp::CursorChanged(int Type)
{
    //THREAD TID_UI
    Layout->SetBrowserCursor(Type);
}

void MainApp::NewMainBrowserContextCreated(int BrowserId, bool IsMain)
{
    WORKER_LOG(std::string("OnProcessMessageReceived<<NewMainBrowserContextCreated<<") + std::to_string(BrowserId) + std::string("<<") + std::to_string(IsMain));
    _HandlersManager->NewContextCreated(BrowserId);
    if(IsMain)
    {
        Data->ScrollX = 0;
        Data->ScrollY = 0;
    }
    if(!v8handler)
        v8handler = new V8Handler();
}


void MainApp::LinkCtrlClick(const std::string& current_url, const std::string& target_url)
{
    /*if(_HandlersManager->GetBrowser())
    {
        AddHeaderCallbackInternal("Referer",current_url,"");

        Data->IsCreatingNewPopup = true;
        Data->IsCreatingNewPopupIsLoaded = false;
        Data->IsCreatingNewPopupIsContextCreated = false;
        Data->IsCreatingNewPopupIsSilent = false;
        Data->IsCreatingNewPopupIsLoadAfterOpen = true;
        Data->IsCreatingNewPopupIndexBeforeChange = -1;
        Data->IsCreatingNewPopupUrl = target_url;

        std::string Script = std::string("window.open('tab://new/')");
        _HandlersManager->GetBrowser()->GetMainFrame()->ExecuteJavaScript(Script,"", 0);
    }*/
}

void MainApp::Reload()
{
    if(_HandlersManager->GetBrowser())
    {
        _HandlersManager->GetBrowser()->Reload();
    }
}

void MainApp::ShowDevTools()
{
    Data->Connector->OpenDevTools();
}



void MainApp::CurrentTabChanged()
{

    if(Data->IsCreatingNewPopup)
    {
        //WORKER_LOG("!!!!!!PopupCreate CurrentTabChanged");
        Data->IsCreatingNewPopupIsContextCreated = true;
        if(Data->IsCreatingNewPopupIsLoaded)
        {
            Data->IsCreatingNewPopup = false;
            //WORKER_LOG("!!!!!!PopupCreate Final");
            //Popup has been created, finalize
            if(Data->IsCreatingNewPopupIsLoadAfterOpen)
            {
                if(_HandlersManager->GetBrowser() && _HandlersManager->GetBrowser()->GetMainFrame())
                    _HandlersManager->GetBrowser()->GetMainFrame()->LoadURL(Data->IsCreatingNewPopupUrl);
            }else
            {
                if(Data->IsCreatingNewPopupIsSilent)
                {
                    //Preserve id of new tab
                    int id = _HandlersManager->GetBrowser()->GetIdentifier();

                    //Switch to old tab
                    _HandlersManager->SwitchByIndex(Data->IsCreatingNewPopupIndexBeforeChange);
                    SetWindowText(GetData()->UrlHandler, s2ws(GetUrl()).c_str());

                    //Set url to open when tab gets active
                    _HandlersManager->SetUrlToOpenOnNextActivation(id,Data->IsCreatingNewPopupUrl);

                }
                SendTextResponce("<PopupCreate></PopupCreate>");
            }
        }
    }

    if(!Data->IsRecord && Data->ManualControl != BrowserData::Indirect)
    {
        _HandlersManager->CloseDevToolsAll();
    }
}

void MainApp::ProcessMessage(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message, bool* is_processed)
{
    if(message->GetName().ToString() == std::string("NewMainBrowserContextCreated"))
    {
        int BrowserId = message->GetArgumentList()->GetInt(0);
        bool IsMain = message->GetArgumentList()->GetBool(1);
        NewMainBrowserContextCreated(BrowserId, IsMain);
        *is_processed = true;
    }
    if(message->GetName().ToString() == std::string("NewCentralBrowserContextCreated"))
    {
        WORKER_LOG(std::string("OnProcessMessageReceived<<NewCentralBrowserContextCreated<<"));
        if(!central8handler)
            central8handler = new CentralV8Handler();
        *is_processed = true;
    }
    if(message->GetName().ToString() == std::string("NewDetectorBrowserContextCreated"))
    {
        WORKER_LOG(std::string("OnProcessMessageReceived<<NewDetectorBrowserContextCreated<<"));
        if(!detector8handler)
            detector8handler = new DetectorV8Handler();
        *is_processed = true;
    }
    if(message->GetName().ToString() == std::string("NewScenarioBrowserContextCreated"))
    {
        if(Settings->AutostartDebug() && Data->IsRecord)
        {
            std::wstring Url = std::wstring(L"http://127.0.0.1:") + std::to_wstring(Data->RemoteDebuggingPort);
            ShellExecute(0, 0, Url.c_str(), 0, 0 , SW_SHOW );
        }
        WORKER_LOG(std::string("OnProcessMessageReceived<<NewScenarioBrowserContextCreated<<"));
        if(!scenariov8handler)
            scenariov8handler = new ScenarioV8Handler();
        *is_processed = true;
    }
    if(message->GetName().ToString() == std::string("NewToolboxBrowserContextCreated"))
    {
        WORKER_LOG(std::string("OnProcessMessageReceived<<NewToolboxBrowserContextCreated<<"));
        if(!toolboxv8handler)
            toolboxv8handler = new ToolboxV8Handler();
        *is_processed = true;
    }

    if(message->GetName().ToString() == std::string("V8HandlerMessage"))
    {
        CefRefPtr<CefListValue> List = message->GetArgumentList()->Copy();
        std::string HandleName = List->GetString(0).ToString();
        std::string Name = List->GetString(1).ToString();
        List->Remove(1);
        List->Remove(0);
        //WORKER_LOG(std::string("<- OnProcessMessageReceived<<V8HandlerMessage<<") + HandleName + std::string("<<") + Name);

        if(HandleName == "Central")
        {
            central8handler->Execute(Name,List);
        }
        if(HandleName == "Main")
        {
            v8handler->Execute(Name,List);
        }
        if(HandleName == "Scenario")
        {
            scenariov8handler->Execute(Name,List);
        }
        if(HandleName == "Toolbox")
        {
            toolboxv8handler->Execute(Name,List);
        }
        if(HandleName == "Detector")
        {
            detector8handler->Execute(Name,List);
        }

        *is_processed = true;
    }


}


void MainApp::Paint(int width, int height)
{
    char *ImageData = Data->Connector->GetPaintData();

    if(!ViewRequestId.empty())
    {
        std::string base64;
        int x = Data->CursorX,y = Data->CursorY;
        std::vector<unsigned char> out;
        std::vector<unsigned char> in;

        for(int j = 0;j<height;j++)
        {
            for(int i = 0;i<width;i++)
            {
                in.push_back((unsigned char)ImageData[i*4+j*width*4 + 2]);
                in.push_back((unsigned char)ImageData[i*4+j*width*4 + 1]);
                in.push_back((unsigned char)ImageData[i*4+j*width*4 + 0]);
                in.push_back((unsigned char)ImageData[i*4+j*width*4 + 3]);
            }
        }

        lodepng::encode(out,(unsigned const char *)(in.data()),width,height);
        base64 = base64_encode(out.data(),out.size());


        std::string xml = std::string("<View id=\"");

        xml += ViewRequestId;
        xml += std::string("\" data=\"");
        xml += base64;
        xml += std::string("\" x=\"");
        xml += std::to_string(x);
        xml += std::string("\" y=\"");
        xml += std::to_string(y);
        xml += std::string("\" width=\"");
        xml += std::to_string(width);
        xml += std::string("\" height=\"");
        xml += std::to_string(height);
        xml += std::string("\" />");

        SendTextResponce(xml);

        ViewRequestId.clear();

    }



    if(/*Layout->GetIsRenderEmpty() && */Data->IsRecord)
    {
        bool is_break = false;
        int len = width * height * 4;
        for(int i = 0;i<len;i++)
        {
            char c = ImageData[i];
            if(c != -1)
            {
                Layout->SetIsRenderEmpty(false);
                is_break = true;
                break;
            }
        }
        if(!is_break)
        {
            Layout->SetIsRenderEmpty(true);
        }
    }

    RECT r = Layout->GetBrowserRectangle(GetData()->WidthBrowser,GetData()->HeightBrowser,GetData()->WidthAll,GetData()->HeightAll);
    InvalidateRect(Data->_MainWindowHandle,&r,false);
    //InvalidateRect(Data->_MainWindowHandle,NULL,false);
}

void MainApp::ClearImageDataCallback()
{
    _ImageFinder.ClearImage();
    SendTextResponce("<ClearImageData></ClearImageData>");
}
void MainApp::SetImageDataCallback(const std::string& base64)
{
    _ImageFinder.SetImage(base64);
    SendTextResponce("<SetImageData></SetImageData>");
}
void MainApp::FindImageCallback()
{
    std::string res = _ImageFinder.FindImage(Data->Connector->GetPaintData(),Data->Connector->GetPaintWidth(),Data->Connector->GetPaintHeight(),Data->ScrollX,Data->ScrollY);
    xml_encode(res);

    SendTextResponce(std::string("<FindImage>") + res + std::string("</FindImage>"));
}

char* MainApp::GetImageData()
{
    return Data->Connector->GetPaintDataScaled();
}

std::string MainApp::GetSubImageDataBase64(int x1, int y1, int x2, int y2)
{
    int RenderX;
    int RenderY;
    int RenderWidth;
    int RenderHeight;
    int width = Data->Connector->GetPaintWidth();
    int height = Data->Connector->GetPaintHeight();
    char * data = Data->Connector->GetPaintData();

    if(x1 < x2)
    {
        RenderX = x1;
        RenderWidth = x2 - x1;
    }else
    {
        RenderX = x2;
        RenderWidth = x1 - x2;
    }

    if(y1 < y2)
    {
        RenderY = y1;
        RenderHeight = y2 - y1;
    }else
    {
        RenderY = y2;
        RenderHeight = y1 - y2;
    }

    std::vector<unsigned char> out;
    std::vector<unsigned char> in;

    int w = 0;
    int h = 0;

    for(int j = 0;j<height;j++)
    {
        if(j>RenderY && j<RenderY + RenderHeight)
        {
            h++;
        }
        for(int i = 0;i<width;i++)
        {

            if(i>RenderX && i<RenderX + RenderWidth && j>RenderY && j<RenderY + RenderHeight)
            {
                if(h==1)
                    w++;
                in.push_back((unsigned char)data[i*4+j*width*4 + 2]);
                in.push_back((unsigned char)data[i*4+j*width*4 + 1]);
                in.push_back((unsigned char)data[i*4+j*width*4 + 0]);
                in.push_back((unsigned char)data[i*4+j*width*4 + 3]);
            }
        }

    }

    lodepng::encode(out,(unsigned const char *)(in.data()),w,h);
    std::string base64 = base64_encode(out.data(),out.size());

    return base64;
}

std::pair<int,int> MainApp::GetImageSize()
{
    std::pair<int,int> res;
    res.first = Data->Connector->GetPaintWidthScaled();
    res.second = Data->Connector->GetPaintHeightScaled();
    return res;
}


void MainApp::UrlLoaded(const std::string& url, int status, int RequestResourceType)
{

}

void MainApp::DisableBrowserCallback()
{
    SendTextResponce("<DisableBrowser></DisableBrowser>");
}

void MainApp::SetFocusOnNextLoad()
{
    IsWaitingForLoad = true;
}

void MainApp::CreateBrowser(const std::string& Url)
{
    NextLoadPage = Url;
    AfterReadyToCreateBrowser(true);
}

void MainApp::LoadCallback(const std::string& page)
{
    Async Result = Data->Connector->Load(page, false);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        this->Data->Connector->Focus();
        if(Result->GetIsSuccess())
        {
            this->SendTextResponce("<Load>0</Load>");
        }else
        {
            this->SendTextResponce("<Load>1</Load>");
        }
    });

    SendTextResponce("<LoadedInstant></LoadedInstant>");
}

void MainApp::Load2Callback(const std::string& url,const std::string& referrer, bool instant)
{
    Async Result = Data->Connector->Load(url, instant, referrer);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        this->Data->Connector->Focus();
        if(Result->GetIsSuccess())
        {
            this->SendTextResponce("<Load2></Load2>");
        }else
        {
            std::string error = Result->GetErrorMessage();
            xml_encode(error);
            this->SendTextResponce(std::string("<Load2>") + error + std::string("</Load2>"));
        }
    });
}

void MainApp::ViewCallback(const std::string& RequestId)
{
    ViewRequestId = RequestId;
    if(_HandlersManager->GetBrowser())
        _HandlersManager->GetBrowser()->GetHost()->Invalidate(PET_VIEW);
}


void MainApp::GetTabsCallback(const std::string& RequestId)
{
    Async Result = Data->Connector->GetTabsList();
    Data->Results->ProcessResult(Result);
    Result->Then([this, RequestId](AsyncResult* Result)
    {
        std::vector<std::string> urls = Result->GetList();
        picojson::array urls_json;
        for(std::string& url:urls)
        {
            urls_json.push_back(picojson::value(url));
        }
        int index = Data->Connector->GetCurrentTabIndex();

        picojson::object res;

        res["urls"] = picojson::value(urls_json);
        res["index"] = picojson::value((double)index);

        std::string string_res = picojson::value(res).serialize();

        xml_encode(string_res);

        this->SendTextResponce(std::string("<GetTabs id=\"") + RequestId + std::string("\" >") + string_res + std::string("</GetTabs>"));
    });

}

void MainApp::LoadNoDataCallback()
{
    if(BrowserCentral)
    {
        BrowserCentral->GetMainFrame()->ExecuteJavaScript("document.body.style.display='none'","file:///html/central/empty.html",0);
        std::string page = std::string("file:///html/central/no_data_") + Lang + std::string(".html");
        BrowserCentral->GetMainFrame()->LoadURL(page);
        RECT r = Layout->GetBrowserOuterRectangle(GetData()->WidthBrowser,GetData()->HeightBrowser,GetData()->WidthAll,GetData()->HeightAll);
        InvalidateRect(Data->_MainWindowHandle,&r,false);
    }
}

void MainApp::LoadManualSelect()
{
    if(BrowserCentral)
    {
        BrowserCentral->GetMainFrame()->ExecuteJavaScript("document.body.style.display='none'","file:///html/central/empty.html",0);
        std::string page = std::string("file:///html/central/manual_select_") + Lang + std::string(".html?d=") + std::to_string((int)Data->ManualControl);
        BrowserCentral->GetMainFrame()->LoadURL(page);
    }
}

void MainApp::IsChangedCallback()
{
    if(BrowserScenario)
    {
        BrowserScenario->GetMainFrame()->ExecuteJavaScript(Javascript("BrowserAutomationStudio_IsChanged()","scenario"),BrowserScenario->GetMainFrame()->GetURL(), 0);
    }
}

void MainApp::NavigateBackCallback(bool IsInstant)
{
    WORKER_LOG("NavigateBackCallback");
    Async Result = Data->Connector->NavigateBack(IsInstant);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        Data->_RequestList.RemoveAll();
        this->SendTextResponce("<NavigateBack></NavigateBack>");
    });
}


void MainApp::PopupCreateCallback(bool is_silent, const std::string& url)
{
    Async Result = Data->Connector->CreateTab(url, false, is_silent, std::string());
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        this->Data->Connector->Focus();
        this->SendTextResponce("<PopupCreate></PopupCreate>");
    });
}

void MainApp::PopupCreate2Callback(bool is_silent, const std::string& url, const std::string& referrer, bool is_instant)
{
    Async Result = Data->Connector->CreateTab(url, is_instant, is_silent, referrer);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        this->Data->Connector->Focus();
        if(Result->GetIsSuccess())
        {
            this->SendTextResponce("<PopupCreate2></PopupCreate2>");
        }else
        {
            std::string error = Result->GetErrorMessage();
            xml_encode(error);
            this->SendTextResponce(std::string("<PopupCreate2>") + error + std::string("</PopupCreate2>"));
        }

    });
}

void MainApp::SetComboboxIndexCallback(int Index)
{
    _PopupEmulation->SetIndex(Index);
    this->SendTextResponce(std::string("<SetComboboxIndex></SetComboboxIndex>"));
}


void MainApp::SetOpenFileNameCallback(const std::string& value)
{
    Data->Connector->SetOpenFileDialogResult(value);
    SendTextResponce("<SetOpenFileName>1</SetOpenFileName>");
}

void MainApp::DragFileCallback(const std::string& value)
{
    Async Result = Data->Connector->StartDragFile(value);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        this->SendTextResponce("<DragFile></DragFile>");
    });
}

void MainApp::SetStartupScriptCallback(const std::string& value,const std::string& target,const std::string& script_id)
{
    auto it = Data->_StartupScript.find(script_id);
    if(it == Data->_StartupScript.end())
    {
        ConfigurableItem<std::string> item;
        item.Set(value, target);
        Data->_StartupScript[script_id] = item;
    }else
    {
        it->second.Set(value, target);
    }

    UpdateBrowserData(Data);

    Async Result = Data->Connector->Sleep(2000);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        SendTextResponce("<SetStartupScript></SetStartupScript>");
    });


}

void MainApp::RunTaskCallback(const std::string& function_name,const std::string& params,const std::string& result_id)
{
    std::string script = std::string(";_WebInterfaceTasks.RunTask(") + result_id + std::string(",") + picojson::value(function_name).serialize() + std::string(",") + picojson::value(params).serialize() + std::string(");");
    if(BrowserScenario)
        BrowserScenario->GetMainFrame()->ExecuteJavaScript(script,BrowserScenario->GetMainFrame()->GetURL(), 0);
    else
        BrowserScenarioDelayScript += script;

    /*xml_encode(script);
    SendTextResponce(
                std::string("<RunTaskResult ResultId=\"") +
                result_id +
                std::string("\">") + script + std::string("</RunTaskResult>")
             );*/

}

void MainApp::CheckResultCallback(const std::string& CheckId,bool IsSuccess,const std::string& ErrorString)
{
    if(BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(std::string("_EmbeddedModel.CheckResult(") + picojson::value(CheckId).serialize() + std::string(",") + picojson::value(IsSuccess).serialize() + std::string(",") + picojson::value(ErrorString).serialize() + std::string(")"),BrowserToolbox->GetMainFrame()->GetURL(), 0);

}

void MainApp::SetWorkerSettingsCallback(bool EncodeUtf8, bool RefreshConnections, const std::string& server, int Port, bool IsHttp, const std::string& username, const std::string& password, const std::string& target, const std::string& browser, const std::string& record_id)
{
    SetProxyCallback(server,Port,IsHttp,username,password,target);
}

void MainApp::ManualBrowserControlCallback(const std::string& message)
{
    Data->ManualControl = BrowserData::DirectNoRecord;

    std::string FullMessage = message;

    if(!FullMessage.empty())
        FullMessage += std::string(". ");

    FullMessage += ws2s(Translate::Tr(L"Browser is under user contol. <A>Return control to the application.</a>"));
    Layout->StartManualControl(FullMessage);

    UpdateManualControl();

    std::string MessageEncoded = message;
    xml_encode(MessageEncoded);
    SendTextResponce(std::string("<ManualControlIndicatorStart>") + MessageEncoded + std::string("</ManualControlIndicatorStart>"));
}

void MainApp::SetFontListCallback(const std::string& fonts)
{
    //FontReplace::GetInstance().Hook();
    //FontReplace::GetInstance().SetFonts(fonts);

    CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("SetFontList");
    msg->GetArgumentList()->SetSize(1);
    msg->GetArgumentList()->SetString(0,fonts);

    _HandlersManager->SendToAll(msg);
    SendTextResponce("<SetFontList></SetFontList>");
}


void MainApp::SetPromptResultCallback(const std::string& value)
{
    Data->Connector->SetPromptResult(value);
    SendTextResponce("<SetPromptResult>1</SetPromptResult>");
}

void MainApp::SetHttpAuthResultCallback(const std::string& login,const std::string& password)
{
    Async Result = Data->Connector->SetHttpAuth(login, password);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        SendTextResponce("<SetHttpAuthResult>1</SetHttpAuthResult>");
    });
}

void MainApp::GetCookiesForUrlCallback(const std::string& value)
{
    std::string cookies;
    xml_encode(cookies);
    SendTextResponce(std::string("<GetCookiesForUrl>") + cookies + std::string("</GetCookiesForUrl>"));
}

void MainApp::SaveCookiesCallback()
{
    Async Result = Data->Connector->SaveCookies();
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        std::string cookies = Result->GetString();
        xml_encode(cookies);
        SendTextResponce(std::string("<SaveCookies>") + cookies + std::string("</SaveCookies>"));
    });
}

void MainApp::RestoreLocalStorageCallback(const std::string& value)
{
    //Not used
    SendTextResponce(std::string("<RestoreLocalStorage></RestoreLocalStorage>"));
}

void MainApp::RestoreCookiesCallback(const std::string& value)
{
    Async Result = Data->Connector->RestoreCookies(value);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        SendTextResponce(std::string("<RestoreCookies></RestoreCookies>"));
    });
}

void MainApp::ResizeCallback(int width, int height)
{
    Async Result = Data->Connector->ResizeBrowser(width, height);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        this->SendTextResponce("<Resize></Resize>");
    });
}

void MainApp::ForceUpdateWindowPositionWithParent()
{
   ParentWidth = 0;
   ParentHeight = 0;
   UpdateWindowPositionWithParent();
}

void MainApp::UpdateWindowPositionWithParent()
{

    if(Data->_ParentWindowHandle && Layout->IsMinimized)
    {
        RECT rc;
        GetClientRect(Data->_ParentWindowHandle, &rc);
        int ParentWidthPrev = ParentWidth;
        int ParentHeightPrev = ParentHeight;
        ParentWidth = rc.right - rc.left;
        ParentHeight = rc.bottom - rc.top;
        if(ParentWidthPrev != ParentWidth || ParentHeightPrev != ParentHeight)
        {
            MoveWindow(Data->_MainWindowHandle,0,0,ParentWidth,ParentHeight,true);
        }
    }
}

void MainApp::HighlightActionCallback(const std::string& ActionId)
{
    WORKER_LOG(std::string("HighlightActionCallback<<") + ActionId);

    if(BrowserScenario)
        BrowserScenario->GetMainFrame()->ExecuteJavaScript(
                    Javascript(std::string("BrowserAutomationStudio_UnfoldParents(") + ActionId + std::string(");") +
                    std::string("BrowserAutomationStudio_FocusAction(") + ActionId + std::string(");"),"scenario")
                    ,BrowserScenario->GetMainFrame()->GetURL(), 0);

}

void MainApp::SetWindowCallback(const std::string& Window, bool IsPlayingScript)
{
    WORKER_LOG(std::string("SetWindowCallback<<") + Window);
    Data->_ParentWindowHandle = (HWND)std::stoi(Window);
    Layout->MinimizeOrMaximize(Data->_MainWindowHandle,Data->_ParentWindowHandle);
    if(Settings->Maximized())
        Layout->MinimizeOrMaximize(Data->_MainWindowHandle,Data->_ParentWindowHandle);
    ForceUpdateWindowPositionWithParent();

    if(IsPlayingScript)
        StartPlayScriptOnStart = true;

    SendTextResponce("<WindowAttached></WindowAttached>");
}



void MainApp::LoadSuccessCallback()
{
    if(IsWaitingForLoad)
    {
        BrowserEventsEmulator::SetFocus(_HandlersManager->GetBrowser());
        IsWaitingForLoad = false;
    }
}

void MainApp::MouseClickCallback(int x, int y)
{
    std::string ScrollToScript = Javascript(std::string("[[RESULT]] = _BAS_HIDE(BrowserAutomationStudio_ScrollToCoordinates)(") + std::to_string(x) + std::string(",") + std::to_string(y) + std::string(")"), "main");
    
    Async Result = Data->Connector->ExecuteJavascript(ScrollToScript, std::string(), std::string("[]"));
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        JsonParser Parser;
        std::string TextResult = Parser.GetStringFromJson(Result->GetString(),"RESULT");
        this->UpdateScrolls(TextResult);
        std::size_t pos = TextResult.find(",");
        int X = -1, Y = -1;
        if(pos != std::string::npos)
        {
            std::string x_string = TextResult.substr(0,pos);
            std::string y_string = TextResult.substr(pos + 1,TextResult.length() - pos - 1);
            X = std::stoi(x_string);
            Y = std::stoi(y_string);
        }

        if(Data->IsTouchScreen)
        {
            this->DelayClickType = 1;
            this->DelayNextClick = -1;
            this->DelayClickX = X;
            this->DelayClickY = Y;
            TouchStartTask = BrowserEventsEmulator::MouseClick(this->Data->Connector,X,Y,GetScrollPosition(),2,this->Data->IsMousePress,this->Data->IsDrag,this->Data->IsTouchScreen,this->Data->TouchEventId,this->Data->IsTouchPressedAutomation,this->TypeTextState);
        }else
        {
            this->DelayClickType = 1;
            this->DelayNextClick = clock() + 80 + (rand()) % 40;
            this->DelayClickX = X;
            this->DelayClickY = Y;
            BrowserEventsEmulator::MouseClick(this->Data->Connector,X,Y,GetScrollPosition(),2,this->Data->IsMousePress,this->Data->IsDrag,this->Data->IsTouchScreen,this->Data->TouchEventId,this->Data->IsTouchPressedAutomation,this->TypeTextState);
        }
    });
}

void MainApp::MouseClickUpCallback(int x, int y)
{
    if(Data->IsTouchScreen)
    {
        x = Data->ScrollX + Data->CursorX;
        y = Data->ScrollY + Data->CursorY;
        BrowserEventsEmulator::MouseClick(Data->Connector,x,y,GetScrollPosition(),1,Data->IsMousePress,Data->IsDrag,Data->IsTouchScreen,Data->TouchEventId,Data->IsTouchPressedAutomation,TypeTextState);
        SendTextResponce("<MouseClickUp></MouseClickUp>");
        return;
    }
    std::string ScrollToScript = Javascript(std::string("[[RESULT]] = _BAS_HIDE(BrowserAutomationStudio_ScrollToCoordinates)(") + std::to_string(x) + std::string(",") + std::to_string(y) + std::string(")"), "main");
    
    Async Result = Data->Connector->ExecuteJavascript(ScrollToScript, std::string(), std::string("[]"));
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        JsonParser Parser;
        std::string TextResult = Parser.GetStringFromJson(Result->GetString(),"RESULT");
        this->UpdateScrolls(TextResult);
        std::size_t pos = TextResult.find(",");
        int X = -1, Y = -1;
        if(pos != std::string::npos)
        {
            std::string x_string = TextResult.substr(0,pos);
            std::string y_string = TextResult.substr(pos + 1,TextResult.length() - pos - 1);
            X = std::stoi(x_string);
            Y = std::stoi(y_string);
        }

        BrowserEventsEmulator::MouseClick(Data->Connector,X,Y,GetScrollPosition(),1,Data->IsMousePress,Data->IsDrag,Data->IsTouchScreen,Data->TouchEventId,Data->IsTouchPressedAutomation,TypeTextState);

        this->SendTextResponce("<MouseClickUp></MouseClickUp>");
    });
}


void MainApp::MouseClickDownCallback(int x, int y)
{
    if(Data->IsTouchScreen)
    {
        x = Data->ScrollX + Data->CursorX;
        y = Data->ScrollY + Data->CursorY;
        BrowserEventsEmulator::MouseClick(Data->Connector,x,y,GetScrollPosition(),2,Data->IsMousePress,Data->IsDrag,Data->IsTouchScreen,Data->TouchEventId,Data->IsTouchPressedAutomation,TypeTextState);
        SendTextResponce("<MouseClickDown></MouseClickDown>");
        return;
    }
    
    std::string ScrollToScript = Javascript(std::string("[[RESULT]] = _BAS_HIDE(BrowserAutomationStudio_ScrollToCoordinates)(") + std::to_string(x) + std::string(",") + std::to_string(y) + std::string(")"), "main");
    
    Async Result = Data->Connector->ExecuteJavascript(ScrollToScript, std::string(), std::string("[]"));
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        JsonParser Parser;
        std::string TextResult = Parser.GetStringFromJson(Result->GetString(),"RESULT");
        this->UpdateScrolls(TextResult);
        std::size_t pos = TextResult.find(",");
        int X = -1, Y = -1;
        if(pos != std::string::npos)
        {
            std::string x_string = TextResult.substr(0,pos);
            std::string y_string = TextResult.substr(pos + 1,TextResult.length() - pos - 1);
            X = std::stoi(x_string);
            Y = std::stoi(y_string);
        }

        BrowserEventsEmulator::MouseClick(Data->Connector,X,Y,GetScrollPosition(),2,Data->IsMousePress,Data->IsDrag,Data->IsTouchScreen,Data->TouchEventId,Data->IsTouchPressedAutomation,TypeTextState);
        this->SendTextResponce("<MouseClickDown></MouseClickDown>");
    }); 
}


void MainApp::PopupCloseCallback(int index)
{
    Async Result = Data->Connector->CloseTab(index);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        this->Data->Connector->Focus();
        this->SendTextResponce("<PopupClose></PopupClose>");
    });
}

void MainApp::PopupSelectCallback(int index)
{
    Async Result = Data->Connector->SwitchToTab(index);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        this->Data->Connector->Focus();
        this->SendTextResponce("<PopupSelect></PopupSelect>");
    });
}

void MainApp::PopupInfoCallback()
{
    Async Result = Data->Connector->GetTabsList();
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        std::vector<std::string> urls = Result->GetList();
        picojson::array urls_json;
        for(std::string& url:urls)
        {
            urls_json.push_back(picojson::value(url));
        }
        int index = Data->Connector->GetCurrentTabIndex();

        picojson::object res;

        res["urls"] = picojson::value(urls_json);
        res["index"] = picojson::value((double)index);

        std::string string_res = picojson::value(res).serialize();

        xml_encode(string_res);

        this->SendTextResponce(std::string("<PopupInfo>") + string_res + std::string("</PopupInfo>"));
    });
}

void MainApp::SetDeviceScaleFactorCallback(double scale)
{
    Data->DeviceScaleFactor = scale;
    UpdateBrowserData(Data);
    Async Result = Data->Connector->ResetDeviceScaleFactor(scale);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        this->SendTextResponce("<SetDeviceScaleFactor></SetDeviceScaleFactor>");
    });
}

void MainApp::MouseMoveCallback(int x, int y, double speed, double gravity, double deviation, bool iscoordinates, bool domouseup, double release_radius, bool relative_coordinates, bool track_scroll)
{
    DoMouseUpOnFinishMove = domouseup;
    MouseReleaseRadius = release_radius;
    ScrollTrackingX = 0;
    ScrollTrackingY = 0;
    ScrollStopTrackingStart = 0;
    ScrollStopTracking = 0;
    DoTrackScroll = track_scroll;
    MouseStartX = Data->CursorX;
    MouseStartY = Data->CursorY;
    if(speed>=-0.01)
    {
        MouseSpeed = speed;
    }else
    {
        MouseSpeed = 100.0;
    }
    if(gravity>=-0.01)
    {
        MouseGravity = gravity;
    }else
    {
        MouseGravity = 6.0;
    }
    if(deviation>=-0.01)
    {
        MouseDeviation = deviation;
    }else
    {
        MouseDeviation = 2.5;
    }
    MouseEndX = x;
    MouseEndY = y;
    if(relative_coordinates)
    {
        IsMouseMoveSimulation = true;
        if(Settings->EmulateMouse())
        {
            int t1,t2;
            BrowserEventsEmulator::MouseMove(Data->Connector, IsMouseMoveSimulation, MouseStartX, MouseStartY, MouseEndX, MouseEndY, t1, t2, 0, 0, 0, 0, 0, 0, true, true,Data->IsMousePress,Data->IsDrag, Data->IsTouchScreen,Data->TouchEventId,Data->IsTouchPressedAutomation,TypeTextState);
        }
    }else
    {
        std::string AllowOutOfBounds = iscoordinates ? "true" : "false";

        std::string ScrollToScript = Javascript(std::string("[[RESULT]] = _BAS_HIDE(BrowserAutomationStudio_ScrollToCoordinates)(") + std::to_string(x) + std::string(",") + std::to_string(y) + std::string(",") + AllowOutOfBounds + std::string(")"), "main");
    
        Async Result = Data->Connector->ExecuteJavascript(ScrollToScript, std::string(), std::string("[]"));
        Data->Results->ProcessResult(Result);
        Result->Then([this](AsyncResult* Result)
        {
            JsonParser Parser;
            std::string TextResult = Parser.GetStringFromJson(Result->GetString(),"RESULT");
            this->UpdateScrolls(TextResult);
            std::size_t pos = TextResult.find(",");
            int X = -1, Y = -1;
            if(pos != std::string::npos)
            {
                std::string x_string = TextResult.substr(0,pos);
                std::string y_string = TextResult.substr(pos + 1,TextResult.length() - pos - 1);
                X = std::stoi(x_string);
                Y = std::stoi(y_string);
            }

            if(X < 0)
            {
                this->MouseEndX = X;
            }else
            {
                this->MouseEndX = X - this->Data->ScrollX;
            }

            if(Y < 0)
            {
                this->MouseEndY = Y;
            }else
            {
                this->MouseEndY = Y - this->Data->ScrollY;
            }

            this->IsMouseMoveSimulation = true;
            if(this->Settings->EmulateMouse())
            {
                int t1,t2;
                BrowserEventsEmulator::MouseMove(this->Data->Connector, this->IsMouseMoveSimulation, this->MouseStartX, this->MouseStartY, this->MouseEndX, this->MouseEndY, t1, t2, 0, 0, 0, 0, 0, 0, true, true,this->Data->IsMousePress,this->Data->IsDrag, this->Data->IsTouchScreen,this->Data->TouchEventId,this->Data->IsTouchPressedAutomation,this->TypeTextState);
            }
        });
    }
}

void MainApp::ScrollCallback(int x, int y)
{
    std::string ScrollToScript = Javascript(std::string("_BAS_HIDE(BrowserAutomationStudio_ScrollToCoordinates)(") + std::to_string(x) + std::string(",") + std::to_string(y) + std::string(")"), "main");

    Async Result = Data->Connector->ExecuteJavascript(ScrollToScript, std::string(), std::string("[]"));
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        this->SendTextResponce("<Scroll></Scroll>");
    });
}

void MainApp::RequestVariablesResultCallback(const std::string & data)
{
    if(BrowserScenario)
        BrowserScenario->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_RequestVariablesResult(") + picojson::value(data).serialize() + std::string(")"),"scenario"),BrowserScenario->GetMainFrame()->GetURL(), 0);

    SendTextResponce("<RequestVariablesResult></RequestVariablesResult>");
}

void MainApp::DebugVariablesResultCallback(const std::string & data)
{
    if(BrowserScenario)
        BrowserScenario->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_UpdateVariablesResult(") + picojson::value(data).serialize() + std::string(")"),"scenario"),BrowserScenario->GetMainFrame()->GetURL(), 0);

    SendTextResponce("<DebugVariablesResult></DebugVariablesResult>");
}

void MainApp::DebugCallstackResultCallback(const std::string & data)
{
    if(BrowserScenario)
        BrowserScenario->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_UpdateCallstackResult(") + picojson::value(data).serialize() + std::string(")"),"scenario"),BrowserScenario->GetMainFrame()->GetURL(), 0);

    SendTextResponce("<DebugCallstackResult></DebugCallstackResult>");
}

void MainApp::RenderCallback(int x, int y, int width, int height)
{
    RenderX = x;
    RenderY = y;
    RenderWidth = width;
    RenderHeight = height;

    std::string ScrollToScript = Javascript(std::string("[[RESULT]] = _BAS_HIDE(BrowserAutomationStudio_ScrollToCoordinates)(") + std::to_string(x + width/2) + std::string(",") + std::to_string(y + height/2) + std::string(");"), "main");
    Async Result = Data->Connector->ExecuteJavascript(ScrollToScript, std::string(), std::string("[]"));
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        JsonParser Parser;
        std::string TextResult = Parser.GetStringFromJson(Result->GetString(),"RESULT");
        this->UpdateScrolls(TextResult);

        RenderX = RenderX - Data->ScrollX;
        RenderY = RenderY - Data->ScrollY;
        IsElementRender = false;
        NeedRenderNextFrame = true;
        RenderNextFrameTime = clock() + CLOCKS_PER_SEC * 2;

    });

}


void MainApp::CreateTooboxBrowser()
{
    if(BrowserToolbox)
        return;
    if(!Data->IsRecord)
        return;
    thandler = new ToolboxHandler();
    thandler->EventProcessMessage.push_back(std::bind(&MainApp::ProcessMessage,this,_1,_2,_3,_4));

    CefWindowInfo window_info;

    RECT r =  Layout->GetToolboxRectangle(GetData()->WidthBrowser,GetData()->HeightBrowser,GetData()->WidthAll,GetData()->HeightAll);
    window_info.SetAsChild(Data->_MainWindowHandle,r);

    CefBrowserSettings browser_settings;
    CefRequestContextSettings settings;
    CefRefPtr<CefRequestContext> Context = CefRequestContext::CreateContext(settings,_EmptyRequestContextHandler);

    IsMainBrowserCreating = false;
    BrowserToolbox = CefBrowserHost::CreateBrowserSync(window_info, thandler, "file:///html/toolbox/index.html", browser_settings, CefDictionaryValue::Create(), Context);
    IsMainBrowserCreating = true;

    std::string ToolboxScript = ReadAllString("html/toolbox/index.html");
    ToolboxPreprocess(Data->_ModulesData, Data->_UnusedModulesData, ToolboxScript);
    WriteStringToFile("html/toolbox/index_prepared.html", ToolboxScript);
    BrowserToolbox->GetMainFrame()->LoadURL("file:///html/toolbox/index_prepared.html");

    Layout->ToolBoxHandle = BrowserToolbox->GetHost()->GetWindowHandle();
}

void MainApp::CreateScenarioBrowser()
{
    if(BrowserScenario)
        return;
    if(!Data->IsRecord)
        return;
    shandler = new ScenarioHandler();
    shandler->Zoom = Settings->Zoom();
    shandler->EventProcessMessage.push_back(std::bind(&MainApp::ProcessMessage,this,_1,_2,_3,_4));

    CefWindowInfo window_info;

    RECT r =  Layout->GetDevToolsRectangle(GetData()->WidthBrowser,GetData()->HeightBrowser,GetData()->WidthAll,GetData()->HeightAll);
    window_info.SetAsChild(Data->_MainWindowHandle,r);

    CefBrowserSettings browser_settings;
    CefRequestContextSettings settings;
    CefRefPtr<CefRequestContext> Context = CefRequestContext::CreateContext(settings,_EmptyRequestContextHandler);

    IsMainBrowserCreating = false;
    BrowserScenario = CefBrowserHost::CreateBrowserSync(window_info, shandler, "file:///html/scenario/index.html", browser_settings, CefDictionaryValue::Create(), Context);
    IsMainBrowserCreating = true;

    std::string ScenarioScript = ReadAllString("html/scenario/index.html");
    ScenarioPreprocess(Data->_ModulesData, ScenarioScript);
    WriteStringToFile("html/scenario/index_prepared.html", ScenarioScript);
    BrowserScenario->GetMainFrame()->LoadURL("file:///html/scenario/index_prepared.html");

    Layout->ScenarioHandle = BrowserScenario->GetHost()->GetWindowHandle();
}

void MainApp::CreateDetectorBrowser()
{
    if(BrowserDetector)
        return;
    if(!Data->IsRecord)
        return;
    detecthandler = new DetectorHandler();
    detecthandler->EventProcessMessage.push_back(std::bind(&MainApp::ProcessMessage,this,_1,_2,_3,_4));


    CefWindowInfo window_info;

    RECT r =  Layout->GetDevToolsRectangle(GetData()->WidthBrowser,GetData()->HeightBrowser,GetData()->WidthAll,GetData()->HeightAll);

    window_info.SetAsChild(Data->_MainWindowHandle,r);

    CefBrowserSettings browser_settings;
    CefRequestContextSettings settings;
    CefRefPtr<CefRequestContext> Context = CefRequestContext::CreateContext(settings,_EmptyRequestContextHandler);

    IsMainBrowserCreating = false;
    BrowserDetector = CefBrowserHost::CreateBrowserSync(window_info, detecthandler, "file:///html/detector/index.html", browser_settings, CefDictionaryValue::Create(), Context);
    IsMainBrowserCreating = true;
    if(Settings->Detector())
    {
        std::string ScenarioScript = ReadAllString("html/detector/index.html");
        std::string FingerprintKey = ReadAllString("fingerprint-detector.txt");
        ReplaceAllInPlace(ScenarioScript,"_RestoreFingerprintKey",picojson::value(FingerprintKey).serialize());
        ReplaceAllInPlace(ScenarioScript,"_CurrentLocale",picojson::value(Lang).serialize());
        ReplaceAllInPlace(ScenarioScript,"_RemoteDebuggingPort",std::to_string(Data->MainRemoteDebuggingPort));
        WriteStringToFile("html/detector/index_prepared.html", ScenarioScript);
        BrowserDetector->GetMainFrame()->LoadURL("file:///html/detector/index_prepared.html");
    }else
    {
        BrowserDetector->GetMainFrame()->LoadURL("file:///html/detector/index_disabled.html");
    }
    Layout->DetectorHandle = BrowserDetector->GetHost()->GetWindowHandle();
}

void MainApp::CreateCentralBrowser()
{
    if(BrowserCentral)
        return;
    if(!Data->IsRecord)
        return;
    chandler = new CentralHandler();
    chandler->EventProcessMessage.push_back(std::bind(&MainApp::ProcessMessage,this,_1,_2,_3,_4));


    CefWindowInfo window_info;

    RECT r =  Layout->GetCentralRectangle(GetData()->WidthBrowser,GetData()->HeightBrowser,GetData()->WidthAll,GetData()->HeightAll,true);

    window_info.SetAsChild(Data->_MainWindowHandle,r);

    CefBrowserSettings browser_settings;
    CefRequestContextSettings settings;
    CefRefPtr<CefRequestContext> Context = CefRequestContext::CreateContext(settings,_EmptyRequestContextHandler);

    std::string page = std::string("file:///html/central/index_") + Lang + std::string(".html");

    IsMainBrowserCreating = false;
    BrowserCentral = CefBrowserHost::CreateBrowserSync(window_info, chandler, page, browser_settings, CefDictionaryValue::Create(), Context);
    IsMainBrowserCreating = true;

    Layout->CentralHandle = BrowserCentral->GetHost()->GetWindowHandle();
    Layout->ShowCentralBrowser(false, true);

}

/*void PrintDictionary(CefRefPtr<CefDictionaryValue> Dictionary, int tab)
{
    CefDictionaryValue::KeyList Keys;
    Dictionary->GetKeys(Keys);
    std::string tab_string;
    for(int i = 0;i<tab;i++)
        tab_string += " ";
    for(auto s:Keys)
    {
        WORKER_LOG(tab_string + s.ToString()+ std::string("<<") + std::to_string(Dictionary->GetValue(s)->GetType()));
        switch(Dictionary->GetValue(s)->GetType())
        {
            case VTYPE_BOOL:
                WORKER_LOG(tab_string + std::string("   ") + std::to_string(Dictionary->GetValue(s)->GetBool()));break;
            case VTYPE_INT:
                WORKER_LOG(tab_string + std::string("   ") + std::to_string(Dictionary->GetValue(s)->GetInt()));break;
            case VTYPE_DOUBLE:
                WORKER_LOG(tab_string + std::string("   ") + std::to_string(Dictionary->GetValue(s)->GetDouble()));break;
            case VTYPE_DICTIONARY:
                PrintDictionary(Dictionary->GetValue(s)->GetDictionary(),tab + 3);break;
            case VTYPE_STRING:
                WORKER_LOG(tab_string + std::string("   ") + Dictionary->GetValue(s)->GetString().ToString());break;
        }
    }
}*/


void MainApp::AfterReadyToCreateBrowser(bool Reload)
{
    WORKER_LOG(std::string("LoadCallback create new ") + NextLoadPage);

    CefWindowInfo window_info;

    window_info.SetAsWindowless(0);

    CefBrowserSettings browser_settings;
    browser_settings.windowless_frame_rate = 30;
    browser_settings.background_color = CefColorSetARGB(255, 255, 255, 255);

    /*if(Settings->Webgl() == "disable")
        browser_settings.webgl = STATE_DISABLED;
    else*/
    browser_settings.webgl = STATE_ENABLED;

    browser_settings.plugins = STATE_ENABLED;

    std::wstring wencoding = L"UTF-8";
    cef_string_utf16_set(wencoding.data(),wencoding.size(),&browser_settings.default_encoding,true);

    CefRefPtr<CefRequestContext> Context = CefRequestContext::CreateContext(CefRequestContext::GetGlobalContext(),this);

    {
        CefRefPtr<CefValue> Value = CefValue::Create();
        Value->SetInt(1);
        CefString Error;
        Context->SetPreference("profile.default_content_setting_values.plugins",Value,Error);
        WORKER_LOG(std::string("Error enable flash<<") + Error.ToString());
    }

    {
        CefRefPtr<CefValue> Value = CefValue::Create();
        CefRefPtr<CefDictionaryValue> Dictionary = CefDictionaryValue::Create();

        WORKER_LOG("System Proxy");
        Dictionary->SetString("mode","direct");

        CefString Error;
        Value->SetDictionary(Dictionary);
        Context->SetPreference("proxy",Value,Error);
        WORKER_LOG(std::string("Error setting proxy<<") + Error.ToString());

    }

    //PrintDictionary(Context->GetAllPreferences(true),3);

    if(Reload || !_HandlersManager->GetBrowser())
    {
        WORKER_LOG("!!!CREATENEWBROWSER!!!");

        _HandlersManager->Init2(CefBrowserHost::CreateBrowserSync(window_info, _HandlersManager->GetHandler(), "about:blank", browser_settings, CefDictionaryValue::Create(), Context));
        Layout->BrowserHandle = _HandlersManager->GetBrowser()->GetHost()->GetWindowHandle();
        _HandlersManager->GetBrowser()->GetMainFrame()->LoadURL(NextLoadPage);

    CreateTooboxBrowser();
    CreateScenarioBrowser();
    CreateCentralBrowser();
        if(Settings->DebugScenario() || Settings->DebugToolbox())
        {
            ToggleDevTools();
        }
    }else
    {
        WORKER_LOG("!!!OPTIMIZEDRELOAD!!!");
        _HandlersManager->GetBrowser()->GetMainFrame()->LoadURL(NextLoadPage);
    }
}

void MainApp::TimezoneCallback(int offset)
{
    //For backward compability only
    SendTextResponce("<Timezone></Timezone>");
}

void MainApp::VisibleCallback(bool visible)
{
    WORKER_LOG(std::string("VisibleCallback ") + std::to_string(visible));
    if(visible)
    {
        if(!Data->IsRecord)
            MoveWindow(Data->_MainWindowHandle, rand() % 100, rand() % 100, Data->WidthBrowser + 100, Data->HeightBrowser + 140, false);

        ShowWindow(Data->_MainWindowHandle, SW_SHOW);
        //MoveWindow(Data->MainWindowHandle,0,0,rect.right-rect.left,rect.bottom-rect.top,true);
        SetForegroundWindow(Data->_MainWindowHandle);
        Layout->Focus();
        Data->Connector->DisableBackgroundMode();
    }else
    {
        ShowWindow(Data->_MainWindowHandle, SW_HIDE);
        Data->Connector->EnableBackgroundMode();
    }
}

void MainApp::FlushCallback()
{
    long long now = duration_cast< milliseconds >( system_clock::now().time_since_epoch() ).count();
    BrowserCloseTime = now + rand()%3000;
}

void MainApp::Hide()
{
    ShowWindow(Data->_MainWindowHandle, SW_HIDE);
    Data->Connector->EnableBackgroundMode();
}

void MainApp::ToggleDevTools()
{
    if(!Data->IsRecord)
        return;

    if(Settings->DebugScenario())
    {
        WORKER_LOG("ToggleDevTools Scenario");
        if(!BrowserScenario.get())
            return;

        CefWindowInfo window_info;
        window_info.SetAsPopup(0, "");
        CefBrowserSettings browser_settings;
        BrowserScenario->GetHost()->ShowDevTools(window_info, dhandler, browser_settings, CefPoint(0,0));

    }else if(Settings->DebugToolbox())
    {
        WORKER_LOG("ToggleDevTools Toolbox");

        if(!BrowserToolbox.get())
            return;

        CefWindowInfo window_info;
        window_info.SetAsPopup(0, "");
        CefBrowserSettings browser_settings;
        BrowserToolbox->GetHost()->ShowDevTools(window_info, dhandler, browser_settings, CefPoint(0,0));
    }else
    {
        WORKER_LOG("ToggleDevTools");
        Data->Connector->OpenDevTools();
    }
}

void MainApp::InspectAt(int x, int y)
{
    Data->Connector->InspectAt(x, y);
}

void MainApp::RepeatInspectMouseAt()
{
    MouseMoveAt(InspectX,InspectY);
}

void MainApp::IncreaseInspectPosition()
{
    InspectPosition++;
    RepeatInspectMouseAt();
}


void MainApp::DecreaseInspectPosition()
{
    InspectPosition--;
    if(InspectPosition < 0)
        InspectPosition = 0;
    RepeatInspectMouseAt();
}


void MainApp::MouseMoveAt(int x, int y)
{
    clock_t CurrentTime = clock();
    float time_spent = float( CurrentTime - LastMouseTrack ) /  CLOCKS_PER_SEC;

    if(InspectTask && time_spent < 3)
        return;

    if(time_spent < 0.1)
        return;

    BrowserDirectControl::InspectTask Task = DirectControl()->GetInspectTask();

    if(Task.IsNull)
        InspectMouseAt(x, y, CurrentTime);
    else
        InspectMouseAt(Task.X, Task.Y, CurrentTime);
}

void MainApp::InspectMouseAt(int x, int y, clock_t CurrentTime)
{
    if(InspectX != x || InspectY != y)
    {
        InspectPosition = 0;
    }
    InspectX = x;
    InspectY = y;
    LastMouseTrack = CurrentTime;

    if(InspectTask)
    {
        Data->Connector->InterruptAction(InspectTask->GetActionUniqueId());
        InspectTask.reset();
    }

    InspectTask = Data->Connector->Inspect(x,y,InspectPosition);
}

void MainApp::MouseLeave()
{
    {
        LOCK_BROWSER_DATA
        Data->_Inspect.active = false;
    }
    RECT r = Layout->GetBrowserRectangle(GetData()->WidthBrowser,GetData()->HeightBrowser,GetData()->WidthAll,GetData()->HeightAll);
    InvalidateRect(Data->_MainWindowHandle,&r,false);
}

void MainApp::SetProxyCallback(const std::string& server, int Port, bool IsHttp, const std::string& username, const std::string& password, const std::string& target)
{
    WORKER_LOG(std::string("SetProxyCallback ") + server + std::string(" ") + std::to_string(Port) + std::string(" ") + target);
    Data->IsProxySet = !server.empty();
    UpdateBrowserData(Data);
    Async Result = Data->Connector->SetProxy(server, Port, IsHttp, username, password);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        this->SendTextResponce("<SendWorkerSettings></SendWorkerSettings>");
    });
}

void MainApp::OnComplete()
{
    WORKER_LOG("MainApp::OnComplete");
    if(!ProxyLibraryLoaded)
    {
        IPCSimple::Write(std::string("out") + Data->_UniqueProcessId,"attach-proxy");
    }else
    {
        SendTextResponce("<SendWorkerSettings></SendWorkerSettings>");
    }
}

CefRefPtr<CefResourceRequestHandler> MainApp::GetResourceRequestHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_navigation, bool is_download, const CefString& request_initiator, bool& disable_default_handling)
{
    //Never use default request handler for dev tools
    if(starts_with(request->GetURL().ToString(),"devtools:") || (browser && browser->GetMainFrame() && starts_with(browser->GetMainFrame()->GetURL().ToString(),"devtools:")))
    return NULL;

    int BrowserId = -1;
    if(!browser && _HandlersManager->GetBrowser())
    {
        BrowserId = _HandlersManager->GetBrowser()->GetIdentifier();
    }

    if(BrowserId != -1)
    {
        CefResourceRequestHandler * Res = _HandlersManager->GetHandlerForBrowserId(BrowserId);
        return Res;
    }
}


void MainApp::AddHeaderCallback(const std::string& key,const std::string& value, const std::string& target)
{
    auto i = Data->_Headers.begin();
    while (i != Data->_Headers.end())
    {
        if(i->first == key)
        {
            i = Data->_Headers.erase(i);
        }else
        {
            ++i;
        }
    }

    if(!value.empty())
        Data->_Headers.push_back(std::pair<std::string,std::string>(key, value));
    UpdateBrowserData(Data);

    std::vector<std::pair<std::string,std::string> > HeadersCopy = PrepareHeaders(Data);

    Async Result = Data->Connector->SetHeaders(HeadersCopy);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        SendTextResponce("<AddHeader></AddHeader>");
    });

}

void MainApp::SetHeaderListCallback(const std::string& json)
{
    //Headers order, this function is not supported, _settings is used to headers order
    SendTextResponce("<SetHeaderList></SetHeaderList>");
}

void MainApp::SetAcceptLanguagePatternCallback(const std::string& pattern)
{
    Data->_AcceptLanguagePattern = pattern;
    UpdateBrowserData(Data);
    std::vector<std::pair<std::string,std::string> > HeadersCopy = PrepareHeaders(Data);

    Async Result = Data->Connector->SetHeaders(HeadersCopy);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        SendTextResponce("<SetAcceptLanguagePattern></SetAcceptLanguagePattern>");
    });
}


void MainApp::SetUserAgentDataCallback(const std::string& data)
{
    Data->_UserAgentDataBase64 = data;
    UpdateBrowserData(Data);
    std::vector<std::pair<std::string,std::string> > HeadersCopy = PrepareHeaders(Data);

    Async Result = Data->Connector->SetUserAgentData(data, HeadersCopy);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        SendTextResponce("<SetUserAgentData></SetUserAgentData>");
    });
}

void MainApp::SetUserAgentCallback(const std::string& value)
{
    /*{
        LOCK_BROWSER_DATA
        if(value.empty())
            Data->_Headers.erase("User-Agent");
        else
            Data->_Headers["User-Agent"] = value;
    }*/
    //SendTextResponce("<SetUserAgent>1</SetUserAgent>");
}

void MainApp::PrepareFunctionCallback(const std::string& value)
{
    if(BrowserScenario)
    {
        BrowserScenario->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_PrepareFunction(") + picojson::value(value).serialize() + std::string(")"),"scenario"),BrowserScenario->GetMainFrame()->GetURL(), 0);
    }
}

void MainApp::BackupDoneCallback(const std::string& full_path)
{
    if(BrowserScenario)
    {
        BrowserScenario->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_FinishedBackup(") + picojson::value(full_path).serialize() + std::string(")"),"scenario"),BrowserScenario->GetMainFrame()->GetURL(), 0);
    }
}

void MainApp::RecaptchaV3ListCallback(const std::string& value)
{
    Data->_RecaptchaV3List = value;

    UpdateBrowserData(Data);

    Async Result = Data->Connector->Sleep(2000);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        SendTextResponce("<RecaptchaV3List></RecaptchaV3List>");
    });


}

void MainApp::ClickExtensionButton(const std::string& id)
{
    Data->Connector->TriggerExtensionButton(id);
    this->SendTextResponce(std::string("<ClickExtensionButton></ClickExtensionButton>"));
}

void MainApp::RecaptchaV3ResultCallback(const std::string& id, const std::string& result)
{
    std::string Js = Javascript(std::string("_BAS_HIDE(BrowserAutomationStudio_RecaptchaV3Solved)(") + picojson::value(id).serialize() + std::string(", ") + picojson::value(result).serialize() + std::string(");"),"main");

    Async Result = Data->Connector->ExecuteJavascript(Js,std::string(),std::string("[]"));
    Data->Results->ProcessResult(Result);
}

void MainApp::CleanHeaderCallback()
{
    Data->_Headers.clear();
    UpdateBrowserData(Data);
    Async Result = Data->Connector->SetHeaders(Data->_Headers);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        SendTextResponce("<CleanHeader></CleanHeader>");
    });
}

void MainApp::GetUrlCallback()
{
    Async Result = Data->Connector->GetCurrentUrl();
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        std::string Url = Result->GetString();
        xml_encode(Url);
        SendTextResponce(std::string("<GetUrl>") + Url + std::string("</GetUrl>"));
    });

}

void MainApp::GetBrowserScreenSettingsCallback()
{
    picojson::object result;
    result["CursorX"] = picojson::value((double)Data->CursorX);
    result["CursorY"] = picojson::value((double)Data->CursorY);
    result["ScrollX"] = picojson::value((double)Data->ScrollX);
    result["ScrollY"] = picojson::value((double)Data->ScrollY);
    result["Width"] = picojson::value((double)Data->WidthBrowser);
    result["Height"] = picojson::value((double)Data->HeightBrowser);

    std::string result_string = picojson::value(result).serialize();
    xml_encode(result_string);
    SendTextResponce(std::string("<GetBrowserScreenSettings>") + result_string + std::string("</GetBrowserScreenSettings>"));
}

std::string MainApp::GetUrl()
{
    std::string url;

    if(_HandlersManager->GetBrowser())
    {
        url = _HandlersManager->GetBrowser()->GetMainFrame()->GetURL();
    }

    return url;
}

void MainApp::ProcessContextMenu(int MenuId)
{
    Data->_BrowserContextMenu.Process(Data->_MainWindowHandle, MenuId, Data->Connector, Data->_UniqueProcessId);
}

void MainApp::ProcessFind(LPFINDREPLACE lpfr)
{
    Data->_BrowserContextMenu.OnFind(Data->Connector, lpfr);
}


void MainApp::OnBeforeCommandLineProcessing(const CefString& process_type,CefRefPtr<CefCommandLine> command_line)
{
    //command_line->AppendSwitch("--single-process");
    command_line->AppendSwitch("--high-dpi-support");
    command_line->AppendSwitch("--disable-site-isolation-trials");

    //command_line->AppendSwitch("--disable-gpu");
    //command_line->AppendSwitch("--disable-gpu-compositing");
    //command_line->AppendSwitch("--disable-gpu-vsync");

    for(auto p:ParseChromeCommandLine())
    {

        if(p.second.length()>0)
        {

            WORKER_LOG("ChromeCommandLine<<" + p.first + "=" + p.second);
            command_line->AppendSwitchWithValue(p.first,p.second);

        }else
        {
            //bool IsGPUSetting = (p.first == "--disable-gpu") || (p.first == "--disable-gpu-compositing");
            bool IsGPUSetting = false;
            //bool DisableWebgl = Settings->Webgl() == "disable";
            bool DisableWebgl = false;

            if(
                    (!IsGPUSetting || (IsGPUSetting && DisableWebgl))
                    &&
                    (!Data->IsRecord || p.first != "--disable-threaded-compositing")
              )
            {
                WORKER_LOG("ChromeCommandLine<<" + p.first);
                command_line->AppendSwitch(p.first);
            }

        }
    }

}

void MainApp::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line)
{
    command_line->AppendSwitchWithValue("parent-process-id",std::to_string(GetCurrentProcessId()));
    if(IsMainBrowserCreating)
    {
        command_line->AppendSwitchWithValue("unique-process-id",Settings->UniqueProcessId());
    }

}

bool MainApp::IsNeedQuit()
{
    if(!_HandlersManager->GetHandler())
        return false;
    
    return _HandlersManager->GetHandler()->IsNeedQuit();
}

void MainApp::SendTextResponce(const std::string& text)
{
    for(auto f:EventSendTextResponce)
        f(text);
}


void MainApp::AddCacheMaskAllowCallback(const std::string& value)
{
    std::pair<bool, std::string> data;
    data.first = true;
    data.second = value;
    Data->_CacheMask.push_back(data);
    Data->Connector->SetCacheMasks(Data->_CacheMask);
    SendTextResponce("<AddCacheMaskAllow/>");
}
void MainApp::AddCacheMaskDenyCallback(const std::string& value)
{
    std::pair<bool, std::string> data;
    data.first = false;
    data.second = value;
    Data->_CacheMask.push_back(data);
    Data->Connector->SetCacheMasks(Data->_CacheMask);
    SendTextResponce("<AddCacheMaskDeny/>");
}
void MainApp::AddRequestMaskAllowCallback(const std::string& value)
{
    std::pair<bool, std::string> data;
    data.first = true;
    data.second = value;
    Data->_RequestMask.push_back(data);
    UpdateBrowserData(Data);

    Async Result = Data->Connector->SetRequestsRestrictions(Data->_RequestMask);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        SendTextResponce("<AddRequestMaskAllow/>");
    });
}

void MainApp::AddRequestMaskDenyCallback(const std::string& value)
{
    std::pair<bool, std::string> data;
    data.first = false;
    data.second = value;
    Data->_RequestMask.push_back(data);
    UpdateBrowserData(Data);

    Async Result = Data->Connector->SetRequestsRestrictions(Data->_RequestMask);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        SendTextResponce("<AddRequestMaskDeny/>");
    });
}
void MainApp::ClearCacheMaskCallback()
{
    Data->_CacheMask.clear();
    Data->Connector->SetCacheMasks(Data->_CacheMask);
    SendTextResponce("<ClearCacheMask/>");
}

void MainApp::AllowPopups()
{
    Data->Connector->AllowPopups();
    SendTextResponce("<AllowPopups/>");
}

void MainApp::RestrictPopups()
{
    Data->Connector->RestrictPopups();
    SendTextResponce("<RestrictPopups/>");
}

void MainApp::AllowDownloads()
{
    Async Result = Data->Connector->AllowDownloads();
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        SendTextResponce("<AllowDownloads/>");
    });

}

void MainApp::RestrictDownloads()
{
    Async Result = Data->Connector->RestrictDownloads();
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        SendTextResponce("<RestrictDownloads/>");
    });
}

void MainApp::ClearRequestMaskCallback()
{
    Data->_RequestMask.clear();
    UpdateBrowserData(Data);
    Async Result = Data->Connector->SetRequestsRestrictions(Data->_RequestMask);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        SendTextResponce("<ClearRequestMask/>");
    });

}
void MainApp::ClearLoadedUrlCallback()
{
    Data->Connector->ClearNetworkData();
    SendTextResponce("<ClearLoadedUrl/>");
}
void MainApp::ClearCachedDataCallback()
{
    Data->Connector->ClearNetworkData();
    SendTextResponce("<ClearCachedData/>");
}
void MainApp::ClearAllCallback()
{
    Data->_CacheMask.clear();
    Data->Connector->SetCacheMasks(Data->_CacheMask);

    Data->Connector->ClearNetworkData();

    Data->_RequestMask.clear();
    UpdateBrowserData(Data);
    Async Result = Data->Connector->SetRequestsRestrictions(Data->_RequestMask);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        SendTextResponce("<ClearAll/>");
    });

}
void MainApp::ClearMasksCallback()
{
    Data->_CacheMask.clear();
    Data->Connector->SetCacheMasks(Data->_CacheMask);
    Data->_RequestMask.clear();
    UpdateBrowserData(Data);
    Async Result = Data->Connector->SetRequestsRestrictions(Data->_RequestMask);
    Data->Results->ProcessResult(Result);
    Result->Then([this](AsyncResult* Result)
    {
        SendTextResponce("<ClearMasks/>");
    });

}
void MainApp::ClearDataCallback()
{
    Data->Connector->ClearNetworkData();
    SendTextResponce("<ClearData/>");
}
void MainApp::WaitCodeCallback()
{
    if(!_HandlersManager->GetBrowser())
    {
        NextLoadPage = "about:blank";
    AfterReadyToCreateBrowser(true);
    }
    CreateTooboxBrowser();
    CreateScenarioBrowser();
    Layout->UpdateState(MainLayout::Ready);
    if(BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript("BrowserAutomationStudio_HideWaiting()","toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
    if(BrowserScenario)
        BrowserScenario->GetMainFrame()->ExecuteJavaScript(Javascript("BrowserAutomationStudio_NotRunningTask()","scenario"),BrowserScenario->GetMainFrame()->GetURL(), 0);
    Layout->UpdateTabs();
}

void MainApp::StartSectionCallback(int Id)
{
    if(Data->SetIndirectControlOnNext)
    {
        Data->SetIndirectControlOnNext = false;
        Data->ManualControl = BrowserData::Indirect;
        UpdateManualControl();
    }

    if(!_HandlersManager->GetBrowser())
    {
        NextLoadPage = "about:blank";
        AfterReadyToCreateBrowser(true);
    }
    CreateTooboxBrowser();
    CreateScenarioBrowser();
    Layout->UpdateState(MainLayout::Ready);
    if(BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript("BrowserAutomationStudio_HideWaiting()","toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
    if(BrowserScenario && scenariov8handler && scenariov8handler->GetIsInitialized())
    {
        BrowserScenario->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_NotRunningTask(") + std::to_string(Id) + std::string(")"),"scenario"),BrowserScenario->GetMainFrame()->GetURL(), 0);
    }else
    {
        SetNextActionId = std::to_string(Id);
    }
    Layout->UpdateTabs();
}

void MainApp::ScriptFinishedCallback()
{
    WORKER_LOG("ScriptFinishedCallback");
    Layout->UpdateState(MainLayout::Finished);
    if(BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript("BrowserAutomationStudio_HideWaiting()","toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
    if(BrowserScenario)
        BrowserScenario->GetMainFrame()->ExecuteJavaScript(Javascript("BrowserAutomationStudio_RunningTask()","scenario"),BrowserScenario->GetMainFrame()->GetURL(), 0);
    Hide();
}

void MainApp::FindCacheByMaskBase64Callback(const std::string& value)
{
    std::string res;

    if(value == "download://*")
    {
        res = Data->Connector->GetDownloadedFilePath();
        res = base64_encode((unsigned char const *)res.data(),res.size());
    }else
    {
        res = Data->Connector->GetSingleCacheData(value, true);
    }

    xml_encode(res);
    SendTextResponce(std::string("<FindCacheByMaskBase64>") + res + ("</FindCacheByMaskBase64>"));
}

void MainApp::FindStatusByMaskCallback(const std::string& value)
{
    std::string res = "0";
    res = std::to_string(Data->Connector->GetStatusForURL(value));
    SendTextResponce(std::string("<FindStatusByMask>") + res + ("</FindStatusByMask>"));
}

void MainApp::FindUrlByMaskCallback(const std::string& value)
{
    std::string res;
    res = Data->Connector->FindLoadedURL(value);
    xml_encode(res);
    SendTextResponce(std::string("<FindUrlByMask>") + res + ("</FindUrlByMask>"));
}

void MainApp::GetLoadStatsCallback()
{
    int is_loading = Data->Connector->IsLoading();

    int64 Oldest = Data->_RequestList.Oldest();

    SendTextResponce(std::string("<GetLoadStats>") + std::to_string(is_loading) + "," + std::to_string(Oldest) + std::string("</GetLoadStats>"));
    return;
}


void MainApp::FindCacheByMaskStringCallback(const std::string& value)
{
    std::string res = "";

    if(value == "download://*")
    {
        res = Data->Connector->GetDownloadedFilePath();
    }else
    {
        res = Data->Connector->GetSingleCacheData(value, false);
    }

    xml_encode(res);
    SendTextResponce(std::string("<FindCacheByMaskString>") + res + ("</FindCacheByMaskString>"));

    /*WORKER_LOG(std::string("FindCacheByMaskStringCallback<<") + value);
    std::string res = "";
    {
        LOCK_BROWSER_DATA
        //Search backward
        for(vector<std::pair<std::string, std::shared_ptr<BrowserData::CachedItem> > >::reverse_iterator i = Data->_CachedData.rbegin(); i != Data->_CachedData.rend(); ++i )
        {
            if(match(value,i->first) || match(urlnormalize(value),urlnormalize(i->first)))
            {
                res = std::string(i->second->body.begin(),i->second->body.end());
                break;
            }
        }
        if(value == "download://*")
        {
            //errase all info about previous download
            auto i = Data->_LoadedUrls.begin();
            while (i != Data->_LoadedUrls.end())
            {
                if(starts_with(i->first,"download://"))
                {
                    i = Data->_LoadedUrls.erase(i);
                }else
                {
                    ++i;
                }
            }
        }
    }
    xml_encode(res);
    SendTextResponce(std::string("<FindCacheByMaskString>") + res + std::string("</FindCacheByMaskString>"));*/
}


void MainApp::FindAllCacheCallback(const std::string& value)
{
    std::string result_string = Data->Connector->GetAllCacheData(value);
    xml_encode(result_string);
    SendTextResponce(std::string("<FindAllCache>") + result_string + std::string("</FindAllCache>"));
}


void MainApp::IsUrlLoadedByMaskCallback(const std::string& value)
{
    std::string res = "0";


    if(value == "download://*")
    {
        res = Data->Connector->IsFileDownloadReady() ? "1" : "0";
    }else
    {
        res = Data->Connector->IsURLLoaded(value) ? "1" : "0";
    }

    SendTextResponce(std::string("<IsUrlLoadedByMask>") + res + ("</IsUrlLoadedByMask>"));

}

void MainApp::SetCodeCallback(const std::string & code,const std::string & embedded,const std::string & schema,bool is_testing, const std::string & interface_state, const std::string & script_engine_version, const std::string & application_engine_version)
{
    ApplicationEngineVersion = application_engine_version;
    ScriptEngineVersion = script_engine_version;
    InterfaceState = interface_state;
    Data->IsTesing = is_testing;
    Schema = schema;
    Code = code;
    EmbeddedData = embedded;
    if(code.empty())
        Code = " ";
    Variables = extract_variables(code);
    GlobalVariables = extract_global_variables(code);
    Labels = extract_labels(code);
    Functions = extract_functions(code);
    std::string AdditionalResourcesPrev = AdditionalResources;
    AdditionalResources = extract_resources(code);
    if(AdditionalResourcesPrev!=AdditionalResources)
        ResourcesChanged = true;
}

void MainApp::SetResourceCallback(const std::string & resources)
{
    Resources = resources;
    ResourcesChanged = true;
}

void MainApp::CrushCallback()
{
    *((unsigned int*)0) = 0xDEAD;
}

void MainApp::SetInitialStateCallback(const std::string & lang)
{
    Lang = lang;
}

void MainApp::SetNextActionCallback(const std::string& NextActionId)
{
    if(scenariov8handler && scenariov8handler->GetIsInitialized())
    {
        if(BrowserScenario)
            BrowserScenario->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_NotRunningTask(") + NextActionId + std::string(")"),"scenario"),BrowserScenario->GetMainFrame()->GetURL(), 0);
    }else
    {
        SetNextActionId = NextActionId;
    }
}

void MainApp::ClearElementCommand()
{
    WORKER_LOG("ClearElementCommand");
    IsLastCommandNull = true;
    ExecuteFrameChain.clear();
    ExecuteFrameSearching = false;
    ExecuteFrameScrolling = false;
    ExecuteFrameScrollingSwitch = false;
    ExecuteSearchCoordinatesX = 0;
    ExecuteSearchCoordinatesY = 0;
    RunElementCommandCallbackOnNextTimer = -1;
    TypeTextTaskIsActive = false;
    IsMouseMoveSimulation = false;
}
void MainApp::ElementCommandCallback(const ElementCommand &Command)
{
    ClearElementCommand();
    LastCommandCopy = Command;
    LastCommand = Command;
    IsLastCommandNull = false;
    bool IsNoWait = LastCommand.IsNoWait;

    if(LastCommand.CommandName == "script2")
    {
        std::string Script = Javascript(LastCommand.CommandParam1,"main");
        std::string Variables = LastCommand.CommandParam2;
        std::string Path = LastCommand.SerializePath();
        Async Result = Data->Connector->ExecuteJavascript(Script,Variables,Path);
        Data->Results->ProcessResult(Result);

        std::string CommandId = LastCommand.CommandId;
        std::string CommandName = LastCommand.CommandName;

        Result->Then([this, CommandId, CommandName, IsNoWait](AsyncResult* Result)
        {
            if(!IsNoWait && !Result->GetIsSuccess() && Result->GetErrorMessage() == "BAS_NOT_EXISTS")
            {
                RunElementCommandCallbackOnNextTimer = 100;
                return;
            }
            std::string Data = Result->GetRawData();
            xml_encode(Data);
            SendTextResponce(std::string("<Element ID=\"") + CommandId + std::string("\"><") + CommandName + std::string(">") + Data + std::string("</") + CommandName + ("></Element>"));
            IsLastCommandNull = true;
        });
    }

    if(LastCommand.CommandName == "script")
    {
        std::string Script = Javascript(std::string("[[RESULT]] = ") + LastCommand.CommandParam1 + std::string(";[[RESULT]] = _BAS_HIDE(BrowserAutomationStudio_ToString)([[RESULT]]);"),"main");

        std::string Path = LastCommand.SerializePath();
        Async Result = Data->Connector->ExecuteJavascript(Script,std::string(),Path);
        Data->Results->ProcessResult(Result);

        std::string CommandId = LastCommand.CommandId;
        std::string CommandName = LastCommand.CommandName;

        Result->Then([this, CommandId, CommandName, IsNoWait](AsyncResult* Result)
        {
            std::string Data;
            if(Result->GetIsSuccess())
            {
                picojson::value v;
                picojson::parse(v, Result->GetString());
                picojson::value Result = v.get<picojson::value::object>()["RESULT"];
                if(Result.is<std::string>())
                {
                    Data = Result.get<std::string>();
                }
            }else if(!IsNoWait && Result->GetErrorMessage() == "BAS_NOT_EXISTS")
            {
                RunElementCommandCallbackOnNextTimer = 100;
                return;
            }
            xml_encode(Data);
            SendTextResponce(std::string("<Element ID=\"") + CommandId + std::string("\"><") + CommandName + std::string(">") + Data + std::string("</") + CommandName + ("></Element>"));
            IsLastCommandNull = true;
        });
    }

    if(LastCommand.CommandName == "exist")
    {
        std::string Script = Javascript(std::string("[[RESULT]] = ((self === null) ? '0' : '1')"),"main");

        std::string Path = LastCommand.SerializePath();
        Async Result = Data->Connector->ExecuteJavascript(Script,std::string(),Path);
        Data->Results->ProcessResult(Result);

        std::string CommandId = LastCommand.CommandId;
        std::string CommandName = LastCommand.CommandName;

        Result->Then([this, CommandId, CommandName](AsyncResult* Result)
        {
            std::string Data("0");
            if(Result->GetIsSuccess())
            {
                picojson::value v;
                picojson::parse(v, Result->GetString());
                picojson::value Result = v.get<picojson::value::object>()["RESULT"];
                if(Result.is<std::string>())
                {
                    Data = Result.get<std::string>();
                }
            }
            xml_encode(Data);
            SendTextResponce(std::string("<Element ID=\"") + CommandId + std::string("\"><") + CommandName + std::string(">") + Data + std::string("</") + CommandName + ("></Element>"));
            IsLastCommandNull = true;
        });
    }

    if(LastCommand.CommandName == "highlight")
    {
        std::string MultiselectScript;
        if(Data->_MultiSelectData.IsDirty)
        {
            MultiselectScript = std::string("_BAS_HIDE(BrowserAutomationStudio_SetMultiSelectData)(") + Data->_MultiSelectData.Serialize() + std::string("); ");
            Data->_MultiSelectData.IsDirty = false;
        }

        std::string Script = Javascript(MultiselectScript + std::string(";[[RESULT]] = '0';if(typeof(self.length) == 'number')[[RESULT]] = _BAS_HIDE(BrowserAutomationStudio_ToString)(self.length); else if(self)[[RESULT]] = '1'; else [[RESULT]] = '0';"),"main");

        std::string Path = LastCommand.SerializePath();
        HighlightSelector = Path;
        RawHighlightSelector = LastCommand.CommandParam1;
        HighlightIndex = -1;
        HighlightDoScrolling = false;
        LastHighlightIndexChanged = 0;
        IsHighlightIndexActive = false;

        Async Result = Data->Connector->ExecuteJavascript(Script,std::string(),Path);

        std::string CommandId = LastCommand.CommandId;
        std::string CommandName = LastCommand.CommandName;

        Result->Then([this, CommandId, CommandName](AsyncResult* Result)
        {
            std::string Data("0");
            if(Result->GetIsSuccess())
            {
                picojson::value v;
                picojson::parse(v, Result->GetString());
                picojson::value Result = v.get<picojson::value::object>()["RESULT"];
                if(Result.is<std::string>())
                {
                    Data = Result.get<std::string>();
                }
            }
            xml_encode(Data);
            SendTextResponce(std::string("<Element ID=\"") + CommandId + std::string("\"><") + CommandName + std::string(">") + Data + std::string("</") + CommandName + ("></Element>"));

            if(this->BrowserToolbox)
                this->BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_SetPathCount(") + Data + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);

            IsLastCommandNull = true;

        });
    }

    if(LastCommand.CommandName == "length")
    {
        std::string Script = Javascript(std::string("[[RESULT]] = _BAS_HIDE(BrowserAutomationStudio_ToString)(self.length)"),"main");

        std::string Path = LastCommand.SerializePath();
        Async Result = Data->Connector->ExecuteJavascript(Script,std::string(),Path);
        Data->Results->ProcessResult(Result);

        std::string CommandId = LastCommand.CommandId;
        std::string CommandName = LastCommand.CommandName;

        Result->Then([this, CommandId, CommandName](AsyncResult* Result)
        {
            std::string Data("0");
            if(Result->GetIsSuccess())
            {
                picojson::value v;
                picojson::parse(v, Result->GetString());
                picojson::value Result = v.get<picojson::value::object>()["RESULT"];
                if(Result.is<std::string>())
                {
                    Data = Result.get<std::string>();
                }
            }
            xml_encode(Data);
            SendTextResponce(std::string("<Element ID=\"") + CommandId + std::string("\"><") + CommandName + std::string(">") + Data + std::string("</") + CommandName + ("></Element>"));
            IsLastCommandNull = true;
        });
    }

    if(LastCommand.CommandName == "xml")
    {
        std::string Script = Javascript(std::string("if(!self)throw 'BAS_NOT_EXISTS';[[RESULT]] = _BAS_SAFE($Node.outerHTML)(self)"),"main");

        std::string Path = LastCommand.SerializePath();
        Async Result = Data->Connector->ExecuteJavascript(Script,std::string(),Path);
        Data->Results->ProcessResult(Result);

        std::string CommandId = LastCommand.CommandId;
        std::string CommandName = LastCommand.CommandName;

        Result->Then([this, CommandId, CommandName, IsNoWait](AsyncResult* Result)
        {
            std::string Data;
            if(Result->GetIsSuccess())
            {
                picojson::value v;
                picojson::parse(v, Result->GetString());
                picojson::value Result = v.get<picojson::value::object>()["RESULT"];
                if(Result.is<std::string>())
                {
                    Data = Result.get<std::string>();
                }
            }else if(!IsNoWait && Result->GetErrorMessage() == "BAS_NOT_EXISTS")
            {
                RunElementCommandCallbackOnNextTimer = 100;
                return;
            }
            xml_encode(Data);
            SendTextResponce(std::string("<Element ID=\"") + CommandId + std::string("\"><") + CommandName + std::string(">") + Data + std::string("</") + CommandName + ("></Element>"));
            IsLastCommandNull = true;
        });
    }

    if(LastCommand.CommandName == "text")
    {
        std::string Script = Javascript(std::string("if(!self)throw 'BAS_NOT_EXISTS';if(_BAS_SAFE($String.toLowerCase)(_BAS_SAFE($Node.tagName)(self))=='input'||_BAS_SAFE($String.toLowerCase)(_BAS_SAFE($Node.tagName)(self))=='textarea')"
                                                        "[[RESULT]]=_BAS_SAFE($Node.value)(self);"
                                                    "else "
                                                        "[[RESULT]]=_BAS_SAFE($Node.textContent)(self);"),"main");

        std::string Path = LastCommand.SerializePath();
        Async Result = Data->Connector->ExecuteJavascript(Script,std::string(),Path);
        Data->Results->ProcessResult(Result);

        std::string CommandId = LastCommand.CommandId;
        std::string CommandName = LastCommand.CommandName;

        Result->Then([this, CommandId, CommandName, IsNoWait](AsyncResult* Result)
        {
            std::string Data;
            if(Result->GetIsSuccess())
            {
                picojson::value v;
                picojson::parse(v, Result->GetString());
                picojson::value Result = v.get<picojson::value::object>()["RESULT"];
                if(Result.is<std::string>())
                {
                    Data = Result.get<std::string>();
                }
            }else if(!IsNoWait && Result->GetErrorMessage() == "BAS_NOT_EXISTS")
            {
                RunElementCommandCallbackOnNextTimer = 100;
                return;
            }
            xml_encode(Data);
            SendTextResponce(std::string("<Element ID=\"") + CommandId + std::string("\"><") + CommandName + std::string(">") + Data + std::string("</") + CommandName + ("></Element>"));
            IsLastCommandNull = true;
        });
    }


    if(LastCommand.CommandName == "focus")
    {
        std::string Path = LastCommand.SerializePath();
        Async Result = Data->Connector->ExecuteJavascript(std::string("if(!self)throw 'BAS_NOT_EXISTS'"),std::string(),Path,true);
        Data->Results->ProcessResult(Result);

        std::string CommandId = LastCommand.CommandId;
        std::string CommandName = LastCommand.CommandName;

        Result->Then([this, CommandId, CommandName, IsNoWait](AsyncResult* Result)
        {
            if(!IsNoWait && !Result->GetIsSuccess() && Result->GetErrorMessage() == "BAS_NOT_EXISTS")
            {
                RunElementCommandCallbackOnNextTimer = 100;
                return;
            }
            std::string Data;
            SendTextResponce(std::string("<Element ID=\"") + CommandId + std::string("\"><") + CommandName + std::string(">") + Data + std::string("</") + CommandName + ("></Element>"));
            IsLastCommandNull = true;
        });
    }

    if(LastCommand.CommandName == "type")
    {
        TypeText = ReplaceAll(LastCommand.CommandParam1,"\r\n","<RETURN>");
        TypeText = ReplaceAll(TypeText,"\n","<RETURN>");
        TypeText = ReplaceAll(TypeText,"<MOUSEDOUBLE>","<MOUSELEFT><MOUSEDOUBLE>");

        TypeTextDelay = std::stoi(LastCommand.CommandParam2);

        if(TypeText == "<CONTROL>a<DELETE>")
        {
            TypeText = "<CONTROL>a<BACK>";
            TypeTextDelay = 30;
        }

        if(TypeText == "<CONTROL><MOUSELEFT>")
        {
            TypeText = "<CONTROL><SHIFT><MOUSELEFT>";
        }



        TypeTextTaskIsActive = true;
        TypeTextIsFirstLetter = false;
        TypeTextLastTime = 0;
        TypeTextState.Clear();
    }

    if(LastCommand.CommandName == "random_point")
    {
        std::string Path = LastCommand.SerializePath();
        
        std::string get_point;
        if(Settings->EmulateMouse())
        {
            get_point = std::string("var x=0;for(var i=0;i<10;i++){x+=_BAS_SAFE(Math.random)()*((rect.right-2-rect.left+1)/10);};x=_BAS_SAFE(Math.floor)(x)+rect.left+1;if(x>rect.right-1)x=rect.right-1;if(x<rect.left+1)x=rect.left+1;"
                                    "var y=0;for(var i=0;i<10;i++){y+=_BAS_SAFE(Math.random)()*((rect.bottom-2-rect.top+1)/10);};y=_BAS_SAFE(Math.floor)(y)+rect.top+1;if(y>rect.bottom-1)y=rect.bottom-1;if(y<rect.top+1)y=rect.top+1;");
        }else
        {
            get_point = std::string("var x=_BAS_SAFE(Math.floor)((rect.right + rect.left)/2);"
                                    "var y=_BAS_SAFE(Math.floor)((rect.bottom + rect.top)/2);");
        }
        std::string script = std::string("{if(!self)throw 'BAS_NOT_EXISTS';"
                                "var items=_BAS_SAFE($Element.getClientRects)(self);"
                                "var len = _BAS_SAFE($DOMRectList.length)(items);"
                                "if(len == 0){throw 'BAS_NOT_EXISTS'};"
                                "var rect=_BAS_SAFE($DOMRectReadOnly.toJSON)(items[_BAS_SAFE(Math.floor)(_BAS_SAFE(Math.random)()*len)]);")
                                + get_point +
                                std::string("x += positionx + scrollx;"
                                "y += positiony + scrolly;"
                                "[[RESULT]]=x+','+y;"
                                "}");
        
        script = Javascript(script,"main");

        Async Result = Data->Connector->ExecuteJavascript(script,std::string(),Path,true);
        Data->Results->ProcessResult(Result);

        std::string CommandId = LastCommand.CommandId;
        std::string CommandName = LastCommand.CommandName;

        Result->Then([this, CommandId, CommandName, IsNoWait](AsyncResult* Result)
        {
            if(!IsNoWait && !Result->GetIsSuccess() && Result->GetErrorMessage() == "BAS_NOT_EXISTS")
            {
                //If selector has been changed after scrolling to element, return empty result
                SendTextResponce(std::string("<Element ID=\"") + CommandId + std::string("\"><") + CommandName + std::string(">") + std::string("</") + CommandName + ("></Element>"));
                IsLastCommandNull = true;
                return;
            }
            JsonParser Parser;
            std::string Data = Parser.GetStringFromJson(Result->GetString(),"RESULT");
            xml_encode(Data);
            SendTextResponce(std::string("<Element ID=\"") + CommandId + std::string("\"><") + CommandName + std::string(">") + Data + std::string("</") + CommandName + ("></Element>"));
            IsLastCommandNull = true;
        });
    }

    if(LastCommand.CommandName == "clarify")
    {
        std::string Path = LastCommand.SerializePath();
        std::string x = LastCommand.CommandParam1;
        std::string y = LastCommand.CommandParam2;
        
        std::string get_point;
        if(Settings->EmulateMouse())
        {
            get_point = std::string("var x=0;for(var i=0;i<10;i++){x+=_BAS_SAFE(Math.random)()*((rect.right-2-rect.left+1)/10);};x=_BAS_SAFE(Math.floor)(x)+rect.left+1;if(x>rect.right-1)x=rect.right-1;if(x<rect.left+1)x=rect.left+1;"
                                    "var y=0;for(var i=0;i<10;i++){y+=_BAS_SAFE(Math.random)()*((rect.bottom-2-rect.top+1)/10);};y=_BAS_SAFE(Math.floor)(y)+rect.top+1;if(y>rect.bottom-1)y=rect.bottom-1;if(y<rect.top+1)y=rect.top+1;");
        }else
        {
            get_point = std::string("var x=_BAS_SAFE(Math.floor)((rect.right + rect.left)/2);"
                                    "var y=_BAS_SAFE(Math.floor)((rect.bottom + rect.top)/2);");
        }
        std::string script = std::string("{if(!self)throw 'BAS_NOT_EXISTS';"
                                "var items=_BAS_SAFE($Element.getClientRects)(self);"
                                "var len = _BAS_SAFE($DOMRectList.length)(items);"
                                "if(len == 0){throw 'BAS_NOT_EXISTS'};"
                                "for(var i = 0;i<len;i++)"
                                "{"
                                    "var item = _BAS_SAFE($DOMRectReadOnly.toJSON)(items[i]);"
                                    "var x = ") + x + std::string(" - positionx - scrollx") + std::string(
                                    ";var y = ") + y + std::string(" - positiony - scrolly") + std::string(
                                    ";if(y >= item.top && y <= item.bottom && x >= item.left && x <= item.right)"
                                    "{"
                                        "[[RESULT]] = '';"
                                        "return;"
                                    "}"
                                "}"
                                "var rect=_BAS_SAFE($DOMRectReadOnly.toJSON)(items[_BAS_SAFE(Math.floor)(_BAS_SAFE(Math.random)()*len)]);")
                                + get_point +
                                std::string("x+=positionx + scrollx;"
                                "y+=positiony + scrolly;"
                                "[[RESULT]]=x+','+y;"
                                "}");
        
        script = Javascript(script,"main");

        Async Result = Data->Connector->ExecuteJavascript(script,std::string(),Path,true);
        Data->Results->ProcessResult(Result);

        std::string CommandId = LastCommand.CommandId;
        std::string CommandName = LastCommand.CommandName;

        Result->Then([this, CommandId, CommandName, IsNoWait](AsyncResult* Result)
        {
            if(!IsNoWait && !Result->GetIsSuccess() && Result->GetErrorMessage() == "BAS_NOT_EXISTS")
            {
                //If selector has been changed after moving mouse on element, we will click on prevoius mouse position.
                SendTextResponce(std::string("<Element ID=\"") + CommandId + std::string("\"><") + CommandName + std::string(">") + std::string("</") + CommandName + ("></Element>"));
                IsLastCommandNull = true;
                return;
            }
            JsonParser Parser;
            std::string Data = Parser.GetStringFromJson(Result->GetString(),"RESULT");
            xml_encode(Data);
            SendTextResponce(std::string("<Element ID=\"") + CommandId + std::string("\"><") + CommandName + std::string(">") + Data + std::string("</") + CommandName + ("></Element>"));
            IsLastCommandNull = true;
        });
    }

    if(LastCommand.CommandName == "render_base64")
    {
        std::string Path = LastCommand.SerializePath();
        std::string Script = Javascript("if(!self)throw 'BAS_NOT_EXISTS';[[RESULT]]= _BAS_HIDE(BrowserAutomationStudio_GetElementCoordinates)(self);[[POSITIONY]] = positiony;[[POSITIONX]] = positionx;", "main");
        Async Result = Data->Connector->ExecuteJavascript(Script,std::string(),Path,true);
        Data->Results->ProcessResult(Result);

        std::string CommandId = LastCommand.CommandId;
        std::string CommandName = LastCommand.CommandName;

        Result->Then([this, CommandId, CommandName, IsNoWait](AsyncResult* Result)
        {
            if(!IsNoWait && !Result->GetIsSuccess() && Result->GetErrorMessage() == "BAS_NOT_EXISTS")
            {
                RunElementCommandCallbackOnNextTimer = 100;
                return;
            }

            JsonParser Parser;

            std::string str = Parser.GetStringFromJson(Result->GetString(),"RESULT");
            int positionx = Parser.GetFloatFromJson(Result->GetString(),"POSITIONX");
            int positiony = Parser.GetFloatFromJson(Result->GetString(),"POSITIONY");


            int left = -1, top = -1, right = -1, bottom = -1, centerx = -1, centery = -1;

            std::size_t pos = str.find(",");
            if(pos != std::string::npos)
            {
                std::string part = str.substr(0,pos);
                str = str.substr(pos + 1,str.length() - pos - 1);
                centerx = std::stoi(part);
            }

            pos = str.find(",");
            if(pos != std::string::npos)
            {
                std::string part = str.substr(0,pos);
                str = str.substr(pos + 1,str.length() - pos - 1);
                centery = std::stoi(part);
            }

            pos = str.find(",");
            if(pos != std::string::npos)
            {
                std::string part = str.substr(0,pos);
                str = str.substr(pos + 1,str.length() - pos - 1);
                left = std::stoi(part);
            }

            pos = str.find(",");
            if(pos != std::string::npos)
            {
                std::string part = str.substr(0,pos);
                str = str.substr(pos + 1,str.length() - pos - 1);
                top = std::stoi(part);
            }

            pos = str.find(",");
            if(pos != std::string::npos)
            {
                std::string part = str.substr(0,pos);
                str = str.substr(pos + 1,str.length() - pos - 1);
                right = std::stoi(part);
            }

            bottom = std::stoi(str);

            left += positionx;
            top += positiony;
            right += positionx;
            bottom += positiony;
            
            RenderX = left;
            RenderY = top;
            RenderWidth = right - left;
            RenderHeight = bottom - top;
            IsElementRender = true;
            NeedRenderNextFrame = true;
            RenderNextFrameTime = clock() + CLOCKS_PER_SEC * 2;
        });
    }

    
    if(LastCommand.CommandName == "attr")
    {
        std::string attr_escaped = picojson::value(LastCommand.CommandParam1).serialize();
        std::string Script = Javascript(std::string("if(!self)throw 'BAS_NOT_EXISTS';[[RESULT]] = '';var attr=") + attr_escaped + std::string(";if(_BAS_SAFE($Node.hasAttribute)(self, attr))[[RESULT]]=_BAS_SAFE($Node.getAttribute)(self, attr);"),"main");

        std::string Path = LastCommand.SerializePath();
        Async Result = Data->Connector->ExecuteJavascript(Script,std::string(),Path);
        Data->Results->ProcessResult(Result);

        std::string CommandId = LastCommand.CommandId;
        std::string CommandName = LastCommand.CommandName;

        Result->Then([this, CommandId, CommandName, IsNoWait](AsyncResult* Result)
        {
            std::string Data;
            if(Result->GetIsSuccess())
            {
                picojson::value v;
                picojson::parse(v, Result->GetString());
                picojson::value Result = v.get<picojson::value::object>()["RESULT"];
                if(Result.is<std::string>())
                {
                    Data = Result.get<std::string>();
                }
            }else if(!IsNoWait && Result->GetErrorMessage() == "BAS_NOT_EXISTS")
            {
                RunElementCommandCallbackOnNextTimer = 100;
                return;
            }
            xml_encode(Data);
            SendTextResponce(std::string("<Element ID=\"") + CommandId + std::string("\"><") + CommandName + std::string(">") + Data + std::string("</") + CommandName + ("></Element>"));
            IsLastCommandNull = true;
        });
    }


    if(LastCommand.CommandName == "set_attr")
    {

        std::string attr_escaped = picojson::value(LastCommand.CommandParam1).serialize();
        std::string val_escaped = picojson::value(LastCommand.CommandParam2).serialize();

        std::string Script = Javascript(std::string("if(!self)throw 'BAS_NOT_EXISTS';var attr=") + attr_escaped + std::string(";var val=") + val_escaped + std::string(";if(val.length === 0)_BAS_SAFE($Node.removeAttribute)(self, attr);else _BAS_SAFE($Node.setAttribute)(self, attr, val);"),"main");

        std::string Path = LastCommand.SerializePath();
        Async Result = Data->Connector->ExecuteJavascript(Script,std::string(),Path);
        Data->Results->ProcessResult(Result);

        std::string CommandId = LastCommand.CommandId;
        std::string CommandName = LastCommand.CommandName;

        Result->Then([this, CommandId, CommandName, IsNoWait](AsyncResult* Result)
        {
            std::string Data;
            if(!Result->GetIsSuccess() && !IsNoWait && Result->GetErrorMessage() == "BAS_NOT_EXISTS")
            {
                RunElementCommandCallbackOnNextTimer = 100;
                return;
            }
            xml_encode(Data);
            SendTextResponce(std::string("<Element ID=\"") + CommandId + std::string("\"><") + CommandName + std::string(">") + Data + std::string("</") + CommandName + ("></Element>"));
            IsLastCommandNull = true;
        });
    }

}

void MainApp::HandleScreenshotCapture()
{
    if(NeedRenderNextFrame && clock() > RenderNextFrameTime)
    {
        char *ImageData = Data->Connector->GetPaintData();
        int width = Data->Connector->GetWidth();
        int height = Data->Connector->GetHeight();
        std::string base64;

        if(RenderWidth > 0 && RenderHeight > 0)
        {
            NeedRenderNextFrame = false;
            RenderNextFrameTime = 0;
            std::vector<unsigned char> out;
            std::vector<unsigned char> in;
            int w = 0;
            int h = 0;
            for(int j = 0;j<height;j++)
            {
                if(j>RenderY && j<RenderY + RenderHeight)
                {
                    h++;
                }
                for(int i = 0;i<width;i++)
                {

                    if(i>RenderX && i<RenderX + RenderWidth && j>RenderY && j<RenderY + RenderHeight)
                    {
                        if(h==1)
                            w++;
                        in.push_back((unsigned char)ImageData[i*4+j*width*4 + 2]);
                        in.push_back((unsigned char)ImageData[i*4+j*width*4 + 1]);
                        in.push_back((unsigned char)ImageData[i*4+j*width*4 + 0]);
                        in.push_back((unsigned char)ImageData[i*4+j*width*4 + 3]);
                    }
                }

            }


            lodepng::encode(out,(unsigned const char *)(in.data()),w,h);
            base64 = base64_encode(out.data(),out.size());
        }

        if(IsElementRender)
        {
            WORKER_LOG(std::string("Render result element <<") + base64);
            FinishedLastCommand(base64);
        }
        else
        {
            WORKER_LOG(std::string("Render result screen <<") + base64);

            xml_encode(base64);
            SendTextResponce(std::string("<Render>") + base64 + std::string("</Render>"));
        }
    }
}

void MainApp::Timer()
{

    //Tracking page scrolling
    if(ScrollStopTracking > 0)
    {
        clock_t CurrentTime = clock();

        if(Data->ScrollX == ScrollTrackingX && Data->ScrollY == ScrollTrackingY)
        {
            if(float( CurrentTime - ScrollStopTracking ) / CLOCKS_PER_SEC > 0.1)
            {
                ScrollStopTracking = 0;
                ScrollTrackingX = 0;
                ScrollTrackingY = 0;
                ScrollStopTrackingStart = 0;
                DoTrackScroll = false;
                SendTextResponce("<MouseMove></MouseMove>");
            }
        }else
        {
            ScrollStopTracking = CurrentTime;
            ScrollTrackingX = Data->ScrollX;
            ScrollTrackingY = Data->ScrollY;
        }

        if(ScrollStopTracking > 0 && float( CurrentTime - ScrollStopTrackingStart ) / CLOCKS_PER_SEC > 5.0)
        {
            ScrollStopTracking = 0;
            ScrollTrackingX = 0;
            ScrollTrackingY = 0;
            ScrollStopTrackingStart = 0;
            DoTrackScroll = false;
            SendTextResponce("<MouseMove></MouseMove>");
        }
    }


    if(DelayClickType == 1 || DelayClickType == 2)
    {
        clock_t CurrentTime = clock();
        //If DelayNextClick equals -1, we need to wait until TouchStartTask will finish and then make decision about delay
        if(DelayNextClick == -1 && TouchStartTask && TouchStartTask->GetIsFinished())
        {
            //Need to understand, if touch was inside frame

            std::string ResultDataRaw = TouchStartTask->GetString();
            JsonParser Parser;
            bool IsInIsolatedFrame = Parser.GetBooleanFromJson(ResultDataRaw, "is_in_isolated_frame", false);

            if(IsInIsolatedFrame)
            {
                DelayNextClick = clock() + 580 + (rand()) % 40;
            }else
            {
                DelayNextClick = clock() + 80 + (rand()) % 40;
            }

            TouchStartTask.reset();

        }else if(CurrentTime >= DelayNextClick && DelayNextClick != -1)
        {
            BrowserEventsEmulator::MouseClick(Data->Connector,DelayClickX,DelayClickY,GetScrollPosition(),1,Data->IsMousePress,Data->IsDrag,Data->IsTouchScreen,Data->TouchEventId,Data->IsTouchPressedAutomation,TypeTextState);
            if(DelayClickType == 1)
            {
                SendTextResponce("<MouseClick></MouseClick>");
            }else
            {
                FinishedLastCommand("");
            }
            DelayClickType = 0;
        }
    }

    if(Data->IsRecord)
    {
        if(Settings->Detector())
        {
            int NewFingerprintDataNumber = Detector.Timer(BrowserDetector,detector8handler && detector8handler->GetIsInitialized());
            if(NewFingerprintDataNumber > 0)
                Layout->UpdateFingerprintDetectorNumber(Layout->GetFingerprintDetectorNumber() + NewFingerprintDataNumber);
            {
                LOCK_BROWSER_DATA
                if(Detector.GetLastGroupCount() > 1000)
                {
                    Data->Saver.TemporaryDisableDetector = true;
                    Data->Saver.Save();
                }
            }
        }else
        {
            if(Layout->GetFingerprintDetectorNumber() != -1)
                Layout->UpdateFingerprintDetectorNumber(-1);
        }
    }

    if(Data->IsRecordHttp && _DevToolsReqest2Action)
    {
        _DevToolsReqest2Action->Timer();
    }

    if(Data->IsRecord && BrowserToolbox)
    {
        Notifications.Timer(BrowserToolbox);
    }

    if(RunElementCommandCallbackOnNextTimer >= 0)
    {
        if(RunElementCommandCallbackOnNextTimer == 0)
        {
            WORKER_LOG("Execute last command");
            if(!IsLastCommandNull)
                ElementCommandCallback(LastCommandCopy);
        }else
        {
            RunElementCommandCallbackOnNextTimer --;
        }
    }

    ExecuteTypeText();

    ExecuteMouseMove();

    HandleMainBrowserEvents();

    if(toolboxv8handler)
        HandleToolboxBrowserEvents();

    if(scenariov8handler)
        HandleScenarioBrowserEvents();

    if(central8handler)
        HandleCentralBrowserEvents();

    if(detector8handler)
        HandleDetectorBrowserEvents();

    /*if(dhandler)
        dhandler->Timer();

    _HandlersManager->Timer();

    if(_HandlersManager->CheckIsClosed())
    {
        SendTextResponce("<PopupClose></PopupClose>");
    }*/

    UpdateWindowPositionWithParent();

    if(Data->IsRecord)
    {
        UpdateHighlight();
        UpdateHighlightMultiselect();
    }

    UpdateRecaptchaV3Check();


    auto now = duration_cast< milliseconds >( system_clock::now().time_since_epoch() ).count();
    if(now > Data->LastClearRequest + 5000 || Data->LastClearRequest == 0)
    {
        Data->LastClearRequest = now;
        Data->_RequestList.RemoveOld();
    }

    if(Data->ManualControl != BrowserData::Indirect && _HandlersManager->GetBrowser())
    {
        Layout->SetCanGoBack(_HandlersManager->GetBrowser()->CanGoBack());
    }

    DirectControlInspectMouse();

    if(!ProxyLibraryLoaded)
        CheckNetworkProcessIPC();

    HandleScreenshotCapture();

    if(BrowserCloseTime > 0 && now >= BrowserCloseTime)
    {
        BrowserCloseTime = 0;
        Data->Connector->CloseBrowser();
    }
}

void MainApp::InitNetworkProcessIPC()
{
    NetworkProcessIPC.Init(std::string("in") + Data->_UniqueProcessId);
}

void MainApp::CheckNetworkProcessIPC()
{
    if(NetworkProcessIPC.Peek())
    {
        std::vector<std::string> DataAll = NetworkProcessIPC.Read();
        if(!DataAll.empty())
        {
            WORKER_LOG("CheckNetworkProcessIPC");
            SendTextResponce("<SendWorkerSettings></SendWorkerSettings>");
            ProxyLibraryLoaded = true;
        }
    }
}

void MainApp::DirectControlInspectMouse()
{
    if(Data->ManualControl != BrowserData::DirectRecord)
        return;

    clock_t CurrentTime = 0;

    if(InspectTask)
    {
        CurrentTime = clock();
        float time_spent = float( CurrentTime - LastMouseTrack ) /  CLOCKS_PER_SEC;

        if(time_spent >= 3)
        {
            Data->Connector->InterruptAction(InspectTask->GetActionUniqueId());
            InspectTask.reset();
            DirectControl()->TimeoutLastInspect();
        }
    }

    if(InspectTask)
       return;

    BrowserDirectControl::InspectTask Task = DirectControl()->GetInspectTask();

    if(Task.IsNull)
        return;

    if(CurrentTime == 0)
        CurrentTime = clock();

    InspectMouseAt(Task.X, Task.Y, CurrentTime);
}

void MainApp::OnPaint()
{
    Paint(Data->Connector->GetWidth(),Data->Connector->GetHeight());
}

void MainApp::OnResize()
{
    Data->WidthBrowser = Data->Connector->GetWidth();
    Data->HeightBrowser = Data->Connector->GetHeight();
    Layout->Update(Data->WidthBrowser,Data->HeightBrowser,Data->WidthAll,Data->HeightAll);
}

void MainApp::OnScroll()
{
    Data->ScrollX = Data->Connector->GetScrollX();
    Data->ScrollY = Data->Connector->GetScrollY();
}

void MainApp::OnRequestStart(std::string RequestId)
{
    DirectControl()->PageLoaded();
    Data->_RequestList.Add(RequestId);
}

void MainApp::OnRecordHttpData(std::string Script)
{
    if(Data->IsRecordHttp && _DevToolsReqest2Action && BrowserScenario && !Script.empty())
    {
        BrowserScenario->GetMainFrame()->ExecuteJavaScript(Script,BrowserScenario->GetMainFrame()->GetURL(), 0);
    }
}

void MainApp::OnRequestStop(std::string RequestId)
{
    Data->_RequestList.Remove(RequestId);
    if(Data->IsRecordHttp && _DevToolsReqest2Action)
    {
        _DevToolsReqest2Action->ConvertStop(RequestId);
    }
}

void MainApp::OnLoadStart()
{
    Data->_RequestList.RemoveAll();
}

void MainApp::OnLoadStop()
{
}

void MainApp::OnAddressChanged(std::string Url)
{
    SetWindowText(Data->UrlHandler, s2ws(Url).c_str());
}

void MainApp::OnNativeDialog(std::string DialogType)
{
    if(Data->IsRecord && BrowserToolbox)
    {
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_Notify('") + DialogType + std::string("')"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
    }
}

void MainApp::OnDownloadStarted(std::wstring FileName)
{
    if(!Data->IsRecord && Data->ManualControl != BrowserData::Indirect)
    {
        std::string data = ws2s(FileName);
        if (OpenClipboard(0))
        {
            HGLOBAL clipbuffer;
            char * buffer;
            EmptyClipboard();
            clipbuffer = GlobalAlloc(GMEM_DDESHARE, data.length() + 1);
            buffer = (char*)GlobalLock(clipbuffer);
            memcpy(buffer, data.data(),data.size());
            buffer[data.length()] = 0;
            GlobalUnlock(clipbuffer);
            SetClipboardData(CF_TEXT,clipbuffer);
            CloseClipboard();
        }

        std::wstring Message = Translate::Tr(std::wstring(L"File downloading has been started to ")) + FileName + Translate::Tr(std::wstring(L". Path has been saved to clipboard."));
        MessageBox(
                NULL,
                Message.data(),
                Translate::Tr(L"File download").data(),
                MB_ICONWARNING | MB_OK
            );


    }
}

void MainApp::ClearHighlight()
{
    if(v8handler.get())
        v8handler->ClearHighlightLast();
    {
        LOCK_BROWSER_DATA
        Data->_Highlight.highlights.clear();
    }

    RECT r = Layout->GetBrowserOuterRectangle(GetData()->WidthBrowser,GetData()->HeightBrowser,GetData()->WidthAll,GetData()->HeightAll);
    InvalidateRect(Data->_MainWindowHandle,&r,false);
}

void MainApp::UpdateMultiSelect()
{
    if(Data->MultiselectMode && Data->_MultiSelectData.IsDirty && !HighlightSelector.empty())
    {
        Data->_MultiSelectData.IsDirty = false;

        std::string MultiselectScript = Javascript(std::string("_BAS_HIDE(BrowserAutomationStudio_SetMultiSelectData)(") + Data->_MultiSelectData.Serialize() + std::string(");"), "main");

        Data->Connector->ExecuteJavascript(MultiselectScript,std::string(),HighlightSelector);
    }
}

void MainApp::UpdateHighlightMultiselect()
{
    clock_t CurrentTime = clock();
    float time_spent = float( CurrentTime - LastHighlightMultiselect ) /  CLOCKS_PER_SEC;

    if(HighlightMultiselectTask && time_spent < 3)
        return;

    LastHighlightMultiselect = CurrentTime;

    if(HighlightMultiselectTask)
    {
        Data->Connector->InterruptAction(HighlightMultiselectTask->GetActionUniqueId());
        HighlightMultiselectTask.reset();
    }

    if(!HighlightSelector.empty())
    {
        std::string Script = Javascript(std::string(";[[POSITIONY]] = positiony;[[POSITIONX]] = positionx;[[RESULT]] = _BAS_HIDE(BrowserAutomationStudio_HighlightMultiselect)(false); [[RESULT]]['elements'] = null;"),"main");

        HighlightMultiselectTask = Data->Connector->ExecuteJavascript(Script,std::string(),HighlightSelector);
    }
}

void MainApp::UpdateRecaptchaV3Check()
{
    if(!Data->HasRecaptchaModule)
        return;

    clock_t CurrentTime = clock();
    float time_spent = float( CurrentTime - LastRecaptchaV3Check ) /  CLOCKS_PER_SEC;

    if(time_spent < 12)
        return;

    LastRecaptchaV3Check = CurrentTime;

    if(RecaptchaV3Task)
    {
        Data->Connector->InterruptAction(RecaptchaV3Task->GetActionUniqueId());
        RecaptchaV3Task.reset();
    }

    std::string Script = Javascript(std::string(";[[RESULT]] = (typeof(_BAS_HIDE(BrowserAutomationStudio_RecaptchaV3Call)) == 'string') ? _BAS_HIDE(BrowserAutomationStudio_RecaptchaV3Call) : '';_BAS_HIDE(BrowserAutomationStudio_RecaptchaV3Call) = '';"),"main");

    RecaptchaV3Task = Data->Connector->ExecuteJavascript(Script,std::string(),std::string("[]"));
}

void MainApp::UpdateHighlight()
{
    clock_t CurrentTime = clock();
    float time_spent = float( CurrentTime - LastHighlight ) /  CLOCKS_PER_SEC;

    if(HighlightTask && time_spent < 3)
        return;

    LastHighlight = CurrentTime;

    if(HighlightTask)
    {
        Data->Connector->InterruptAction(HighlightTask->GetActionUniqueId());
        HighlightTask.reset();
    }

    if(LastHighlightIndexChanged > 0)
    {
        float time_spent_index = float( CurrentTime - LastHighlightIndexChanged ) /  CLOCKS_PER_SEC;
        if(time_spent_index > 3)
        {
            IsHighlightIndexActive = false;
            LastHighlightIndexChanged = 0;
        }
    }

    if(!HighlightSelector.empty())
    {

        std::string MultiselectScript;
        if(Data->_MultiSelectData.IsDirty)
        {
            MultiselectScript = std::string("_BAS_HIDE(BrowserAutomationStudio_SetMultiSelectData)(") + Data->_MultiSelectData.Serialize() + std::string("); ");
            Data->_MultiSelectData.IsDirty = false;
        }

        int CurrentHighlightIndex = -1;

        if(IsHighlightIndexActive)
            CurrentHighlightIndex = HighlightIndex;

        std::string Script = Javascript(MultiselectScript + std::string(";[[POSITIONY]] = positiony;[[POSITIONX]] = positionx;[[RESULT]] = _BAS_HIDE(BrowserAutomationStudio_Highlight)(self, ") + std::to_string(CurrentHighlightIndex) + std::string(", ") + std::string(HighlightDoScrolling ? "true" : "false") + std::string(")"),"main");

        HighlightDoScrolling = false;

        HighlightTask = Data->Connector->ExecuteJavascript(Script,std::string(),HighlightSelector);
    }
}

std::pair<std::string, bool> MainApp::GetMenuSelected()
{

    if(central8handler)
        return central8handler->GetMenuSelected();
    else
    {
        std::pair<std::string, bool> res;
        res.second = false;
        return res;
    }
}



void MainApp::HandleCentralBrowserEvents()
{
    std::pair<std::string, bool> res = central8handler->GetLoadUrl();
    if(res.second)
    {
        std::string url = res.first;
        if(url.length() >= 7 && url[0] == 'f'&& url[1] == 'i'&& url[2] == 'l'&& url[3] == 'e'&& url[4] == ':'&& url[5] == '/'&& url[6] == '/')
        {
            url = url.substr(7,url.length() - 7);
            WORKER_LOG(std::string("OpenScriptExample<<") + url);
            xml_encode(url);
            SendTextResponce(std::string("<LoadScript>") + url + std::string("</LoadScript>"));

        }else if(starts_with(url,"open://browser"))
        {
            Layout->ShowManualSelect();
            LoadManualSelect();
        }else if(starts_with(url,"open://help"))
        {
            if(BrowserScenario)
                BrowserScenario->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_HighlightHelp()"),"scenario"),BrowserScenario->GetMainFrame()->GetURL(), 0);
        }else
        {
            ReplaceAllInPlace(res.first, "%BAS_DEBUG_PORT%", std::to_string(Data->RemoteDebuggingPort));
            WORKER_LOG(std::string("LoadUrlFromCentralBrowser<<") + res.first);
            ShellExecute(0, 0, s2ws(res.first).c_str(), 0, 0 , SW_SHOW );
        }
    }

    if(central8handler->GetClose())
    {
        BrowserCentral->GetMainFrame()->ExecuteJavaScript("document.body.style.display='none'","file:///html/central/empty.html",0);
        Layout->HideCentralBrowser();
    }

    res = central8handler->GetSettings();
    if(res.second)
    {
        std::string settings = res.first;
        WORKER_LOG(std::string("Settings updated") + settings);
        Settings->Deserialize(settings);
        Restart();
    }


    std::pair<int, bool> res2 = central8handler->GetManualControl();
    if(res2.second)
    {
        if(res2.first == 0)
        {
            Data->ManualControl = BrowserData::Indirect;
        }
        if(res2.first == 1)
        {
            Data->ManualControl = BrowserData::DirectNoRecord;
        }
        if(res2.first == 2)
        {
            Data->ManualControl = BrowserData::DirectRecord;
        }
        UpdateManualControl();

    }
}


void MainApp::HandleDetectorBrowserEvents()
{
    std::pair<std::string, bool> res = detector8handler->GetLoadUrl();
    if(res.second)
    {
        if(res.first == std::string("settings://settings"))
        {
            LoadSettingsPage();
        }else
            ShellExecute(0, 0, s2ws(res.first).c_str(), 0, 0 , SW_SHOW );
        }

    std::pair<std::string, bool> res2 = detector8handler->GetFingerprintKey();
    if(res2.second)
    {
        WriteStringToFile("fingerprint-detector.txt", res2.first);
    }

    std::pair<std::string, bool> res4 = detector8handler->GetEditSource();
    if(res4.second)
    {
        WriteStringToFile("detector-source.js", res4.first);
        ShellExecute(0, 0, L"detector-source.js", 0, 0 , SW_SHOW );

    }

    if(detector8handler->GetLogout())
    {
        WriteStringToFile("fingerprint-detector.txt", "");
    }

    if(detector8handler->GetClearAll())
    {
        Layout->UpdateFingerprintDetectorNumber(0);
    }

}

void MainApp::HandleScenarioBrowserEvents()
{
    if(Data->IsRecord && BrowserScenario && scenariov8handler->GetIsInitialized() && !BrowserScenarioDelayScript.empty())
    {
        BrowserScenario->GetMainFrame()->ExecuteJavaScript(BrowserScenarioDelayScript,BrowserScenario->GetMainFrame()->GetURL(), 0);
        BrowserScenarioDelayScript.clear();

    }
    if(scenariov8handler->GetIsInitialized() && !Code.empty())
    {
        std::string script = Javascript(std::string("BrowserAutomationStudio_Parse(") + picojson::value(Code.data()).serialize() + std::string(")"),"scenario");
        BrowserScenario->GetMainFrame()->ExecuteJavaScript(script,BrowserScenario->GetMainFrame()->GetURL(), 0);
        Code.clear();
        if(Data->IsTesing)
        {
            Data->IsTesing = false;
            BrowserScenario->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_Play()"),"scenario"),BrowserScenario->GetMainFrame()->GetURL(), 0);
        }
    }

    if(scenariov8handler->GetIsInitialized() && !SetNextActionId.empty())
    {
        if(BrowserScenario)
            BrowserScenario->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_NotRunningTask(") + SetNextActionId + std::string(")"),"scenario"),BrowserScenario->GetMainFrame()->GetURL(), 0);
        SetNextActionId.clear();
    }

    if(scenariov8handler->GetIsInitialized() && StartPlayScriptOnStart)
    {
        if(BrowserScenario)
            BrowserScenario->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_Play()"),"scenario"),BrowserScenario->GetMainFrame()->GetURL(), 0);
        StartPlayScriptOnStart = false;
    }

    {
        std::pair<bool, bool> res = scenariov8handler->GetIsInsideElementLoop();
        if(res.second)
        {
            Data->MultiselectIsInsideElementLoop = res.first;
        }
    }

    std::pair<std::vector<ScenarioV8Handler::LastResultStruct>, bool> res = scenariov8handler->GetResult();
    if(res.second)
    {
        for(ScenarioV8Handler::LastResultStruct & Result:res.first)
        {
            std::string new_code = Result.LastResultCodeDiff;
            WORKER_LOG(std::string("HandleScenarioBrowserEvents<<") + new_code);
            Variables = Result.LastResultVariables;
            GlobalVariables = Result.LastResultGlobalVariables;
            Labels = Result.LastResultLabels;
            Functions = Result.LastResultFunctions;
            std::string AdditionalResourcesPrev = AdditionalResources;
            AdditionalResources = Result.LastResultResources;
            if(AdditionalResourcesPrev != AdditionalResources)
                ResourcesChanged = true;
            xml_encode(new_code);
            SendTextResponce(std::string("<ReceivedCode execution_point=\"") + std::to_string(Result.ExecuteNextId) + std::string("\" >") + new_code + std::string("</ReceivedCode>"));
        }
        if(!DelayedSend.empty())
        {
            SendTextResponce(DelayedSend);
            DelayedSend.clear();
        }
    }

    if(Data->IsRecord)
    {
        std::pair<ScenarioV8Handler::WebInterfaceTaskResultStruct, bool> res = scenariov8handler->GetWebInterfaceTaskResult();
        if(res.second)
        {
            std::string Result = res.first.LastWebInterfaceResultData;
            xml_encode(Result);
            SendTextResponce(
                        std::string("<RunTaskResult ResultId=\"") +
                        std::to_string(res.first.LastWebInterfaceResultId) +
                        std::string("\">") + Result + std::string("</RunTaskResult>")
                     );
        }
    }

    if(Data->IsRecord)
    {
        std::pair<ScenarioV8Handler::PrepareFunctionResultStruct, bool> res = scenariov8handler->GetPrepareFunctionResult();
        if(res.second)
        {
            std::string Data = res.first.FunctionData;
            std::string Name = res.first.FunctionName;
            xml_encode(Data);
            xml_encode(Name);
            SendTextResponce(
                        std::string("<PrepareFunction FunctionName=\"") +
                        Name +
                        std::string("\">") + Data + std::string("</PrepareFunction>")
                     );
        }
    }


    std::pair< std::pair<std::string,bool>, std::pair<bool,bool> > res2 = scenariov8handler->GetExecuteCode();
    if(res2.second.first)
    {
        if(!res2.second.second)
            Layout->UpdateState(MainLayout::Hold);
        if(BrowserToolbox)
            BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_ShowWaiting(") + picojson::value(res2.first.first).serialize() + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
        std::string CodeSend = res2.first.first;
        WORKER_LOG(std::string("GetExecuteCode<<") + CodeSend);
        xml_encode(CodeSend);
        SendTextResponce(std::string("<WaitCode is_play='") + std::to_string(res2.first.second) + std::string("' >") + CodeSend + std::string("</WaitCode>"));
    }

    if(Data->IsRecord)
    {
        std::pair<std::string, bool> res = scenariov8handler->GetCurrentFunction();
        if(res.second)
        {
            if(BrowserToolbox)
                BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_SetCurrentFunction(") + picojson::value(res.first).serialize() + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
        }
    }

    std::pair<std::pair<std::string, std::pair<bool, bool>>, bool> res6 = scenariov8handler->GetClipboardSetRequest();
    if(res6.second)
    {
        write_clipboard(res6.first.first, res6.first.second.first, res6.first.second.second);
    }

    if(Data->IsRecord)
    {
        std::pair<std::string, bool> res = scenariov8handler->GetUpdateEmbeddedData();
        if(res.second)
        {
            if(BrowserToolbox)
                BrowserToolbox->GetMainFrame()->ExecuteJavaScript(std::string("_EmbeddedModel.Update(") + picojson::value(res.first).serialize() + std::string(")"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
        }
    }

    if(scenariov8handler->GetClipboardGetRequest())
    {
        std::string res = read_clipboard();
        std::string script = Javascript(std::string("BrowserAutomationStudio_GetClipboardResult(") + picojson::value(res).serialize() + std::string(")"),"scenario");
        if(BrowserScenario)
            BrowserScenario->GetMainFrame()->ExecuteJavaScript(script,BrowserScenario->GetMainFrame()->GetURL(), 0);
    }

    if(scenariov8handler->GetStartBackup())
    {
        SendTextResponce("<StartBackup></StartBackup>");
    }

    std::pair<std::string, bool> res5 = scenariov8handler->GetIsEditStart();
    if(res5.second)
    {
        std::string data = res5.first;
        WORKER_LOG(std::string("EditStart<<") + data);
        std::string script = Javascript(std::string("BrowserAutomationStudio_EditStart(") + picojson::value(data).serialize() + std::string(")"),"toolbox");
        if(BrowserToolbox)
            BrowserToolbox->GetMainFrame()->ExecuteJavaScript(script,BrowserToolbox->GetMainFrame()->GetURL(), 0);
    }


    if(scenariov8handler->GetIsEditSaveStart())
    {
        std::string script = Javascript(std::string("BrowserAutomationStudio_EditSaveStart()"),"toolbox");
        if(BrowserToolbox)
            BrowserToolbox->GetMainFrame()->ExecuteJavaScript(script,BrowserToolbox->GetMainFrame()->GetURL(), 0);
    }


    bool res4 = scenariov8handler->GetIsEditEnd();
    if(res4)
    {
        WORKER_LOG(std::string("EditEnd<<"));
        std::string script = Javascript("BrowserAutomationStudio_EditEnd()","toolbox");
        if(BrowserToolbox)
            BrowserToolbox->GetMainFrame()->ExecuteJavaScript(script,BrowserToolbox->GetMainFrame()->GetURL(), 0);
    }

    std::pair<std::string, bool> res7 = scenariov8handler->GetDetectorDataCode();
    if(res7.second)
    {
        Detector.BrowserData(BrowserDetector,res7.first,detector8handler && detector8handler->GetIsInitialized());
        {
            LOCK_BROWSER_DATA
            Data->Saver.TemporaryDisableDetector = false;
            Data->Saver.Save();
        }
    }

    if(!IsScenarioInterfaceInitialSent && scenariov8handler->GetIsInitialized())
    {
        std::string InterfaceInitial = ReadAllString("scenario.json");
        std::string Script = Javascript(std::string("BrowserAutomationStudio_LoadInterfaceState(") + picojson::value(InterfaceInitial).serialize() + std::string(")"),"scenario");
        BrowserScenario->GetMainFrame()->ExecuteJavaScript(Script,BrowserScenario->GetMainFrame()->GetURL(), 0);
        IsScenarioInterfaceInitialSent = true;
    }

    if(scenariov8handler->GetIsThreadNumberEditStart() && BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript("BrowserAutomationStudio_ThreadNumberEdit()","toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);

    if(scenariov8handler->GetIsSuccessNumberEditStart() && BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript("BrowserAutomationStudio_SuccessNumberEdit()","toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);

    if(scenariov8handler->GetIsFailNumberEditStart() && BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript("BrowserAutomationStudio_FailNumberEdit()","toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);

    if(scenariov8handler->GetIsEventTrigger() && BrowserToolbox)
    {
        std::string Name = picojson::value(scenariov8handler->GetEventTriggerName()).serialize();
        std::string Data = picojson::value(scenariov8handler->GetEventTriggerData()).serialize();
        std::string Script = Javascript(std::string("BrowserAutomationStudio_HandleEvent(") + Name + "," + Data + std::string(")"), "toolbox");
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Script, BrowserToolbox->GetMainFrame()->GetURL(), 0);
    }

    std::pair<std::string, bool> res8 = scenariov8handler->GetIsRunFunctionStart();

    if(res8.second && BrowserToolbox)
    {
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_RunFunction(") + picojson::value(res8.first).serialize() + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
    }

    {
        std::pair<std::string, bool> res = scenariov8handler->GetLoadUrl();
        if(res.second)
        {
            WORKER_LOG(std::string("LoadUrlFromUrlBrowser<<") + res.first);
            if(res.first == std::string("settings://settings"))
            {
                LoadSettingsPage();
            }else
            {
                ShellExecute(0, 0, s2ws(res.first).c_str(), 0, 0 , SW_SHOW );
            }
        }
    }

    if(BrowserToolbox)
    {
        std::pair<std::string, bool> res = scenariov8handler->GetIsOpenAction();
        if(res.second)
        {
            BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_OpenAction(") + picojson::value(res.first).serialize() + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
        }
    }

    std::pair<std::string, bool> res9 = scenariov8handler->GetIsRunFunctionSeveralThreadsStart();

    if(res9.second && BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_RunFunctionSeveralThreads(") + picojson::value(res9.first).serialize() + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);

    std::pair<std::string, bool> res10 = scenariov8handler->GetIsRunFunctionAsync();

    if(res10.second && BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_RunFunctionAsync(") + picojson::value(res10.first).serialize() + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);

    std::pair<std::string,bool> res12 = scenariov8handler->GetIsInterfaceState();
    if(res12.second) WriteStringToFile("scenario.json", res12.first);

    std::pair<std::string, bool> res11 = scenariov8handler->GetIsHighlightMenuItem();
    if(res11.second) for (auto f : EventHighlightMenu) f(res11.first);

    std::pair<std::string,bool> res13 = scenariov8handler->GetIsInterfaceJson();
    if(res13.second)
    {
        SendTextResponce(std::string("<SaveInterface>") + res13.first + std::string("</SaveInterface>"));
    }

    ScenarioV8Handler::RestartType res3 = scenariov8handler->GetNeedRestart();

    if(res3 == ScenarioV8Handler::Restart)
        Restart();
    else if(res3 == ScenarioV8Handler::Stop)
        Terminate();

}

void MainApp::HandleToolboxBrowserEvents()
{
    if(toolboxv8handler->GetClearHighlight())
    {
        HighlightSelector.clear();
        Data->_Highlight.highlights.clear();
        HighlightIndex = -1;
        HighlightDoScrolling = false;
        LastHighlightIndexChanged = 0;
        IsHighlightIndexActive = false;

        if(HighlightTask)
        {
            Data->Connector->InterruptAction(HighlightTask->GetActionUniqueId());
            HighlightTask.reset();
        }
    }


    std::pair<std::string,bool> InterfaceJson = toolboxv8handler->GetInterfaceState();
    if(InterfaceJson.second) WriteStringToFile("interface.json", InterfaceJson.first);


    std::pair<ToolboxV8Handler::ResultClass,bool> res = toolboxv8handler->GetResult();

    if(res.second)
    {
        if(BrowserScenario)
        {
            std::string id;
            if(!res.first.Id.empty())
            {
                id = res.first.Id;
            }else
            {
                id += std::to_string( std::rand()%9 + 1);
                for(int i = 0;i<8;i++)
                {
                    id += std::to_string( std::rand()%10 );
                }
            }




            std::string scriptscenario;
            if(Layout->State == MainLayout::Ready)
            {
                if(res.first.Name.length() == 0 || res.first.Name.at(0) != '_')
                {
                    Layout->UpdateState(MainLayout::Hold);
                    std::string CodeSend;
                    if(res.first.HowToExecute != ToolboxV8Handler::OnlyAdd)
                    {
                        CodeSend += std::string("_sa(") + id + std::string(");") + res.first.Code;
                    }

                    if(res.first.CanRestartBrowser)
                    {
                        //Set current action after possible browser restart
                        CodeSend += std::string(" \n section_start(\"_execution_point\", 0)!");
                    }else
                    {
                        //Do nothing with execution point
                        CodeSend += std::string(" \n section_end()!");
                    }
                    if(BrowserToolbox)
                        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_ShowWaiting(") + picojson::value(CodeSend).serialize() + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
                    xml_encode(CodeSend);
                    std::string DelayedSendCode = std::string("<WaitCode is_play='0' >") + CodeSend + std::string("</WaitCode>");
                    if(res.first.HowToExecute == ToolboxV8Handler::OnlyExecute)
                    {
                        SendTextResponce(DelayedSendCode);
                        DelayedSend.clear();
                    }
                    else
                        DelayedSend = DelayedSendCode;

                    scriptscenario += Javascript("BrowserAutomationStudio_RunningTask();","scenario");

                }
            }

            if(res.first.HowToExecute != ToolboxV8Handler::OnlyExecute)
            {
                std::string script = "BrowserAutomationStudio_AddTask(";
                std::string Name;
                /*if(res.first.HowToExecute == ToolboxV8Handler::OnlyAdd && res.first.DisableIfAdd)
                {
                    Name += "_";
                }*/
                Name += res.first.Name;
                script.append(picojson::value(Name).serialize());
                script.append(",");
                script.append(picojson::value(res.first.Code).serialize());
                script.append(",");
                script.append(id);
                script.append(");");
                script = Javascript(script,"scenario");
                WORKER_LOG(std::string("ScenarioExecuteCode<<") + script);
                scriptscenario += script;
            }

            if(!scriptscenario.empty())
                BrowserScenario->GetMainFrame()->ExecuteJavaScript(scriptscenario,BrowserScenario->GetMainFrame()->GetURL(), 0);
        }
    }

    {
        std::pair<ToolboxV8Handler::ExecuteClass, bool> res2 = toolboxv8handler->GetExecuteCode();
        if(res2.second)
        {
            std::string CodeSend = res2.first.Execute;
            WORKER_LOG(std::string("GetExecuteCode<<") + CodeSend);
            xml_encode(CodeSend);
            SendTextResponce(std::string("<WaitCode is_play='0' >") + CodeSend + std::string("</WaitCode>"));

        }
    }

    if(toolboxv8handler->GetIncrementHighlightIndex())
    {
        HighlightIndex++;
        HighlightDoScrolling = true;
        LastHighlightIndexChanged = clock();
        IsHighlightIndexActive = true;
    }

    if(Data->IsRecord)
    {
        std::pair<std::string, bool> res = toolboxv8handler->GetEmbeddedData();
        if(res.second)
        {
            std::string DataSend = res.first;
            xml_encode(DataSend);
            SendTextResponce(std::string("<EmbeddedData>") + DataSend + std::string("</EmbeddedData>"));
        }
    }


    if(toolboxv8handler->GetIsEditCancel())
    {
        std::string script = Javascript(std::string("BrowserAutomationStudio_EditCancel()"),"scenario");
        WORKER_LOG("BrowserAutomationStudio_EditCancel<<");
        if(BrowserScenario)
            BrowserScenario->GetMainFrame()->ExecuteJavaScript(script,BrowserScenario->GetMainFrame()->GetURL(), 0);
    }

    if(toolboxv8handler->GetIsInterrupt())
    {
        WORKER_LOG("BrowserAutomationStudio_Interrupt<<");
        if(Layout->IsManualControlAction)
        {
            Data->SetIndirectControlOnNext = true;
        }
        SendTextResponce("<Interrupt></Interrupt>");
    }

    if(toolboxv8handler->GetIsInitialized() && (ResourcesChanged))
    {
        std::string script = Javascript(std::string("BrowserAutomationStudio_SetResources(") + picojson::value(Resources.data()).serialize() + "," + picojson::value(AdditionalResources.data()).serialize() + std::string(")"),"toolbox");
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(script,BrowserToolbox->GetMainFrame()->GetURL(), 0);
        ResourcesChanged = false;
    }

    if(toolboxv8handler->GetIsInitialized() && !Variables.empty())
    {
        std::string script = Javascript(std::string("BrowserAutomationStudio_SetVariables(") + picojson::value(Variables.data()).serialize() + std::string(")"),"toolbox");
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(script,BrowserToolbox->GetMainFrame()->GetURL(), 0);
        Variables.clear();
    }

    if(toolboxv8handler->GetIsInitialized() && !GlobalVariables.empty())
    {
        std::string script = Javascript(std::string("BrowserAutomationStudio_SetGlobalVariables(") + picojson::value(GlobalVariables.data()).serialize() + std::string(")"),"toolbox");
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(script,BrowserToolbox->GetMainFrame()->GetURL(), 0);
        GlobalVariables.clear();
    }

    if(!IsToolboxInterfaceInitialSent && toolboxv8handler->GetIsInitialized())
    {
        std::string InterfaceInitial = ReadAllString("interface.json");
        std::string script = Javascript(std::string("BrowserAutomationStudio_LoadInterfaceState(") + picojson::value(InterfaceInitial).serialize() + std::string(")"),"toolbox");
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(script,BrowserToolbox->GetMainFrame()->GetURL(), 0);
        IsToolboxInterfaceInitialSent = true;
    }

    if(toolboxv8handler->GetIsInitialized() && !Schema.empty())
    {
        std::string script = Javascript(std::string("BrowserAutomationStudio_SetSchema(") + picojson::value(Schema).serialize() + std::string(")"),"toolbox");
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(script,BrowserToolbox->GetMainFrame()->GetURL(), 0);
        Schema.clear();
    }

    if(toolboxv8handler->GetIsInitialized() && !Labels.empty())
    {
        std::string script = Javascript(std::string("BrowserAutomationStudio_SetLabels(") + picojson::value(Labels.data()).serialize() + std::string(")"),"toolbox");
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(script,BrowserToolbox->GetMainFrame()->GetURL(), 0);
        Labels.clear();
    }

    if(toolboxv8handler->GetIsInitialized() && !Functions.empty())
    {
        std::string script = Javascript(std::string("BrowserAutomationStudio_SetFunctions(") + picojson::value(Functions.data()).serialize() + std::string(")"),"toolbox");
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(script,BrowserToolbox->GetMainFrame()->GetURL(), 0);
        Functions.clear();
    }

    if(toolboxv8handler->GetIsInitialized() && !EmbeddedData.empty())
    {
        std::string script = Javascript(std::string("BrowserAutomationStudio_SetEmbeddedData(") + picojson::value(EmbeddedData.data()).serialize() + std::string(",") + picojson::value(Settings->Languages().data()).serialize() + std::string(")"),"toolbox");
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(script,BrowserToolbox->GetMainFrame()->GetURL(), 0);
        EmbeddedData.clear();
    }

    if(toolboxv8handler->GetIsMaximize())
    {
        if(BrowserToolbox)
        {
            Layout->MaximizeToolbox(GetData()->WidthBrowser,GetData()->HeightBrowser,GetData()->WidthAll,GetData()->HeightAll);
            BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript("BrowserAutomationStudio_MaximizeCallback()","toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);

        }
    }

    if(toolboxv8handler->GetIsMinimize())
    {
        if(BrowserToolbox)
        {
            Layout->MinimizeToolbox(GetData()->WidthBrowser,GetData()->HeightBrowser,GetData()->WidthAll,GetData()->HeightAll);
        }
    }

    {
        std::pair<std::string, bool> res = toolboxv8handler->GetLoadUrl();
        if(res.second)
        {
            WORKER_LOG(std::string("LoadUrlFromUrlBrowser<<") + res.first);
            if(res.first == std::string("settings://settings"))
            {
                LoadSettingsPage();
            }else
            {
                ShellExecute(0, 0, s2ws(res.first).c_str(), 0, 0 , SW_SHOW );
            }
        }
    }

    {
        std::pair<std::string, bool> res = toolboxv8handler->GetEnableModule();
        if(res.second)
        {
            WORKER_LOG(std::string("EnableModule<<") + res.first);
            EnableModule(res.first);
            Restart();
        }
    }

    {
        std::pair<ToolboxV8Handler::MultiSelectStateClass, bool> res = toolboxv8handler->GetMultiselectStateChanged();
        if(res.second)
        {
            Data->_MultiSelectData.IsDirty = true;
            Data->_MultiSelectData.Clear();
            Data->MultiselectMode = res.first.IsActive;
            if(Data->MultiselectMode)
            {
                if(res.first.Serialized.empty())
                {
                    //New action
                    MultiSelectDataItem Item;
                    Item.IsInclude = true;
                    Item.selector = LastUsedSelector;
                    Item.label = LastUsedLabel;
                    Data->_MultiSelectData.OriginalSelector = LastUsedSelector;
                    Data->_MultiSelectData.Items.push_back(Item);
                }else
                {
                    //Editing old action
                }
                //WORKER_LOG(std::string("MultiselectStateChanged<<") + Data->_Inspect.css);
            }
        }
    }

    if(toolboxv8handler->GetMultiselectReset())
    {
        Data->_MultiSelectData.IsDirty = true;

        Data->_MultiSelectData.Clear();

        UpdateMultiSelect();
    }

    if(toolboxv8handler->GetIsEventTrigger())
    {
        if(BrowserScenario)
        {
            std::string Name = picojson::value(toolboxv8handler->GetEventTriggerName()).serialize();
            std::string Data = picojson::value(toolboxv8handler->GetEventTriggerData()).serialize();
            std::string Script = Javascript(std::string("BrowserAutomationStudio_HandleEvent(") + Name + "," + Data + std::string(")"), "scenario");
            BrowserScenario->GetMainFrame()->ExecuteJavaScript(Script, BrowserScenario->GetMainFrame()->GetURL(), 0);
        }
    }
}

void MainApp::UpdateScrolls(std::string& data)
{
    std::string str = data;

    std::size_t pos = str.find(",");
    if(pos != std::string::npos)
    {
        std::string part = str.substr(0,pos);
        str = str.substr(pos + 1,str.length() - pos - 1);
        if(ExecuteFrameChain.empty())
            Data->ScrollX = std::stoi(part);
    }

    pos = str.find(",");
    if(pos != std::string::npos)
    {
        std::string part = str.substr(0,pos);
        str = str.substr(pos + 1,str.length() - pos - 1);
        if(ExecuteFrameChain.empty())
            Data->ScrollY = std::stoi(part);
    }
    data = str;
}


void MainApp::HandleMainBrowserEvents()
{

    if(Data->IsRecord && HighlightTask && HighlightTask->GetIsFinished())
    {

        JsonParser Parser;
        std::string HighlightNow = Parser.GetStringFromJson(HighlightTask->GetString(),"RESULT");
        int positionx = Parser.GetFloatFromJson(HighlightTask->GetString(),"POSITIONX");
        int positiony = Parser.GetFloatFromJson(HighlightTask->GetString(),"POSITIONY");


        Data->_Highlight.highlights.clear();

        std::vector<std::string> s = split(HighlightNow,';');

        HighlightResult::rect r;

        for(int i = 0;i<s.size();i++)
        {
            switch(i%6)
            {
                case 0: r.x = std::stoi(s[i]) + positionx;r.y = 0;r.height = -1;r.width = -1; break;
                case 1: r.y = std::stoi(s[i]) + positiony; break;
                case 2: r.width = std::stoi(s[i]); break;
                case 3: r.height = std::stoi(s[i]);  break;
                case 4: r.index = std::stoi(s[i]);  break;
                case 5: r.is_alternative = s[i] == std::string("true"); Data->_Highlight.highlights.push_back(r);  break;
            }
        }
        for(HighlightResult::rect & r:Data->_Highlight.highlights)
        {
            r.selector = RawHighlightSelector;
        }
        {
            RECT r = Layout->GetBrowserRectangle(GetData()->WidthBrowser,GetData()->HeightBrowser,GetData()->WidthAll,GetData()->HeightAll);
            InvalidateRect(Data->_MainWindowHandle,&r,false);
        }
        HighlightTask.reset();
    }

    if(RecaptchaV3Task && RecaptchaV3Task->GetIsFinished())
    {
        JsonParser Parser;
        std::string Result = Parser.GetStringFromJson(RecaptchaV3Task->GetString(),"RESULT");

        std::string InspectId = Parser.GetStringFromJson(Result,"InspectId");
        std::string ActionName = Parser.GetStringFromJson(Result,"ActionName");
        std::string SiteKey = Parser.GetStringFromJson(Result,"SiteKey");
        std::string Url = Parser.GetStringFromJson(Result,"Url");

        if(!ActionName.empty() && Data->IsRecord)
        {
            if(BrowserToolbox)
            {
                BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_Notify('recaptchav3',") + picojson::value(ActionName).serialize() + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
            }
        }

        if(!InspectId.empty())
        {


            xml_encode(ActionName);
            xml_encode(SiteKey);
            xml_encode(Url);
            xml_encode(InspectId);
            SendTextResponce(
                        std::string("<SolveRecaptchaV3 Id=\"") + InspectId + std::string("\"") +
                        std::string(" Action=\"") + ActionName + std::string("\"") +
                        std::string(" SiteKey=\"") + SiteKey + std::string("\"") +
                        std::string(" Url=\"") + Url + std::string("\"") +
                        std::string(" ></SolveRecaptchaV3>"));

        }

        RecaptchaV3Task.reset();
    }

    if(HighlightMultiselectTask && HighlightMultiselectTask->GetIsFinished())
    {
        JsonParser Parser;
        std::string Result = Parser.GetStringFromJson(HighlightMultiselectTask->GetString(),"RESULT.result");
        std::string Report = Parser.GetStringFromJson(HighlightMultiselectTask->GetString(),"RESULT.report");
        int PositionX = Parser.GetFloatFromJson(HighlightMultiselectTask->GetString(),"POSITIONX");
        int PositionY = Parser.GetFloatFromJson(HighlightMultiselectTask->GetString(),"POSITIONY");
        Data->_MultiSelectData.UpdatePositions(Result,PositionX,PositionY);
        if(!Report.empty() && BrowserToolbox && Data->MultiselectMode)
        {
            BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_MultiSelectReport(") + Report + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
        }
        HighlightMultiselectTask.reset();
    }

    if(InspectTask && InspectTask->GetIsFinished())
    {
        JsonParser Parser;

        Data->_Inspect.x = Parser.GetFloatFromJson(InspectTask->GetString(),"x");
        Data->_Inspect.y = Parser.GetFloatFromJson(InspectTask->GetString(),"y");
        Data->_Inspect.mousex = Parser.GetFloatFromJson(InspectTask->GetString(),"mousex");
        Data->_Inspect.mousey = Parser.GetFloatFromJson(InspectTask->GetString(),"mousey");
        Data->_Inspect.width = Parser.GetFloatFromJson(InspectTask->GetString(),"width");
        Data->_Inspect.height = Parser.GetFloatFromJson(InspectTask->GetString(),"height");
        Data->_Inspect.label = Parser.GetStringFromJson(InspectTask->GetString(),"label");
        Data->_Inspect.css = Parser.GetStringFromJson(InspectTask->GetString(),"css");
        Data->_Inspect.css2 = Parser.GetStringFromJson(InspectTask->GetString(),"css2");
        Data->_Inspect.css3 = Parser.GetStringFromJson(InspectTask->GetString(),"css3");
        Data->_Inspect.xpath = Parser.GetStringFromJson(InspectTask->GetString(),"xpath");
        Data->_Inspect.match = Parser.GetStringFromJson(InspectTask->GetString(),"match");
        Data->_Inspect.active = Parser.GetBooleanFromJson(InspectTask->GetString(),"active");
        Data->_Inspect.position = Parser.GetFloatFromJson(InspectTask->GetString(),"position");

        Data->_MultiSelectData.MouseOverInspectData = Data->_Inspect;
        Data->_MultiSelectData.MouseOverType = MouseOverInspect;


        if(Data->ManualControl == BrowserData::DirectRecord)
        {
            DirectControl()->SetInspectResult(Data->_Inspect);
        }

        InspectTask.reset();


        RECT r = Layout->GetBrowserRectangle(GetData()->WidthBrowser,GetData()->HeightBrowser,GetData()->WidthAll,GetData()->HeightAll);
        InvalidateRect(Data->_MainWindowHandle,&r,false);
    }

}

std::pair<int,int> MainApp::GetScrollPosition()
{
    std::pair<int,int> res;
    res.first = Data->ScrollX;
    res.second = Data->ScrollY;
    return res;
}


void MainApp::FinishedLastCommand(const std::string& data)
{
    SendTextResponce(std::string("<Element ID=\"") + LastCommand.CommandId + std::string("\"><") + LastCommand.CommandName + std::string(">") + data + std::string("</") + LastCommand.CommandName + ("></Element>"));
}

void MainApp::ExecuteMouseMove()
{
    if(!IsMouseMoveSimulation)
        return;

    int CursorX = Data->CursorX;
    int CursorY = Data->CursorY;

    double MouseSpeedActual = MouseSpeed;
    double MouseDeviationActual = MouseDeviation;

    if(Settings->EmulateMouse())
    {
        BrowserEventsEmulator::MouseMove(Data->Connector, IsMouseMoveSimulation, MouseStartX, MouseStartY, MouseEndX, MouseEndY , CursorX, CursorY, MouseSpeedActual, Data->WidthBrowser, Data->HeightBrowser, MouseGravity, MouseDeviationActual, 0.0f, false, true,Data->IsMousePress,Data->IsDrag,Data->IsTouchScreen,Data->TouchEventId,Data->IsTouchPressedAutomation, TypeTextState);
    }
    else
    {
        BrowserEventsEmulator::MouseMoveLine(Data->Connector, IsMouseMoveSimulation, MouseStartX, MouseStartY, MouseEndX, MouseEndY , CursorX, CursorY, MouseSpeedActual, Data->WidthBrowser, Data->HeightBrowser,Data->IsMousePress,Data->IsDrag,Data->IsTouchScreen,Data->TouchEventId,Data->IsTouchPressedAutomation, TypeTextState);
    }
    Data->CursorX = CursorX;
    Data->CursorY = CursorY;
    Data->DirectControlOrAutomationCursorX = Data->CursorX;
    Data->DirectControlOrAutomationCursorY = Data->CursorY;

    RECT r = Layout->GetBrowserRectangle(GetData()->WidthBrowser,GetData()->HeightBrowser,GetData()->WidthAll,GetData()->HeightAll);
    InvalidateRect(Data->_MainWindowHandle,&r,false);

    bool TrackScroll = false;

    if(MouseReleaseRadius > 0.01 && DoMouseUpOnFinishMove)
    {
        //Check if need to release mouse during movement
        float DistanceSquareCurrent = sqrtf((MouseEndX - CursorX) * (MouseEndX - CursorX) + (MouseEndY - CursorY) * (MouseEndY - CursorY));

        if(DistanceSquareCurrent < MouseReleaseRadius)
        {
            IsMouseMoveSimulation = false;

            if(Data->IsTouchScreen && DoTrackScroll)
            {
                TrackScroll = true;
                DoTrackScroll = false;
            }
        }
    }



    if(!IsMouseMoveSimulation)
    {
        if(LastCommand.CommandName == "move")
            FinishedLastCommand("");
        else
        {
            if(DoMouseUpOnFinishMove)
            {
                Data->LastClickIsFromIndirectControl = true;
                BrowserEventsEmulator::MouseClick(Data->Connector,Data->CursorX,Data->CursorY,GetScrollPosition(),1,Data->IsMousePress,Data->IsDrag,Data->IsTouchScreen,Data->TouchEventId,Data->IsTouchPressedAutomation,TypeTextState);
            }
            if(TrackScroll)
            {
                ScrollStopTracking = clock();
                ScrollTrackingX = Data->ScrollX;
                ScrollTrackingY = Data->ScrollY;
                ScrollStopTrackingStart = ScrollStopTracking;
            }else
            {
                SendTextResponce("<MouseMove></MouseMove>");
            }

        }
    }
}

void MainApp::ExecuteTypeText()
{
    if(!TypeTextTaskIsActive)
        return;

    clock_t CurrentTime = clock();


    //Finish pressing character

    if(TypeTextState.IsPresingCharacter())
    {
        if(CurrentTime > TypeTextState.PresingKeyNext)
        {
            BrowserEventsEmulator::Key(Data->Connector,TypeText,TypeTextState,Data->CursorX,Data->CursorY,Data->IsTouchScreen);
            if(TypeText.length() == 0 && TypeTextState.IsClear())
            {
                //Nothing more to type
                TypeTextTaskIsActive = false;
                FinishedLastCommand("");
            }else
            {
                TypeTextDelayCurrent = TypeTextDelay + (rand()) % ((int)(TypeTextDelay * 1.6)) - (int)(TypeTextDelay * 0.8);
                TypeTextLastTime = CurrentTime;
            }
        }
        return;
    }


    //Do a click before first text input

    if(TypeTextIsFirstLetter && DelayClickType == 3 && DelayNextClick == 0)
    {
        DelayNextClick = clock() + 80 + (rand()) % 40;
        BrowserEventsEmulator::MouseClick(Data->Connector,TypeTextX,TypeTextY,GetScrollPosition(),2,Data->IsMousePress,Data->IsDrag,Data->IsTouchScreen,Data->TouchEventId,Data->IsTouchPressedAutomation,TypeTextState);
        return;
    }

    if(TypeTextIsFirstLetter && DelayClickType == 3 && DelayNextClick > 0)
    {
        if(CurrentTime < DelayNextClick)
            return;

        BrowserEventsEmulator::MouseClick(Data->Connector,TypeTextX,TypeTextY,GetScrollPosition(),1,Data->IsMousePress,Data->IsDrag,Data->IsTouchScreen,Data->TouchEventId,Data->IsTouchPressedAutomation,TypeTextState);
        DelayClickType = 0;
    }



    //Check if there was enough time to do a next click

    if(!TypeTextIsFirstLetter && float( CurrentTime - TypeTextLastTime ) /  CLOCKS_PER_SEC < (float)TypeTextDelayCurrent / 1000.0f)
        return;
    TypeTextLastTime = CurrentTime;



    //Type first letter - type all instantly all delay next key press

    if(TypeTextIsFirstLetter)
    {
        Data->LastClickIsFromIndirectControl = true;
        WORKER_LOG(std::string("TypeTextIsFirstLetter<<") + std::to_string(TypeTextX) + std::string("<<") + std::to_string(TypeTextY));
        TypeTextIsFirstLetter = false;
        if(TypeTextDelay == 0)
        {
            for(int i = 0;i<1000;i++)
            {
                BrowserEventsEmulator::Key(Data->Connector,TypeText,TypeTextState,Data->CursorX,Data->CursorY,Data->IsTouchScreen);

                if(TypeText.length() == 0 && TypeTextState.IsClear() && !TypeTextState.IsPresingCharacter())
                {
                    TypeTextTaskIsActive = false;
                    FinishedLastCommand("");
                    return;
                }

                if(i>100 && !TypeTextState.IsPresingCharacter())
                    break;
            }
            return;
        }else
        {
            TypeTextDelayCurrent = TypeTextDelay + (rand()) % ((int)(TypeTextDelay * 1.6)) - (int)(TypeTextDelay * 0.8);
        }
        return;
    }

    //Start typing

    if(TypeTextDelay == 0)
    {
        //Print all letters instantly
        for(int i = 0;i<1000;i++)
        {
            BrowserEventsEmulator::Key(Data->Connector,TypeText,TypeTextState,Data->CursorX,Data->CursorY,Data->IsTouchScreen);
            if(TypeText.length() == 0 && TypeTextState.IsClear() && !TypeTextState.IsPresingCharacter())
            {
                TypeTextTaskIsActive = false;
                FinishedLastCommand("");
                return;
            }

            if(i>100 && !TypeTextState.IsPresingCharacter())
                break;
        }
        return;
    }else
    {
        //Print one letter
        BrowserEventsEmulator::Key(Data->Connector,TypeText,TypeTextState,Data->CursorX,Data->CursorY,Data->IsTouchScreen);
        TypeTextDelayCurrent = TypeTextDelay + (rand()) % ((int)(TypeTextDelay * 1.6)) - (int)(TypeTextDelay * 0.8);
    }

    //Need to generate key up
    if(TypeTextState.IsPresingCharacter())
    {
        TypeTextState.PresingKeyNext = clock() + 80 + (rand()) % 40;
        return;
    }

    if(TypeText.length() == 0 && TypeTextState.IsClear())
    {
        TypeTextTaskIsActive = false;
        FinishedLastCommand("");
    }
}



void MainApp::ScrollUp()
{
    Layout->HideCentralBrowser();
    {
        Data->_Inspect.active = false;
    }
    KeyState TypeTextState;
    std::string KeyText = "<MOUSESCROLLUP>";
    BrowserEventsEmulator::Key(Data->Connector,KeyText,TypeTextState,Data->CursorX,Data->CursorY,false);
}

void MainApp::ScrollDown()
{
    Layout->HideCentralBrowser();
    {
        Data->_Inspect.active = false;
    }

    KeyState TypeTextState;
    std::string KeyText = "<MOUSESCROLLDOWN>";

    BrowserEventsEmulator::Key(Data->Connector,KeyText,TypeTextState,Data->CursorX,Data->CursorY,false);
}

void MainApp::ScrollUpUp()
{
    Layout->HideCentralBrowser();
    {
        Data->_Inspect.active = false;
    }

    KeyState TypeTextState;
    std::string KeyText = "<HOME>";
    BrowserEventsEmulator::Key(Data->Connector,KeyText,TypeTextState,Data->CursorX,Data->CursorY,false);
}
void MainApp::ScrollDownDown()
{
    Layout->HideCentralBrowser();
    {
        Data->_Inspect.active = false;
    }

    KeyState TypeTextState;
    std::string KeyText = "<END>";

    BrowserEventsEmulator::Key(Data->Connector,KeyText,TypeTextState,Data->CursorX,Data->CursorY,false);
}

void MainApp::ScrollLeft()
{
    Layout->HideCentralBrowser();
    {
        Data->_Inspect.active = false;
    }

    Data->Connector->ExecuteJavascript(Javascript("_BAS_HIDE(BrowserAutomationStudio_ScrollLeft)()","main"),std::string(),std::string("[]"));
}
void MainApp::ScrollRight()
{
    Layout->HideCentralBrowser();
    {
        Data->_Inspect.active = false;
    }

    Data->Connector->ExecuteJavascript(Javascript("_BAS_HIDE(BrowserAutomationStudio_ScrollRight)()","main"),std::string(),std::string("[]"));
}

void MainApp::ScrollLeftLeft()
{
    Layout->HideCentralBrowser();
    {
        Data->_Inspect.active = false;
    }

    Data->Connector->ExecuteJavascript(Javascript("_BAS_HIDE(BrowserAutomationStudio_ScrollLeftLeft)()","main"),std::string(),std::string("[]"));

}
void MainApp::ScrollRightRight()
{
    Layout->HideCentralBrowser();
    {
        Data->_Inspect.active = false;
    }

    Data->Connector->ExecuteJavascript(Javascript("_BAS_HIDE(BrowserAutomationStudio_ScrollRightRight)()","main"),std::string(),std::string("[]"));

}

void MainApp::EmulateClick(int x, int y)
{
    if(BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_Click(") + std::to_string(x) + std::string(",") + std::to_string(y) + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
}

void MainApp::EmulateMove(int x, int y)
{
    if(BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_Move(") + std::to_string(x) + std::string(",") + std::to_string(y) + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
}

void MainApp::EmulateDrag(int x, int y)
{
    if(BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_Drag(") + std::to_string(x) + std::string(",") + std::to_string(y) + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
}

void MainApp::EmulateDrop(int x, int y)
{
    if(BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_Drop(") + std::to_string(x) + std::string(",") + std::to_string(y) + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
}

void MainApp::EmulateMoveAndClick(int x, int y)
{
    if(BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_MoveAndClick(") + std::to_string(x) + std::string(",") + std::to_string(y) + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
}

void MainApp::AddTab()
{
    if(BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_AddTab()"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
}

void MainApp::SelectTab(int i)
{
    if(BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_SelectTab(") + std::to_string(i) + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
}

void MainApp::TabInfo()
{
    if(BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_TabInfo()"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
}


void MainApp::CloseTab(int i)
{
    if(BrowserToolbox)
        BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_CloseTab(") + std::to_string(i) + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
}

void MainApp::Terminate()
{
    SendTextResponce("<Terminate/>");
}

void MainApp::Restart()
{
    SendTextResponce("<Restart/>");
}

//Element Subtasks
void MainApp::ExecuteElementFunction(const std::string& FuncName, bool AskIfUseLoopFunction, bool IsDisabled, const std::string& ModuleName, const std::string& ModuleDescription)
{
    if(BrowserToolbox)
    {
        if(IsDisabled)
        {
            std::string ModuleNameSerialized = picojson::value(ModuleName).serialize();
            std::string ModuleDescriptionSerialized = picojson::value(ModuleDescription).serialize();

            BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_AskEnableModule(") + ModuleNameSerialized + std::string(",") + ModuleDescriptionSerialized + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
        }else
        {
            std::string serialize;
            {
                LOCK_BROWSER_DATA
                serialize = Data->_Inspect.Serialize();
            }
            std::string AskIfUseLoopFunctionString = (AskIfUseLoopFunction ? std::string("1") : std::string("0"));
            BrowserToolbox->GetMainFrame()->ExecuteJavaScript(Javascript(std::string("BrowserAutomationStudio_") + FuncName + std::string("(") + serialize + std::string(",") + AskIfUseLoopFunctionString + std::string(")"),"toolbox"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
        }
    }
}

void MainApp::LoadSettingsPage()
{
    if(BrowserCentral)
    {
        BrowserCentral->GetMainFrame()->ExecuteJavaScript("document.body.style.display='none'","file:///html/central/empty.html",0);
        std::string page = std::string("file:///html/central/index_settings.html?d=") + CefURIEncode(Settings->Serialize(),true).ToString();
        BrowserCentral->GetMainFrame()->LoadURL(page);
        Layout->ShowCentralBrowser(true,false);
    }

}

void MainApp::ShowContextMenu(int X, bool IsImageSelect, const std::string & Json)
{
    if(BrowserCentral)
    {
        {
            LOCK_BROWSER_DATA
            LastUsedSelector = Data->_Inspect.css;
            LastUsedLabel = Data->_Inspect.label;
        }
        BrowserCentral->GetMainFrame()->ExecuteJavaScript("document.body.style.display='none'","file:///html/central/empty.html",0);
        std::string page = std::string("file:///html/menu/index.html?is_image_select=") + std::to_string(IsImageSelect) + std::string("&data=") + Json;
        BrowserCentral->GetMainFrame()->LoadURL(page);
        Layout->ShowContextMenu(X, GetData()->WidthBrowser, GetData()->HeightBrowser, GetData()->WidthAll, GetData()->HeightAll, Settings->Zoom());
    }
}

void MainApp::MainContextMenu(POINT& p)
{
    Async Result = Data->Connector->GetHistory();
    std::vector<std::pair<std::string, std::string> > Extensions = Data->Connector->GetExtensionList();
    Result->Then([this, p, Extensions](AsyncResult* Result)
    {
        POINT pcopy = p;
        Data->_BrowserContextMenu.ShowMenu(Data->_MainWindowHandle, pcopy, Data->IsRecord, Result->GetInteger() > 0 , Result->GetInteger() < Result->GetList().size() - 1, Extensions);
    });

}

void MainApp::ShowActionUpdater()
{
    if(!BrowserScenario) return;
    BrowserScenario->GetMainFrame()->ExecuteJavaScript("BrowserAutomationStudio_ShowActionUpdater()",BrowserScenario->GetMainFrame()->GetURL(), 0);
}

void MainApp::HideActionUpdater()
{
    if(!BrowserScenario) return;
    BrowserScenario->GetMainFrame()->ExecuteJavaScript("BrowserAutomationStudio_HideActionUpdater()",BrowserScenario->GetMainFrame()->GetURL(), 0);
}
