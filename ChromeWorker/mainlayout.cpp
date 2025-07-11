#include "mainlayout.h"
#include "log.h"
#include <chrono>
#include <converter.h>
#include "fillalpharectangle.h"

using namespace std::chrono;
MainLayout* _Layout;

MainLayout::MainLayout(int ToolboxHeight, int ScenarioWidth, double DeviceScaleFactor)
{
    IsRecord = false;
    IsHome = false;
    IsTouchMode = false;
    CanGoBack = false;

    IsManualControlAction = false;

    IsCursorOverBrowser = false;

    Cursor = 0;

    BrowserWidth = -1;
    BrowserHeight = -1;
    WindowWidth = -1;
    WindowHeight = -1;

    HButtonUp = 0;
    HButtonDown = 0;
    HButtonLeft = 0;
    HButtonRight = 0;
    HButtonUpUp = 0;
    HButtonDownDown = 0;
    HButtonLeftLeft = 0;
    HButtonRightRight = 0;
    HButtonMenu = 0;
    HButtonSettings = 0;
    HButtonIndirect = 0;
    HButtonDirectRecord = 0;
    HButtonDirectNoRecord = 0;
    HEditUrl = 0;
    HLoadUrl = 0;
    HBackUrl = 0;
    HBrowserTabs = 0;
    HBrowserMenu = 0;
    HLabelTop = 0;

    IsMinimized = false;
    IsToolboxMaximized = false;
    IsCentralShown = false;
    IsContextMenuShown = false;
    ContextMenuWidth = 0;
    ContextMenuIsRight = true;
    IsSettingsShown = false;
    IsManualSelectShown = false;
    IsNoDataShown = false;
    HButtonMinimizeMaximize = 0;
    ButtonMinimize = 0;
    ButtonMaximize = 0;

    FingerprintDetectorNumber = 0;

    //Browsers
    BrowserHandle = 0;
    ToolBoxHandle = 0;
    CentralHandle = 0;
    ScenarioHandle = 0;
    DevToolsHandle = 0;
    DetectorHandle = 0;

    //Mainwindow
    MainWindowHandle = 0;

    _TabType = Scenario;

    _Layout = this;

    State = Hold;

    DevToolsRectWidth = (ScenarioWidth == 0)?500:ScenarioWidth;
    ToolBoxRectHeight = (ToolboxHeight == 0)?300:ToolboxHeight;

    LastTimeManualChangePicture = 0;
    ManualAnimation = 0;

    //Splitter
    IsInsideScrollVertical = false;
    IsInsideScrollHorizontal = false;
    IsMoveScrollVertical = false;
    IsMoveScrollHorizontal = false;


    /*int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);


    if(DevToolsRectWidth > ScreenWidth - 600)
        DevToolsRectWidth = ScreenWidth - 600;

    if(ToolBoxRectHeight > ScreenHeight - 600)
        ToolBoxRectHeight = ScreenHeight - 600;*/

    //Is image select
    IsImageSelect = false;
    IsImageSelectLast = false;
    ImageSelectStartX = -1;
    ImageSelectStartY = -1;

    IsRenderEmpty = true;

    ManualControl = BrowserData::Indirect;

    this->DeviceScaleFactor = DeviceScaleFactor;

}

double MainLayout::GetScaleFactor()
{
    return DeviceScaleFactor;
}

int MainLayout::Scale(int InputParam)
{
    return InputParam * GetScaleFactor();
}

void MainLayout::ShowCentralBrowser(bool IsSettings, bool IsHome)
{
    if(CentralHandle)
    {
        bool IsManualSelectShownPrev = IsManualSelectShown;
        IsSettingsShown = IsSettings;
        this->IsHome = IsHome;
        IsCentralShown = true;
        IsNoDataShown = false;
        IsContextMenuShown = false;
        IsManualSelectShown = false;
        if(IsHome)
        {
            RECT r = GetBrowserOuterRectangle(BrowserWidth, BrowserHeight, WindowWidth, WindowHeight);
            MoveWindow(CentralHandle,r.left,r.top,r.right - r.left,r.bottom - r.top,true);
        }else
        {
            RECT r = GetBrowserRectangle(BrowserWidth, BrowserHeight, WindowWidth, WindowHeight);
            MoveWindow(CentralHandle,r.left,r.top,r.right - r.left,r.bottom - r.top,true);
        }

        ShowWindow(HButtonUp,SW_HIDE);
        ShowWindow(HLabelBrowserBottom,(IsManualControlAction && !IsSettings)?SW_SHOW:SW_HIDE);
        ShowWindow(HButtonDown,SW_HIDE);
        ShowWindow(HButtonLeft,SW_HIDE);
        ShowWindow(HButtonRight,SW_HIDE);
        ShowWindow(CentralHandle,SW_SHOW);
        ShowWindow(HButtonUpUp,SW_HIDE);
        ShowWindow(HButtonDownDown,SW_HIDE);
        ShowWindow(HButtonLeftLeft,SW_HIDE);
        ShowWindow(HButtonRightRight,SW_HIDE);

        if(HEditUrl)
            ShowWindow(HEditUrl,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);

        if(HLoadUrl)
            ShowWindow(HLoadUrl,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);

        if(HBackUrl)
            ShowWindow(HBackUrl,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);

        if(HBrowserTabs)
            ShowWindow(HBrowserTabs,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);

        if(HBrowserMenu)
            ShowWindow(HBrowserMenu,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);

        if(IsSettings || IsManualSelectShownPrev)
        {
            ShowWindow(HButtonSettings,SW_HIDE);
            ShowWindow(HButtonMinimizeMaximize,SW_HIDE);
            ShowWindow(HButtonDirectRecord,SW_HIDE);
            ShowWindow(HButtonIndirect,SW_HIDE);
            ShowWindow(HButtonDirectNoRecord,SW_HIDE);

        }else
        {
            ShowWindow(HButtonSettings,SW_SHOW);
            ShowWindow(HButtonMinimizeMaximize,SW_SHOW);
            if(ManualControl == BrowserData::Indirect)
            {
                ShowWindow(HButtonIndirect,SW_SHOW);
                ShowWindow(HButtonDirectRecord,SW_HIDE);
                ShowWindow(HButtonDirectNoRecord,SW_HIDE);
            }
            if(ManualControl == BrowserData::DirectRecord)
            {
                ShowWindow(HButtonIndirect,SW_HIDE);
                ShowWindow(HButtonDirectRecord,SW_SHOW);
                ShowWindow(HButtonDirectNoRecord,SW_HIDE);
            }
            if(ManualControl == BrowserData::DirectNoRecord)
            {
                ShowWindow(HButtonIndirect,SW_HIDE);
                ShowWindow(HButtonDirectRecord,SW_HIDE);
                ShowWindow(HButtonDirectNoRecord,SW_SHOW);
            }

        }

        UpdateNoSizeChange();
    }

}

void MainLayout::Focus(bool ForceFocus)
{
    /*if(GetActiveWindow() == HEditUrl || GetFocus() == HEditUrl)
    {
        return;
    }*/
    HWND Handle = MainWindowHandle;

    HWND hCurWnd = ::GetForegroundWindow();
    static time_t prev = time(0);

    time_t now = time(0);

    if(!ForceFocus)
    {
        if((Handle == hCurWnd) && now - prev < 3)
            return;
    }

    prev = now;

    SwitchToThisWindow(Handle,true);
    DWORD dwMyID = ::GetCurrentThreadId();
    DWORD dwCurID = ::GetWindowThreadProcessId(hCurWnd, NULL);
    ::AttachThreadInput(dwCurID, dwMyID, TRUE);
    ::SetWindowPos(Handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    ::SetWindowPos(Handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    ::SetForegroundWindow(Handle);
    ::AttachThreadInput(dwCurID, dwMyID, FALSE);
    ::SetFocus(Handle);
    ::SetActiveWindow(Handle);
}

void MainLayout::FocusUrl()
{
    Focus();
    SetFocus(HEditUrl);
    SetActiveWindow(HEditUrl);
    SendMessage(HEditUrl, EM_SETSEL, 0, -1);

}

void MainLayout::StartManualControl(const std::string& Message)
{
    this->ManualControlLabel = Message;
    IsManualControlAction = true;
    UpdateState(State);
    std::wstring Str = s2ws(Message);
    SetWindowText(HLabelBrowserBottom,Str.c_str());
    ShowWindow(HLabelBrowserBottom,SW_SHOW);

    {
        RECT r = GetManualControlIndicatorRectangle(BrowserWidth, BrowserHeight, WindowWidth, WindowHeight);
        InvalidateRect(MainWindowHandle,&r,true);
    }
}

void MainLayout::ChangeManualBrowserControlType(BrowserData::ManualControlType ManualControl, bool NoFocus)
{
    this->ManualControl = ManualControl;
    if(ManualControl == BrowserData::Indirect)
    {
        SetBrowserCursor(0);
        ShowWindow(HButtonIndirect,SW_SHOW);
        ShowWindow(HButtonDirectRecord,SW_HIDE);
        ShowWindow(HButtonDirectNoRecord,SW_HIDE);
    }
    if(ManualControl == BrowserData::DirectRecord)
    {
        ShowWindow(HButtonIndirect,SW_HIDE);
        ShowWindow(HButtonDirectRecord,SW_SHOW);
        ShowWindow(HButtonDirectNoRecord,SW_HIDE);
        if(IsNoDataShown && !NoFocus)
        {
            Focus();
            SetFocus(HEditUrl);
            SetActiveWindow(HEditUrl);
            SendMessage(HEditUrl, EM_SETSEL, 0, -1);
        }
    }
    if(ManualControl == BrowserData::DirectNoRecord)
    {
        ShowWindow(HButtonIndirect,SW_HIDE);
        ShowWindow(HButtonDirectRecord,SW_HIDE);
        ShowWindow(HButtonDirectNoRecord,SW_SHOW);
        if(IsNoDataShown && !NoFocus)
        {
            Focus();
            SetFocus(HEditUrl);
            SetActiveWindow(HEditUrl);
            SendMessage(HEditUrl, EM_SETSEL, 0, -1);
        }
    }

    if(ManualControl != BrowserData::DirectNoRecord)
    {
        this->ManualControlLabel.clear();
        IsManualControlAction = false;
        SetWindowText(HLabelBrowserBottom,L"");
        {
            RECT r = GetManualControlIndicatorRectangle(BrowserWidth, BrowserHeight, WindowWidth, WindowHeight);
            InvalidateRect(MainWindowHandle,&r,true);
        }

    }

    UpdateNoSizeChange();

    IsManualSelectShown = false;
}

void MainLayout::ShowManualSelect()
{
    IsManualSelectShown = true;
    ShowCentralBrowser(false, false);
    IsNoDataShown = false;
    IsCentralShown = true;
    IsSettingsShown = false;
    IsManualSelectShown = true;

    UpdateNoSizeChange();
}


void MainLayout::SetBrowserLabel(const std::string& BrowserLabel)
{
    if(!IsManualControlAction)
    {
        std::string label = BrowserLabel;
        std::size_t start = label.find("<A>");
        std::size_t end = label.find("</A>");
        if(start != std::string::npos && end != std::string::npos && end > start)
        {
            std::string prefix = label.substr(start,end - start + 4);
            label.erase(start,end - start + 4);
            label = prefix + label;

        }

        std::wstring Str = s2ws(label);
        SetWindowText(HLabelBrowserBottom,Str.c_str());
    }
}

void MainLayout::SetLabelTop(const std::wstring& Str)
{
    SetWindowText(HLabelTop,Str.c_str());
    ShowWindow(HLabelTop,SW_SHOW);

}

void MainLayout::ShowContextMenu(int X, int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight, int Zoom)
{

    if(CentralHandle)
    {
        HideCentralBrowser();

        this->ContextMenuIsRight = ContextMenuIsRight;

        IsCentralShown = false;
        IsContextMenuShown = true;

        RECT r = GetBrowserRectangle(BrowserWidth, BrowserHeight, WindowWidth, WindowHeight);

        int half = (r.right - r.left)/2;
        ContextMenuWidth = half;
        if(ContextMenuWidth < Scale(250))
        {
            ContextMenuWidth = Scale(250);
        }
        int MaxWidth = Scale(3 * Zoom);
        if(ContextMenuWidth > MaxWidth)
        {
            ContextMenuWidth = MaxWidth;
        }

        if(ContextMenuWidth > (r.right - r.left) * 0.8)
        {
            ContextMenuWidth = (r.right - r.left) * 0.8;
        }

        if(X > half)
        {
            ContextMenuIsRight = false;
            MoveWindow(CentralHandle,r.left,r.top,ContextMenuWidth,r.bottom - r.top,true);
        }else
        {
            ContextMenuIsRight = true;
            MoveWindow(CentralHandle,r.right - ContextMenuWidth,r.top,ContextMenuWidth,r.bottom - r.top,true);
        }



        ShowWindow(HButtonUp,SW_SHOW);
        ShowWindow(HLabelBrowserBottom,SW_SHOW);
        ShowWindow(HButtonDown,SW_SHOW);
        ShowWindow(HButtonLeft,SW_SHOW);
        ShowWindow(HButtonRight,SW_SHOW);
        ShowWindow(HButtonUpUp,SW_SHOW);
        ShowWindow(HButtonDownDown,SW_SHOW);
        ShowWindow(HButtonLeftLeft,SW_SHOW);
        ShowWindow(HButtonRightRight,SW_SHOW);
        ShowWindow(CentralHandle,SW_SHOW);

        {
            RECT r = GetBrowserOuterRectangle(BrowserWidth, BrowserHeight, WindowWidth, WindowHeight);
            InvalidateRect(MainWindowHandle,&r,true);
        }

    }
}

void MainLayout::HideCentralBrowser()
{
    if(CentralHandle)
    {
        this->IsHome = false;

        ShowWindow(HButtonSettings,SW_SHOW);
        ShowWindow(HButtonMinimizeMaximize,SW_SHOW);
        if(ManualControl == BrowserData::Indirect)
        {
            ShowWindow(HButtonIndirect,SW_SHOW);
            ShowWindow(HButtonDirectRecord,SW_HIDE);
            ShowWindow(HButtonDirectNoRecord,SW_HIDE);
        }
        if(ManualControl == BrowserData::DirectRecord)
        {
            ShowWindow(HButtonIndirect,SW_HIDE);
            ShowWindow(HButtonDirectRecord,SW_SHOW);
            ShowWindow(HButtonDirectNoRecord,SW_HIDE);
        }
        if(ManualControl == BrowserData::DirectNoRecord)
        {
            ShowWindow(HButtonIndirect,SW_HIDE);
            ShowWindow(HButtonDirectRecord,SW_HIDE);
            ShowWindow(HButtonDirectNoRecord,SW_SHOW);
        }

        if(HEditUrl)
            ShowWindow(HEditUrl,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);

        if(HLoadUrl)
            ShowWindow(HLoadUrl,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);

        if(HBackUrl)
            ShowWindow(HBackUrl,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);

        if(HBrowserTabs)
            ShowWindow(HBrowserTabs,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);

        if(HBrowserMenu)
            ShowWindow(HBrowserMenu,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);

        if((IsNoDataShown || IsRenderEmpty) && IsRecord)
        {
            IsSettingsShown = false;
            IsNoDataShown = true;
            IsCentralShown = true;
            for(auto f:EventLoadNoDataPage)
                f();
            UpdateNoSizeChange();
            return;
        }

        IsSettingsShown = false;
        IsCentralShown = false;
        IsNoDataShown = false;
        IsContextMenuShown = false;
        IsManualSelectShown = false;


        ShowWindow(HButtonUp,SW_SHOW);
        ShowWindow(HLabelBrowserBottom,SW_SHOW);
        ShowWindow(HButtonDown,SW_SHOW);
        ShowWindow(HButtonLeft,SW_SHOW);
        ShowWindow(HButtonRight,SW_SHOW);
        ShowWindow(HButtonUpUp,SW_SHOW);
        ShowWindow(HButtonDownDown,SW_SHOW);
        ShowWindow(HButtonLeftLeft,SW_SHOW);
        ShowWindow(HButtonRightRight,SW_SHOW);
        ShowWindow(CentralHandle,SW_HIDE);




        UpdateNoSizeChange();
    }
}

void MainLayout::MaximizeToolbox(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight)
{
    if(ToolBoxHandle)
    {
        MoveWindow(ToolBoxHandle,0,0,WindowWidth,WindowHeight,true);
        ShowWindow(HButtonUp,SW_HIDE);
        ShowWindow(HLabelBrowserBottom,SW_HIDE);
        ShowWindow(HLabelTop,SW_HIDE);
        ShowWindow(HButtonDown,SW_HIDE);
        ShowWindow(HButtonLeft,SW_HIDE);
        ShowWindow(HButtonRight,SW_HIDE);
        ShowWindow(HButtonUpUp,SW_HIDE);
        ShowWindow(HButtonDownDown,SW_HIDE);
        ShowWindow(HButtonLeftLeft,SW_HIDE);
        ShowWindow(HButtonRightRight,SW_HIDE);
        ShowWindow(HButtonMenu,SW_HIDE);
        ShowWindow(HButtonMinimizeMaximize,SW_HIDE);
        ShowWindow(HButtonSettings,SW_HIDE);
        ShowWindow(HEditUrl,SW_HIDE);
        ShowWindow(HLoadUrl,SW_HIDE);
        ShowWindow(HBackUrl,SW_HIDE);
        ShowWindow(HBrowserTabs,SW_HIDE);
        ShowWindow(HBrowserMenu,SW_HIDE);
        ShowWindow(HButtonIndirect,SW_HIDE);
        ShowWindow(HButtonDirectNoRecord,SW_HIDE);
        ShowWindow(HButtonDirectRecord,SW_HIDE);

        IsToolboxMaximized = true;
    }

}
void MainLayout::MinimizeToolbox(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight)
{
    if(ToolBoxHandle)
    {
        RECT r = GetToolboxRectangle(BrowserWidth,BrowserHeight,WindowWidth,WindowHeight);
        MoveWindow(ToolBoxHandle,r.left,r.top,r.right - r.left,r.bottom - r.top,true);
        ShowWindow(HButtonUp,SW_SHOW);
        ShowWindow(HLabelBrowserBottom,SW_SHOW);
        ShowWindow(HLabelTop,SW_SHOW);
        ShowWindow(HButtonDown,SW_SHOW);
        ShowWindow(HButtonLeft,SW_SHOW);
        ShowWindow(HButtonRight,SW_SHOW);
        ShowWindow(HButtonUpUp,SW_SHOW);
        ShowWindow(HButtonDownDown,SW_SHOW);
        ShowWindow(HButtonLeftLeft,SW_SHOW);
        ShowWindow(HButtonRightRight,SW_SHOW);
        ShowWindow(HButtonMenu,SW_SHOW);
        ShowWindow(HButtonMinimizeMaximize,SW_SHOW);
        ShowWindow(HButtonSettings,SW_SHOW);
        if(HEditUrl)
            ShowWindow(HEditUrl,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);
        if(HLoadUrl)
            ShowWindow(HLoadUrl,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);
        if(HBackUrl)
            ShowWindow(HBackUrl,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);
        if(HBrowserTabs)
            ShowWindow(HBrowserTabs,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);
        if(HBrowserMenu)
            ShowWindow(HBrowserMenu,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);
        if(ManualControl == BrowserData::Indirect)
        {
            ShowWindow(HButtonIndirect,SW_SHOW);
            ShowWindow(HButtonDirectRecord,SW_HIDE);
            ShowWindow(HButtonDirectNoRecord,SW_HIDE);
        }
        if(ManualControl == BrowserData::DirectRecord)
        {
            ShowWindow(HButtonIndirect,SW_HIDE);
            ShowWindow(HButtonDirectRecord,SW_SHOW);
            ShowWindow(HButtonDirectNoRecord,SW_HIDE);
        }
        if(ManualControl == BrowserData::DirectNoRecord)
        {
            ShowWindow(HButtonIndirect,SW_HIDE);
            ShowWindow(HButtonDirectRecord,SW_HIDE);
            ShowWindow(HButtonDirectNoRecord,SW_SHOW);
        }

        IsToolboxMaximized = false;
    }
}

void MainLayout::MinimizeOrMaximize(HWND MainWindow, HWND ParentWindow)
{
    IsMinimized = !IsMinimized;
    if(!MainWindow || !ParentWindow || !HButtonMinimizeMaximize || !ButtonMinimize || !ButtonMaximize)
        return;
    if(IsMinimized)
    {
        SetParent(MainWindow,ParentWindow);

        LONG lStyle = GetWindowLong( MainWindow, GWL_STYLE );
        SavelStyle = lStyle;
        //lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
        lStyle &= ~(WS_CAPTION | WS_THICKFRAME);
        SetWindowLong( MainWindow, GWL_STYLE, lStyle);
        LONG lExStyle = GetWindowLong( MainWindow, GWL_EXSTYLE );
        SavelExStyle = lExStyle;
        lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
        SetWindowLong( MainWindow, GWL_EXSTYLE, lExStyle );

        SendMessage(HButtonMinimizeMaximize, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)ButtonMaximize);

    }else
    {
        SetParent(MainWindow,0);

        SetWindowLong( MainWindow, GWL_STYLE, SavelStyle );
        SetWindowLong( MainWindow, GWL_EXSTYLE, SavelExStyle );

        ShowWindow(MainWindow,SW_SHOWMAXIMIZED);
        SetForegroundWindow(MainWindow);
        SetActiveWindow(MainWindow);
        FlashWindow(MainWindow,true);
        SendMessage(HButtonMinimizeMaximize, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)ButtonMinimize);
    }
    InvalidateRect(MainWindowHandle,NULL,false);

}


void MainLayout::CalculateAllSize(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight, RECT& DevToolsRectangle, RECT& ToolboxRectangle, RECT& BrowserRectangle)
{
    BrowserWidth = Scale(BrowserWidth);
    BrowserHeight = Scale(BrowserHeight);

    int UrlOfset = 0;
    int UrlOfset2 = 0;
    if(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))
    {
        UrlOfset = 2 + 2 + Scale(20);
        UrlOfset2 = 1 + 5 + 5 + Scale(20);
    }
    DevToolsRectangle.left = 0;
    DevToolsRectangle.right = DevToolsRectWidth-3;
    DevToolsRectangle.top = Scale(30) + 1;
    DevToolsRectangle.bottom = WindowHeight;

    ToolboxRectangle.left = DevToolsRectWidth+6;
    ToolboxRectangle.right = WindowWidth;
    ToolboxRectangle.top = 0;
    ToolboxRectangle.bottom = ToolBoxRectHeight - 4;

    if(IsRecord)
    {
        BrowserRectangle.left = DevToolsRectWidth+5;
        BrowserRectangle.right = WindowWidth - 1 - 5 - 5 - Scale(20);
        BrowserRectangle.top = ToolBoxRectHeight + 4 + UrlOfset;
        BrowserRectangle.bottom = WindowHeight - 1 - 5 - 5 - Scale(20);
    }else
    {
        BrowserRectangle.left = 0;
        BrowserRectangle.right = WindowWidth;
        BrowserRectangle.top = UrlOfset;
        BrowserRectangle.bottom = WindowHeight - UrlOfset2;
    }

    int AvailableWidth = BrowserRectangle.right - BrowserRectangle.left;
    int AvailableHeight = BrowserRectangle.bottom - BrowserRectangle.top;

    int ResultWidth = BrowserWidth, ResultHeight = BrowserHeight;
    if(AvailableWidth < ResultWidth)
    {
        ResultWidth = AvailableWidth;
        ResultHeight = (float)(BrowserHeight * ResultWidth) / (float)BrowserWidth;
    }

    if(AvailableHeight < ResultHeight)
    {
        ResultHeight = AvailableHeight;
        ResultWidth = (float)(BrowserWidth * ResultHeight) / (float)BrowserHeight;
    }

    BrowserRectangle.left = BrowserRectangle.left + (AvailableWidth - ResultWidth) / 2;
    BrowserRectangle.top = BrowserRectangle.top + (AvailableHeight - ResultHeight) / 2;
    BrowserRectangle.right = BrowserRectangle.left + ResultWidth;
    BrowserRectangle.bottom = BrowserRectangle.top + ResultHeight;

}


BOOL CALLBACK FindDevToolsIteration(HWND hwnd, LPARAM lParam)
{
    if(GetParent(hwnd) == _Layout->MainWindowHandle)
    {
        std::wstring CefBrowserWindow = L"CefBrowserWindow";
        wchar_t data[256];
        int res = GetClassName(hwnd,data,256);
        if(res && std::wstring(data,res) == CefBrowserWindow && (!_Layout->ToolBoxHandle || _Layout->ToolBoxHandle != hwnd)&& (!_Layout->ScenarioHandle || _Layout->ScenarioHandle != hwnd)&& (!_Layout->CentralHandle || _Layout->CentralHandle != hwnd)&& (!_Layout->DetectorHandle || _Layout->DetectorHandle != hwnd))
        {
            _Layout->DevToolsHandle = hwnd;
            return false;
        }
    }
    return true;
}

HWND MainLayout::GetDevToolsHandle()
{
    DevToolsHandle = 0;
    if(MainWindowHandle)
        EnumChildWindows(MainWindowHandle,FindDevToolsIteration,NULL);
    return DevToolsHandle;
}

void MainLayout::MoveDevTools()
{
    HWND DevToolsHandle = GetDevToolsHandle();
    if(DevToolsHandle)
    {
        RECT DevToolsRectangle = _Layout->GetDevToolsRectangle(_Layout->BrowserWidth, _Layout->BrowserHeight, _Layout->WindowWidth, _Layout->WindowHeight);
        MoveWindow(DevToolsHandle,DevToolsRectangle.left,DevToolsRectangle.top,DevToolsRectangle.right - DevToolsRectangle.left,DevToolsRectangle.bottom - DevToolsRectangle.top,true);
    }

    if(ScenarioHandle)
    {
        RECT Rectangle = _Layout->GetDevToolsRectangle(_Layout->BrowserWidth, _Layout->BrowserHeight, _Layout->WindowWidth, _Layout->WindowHeight);
        MoveWindow(ScenarioHandle,Rectangle.left,Rectangle.top,Rectangle.right - Rectangle.left,Rectangle.bottom - Rectangle.top,true);
    }

    if(DetectorHandle)
    {
        RECT Rectangle = _Layout->GetDevToolsRectangle(_Layout->BrowserWidth, _Layout->BrowserHeight, _Layout->WindowWidth, _Layout->WindowHeight);
        MoveWindow(DetectorHandle,Rectangle.left,Rectangle.top,Rectangle.right - Rectangle.left,Rectangle.bottom - Rectangle.top,true);
    }
}


void MainLayout::UpdateFingerprintDetectorNumber(int FingerprintDetectorNumber)
{
    this->FingerprintDetectorNumber = FingerprintDetectorNumber;
}

int MainLayout::GetFingerprintDetectorNumber()
{
    return FingerprintDetectorNumber;
}

void MainLayout::UpdateTabs()
{
    UpdateTabs(_TabType);
}

void MainLayout::UpdateTabs(TabType _TabType)
{
    this->_TabType = _TabType;
    if(!SplitterIsChangingSize())
    {
        HWND DevToolsHandle = GetDevToolsHandle();
        if(DevToolsHandle)
            ShowWindow(DevToolsHandle,(_TabType == Devtools)?SW_SHOW:SW_HIDE);
        if(ScenarioHandle)
            ShowWindow(ScenarioHandle,(_TabType == Scenario)?SW_SHOW:SW_HIDE);
        if(DetectorHandle)
            ShowWindow(DetectorHandle,(_TabType == Detector)?SW_SHOW:SW_HIDE);
    }

}

bool MainLayout::IsDevToolsTab()
{
    return _TabType == Devtools;
}

bool MainLayout::IsScenarioTab()
{
    return _TabType == Scenario;
}

bool MainLayout::IsFingerprintDetectorTab()
{
    return _TabType == Detector;
}

void MainLayout::UpdateState(StateClass State)
{
    if(IsRecord)
    {
        StateClass OldState = this->State;
        this->State = State;
        //EnableWindow(ToolBoxHandle,State == Ready);

        if((State == Hold || State == Finished) && !IsSettingsShown && !IsManualSelectShown && OldState != this->State)
            HideCentralBrowser();

        if(State == Hold)
        {
            EnableWindow(HEditUrl,IsManualControlAction);
            EnableWindow(HLoadUrl,IsManualControlAction);
            EnableWindow(HBackUrl,IsManualControlAction && CanGoBack);
            EnableWindow(HBrowserTabs,IsManualControlAction);
            EnableWindow(HBrowserMenu,IsManualControlAction);
        }else
        {
            EnableWindow(HEditUrl,true);
            EnableWindow(HLoadUrl,true);
            EnableWindow(HBackUrl,CanGoBack);
            EnableWindow(HBrowserTabs,true);
            EnableWindow(HBrowserMenu,true);
        }
    }
}

void MainLayout::SetCanGoBack(bool CanGoBack)
{
    this->CanGoBack = CanGoBack;
    EnableWindow(HBackUrl,CanGoBack && (State == Ready || IsManualControlAction));
}

bool MainLayout::GetIsRenderEmpty()
{
    return IsRenderEmpty;
}

void MainLayout::SetIsRenderEmpty(bool IsRenderEmpty)
{
    if(IsRecord)
    {
        if(this->IsRenderEmpty && !IsRenderEmpty && IsNoDataShown)
        {
            this->IsRenderEmpty = IsRenderEmpty;
            IsNoDataShown = false;

            HideCentralBrowser();
        }else if(!this->IsRenderEmpty && IsRenderEmpty && !IsCentralShown)
        {
            this->IsRenderEmpty = IsRenderEmpty;

            ShowCentralBrowser(false, false);
            IsNoDataShown = true;
            IsCentralShown = true;
            IsSettingsShown = false;
            IsManualSelectShown = false;
            for(auto f:EventLoadNoDataPage)
                f();

            UpdateNoSizeChange();
        }
        else
        {
            this->IsRenderEmpty = IsRenderEmpty;
        }
    }
}

void MainLayout::CustomDraw(HDC hdc,int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight)
{
    if(!IsRecord)
        return;

    if(IsToolboxMaximized)
        return;
    if(!SplitterIsChangingSize())
    {
        POINT pt;
        SelectObject(hdc, GetStockObject(DC_PEN));
        SetDCPenColor(hdc, RGB(188,188,188));

        MoveToEx(hdc, 0, Scale(30), &pt);
        LineTo(hdc, DevToolsRectWidth - 3, Scale(30));

        /*SetDCPenColor(hdc, RGB(200,200,200));

        MoveToEx(hdc, 0, 30, &pt);
        LineTo(hdc, DevToolsRectWidth - 3, 30);*/

    }

    RECT rect;
    HBRUSH brush = CreateSolidBrush(RGB(225, 225, 225));

    rect.left = DevToolsRectWidth - 2;
    rect.right = DevToolsRectWidth + 5;
    rect.top = 0;
    rect.bottom = WindowHeight;

    FillRect(hdc, &rect, brush);


    rect.left = DevToolsRectWidth + 5;
    rect.right = WindowWidth;
    rect.top = ToolBoxRectHeight-3;
    rect.bottom = ToolBoxRectHeight+3;

    FillRect(hdc, &rect, brush);
    DeleteObject(brush);

}

bool MainLayout::DrawImageSelect(HDC hdc)
{
    

    bool NeedToDraw = GetIsImageSelect() || (IsContextMenuShown && IsImageSelectLast);
    //WORKER_LOG(std::string("GetIsImageSelect() = ") + std::to_string(GetIsImageSelect()));
    //WORKER_LOG(std::string("IsContextMenuShown = ") + std::to_string(IsContextMenuShown));
    //WORKER_LOG(std::string("IsImageSelectLast = ") + std::to_string(IsImageSelectLast));
    //WORKER_LOG(std::string("NeedToDraw = ") + std::to_string(NeedToDraw));
    if(NeedToDraw)
    {
        POINT pt;
        HPEN hPen = CreatePen(PS_SOLID,2,RGB(50,250,50));
        SelectObject(hdc, hPen);

        MoveToEx(hdc, ImageSelectStartX, ImageSelectStartY, &pt);
        LineTo(hdc, ImageSelectStartX, ImageSelectEndY);

        MoveToEx(hdc, ImageSelectStartX, ImageSelectEndY, &pt);
        LineTo(hdc, ImageSelectEndX, ImageSelectEndY);

        MoveToEx(hdc, ImageSelectEndX, ImageSelectEndY, &pt);
        LineTo(hdc, ImageSelectEndX, ImageSelectStartY);

        MoveToEx(hdc, ImageSelectEndX, ImageSelectStartY, &pt);
        LineTo(hdc, ImageSelectStartX, ImageSelectStartY);

        DeleteObject(hPen);

        /*HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));

        {
            RECT r;
            if(ImageSelectEndY > ImageSelectStartY)
            {
                r.top = ImageSelectStartY;
                r.bottom = ImageSelectEndY;
            }else
            {
                r.top = ImageSelectEndY;
                r.bottom = ImageSelectStartY;
            }

            if(ImageSelectEndX > ImageSelectStartX)
            {
                r.left = ImageSelectStartX;
                r.right = ImageSelectEndX;
            }else
            {
                r.left = ImageSelectEndX;
                r.right = ImageSelectStartX;
            }

            FillAlphaRect(hdc,&r,brush,20);
        }


        DeleteObject(brush);*/
    }

    return NeedToDraw;
}

bool MainLayout::GetIsImageSelect()
{
    return IsImageSelect && std::abs(ImageSelectStartX - ImageSelectEndX) > 5 && std::abs(ImageSelectStartY - ImageSelectEndY) > 5;
}

RECT MainLayout::GetBrowserOuterRectangle(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight)
{
    RECT r;
    r.left = DevToolsRectWidth + 5;
    r.top = ToolBoxRectHeight + 3;
    if(!IsRecord)
    {
        r.left = 0;
        r.top = 0;
    }
    r.right = WindowWidth;

    r.bottom = WindowHeight;
    return r;
}

void MainLayout::UpdateNoSizeChange()
{
    Update(this->BrowserWidth,this->BrowserHeight,this->WindowWidth,this->WindowHeight, true);
}

void MainLayout::Update(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight, bool OnlyMove)
{
    if(!IsRecord)
    {
        ShowWindow(HButtonMinimizeMaximize,SW_HIDE);
        ShowWindow(HButtonUp,SW_HIDE);
        //ShowWindow(HLabelBrowserBottom,SW_HIDE);
        ShowWindow(HButtonDown,SW_HIDE);
        ShowWindow(HButtonLeft,SW_HIDE);
        ShowWindow(HButtonRight,SW_HIDE);
        ShowWindow(HButtonUpUp,SW_HIDE);
        ShowWindow(HButtonDownDown,SW_HIDE);
        ShowWindow(HButtonLeftLeft,SW_HIDE);
        ShowWindow(HButtonRightRight,SW_HIDE);
        ShowWindow(HButtonMenu,SW_HIDE);
        ShowWindow(HButtonSettings,SW_HIDE);
        ShowWindow(HLabelTop,SW_HIDE);
        //ShowWindow(HEditUrl,SW_HIDE);
        //ShowWindow(HLoadUrl,SW_HIDE);
        //ShowWindow(HBackUrl,SW_HIDE);
        ShowWindow(HButtonIndirect,SW_HIDE);
        ShowWindow(HButtonDirectNoRecord,SW_HIDE);
        ShowWindow(HButtonDirectRecord,SW_HIDE);

    }


    if(!OnlyMove)
    {
        UpdateState(State);
        UpdateTabs();

        RECT r = GetBrowserOuterRectangle(BrowserWidth, BrowserHeight, WindowWidth, WindowHeight);
        if(!IsCentralShown && MainWindowHandle)
            InvalidateRect(MainWindowHandle,&r,true);
    }

    this->BrowserWidth = BrowserWidth;
    this->BrowserHeight = BrowserHeight;
    this->WindowWidth = WindowWidth;
    this->WindowHeight = WindowHeight;
    RECT ToolboxRectangle = GetToolboxRectangle(BrowserWidth, BrowserHeight, WindowWidth, WindowHeight);
    RECT BrowserRectangle = GetBrowserRectangle(BrowserWidth, BrowserHeight, WindowWidth, WindowHeight);


    if(IsToolboxMaximized)
    {
        MoveWindow(ToolBoxHandle,0,0,WindowWidth,WindowHeight,true);
    }else
    {
        MoveWindow(ToolBoxHandle,ToolboxRectangle.left,ToolboxRectangle.top,ToolboxRectangle.right - ToolboxRectangle.left,ToolboxRectangle.bottom - ToolboxRectangle.top,true);
    }

    if(CentralHandle)
    {
        RECT CentralRectangle = GetCentralRectangle(BrowserWidth, BrowserHeight, WindowWidth, WindowHeight,IsHome);
        MoveWindow(CentralHandle,CentralRectangle.left,CentralRectangle.top,CentralRectangle.right - CentralRectangle.left,CentralRectangle.bottom - CentralRectangle.top,true);
    }

    MoveDevTools();

    if(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))
    {
        if(IsRecord)
        {
            MoveWindow(HEditUrl,BrowserRectangle.left + 1 + 2 + Scale(20) + 2 + Scale(20) + 2 + Scale(20), BrowserRectangle.top - 1 - Scale(22), BrowserRectangle.right - BrowserRectangle.left + 2 - 2 - Scale(20) - 2 - Scale(20) - 2 - Scale(20), Scale(22), true);
            MoveWindow(HLoadUrl,BrowserRectangle.right + 5, BrowserRectangle.top - 2 - Scale(20), Scale(20), Scale(20), true);
            MoveWindow(HBackUrl,BrowserRectangle.left + 1 + 2 + Scale(20) + 2 + Scale(20), BrowserRectangle.top - 2 - Scale(20), Scale(20), Scale(20), true);
            MoveWindow(HBrowserTabs,BrowserRectangle.left + 1 + 2 + Scale(20), BrowserRectangle.top - 2 - Scale(20), Scale(20), Scale(20), true);
            MoveWindow(HBrowserMenu,BrowserRectangle.left + 1, BrowserRectangle.top - 2 - Scale(20), Scale(20), Scale(20), true);
        }else
        {
            MoveWindow(HEditUrl,BrowserRectangle.left + 1 + 2 + Scale(20) + 2 + Scale(20) + 2 + Scale(20), BrowserRectangle.top - 1 - Scale(22), BrowserRectangle.right - BrowserRectangle.left + 2 - 2 - Scale(20) - 5 - 1 - Scale(20) - 2 - Scale(20) - 2 - Scale(20), Scale(22), true);
            MoveWindow(HLoadUrl,BrowserRectangle.right - 5 - 1 + 5 - Scale(20), BrowserRectangle.top - 2 - Scale(20), Scale(20), Scale(20), true);
            MoveWindow(HBackUrl,BrowserRectangle.left + 1 + 2 + Scale(20) + 2 + Scale(20), BrowserRectangle.top - 2 - Scale(20), Scale(20), Scale(20), true);
            MoveWindow(HBrowserTabs,BrowserRectangle.left + 1 + 2 + Scale(20), BrowserRectangle.top - 2 - Scale(20), Scale(20), Scale(20), true);
            MoveWindow(HBrowserMenu,BrowserRectangle.left + 1, BrowserRectangle.top - 2 - Scale(20), Scale(20), Scale(20), true);
        }
        ShowWindow(HEditUrl,SW_SHOW);
        ShowWindow(HLoadUrl,SW_SHOW);
        ShowWindow(HBackUrl,SW_SHOW);
        ShowWindow(HBrowserTabs,SW_SHOW);
        ShowWindow(HBrowserMenu,SW_SHOW);
    }else
    {
        ShowWindow(HEditUrl,SW_HIDE);
        ShowWindow(HLoadUrl,SW_HIDE);
        ShowWindow(HBackUrl,SW_HIDE);
        ShowWindow(HBrowserTabs,SW_HIDE);
        ShowWindow(HBrowserMenu,SW_HIDE);
    }


    MoveWindow(HButtonUpUp,BrowserRectangle.right + 5, BrowserRectangle.bottom - Scale(20) - 2 - 10 - Scale(20) - Scale(20) - 2 - Scale(20) - Scale(19),Scale(20),Scale(20),true);
    MoveWindow(HButtonUp,BrowserRectangle.right + 5,BrowserRectangle.bottom - 10 - Scale(20) - Scale(20) - 2 - Scale(20) - Scale(19),Scale(20),Scale(20),true);
    {
        RECT r = GetLabelRectangle(BrowserWidth, BrowserHeight, WindowWidth, WindowHeight);
        MoveWindow(HLabelBrowserBottom,r.left,r.top + 2,r.right - r.left,r.bottom - r.top,true);
    }

    MoveWindow(HLabelTop,Scale(30) + 5,1,DevToolsRectWidth - 3 - Scale(30) - 5,Scale(30) - 2,true);

    MoveWindow(HButtonDown,BrowserRectangle.right + 5, BrowserRectangle.bottom - Scale(20) - 2 - Scale(20) - Scale(19),Scale(20),Scale(20),true);
    MoveWindow(HButtonDownDown,BrowserRectangle.right + 5, BrowserRectangle.bottom - Scale(20) - Scale(19),Scale(20),Scale(20),true);

    MoveWindow(HButtonLeftLeft,BrowserRectangle.right - Scale(20) - 2 - 10 - Scale(20) - Scale(20) - 2 - Scale(20) - Scale(19), BrowserRectangle.bottom + 5,Scale(20),Scale(20),true);
    MoveWindow(HButtonLeft,BrowserRectangle.right - 10 - Scale(20) - Scale(20) - 2 - Scale(20) - Scale(19), BrowserRectangle.bottom + 5,Scale(20),Scale(20),true);
    MoveWindow(HButtonRight,BrowserRectangle.right - Scale(20) - 2 - Scale(20) - Scale(19), BrowserRectangle.bottom + 5,Scale(20),Scale(20),true);
    MoveWindow(HButtonRightRight,BrowserRectangle.right - Scale(20) - Scale(19), BrowserRectangle.bottom + 5,Scale(20),Scale(20),true);


    if(IsHome)
    {
        RECT OuterRectangle = GetBrowserOuterRectangle(BrowserWidth, BrowserHeight, WindowWidth, WindowHeight);
        MoveWindow(HButtonIndirect,OuterRectangle.right - Scale(20) - Scale(19), OuterRectangle.top + Scale(10), Scale(20), Scale(31), true);
        MoveWindow(HButtonDirectRecord,OuterRectangle.right - Scale(20) - Scale(19), OuterRectangle.top + Scale(10), Scale(20), Scale(31), true);
        MoveWindow(HButtonDirectNoRecord,OuterRectangle.right - Scale(20) - Scale(19), OuterRectangle.top + Scale(10), Scale(20), Scale(31), true);

        MoveWindow(HButtonSettings,OuterRectangle.right - Scale(20) - Scale(19), OuterRectangle.top + 2 + Scale(20) + Scale(31) + 2 + Scale(10),Scale(20),Scale(20),true);
        MoveWindow(HButtonMinimizeMaximize,OuterRectangle.right - Scale(20) - Scale(19),OuterRectangle.top + Scale(31) + 2 + Scale(10),Scale(20),Scale(20),true);

    }else
    {
        MoveWindow(HButtonIndirect,BrowserRectangle.right + 5, BrowserRectangle.top, Scale(20), Scale(31), true);
        MoveWindow(HButtonDirectRecord,BrowserRectangle.right + 5, BrowserRectangle.top, Scale(20), Scale(31), true);
        MoveWindow(HButtonDirectNoRecord,BrowserRectangle.right + 5, BrowserRectangle.top, Scale(20), Scale(31), true);

        MoveWindow(HButtonSettings,BrowserRectangle.right + 5, BrowserRectangle.top + 2 + Scale(20) + Scale(31) + 2,Scale(20),Scale(20),true);
        MoveWindow(HButtonMinimizeMaximize,BrowserRectangle.right + 5,BrowserRectangle.top + Scale(31) + 2,Scale(20),Scale(20),true);
    }

    MoveWindow(HButtonMenu,Scale(5),Scale(5),Scale(20),Scale(20),true);



}

std::pair<int,int> MainLayout::GetDefaultWindowSize()
{
    std::pair<int,int> res;
    res.first = 1024 + DevToolsRectWidth + 30 + 70 + 3;
    res.second = 600 + ToolBoxRectHeight - 4 + 30 + 120;
    return res;
}

std::pair<int,int> MainLayout::GetDefaultBrowserSize()
{
    std::pair<int,int> res;
    res.first = 1024;
    res.second = 600;
    return res;
}


RECT MainLayout::GetDevToolsRectangle(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight)
{
    RECT DevToolsRectangle, ToolboxRectangle, BrowserRectangle;
    CalculateAllSize(BrowserWidth,BrowserHeight,WindowWidth,WindowHeight,DevToolsRectangle, ToolboxRectangle, BrowserRectangle);
    return DevToolsRectangle;
}

RECT MainLayout::GetToolboxRectangle(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight)
{
    RECT DevToolsRectangle, ToolboxRectangle, BrowserRectangle;
    CalculateAllSize(BrowserWidth,BrowserHeight,WindowWidth,WindowHeight,DevToolsRectangle, ToolboxRectangle, BrowserRectangle);
    return ToolboxRectangle;
}

RECT MainLayout::GetBrowserRectangle(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight)
{
    RECT DevToolsRectangle, ToolboxRectangle, BrowserRectangle;
    CalculateAllSize(BrowserWidth,BrowserHeight,WindowWidth,WindowHeight,DevToolsRectangle, ToolboxRectangle, BrowserRectangle);
    return BrowserRectangle;
}

RECT MainLayout::GetCentralRectangle(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight, bool IsHome)
{
    if(IsHome)
    {
        return GetBrowserOuterRectangle(BrowserWidth,BrowserHeight,WindowWidth,WindowHeight);
    }else
    {
        RECT DevToolsRectangle, ToolboxRectangle, BrowserRectangle;
        CalculateAllSize(BrowserWidth,BrowserHeight,WindowWidth,WindowHeight,DevToolsRectangle, ToolboxRectangle, BrowserRectangle);
        BrowserRectangle.bottom += 30;
        return BrowserRectangle;
    }
}

RECT MainLayout::GetLabelRectangle(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight)
{
    RECT DevToolsRectangle, ToolboxRectangle, BrowserRectangle;
    CalculateAllSize(BrowserWidth,BrowserHeight,WindowWidth,WindowHeight,DevToolsRectangle, ToolboxRectangle, BrowserRectangle);
    RECT res;
    res.left = BrowserRectangle.left + 4 + Scale(20);
    res.right = BrowserRectangle.right - 10 - Scale(20) - 1 - 10 - Scale(20) - Scale(20) - 1 - Scale(20) - Scale(19);
    res.top = BrowserRectangle.bottom + Scale(5) + 1;
    res.bottom = BrowserRectangle.bottom + Scale(16) + Scale(5) + 1;
    return res;
}

RECT MainLayout::GetManualControlIndicatorRectangle(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight)
{
    RECT DevToolsRectangle, ToolboxRectangle, BrowserRectangle;
    CalculateAllSize(BrowserWidth,BrowserHeight,WindowWidth,WindowHeight,DevToolsRectangle, ToolboxRectangle, BrowserRectangle);
    RECT res;
    res.left = BrowserRectangle.left + 1;
    res.right = BrowserRectangle.left + 1 + Scale(20);
    res.top = BrowserRectangle.bottom + 5;
    res.bottom = BrowserRectangle.bottom + 5 + Scale(20);
    return res;
}


void MainLayout::Timer(int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight)
{
    //Manual control animation
    if(IsManualControlAction)
    {
        long long now = duration_cast< milliseconds >( system_clock::now().time_since_epoch() ).count();
        long long delta = now - LastTimeManualChangePicture;
        int interval = 600;
        bool res = delta > interval;
        if(res)
        {
            LastTimeManualChangePicture += (delta / interval) * interval;
            ManualAnimation += (delta / interval);
            ManualAnimation %=4;

            RECT r = GetManualControlIndicatorRectangle(BrowserWidth,BrowserHeight,WindowWidth,WindowHeight);
            InvalidateRect(MainWindowHandle,&r,false);
        }
    }
}


HBITMAP MainLayout::GetManualAnimationButton()
{
    switch(ManualAnimation)
    {
        case 0:return BManualControlAction;
        case 1:return BManualControlAction1;
        case 2:return BManualControlAction2;
        case 3:return BManualControlAction3;
    }
    return BManualControlAction;
}

//Splitter

void MainLayout::SetBrowserCursor(int Cursor)
{
    this->Cursor = Cursor;
}

void MainLayout::SetTouchMode(bool IsTouchMode)
{
    this->IsTouchMode = IsTouchMode;
}

bool MainLayout::IsTouchCursor()
{
    if(IsMoveScrollVertical || IsInsideScrollVertical)
    {
        return false;
    }else if(IsMoveScrollHorizontal || IsInsideScrollHorizontal)
    {
        return false;
    }
    else
    {
        if(IsTouchMode && ManualControl != BrowserData::Indirect && !IsContextMenuShown)
        {
            return true;
        }else
        {
            return false;
        }

    }
    return false;
}

HCURSOR MainLayout::GetCursor(bool& NeedUpdate)
{
    NeedUpdate = true;
    if(IsMoveScrollVertical || IsInsideScrollVertical)
    {
        return hcSizeNS;


    }else if(IsMoveScrollHorizontal || IsInsideScrollHorizontal)
    {
        return hcSizeEW;
    }
    else
    {
        if(IsCursorOverBrowser)
        {
            if(IsTouchMode && ManualControl != BrowserData::Indirect && !IsContextMenuShown)
            {
                return hcArrowTouch;
            }else
            {
                switch(Cursor)
                {
                    case 0: return hcArrow; break;
                    case 1: return hcCross; break;
                    case 2: return hcHand; break;
                    case 5: return hcHelp; break;
                    case 3: return hcIbeam; break;
                    case 4: return hcWait; break;
                    default: return hcArrow;
                }
            }
        }else
        {
            NeedUpdate = false;
            return hcArrow;
        }
    }
    NeedUpdate = false;
    return hcArrow;
}


bool MainLayout::OnMouseMove(int x, int y, int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight)
{
    if(!IsRecord)
        return false;

    if(!IsContextMenuShown)
    {
        if(!IsImageSelect)
        {
            RECT r = GetBrowserRectangle(BrowserWidth,BrowserHeight,WindowWidth,WindowHeight);

            ImageSelectEndX = -1;
            ImageSelectEndY = -1;

            if(x > r.left && x < r.right && y > r.top && y < r.bottom)
            {
                ImageSelectStartX = x - r.left;
                ImageSelectStartY = y - r.top;
            }else
            {
                ImageSelectStartX = -1;
                ImageSelectStartY = -1;
            }

        }else
        {

            RECT r = GetBrowserRectangle(BrowserWidth,BrowserHeight,WindowWidth,WindowHeight);
            if(x > r.left && x < r.right && y > r.top && y < r.bottom)
            {
                ImageSelectEndX = x - r.left;
                ImageSelectEndY = y - r.top;
            }else
            {
                IsImageSelect = false;
                ImageSelectStartX = -1;
                ImageSelectStartY = -1;
                ImageSelectEndX = -1;
                ImageSelectEndY = -1;
            }
        }
    }

    SplitterX = x;
    SplitterY = y;

    if(x > WindowWidth - 20 || y > WindowHeight - 20 || x < 20 || y < 20)
    {
        return OnMouseLeave();
    }

    if(IsMoveScrollVertical)
    {
        ToolBoxRectHeight = y;
        if(MainWindowHandle)
            InvalidateRect(MainWindowHandle,NULL,true);
    }

    if(IsMoveScrollHorizontal)
    {
        DevToolsRectWidth = x;
        if(MainWindowHandle)
            InvalidateRect(MainWindowHandle,NULL,true);

    }

    if(x >= DevToolsRectWidth - 2 && x <= DevToolsRectWidth + 5)
    {
        IsInsideScrollHorizontal = true;
        IsInsideScrollVertical = false;
    }else if(y >= ToolBoxRectHeight - 3 && y <= ToolBoxRectHeight + 3 && x > DevToolsRectWidth - 2)
    {
        IsInsideScrollHorizontal = false;
        IsInsideScrollVertical = true;
    }
    else
    {
        IsInsideScrollHorizontal = false;
        IsInsideScrollVertical = false;
    }
    return false;
}

bool MainLayout::OnMouseLeave()
{
    IsInsideScrollVertical = false;
    IsInsideScrollHorizontal = false;

    if(!IsRecord)
        return false;

    IsImageSelect = false;

    if(!IsMoveScrollVertical && !IsMoveScrollHorizontal)
        return false;
    if(IsMoveScrollHorizontal)
        DevToolsRectWidth = DevToolsRectWidthStart;
    if(IsMoveScrollVertical)
        ToolBoxRectHeight = ToolBoxRectHeightStart;
    return OnMouseUp();

}

bool MainLayout::OnMouseUp()
{
    if(!IsRecord)
        return false;

    IsImageSelectLast = GetIsImageSelect();
    IsImageSelect = false;

    if(!IsMoveScrollVertical && !IsMoveScrollHorizontal)
        return false;

    bool NeedUpdate = SplitterIsChangingSize();
    IsMoveScrollHorizontal = false;
    IsMoveScrollVertical = false;
    SplitterShowInterface();

    return NeedUpdate;
}
bool MainLayout::OnMouseDown(int x, int y, int BrowserWidth,int BrowserHeight,int WindowWidth,int WindowHeight, bool IsControlButton)
{
    if(!IsRecord)
        return false;

    if(IsInsideScrollHorizontal)
    {
        IsMoveScrollHorizontal = true;
        DevToolsRectWidthStart = DevToolsRectWidth;
        SplitterHideInterface();

    }

    if(IsInsideScrollVertical)
    {
        IsMoveScrollVertical = true;
        ToolBoxRectHeightStart = ToolBoxRectHeight;
        SplitterHideInterface();

    }
    if(IsControlButton)
    {
        if(!IsImageSelect)
        {
            RECT r = GetBrowserRectangle(BrowserWidth,BrowserHeight,WindowWidth,WindowHeight);

            ImageSelectEndX = -1;
            ImageSelectEndY = -1;

            if(x > r.left && x < r.right && y > r.top && y < r.bottom)
            {
                ImageSelectStartX = x - r.left;
                ImageSelectStartY = y - r.top;
            }else
            {
                ImageSelectStartX = -1;
                ImageSelectStartY = -1;
            }

            if(ImageSelectStartX >= 0 && ImageSelectStartY >= 0)
            {
                IsImageSelect = true;
                ImageSelectEndX = ImageSelectStartX;
                ImageSelectEndY = ImageSelectStartY;
            }
        }
    }

    return false;
}

void MainLayout::SplitterHideInterface()
{
    if(IsContextMenuShown)
        HideCentralBrowser();

    if(ToolBoxHandle)
        ShowWindow(ToolBoxHandle,SW_HIDE);

    if(ScenarioHandle)
        ShowWindow(ScenarioHandle,SW_HIDE);

    if(DetectorHandle)
        ShowWindow(DetectorHandle,SW_HIDE);

    if(CentralHandle)
        ShowWindow(CentralHandle,SW_HIDE);

    GetDevToolsHandle();
    if(DevToolsHandle)
        ShowWindow(DevToolsHandle,SW_HIDE);

    ShowWindow(HButtonIndirect,SW_HIDE);
    ShowWindow(HButtonDirectRecord,SW_HIDE);
    ShowWindow(HButtonDirectNoRecord,SW_HIDE);

    if(HButtonSettings)
        ShowWindow(HButtonSettings,SW_HIDE);
    if(HButtonMenu)
        ShowWindow(HButtonMenu,SW_HIDE);


    if(HButtonUp)
        ShowWindow(HButtonUp,SW_HIDE);
    if(HLabelBrowserBottom)
        ShowWindow(HLabelBrowserBottom,SW_HIDE);
    if(HLabelTop)
        ShowWindow(HLabelTop,SW_HIDE);
    if(HButtonDown)
        ShowWindow(HButtonDown,SW_HIDE);
    if(HButtonLeft)
        ShowWindow(HButtonLeft,SW_HIDE);
    if(HButtonRight)
        ShowWindow(HButtonRight,SW_HIDE);

    if(HButtonUpUp)
        ShowWindow(HButtonUpUp,SW_HIDE);
    if(HEditUrl)
        ShowWindow(HEditUrl,SW_HIDE);
    if(HLoadUrl)
        ShowWindow(HLoadUrl,SW_HIDE);
    if(HBackUrl)
        ShowWindow(HBackUrl,SW_HIDE);
    if(HBrowserTabs)
        ShowWindow(HBrowserTabs,SW_HIDE);
    if(HBrowserMenu)
        ShowWindow(HBrowserMenu,SW_HIDE);
    if(HButtonDownDown)
        ShowWindow(HButtonDownDown,SW_HIDE);
    if(HButtonLeftLeft)
        ShowWindow(HButtonLeftLeft,SW_HIDE);
    if(HButtonRightRight)
        ShowWindow(HButtonRightRight,SW_HIDE);

    if(HButtonMinimizeMaximize)
        ShowWindow(HButtonMinimizeMaximize,SW_HIDE);

    if(MainWindowHandle)
        InvalidateRect(MainWindowHandle,NULL,true);
}

void MainLayout::SplitterShowInterface()
{
    if(ToolBoxHandle)
        ShowWindow(ToolBoxHandle,SW_SHOW);

    if(ScenarioHandle)
        ShowWindow(ScenarioHandle,(_TabType == Scenario)?SW_SHOW:SW_HIDE);

    if(DetectorHandle)
        ShowWindow(DetectorHandle,(_TabType == Detector)?SW_SHOW:SW_HIDE);

    if(CentralHandle)
        ShowWindow(CentralHandle,(IsCentralShown || IsContextMenuShown)?SW_SHOW:SW_HIDE);

    GetDevToolsHandle();
    if(DevToolsHandle)
        ShowWindow(DevToolsHandle,(_TabType == Devtools)?SW_SHOW:SW_HIDE);

    if(HLabelBrowserBottom)
        ShowWindow(HLabelBrowserBottom,((IsManualControlAction) || (!IsCentralShown && !IsSettingsShown && !IsManualSelectShown))?SW_SHOW:SW_HIDE);
    if(HLabelTop)
        ShowWindow(HLabelTop,SW_SHOW);
    if(HButtonUp)
        ShowWindow(HButtonUp,(!IsCentralShown && !IsSettingsShown && !IsManualSelectShown)?SW_SHOW:SW_HIDE);
    if(HButtonDown)
        ShowWindow(HButtonDown,(!IsCentralShown && !IsSettingsShown && !IsManualSelectShown)?SW_SHOW:SW_HIDE);
    if(HButtonLeft)
        ShowWindow(HButtonLeft,(!IsCentralShown && !IsSettingsShown && !IsManualSelectShown)?SW_SHOW:SW_HIDE);
    if(HButtonRight)
        ShowWindow(HButtonRight,(!IsCentralShown && !IsSettingsShown && !IsManualSelectShown)?SW_SHOW:SW_HIDE);

    if(HEditUrl)
        ShowWindow(HEditUrl,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);

    if(HBackUrl)
        ShowWindow(HBackUrl,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);

    if(HBrowserTabs)
        ShowWindow(HBrowserTabs,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);

    if(HBrowserMenu)
        ShowWindow(HBrowserMenu,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);

    if(HLoadUrl)
        ShowWindow(HLoadUrl,(ManualControl != BrowserData::Indirect && (!IsCentralShown || IsNoDataShown))?SW_SHOW:SW_HIDE);

    if(HButtonUpUp)
        ShowWindow(HButtonUpUp,(!IsCentralShown && !IsSettingsShown && !IsManualSelectShown)?SW_SHOW:SW_HIDE);
    if(HButtonDownDown)
        ShowWindow(HButtonDownDown,(!IsCentralShown && !IsSettingsShown && !IsManualSelectShown)?SW_SHOW:SW_HIDE);
    if(HButtonLeftLeft)
        ShowWindow(HButtonLeftLeft,(!IsCentralShown && !IsSettingsShown && !IsManualSelectShown)?SW_SHOW:SW_HIDE);
    if(HButtonRightRight)
        ShowWindow(HButtonRightRight,(!IsCentralShown && !IsSettingsShown && !IsManualSelectShown)?SW_SHOW:SW_HIDE);

    if(!IsSettingsShown && !IsManualSelectShown)
    {
        if(ManualControl == BrowserData::Indirect)
        {
            ShowWindow(HButtonIndirect,SW_SHOW);
            ShowWindow(HButtonDirectRecord,SW_HIDE);
            ShowWindow(HButtonDirectNoRecord,SW_HIDE);
        }
        if(ManualControl == BrowserData::DirectRecord)
        {
            ShowWindow(HButtonIndirect,SW_HIDE);
            ShowWindow(HButtonDirectRecord,SW_SHOW);
            ShowWindow(HButtonDirectNoRecord,SW_HIDE);
        }
        if(ManualControl == BrowserData::DirectNoRecord)
        {
            ShowWindow(HButtonIndirect,SW_HIDE);
            ShowWindow(HButtonDirectRecord,SW_HIDE);
            ShowWindow(HButtonDirectNoRecord,SW_SHOW);
        }
    }else
    {
        ShowWindow(HButtonIndirect,SW_HIDE);
        ShowWindow(HButtonDirectRecord,SW_HIDE);
        ShowWindow(HButtonDirectNoRecord,SW_HIDE);
    }

    if(HButtonSettings)
        ShowWindow(HButtonSettings,(!IsSettingsShown && !IsManualSelectShown)?SW_SHOW:SW_HIDE);

    if(HButtonMinimizeMaximize)
        ShowWindow(HButtonMinimizeMaximize,(!IsSettingsShown && !IsManualSelectShown)?SW_SHOW:SW_HIDE);


    if(HButtonMenu)
        ShowWindow(HButtonMenu,SW_SHOW);


    if(MainWindowHandle)
        InvalidateRect(MainWindowHandle,NULL,true);


}

bool MainLayout::SplitterIsChangingSize()
{
    return IsMoveScrollVertical || IsMoveScrollHorizontal;
}


