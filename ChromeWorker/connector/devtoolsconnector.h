#ifndef DEVTOOLSCONNECTOR_H
#define DEVTOOLSCONNECTOR_H

#include "idevtoolsconnector.h"
#include "IWebSocketClientFactory.h"
#include "ISimpleHttpClientFactory.h"
#include <map>
#include <memory>
#include "idevtoolsaction.h"
#include "devtoolsactionfactory.h"
#include "ActionSaver.h"
#include "JsonParser.h"
#include "JsonSerializer.h"
#include "InputEventsEnumerations.h"
#include "KeyboardEmulation.h"
#include <windows.h>
#include "sharedmemoryipc.h"
#include "chromeprocesslauncher.h"

class DevToolsConnector : public IDevToolsConnector
{
    KeyboardEmulation EmulateKeyboard;

    std::shared_ptr<ISimpleHttpClientFactory> SimpleHttpClientFactory;
    std::shared_ptr<IWebSocketClientFactory> WebSocketClientFactory;
    DevToolsGlobalState GlobalState;

    JsonParser Parser;
    JsonSerializer Serializer;

    DevToolsActionFactory ActionsFactory;
    ActionSaver ActionsSaver;

    std::vector<std::shared_ptr<IDevToolsAction> > Actions;
    int CurrentHttpClientActionId = 0;

    std::wstring ProfilePath;
    std::vector<std::pair<std::string,std::string> > CommandLineAdditional;
    ChromeProcessLauncher ProcessLauncher;

    ///Extensions
    std::vector<std::wstring> Extensions;
    std::vector<std::wstring> DefaultExtensions;
    /*std::vector<std::wstring> OptionalExtensions;*/

    //Switch tab after close
    int SwitchTabAfterCloseCurrentActionId = 0;
    std::string SwitchTabAfterCloseCurrentTabId;
    std::string SwitchTabAfterCloseCurrentFrameId;

    //Connection settings

    bool IsConnectionOrLaunch = true;
    int TargetPort = -1;

    //Connection data

    std::string Endpoint;

    bool WasBrowserCreationEvent = false;

    //Connection methods

    void TryToConnect();
    void OnHttpClientResult(bool IsSuccess,int StatusCode,std::string& Data);
    void OnBrowserEndpointObtained(bool IsSuccess,int StatusCode,std::string& Data);
    void OnWebSocketConnected(bool IsSuccess);
    void OnWebSocketDisconnected();
    void OnWebSocketMessage(std::string& Message);
    void ProcessTabConnection(std::shared_ptr<TabData> Tab);
    void StartFirstSavedAction(std::shared_ptr<TabData> Tab);

    //InspectAt

    bool IsInspectAtScheduled = false;
    long long InspectAtTime = 0;
    int InspectAtX = 0;
    int InspectAtY = 0;
    void InspectAtFinalize();

    //Helpers

    int GenerateId();
    std::string GenerateMessage(const std::string& Method, const std::map<std::string, Variant>& Params, const std::string& SessionId, int &Id);
    int SendWebSocket(const std::string& Method, const std::map<std::string, Variant>& Params, const std::string& SessionId);
    void InsertAction(std::shared_ptr<IDevToolsAction> Action);
    std::vector<std::shared_ptr<IDevToolsAction> > GetAllActions();
    void OpenDevToolsInternal(bool IsInspect);

    //Callbacks

    void OnFetchRequestPaused(std::string& Result);
    void OnFetchAuthRequired(std::string& Result);
    void OnNetworkRequestWillBeSent(std::string& Result, std::string& TabId);
    void OnDragIntercepted(std::string& DragData);
    void OnNetworkResponseReceived(std::string& Result);
    void OnNetworkLoadingCompleted(std::string& Result, bool HasError);



    //Reset method
    Async ResetResult;
    long long ResetMethodDeadline = 0;

    //Paint data
    std::vector<char> ImageData;
    int PaintWidth = 0;
    int PaintHeight = 0;
    int LastMetadataPaintWidth = 0;
    int LastMetadataPaintHeight = 0;

    //Store scaled image due to _set_device_scale_factor usage
    bool HasScaledImage = false;
    std::vector<char> ImageDataScaled;
    int PaintWidthScaled = 0;
    int PaintHeightScaled = 0;

    SharedMemoryIPC* IPC = 0;
    void HandleIPCData();
    void HandleIPCDataNoDeviceScale();
    void HandleIPCDataWithDeviceScale();
    void PaintNotify();
    void ParseNewTabReferrer(const std::string& NewTabReferrer);
    void CheckIfTabsNeedsToLoadFirstUrl(std::shared_ptr<TabData> Tab);
    void ResetProxy(const std::string& ParentProcessId);
    void SetInitialProxy(const std::string& InitalProxy);
    //https://source.chromium.org/chromium/chromium/src/+/master:content/browser/devtools/devtools_video_consumer.cc;drc=267e9d603200302cd937cc5b788f044186a1b8c6;l=25
    void SetMinCapturePeriod(int MinCapturePeriod);

    //Inputs private
    //Drag is used only with Mouse method
    void Drag(DragEvent Event, int X, int Y, int KeyboardPresses = KeyboardModifiersNone);

    public:

        char* GetPaintData();
        int GetPaintWidth();
        int GetPaintHeight();

        char* GetPaintDataScaled();
        int GetPaintWidthScaled();
        int GetPaintHeightScaled();
        int GetWidth();
        int GetHeight();
        int GetScrollX();
        int GetScrollY();

        void Timer();

        void Initialize
        (
                std::shared_ptr<ISimpleHttpClientFactory> SimpleHttpClientFactory,
                std::shared_ptr<IWebSocketClientFactory> WebSocketClientFactory,
                int Port, const std::string& UniqueProcessId, const std::string& ParentProcessId,
                const std::string& ConstantStartupScript,
                const std::vector<std::pair<std::string,std::string> >& CommandLineAdditional,
                const std::string& InitalProxy, int PcapPort
        );
        void SetProfilePath(const std::wstring& Path);
        void SetExtensionList(const std::vector<std::wstring>& Extensions);
        bool InterruptAction(int ActionUniqueId);
        void StartProcess();
        void OpenDevTools();
        void InspectAt(int X, int Y);

        std::wstring GetProxyConfigFolder();

        //Browser life cycle
        Async Reset(int Timeout = -1);
        bool IsLoading();
        void CloseBrowser();

        //Tabs
        Async GetTabsList(int Timeout = -1);
        Async CreateTab(const std::string& Url, bool IsInstant = false, bool IsDelayed = false, const std::string& Referrer = std::string(), int Timeout = -1);
        Async SwitchToTab(int Index, int Timeout = -1);
        Async CloseTab(int Index, int Timeout = -1);
        int GetTabNumber();
        int GetCurrentTabIndex();

        //Rendering
        Async Screenshot(int X, int Y, int Width, int Height, int Timeout = -1);
        Async StartScreenCast(int Timeout = -1);
        Async StopScreenCast(int Timeout = -1);
        void EnableBackgroundMode();
        void DisableBackgroundMode();
        Async ResetDeviceScaleFactor(float DeviceScaleFactor, int Timeout = -1);

        //Network
        Async Load(const std::string& Url, bool IsInstant = false, const std::string& Referrer = std::string(), int Timeout = -1);
        Async Reload(bool IsInstant = false, int Timeout = -1);
        Async NavigateBack(bool IsInstant = false, int Timeout = -1);
        Async NavigateForward(bool IsInstant = false, int Timeout = -1);
        Async GetCurrentUrl(int Timeout = -1);
        Async SetRequestsRestrictions(const std::vector<std::pair<bool, std::string> >& Rules, int Timeout = -1);
        Async SetHttpAuth(const std::string& UserName, const std::string& Password, int Timeout = -1);


        //Cache
        void SetCacheMasks(const std::vector<std::pair<bool, std::string> >& Rules);
        std::string GetSingleCacheData(const std::string& Mask, bool IsBase64);
        std::string GetAllCacheData(const std::string& Mask);
        void ClearNetworkData();

        int GetStatusForURL(const std::string& UrlPattern);
        std::string FindLoadedURL(const std::string& UrlPattern);
        bool IsURLLoaded(const std::string& UrlPattern);
        Async SetProxy(const std::string Server, int Port, bool IsHttp = true, const std::string Login = std::string(), const std::string Password = std::string(), int Timeout = -1);
        Async GetHistory(int Timeout = -1);
        Async SetHeaders(const std::vector<std::pair<std::string, std::string>>& Headers, int Timeout = -1);
        Async SetUserAgentData(const std::string& Data, const std::vector<std::pair<std::string, std::string>>& Headers, int Timeout = -1);

        //Size
        Async GetBrowserSize(int Timeout = -1);
        Async ResizeBrowser(int Width, int Height, int Timeout = -1);

        //Javascript
        Async SetStartupScript(const std::string& Script, int Timeout = -1);
        Async ExecuteJavascript(const std::string& Script, const std::string& Variables = std::string(), const std::string& ElementPath = std::string(), bool ScrollToElement = false, int Timeout = -1);

        //Inputs
        void Mouse(MouseEvent Event, int X, int Y, MouseButton Button = MouseButtonLeft, int MousePressed = MouseButtonNone, int KeyboardPresses = KeyboardModifiersNone, int ClickCount = 1);
        void Wheel(int X, int Y, bool IsUp, int Delta = 100, int MousePressed = MouseButtonNone, int KeyboardPresses = KeyboardModifiersNone);
        Async Touch(TouchEvent Event, int X, int Y, int Id, double RadiusX = 11.5, double RadiusY = 11.5, double RotationAngle = 0.0, double Pressure = 1.0, int Timeout = -1);
        void Key(KeyEvent Event, const std::string& Char, int KeyboardPresses = KeyboardModifiersNone);
        void KeyRaw(KeyEvent Event, WPARAM WindowsVirtualKeyCode, LPARAM NativeVirtualKeyCode, int KeyboardPresses = KeyboardModifiersNone);
        void Focus();

        //Interacting with elements
        Async Inspect(int X, int Y, int Position = 0, int Timeout = -1);

        //Dialogs
        void SetOpenFileDialogResult(const std::string& Result);
        void SetOpenFileDialogManualMode(bool IsManual);
        void SetPromptResult(const std::string& PromptResult);

        //Downloads
        Async AllowDownloads(int Timeout = -1);
        Async RestrictDownloads(int Timeout = -1);
        bool IsFileDownloadReady();
        std::string GetDownloadedFilePath();

        //Popups
        void AllowPopups();
        void RestrictPopups();

        //Cookies
        Async RestoreCookies(const std::string& Cookies, int Timeout = -1);
        Async SaveCookies(int Timeout = -1);

        //Extensions
        void TriggerExtensionButton(const std::string ExtensionIdOrNamePart);
        std::vector<std::pair<std::string, std::string> > GetExtensionList();

        //Drag and drop
        Async StartDragFile(const std::string& Path, int Timeout = -1);

        //Helpers
        Async Sleep(int TimeMilliseconds, int Timeout = -1);

};

#endif // DEVTOOLSCONNECTOR_H
