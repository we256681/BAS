#ifndef MAINLAYOUT_H
#define MAINLAYOUT_H

#include <windows.h>
#include <vector>
#include <functional>
#include "browserdata.h"

class MainLayout
{
    void MoveDevTools();
    void CalculateAllSize(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight, RECT& DevToolsRectangle, RECT& ToolboxRectangle, RECT& BrowserRectangle);

    long long LastTimeManualChangePicture;
    int ManualAnimation;
    bool IsRenderEmpty;

    double DeviceScaleFactor = 1.0;


public:
    enum TabType
    {
        Scenario,
        Devtools,
        Detector
    };
    enum StateClass{Ready,Hold,Finished}State;

    bool IsManualControlAction;
    std::string ManualControlLabel;

    bool IsRecord;
    bool IsTouchMode;
    bool IsHome;
    bool IsCentralShown;
    bool IsContextMenuShown;
    int ContextMenuWidth;
    bool ContextMenuIsRight;
    bool IsSettingsShown;
    bool IsManualSelectShown;
    bool IsNoDataShown;
    bool CanGoBack;
    BrowserData::ManualControlType ManualControl;

    int DevToolsRectWidth;
    int ToolBoxRectHeight;

    //Minimize/Maximize
    bool IsMinimized;
    bool IsToolboxMaximized;
    HBITMAP ButtonMinimize, ButtonMaximize;
    HBITMAP BManualControlAction,BManualControlAction1,BManualControlAction2,BManualControlAction3;
    HWND HButtonMinimizeMaximize;
    LONG SavelStyle,SavelExStyle;

    //Fingerprint detector
    int FingerprintDetectorNumber;


    //Buttons
    HWND HButtonUp,HButtonDown,HButtonLeft,HButtonRight;
    HWND HButtonUpUp,HButtonDownDown,HButtonLeftLeft,HButtonRightRight;
    HWND HButtonMenu;
    HWND HButtonSettings;
    HWND HButtonIndirect;
    HWND HButtonDirectNoRecord;
    HWND HButtonDirectRecord;
    HWND HEditUrl;
    HWND HLoadUrl;
    HWND HBackUrl;
    HWND HBrowserTabs;
    HWND HBrowserMenu;
    HWND HLabelBrowserBottom;
    HWND HLabelTop;

    //Cursors
    HCURSOR hcSizeNS, hcSizeEW, hcArrow, hcArrowTouch, hcCross, hcHand, hcHelp, hcIbeam, hcIcon, hcWait;
    int Cursor;
    bool IsCursorOverBrowser;

    //Browsers
    HWND BrowserHandle;
    HWND ToolBoxHandle;
    HWND ScenarioHandle;
    HWND DetectorHandle;
    HWND CentralHandle;
    HWND DevToolsHandle;

    //Mainwindow
    HWND MainWindowHandle = 0;


    int BrowserWidth, BrowserHeight, WindowWidth, WindowHeight;

    //High DPI support
    double GetScaleFactor();
    int Scale(int InputParam);

    MainLayout(int ToolboxHeight, int ScenarioWidth, double DeviceScaleFactor);
    void MinimizeOrMaximize(HWND MainWindow, HWND ParentWindow);

    void Update(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight, bool OnlyMove = false);
    void UpdateNoSizeChange();
    void CustomDraw(HDC hdc,int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight);
    bool DrawImageSelect(HDC hdc);
    void UpdateTabs(TabType _TabType);
    void UpdateFingerprintDetectorNumber(int FingerprintDetectorNumber);
    int GetFingerprintDetectorNumber();
    bool IsDevToolsTab();
    bool IsScenarioTab();
    bool IsFingerprintDetectorTab();
    void UpdateTabs();
    void UpdateState(StateClass State);
    void SetCanGoBack(bool CanGoBack);
    void SetIsRenderEmpty(bool IsRenderEmpty);
    bool GetIsRenderEmpty();
    void MaximizeToolbox(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight);
    void MinimizeToolbox(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight);
    void ShowCentralBrowser(bool IsSettings, bool IsHome);
    void ShowContextMenu(int X, int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight, int Zoom);
    void StartManualControl(const std::string& Message);
    void ChangeManualBrowserControlType(BrowserData::ManualControlType ManualControl, bool NoFocus);
    void ShowManualSelect();
    void SetBrowserLabel(const std::string& BrowserLabel);
    void SetLabelTop(const std::wstring& Str);

    void Focus(bool ForceFocus = false);
    void FocusUrl();

    void HideCentralBrowser();
    std::pair<int,int> GetDefaultWindowSize();
    std::pair<int,int> GetDefaultBrowserSize();
    RECT GetDevToolsRectangle(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight);
    RECT GetToolboxRectangle(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight);
    RECT GetBrowserRectangle(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight);
    RECT GetBrowserOuterRectangle(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight);
    RECT GetLabelRectangle(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight);
    RECT GetManualControlIndicatorRectangle(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight);
    RECT GetCentralRectangle(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight, bool IsHome);

    //Helpers
    HWND GetDevToolsHandle();

    void Timer(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight);
    HBITMAP GetManualAnimationButton();

    //Splitter
    int DevToolsRectWidthStart;
    int ToolBoxRectHeightStart;
    bool IsInsideScrollVertical;
    bool IsInsideScrollHorizontal;
    bool IsMoveScrollVertical;
    bool IsMoveScrollHorizontal;
    int SplitterX;
    int SplitterY;
    bool OnMouseMove(int x, int y, int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight);
    bool OnMouseUp();
    bool OnMouseLeave();
    bool OnMouseDown(int x, int y, int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight, bool IsControlButton);
    void SetBrowserCursor(int Cursor);
    void SetTouchMode(bool IsTouchMode);
    HCURSOR GetCursor(bool& NeedUpdate);
    bool IsTouchCursor();

    void SplitterHideInterface();
    void SplitterShowInterface();
    bool SplitterIsChangingSize();

    //Image select
    bool GetIsImageSelect();
    bool IsImageSelect;
    bool IsImageSelectLast;
    int ImageSelectStartX;
    int ImageSelectStartY;
    int ImageSelectEndX;
    int ImageSelectEndY;

    std::vector<std::function<void()> > EventLoadNoDataPage;
private:
    TabType _TabType;



};

#endif // MAINLAYOUT_H

