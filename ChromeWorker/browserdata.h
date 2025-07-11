#ifndef BROWSERDATA_H
#define BROWSERDATA_H

#include <vector>
#include <memory>
#include <map>
#include "include/cef_base.h"
#include "inspectresult.h"
#include "highlightresult.h"
#include <atomic>
#include "modulesdata.h"
#include "configurableitem.h"
#include "localstoragedata.h"
#include "requestlist.h"
#include "sharedmemoryipc.h"
#include "browsersettingssaver.h"
#include "browsercontextmenu.h"
#include "idevtoolsconnector.h"
#include "resultmanager.h"


class BrowserData
{
public:
    BrowserData();

    enum ManualControlType
    {
        Indirect,
        DirectNoRecord,
        DirectRecord
    }ManualControl;
    bool SetIndirectControlOnNext;

    struct CachedItem
    {
        int status = 0;
        std::vector<std::pair<std::string, std::string> > request_headers;
        std::vector<std::pair<std::string, std::string> > response_headers;
        std::vector<char> body;
        std::vector<char> post_data;
        std::string url;
        bool is_error = false;
        std::string error;
        bool is_finished = false;
    };

    BrowserSettingsSaver Saver;

    RequestList _RequestList;
    std::atomic<int64> LastClearRequest;

    std::vector<std::pair<std::string,std::string> > _Headers;
    std::string _AcceptLanguagePattern;
    std::string _UserAgentDataBase64;
    std::string _UniqueProcessId;
    std::map<std::string,ConfigurableItem<std::string> > _StartupScript;
    CefWindowHandle _MainWindowHandle;
    CefWindowHandle _ParentWindowHandle;
    HWND UrlHandler;
    std::atomic<int64> OldestRequestTime;
    std::vector<std::pair<bool, std::string> > _CacheMask;
    std::vector<std::pair<bool, std::string> > _RequestMask;
    std::atomic_int WidthBrowser;
    std::atomic_int HeightBrowser;
    std::atomic_int WidthAll;
    std::atomic_int HeightAll;
    std::atomic_int ScrollX;
    std::atomic_int ScrollY;
    int CursorX;
    int CursorY;
    int DirectControlOrAutomationCursorX;
    int DirectControlOrAutomationCursorY;
    double DeviceScaleFactor = 1.0;
    std::atomic_bool IsRecord;
    std::atomic_bool IsRecordHttp;
    std::atomic_bool MultiselectMode;
    std::atomic_bool MultiselectIsInsideElementLoop;
    InspectResult _Inspect;
    HighlightResult _Highlight;
    ModulesDataList _ModulesData;
    std::atomic_bool HasHcaptchaModule;
    std::atomic_bool HasRecaptchaModule;
    std::atomic_bool HasFuncaptchaModule;
    std::string BrowserCode;
    ModulesDataList _UnusedModulesData;
    MultiSelectData _MultiSelectData;
    std::atomic_bool IsProxySet;

    //Touch
    std::atomic_bool IsTouchScreen;
    std::atomic_bool IsTouchPressedDirectControl;
    std::atomic_bool IsTouchPressedAutomation;
    std::atomic_int TouchEventId;


    //Recaptcha V3
    std::string _RecaptchaV3List;

    //Dialogs
    std::string _HttpAuthLogin;
    std::string _HttpAuthPassword;

    //DragAndDrop
    bool IsDrag;
    bool IsMousePress;
    bool LastClickIsFromIndirectControl;

    //Reset
    std::atomic_bool IsReset;
    std::atomic_bool IsAboutBlankLoaded;

    //Creating popup
    std::atomic_bool IsCreatingNewPopup;
    std::atomic_bool IsCreatingNewPopupIsLoaded;
    std::atomic_bool IsCreatingNewPopupIsContextCreated;
    std::atomic_bool IsCreatingNewPopupIsSilent;
    std::atomic_bool IsCreatingNewPopupIsLoadAfterOpen;
    std::string IsCreatingNewPopupUrl;
    std::atomic_int IsCreatingNewPopupIndexBeforeChange;

    //LocalStorage
    //LocalStorageData _LocalStorageData;

    //Timezone
    std::atomic_bool TimezoneSelected;
    std::atomic_int Timezone;

    //Tesing
    bool IsTesing;

    //Remote debugging port for CEF browsers
    int RemoteDebuggingPort;
    //Remote debugging port for Chromium browsers
    int MainRemoteDebuggingPort;

    BrowserContextMenu _BrowserContextMenu;


    IDevToolsConnector *Connector = 0;
    ResultManager *Results = 0;

};

#endif // BROWSERDATA_H
