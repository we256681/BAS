#ifndef BROWSERDIRECTCONTROL_H
#define BROWSERDIRECTCONTROL_H
#include "browserdata.h"
#include <functional>
#include "include/cef_app.h"

class HandlersManager;

class BrowserDirectControl
{
    //Structures
    struct MouseClickItem
    {
        int X;
        int Y;
        bool IsDownOrUp;
        bool IsDrop;
        bool IsDoubleClick;
        bool IsDrag;
        bool IsLeftMousePressed;
        bool IsRightMousePressed;
        bool IsCtrlPressed;
        bool IsShiftPressed;
        bool IsLeftMouseButton;
        CefBrowserHost::DragOperationsMask AllowedOptions;

    };

    enum SequenceItemType
    {
        ClickType = 0,
        DoubleClickType,
        DragStartType,
        DragEndType,
        KeyType,
        WaitFullPageLoadType,
        GoBackType,
        LoadType,
        PopupSelect,
        PopupClose
    };

    struct SequenceItem
    {
        std::string Code;
        std::string Meta;
        std::string Text;
        SequenceItemType Type;
        InspectResult Inspect;
        int64 Time;
    };

    //External Data
    BrowserData *_BrowserData;

    //Helpers
    std::string BrowserDirectControl::JsonEscape(const std::string& Text);
    std::string BrowserDirectControl::JsonEscapeInsideString(const std::string & Text);
    int64 Now();
    bool IsKeyDown(WPARAM wparam);
    void DoMouseEvent(MouseClickItem Item);
    void ApplyInspectResult(MouseClickItem Item, InspectResult Inspect);

    bool IsPageLoaded;

    //Internal data
    bool IsDrag;
    CefBrowserHost::DragOperationsMask allowedops;

    int64 LastClickTime;
    int LastClickX;
    int LastClickY;
    int LastMoveX = 0;
    int LastMoveY = 0;
    bool DontSendNextWaitFullPageLoaded;

    const int MaxDelay = 5000;

    bool LastAddTab = false;



    std::vector<SequenceItem> Sequence;

    std::string PreviousMouseMeta;
    std::string PreviousMouseCode;
    InspectResult PreviousMouseInspect;


    bool IsInspecting;
    std::vector<MouseClickItem> MouseClicks;

    //Working with sequence
    void SendCode(const std::string& Meta,const std::string& Code, bool DontSendNextWaitFullPageLoaded, bool IsWaitFullPageLoaded);
    void SendDetector(const std::string& Name,const std::string& Description);
    void SendSequenceItem(const SequenceItem& Item);
    void SendSequenceItems(std::vector<SequenceItem>& Items);
    void ProcessSequence();

    void AddItem(const std::string& Meta,const std::string& Code, const std::string& Text, SequenceItemType Type, InspectResult Inspect = InspectResult());

    void PageLoadedInternal();

    //Pending touch end event
    Async TouchStartTask;
    int64 TouchStartTime = 0;
    int TouchEndPendingX = 0;
    int TouchEndPendingY = 0;
    int TouchEndPendingId = 0;
    bool TouchEndIsPending = false;


public:

    //Pending touch end event
    void ProcessPendingTouchEndEvent();


    //Main methods
    BrowserDirectControl();
    void Init(BrowserData *_BrowserData);
    void Timer();

    //Mouse clicks
    struct InspectTask
    {
        int X = 0;
        int Y = 0;
        bool IsNull = false;
    };
    InspectTask GetInspectTask();
    void SetInspectResult(InspectResult Inspect);
    void TimeoutLastInspect();

    //Browser events
    void Load(const std::string& Url);
    void StartAddTab();
    void StopAddTab();
    void SelectTab(int i);
    void CloseTab(int i);
    void GoBack();
    void PageLoaded();
    void MouseMove(int X, int Y, bool IsMousePressed, bool IsCtrlPressed, bool IsShiftPressed);
    void ScrollUp();
    void ScrollDown();
    void MouseClick(int X, int Y, bool IsDownOrUp, bool IsLeftMousePressed, bool IsRightMousePressed, bool IsCtrlPressed, bool IsShiftPressed, bool IsLeftMouseButton);
    void Key(UINT msg, WPARAM wParam, LPARAM lParam);
    void StartDrag(CefRefPtr<CefBrowser> Browser, CefRefPtr<CefDragData> drag_data,CefBrowserHost::DragOperationsMask allowed_ops, int x, int y);
    void ClearSequence();

    //Events
    std::vector<std::function<void(const std::string&)> > EventExecuteScenarioCode;
};

#endif // BROWSERDIRECTCONTROL_H
