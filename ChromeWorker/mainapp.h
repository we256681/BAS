#ifndef MAINAPP_H
#define MAINAPP_H
#include "include/cef_app.h"
#include "include/cef_request_context_handler.h"
#include "mainhandler.h"
#include "devtoolshandler.h"
#include "cookievisitor.h"
#include "browserdata.h"
#include "v8handler.h"
#include "elementcommand.h"
#include "browsereventsemulator.h"
#include "toolboxhandler.h"
#include "scenariohandler.h"
#include "centralhandler.h"
#include <ctime>
#include "mainlayout.h"
#include "toolboxv8handler.h"
#include "detectorv8handler.h"
#include "scenariov8handler.h"
#include "centralv8handler.h"
#include "detectorhandler.h"
#include "variablesextractor.h"
#include "extract_functions.h"
#include "extract_resources.h"
#include "settings.h"
#include "handlersmanager.h"
#include "postmanager.h"
#include "imagefinder.h"
#include "devtoolsrequest2action.h"
#include "fingerprintdetector.h"
#include "notificationmanager.h"
#include "browserdirectcontrol.h"
#include "ipcsimple.h"
#include "donothingcallback.h"
#include "emptyrequestcontexthandler.h"
#include "popupemulation.h"



class MainApp: public CefApp, public CefBrowserProcessHandler, public CefCompletionCallback, public CefRequestContextHandler
{
    std::string ViewRequestId;

    CefRefPtr<DoNothingCallback> DoNothing;
    CefRefPtr<EmptyRequestContextHandler> _EmptyRequestContextHandler;

    std::shared_ptr<HandlersManager> _HandlersManager;
    std::shared_ptr<BrowserDirectControl> _BrowserDirectControl;

    CefRefPtr<ToolboxHandler> thandler;
    CefRefPtr<ScenarioHandler> shandler;
    CefRefPtr<DetectorHandler> detecthandler;
    CefRefPtr<CentralHandler> chandler;
    CefRefPtr<CefBrowser> BrowserToolbox;
    CefRefPtr<CefBrowser> BrowserScenario;
    CefRefPtr<CefBrowser> BrowserDetector;
    CefRefPtr<CefBrowser> BrowserCentral;
    CefRefPtr<DevToolsHandler> dhandler;
    CefRefPtr<CookieVisitor> cookievisitor;
    CefRefPtr<V8Handler> v8handler;
    CefRefPtr<ToolboxV8Handler> toolboxv8handler;
    CefRefPtr<ScenarioV8Handler> scenariov8handler;
    CefRefPtr<CentralV8Handler> central8handler;
    CefRefPtr<DetectorV8Handler> detector8handler;
    ElementCommand LastCommand;
    ElementCommand LastCommandCopy;

    std::string BrowserScenarioDelayScript;

    IPCSimple NetworkProcessIPC;
    bool ProxyLibraryLoaded = false;


    bool IsLastCommandNull;
    BrowserData *Data;
    PopupEmulation *_PopupEmulation = 0;
    PostManager *_PostManager;
    DevToolsReqest2Action *_DevToolsReqest2Action;
    settings* Settings;
    int ScrollX;
    int ScrollY;

    bool DoTour;

    std::string LastUsedSelector;
    std::string LastUsedLabel;

    //Browser close delayed
    long long BrowserCloseTime = 0;

    //MouseMove
    bool IsMouseMoveSimulation;
    int MouseStartX;
    int MouseStartY;
    int MouseEndX;
    int MouseEndY;
    bool DoMouseUpOnFinishMove;
    double MouseReleaseRadius;

    //Scroll stop tracking
    int ScrollTrackingX;
    int ScrollTrackingY;
    clock_t ScrollStopTracking;
    clock_t ScrollStopTrackingStart;
    bool DoTrackScroll;

    double MouseSpeed;
    double MouseGravity;
    double MouseDeviation;

    //MouseTrack
    clock_t LastMouseTrack;

    //Highlight
    clock_t LastHighlight;
    clock_t LastHighlightMultiselect;
    clock_t LastRecaptchaV3Check;
    Async TouchStartTask;
    Async HighlightTask;
    Async HighlightMultiselectTask;
    Async RecaptchaV3Task;
    std::string HighlightSelector;
    std::string RawHighlightSelector;
    int HighlightIndex;
    bool HighlightDoScrolling = false;
    bool IsHighlightIndexActive = false;
    clock_t LastHighlightIndexChanged = 0;

    //Delay for click
    //0 - no delay click
    //1 - Coordinates click
    //2 - Element click
    //3 - Type
    clock_t DelayNextClick;
    int DelayClickType;
    int DelayClickX;
    int DelayClickY;

    //TypeTextTask
    bool TypeTextTaskIsActive;
    int TypeTextX;
    int TypeTextY;
    bool TypeTextIsFirstLetter;
    std::string TypeText;
    KeyState TypeTextState;

    //Parent
    int ParentWidth;
    int ParentHeight;

    //Render
    bool NeedRenderNextFrame;
    clock_t RenderNextFrameTime;
    bool IsElementRender;
    int RenderX,RenderY,RenderWidth,RenderHeight;

    //Frame Chain Inspect
    int InspectX;
    int InspectY;
    int InspectPosition;
    Async InspectTask;

    //Frame Chain Execute Command
    std::vector<InspectResult> ExecuteFrameChain;
    int ExecuteFrameSearchingLength = 0;
    bool ExecuteFrameSearching = false;
    bool ExecuteFrameScrolling = false;
    //bool ExecuteInnerFrameScrolling = false;
    bool ExecuteFrameScrollingSwitch = false;
    int ExecuteSearchCoordinatesX;
    int ExecuteSearchCoordinatesY;

    //Load
    bool IsWaitingForLoad;

    std::string DelayedSend;

    std::string SetNextActionId;
    bool StartPlayScriptOnStart = false;

    bool IsMainBrowserCreating;

    int TypeTextDelay;
    int TypeTextDelayCurrent;
    clock_t TypeTextLastTime;

    void InitBrowser();
    std::string NextLoadPage;
    ImageFinder _ImageFinder;
    MainLayout *Layout;

    std::string Code, Schema, Resources, AdditionalResources, Variables, GlobalVariables, Functions, Labels, EmbeddedData, ApplicationEngineVersion, ScriptEngineVersion, InterfaceState;
    bool IsScenarioInterfaceInitialSent;
    bool IsToolboxInterfaceInitialSent;
    bool ResourcesChanged;
    void UpdateScrolls(std::string& data);
    void HandleMainBrowserEvents();
    void HandleToolboxBrowserEvents();
    void HandleScenarioBrowserEvents();
    void HandleCentralBrowserEvents();
    void HandleDetectorBrowserEvents();

    void ReadDoTour();
    std::string Lang;

    int RunElementCommandCallbackOnNextTimer;

public:
    MainApp();
    void WriteBrowserData();
    BrowserDirectControl * DirectControl();
    void UpdateManualControl(bool NoFocus = false);
    void DirectControlAddAction(const std::string& Script);
    FingerprintDetector Detector;
    NotificationManager Notifications;
    std::string Javascript(const std::string& Script, const std::string& BrowserType);

    void IncreaseInspectPosition();
    void DecreaseInspectPosition();

    void ForceUpdateWindowPositionWithParent();
    void UpdateWindowPositionWithParent();
    void SetData(BrowserData *Data);
    void SetPostManager(PostManager *_PostManager);
    void SetPopupEmulation(PopupEmulation *_PopupEmulation);
    void SetDevToolsReqest2Action(DevToolsReqest2Action *_DevToolsReqest2Action);
    DevToolsReqest2Action * GetDevToolsReqest2Action();
    void SetSettings(settings *Settings);
    void SetLayout(MainLayout *Layout);
    BrowserData * GetData();
    void GetAllPopupsUrls(std::function<void(const std::vector<std::string>&)> Callback);
    int GetActivePopupIndex();
    void ContextMenu(int X, int Y);


    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE;

    //CefBrowserProcessHandler
    virtual void OnBeforeCommandLineProcessing(const CefString& process_type,CefRefPtr<CefCommandLine> command_line) OVERRIDE;
    virtual void OnContextInitialized() OVERRIDE;
    virtual void OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info) OVERRIDE;
    virtual void OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line) OVERRIDE;

    //CefCompletionCallback
    virtual void OnComplete() OVERRIDE;

    //CefRequestContextHandler
    virtual CefRefPtr<CefResourceRequestHandler> GetResourceRequestHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_navigation, bool is_download, const CefString& request_initiator, bool& disable_default_handling) OVERRIDE;

    bool IsNeedQuit();

    //EventCallbacks
    void DisableBrowserCallback();
    void LoadCallback(const std::string& page);
    void Load2Callback(const std::string& url,const std::string& referrer, bool instant);
    void SetFocusOnNextLoad();
    void CreateBrowser(const std::string& Url);
    void ViewCallback(const std::string& RequestId);
    void GetTabsCallback(const std::string& RequestId);
    void LoadNoDataCallback();
    void LoadManualSelect();
    void NavigateBackCallback(bool IsInstant);

    void SetNextActionCallback(const std::string& NextActionId);
    void TimezoneCallback(int offset);

    void VisibleCallback(bool visible);
    void FlushCallback();
    void SetProxyCallback(const std::string& server, int Port, bool IsHttp, const std::string& username, const std::string& password, const std::string& target);
    void AddHeaderCallback(const std::string& key,const std::string& value, const std::string& target);
    void SetHeaderListCallback(const std::string& json);
    void SetAcceptLanguagePatternCallback(const std::string& pattern);
    void SetUserAgentDataCallback(const std::string& data);
    void CleanHeaderCallback();
    void GetUrlCallback();
    void GetBrowserScreenSettingsCallback();
    std::string GetUrl();
    void ProcessContextMenu(int MenuId);
    void ProcessFind(LPFINDREPLACE lpfr);
    void SetUserAgentCallback(const std::string& value);
    void PrepareFunctionCallback(const std::string& value);
    void BackupDoneCallback(const std::string& full_path);
    void RecaptchaV3ResultCallback(const std::string& id, const std::string& result);
    void RecaptchaV3ListCallback(const std::string& value);
    void ClickExtensionButton(const std::string& id);
    void SetOpenFileNameCallback(const std::string& value);
    void SetComboboxIndexCallback(int Index);
    void DragFileCallback(const std::string& value);
    void SetStartupScriptCallback(const std::string& value,const std::string& target,const std::string& script_id);
    void RunTaskCallback(const std::string& function_name,const std::string& params,const std::string& result_id);
    void CheckResultCallback(const std::string& CheckId,bool IsSuccess,const std::string& ErrorString);
    void SetWorkerSettingsCallback(bool EncodeUtf8, bool RefreshConnections, const std::string& server, int Port, bool IsHttp, const std::string& username, const std::string& password, const std::string& target, const std::string& browser, const std::string& record_id);
    void ManualBrowserControlCallback(const std::string& message);
    void SetFontListCallback(const std::string& fonts);
    void SetPromptResultCallback(const std::string& value);
    void SetHttpAuthResultCallback(const std::string& login,const std::string& password);
    void GetCookiesForUrlCallback(const std::string& value);
    void SaveCookiesCallback();
    void RestoreLocalStorageCallback(const std::string& value);
    void RestoreCookiesCallback(const std::string& value);
    void IsChangedCallback();

    void ClearImageDataCallback();
    void SetImageDataCallback(const std::string& base64);
    void FindImageCallback();

    void CrushCallback();
    void AddCacheMaskAllowCallback(const std::string& value);
    void AddCacheMaskDenyCallback(const std::string& value);
    void AddRequestMaskAllowCallback(const std::string& value);
    void AddRequestMaskDenyCallback(const std::string& value);
    void ClearCacheMaskCallback();
    void AllowPopups();
    void RestrictPopups();
    void AllowDownloads();
    void RestrictDownloads();
    void ClearRequestMaskCallback();
    void ClearLoadedUrlCallback();
    void ClearCachedDataCallback();
    void ScrollCallback(int x, int y);
    void RenderCallback(int x, int y, int width, int height);
    void ClearAllCallback();
    void ClearMasksCallback();
    void ClearDataCallback();
    void WaitCodeCallback();
    void StartSectionCallback(int);
    void ScriptFinishedCallback();
    void FindCacheByMaskBase64Callback(const std::string& value);
    void FindStatusByMaskCallback(const std::string& value);
    void FindUrlByMaskCallback(const std::string& value);
    void FindCacheByMaskStringCallback(const std::string& value);
    void FindAllCacheCallback(const std::string& value);
    void IsUrlLoadedByMaskCallback(const std::string& value);
    void GetLoadStatsCallback();
    void ElementCommandCallback(const ElementCommand &Command);
    void ClearElementCommand();

    void SetCodeCallback(const std::string & code,const std::string & embedded,const std::string & schema,bool is_testing, const std::string & interface_state, const std::string & script_engine_version, const std::string & application_engine_version);
    void SetResourceCallback(const std::string & resources);
    void SetInitialStateCallback(const std::string & lang);
    void RequestVariablesResultCallback(const std::string & data);
    void DebugVariablesResultCallback(const std::string & data);
    void DebugCallstackResultCallback(const std::string & data);

    void MouseClickCallback(int x, int y);
    void MouseClickUpCallback(int x, int y);
    void MouseClickDownCallback(int x, int y);
    void PopupCloseCallback(int index);
    void PopupSelectCallback(int index);
    void PopupCreateCallback(bool is_silent, const std::string& url);
    void PopupCreate2Callback(bool is_silent, const std::string& url, const std::string& referrer, bool is_instant);
    void PopupInfoCallback();
    void SetDeviceScaleFactorCallback(double scale);
    void MouseMoveCallback(int x, int y, double speed, double gravity, double deviation, bool iscoordinates, bool domouseup, double release_radius, bool relative_coordinates, bool track_scroll);
    void LoadSuccessCallback();
    void ResizeCallback(int width, int height);
    void SetWindowCallback(const std::string& Window, bool IsPlayingScript);
    void HighlightActionCallback(const std::string& ActionId);


    void UrlLoaded(const std::string&, int, int);
    void AfterReadyToCreateBrowser(bool Reload);
    void Timer();
    void Reload();
    void ShowDevTools();
    void DirectControlInspectMouse();
    void UpdateHighlight();
    void UpdateHighlightMultiselect();
    void UpdateRecaptchaV3Check();
    void UpdateMultiSelect();
    void ClearHighlight();
    void HandleScreenshotCapture();
    void ExecuteTypeText();
    void ExecuteMouseMove();
    void FinishedLastCommand(const std::string& data);
    void Paint(int width, int height);
    void UploadStart();
    void ComboboxOpened();
    void StartRequest(CefRefPtr<CefRequest> Request);
    void OnRequestDataMain(std::string RequestData);
    void OnRequestDataAdditional(std::string RequestData);
    void CursorChanged(int Type);
    void ProcessMessage(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message, bool* is_processed);
    void NewMainBrowserContextCreated(int BrowserId, bool IsMain);
    void LinkCtrlClick(const std::string& current_url, const std::string& target_url);
    void CurrentTabChanged();
    void CheckNetworkProcessIPC();
    void InitNetworkProcessIPC();
    void DownloadStart();
    void OldestRequestTimeChanged(int64 OldestTime);
    char* GetImageData();
    std::string GetSubImageDataBase64(int x1, int y1, int x2, int y2);
    std::pair<int,int> GetImageSize();
    void CreateTooboxBrowser();
    void CreateScenarioBrowser();
    void CreateDetectorBrowser();
    void CreateCentralBrowser();

    void EmulateClick(int x, int y);
    void EmulateMove(int x, int y);
    void EmulateMoveAndClick(int x, int y);
    void EmulateDrag(int x, int y);
    void EmulateDrop(int x, int y);

    //Tabs
    void AddTab();
    void SelectTab(int i);
    void TabInfo();
    void CloseTab(int i);

    //Events
    std::vector<std::function<void(const std::string&)> > EventSendTextResponce;
    std::vector<std::function<void(const std::string&)> > EventHighlightMenu;

    void Hide();
    void Terminate();
    void Restart();
    void ToggleDevTools();
    void ShowActionUpdater();
    void HideActionUpdater();
    std::pair<int,int> GetScrollPosition();
    void ScrollUp();
    void ScrollDown();
    void ScrollLeft();
    void ScrollRight();

    void ScrollUpUp();
    void ScrollDownDown();
    void ScrollLeftLeft();
    void ScrollRightRight();

    void InspectAt(int x, int y);

    void MouseMoveAt(int x, int y);
    void RepeatInspectMouseAt();
    void InspectMouseAt(int x, int y, clock_t CurrentTime);
    void MouseLeave();

    //Element Subtasks
    void ExecuteElementFunction(const std::string& FuncName, bool AskIfUseLoopFunction, bool IsDisabled = false, const std::string& ModuleName = std::string(), const std::string& ModuleDescription = std::string());

    void LoadSettingsPage();
    void ShowContextMenu(int X, bool IsImageSelect, const std::string & Json);
    void MainContextMenu(POINT& p);
    std::pair<std::string, bool> GetMenuSelected();

    void SendTextResponce(const std::string&);

    //Connector events
    void OnPaint();
    void OnResize();
    void OnScroll();
    void OnRequestStart(std::string RequestId);
    void OnRequestStop(std::string RequestId);
    void OnRecordHttpData(std::string Script);
    void OnLoadStart();
    void OnLoadStop();
    void OnAddressChanged(std::string Url);
    void OnNativeDialog(std::string DialogType);
    void OnDownloadStarted(std::wstring FileName);




private:
    IMPLEMENT_REFCOUNTING(MainApp);
};

#endif // MAINAPP_H

