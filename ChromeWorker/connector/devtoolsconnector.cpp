#include "devtoolsconnector.h"
#include <string>
#include <set>
#include <random>
#include <chrono>
#include <windows.h>
#include <fstream>
#include "converter.h"
#include "json/picojson.h"
#include "devtoolsactionwebsocketquery.h"
#include "match.h"
#include "fileutils.h"
#include "startwith.h"
#include "base64.h"
#include "replaceall.h"
#include "readallfile.h"

using namespace std::placeholders;
using namespace std::chrono;

void DevToolsConnector::ResetProxy(const std::string& ParentProcessId)
{
    // Generate proxy data
    GlobalState.SaveProxy->WriteDirectConnectionConfig();
}

void DevToolsConnector::SetInitialProxy(const std::string& InitalProxy)
{
    if(InitalProxy == "block")
    {
        return;
    }

    if(InitalProxy == "direct")
    {
        ResetProxy(GlobalState.ParentProcessId);
        GlobalState.IsProxySet = true;
        return;
    }

    try
    {
        picojson::value Value;
        picojson::parse(Value, base64_decode(InitalProxy));
        picojson::object Object = Value.get<picojson::object>();
        std::string server = Object["server"].get<std::string>();
        int Port = Object["Port"].get<double>();
        bool IsHttp = Object["IsHttp"].get<bool>();
        std::string name = Object["name"].get<std::string>();
        std::string password = Object["password"].get<std::string>();
        GlobalState.SaveProxy->WriteProxyConfig(server, Port, IsHttp, name, password);
        GlobalState.IsProxySet = true;
        return;

    }catch(...)
    {

    }

}

std::wstring DevToolsConnector::GetProxyConfigFolder()
{
    return GlobalState.SaveProxy->GetConfigFilePath(std::string());
}


void DevToolsConnector::Initialize
(
        std::shared_ptr<ISimpleHttpClientFactory> SimpleHttpClientFactory,
        std::shared_ptr<IWebSocketClientFactory> WebSocketClientFactory,
        int Port, const std::string& UniqueProcessId, const std::string& ParentProcessId,
        const std::string& ConstantStartupScript,
        const std::vector<std::pair<std::string,std::string> >& CommandLineAdditional,
        const std::string& InitalProxy, int PcapPort
)
{

    std::wstring RootFolder = GetRelativePathToParentFolder(L"");

    this->CommandLineAdditional = CommandLineAdditional;
    this->SimpleHttpClientFactory = SimpleHttpClientFactory;
    this->WebSocketClientFactory = WebSocketClientFactory;

    GlobalState.SaveProxy.reset(new ProxySaver());
    GlobalState.SaveProxy->Initialize(ParentProcessId, PcapPort);

    ISimpleHttpClient * HttpClient = this->SimpleHttpClientFactory->Create();
    HttpClient->GlobalActivate();
    delete HttpClient;

    IWebSocketClient * WebSocketClient = this->WebSocketClientFactory->Create();
    WebSocketClient->GlobalActivate();
    delete WebSocketClient;

    this->IsConnectionOrLaunch = true;
    this->TargetPort = Port;
    GlobalState.Port = Port;
    GlobalState.UniqueProcessId = UniqueProcessId;
    GlobalState.ParentProcessId = ParentProcessId;
    GlobalState.ConstantStartupScript = ConstantStartupScript;

    ImageData.clear();
    ImageDataScaled.clear();
    HasScaledImage = false;
    IPC = new SharedMemoryIPC();
    IPC->Start(UniqueProcessId);

    for(auto Entry : GetFilesInDirectory(RootFolder + L"\\ex\\d"))
    {
        if(Entry.IsDirectory)
            this->DefaultExtensions.push_back(Entry.PathWString);
    }

    /*for(auto Entry : GetFilesInDirectory(RootFolder + L"\\ex\\o"))
    {
        if(Entry.IsDirectory)
            this->OptionalExtensions.push_back(s2ws(Entry.Path));
    }*/

    SetInitialProxy(InitalProxy);
}

char* DevToolsConnector::GetPaintData()
{
    return ImageData.data();
}

int DevToolsConnector::GetPaintWidth()
{
    return PaintWidth;
}

int DevToolsConnector::GetPaintHeight()
{
    return PaintHeight;
}

char* DevToolsConnector::GetPaintDataScaled()
{
    if(HasScaledImage && !ImageDataScaled.empty())
    {
        return ImageDataScaled.data();
    }
    return ImageData.data();
}

int DevToolsConnector::GetPaintWidthScaled()
{
    if(HasScaledImage && !ImageDataScaled.empty())
    {
        return PaintWidthScaled;
    }
    return PaintWidth;
}

int DevToolsConnector::GetPaintHeightScaled()
{
    if(HasScaledImage && !ImageDataScaled.empty())
    {
        return PaintHeightScaled;
    }
    return PaintHeight;
}

int DevToolsConnector::GetWidth()
{
    return GlobalState.Width;
}

int DevToolsConnector::GetHeight()
{
    return GlobalState.Height;
}

int DevToolsConnector::GetScrollX()
{
    return GlobalState.ScrollX;
}

int DevToolsConnector::GetScrollY()
{
    return GlobalState.ScrollY;
}

void DevToolsConnector::SetProfilePath(const std::wstring& Path)
{
    this->ProfilePath = Path;
}

void DevToolsConnector::SetExtensionList(const std::vector<std::wstring>& Extensions)
{
    this->Extensions = Extensions;
}

void DevToolsConnector::OpenDevToolsInternal(bool IsInspect)
{
    std::wstring PageId;

    for(auto const& Tab : GlobalState.Tabs)
    {
        if(Tab->ConnectionState == TabData::Connected)
        {
            if(Tab->TabId == GlobalState.TabId)
            {
                PageId = s2ws(Tab->FrameId);
                break;
            }
        }
    }

    std::wstring IsInspectorString;

    if(IsInspect)
    {
        IsInspectorString = L"inspect=true&";
    }

    if(!PageId.empty())
    {
        std::wstring ChromePath = GetRelativePathToExe(L"chrome");
        std::wstring WorkerPath = ChromePath + std::wstring(L"/worker.exe");



        std::wstring Url =
                std::wstring(L"http://127.0.0.1:")
                + std::to_wstring(GlobalState.Port)
                + std::wstring(L"/devtools/inspector.html?") + IsInspectorString + std::wstring(L"ws=127.0.0.1:")
                + std::to_wstring(GlobalState.Port)
                + std::wstring(L"/devtools/page/")
                + PageId;

        std::wstring CommandLine;

        CommandLine += std::wstring(L"--no-proxy-server");
        CommandLine += std::wstring(L" ");

        /*CommandLine += std::wstring(L"--disable-site-isolation-trials");
        CommandLine += std::wstring(L" ");*/

        /*CommandLine += std::wstring(L"--no-sandbox");
        CommandLine += std::wstring(L" ");*/

        /*CommandLine += std::wstring(L"--force-device-scale-factor=1");
        CommandLine += std::wstring(L" ");*/

        CommandLine += std::wstring(L"--noerrdialogs");
        CommandLine += std::wstring(L" ");

        CommandLine += std::wstring(L"--disable-smooth-scrolling");
        CommandLine += std::wstring(L" ");

        CommandLine += std::wstring(L"--bas-force-visible-window");
        CommandLine += std::wstring(L" ");

        CommandLine += std::wstring(std::wstring(L"--window-size=800,600"));
        CommandLine += std::wstring(L" ");

        CommandLine += std::wstring(std::wstring(L"--window-position=0,0"));
        CommandLine += std::wstring(L" ");

        CommandLine += Url;

        ShellExecute(0, 0, WorkerPath.c_str(), CommandLine.c_str(), ChromePath.c_str(), SW_SHOW );
    }
}

void DevToolsConnector::OpenDevTools()
{
    OpenDevToolsInternal(false);
}

void DevToolsConnector::InspectAt(int X, int Y)
{
    OpenDevToolsInternal(true);

    IsInspectAtScheduled = true;
    InspectAtTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() + 5000;
    InspectAtX = X;
    InspectAtY = Y;
}

void DevToolsConnector::InspectAtFinalize()
{
    IsInspectAtScheduled = false;
    InspectAtTime = 0;
    Mouse(MouseEventMove, InspectAtX, InspectAtY);
    Mouse(MouseEventDown, InspectAtX, InspectAtY);
    Mouse(MouseEventUp, InspectAtX, InspectAtY);
}

void DevToolsConnector::StartProcess()
{
    std::wstring CommandLine;
    CommandLine += std::wstring(L"--remote-debugging-port=") + std::to_wstring(GlobalState.Port);
    CommandLine += std::wstring(L" ");

    CommandLine += std::wstring(L"--remote-allow-origins=http://127.0.0.1:") + std::to_wstring(GlobalState.Port);
    CommandLine += std::wstring(L" ");

    CommandLine += std::wstring(L"--unique-process-id=") + s2ws(GlobalState.UniqueProcessId);
    CommandLine += std::wstring(L" ");

    CommandLine += std::wstring(L"--parent-process-id=") + s2ws(GlobalState.ParentProcessId);
    CommandLine += std::wstring(L" ");

    CommandLine += std::wstring(L"--no-proxy-server");
    CommandLine += std::wstring(L" ");

    /*CommandLine += std::wstring(L"--disable-site-isolation-trials");
    CommandLine += std::wstring(L" ");*/

    //Temporary disable sandboxing, otherwise render process can't access filesystem and therefore obtain fingerprint data.
    /*CommandLine += std::wstring(L"--no-sandbox");
    CommandLine += std::wstring(L" ");*/

    CommandLine += std::wstring(L"--force-device-scale-factor=1");
    CommandLine += std::wstring(L" ");

    CommandLine += std::wstring(L"--noerrdialogs");
    CommandLine += std::wstring(L" ");

    CommandLine += std::wstring(std::wstring(L"--window-position=0,0"));
    CommandLine += std::wstring(L" ");

    CommandLine += std::wstring(L"--disable-smooth-scrolling");
    CommandLine += std::wstring(L" ");

    for(auto p:CommandLineAdditional)
    {
        CommandLine += std::wstring(s2ws(p.first));
        if(p.second.length()>0)
        {
            CommandLine += std::wstring(L"=");
            CommandLine += std::wstring(s2ws(p.second));
        }
        CommandLine += std::wstring(L" ");
    }

    int BrowserWidth = 1024 + GlobalState.WidthDifference;
    int BrowserHeight = 600 + GlobalState.HeightDifference;
    CommandLine += std::wstring(std::wstring(L"--window-size=") + std::to_wstring(BrowserWidth) + std::wstring(L",") + std::to_wstring(BrowserHeight));
    CommandLine += std::wstring(L" ");

    if(!ProfilePath.empty())
    {
        std::wstring ProfilePathActual = ProfilePath;
        ReplaceAllInPlace(ProfilePathActual, L"\\", L"\\\\");

        CommandLine += std::wstring(L"--user-data-dir=\"") + ProfilePathActual;
        CommandLine += std::wstring(L"\" ");
    }

    std::wstring ExtensionsString;
    /*for(const std::wstring& ExtensionPath : OptionalExtensions)
    {
        if(!ExtensionsString.empty())
        {
            ExtensionsString += std::wstring(L",");
        }
        ExtensionsString += ExtensionPath;
    }*/
    for(const std::wstring& ExtensionPath : DefaultExtensions)
    {
        if(!ExtensionsString.empty())
        {
            ExtensionsString += std::wstring(L",");
        }
        ExtensionsString += ExtensionPath;
    }
    if(!Extensions.empty())
    {
        for(const std::wstring& ExtensionString : Extensions)
        {
            if(!ExtensionsString.empty())
            {
                ExtensionsString += std::wstring(L",");
            }
            ExtensionsString += ExtensionString;
        }
    }
	if(!ExtensionsString.empty())
	{
    	ReplaceAllInPlace(ExtensionsString, L"\\", L"\\\\");

    	CommandLine += std::wstring(L"--load-extension=\"") + ExtensionsString;
    	CommandLine += std::wstring(L"\" ");
	}

    CommandLine += std::wstring(L"about:blank");


    std::wstring ChromePath = GetRelativePathToExe(L"chrome");
    std::wstring WorkerPath = ChromePath + std::wstring(L"/worker.exe");

    ProcessLauncher.Start(WorkerPath, CommandLine, ChromePath);
}

void DevToolsConnector::TryToConnect()
{

    //Launch type is not set yet
    if(TargetPort < 0)
        return;

    //Already connected
    if(ConnectionState == Connected)
        return;

    //Start connection
    if(ConnectionState == NotStarted)
    {
        //Http client is in use, wait.
        if(GlobalState.HttpClient)
        {
            return;
        }


        GlobalState.HttpClient.reset(this->SimpleHttpClientFactory->Create());
        GlobalState.HttpClient->OnResult.push_back(std::bind(&DevToolsConnector::OnBrowserEndpointObtained,this,_1,_2,_3));
        std::string Url = std::string("http://127.0.0.1:") + std::to_string(TargetPort) + std::string("/json/version");
        GlobalState.HttpClient->Get(Url);
        ConnectionState = WaitingForBrowserEndpoint;
    }
}

void DevToolsConnector::OnHttpClientResult(bool IsSuccess,int StatusCode,std::string& Data)
{
    for(std::shared_ptr<IDevToolsAction> Action: Actions)
    {
        if(Action->UsesHttpClient() && Action->GetId() == CurrentHttpClientActionId)
        {
            Action->OnHttpClientMessage(IsSuccess,StatusCode,Data);
        }
    }
}

void DevToolsConnector::OnBrowserEndpointObtained(bool IsSuccess,int StatusCode,std::string& Data)
{
    if(GlobalState.HttpClient)
        GlobalState.HttpClient->DeleteLater = true;
    if(!IsSuccess || StatusCode != 200)
    {
        ConnectionState = NotStarted;
    }else
    {
        picojson::value AllValue;
        picojson::parse(AllValue, Data);
        picojson::object AllObject = AllValue.get<picojson::object>();
        Endpoint = AllObject["webSocketDebuggerUrl"].get<std::string>();

        size_t start_pos = 0;
        std::string ReplaceFrom = "127.0.0.1/";
        std::string ReplaceTo = std::string("127.0.0.1:") + std::to_string(TargetPort) + std::string("/");
        while((start_pos = Endpoint.find(ReplaceFrom, start_pos)) != std::string::npos)
        {
            Endpoint.replace(start_pos, ReplaceFrom.length(), ReplaceTo);
            start_pos += ReplaceTo.length();
        }

        GlobalState.WebSocketClient.reset(this->WebSocketClientFactory->Create());

        GlobalState.WebSocketClient->OnConnected.push_back(std::bind(&DevToolsConnector::OnWebSocketConnected, this, _1));
        GlobalState.WebSocketClient->OnDisconnected.push_back(std::bind(&DevToolsConnector::OnWebSocketDisconnected, this));
        GlobalState.WebSocketClient->OnMessage.push_back(std::bind(&DevToolsConnector::OnWebSocketMessage, this, _1));
        GlobalState.WebSocketClient->Connect(Endpoint);

        ConnectionState = WaitingForWebsocket;
    }
}

void DevToolsConnector::OnWebSocketConnected(bool IsSuccess)
{
    if(!IsSuccess)
    {
        ConnectionState = NotStarted;
        if(GlobalState.WebSocketClient)
        {
            GlobalState.WebSocketClient->DeleteLater = true;
        }
        return;
    }

    ProcessLauncher.Stop();

    if(!WasBrowserCreationEvent)
    {
        WasBrowserCreationEvent = true;
        for(auto f:OnBrowserCreated)
            f();
    }

    std::map<std::string, Variant> CurrentParams;

    CurrentParams["downloadPath"] = Variant(ws2s(GetRelativePathToParentFolder(L"")));
    CurrentParams["behavior"] = Variant(std::string("allowAndName"));

    SendWebSocket("Browser.setDownloadBehavior",  CurrentParams, std::string());
    ConnectionState = WaitingForDownloadsEnable;
}

void DevToolsConnector::OnWebSocketDisconnected()
{
    ConnectionState = NotStarted;
    if(GlobalState.WebSocketClient)
        GlobalState.WebSocketClient->DeleteLater = true;

    if(ResetResult)
    {
        StartProcess();
    }

    return;
}

void DevToolsConnector::StartFirstSavedAction(std::shared_ptr<TabData> Tab)
{
    long long Now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    Tab->SavedActions[0]->SetTimeStarted(Now);
    if(Tab->SavedActions[0]->GetTimeout() >= 0)
    {
        Tab->SavedActions[0]->SetDeadline(Tab->SavedActions[0]->GetTimeStarted() + Tab->SavedActions[0]->GetTimeout());
    } else
    {
        Tab->SavedActions[0]->SetDeadline(0);
    }

    if(Tab->IsSwitchingToTab)
        Tab->SavedActions[0]->OnTabSwitching();
    else
        Tab->SavedActions[0]->OnTabCreation();
}

void DevToolsConnector::ProcessTabConnection(std::shared_ptr<TabData> Tab)
{
    if(Tab->ConnectionState == TabData::NotStarted)
    {
        Tab->ConnectionState = TabData::WaitingForAttachment;
        std::map<std::string, Variant> Params;
        Params["targetId"] = Variant(Tab->FrameId);
        Params["flatten"] = Variant(true);
        Tab->CurrentWebsocketActionId = -1;
        SendWebSocket("Target.attachToTarget", Params, std::string());
    }else if(Tab->ConnectionState == TabData::WaitingForAttachment)
    {
        Tab->ConnectionState = TabData::WaitingForPageEnable;
        std::map<std::string, Variant> Params;
        Tab->CurrentWebsocketActionId = SendWebSocket("Page.enable", Params, Tab->TabId);
    }else if(Tab->ConnectionState == TabData::WaitingForPageEnable)
    {
        Tab->ConnectionState = TabData::WaitingForRuntimeEnable;
        std::map<std::string, Variant> Params;
        Tab->CurrentWebsocketActionId = SendWebSocket("Runtime.enable", Params, Tab->TabId);
    } else if(Tab->ConnectionState == TabData::WaitingForRuntimeEnable)
    {
        Tab->ConnectionState = TabData::WaitingForDragAndDropInit;
        std::map<std::string, Variant> Params;
        Tab->CurrentWebsocketActionId = SendWebSocket("Network.enable", Params, Tab->TabId);
    }else if(Tab->ConnectionState == TabData::WaitingForDragAndDropInit)
    {
        Tab->ConnectionState = TabData::WaitingForNetworkEnable;
        std::map<std::string, Variant> Params;
        Params["enabled"] = Variant(true);
        Tab->CurrentWebsocketActionId = SendWebSocket("Input.setInterceptDrags", Params, Tab->TabId);
    } else if(Tab->ConnectionState == TabData::WaitingForNetworkEnable)
    {
        Tab->CurrentWebsocketActionId = 0;

        std::vector<std::shared_ptr<IDevToolsAction> > SavedActions;

        if(ConnectionState != WaitingFirstTab)
        {
            //Creating new tab, need to execute actions
            SavedActions = ActionsSaver.GetActions(Tab->TargetType == TabData::FrameType);
        }

        for(std::shared_ptr<IDevToolsAction> Action : SavedActions)
        {
            Action->SetOverrideDefaultTabId(Tab->TabId);
        }

        Tab->ConnectionState = TabData::WaitingForExecutingSavedActions;
        Tab->SavedActions = SavedActions;
        if(!SavedActions.empty())
        {
            //Run action
            StartFirstSavedAction(Tab);
        }else
            ProcessTabConnection(Tab);

    }else if(Tab->ConnectionState == TabData::WaitingForExecutingSavedActions)
    {
        if(!Tab->SavedActions.empty())
        {
            Tab->SavedActions.erase(Tab->SavedActions.begin(), Tab->SavedActions.begin() + 1);
        }

        if(Tab->SavedActions.empty())
        {
            Tab->ConnectionState = TabData::Connected;
            Tab->CurrentWebsocketActionId = 0;

        } else
        {
            //Run action
            StartFirstSavedAction(Tab);
        }
    }

}

void DevToolsConnector::OnWebSocketMessage(std::string& Message)
{
    picojson::value AllValue;
    picojson::parse(AllValue,Message);
    picojson::object AllObject = AllValue.get<picojson::object>();
    int Id = 0;
    if(AllObject.count("id") > 0 && AllObject["id"].is<double>())
    {
        Id = AllObject["id"].get<double>();
    }

    if(Id > 0)
    {
        //Process cache requests
        if(GlobalState.CachedRequests.count(Id))
        {
            std::string RequestId = GlobalState.CachedRequests[Id];
            if(GlobalState.CachedData.count(RequestId))
            {
                std::shared_ptr<CachedItem> Item = GlobalState.CachedData[RequestId];

                if(AllObject["result"].is<picojson::object>())
                {
                    picojson::object Result = AllObject["result"].get<picojson::object>();

                    if(Result["base64Encoded"].is<bool>() && Result["body"].is<std::string>())
                    {
                        std::string Body = Result["body"].get<std::string>();

                        if(!Result["base64Encoded"].get<bool>())
                        {
                            Item->Body = base64_encode((unsigned char*)Body.data(), Body.size());
                        } else
                        {
                            Item->Body = Body;
                        }
                    }
                }
                Item->IsFinished = true;
                GlobalState.LoadedUrls[Item->Url] = Item->Status;
            }
            GlobalState.CachedRequests.erase(Id);
        }

        if (ConnectionState == WaitingForDownloadsEnable)
        {
            ConnectionState = WaitingForAutoconnectEnable;
            GlobalState.WebSocketClient->Send(std::string("{\"id\": 1,\"method\": \"Target.setDiscoverTargets\", \"params\": {\"discover\": true}}"));
            return;
        }

        //Autocunnect responce has been obtained, waiting to connect for at least one tab
        if (ConnectionState == WaitingForAutoconnectEnable && Id == 1)
        {
            ConnectionState = WaitingFirstTab;
            return;
        }

        //Connecting to tab
        for(auto const& Tab : GlobalState.Tabs)
        {
            if(Tab->CurrentWebsocketActionId == Id)
            {
                ProcessTabConnection(Tab);
                return;
            }
        }

        //Connecting to frame
        for(auto const& Frame : GlobalState.Frames)
        {
            if(Frame->CurrentWebsocketActionId == Id)
            {
                ProcessTabConnection(Frame);
                return;
            }
        }

        //Switching tab after closing current
        if(SwitchTabAfterCloseCurrentActionId == Id && GlobalState.SwitchToTabId.empty() && GlobalState.SwitchToTabFrameId.empty())
        {
            GlobalState.SwitchToTabId = SwitchTabAfterCloseCurrentTabId;
            GlobalState.SwitchToTabFrameId = SwitchTabAfterCloseCurrentFrameId;
            GlobalState.SwitchToTabResetSavedActions = true;

            return;
        }


        std::vector<std::shared_ptr<IDevToolsAction> > AllActions = GetAllActions();

        //Send received message to action which expects it
        for(std::shared_ptr<IDevToolsAction> Action: AllActions)
        {
            if(Action->GetState() == IDevToolsAction::Running && Action->GetId() == Id)
            {
                std::string Result;
                std::string Error;
                if(AllObject["result"].is<picojson::object>())
                {
                    picojson::object ResultObject = AllObject["result"].get<picojson::object>();
                    Result = picojson::value(ResultObject).serialize();
                }
                if(AllObject["error"].is<picojson::object>())
                {
                    picojson::object ErrorObject = AllObject["error"].get<picojson::object>();
                    Error = picojson::value(ErrorObject).serialize();
                }
                Action->OnWebSocketMessage(Result, Error);
                break;
            }
        }



        return;
    }

    //Process incoming events
    if(AllObject.count("method") > 0 && AllObject["method"].is<std::string>())
    {
        std::string Method = AllObject["method"].get<std::string>();

        if (Method == "Page.frameRequestedNavigation")
        {
            picojson::object ResultObject = AllObject["params"].get<picojson::object>();
            std::string Result = picojson::value(ResultObject).serialize();

            if (AllObject["params"].is<picojson::object>())
            {
                std::string Reason = Parser.GetStringFromJson(Result, "reason");
                std::string Url = Parser.GetStringFromJson(Result, "url");
                std::string Disposition = Parser.GetStringFromJson(Result, "disposition");

                if(Reason == "anchorClick" && Disposition != "currentTab")
                {
                    ParseNewTabReferrer(Url);
                }
            }
        }

        if (Method == "Page.windowOpen")
        {
            picojson::object ResultObject = AllObject["params"].get<picojson::object>();
            std::string Result = picojson::value(ResultObject).serialize();

            if (AllObject["params"].is<picojson::object>())
            {
                std::string Url = Parser.GetStringFromJson(Result, "url");

                GlobalState.WindowOpenNewTabUrl = Url;

                std::string WindowName = Parser.GetStringFromJson(Result, "windowName");
                ParseNewTabReferrer(WindowName);
            }
        }


        if (Method == "Page.downloadWillBegin")
        {
            for (auto f : OnNativeDialog)
                f("download");

            if(GlobalState.IsDownloadsAllowed)
            {
                picojson::object ResultObject = AllObject["params"].get<picojson::object>();
                std::string Result = picojson::value(ResultObject).serialize();
                std::string FileName = Parser.GetStringFromJson(Result, "guid");


                for (auto f : OnDownloadStarted)
                    f(GetRelativePathToParentFolder(s2ws(FileName)));
            }
        }


        if (Method == "Page.downloadProgress")
        {
            picojson::object ResultObject = AllObject["params"].get<picojson::object>();
            std::string Result = picojson::value(ResultObject).serialize();

            if (AllObject["params"].is<picojson::object>())
            {
                std::string State = Parser.GetStringFromJson(Result, "state");
                if(State == "inProgress")
                {
                    GlobalState.IsDownloading = true;
                    GlobalState.CurrentDownloadFileName = Parser.GetStringFromJson(Result, "guid");
                }else if(State == "completed")
                {
                    GlobalState.IsDownloading = false;
                    GlobalState.CurrentDownloadFileName = Parser.GetStringFromJson(Result, "guid");
                }else if(State == "canceled")
                {
                    GlobalState.IsDownloading = false;
                    GlobalState.CurrentDownloadFileName.clear();
                }
            }
        }



        if(Method == "Page.javascriptDialogOpening")
        {
            if (AllObject["params"].is<picojson::object>())
            {
                std::string DialogType;

                if(AllObject["params"].contains("type") && AllObject["params"].get("type").is<std::string>())
                {
                    DialogType = AllObject["params"].get("type").get<std::string>();
                }

                std::shared_ptr<IDevToolsAction> NewAction;
                std::map<std::string, Variant> Params;

                NewAction.reset(ActionsFactory.Create("DialogResult", &GlobalState));

                Params["type"] = Variant(DialogType);

                std::string TabId = Parser.GetStringFromJson(Message, "sessionId");
                if(!TabId.empty())
                    Params["session"] = Variant(TabId);

                NewAction->SetTimeout(-1);
                NewAction->SetParams(Params);

                InsertAction(NewAction);

                for (auto f : OnNativeDialog)
                    f(DialogType);
            }
        }

        if(Method == "Page.fileChooserOpened")
        {
            if (AllObject["params"].is<picojson::object>())
            {
                if(AllObject["params"].contains("backendNodeId") && AllObject["params"].get("backendNodeId").is<double>())
                {
                    double BackendNodeId = AllObject["params"].get("backendNodeId").get<double>();
                    std::string TargetId;

                    if(AllObject["params"].contains("frameId"))
                    {
                        std::string FrameId = AllObject["params"].get("frameId").get<std::string>();
                        for(std::shared_ptr<TabData> TabInfo : GlobalState.Frames)
                        {
                            if(FrameId == TabInfo->FrameId)
                            {
                                TargetId = TabInfo->TabId;
                                break;
                            }
                        }
                    }


                    bool IsMultiple = false;

                    if(AllObject["params"].contains("mode") && AllObject["params"].get("mode").is<std::string>())
                    {
                        std::string Mode = AllObject["params"].get("mode").get<std::string>();
                        if(Mode == "selectMultiple")
                        {
                            IsMultiple = true;
                        }
                    }

                    for (auto f : OnNativeDialog)
                        f("upload");

                    if(GlobalState.OpenFileDialogIsManual)
                    {
                        OPENFILENAME ofn = {0};
                        TCHAR szFile[4096]={0};
                        ofn.lStructSize = sizeof(ofn);
                        ofn.hwndOwner = NULL;
                        ofn.lpstrFile = szFile;
                        ofn.nMaxFile = sizeof(szFile);
                        ofn.lpstrFilter = L"All\0*.*\0";
                        ofn.nFilterIndex = 1;
                        ofn.lpstrFileTitle = NULL;
                        ofn.nMaxFileTitle = 0;
                        ofn.lpstrInitialDir = NULL;
                        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

                        if(IsMultiple)
                            ofn.Flags |= OFN_ALLOWMULTISELECT;

                        if(GetOpenFileName(&ofn) == TRUE)
                        {
                            std::vector<Variant> Files;

                            if(IsMultiple)
                            {
                                wchar_t* Pointer = ofn.lpstrFile;
                                std::wstring Folder = Pointer;
                                Pointer += ( Folder.length() + 1 );
                                int FileNumber = 0;
                                while ( *Pointer )
                                {
                                  std::wstring Filename = Pointer;
                                  Pointer += ( Filename.length() + 1 );
                                  Filename = Folder + L"/" + Filename;

                                  Files.push_back(ws2s(Filename));
                                  FileNumber++;
                                }
                                if(!FileNumber)
                                {
                                    Files.push_back(ws2s(Folder));
                                }
                            }else
                            {
                                std::wstring ResultFileWstring(ofn.lpstrFile);
                                std::string ResultFile = ws2s(ResultFileWstring);
                                Files.push_back(Variant(ResultFile));
                            }

                            std::map<std::string, Variant> CurrentParams;

                            CurrentParams["backendNodeId"] = Variant(BackendNodeId);
                            CurrentParams["files"] = Variant(Files);

                            if(TargetId.empty())
                            {
                                SendWebSocket("DOM.setFileInputFiles", CurrentParams, GlobalState.TabId);
                            }else
                            {
                                SendWebSocket("DOM.setFileInputFiles", CurrentParams, TargetId);
                            }

                        }
                        return;
                    }

                    std::shared_ptr<IDevToolsAction> NewAction;
                    std::map<std::string, Variant> Params;

                    NewAction.reset(ActionsFactory.Create("OpenFile", &GlobalState));

                    Params["node_id"] = Variant(BackendNodeId);
                    Params["is_multiple"] = Variant(IsMultiple);
                    Params["target_id"] = Variant(TargetId);

                    NewAction->SetTimeout(-1);
                    NewAction->SetParams(Params);

                    InsertAction(NewAction);


                }
            }
        }

        if(Method == "Target.targetInfoChanged")
        {
            //Check if tab url has been changed

            if(AllObject["params"].is<picojson::object>())
            {
                picojson::object ResultObject = AllObject["params"].get<picojson::object>();
                std::string Result = picojson::value(ResultObject).serialize();
                std::string FrameId = Parser.GetStringFromJson(Result, "targetInfo.targetId");

                bool IsFrame = false;
                //Check if frame
                for(auto it = GlobalState.Frames.begin(); it != GlobalState.Frames.end(); )
                {
                    if((*it)->FrameId == FrameId)
                    {
                        IsFrame = true;
                        break;
                    } else
                    {
                        ++it;
                    }
                }

                if(!IsFrame)
                {

                    std::string NewUrl = Parser.GetStringFromJson(Result, "targetInfo.url");
                    bool Attached = Parser.GetBooleanFromJson(Result, "targetInfo.attached", false);

                    bool IsChanged = false;

                    //During chrome extensions initial load real url is replaced with not_exists_url.html#real_url. Real url will be loaded after initialization.
                    if(NewUrl.find("chrome-extension://") != std::string::npos && NewUrl.find("not_exists_url.html#") != std::string::npos && !Attached)
                    {
                        //Extension popup is created
                        std::shared_ptr<TabData> TabInfo = std::make_shared<TabData>();
                        TabInfo->ConnectionState = TabData::NotStarted;
                        TabInfo->FrameId = FrameId;
                        TabInfo->IsWaitingForFirstUrl = true;
                        //Obtain real url
                        TabInfo->FirstUrl = ReplaceAll(NewUrl, "not_exists_url.html#", std::string());
                        GlobalState.Tabs.push_back(TabInfo);
                        ProcessTabConnection(TabInfo);
                    }


                    for(std::shared_ptr<TabData> TabInfo : GlobalState.Tabs)
                    {
                        if(FrameId == TabInfo->FrameId)
                        {
                            //If using tabs.create from extension, real url will be replaced with about:blank#replaceurlreal_url. Real url will be loaded after initialization.
                            if(TabInfo->IsWaitingForFirstUrl && TabInfo->FirstUrl.empty() && NewUrl.find("about:blank#replaceurl") != std::string::npos)
                            {
                                //Obtain real url
                                TabInfo->FirstUrl = ReplaceAll(NewUrl, "about:blank#replaceurl", std::string());
                                CheckIfTabsNeedsToLoadFirstUrl(TabInfo);
                            }

                            if(TabInfo->CurrentUrl != NewUrl && GlobalState.TabId == TabInfo->TabId)
                                IsChanged = true;
                            TabInfo->CurrentUrl = NewUrl;
                        }
                    }

                    if(IsChanged)
                    {
                        for(auto f:OnAddressChanged)
                            f(NewUrl);
                    }
                }

            }

        }

        if(Method == "Page.frameStartedLoading")
        {
            //Page loading has been started
            if(AllObject["params"].is<picojson::object>())
            {
                picojson::object ResultObject = AllObject["params"].get<picojson::object>();
                std::string Result = picojson::value(ResultObject).serialize();

                std::string FrameId = Parser.GetStringFromJson(Result, "frameId");

                for(std::shared_ptr<TabData> TabInfo : GlobalState.Tabs)
                {
                    if(FrameId == TabInfo->FrameId)
                    {
                        for(auto f:OnLoadStart)
                            f();
                        TabInfo->IsLoading = true;
                    }
                }

            }
        }
        if(Method == "Page.frameStoppedLoading")
        {
            //Page loading has been started
            if(AllObject["params"].is<picojson::object>())
            {
                picojson::object ResultObject = AllObject["params"].get<picojson::object>();
                std::string Result = picojson::value(ResultObject).serialize();

                std::string FrameId = Parser.GetStringFromJson(Result, "frameId");

                for(std::shared_ptr<TabData> TabInfo : GlobalState.Tabs)
                {
                    if(FrameId == TabInfo->FrameId)
                    {
                        for(auto f:OnLoadStop)
                            f();
                        TabInfo->IsLoading = false;
                    }
                }

            }
        }
        if(Method == "Input.dragIntercepted")
        {
            //Request started
            if(AllObject["params"].is<picojson::object>())
            {
                picojson::object ResultObject = AllObject["params"].get<picojson::object>();
                if(ResultObject["data"].is<picojson::object>())
                {
                    picojson::object DragDataObject = ResultObject["data"].get<picojson::object>();

                    std::string DragData = picojson::value(DragDataObject).serialize();


                    OnDragIntercepted(DragData);
                }
            }
        }
        if(Method == "Network.requestWillBeSent")
        {
            //Request started
            if(AllObject["params"].is<picojson::object>())
            {
                std::string TabId = GlobalState.TabId;
                if(AllObject.count("sessionId") > 0)
                {
                    TabId = AllObject["sessionId"].get<std::string>();
                }

                picojson::object ResultObject = AllObject["params"].get<picojson::object>();
                std::string Result = picojson::value(ResultObject).serialize();

                OnNetworkRequestWillBeSent(Result, TabId);

                std::string RequestId = Parser.GetStringFromJson(Result, "requestId");
                for(auto f:OnRequestStart)
                    f(RequestId);
            }
        }
        if(Method == "Network.requestWillBeSentExtraInfo")
        {
            //Additional info
            if(AllObject["params"].is<picojson::object>())
            {
                picojson::object ResultObject = AllObject["params"].get<picojson::object>();
                std::string Result = picojson::value(ResultObject).serialize();

                for(auto f:OnRequestDataAdditional)
                    f(Result);
            }
        }
        if(Method == "Network.loadingFinished" || Method == "Network.loadingFailed" || Method == "Network.requestServedFromCache" || Method == "Network.responseReceived")
        {
            //Request ended
            if(AllObject["params"].is<picojson::object>())
            {
                picojson::object ResultObject = AllObject["params"].get<picojson::object>();
                std::string Result = picojson::value(ResultObject).serialize();
                std::string RequestId = Parser.GetStringFromJson(Result, "requestId");

                if(Method == "Network.responseReceived")
                {
                    OnNetworkResponseReceived(Result);
                }

                if(Method == "Network.loadingFinished")
                {
                    OnNetworkLoadingCompleted(Result, false);
                }

                if(Method == "Network.loadingFailed")
                {
                    OnNetworkLoadingCompleted(Result, true);
                }

                if(Method == "Network.responseReceived")
                {
                    std::string Url = Parser.GetStringFromJson(Result, "response.url");
                    double Status = Parser.GetFloatFromJson(Result, "response.status");
                    if(GlobalState.CachedData.count(RequestId) == 0 || GlobalState.CachedData[RequestId]->IsFinished)
                        GlobalState.LoadedUrls[Url] = Status;
                }

                for(auto f:OnRequestStop)
                    f(RequestId);
            }
        }

        //Intercept screencast event, replace it with OnPaint event
        if(Method == "Page.screencastFrame")
        {
            int NewScrollX = Parser.GetFloatFromJson(Message,"params.metadata.scrollOffsetX");
            int NewScrollY = Parser.GetFloatFromJson(Message,"params.metadata.scrollOffsetY");

            LastMetadataPaintWidth = Parser.GetFloatFromJson(Message,"params.metadata.deviceWidth");
            LastMetadataPaintHeight = Parser.GetFloatFromJson(Message,"params.metadata.deviceHeight");

            if(GlobalState.ScrollX != NewScrollX || GlobalState.ScrollY != NewScrollY)
            {
                GlobalState.ScrollX = NewScrollX;
                GlobalState.ScrollY = NewScrollY;
                for(auto f:OnScroll)
                    f();
            }
        }else if(Method == "Fetch.requestPaused")
        {
            if (AllObject["params"].is<picojson::object>())
            {
                //This event is generated by using Fetch.enable
                picojson::object ResultObject = AllObject["params"].get<picojson::object>();
                std::string Result = picojson::value(ResultObject).serialize();
                OnFetchRequestPaused(Result);
            }
        }else if(Method == "Fetch.authRequired")
        {
            if (AllObject["params"].is<picojson::object>())
            {
                //Http auth, this event is generated by using Fetch.enable
                picojson::object ResultObject = AllObject["params"].get<picojson::object>();
                std::string Result = picojson::value(ResultObject).serialize();
                OnFetchAuthRequired(Result);
            }
        } else if(Method == "Target.targetCreated")
        {
            //New tab has been created, attach to it
            if(AllObject["params"].is<picojson::object>())
            {
                picojson::object ResultObject = AllObject["params"].get<picojson::object>();
                std::string Result = picojson::value(ResultObject).serialize();

                std::string FrameId = Parser.GetStringFromJson(Result, "targetInfo.targetId");
                std::string TypeName = Parser.GetStringFromJson(Result, "targetInfo.type");
                std::string Url = Parser.GetStringFromJson(Result, "targetInfo.url");
                if(Url.empty())
                    Url = GlobalState.WindowOpenNewTabUrl;

                if(TypeName == "iframe")
                {
                    //Initialize frame
                    std::shared_ptr<TabData> FrameInfo = std::make_shared<TabData>();
                    FrameInfo->TargetType = TabData::FrameType;
                    FrameInfo->ConnectionState = TabData::NotStarted;
                    FrameInfo->FrameId = FrameId;
                    GlobalState.Frames.push_back(FrameInfo);
                    ProcessTabConnection(FrameInfo);

                }else if(TypeName == "page")
                {
                    if(!GlobalState.IsPopupsAllowed && ConnectionState == Connected)
                    {
                        //Tab creation is not allowed, close it instantly
                        std::map<std::string, Variant> CurrentParams;
                        CurrentParams["targetId"] = Variant(FrameId);
                        SendWebSocket("Target.closeTarget", CurrentParams, std::string());
                    }else
                    {
                        if(Url == "about:blank#newtab")
                        {
                            //New tab is opened by using ctrl-click or window.open

                            //Initialize new tab first
                            std::shared_ptr<TabData> TabInfo = std::make_shared<TabData>();
                            TabInfo->ConnectionState = TabData::NotStarted;
                            TabInfo->FrameId = FrameId;
                            GlobalState.Tabs.push_back(TabInfo);
                            ProcessTabConnection(TabInfo);

                            //After this will be done, original url will be loaded
                            GlobalState.LoadNextTargetId = FrameId;
                            GlobalState.LoadNext = true;

                        }else if(starts_with(Url, "about:blank#replaceurl"))
                        {
                            //If tab created with tabs.create from extension, real url will be replaced with about:blank#replaceurlreal_url. Real url will be loaded after initialization.

                            if(ConnectionState != Connected)
                            {
                                //Not connected yet, don't open a new tab
                                std::map<std::string, Variant> CurrentParams;
                                CurrentParams["targetId"] = Variant(FrameId);
                                SendWebSocket("Target.closeTarget", CurrentParams, std::string());
                            }else
                            {
                                std::shared_ptr<TabData> TabInfo = std::make_shared<TabData>();
                                TabInfo->ConnectionState = TabData::NotStarted;
                                TabInfo->FrameId = FrameId;

                                //After initialization will be done, original url will be loaded
                                TabInfo->FirstUrl = ReplaceAll(Url, "about:blank#replaceurl", std::string());
                                TabInfo->IsWaitingForFirstUrl = true;

                                GlobalState.Tabs.push_back(TabInfo);
                                ProcessTabConnection(TabInfo);
                            }
                        }else if(Url != "chrome://newtab/" && Url != "about:blank")
                        {
                            //New tab is opened without non-empty URL, could be bug

                            std::map<std::string, Variant> CurrentParams;
                            CurrentParams["targetId"] = Variant(FrameId);
                            SendWebSocket("Target.closeTarget", CurrentParams, std::string());

                        }else
                        {
                            if(GlobalState.SwitchingToDelayedTabIndex >= 0 && GlobalState.SwitchingToDelayedTabIndex < GlobalState.Tabs.size() && GlobalState.Tabs[GlobalState.SwitchingToDelayedTabIndex]->ConnectionState == TabData::Delayed)
                            {
                                //Loading delayed tab
                                std::shared_ptr<TabData> TabInfo = GlobalState.Tabs[GlobalState.SwitchingToDelayedTabIndex];
                                TabInfo->ConnectionState = TabData::NotStarted;
                                TabInfo->FrameId = FrameId;
                                ProcessTabConnection(TabInfo);
                            }else
                            {
                                std::shared_ptr<TabData> TabInfo = std::make_shared<TabData>();
                                TabInfo->ConnectionState = TabData::NotStarted;
                                TabInfo->FrameId = FrameId;
                                GlobalState.Tabs.push_back(TabInfo);
                                ProcessTabConnection(TabInfo);
                            }

                            GlobalState.SwitchingToDelayedTabIndex = -1;
                        }
                    }
                }else if(TypeName == "background_page" || TypeName == "service_worker")
                {
                    //Background page for extension has been created, collect info
                    std::shared_ptr<ExtensionInfo> ExtInfo;
                    std::string Id = Parser.GetStringFromJson(Result, "targetInfo.url");
                    if(starts_with(Id,"chrome-extension://"))
                    {
                        ReplaceAllInPlace(Id,"chrome-extension://","");
                        if(Id.length() >= 32)
                        {
                            Id = Id.substr(0,32);

                            for(std::shared_ptr<ExtensionInfo> ExtInfoCurrent: GlobalState.ExtensionList)
                            {
                                if(Id == ExtInfoCurrent->Id)
                                {
                                    ExtInfo = ExtInfoCurrent;
                                    break;
                                }
                            }

                            bool NeedToAdd = false;
                            if(!ExtInfo)
                            {
                                ExtInfo = std::make_shared<ExtensionInfo>();
                                NeedToAdd = true;
                            }

                            ExtInfo->Id = Id;
                            ExtInfo->FrameId = FrameId;
                            ExtInfo->Name = Parser.GetStringFromJson(Result, "targetInfo.title");

                            if(NeedToAdd)
                                GlobalState.ExtensionList.push_back(ExtInfo);
                        }
                    }
                }
            }
        }else if(Method == "Target.attachedToTarget")
        {
            //New tab has been attached, start initialization of new tab
            if(AllObject["params"].is<picojson::object>())
            {
                picojson::object ResultObject = AllObject["params"].get<picojson::object>();
                std::string Result = picojson::value(ResultObject).serialize();

                std::string TabId = Parser.GetStringFromJson(Result, "sessionId");
                std::string FrameId = Parser.GetStringFromJson(Result, "targetInfo.targetId");

                for(std::shared_ptr<TabData> TabInfo : GlobalState.Tabs)
                {
                    if(FrameId == TabInfo->FrameId)
                    {
                        TabInfo->TabId = TabId;
                        GlobalState.SwitchToTabId = TabId;
                        GlobalState.SwitchToTabFrameId = FrameId;
                        GlobalState.SwitchToTabResetSavedActions = false;
                        ProcessTabConnection(TabInfo);
                    }
                }

                for(std::shared_ptr<TabData> TabInfo : GlobalState.Frames)
                {
                    if(FrameId == TabInfo->FrameId)
                    {
                        TabInfo->TabId = TabId;
                        ProcessTabConnection(TabInfo);
                    }
                }
            }
        } else if(Method == "Target.detachedFromTarget")
        {
            //Tab removed, need to update list and switch tab if current tab is closed
            if(AllObject["params"].is<picojson::object>())
            {
                picojson::object ResultObject = AllObject["params"].get<picojson::object>();
                std::string Result = picojson::value(ResultObject).serialize();
                std::string TabId = Parser.GetStringFromJson(Result, "sessionId");

                bool IsFrame = false;
                //Check if frame detached
                for(auto it = GlobalState.Frames.begin(); it != GlobalState.Frames.end(); )
                {
                    if((*it)->TabId == TabId)
                    {
                        it = GlobalState.Frames.erase(it);
                        IsFrame = true;
                    } else
                    {
                        ++it;
                    }
                }

                //Fire all events before closing tab
                std::vector<std::shared_ptr<IDevToolsAction> > AllActions = GetAllActions();
                for(std::shared_ptr<IDevToolsAction> Action: AllActions)
                {
                    std::vector<std::string> SubscribbedEvents = Action->GetSubscribbedEvents();
                    if(Action->GetState() == IDevToolsAction::Running && std::find(SubscribbedEvents.begin(), SubscribbedEvents.end(), Method) != SubscribbedEvents.end())
                    {
                        Action->OnWebSocketEvent(Method, Result);
                    }
                }

                if(!IsFrame)
                {
                    bool IsCurrentTabClosing = TabId == GlobalState.TabId;
                    std::string FirstTabId;
                    std::string FirstFrameId;
                    //Don't start to switch tab if already switching
                    bool EveryTabIsConnected = true;

                    for(auto it = GlobalState.Tabs.begin(); it != GlobalState.Tabs.end(); )
                    {
                        if((*it)->TabId == TabId)
                        {
                            it = GlobalState.Tabs.erase(it);
                        } else
                        {
                            if((*it)->ConnectionState != TabData::Connected && (*it)->ConnectionState != TabData::Delayed)
                            {
                                EveryTabIsConnected = false;
                            }
                            if(FirstTabId.empty())
                            {
                                FirstTabId = (*it)->TabId;
                                FirstFrameId = (*it)->FrameId;
                            }
                            ++it;
                        }
                    }

                    if(EveryTabIsConnected && IsCurrentTabClosing && !FirstTabId.empty() && !FirstFrameId.empty())
                    {
                        std::map<std::string, Variant> CurrentParams;
                        CurrentParams["targetId"] = Variant(FirstFrameId);
                        SwitchTabAfterCloseCurrentTabId = FirstTabId;
                        SwitchTabAfterCloseCurrentFrameId = FirstFrameId;
                        SwitchTabAfterCloseCurrentActionId = SendWebSocket("Target.activateTarget", CurrentParams, std::string());
                    }
                }

            }
        }
        else
        {
            if(AllObject["params"].is<picojson::object>())
            {
                picojson::object ResultObject = AllObject["params"].get<picojson::object>();
                std::string SessionId;
                if(AllObject.count("sessionId") > 0)
                {
                    SessionId = AllObject["sessionId"].get<std::string>();
                }

                std::string Result = picojson::value(ResultObject).serialize();

                //Save information about frames and context
                if (Method == "Runtime.executionContextCreated" && Parser.GetBooleanFromJson(Result, "context.auxData.isDefault", true))
                {
                    std::string FrameId = Parser.GetStringFromJson(Result, "context.auxData.frameId");

                    std::string ContextId = Parser.GetStringFromJson(Result, "context.uniqueId");

                    //WORKER_LOG("!!!!!!!!!!!!!!!!! CREATE ExecutionContexts id = " + FrameId + std::string(", context id = ") + std::string(ContextId) + std::string(", session id = ") + std::string(SessionId));

                    std::shared_ptr<ExecutionContextData> ExecutionContext = std::make_shared<ExecutionContextData>();
                    ExecutionContext->TabId = SessionId;
                    ExecutionContext->FrameId = FrameId;
                    ExecutionContext->ContextId = ContextId;

                    GlobalState.ExecutionContexts.push_back(ExecutionContext);
                }

        //Don't delete info about removed context
                /*if (Method == "Runtime.executionContextDestroyed")
                {
                    int ContextId = Parser.GetFloatFromJson(Result, "executionContextId");
                    for (auto it = GlobalState.FrameIdToContextId.cbegin(); it != GlobalState.FrameIdToContextId.cend(); )
                    {
                        if (it->second == ContextId)
                        {
                            GlobalState.FrameIdToContextId.erase(it++);
                        }
                        else
                        {
                            ++it;
                        }
                    }
                }*/

                //Send event information to subscribers of OnMessage

                for(auto f:OnMessage)
                    f(Method, Result);

                int PreviousScrollX = GlobalState.ScrollX;
                int PreviousScrollY = GlobalState.ScrollY;
                int PreviousWidth = GlobalState.Width;
                int PreviousHeight = GlobalState.Height;


                std::vector<std::shared_ptr<IDevToolsAction> > AllActions = GetAllActions();

                //Send event information to actions which expects them
                for(std::shared_ptr<IDevToolsAction> Action: AllActions)
                {
                    std::vector<std::string> SubscribbedEvents = Action->GetSubscribbedEvents();
                    if(Action->GetState() == IDevToolsAction::Running && std::find(SubscribbedEvents.begin(), SubscribbedEvents.end(), Method) != SubscribbedEvents.end())
                    {
                        Action->OnWebSocketEvent(Method, Result);
                    }
                }

                if(GlobalState.ScrollX != PreviousScrollX || GlobalState.ScrollY != PreviousScrollY)
                {
                    for(auto f:OnScroll)
                        f();
                }

                if(GlobalState.Width != PreviousWidth || GlobalState.Height != PreviousHeight)
                {
                    for(auto f:OnResize)
                        f();
                }
            }
        }
        return;
    }


}

void DevToolsConnector::PaintNotify()
{
    if(GlobalState.Width != PaintWidth || GlobalState.Height != PaintHeight)
    {
        GlobalState.Width = PaintWidth;
        GlobalState.Height = PaintHeight;
        for(auto f:OnResize)
            f();
    }

    for(auto f:OnPaint)
        f();
}


void DevToolsConnector::HandleIPCDataNoDeviceScale()
{
    bool IsNewImage = false;

    //Get data
    if(IPC->GetImageId())
    {
        SharedMemoryIPCLockGuard Lock(IPC);

        if(IPC->GetImageId())
        {
            ImageData.assign(IPC->GetImagePointer(),IPC->GetImagePointer() + IPC->GetImageSize());
            PaintWidth = IPC->GetImageWidth();
            PaintHeight = IPC->GetImageHeight();
            IPC->SetImageId(0);
            IsNewImage = true;
        }

    }

    if(IsNewImage)
    {
        HasScaledImage = false;
        ImageDataScaled.clear();

        PaintNotify();
    }
}

void DevToolsConnector::HandleIPCDataWithDeviceScale()
{
    //More complicated version of HandleIPCDataNoDeviceScale, which includes image scaling.
    //Target image size is LastMetadataPaintWidth * LastMetadataPaintHeight.
    //These values are obtained from Page.screencastFrame message.
    bool IsNewImage = false;

    //Get data
    if(IPC->GetImageId())
    {
        SharedMemoryIPCLockGuard Lock(IPC);

        if(IPC->GetImageId())
        {
            ImageDataScaled.assign(IPC->GetImagePointer(),IPC->GetImagePointer() + IPC->GetImageSize());
            PaintWidthScaled = IPC->GetImageWidth();
            PaintHeightScaled = IPC->GetImageHeight();
            IPC->SetImageId(0);
            IsNewImage = true;
        }
    }


    if(IsNewImage)
    {
        HasScaledImage = true;

        //Prepare data for scaled image
        ImageData.clear();
        PaintWidth = LastMetadataPaintWidth;
        PaintHeight = LastMetadataPaintHeight;
        ImageData.resize(PaintWidth * PaintHeight * 4, 255);

        //Scale image
        for(int j = 0;j<PaintHeight;j++)
        {
            for(int i = 0;i<PaintWidth;i++)
            {
                int ic = i;
                int jc = j;
                ic = ((float)i * (float)PaintWidthScaled) / ((float)PaintWidth);
                jc = ((float)j * (float)PaintHeightScaled) / ((float)PaintHeight);
                if(ic < 0)
                    ic = 0;
                if(ic >= PaintWidthScaled)
                    ic = PaintHeightScaled - 1;
                if(jc < 0)
                    jc = 0;
                if(jc >= PaintHeightScaled)
                    jc = PaintHeightScaled - 1;
                if(ic >= 0 && ic < PaintWidthScaled && jc >= 0 && jc < PaintHeightScaled)
                {
                    ImageData[i*4+j*PaintWidth*4 + 0] = ImageDataScaled[ic*4+jc*PaintWidthScaled*4 + 0];
                    ImageData[i*4+j*PaintWidth*4 + 1] = ImageDataScaled[ic*4+jc*PaintWidthScaled*4 + 1];
                    ImageData[i*4+j*PaintWidth*4 + 2] = ImageDataScaled[ic*4+jc*PaintWidthScaled*4 + 2];
                    ImageData[i*4+j*PaintWidth*4 + 3] = ImageDataScaled[ic*4+jc*PaintWidthScaled*4 + 3];
                }

            }
        }

        PaintNotify();
    }
}


void DevToolsConnector::HandleIPCData()
{
    if(GlobalState.IsDeviceScaleFactorModified)
    {
        HandleIPCDataWithDeviceScale();
    }else
    {
        HandleIPCDataNoDeviceScale();
    }
}

void DevToolsConnector::Timer()
{
    ProcessLauncher.Timer();

    TryToConnect();

    //Delete http client if needed
    if(GlobalState.HttpClient)
    {
        if(GlobalState.HttpClient->DeleteLater)
        {
            GlobalState.HttpClient.reset();
        }else
        {
            GlobalState.HttpClient->Timer();
        }

    }

    //Delete web socket if needed
    if(GlobalState.WebSocketClient)
    {
        if(GlobalState.WebSocketClient->DeleteLater)
        {
            GlobalState.WebSocketClient.reset();
        }else
        {
            GlobalState.WebSocketClient->Timer();
        }
    }

    //If at least one tab is connected, mark connection state as connected
    if(ConnectionState == WaitingFirstTab && !GlobalState.Tabs.empty())
    {
        for(auto const& Tab : GlobalState.Tabs)
        {
            if(Tab->ConnectionState == TabData::Connected)
            {
                ConnectionState = Connected;

                //If reset action waits for connected status, notify about result
                if(ResetResult)
                {
                    ResetResult->Success();
                    ResetResult.reset();
                    ResetMethodDeadline = 0;
                }
                break;
            }
        }
    }

    //Executing saved action when connecting to tab
    for(auto const& Tab : GlobalState.Tabs)
    {
        if(Tab->ConnectionState == TabData::WaitingForExecutingSavedActions && !Tab->SavedActions.empty() && (Tab->SavedActions[0]->GetState() == IDevToolsAction::Finished || Tab->SavedActions[0]->GetState() == IDevToolsAction::NotStarted))
        {
            ProcessTabConnection(Tab);
            return;
        }
    }

    //Tabs switching
    for(auto const& Tab : GlobalState.Tabs)
    {
        if(GlobalState.SwitchToTabId == Tab->TabId && !GlobalState.SwitchToTabId.empty())
        {
            if(GlobalState.SwitchToTabResetSavedActions && Tab->ConnectionState == TabData::Connected)
            {
                Tab->ConnectionState = TabData::WaitingForAttachment;
                Tab->IsSwitchingToTab = true;
                GlobalState.SwitchToTabResetSavedActions = false;
                ProcessTabConnection(Tab);
            }
            if(Tab->ConnectionState == TabData::Connected)
            {
                GlobalState.TabId = Tab->TabId;

                for(auto f:OnAddressChanged)
                    f(Tab->CurrentUrl);

                //Load first url if tab was open by using ctrl-click or window.open
                if(GlobalState.LoadNext)
                {
                    if(GlobalState.SwitchToTabFrameId == GlobalState.LoadNextTargetId)
                    {
                        std::map<std::string, Variant> CurrentParams;

                        CurrentParams["url"] = Variant(GlobalState.LoadNextUrl);
                        if(!GlobalState.LoadNextData.empty())
                        {
                            CurrentParams["referrer"] = Variant(GlobalState.LoadNextData);
                            CurrentParams["referrerPolicy"] = Variant(std::string("unsafeUrl"));
                        }

                        SendWebSocket("Page.navigate", CurrentParams, GlobalState.SwitchToTabId);
                    }
                    GlobalState.LoadNext = false;
                }

                CheckIfTabsNeedsToLoadFirstUrl(Tab);

                //Generate Target.targetActivated event. This event doesn't exist in Chrome Developer tools protocol.
                std::string TabId = GlobalState.SwitchToTabId;
                std::string FrameId = GlobalState.SwitchToTabFrameId;
                std::string Method = "Target.targetActivated";
                GlobalState.SwitchToTabId.clear();
                GlobalState.SwitchToTabFrameId.clear();

                int PreviousScrollX = GlobalState.ScrollX;
                int PreviousScrollY = GlobalState.ScrollY;
                int PreviousWidth = GlobalState.Width;
                int PreviousHeight = GlobalState.Height;

                for(std::shared_ptr<IDevToolsAction> Action : Actions)
                {
                    std::vector<std::string> SubscribbedEvents = Action->GetSubscribbedEvents();
                    if(Action->GetState() == IDevToolsAction::Running && std::find(SubscribbedEvents.begin(), SubscribbedEvents.end(), Method) != SubscribbedEvents.end())
                    {
                        std::map<std::string, Variant> Params;
                        Params["targetInfo.targetId"] = Variant(std::string(FrameId));
                        Params["targetInfo.sessionId"] = Variant(std::string(TabId));
                        std::string Result = Serializer.SerializeObjectToString(Params);
                        Action->OnWebSocketEvent(Method, Result);
                    }
                }

                if(GlobalState.ScrollX != PreviousScrollX || GlobalState.ScrollY != PreviousScrollY)
                {
                    for(auto f:OnScroll)
                        f();
                }

                if(GlobalState.Width != PreviousWidth || GlobalState.Height != PreviousHeight)
                {
                    for(auto f:OnResize)
                        f();
                }
            }
        }
    }

    long long Now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();


    if(IsInspectAtScheduled && Now > InspectAtTime)
    {
        InspectAtFinalize();
    }

    //Process timeout for reset acion
    if(ResetResult)
    {
        if(ResetMethodDeadline > 0 && Now > ResetMethodDeadline)
        {
            ResetResult->Fail("Timeout", "Timeout");
            ResetResult.reset();
            ResetMethodDeadline = 0;
        }
    }

    if(ConnectionState == Connected)
    {
        HandleIPCData();

        int PreviousScrollX = GlobalState.ScrollX;
        int PreviousScrollY = GlobalState.ScrollY;
        int PreviousWidth = GlobalState.Width;
        int PreviousHeight = GlobalState.Height;

        std::vector<std::shared_ptr<IDevToolsAction> >::iterator it = Actions.begin();
        while(it != Actions.end())
        {
            std::shared_ptr<IDevToolsAction> Action = *it;

            bool DeleteThisAction = false;

            if(Action->GetState() == IDevToolsAction::NotStarted)
            {
                //Check pending actions, start if possible
                bool CanRun = false;
                if(Action->UsesHttpClient())
                {
                    if(GlobalState.HttpClient)
                    {
                        CanRun = false;
                    }else
                    {
                        CanRun = true;
                        CurrentHttpClientActionId = Action->GetId();
                        GlobalState.HttpClient.reset(this->SimpleHttpClientFactory->Create());
                        GlobalState.HttpClient->OnResult.push_back(std::bind(&DevToolsConnector::OnHttpClientResult,this,_1,_2,_3));
                    }
                }else
                {
                    bool HasRunningActionWithSameGroupId = false;
                    bool HasRunningScreenCastAction = false;

                    for(std::shared_ptr<IDevToolsAction> CurrentAction : Actions)
                    {
                        if(CurrentAction->GetTypeName() == "StartScreencast" && CurrentAction->GetState() == IDevToolsAction::Running)
                        {
                            HasRunningScreenCastAction = true;
                            break;
                        }
                    }

                    for(std::shared_ptr<IDevToolsAction> CurrentAction : Actions)
                    {
                        if(CurrentAction->GetGroupId() == Action->GetGroupId() && CurrentAction->GetId() != Action->GetId() && CurrentAction->GetState() == IDevToolsAction::Running)
                        {
                            HasRunningActionWithSameGroupId = true;
                            break;
                        }
                    }

                    CanRun = !HasRunningActionWithSameGroupId && !HasRunningScreenCastAction;
                }

                if(CanRun)
                {
                    Action->SetTimeStarted(Now);
                    if(Action->GetTimeout() >= 0)
                    {
                        Action->SetDeadline(Action->GetTimeStarted() + Action->GetTimeout());
                    }else
                    {
                        Action->SetDeadline(0);
                    }
                    if(Action->GetRunnningForAllRunActiveTab())
                    {
                        Action->OnRunnningForAllRunActiveTab();
                    } else
                    {
                        Action->Run();
                    }
                }
            }else if(Action->GetState() == IDevToolsAction::Finished)
            {
                //Clear finished acitons
                if(Action->UsesHttpClient())
                {
                    if(GlobalState.HttpClient)
                        GlobalState.HttpClient->DeleteLater = true;
                    CurrentHttpClientActionId = 0;
                }

                DeleteThisAction = true;
            }else if(Action->GetState() == IDevToolsAction::Running)
            {
                //Stop action if timeout
                if(Action->GetDeadline() > 0 && Now > Action->GetDeadline())
                {
                    DeleteThisAction = true;
                    Action->GetResult()->Fail("Timeout", "Timeout");
                }

                //Send timer event
                if(!DeleteThisAction)
                {
                    std::vector<std::string> SubscribbedEvents = Action->GetSubscribbedEvents();
                    if(std::find(SubscribbedEvents.begin(), SubscribbedEvents.end(), "Timer") != SubscribbedEvents.end())
                    {
                        Action->OnWebSocketEvent("Timer", std::string());
                    }
                }
            }

            if(DeleteThisAction)
            {
                it = Actions.erase(it);
            }else
            {
                ++it;
            }
        }

        // Process saved actions
        for(auto const& Tab : GlobalState.Tabs)
        {
            if(Tab->ConnectionState == TabData::WaitingForExecutingSavedActions)
            {
                std::vector<std::shared_ptr<IDevToolsAction> >::iterator it = Tab->SavedActions.begin();
                while(it != Tab->SavedActions.end())
                {
                    std::shared_ptr<IDevToolsAction> Action = *it;

                    bool DeleteThisAction = false;

                    if(Action->GetState() == IDevToolsAction::Running)
                    {
                        //Stop action if timeout
                        if(Action->GetDeadline() > 0 && Now > Action->GetDeadline())
                        {
                            DeleteThisAction = true;
                            Action->GetResult()->Fail("Timeout", "Timeout");
                        }
                    }

                    if(DeleteThisAction)
                    {
                        Action->SetState(IDevToolsAction::Finished);
                    }
                    ++it;

                }
            }
        }

        if(GlobalState.ScrollX != PreviousScrollX || GlobalState.ScrollY != PreviousScrollY)
        {
            for(auto f:OnScroll)
                f();
        }

        if(GlobalState.Width != PreviousWidth || GlobalState.Height != PreviousHeight)
        {
            for(auto f:OnResize)
                f();
        }

    }
}

int DevToolsConnector::GenerateId()
{
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0xF, 0x7FFFFFFF);
    return distribution(generator);
}

std::vector<std::shared_ptr<IDevToolsAction> > DevToolsConnector::GetAllActions()
{
    std::vector<std::shared_ptr<IDevToolsAction> > AllActions = Actions;

    //Add saved actions
    for(auto const& Tab : GlobalState.Tabs)
    {
        if(Tab->ConnectionState == TabData::WaitingForExecutingSavedActions)
        {
            for(std::shared_ptr<IDevToolsAction> Action : Tab->SavedActions)
            {
                AllActions.push_back(Action);
            }
        }
    }

    return AllActions;
}

void DevToolsConnector::InsertAction(std::shared_ptr<IDevToolsAction> Action)
{
    ActionsSaver.ProcessAction(Action);

    //Run this action for each connected tab
    if(Action->IsNeedToRunForAllActiveTabs())
    {
        for(auto const& Tab : GlobalState.Tabs)
        {
            if(Tab->ConnectionState == TabData::Connected && Tab->TabId != GlobalState.TabId)
            {
                std::shared_ptr<IDevToolsAction> NewAction = ActionsSaver.CloneAction(Action);
                NewAction->SetOverrideDefaultTabId(Tab->TabId);
                NewAction->SetRunnningForAllRunActiveTab(true);
                NewAction->SetGroupId(Action->GetGroupId());
                Actions.push_back(NewAction);
            }
        }
    }

    Actions.push_back(Action);
}

std::string DevToolsConnector::GenerateMessage(const std::string& Method, const std::map<std::string, Variant>& Params, const std::string& SessionId, int &Id)
{
    picojson::value::object Data;
    picojson::value::object ParamsObject = Serializer.SerializeObjectToObject(Params);

    Id = GenerateId();

    Data["id"] = picojson::value((double)Id);
    Data["method"] = picojson::value(Method);
    Data["params"] = picojson::value(ParamsObject);

    if(!SessionId.empty())
    {
        Data["sessionId"] = picojson::value(SessionId);
    }

    return picojson::value(Data).serialize();
}

int DevToolsConnector::SendWebSocket(const std::string& Method, const std::map<std::string, Variant>& Params, const std::string& SessionId)
{
    if(!GlobalState.WebSocketClient)
        return -1;

    int Id = 0;

    std::string DataString = GenerateMessage(Method, Params, SessionId, Id);

    GlobalState.WebSocketClient->Send(DataString);

    return Id;
}

Async DevToolsConnector::GetTabsList(int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction(ActionsFactory.Create("GetTabsList", &GlobalState));
    InsertAction(NewAction);
    NewAction->SetTimeout(Timeout);
    return NewAction->GetResult();
}

Async DevToolsConnector::GetCurrentUrl(int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction(ActionsFactory.Create("GetCurrentUrl", &GlobalState));
    InsertAction(NewAction);
    NewAction->SetTimeout(Timeout);
    return NewAction->GetResult();
}

int DevToolsConnector::GetTabNumber()
{
    int Result = 0;
    for(auto const& Tab : GlobalState.Tabs)
    {
        if(Tab->ConnectionState == TabData::Connected || Tab->ConnectionState == TabData::Delayed)
        {
            Result++;
        }
    }
    return Result;
}

int DevToolsConnector::GetCurrentTabIndex()
{
    int Result = 0;
    for(auto const& Tab : GlobalState.Tabs)
    {
        if(Tab->ConnectionState == TabData::Connected || Tab->ConnectionState == TabData::Delayed)
        {
            if(Tab->TabId == GlobalState.TabId)
            {
                return Result;
            }
            Result++;
        }
    }
    return -1;
}

Async DevToolsConnector::GetBrowserSize(int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction(ActionsFactory.Create("GetBrowserSize", &GlobalState));
    InsertAction(NewAction);
    NewAction->SetTimeout(Timeout);
    return NewAction->GetResult();
}

Async DevToolsConnector::ResizeBrowser(int Width, int Height, int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction(ActionsFactory.Create("ResizeBrowserWithCorrection", &GlobalState));
    std::map<std::string, Variant> Params;
    Params["bounds.width"] = Variant(Width);
    Params["bounds.height"] = Variant(Height);
    NewAction->SetParams(Params);
    InsertAction(NewAction);
    NewAction->SetTimeout(Timeout);
    return NewAction->GetResult();
}

Async DevToolsConnector::ResetDeviceScaleFactor(float DeviceScaleFactor, int Timeout)
{
    GlobalState.IsDeviceScaleFactorModified = DeviceScaleFactor >= 1.01 || DeviceScaleFactor <= 0.99;

    std::shared_ptr<IDevToolsAction> NewAction;
    NewAction.reset(ActionsFactory.Create("ResetDeviceScaleFactor", &GlobalState));
    NewAction->SetTimeout(Timeout);
    InsertAction(NewAction);
    return NewAction->GetResult();
}

Async DevToolsConnector::Screenshot(int X, int Y, int Width, int Height, int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction(ActionsFactory.CreateWebsocketQuery("Page.captureScreenshot", &GlobalState,"data",DevToolsActionWebsocketQuery::String));
    std::map<std::string, Variant> Params;
    Params["clip.x"] = Variant(X);
    Params["clip.y"] = Variant(Y);
    Params["clip.width"] = Variant(Width);
    Params["clip.height"] = Variant(Height);
    Params["format"] = Variant(std::string("png"));
    Params["fromSurface"] = Variant(true);
    Params["clip.scale"] = Variant(1);
    NewAction->SetParams(Params);
    InsertAction(NewAction);
    NewAction->SetTimeout(Timeout);
    return NewAction->GetResult();
}

void DevToolsConnector::EnableBackgroundMode()
{
    SetMinCapturePeriod(400);
}

void DevToolsConnector::DisableBackgroundMode()
{
    SetMinCapturePeriod(35);
}

void DevToolsConnector::SetMinCapturePeriod(int MinCapturePeriod)
{
    GlobalState.SaveProxy->SetMinCapturePeriod(MinCapturePeriod);
}

Async DevToolsConnector::StartScreenCast(int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction;

    NewAction.reset(ActionsFactory.Create("StartScreencast", &GlobalState));

    NewAction->SetTimeout(Timeout);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

Async DevToolsConnector::StopScreenCast(int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction;

    NewAction.reset(ActionsFactory.Create("StopScreencast", &GlobalState));

    NewAction->SetTimeout(Timeout);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

Async DevToolsConnector::Load(const std::string& Url, bool IsInstant, const std::string& Referrer, int Timeout)
{
    if(!GlobalState.IsProxySet && Url != "data:text/plain," /* This url is used when apply fingerprint in record mode */)
    {
        ResetProxy(GlobalState.ParentProcessId);
        GlobalState.IsProxySet = true;
    }

    std::shared_ptr<IDevToolsAction> NewAction;
    std::map<std::string, Variant> Params;

    NewAction.reset(ActionsFactory.Create("Load", &GlobalState));

    Params["instant"] = Variant(IsInstant);

    Params["url"] = Variant(std::string(Url));
    if(!Referrer.empty())
    {
        Params["referrer"] = Variant(std::string(Referrer));
        Params["referrerPolicy"] = Variant(std::string("unsafeUrl"));
    }

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

Async DevToolsConnector::Reload(bool IsInstant, int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction;
    std::map<std::string, Variant> Params;

    NewAction.reset(ActionsFactory.Create("Reload", &GlobalState));

    Params["instant"] = Variant(IsInstant);

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

Async DevToolsConnector::SetProxy(const std::string Server, int Port, bool IsHttp, const std::string Login, const std::string Password, int Timeout)
{
    GlobalState.IsProxySet = true;

    std::shared_ptr<IDevToolsAction> NewAction;
    std::map<std::string, Variant> Params;

    NewAction.reset(ActionsFactory.Create("SetProxy", &GlobalState));

    Params["server"] = Variant(Server);
    Params["port"] = Variant(Port);
    Params["is_http"] = Variant(IsHttp);
    Params["login"] = Variant(Login);
    Params["password"] = Variant(Password);

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

Async DevToolsConnector::GetHistory(int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction;
    std::map<std::string, Variant> Params;

    NewAction.reset(ActionsFactory.Create("GetHistory", &GlobalState));

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

Async DevToolsConnector::Sleep(int TimeMilliseconds, int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction;
    std::map<std::string, Variant> Params;
    Params["time"] = Variant(TimeMilliseconds);

    NewAction.reset(ActionsFactory.Create("Sleep", &GlobalState));

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}


Async DevToolsConnector::SetHeaders(const std::vector<std::pair<std::string, std::string>>& Headers, int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction;

    NewAction.reset(ActionsFactory.Create("SetHeaders", &GlobalState));

    std::map<std::string, Variant> Params;
    std::map<std::string, Variant> Object;

    for (const auto& Header : Headers)
    {
        if(Header.first != "Accept-Language" && Header.first != "User-Agent")
            Object[Header.first] = Variant(Header.second);
    }

    Params["headers"] = Variant(Object);

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

Async DevToolsConnector::SetUserAgentData(const std::string& Data, const std::vector<std::pair<std::string, std::string>>& Headers, int Timeout)
{
    GlobalState.UserAgentData = Data;

    if(GlobalState.IsUserAgentChanged)
    {
        return SetHeaders(Headers, Timeout);
    }

    Async Result = std::make_shared<AsyncResult>();
    Result->Success();
    return Result;
}



Async DevToolsConnector::NavigateBack(bool IsInstant, int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction;
    std::map<std::string, Variant> Params;

    NewAction.reset(ActionsFactory.Create("NavigateBack", &GlobalState));

    Params["instant"] = Variant(IsInstant);

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}


Async DevToolsConnector::NavigateForward(bool IsInstant, int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction;
    std::map<std::string, Variant> Params;

    NewAction.reset(ActionsFactory.Create("NavigateForward", &GlobalState));

    Params["instant"] = Variant(IsInstant);

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

Async DevToolsConnector::CreateTab(const std::string& Url, bool IsInstant, bool IsDelayed, const std::string& Referrer, int Timeout)
{
    if(!GlobalState.IsProxySet && Url != "data:text/plain," /* This url is used when apply fingerprint in record mode */)
    {
        ResetProxy(GlobalState.ParentProcessId);
        GlobalState.IsProxySet = true;
    }

    std::shared_ptr<IDevToolsAction> NewAction;
    std::map<std::string, Variant> Params;

    NewAction.reset(ActionsFactory.Create("CreateTab", &GlobalState));

    //Group id is set in order to avoid running several tab action simultaneously
    NewAction->SetGroupId(1);

    Params["instant"] = Variant(IsInstant);

    Params["url"] = Variant(std::string(Url));

    Params["referrer"] = Variant(std::string(Referrer));

    Params["delayed"] = Variant(IsDelayed);

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

Async DevToolsConnector::SwitchToTab(int Index, int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction;
    std::map<std::string, Variant> Params;

    bool IsDelayed = false;
    std::string DelayedUrl;
    int CurrentIndex = 0;

    //Check if switching to delayed tab
    for(std::shared_ptr<TabData> Tab : GlobalState.Tabs)
    {
        if(Tab->ConnectionState == TabData::Connected || Tab->ConnectionState == TabData::Delayed)
        {
            if(CurrentIndex == Index)
            {
                IsDelayed = Tab->ConnectionState == TabData::Delayed;
                if(IsDelayed)
                {
                    DelayedUrl = Tab->DelayedUrl;
                }
                break;
            }
            CurrentIndex++;
        }
    }

    if(IsDelayed)
    {
        GlobalState.SwitchingToDelayedTabIndex = CurrentIndex;

        NewAction.reset(ActionsFactory.Create("CreateTab", &GlobalState));

        //Group id is set in order to avoid running several tab action simultaneously
        NewAction->SetGroupId(1);

        Params["instant"] = Variant(true);

        Params["url"] = Variant(DelayedUrl);

        Params["referrer"] = Variant(std::string());

        Params["delayed"] = Variant(false);
    }else
    {
        NewAction.reset(ActionsFactory.Create("SwitchToTab", &GlobalState));

        //Group id is set in order to avoid running several tab action simultaneously
        NewAction->SetGroupId(1);

        Params["index"] = Variant(Index);
    }

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

Async DevToolsConnector::CloseTab(int Index, int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction;
    std::map<std::string, Variant> Params;

    NewAction.reset(ActionsFactory.Create("CloseTab", &GlobalState));

    //Group id is set in order to avoid running several tab action simultaneously
    NewAction->SetGroupId(1);

    Params["index"] = Variant(Index);

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

void DevToolsConnector::CloseBrowser()
{
    std::map<std::string, Variant> Params;
    SendWebSocket("Browser.close",Params,std::string());
}

Async DevToolsConnector::SetStartupScript(const std::string& Script, int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction;
    std::map<std::string, Variant> Params;

    NewAction.reset(ActionsFactory.Create("SetStartupScript", &GlobalState));

    Params["source"] = Variant(Script);

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}
Async DevToolsConnector::ExecuteJavascript(const std::string& Script, const std::string& Variables, const std::string& ElementPath, bool ScrollToElement, int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction;
    std::map<std::string, Variant> Params;

    NewAction.reset(ActionsFactory.Create("ExecuteJavascript", &GlobalState));

    Params["expression"] = Variant(Script);
    Params["path"] = Variant(ElementPath);
    Params["variables"] = Variant(Variables);
    Params["do_scroll"] = Variant(ScrollToElement);

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

Async DevToolsConnector::Inspect(int X, int Y, int Position, int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction;
    std::map<std::string, Variant> Params;

    NewAction.reset(ActionsFactory.Create("Inspect", &GlobalState));

    Params["x"] = Variant(X);
    Params["y"] = Variant(Y);
    Params["position"] = Variant(Position);

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

Async DevToolsConnector::SetRequestsRestrictions(const std::vector<std::pair<bool, std::string> >& Rules, int Timeout)
{
    GlobalState.BlockRequests.clear();
    for(const std::pair<bool, std::string>& Rule:Rules)
    {
        RequestRestriction RuleNative;
        RuleNative.IsAllow = Rule.first;
        RuleNative.Mask = Rule.second;
        GlobalState.BlockRequests.push_back(RuleNative);
    }

    std::shared_ptr<IDevToolsAction> NewAction;

    NewAction.reset(ActionsFactory.Create("SetRequestsRestrictions", &GlobalState));

    NewAction->SetTimeout(Timeout);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

Async DevToolsConnector::SetHttpAuth(const std::string& UserName, const std::string& Password, int Timeout)
{
    GlobalState.HttpAuthEnabled = true;
    GlobalState.HttpAuthUserName = UserName;
    GlobalState.HttpAuthPassword = Password;

    std::shared_ptr<IDevToolsAction> NewAction;

    NewAction.reset(ActionsFactory.Create("SetRequestsRestrictions", &GlobalState));

    NewAction->SetTimeout(Timeout);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

void DevToolsConnector::SetCacheMasks(const std::vector<std::pair<bool, std::string> >& Rules)
{
    GlobalState.CacheCapture.clear();

    for (const auto& Rule : Rules)
    {
        RequestRestriction RuleNative;
        RuleNative.IsAllow = Rule.first;
        RuleNative.Mask = Rule.second;
        GlobalState.CacheCapture.push_back(RuleNative);
    }
}

std::string DevToolsConnector::GetAllCacheData(const std::string& Mask)
{
    picojson::array Items;

    for (const auto& data : GlobalState.CachedData)
    {
        if (match(Mask, data.second->Url))
        {
            Items.push_back(data.second->Serialize());
        }
    }

    return picojson::value(Items).serialize();
}

std::string DevToolsConnector::GetSingleCacheData(const std::string& Mask, bool IsBase64)
{
    std::string Result;
    long long LastTime = 0;
    for (const auto& data : GlobalState.CachedData)
    {
        if(data.second->CreatedAt > LastTime && match(Mask, data.second->Url))
        {
            LastTime = data.second->CreatedAt;
            if(IsBase64)
            {
                Result = data.second->Body;
            }
            else
            {
                Result = base64_decode(data.second->Body);
            }
        }
    }

    return Result;
}

void DevToolsConnector::ClearNetworkData()
{
    GlobalState.CachedData.clear();
    GlobalState.CachedRequests.clear();
    GlobalState.LoadedUrls.clear();
}

void DevToolsConnector::OnFetchAuthRequired(std::string& Result)
{
    //Send auth response
    std::string RequestId = Parser.GetStringFromJson(Result, "requestId");

    std::map<std::string, Variant> Params;
    Params["requestId"] = Variant(RequestId);

    std::map<std::string, Variant> Auth;
    Auth["username"] = Variant(GlobalState.HttpAuthUserName);
    Auth["password"] = Variant(GlobalState.HttpAuthPassword);
    Auth["response"] = Variant(std::string("ProvideCredentials"));
    Params["authChallengeResponse"] = Variant(Auth);

    SendWebSocket("Fetch.continueWithAuth", Params, GlobalState.TabId);

    //Disable http auth

    GlobalState.HttpAuthUserName.clear();
    GlobalState.HttpAuthPassword.clear();
    GlobalState.HttpAuthEnabled = false;

    std::shared_ptr<IDevToolsAction> NewAction;

    NewAction.reset(ActionsFactory.Create("SetRequestsRestrictions", &GlobalState));

    NewAction->SetTimeout(-1);

    InsertAction(NewAction);
}

void DevToolsConnector::OnFetchRequestPaused(std::string& Result)
{
    std::string RequestUrl = Parser.GetStringFromJson(Result, "request.url");
    std::string RequestId = Parser.GetStringFromJson(Result, "requestId");

    bool Allow = true;

    for (const auto& Rule : GlobalState.BlockRequests)
    {
        if (match(Rule.Mask, RequestUrl))
        {
            Allow = Rule.IsAllow;
        }
    }

    std::map<std::string, Variant> Params = { {"requestId", Variant(RequestId)} };

    if (Allow)
    {
        SendWebSocket("Fetch.continueRequest", Params, GlobalState.TabId);
    }
    else
    {
        Params["errorReason"] = Variant(std::string("Failed"));
        SendWebSocket("Fetch.failRequest", Params, GlobalState.TabId);
    }

}

void DevToolsConnector::OnDragIntercepted(std::string& DragData)
{
    GlobalState.DragAndDropIsEnabled = true;
    GlobalState.DragAndDropData = DragData;

    Drag(DragEventEnter,GlobalState.CursorX,GlobalState.CursorY);
}

void DevToolsConnector::OnNetworkRequestWillBeSent(std::string& Result, std::string& TabId)
{
    std::string RequestId = Parser.GetStringFromJson(Result, "requestId");
    std::string Url = Parser.GetStringFromJson(Result, "request.url");

    if (Url.find("data:") == std::string::npos)
    {
        for(auto f:OnRequestDataMain)
            f(Result);

        bool Allow = false;

        for (const auto& CacheMask : GlobalState.CacheCapture)
        {
            if (match(CacheMask.Mask, Url))
            {
                Allow = CacheMask.IsAllow;
            }
        }

        if (Allow)
        {
            std::shared_ptr<CachedItem> Item = GlobalState.CachedData.count(RequestId) ? GlobalState.CachedData[RequestId] : std::make_shared<CachedItem>();
            Item->UpdateRequestHeaders(Result, "request", "headers");
            Item->IsFinished = false;
            Item->IsError = false;
            Item->Url = Url;
            Item->TabId = TabId;

            std::string PostData = Parser.GetStringFromJson(Result, "request.postData");
            Item->PostData = base64_encode((unsigned char *)PostData.data(), PostData.size());

            GlobalState.CachedData[RequestId] = Item;
        }
    }
}

void DevToolsConnector::OnNetworkResponseReceived(std::string& Result)
{
    std::string RequestId = Parser.GetStringFromJson(Result, "requestId");
    double Status = Parser.GetFloatFromJson(Result, "response.status");

    if (GlobalState.CachedData.count(RequestId))
    {
        std::shared_ptr<CachedItem> Item = GlobalState.CachedData[RequestId];

        Item->UpdateRequestHeaders(Result, "response", "requestHeaders");

        Item->UpdateResponseHeaders(Result, "response", "headers");

        //We can't retrieve content for redirected requests
        if (Status >= 300 && Status <= 399)
        {
            Item->IsFinished = true;
        }

        Item->Status = Status;
    }

}

void DevToolsConnector::OnNetworkLoadingCompleted(std::string& Result, bool HasError)
{
    std::string RequestId = Parser.GetStringFromJson(Result, "requestId");

    if (GlobalState.CachedData.count(RequestId))
    {
        std::shared_ptr<CachedItem> Item = GlobalState.CachedData[RequestId];
        Item->IsFinished = HasError;
        Item->IsError = HasError;

        if (!HasError)
        {
            //Need to get the response body using a separate method.
            std::map<std::string, Variant> Params = { {"requestId", Variant(RequestId)} };
            int Id = SendWebSocket("Network.getResponseBody", Params, Item->TabId);
            GlobalState.CachedRequests[Id] = RequestId;
        }

        Item->Error = HasError ? Parser.GetStringFromJson(Result, "errorText") : std::string();
    }
}


int DevToolsConnector::GetStatusForURL(const std::string& UrlPattern)
{
    for (const auto& urlPair : GlobalState.LoadedUrls)
    {
        if (match(UrlPattern, urlPair.first))
        {
            return urlPair.second;
        }
    }

    return 0;
}

std::string DevToolsConnector::FindLoadedURL(const std::string& UrlPattern)
{
    for (const auto& urlPair : GlobalState.LoadedUrls)
    {
        if (match(UrlPattern, urlPair.first))
        {
            return urlPair.first;
        }
    }
    return std::string();
}

bool DevToolsConnector::IsURLLoaded(const std::string& UrlPattern)
{
    for (const auto& urlPair : GlobalState.LoadedUrls)
    {
        if (match(UrlPattern, urlPair.first))
        {
            return true;
        }
    }

    return false;
}

void DevToolsConnector::Focus()
{
    std::map<std::string, Variant> Params;
    SendWebSocket("Page.bringToFront", Params, GlobalState.TabId);
}

std::vector<std::pair<std::string, std::string> > DevToolsConnector::GetExtensionList()
{
    std::vector<std::pair<std::string, std::string> > Res;
    for(std::shared_ptr<ExtensionInfo> ExtInfo : GlobalState.ExtensionList)
    {
        //Disable CryptoTokenExtension, Google Docs Offline, Google Network Speech, Chrome Web Store Payments, Google Hangouts
        if(ExtInfo->Id != "kmendfapggjehodndflmmgagdbamhnfd" &&
           ExtInfo->Id != "ghbmnnjooekpmoecnnnilnnbdlolhkhi" &&
           ExtInfo->Id != "neajdppkdcdipfabeoofebfddakdcjhd" &&
           ExtInfo->Id != "nmmhkkegccagdldgiimedpiccmgmieda" &&
           ExtInfo->Id != "nkeimhogjdpnpccoofpliimaahmaaome")
        {
            std::pair<std::string, std::string> ExtensionPair;
            ExtensionPair.first = ExtInfo->Id;
            ExtensionPair.second = ExtInfo->Name;
            Res.push_back(ExtensionPair);
        }
    }
    return Res;
}


void DevToolsConnector::TriggerExtensionButton(const std::string ExtensionIdOrNamePart)
{
    std::string Id;
    for(std::shared_ptr<ExtensionInfo> ExtInfo : GlobalState.ExtensionList)
    {
        if(
            ExtInfo->Id.find(ExtensionIdOrNamePart) != std::string::npos ||
            ExtInfo->Name.find(ExtensionIdOrNamePart) != std::string::npos
         )
        {
            Id = ExtInfo->Id;
        }
    }

    if(Id.empty())
    {
        Id = ExtensionIdOrNamePart;
    }

    GlobalState.SaveProxy->TriggerExtensionButton(Id);
}

Async DevToolsConnector::StartDragFile(const std::string& Path, int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction;

    NewAction.reset(ActionsFactory.Create("StartDragFile", &GlobalState));

    std::map<std::string, Variant> Params;

    Params["filename"] = Variant(Path);

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

void DevToolsConnector::Drag(DragEvent Event, int X, int Y, int KeyboardPresses)
{
    if(IsInspectAtScheduled)
        return;

    std::map<std::string, Variant> Params;
    std::string TypeName;

    if(Event == DragEventEnter)
    {
        TypeName = "dragEnter";
    } else if(Event == DragEventOver)
    {
        TypeName = "dragOver";
    }else if(Event == DragEventDrop)
    {
        TypeName = "drop";
    }else if(Event == DragEventCancel)
    {
        TypeName = "dragCancel";
    }

    Params["type"] = Variant(TypeName);
    Params["x"] = Variant(X);
    Params["y"] = Variant(Y);
    Params["modifiers"] = Variant(KeyboardPresses);
    Params["data"] = Variant(std::string("BAS_DRAG_DATA"));

    int Id = 0;

    std::string DataString = GenerateMessage("Input.dispatchDragEvent", Params, GlobalState.TabId, Id);

    ReplaceAllInPlace(DataString, "\"BAS_DRAG_DATA\"", GlobalState.DragAndDropData);

    GlobalState.WebSocketClient->Send(DataString);

}


void DevToolsConnector::Mouse(MouseEvent Event, int X, int Y, MouseButton Button, int MousePressed, int KeyboardPresses, int ClickCount)
{
    if(IsInspectAtScheduled)
        return;

    GlobalState.CursorX = X;
    GlobalState.CursorY = Y;

    if(GlobalState.DragAndDropIsEnabled)
    {
        if(Event == MouseEventUp || Event == MouseEventDown)
        {
            Drag(DragEventDrop,X,Y);
            GlobalState.DragAndDropIsEnabled = false;
            GlobalState.DragAndDropData.clear();
        } else if(Event == MouseEventMove)
        {
            Drag(DragEventOver,X,Y);
        }
        return;
    }



    std::map<std::string, Variant> Params;
    std::string TypeName = "mousePressed";
    if(Event == MouseEventUp)
    {
        TypeName = "mouseReleased";
    } else if(Event == MouseEventMove)
    {
        TypeName = "mouseMoved";
    }

    std::string ButtonName = "left";
    if(Button == MouseButtonRight)
    {
        ButtonName = "right";
    } else if(Button == MouseButtonMiddle)
    {
        ButtonName = "middle";
    }else if(Button == MouseButtonNone)
    {
        ButtonName = "none";
    }
    Params["type"] = Variant(TypeName);
    Params["x"] = Variant(X);
    Params["y"] = Variant(Y);
    Params["modifiers"] = Variant(KeyboardPresses);
    Params["buttons"] = Variant(MousePressed);
    Params["button"] = Variant(ButtonName);
    Params["clickCount"] = Variant(ClickCount);

    SendWebSocket("Input.dispatchMouseEvent", Params, GlobalState.TabId);
}

void DevToolsConnector::Wheel(int X, int Y, bool IsUp, int Delta, int MousePressed, int KeyboardPresses)
{
    if(IsInspectAtScheduled)
        return;

    GlobalState.CursorX = X;
    GlobalState.CursorY = Y;

    std::map<std::string, Variant> Params;
    std::string TypeName = "mouseWheel";
    std::string ButtonName = "none";
    Params["type"] = Variant(TypeName);
    Params["x"] = Variant(X);
    Params["y"] = Variant(Y);
    Params["modifiers"] = Variant(KeyboardPresses);
    Params["buttons"] = Variant(MousePressed);
    Params["button"] = Variant(ButtonName);
    Params["clickCount"] = Variant(0);
    Params["deltaY"] = Variant(Delta * ((IsUp) ? -1 : 1));
    Params["deltaX"] = Variant(0);
    SendWebSocket("Input.dispatchMouseEvent", Params, GlobalState.TabId);
}

Async DevToolsConnector::Touch(TouchEvent Event, int X, int Y, int Id, double RadiusX, double RadiusY, double RotationAngle, double Pressure, int Timeout)
{
    if(IsInspectAtScheduled)
    {
        return std::make_shared<AsyncResult>();
    }

    GlobalState.CursorX = X;
    GlobalState.CursorY = Y;

    if(GlobalState.DragAndDropIsEnabled)
    {
        if(Event == MouseEventUp || Event == MouseEventDown)
        {
            Drag(DragEventDrop,X,Y);
            GlobalState.DragAndDropIsEnabled = false;
            GlobalState.DragAndDropData.clear();
        } else if(Event == MouseEventMove)
        {
            Drag(DragEventOver,X,Y);
        }
        return std::make_shared<AsyncResult>();
    }

    std::map<std::string, Variant> Params;
    std::map<std::string, Variant> Point;
    std::string TypeName = "touchStart";
    if(Event == TouchEventUp)
    {
        TypeName = "touchEnd";
    } else if(Event == TouchEventMove)
    {
        TypeName = "touchMove";
    }

    Params["type"] = Variant(TypeName);
    Point["x"] = Variant(X);
    Point["y"] = Variant(Y);

    Point["radiusX"] = Variant(RadiusX);
    Point["radiusY"] = Variant(RadiusY);
    Point["rotationAngle"] = Variant(RotationAngle);
    Point["force"] = Variant(Pressure);
    Point["id"] = Variant(Id);


    std::vector<Variant> Points;
    Points.push_back(Variant(Point));
    Params["touchPoints"] = Variant(Points);

    std::shared_ptr<IDevToolsAction> NewAction;
    NewAction.reset(ActionsFactory.Create("Touch", &GlobalState));

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

void DevToolsConnector::Key(KeyEvent Event, const std::string& Char, int KeyboardPresses)
{
    if(IsInspectAtScheduled)
        return;

    if(EmulateKeyboard.IsKeyboardCharacter(Char))
    {
        std::map<std::string, Variant> Params = EmulateKeyboard.PrepareKeyboardEvent(Event, Char, KeyboardPresses);
        SendWebSocket("Input.dispatchKeyEvent", Params, GlobalState.TabId);
    } else
    {
        if(Event == KeyEventCharacter)
        {
            std::map<std::string, Variant> Params = EmulateKeyboard.PrepareSpecialCharacterEvent(Char);
            SendWebSocket("Input.insertText", Params, GlobalState.TabId);
        }
    }
}

void DevToolsConnector::KeyRaw(KeyEvent Event, WPARAM WindowsVirtualKeyCode, LPARAM NativeVirtualKeyCode, int KeyboardPresses)
{
    if(IsInspectAtScheduled)
        return;

    std::map<std::string, Variant> Params = EmulateKeyboard.PrepareRawKeyboardEvent(Event, WindowsVirtualKeyCode, NativeVirtualKeyCode, KeyboardPresses);
    SendWebSocket("Input.dispatchKeyEvent", Params, GlobalState.TabId);
}

bool DevToolsConnector::IsLoading()
{
    for(auto const& Tab : GlobalState.Tabs)
    {
        if(Tab->ConnectionState == TabData::Connected)
        {
            if(Tab->TabId == GlobalState.TabId)
            {
                return Tab->IsLoading;
            }
        }
    }
    return false;
}

Async DevToolsConnector::Reset(int Timeout)
{
    //Stop all actions
    for(std::shared_ptr<IDevToolsAction> Action : Actions)
    {
        Action->GetResult()->Fail("Action is stopped because of reset", "Reset");
    }
    Actions.clear();

    //If waiting for other reset action - stop it also
    if(ResetResult)
    {
        ResetResult->Fail("Action is stopped because of reset", "Reset");
        ResetResult.reset();
        ResetMethodDeadline = 0;
    }

    //Reset global state
    GlobalState.Reset();

    //Don't start to connect to port until browser will close
    ConnectionState = WaitingForBrowserClose;

    //Set deadline
    if(Timeout >= 0)
    {
        long long Now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        ResetMethodDeadline = Now + Timeout;
    } else
    {
        ResetMethodDeadline = 0;
    }

    //Send command to close browse
    std::map<std::string, Variant> Params;
    SendWebSocket("Browser.close", Params, std::string());

    //Return result
    ResetResult = std::make_shared<AsyncResult>();
    return ResetResult;
}

bool DevToolsConnector::InterruptAction(int ActionUniqueId)
{
    bool IsInterrupted = false;
    std::vector<std::shared_ptr<IDevToolsAction> >::iterator it = Actions.begin();
    while(it != Actions.end())
    {
        std::shared_ptr<IDevToolsAction> Action = *it;

        bool DeleteThisAction = false;

        if(Action->GetUniqueId() == ActionUniqueId && Action->GetState() != IDevToolsAction::Finished)
        {
            IsInterrupted = true;
            DeleteThisAction = true;
            Action->GetResult()->Interrupt();
        }

        if(DeleteThisAction)
        {
            it = Actions.erase(it);
        } else
        {
            ++it;
        }
    }
    return IsInterrupted;

}

void DevToolsConnector::SetOpenFileDialogResult(const std::string& Result)
{
    this->GlobalState.OpenFileDialogResult = Result;
}

void DevToolsConnector::SetOpenFileDialogManualMode(bool IsManual)
{
    this->GlobalState.OpenFileDialogIsManual = IsManual;
}

void DevToolsConnector::SetPromptResult(const std::string& PromptResult)
{
    this->GlobalState.PromptResult = PromptResult;
}

Async DevToolsConnector::AllowDownloads(int Timeout)
{
    GlobalState.IsDownloadsAllowed = true;
    std::shared_ptr<IDevToolsAction> NewAction;
    NewAction.reset(ActionsFactory.CreateWebsocketQuery("Browser.setDownloadBehavior",&GlobalState,std::string(),DevToolsActionWebsocketQuery::None));
    std::map<std::string, Variant> Params;

    Params["downloadPath"] = Variant(ws2s(GetRelativePathToParentFolder(L"")));
    Params["behavior"] = Variant(std::string("allowAndName"));

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}


Async DevToolsConnector::RestrictDownloads(int Timeout)
{
    GlobalState.IsDownloadsAllowed = false;
    std::shared_ptr<IDevToolsAction> NewAction;
    NewAction.reset(ActionsFactory.CreateWebsocketQuery("Browser.setDownloadBehavior",&GlobalState,std::string(),DevToolsActionWebsocketQuery::None));
    std::map<std::string, Variant> Params = NewAction->GetParams();

    Params["behavior"] = Variant(std::string("deny"));

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

bool DevToolsConnector::IsFileDownloadReady()
{
    return !GlobalState.IsDownloading && !GlobalState.CurrentDownloadFileName.empty();
}

std::string DevToolsConnector::GetDownloadedFilePath()
{
    std::string Res;
    if(IsFileDownloadReady())
    {
        Res = GlobalState.CurrentDownloadFileName;
        GlobalState.CurrentDownloadFileName.clear();
        GlobalState.IsDownloading = false;
    }
    return Res;
}


void DevToolsConnector::RestrictPopups()
{
    GlobalState.IsPopupsAllowed = false;
}

void DevToolsConnector::AllowPopups()
{
    GlobalState.IsPopupsAllowed = true;
}

void DevToolsConnector::CheckIfTabsNeedsToLoadFirstUrl(std::shared_ptr<TabData> Tab)
{
    if(Tab->IsWaitingForFirstUrl && !Tab->FirstUrl.empty() && Tab->ConnectionState == TabData::Connected)
    {
        std::map<std::string, Variant> CurrentParams;
        CurrentParams["url"] = Variant(Tab->FirstUrl);
        SendWebSocket("Page.navigate", CurrentParams, GlobalState.SwitchToTabId);

        Tab->IsWaitingForFirstUrl = false;
        Tab->FirstUrl.clear();
    }
}


void DevToolsConnector::ParseNewTabReferrer(const std::string& NewTabReferrer)
{
    std::string Prefix("https://request.bablosoft.com/#");
    if(starts_with(NewTabReferrer,Prefix))
    {
        GlobalState.LoadNextData = NewTabReferrer;
        std::string DataString = NewTabReferrer;
        DataString = DataString.erase(0,Prefix.size());
        DataString = base64_decode(DataString);
        GlobalState.LoadNextUrl = Parser.GetStringFromJson(DataString,"url");
    }else
    {
        GlobalState.LoadNextData.clear();
        GlobalState.LoadNextUrl.clear();
    }
}

Async DevToolsConnector::RestoreCookies(const std::string& Cookies, int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction;
    NewAction.reset(ActionsFactory.Create("RestoreCookies", &GlobalState));

    std::map<std::string, Variant> Params;
    Params["cookies"] = Variant(Cookies);

    NewAction->SetTimeout(Timeout);
    NewAction->SetParams(Params);

    InsertAction(NewAction);
    return NewAction->GetResult();
}

Async DevToolsConnector::SaveCookies(int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction;
    NewAction.reset(ActionsFactory.Create("SaveCookies", &GlobalState));

    NewAction->SetTimeout(Timeout);

    InsertAction(NewAction);
    return NewAction->GetResult();
}
