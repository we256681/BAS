#include <windows.h>
#include "fileexists.h"
#include <thread>
#include "converter.h"
#include "mainapp.h"
#include "include/cef_app.h"
#include "pipesclient.h"
#include "commandparser.h"
#include "log.h"
#include "processcheck.h"
#include "browserdata.h"
#include "resource.h"
#include "mainlayout.h"
#include "settings.h"
#include "translate.h"
#include "opensslmultithreaded.h"
#include "multithreading.h"
#include "modulesdata.h"
#include "postmanager.h"
#include "devtoolsrequest2action.h"
#include "gToolTip.h"
#include "generatejsonmenu.h"
#include "startwith.h"
#include "sharedmemoryipc.h"
#include <fstream>
#include "emptyapp.h"
#include "renderapp.h"
#include "fileutils.h"
#include "prepareurladressbar.h"
#include "proxyconfigreplace.h"
#include <iostream>
#include <fstream>
#include "ipcsimple.h"
#include "rawcpphttpclientfactory.h"
#include "rawcppwebsocketclientfactory.h"
#include "preparestartupscript.h"
#include "chromecommandlineparser.h"
#include "mixnumbers.h"
#include "installcomponents.h"
#include "createemptyprofile.h"
#include "readallfile.h"
#include "base64.h"
#include "writefile.h"
#include "popupemulation.h"
#include "noneconnector.h"
#include "devtoolsconnector.h"


#if defined(BAS_DEBUG)
    #include "CrashHandler.h"
#endif

int pid = -1;
CefRefPtr<MainApp> app;
PipesClient *Client;
PopupEmulation *_PopupEmulation;
CommandParser *Parser;
HWND MousePositionMouseHandle,hwnd,HButtonUp,HButtonDown,HButtonLeft,HButtonRight,HButtonMenu;
HWND HButtonUpUp,HButtonDownDown,HButtonLeftLeft,HButtonRightRight;
enum{IDButtonTerminate = 1000,IDButtonQuit,IDButtonUp,IDButtonBackUrl,IDBrowserTabs,IDBrowserMenu,IDButtonLoadUrl,IDButtonDown,IDButtonLeft,IDButtonRight,IDButtonUpUp,IDButtonDownDown,IDButtonLeftLeft,IDButtonRightRight,IDButtonMinimizeMaximize,IDButtonMenu,IDButtonSettings,IDButtonDirectRecord,IDButtonDirectNoRecord,IDButtonIndirect,IDTextHold,IDBrowserLabel,IDLabelTop,IDTextFinished,IDClick,IDMove,IDNone,IDMoveAndClick,IDDrag,IDDrop,IDDragElement,IDDropElement,IDInspect,IDXml,IDText,IDScript,IDClickElement,IDMoveElement,IDMoveAndClickElement,IDClear,IDType,IDExists,IDStyle,IDCheck,IDScreenshot,IDCoordinates,IDFocus,IDSet,IDSetInteger,IDSetRandom,IDGetAttr,IDSetAttr,IDCaptcha,IDLength,IDWaitElement,
    IDLoop,IDXmlLoop,IDTextLoop,IDScriptLoop,IDClickElementLoop,IDMoveElementLoop,IDMoveAndClickElementLoop,IDClearLoop,IDTypeLoop,IDExistsLoop,IDStyleLoop,IDCheckLoop,IDScreenshotLoop,IDCoordinatesLoop,IDFocusLoop,IDSetLoop,IDSetIntegerLoop,IDSetRandomLoop,IDGetAttrLoop,IDSetAttrLoop,IDCaptchaLoop,IDAddTabManual,IDShowUpdater,IDShowScenario,IDShowDevtools,IDShowFingerprintDetector,IDRecordHttpRequests,IDCustom = 30000,IDCustomForeach = 40000,IDCustomPopups = 50000, IDManualTabSwitch = 50000, IDManualTabClose = 60000, IDPopupEmulation = 60500};
HCURSOR HCursor = 0;
HCURSOR HCursorTouch = 0;
using namespace std::placeholders;
HMENU hPopupMenu = 0;
HMENU hTabsManualMenu = 0;
HMENU hMenu = 0;
HMENU hForEachMenu = 0;
HINSTANCE hInst;
MainLayout *Layout;
int MouseMenuPositionX,MouseMenuPositionY;
int InspectLastX = -1;
int InspectLastY = -1;
int LastMousePositionX;
int LastMousePositionY;
std::string LastLabel;
settings Settings;
int TimerLoop = 0;
HHOOK KeyHook = 0;
bool SetFocusForUrl = false;
bool IsCloseAsk = false;
HFONT InspectFont;
int LastMousePositionRawX = 0;
int LastMousePositionRawY = 0;
int ElementTweakUpOrDownUsed = 0;
bool ElementTweakReturnUsed = false;
UINT uFindReplaceMsg = 0;
std::string PidGlobal;
std::string KeyGlobal;


void TerminateOnCloseMutex(const std::string& Id, bool DoSleep, bool DoFlush)
{
    HANDLE HandleMutex = OpenMutexA(MUTEX_ALL_ACCESS,false,Id.c_str());

    if(HandleMutex)
        WaitForSingleObject(HandleMutex,INFINITE);

    ReleaseMutex(HandleMutex);
    CloseHandle(HandleMutex);


    if(DoFlush)
    {
        app->FlushCallback();
    }

    if(DoSleep)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000 + rand()%5000));
    }

    exit(0);
}

void RestoreOriginalStage()
{
    Layout->DevToolsRectWidth = 500;
    Layout->ToolBoxRectHeight = 300;
    Layout->Update(app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll);
    InvalidateRect(Layout->MainWindowHandle,NULL,true);

    Settings.SetToolboxHeight(Layout->ToolBoxRectHeight);
    Settings.SetScenarioWidth(Layout->DevToolsRectWidth);
    Settings.SaveToFile();
}

void SetMode(const std::string& mode)
{
    app->GetData()->IsTouchScreen = mode == "mobile";
    Layout->IsTouchMode = app->GetData()->IsTouchScreen;
    app->SendTextResponce("<SetMode/>");
}

std::string GetUrl()
{
    int Size = GetWindowTextLength(Layout->HEditUrl);
    if(Size == 0)
        return std::string();
    std::vector<wchar_t> UrlData(Size + 1);
    GetWindowText(Layout->HEditUrl, UrlData.data(), Size + 1);
    std::wstring Url(UrlData.begin(),UrlData.begin() + Size);
    return ws2s(Url);
}

bool IsUrlEditSelected()
{
    return GetFocus() == Layout->HEditUrl;
}



void CreateHMenu(int HighlightedMenuItem = -1)
{
    if(hMenu)
    {
        DestroyMenu(hMenu);
        hMenu = 0;
    }

    hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_BYPOSITION | MF_STRING, IDShowScenario, Translate::Tr(L"Script editor").c_str());
    AppendMenu(hMenu, MF_BYPOSITION | MF_STRING, IDShowDevtools, Translate::Tr(L"Chrome developer tools").c_str());
    std::wstring DetectorText = Translate::Tr(L"Detect site fingerprinting activities");
    DetectorText += std::wstring(L" (");
    DetectorText += std::to_wstring(Layout->GetFingerprintDetectorNumber());
    DetectorText += std::wstring(L")");
    AppendMenu(hMenu, MF_BYPOSITION | MF_STRING, IDShowFingerprintDetector, DetectorText.c_str());
    AppendMenu(hMenu, MF_SEPARATOR,NULL,L"Separator");
    AppendMenu(hMenu, MF_BYPOSITION | MF_STRING, IDRecordHttpRequests, Translate::Tr(L"Http request recorder").c_str());
    AppendMenu(hMenu, MF_SEPARATOR,NULL,L"Separator");
    AppendMenu(hMenu, MF_BYPOSITION | MF_STRING, IDShowUpdater, Translate::Tr(L"Update project actions").c_str());

    if(app->GetData()->IsRecordHttp)
        CheckMenuItem(hMenu, IDRecordHttpRequests, MF_CHECKED);
    else
        CheckMenuItem(hMenu, IDRecordHttpRequests, MF_UNCHECKED);

    //SetMenuItemBitmaps(hMenu, 1, MF_BITMAP|MF_BYPOSITION, Layout->ButtonDevToolsBitmap, Layout->ButtonDevToolsBitmapGray );

    POINT p;
    p.x = -4;
    p.y = Layout->Scale(20) + Layout->Scale(5) + 1;
    ClientToScreen(Layout->HButtonMenu,&p);
    if (HighlightedMenuItem > -1) HiliteMenuItem(hwnd, hMenu, HighlightedMenuItem, MF_BYCOMMAND | MF_HILITE);
    TrackPopupMenu(hMenu, TPM_TOPALIGN | TPM_LEFTALIGN, p.x, p.y, 0, hwnd, NULL);
    if(hMenu)
    {
        DestroyMenu(hMenu);
        hMenu = 0;
    }
}

void HighlightHMenu(const std::string& Item)
{
    if (Item == std::string("ShowDevtools"))
    {
        CreateHMenu(IDShowDevtools);
    }
    if (Item == std::string("ShowScenario"))
    {
        CreateHMenu(IDShowScenario);
    }
    if (Item == std::string("ShowUpdater"))
    {
        CreateHMenu(IDShowUpdater);
    }
}

void ProcessMenu(const std::string& Command)
{
    Layout->HideCentralBrowser();
    int IdIterator = 0;
    int IdIteratorForeach = 0;
    for(ModulesData Module:app->GetData()->_ModulesData)
    {
        for(ActionData Action:Module->Actions)
        {
            if(Action->IsElement)
            {

                 if(Command == std::string("IDModules") + std::to_string(IdIterator))
                 {
                     if(Layout->State == MainLayout::Ready)
                     {
                        app->ExecuteElementFunction(Action->Name,app->GetData()->MultiselectIsInsideElementLoop);
                     }
                 }
                 IdIterator ++;
                 IdIteratorForeach ++;
            }
        }
    }
    for(ModulesData Module:app->GetData()->_UnusedModulesData)
    {
        for(ActionData Action:Module->Actions)
        {
            if(Action->IsElement)
            {

                 if(Command == std::string("IDModules") + std::to_string(IdIterator))
                 {
                     if(Layout->State == MainLayout::Ready)
                     {
                        app->ExecuteElementFunction(Action->Name,app->GetData()->MultiselectIsInsideElementLoop, true, Module->Name, Module->Description);
                     }
                 }
                 IdIterator ++;
                 IdIteratorForeach ++;
            }
        }
    }

    if(starts_with(Command,"IDTab"))
    {
        std::string CommandCopy = Command;
        CommandCopy.erase(0,5);
        int diff = std::stoi(CommandCopy);

        if(diff == 0)
            app->AddTab();
        else if(diff == 1)
            app->SelectTab(0);
        else
        {
            if(diff % 2 == 0)
                app->SelectTab(diff / 2);
            if(diff % 2 == 1)
                app->CloseTab(diff / 2);
        }
    }

    if(starts_with(Command,"IDInfoTab"))
    {
        if(Layout->State == MainLayout::Ready)
        {
            app->TabInfo();
        }
    }

    if(Command == "IDManualControl")
    {
        if(Layout->State == MainLayout::Ready)
        {
            Layout->ShowManualSelect();
            app->LoadManualSelect();
            return;
        }
    }
    if(Command == "IDClick")
    {
        if(Layout->State == MainLayout::Ready)
        {
            app->EmulateClick(MouseMenuPositionX,MouseMenuPositionY);
            return;
        }
    }
    if(Command == "IDMove")
    {
        if(Layout->State == MainLayout::Ready)
        {
            app->EmulateMove(MouseMenuPositionX,MouseMenuPositionY);
            return;
        }
    }

    if(Command == "IDDrag")
    {
        if(Layout->State == MainLayout::Ready)
        {
            app->EmulateDrag(MouseMenuPositionX,MouseMenuPositionY);
            return;
        }
    }

    if(Command == "IDDrop"){
        if(Layout->State == MainLayout::Ready)
        {
            app->EmulateDrop(MouseMenuPositionX,MouseMenuPositionY);
            return;
        }
    }

    if(Command == "IDMoveAndClick"){
        if(Layout->State == MainLayout::Ready)
        {
            app->EmulateMoveAndClick(MouseMenuPositionX,MouseMenuPositionY);
            return;
        }
    }

    if(Command == "IDInspect"){
        if(InspectLastX >= 0 && InspectLastY >= 0)
        {
            app->InspectAt(MouseMenuPositionX - app->GetData()->ScrollX,MouseMenuPositionY - app->GetData()->ScrollY);
            InspectLastX = -1;
            InspectLastY = -1;
        }
    }

    if(Command == "IDXml"){
        if(Layout->State == MainLayout::Ready)
        {
            app->ExecuteElementFunction("GetXml",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDText"){
        if(Layout->State == MainLayout::Ready)
        {
            app->ExecuteElementFunction("GetText",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDScript"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("ExecuteScript",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDClickElement"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("ClickElement",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }


    if(Command == "IDMoveElement"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("MoveElement",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDDragElement"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("DragElement", false);
        }
    }

    if(Command == "IDDropElement"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("DropElement", false);
        }
    }

    if(Command == "IDMoveAndClickElement"){
        if(Layout->State == MainLayout::Ready)
        {
            app->ExecuteElementFunction("MoveAndClickElement",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDClear"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("Clear",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDType"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("Type",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDExists"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("Exists",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDStyle"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("Style",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDCheck"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("Check",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDScreenshot"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("Screenshot",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDCoordinates"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("GetCoordinates",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDFocus"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("Focus",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDSet"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("Set",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDSetInteger"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("SetInteger",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDSetRandom"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("SetRandom",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDGetAttr"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("GetAttr",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDGetLinkURL"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("GetLinkURL",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDSetAttr"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("SetAttr",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDCaptcha"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("Captcha",app->GetData()->MultiselectIsInsideElementLoop);
        }
    }

    if(Command == "IDWaitElement"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("Wait", false);
        }
    }

    if(Command == "IDLength"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("Length", false);
        }
    }

    if(Command == "IDLoop"){
        if(Layout->State == MainLayout::Ready)
        {
        app->ExecuteElementFunction("LoopElement", false);
        }
    }
}

LRESULT CALLBACK UrlEditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    if(msg == WM_CHAR && wParam == VK_RETURN)
    {
        app->SetFocusOnNextLoad();
        std::string Url = prepare_url_adressbar(GetUrl());
        app->DirectControl()->Load(Url);
        return true;
    }
    if(msg == WM_LBUTTONDBLCLK)
    {
        auto ret = DefSubclassProc(hwnd, msg, wParam, lParam);
        SendMessage(Layout->HEditUrl, EM_SETSEL, 0, -1);
        return ret;
    }


    return DefSubclassProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    if((HWND)lParam == Layout->HEditUrl && Layout->HEditUrl != 0)
    {
        if(msg == WM_CTLCOLOREDIT)
        {
            HBRUSH hbr = (HBRUSH) DefWindowProc(hwnd, msg, wParam, lParam);
            SetTextColor((HDC) wParam, RGB(60, 60, 60));
            return (LRESULT)hbr;
        }else
        {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }

    }

    int IsControlButton = -1;
    bool IsLeftMouseButton = true;

    if(msg == uFindReplaceMsg)
    {
        LPFINDREPLACE lpfr = (LPFINDREPLACE)lParam;
        app->ProcessFind(lpfr);

        return 0;
    }

    switch(msg)
    {
        case WM_CREATE:
        {

            uFindReplaceMsg = RegisterWindowMessage(FINDMSGSTRING);
            Layout->hcSizeNS = LoadCursor(NULL, IDC_SIZENS);
            Layout->hcSizeEW = LoadCursor(NULL, IDC_SIZEWE);
            Layout->hcArrow = LoadCursor(NULL, IDC_ARROW);
            Layout->hcArrowTouch = LoadCursor(hInst, MAKEINTRESOURCEW(IDB_TOUCHCURSOR));

            Layout->hcCross = LoadCursor(NULL, IDC_CROSS);
            Layout->hcHand = LoadCursor(NULL, IDC_HAND);
            Layout->hcHelp = LoadCursor(NULL, IDC_HELP);
            Layout->hcIbeam = LoadCursor(NULL, IDC_IBEAM);
            Layout->hcIcon = LoadCursor(NULL, IDC_ICON);
            Layout->hcWait = LoadCursor(NULL, IDC_WAIT);

            if(app->GetData()->IsRecord)
            {
                /*HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_LOGO));
                SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);*/
                HButtonUp = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON, 0, 0, 20, 20, hwnd, (HMENU)IDButtonUp, hInst, NULL);
                Layout->HButtonUp = HButtonUp;
                {
                    HBITMAP bitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_UP), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
                    SendMessage(HButtonUp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
                }
                HButtonDown = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON, 0,0 , 20, 20, hwnd, (HMENU)IDButtonDown, hInst, NULL);
                Layout->HButtonDown = HButtonDown;
                {
                    HBITMAP bitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_DOWN), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
                    SendMessage(HButtonDown, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
                }
                HButtonLeft = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON, 0,0 , 20, 20, hwnd, (HMENU)IDButtonLeft, hInst, NULL);
                Layout->HButtonLeft = HButtonLeft;
                {
                    HBITMAP bitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_LEFT), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
                    SendMessage(HButtonLeft, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
                }
                HButtonRight = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON, 0,0 , 20, 20, hwnd, (HMENU)IDButtonRight, hInst, NULL);
                Layout->HButtonRight = HButtonRight;
                {
                    HBITMAP bitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_RIGHT), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
                    SendMessage(HButtonRight, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
                }

                HButtonUpUp = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON, 0, 0, 20, 20, hwnd, (HMENU)IDButtonUpUp, hInst, NULL);
                Layout->HButtonUpUp = HButtonUpUp;
                {
                    HBITMAP bitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_UPUP), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
                    SendMessage(HButtonUpUp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
                }
                HButtonDownDown = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON, 0,0 , 20, 20, hwnd, (HMENU)IDButtonDownDown, hInst, NULL);
                Layout->HButtonDownDown = HButtonDownDown;
                {
                    HBITMAP bitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_DOWNDOWN), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
                    SendMessage(HButtonDownDown, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
                }
                HButtonLeftLeft = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON, 0,0 , 20, 20, hwnd, (HMENU)IDButtonLeftLeft, hInst, NULL);
                Layout->HButtonLeftLeft = HButtonLeftLeft;
                {
                    HBITMAP bitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_LEFTLEFT), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
                    SendMessage(HButtonLeftLeft, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
                }
                HButtonRightRight = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON, 0,0 , 20, 20, hwnd, (HMENU)IDButtonRightRight, hInst, NULL);
                Layout->HButtonRightRight = HButtonRightRight;
                {
                    HBITMAP bitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_RIGHTRIGHT), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
                    SendMessage(HButtonRightRight, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
                }


                HButtonMenu = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON, 0,0, 20, 20, hwnd, (HMENU)IDButtonMenu, hInst, NULL);
                Layout->HButtonMenu = HButtonMenu;
                {
                    HBITMAP bitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_MENU), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
                    SendMessage(Layout->HButtonMenu, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
                }

                Layout->HButtonSettings = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON, 0,0, 20, 20, hwnd, (HMENU)IDButtonSettings, hInst, NULL);
                {
                    HBITMAP bitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_SETTINGS), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
                    SendMessage(Layout->HButtonSettings, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
                }

                Layout->HButtonIndirect = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON, 0,0, 20, 20, hwnd, (HMENU)IDButtonIndirect, hInst, NULL);
                {
                    HBITMAP bitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_INDIRECT), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(31), 0);
                    SendMessage(Layout->HButtonIndirect, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
                }
                gToolTip::AddTip(hwnd,hInst,(TCHAR *)Translate::Tr(L"Use this button to start control browser directly.").data(),IDButtonIndirect,TRUE);

                Layout->HButtonDirectRecord = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_CHILD|BS_PUSHBUTTON, 0,0, 20, 20, hwnd, (HMENU)IDButtonDirectRecord, hInst, NULL);
                {
                    HBITMAP bitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_DIRECT_RECORD), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(31), 0);
                    SendMessage(Layout->HButtonDirectRecord, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
                }
                gToolTip::AddTip(hwnd,hInst,(TCHAR *)Translate::Tr(L"Use this button to start control browser directly.").data(),IDButtonDirectRecord,TRUE);

                Layout->HButtonDirectNoRecord = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_CHILD|BS_PUSHBUTTON, 0,0, 20, 20, hwnd, (HMENU)IDButtonDirectNoRecord, hInst, NULL);
                {
                    HBITMAP bitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_DIRECT_NO_RECORD), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(31), 0);
                    SendMessage(Layout->HButtonDirectNoRecord, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
                }
                gToolTip::AddTip(hwnd,hInst,(TCHAR *)Translate::Tr(L"Use this button to start control browser directly.").data(),IDButtonDirectNoRecord,TRUE);

                Layout->ButtonMinimize = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_MINIMIZE), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
                Layout->ButtonMaximize = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_MAXIMIZE), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
                Layout->HButtonMinimizeMaximize = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON, 0,0, 20, 20, hwnd, (HMENU)IDButtonMinimizeMaximize, hInst, NULL);

                InspectFont = CreateFont (Layout->Scale(14), 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");

            }

            Layout->BManualControlAction = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_MANUALCONTROLACTION), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
            Layout->BManualControlAction1 = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_MANUALCONTROLACTION1), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
            Layout->BManualControlAction2 = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_MANUALCONTROLACTION2), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
            Layout->BManualControlAction3 = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_MANUALCONTROLACTION3), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);

            Layout->HEditUrl = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"about:blank", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,0,0,100,100,hwnd,(HMENU)NULL, NULL, NULL);
            app->GetData()->UrlHandler = Layout->HEditUrl;

            SetWindowSubclass(Layout->HEditUrl, UrlEditProc, 0,0);

            {
                HFONT hFont = CreateFont (Layout->Scale(16), 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
                if(hFont)
                {
                    SendMessage (Layout->HEditUrl, WM_SETFONT, WPARAM (hFont), TRUE);
                }
            }

            Layout->HLoadUrl = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON, 0,0 , 20, 20, hwnd, (HMENU)IDButtonLoadUrl, hInst, NULL);
            {
                HBITMAP bitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_LOADURL), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
                SendMessage(Layout->HLoadUrl, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
            }

            Layout->HBackUrl = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON, 0,0 , 20, 20, hwnd, (HMENU)IDButtonBackUrl, hInst, NULL);
            {
                HBITMAP bitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_BACKURL), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
                SendMessage(Layout->HBackUrl, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
            }

            Layout->HBrowserTabs = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON, 0,0 , 20, 20, hwnd, (HMENU)IDBrowserTabs, hInst, NULL);
            {
                HBITMAP bitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_BROWSERTABS), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
                SendMessage(Layout->HBrowserTabs, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
            }

            Layout->HBrowserMenu = CreateWindow(L"BUTTON", NULL, BS_BITMAP|WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON, 0,0 , 20, 20, hwnd, (HMENU)IDBrowserMenu, hInst, NULL);
            {
                HBITMAP bitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCEW(IDB_BROWSERMENU), IMAGE_BITMAP, Layout->Scale(20), Layout->Scale(20), 0);
                SendMessage(Layout->HBrowserMenu, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
            }

            Layout->HLabelBrowserBottom = CreateWindow(L"SysLink", L"", WS_CHILD, 0, 0, 100, 30, hwnd, (HMENU)IDBrowserLabel, hInst, NULL);
            {
                HFONT hFont = CreateFont (Layout->Scale(14), 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
                if(hFont)
                {
                    SendMessage (Layout->HLabelBrowserBottom, WM_SETFONT, WPARAM (hFont), TRUE);
                }
            }

            if(app->GetData()->IsRecord)
            {
                Layout->HLabelTop = CreateWindow(L"STATIC", L"", WS_CHILD, 0, 0, 100, 30, hwnd, (HMENU)IDLabelTop, hInst, NULL);

                Layout->SetLabelTop(Translate::Tr(L"Script editor"));

                {
                    long s = GetWindowLongPtr(Layout->HLabelTop,GWL_STYLE);
                    s =  ( s & ~( SS_LEFT | SS_CENTER | SS_RIGHT | SS_LEFTNOWORDWRAP ) ) | SS_LEFT | SS_CENTERIMAGE;
                    SetWindowLongPtr(Layout->HLabelTop,GWL_STYLE,(LONG_PTR)s);
                }

                HFONT hFont = CreateFont (Layout->Scale(16), 0, 0, 0, FW_REGULAR, TRUE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
                if(hFont)
                {
                    SendMessage (Layout->HLabelTop, WM_SETFONT, WPARAM (hFont), TRUE);
                }
            }

            HCursor = LoadCursor(NULL, IDC_ARROW);
            HCursorTouch = LoadCursor(hInst, MAKEINTRESOURCEW(IDB_TOUCHCURSOR));


        }
        break;
        case WM_NOTIFY:
        {

            switch (((LPNMHDR)lParam)->code)
            {
                case NM_CLICK:

                case NM_RETURN:
                {
                    if(Layout->IsManualControlAction)
                    {
                        app->GetData()->ManualControl = BrowserData::Indirect;

                        app->UpdateManualControl();
                    }
                    break;
                }
            }

        }
        break;
        case WM_DRAWITEM:
        {
            /*HBITMAP hBmp;
            LPDRAWITEMSTRUCT lpDIS = LPDRAWITEMSTRUCT(lParam);
            RECT rectBtn;
            rectBtn.bottom = lpDIS->rcItem.bottom;
            rectBtn.top = lpDIS->rcItem.top;
            rectBtn.left = lpDIS->rcItem.left;
            rectBtn.right = lpDIS->rcItem.right;
            HDC hdcBmp = CreateCompatibleDC(lpDIS->hDC);

            int ImageId = IDB_DETECTOR_ONE;
            std::wstring Text = std::to_wstring(Layout->GetFingerprintDetectorNumber());
            int Offset = 24;

            if(Layout->GetFingerprintDetectorNumber() < 0)
            {
                ImageId = IDB_DETECTOR_ONE;
                Text = L"?";
            }else if(Layout->GetFingerprintDetectorNumber() < 10)
            {
                ImageId = IDB_DETECTOR_ONE;
            }else if(Layout->GetFingerprintDetectorNumber() < 100)
            {
                Offset = 22;
                ImageId = IDB_DETECTOR_TWO;
            }else
            {
                Offset = 21;
                ImageId = IDB_DETECTOR_THREE;
                Text = L"99+";
            }


            hBmp = LoadBitmap(hInst,MAKEINTRESOURCE(ImageId));
            HGDIOBJ hOldSel = SelectObject(hdcBmp,hBmp);
            BITMAP bmp;
            GetObject(hBmp,sizeof(BITMAP),&bmp);
            SetBkMode(hdcBmp, TRANSPARENT);
            SetTextColor(hdcBmp,RGB(255,255,255));

            TextOut(hdcBmp, Offset, 2, Text.data(), Text.size());
            BitBlt(lpDIS->hDC,rectBtn.left,rectBtn.top,bmp.bmWidth,bmp.bmHeight,hdcBmp,0,0,SRCCOPY);

            SelectObject(hdcBmp,hOldSel);
            DeleteObject(hBmp);
            DeleteDC(hdcBmp);*/
        }
        break;
        case WM_CTLCOLORSTATIC:
        {
            HDC hdcStatic = (HDC) wParam;
            if((HWND)lParam == Layout->HLabelBrowserBottom)
            {
                if(Layout->IsManualControlAction)
                {
                    //SetTextColor(hdcStatic, RGB(128,0,0));
                    SetTextColor(hdcStatic, RGB(60,60,60));
                }else
                {
                    SetTextColor(hdcStatic, RGB(60,60,60));
                }
            }else if((HWND)lParam == Layout->HLabelTop)
            {
                SetTextColor(hdcStatic, RGB(70,70,70));
            }else
            {
                SetBkColor(hdcStatic, RGB(255,255,255));
            }
        }
        break;
        case WM_MOUSEWHEEL:
        {
            if(!Layout->IsCentralShown && !Layout->IsContextMenuShown)
            {
                int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
                if(app->GetData()->ManualControl == BrowserData::Indirect)
                {
                    if(zDelta > 0)
                    {
                        app->ScrollUp();
                    }else
                    {
                        app->ScrollDown();
                    }
                }else
                {
                    if(zDelta > 0)
                    {
                        app->DirectControl()->ScrollUp();
                    }else
                    {
                        app->DirectControl()->ScrollDown();
                    }
                }
            }
        }
        break;
        /*case WM_RBUTTONUP:
        {
            if(Layout->IsContextMenuShown)
            {
                Layout->HideCentralBrowser();
            }
        }
        break;*/
        case WM_RBUTTONUP:

            if(app->GetData()->ManualControl != BrowserData::Indirect && !app->GetData()->IsRecord)
            {
                //Show context menu for run mode

                int xPos = LOWORD(lParam);
                int yPos = HIWORD(lParam);

                RECT r = Layout->GetBrowserRectangle(app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll);

                int MousePositionX = (float)(xPos - r.left) * (float)(app->GetData()->WidthBrowser) / (float)(r.right - r.left),MousePositionY = (float)(yPos - r.top) * (float)(app->GetData()->HeightBrowser) / (float)(r.bottom - r.top);

                app->ContextMenu(MousePositionX, MousePositionY);
                break;
            }
            IsControlButton = 1;
            IsLeftMouseButton = false;

        case WM_LBUTTONUP:
        {
            if(IsControlButton == -1)
            {
                IsControlButton = app->GetData()->ManualControl == BrowserData::Indirect || Layout->IsContextMenuShown;
            }


            bool IsImageSelect = false;
            std::string imagedata;

            if(IsControlButton)
            {
                IsImageSelect = Layout->GetIsImageSelect();

                if(IsImageSelect)
                {
                    RECT r = Layout->GetBrowserRectangle(app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll);

                    float kx = (float)(app->GetData()->WidthBrowser) / (float)(r.right - r.left);
                    float ky = (float)(app->GetData()->HeightBrowser) / (float)(r.bottom - r.top);
                    imagedata = app->GetSubImageDataBase64(Layout->ImageSelectStartX * kx,Layout->ImageSelectStartY*ky,Layout->ImageSelectEndX * kx,Layout->ImageSelectEndY*ky);
                }
            }

            if((!IsControlButton || (Layout->IsManualControlAction && !app->GetData()->IsRecord)) && (Layout->State == MainLayout::Ready || Layout->IsManualControlAction))
            {
                int xPos = LOWORD(lParam);
                int yPos = HIWORD(lParam);

                if(Layout->IsTouchCursor())
                {
                    xPos += 7;
                    yPos += 7;
                }

                RECT r = Layout->GetBrowserRectangle(app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll);

                int MousePositionX = (float)(xPos - r.left) * (float)(app->GetData()->WidthBrowser) / (float)(r.right - r.left),MousePositionY = (float)(yPos - r.top) * (float)(app->GetData()->HeightBrowser) / (float)(r.bottom - r.top);
                app->DirectControl()->MouseClick(MousePositionX,MousePositionY,false, wParam & MK_LBUTTON, wParam & MK_RBUTTON, wParam & MK_CONTROL,wParam & MK_SHIFT,IsLeftMouseButton);
            }

            if(Layout->OnMouseUp())
            {
                Layout->Update(app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll);
                Settings.SetToolboxHeight(Layout->ToolBoxRectHeight);
                Settings.SetScenarioWidth(Layout->DevToolsRectWidth);
                Settings.SaveToFile();
            }

            if(IsControlButton)
            {
                if(app->GetData()->MultiselectMode)
                {
                    //In multiselect mode behave differently
                    int xPos = LOWORD(lParam);
                    int yPos = HIWORD(lParam);
                    RECT r = Layout->GetBrowserRectangle(app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll);
                    if(xPos >= r.left && xPos <= r.right && yPos >= r.top && yPos <= r.bottom && Layout->State == MainLayout::Ready)
                    {
                        bool NeedInspect = false;
                        {
                            LOCK_BROWSER_DATA
                            NeedInspect = app->GetData()->_MultiSelectData.MouseClick();
                        }
                        app->UpdateMultiSelect();
                        if(NeedInspect)
                            app->RepeatInspectMouseAt();
                        }
                }else if(!Layout->IsToolboxMaximized && !Layout->IsCentralShown)
                {
                    int xPos = LOWORD(lParam);
                    int yPos = HIWORD(lParam);
                    RECT r = Layout->GetBrowserRectangle(app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll);

                    if(hPopupMenu)
                    {
                        DestroyMenu(hPopupMenu);
                        hPopupMenu = 0;
                    }
                    if(hForEachMenu)
                    {
                        DestroyMenu(hForEachMenu);
                        hForEachMenu = 0;
                    }

                    if(xPos >= r.left && xPos <= r.right && yPos >= r.top && yPos <= r.bottom && Layout->State == MainLayout::Ready)
                    {
                        {
                            LOCK_BROWSER_DATA
                            app->GetData()->_Inspect.isimage = IsImageSelect;
                            app->GetData()->_Inspect.imagedata = imagedata;
                        }
                        if(Layout->IsContextMenuShown)
                        {
                            Layout->HideCentralBrowser();
                        }
                        else
                        {
                            MouseMenuPositionX = (float)(xPos - r.left) * (float)(app->GetData()->WidthBrowser) / (float)(r.right - r.left),MouseMenuPositionY = (float)(yPos - r.top) * (float)(app->GetData()->HeightBrowser) / (float)(r.bottom - r.top);
                            InspectLastX = MouseMenuPositionX + app->GetData()->ScrollX;
                            InspectLastY = MouseMenuPositionY + app->GetData()->ScrollY;
                            MouseMenuPositionX += app->GetData()->ScrollX;
                            MouseMenuPositionY += app->GetData()->ScrollY;

                            int MouseMenuPositionXCopy = MouseMenuPositionX;
                            int MouseMenuPositionYCopy = MouseMenuPositionY;
                            MainApp * App = app.get();

                            app->GetAllPopupsUrls([xPos, r, IsImageSelect, App, MouseMenuPositionXCopy, MouseMenuPositionYCopy](const std::vector<std::string>& Urls)
                            {
                                App->ShowContextMenu(xPos - r.left,IsImageSelect,GenerateJsonMenu(IsImageSelect, MouseMenuPositionXCopy, MouseMenuPositionYCopy,Urls,App->GetData()->_ModulesData,App->GetData()->_UnusedModulesData));
                            });


                        }

                    }
                }
            }
        }
        break;

        case WM_SETCURSOR:
        {
            bool NeedUpdate;
            HCURSOR Cursor = Layout->GetCursor(NeedUpdate);
            if(NeedUpdate)
            {
                SetCursor(Cursor);
            }else
            {
                return DefWindowProc(hwnd, msg, wParam, lParam);
            }
        }
        break;

        case WM_MOUSEMOVE:
        {

            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);






            {
                RECT r = Layout->GetBrowserRectangle(app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll);

                if(xPos > r.left + 1 && xPos < r.right - 1 && yPos > r.top + 1 && yPos < r.bottom - 1)
                {
                    Layout->IsCursorOverBrowser = true;                    
                }
                else
                {
                    Layout->IsCursorOverBrowser = false;                    
                }


                if(xPos > r.right || yPos > r.bottom || xPos < r.left || yPos < r.top)
                {
                    LastMousePositionX = -1;
                    LastMousePositionY = -1;
                }else
                {
                    //compensate touch cursor offset
                    if(Layout->IsTouchCursor())
                    {
                        xPos += 7;
                        yPos += 7;
                    }
                    LastMousePositionX = xPos - r.left;
                    LastMousePositionY = yPos - r.top;
                }
            }

            LastMousePositionRawX = xPos;
            LastMousePositionRawY = yPos;


            if(app->GetData()->IsRecord && Layout->OnMouseMove(xPos,yPos,app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll))
            {
                Layout->Update(app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll);
                InvalidateRect(Layout->MainWindowHandle,NULL,true);
                Settings.SetToolboxHeight(Layout->ToolBoxRectHeight);
                Settings.SetScenarioWidth(Layout->DevToolsRectWidth);
                Settings.SaveToFile();
            }

            if(!IsUrlEditSelected() && !hTabsManualMenu && (Layout->State == MainLayout::Ready || Layout->IsManualControlAction))
            {
                if(!Layout->IsToolboxMaximized && !Layout->IsCentralShown && !Layout->IsContextMenuShown)
                {
                    RECT r = Layout->GetBrowserRectangle(app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll);

                    int MousePositionX = (float)(xPos - r.left) * (float)(app->GetData()->WidthBrowser) / (float)(r.right - r.left),MousePositionY = (float)(yPos - r.top) * (float)(app->GetData()->HeightBrowser) / (float)(r.bottom - r.top);
                    if(app->GetData()->ManualControl != BrowserData::Indirect)
                        app->DirectControl()->MouseMove(MousePositionX,MousePositionY, wParam & MK_LBUTTON, wParam & MK_CONTROL,wParam & MK_SHIFT);


                    if(xPos >= r.left && xPos <= r.right && yPos >= r.top && yPos <= r.bottom && !hPopupMenu && (Layout->State == MainLayout::Ready || Layout->IsManualControlAction) )
                    {
                        if(!IsCloseAsk && app->GetData()->IsRecord)
                            Layout->Focus();
                        if(app->GetData()->IsRecord)
                            app->MouseMoveAt(MousePositionX,MousePositionY);
                    }else
                    {
                        Layout->SetBrowserCursor(0);
                        if(app->GetData()->IsRecord)
                            app->MouseLeave();
                    }
                }
            }
        }
        break;
        /*case WM_SETFOCUS:
        if(app->GetData()->IsRecord)
        {
            Client->Write("<MaximizeWindow></MaximizeWindow>");
        }
        break;*/
        case WM_RBUTTONDOWN:
            if(app->GetData()->ManualControl != BrowserData::Indirect && !app->GetData()->IsRecord)
            {
                //Show context menu for run mode
                break;
            }
            IsControlButton = 1;
            IsLeftMouseButton = false;
        case WM_LBUTTONDOWN:

        {
            if(IsControlButton == -1)
                IsControlButton = app->GetData()->ManualControl == BrowserData::Indirect || Layout->IsContextMenuShown;

            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);

            if(Layout->IsTouchCursor())
            {
                xPos += 7;
                yPos += 7;
            }


            if((!IsControlButton || (Layout->IsManualControlAction && !app->GetData()->IsRecord)) && (Layout->State == MainLayout::Ready || Layout->IsManualControlAction))
            {
                Layout->Focus(true);

                RECT r = Layout->GetBrowserRectangle(app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll);

                int MousePositionX = (float)(xPos - r.left) * (float)(app->GetData()->WidthBrowser) / (float)(r.right - r.left),MousePositionY = (float)(yPos - r.top) * (float)(app->GetData()->HeightBrowser) / (float)(r.bottom - r.top);
                app->DirectControl()->MouseClick(MousePositionX,MousePositionY,true, wParam & MK_LBUTTON, wParam & MK_RBUTTON, wParam & MK_CONTROL,wParam & MK_SHIFT,IsLeftMouseButton);
            }


            Layout->OnMouseDown(xPos,yPos,app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll,IsControlButton);

        }
        break;
        case WM_CLOSE:
        {
            if(IsCloseAsk)
                return 0;
            if(app->GetData()->IsRecord)
                return 0;
            int result = IDNO;


            if(!app->GetData()->IsRecord && Layout->IsManualControlAction)
            {
                IsCloseAsk = true;

                result = MessageBox(NULL, Translate::Tr(L"Browser is under user control, return control to application?").c_str(), Translate::Tr(L"Stop manual control?").c_str(),  MB_YESNOCANCEL);
                IsCloseAsk = false;
            }

            if(result == IDCANCEL)
                return 0;

            if(result == IDYES)
            {
                app->GetData()->ManualControl = BrowserData::Indirect;
                app->UpdateManualControl();
            }

            WORKER_LOG("Minimized");
            app->Hide();
            Client->Write("<Minimized>1</Minimized>");
        }
        break;
        case WM_DESTROY:
           //PostQuitMessage(0);
        break;
        case WM_SIZE:
        {
            if(Layout->IsContextMenuShown)
                Layout->HideCentralBrowser();
            RECT rect;
            GetClientRect(hwnd,&rect);
            app->GetData()->WidthAll = rect.right - rect.left;
            app->GetData()->HeightAll = rect.bottom - rect.top;
            Layout->Update(app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll);
        }
        break;
        case WM_COMMAND:
            WORKER_LOG(std::string("+WM_COMMAND<<") + std::to_string((int)LOWORD(wParam)));
            {
                WORD Command = LOWORD(wParam);

                if(Command == BrowserContextMenu::IdOpenDeveloperTools && app->GetData()->IsRecord)
                {
                    //In record mode open developer tools in old window
                    if(Settings.DebugScenario() || Settings.DebugToolbox())
                    {
                        Layout->SetLabelTop(Translate::Tr(L"Chrome developer tools"));
                        Layout->UpdateTabs(MainLayout::Devtools);
                    }
                    app->ToggleDevTools();
                }else
                    app->ProcessContextMenu(Command);


                if(Command >= IDManualTabClose && Command < IDPopupEmulation)
                {
                    int i = Command - IDManualTabClose;
                    app->DirectControl()->CloseTab(i);
                }

                if(Command >= IDManualTabSwitch && Command < IDManualTabClose)
                {
                    int i = Command - IDManualTabSwitch;
                    app->DirectControl()->SelectTab(i);
                }

                switch(Command)
                {
                    case IDBrowserMenu:
                    {
                        POINT p;
                        p.x = 0;
                        p.y = Layout->Scale(20) + 1;
                        ClientToScreen(Layout->HBrowserMenu,&p);
                        app->MainContextMenu(p);
                    }
                    break;
                    case IDBrowserTabs:
                    {
                        app->DirectControl()->StopAddTab();
                        if(hTabsManualMenu)
                        {
                            DestroyMenu(hTabsManualMenu);
                            hTabsManualMenu = 0;
                        }
                        HWND TabsButton = Layout->HBrowserTabs;
                        int SelectIndex = app->GetActivePopupIndex();


                        HMENU *hTabsManualMenuRef = &hTabsManualMenu;

                        app->GetAllPopupsUrls([SelectIndex,hTabsManualMenuRef,TabsButton,hwnd](const std::vector<std::string>& Urls)
                        {
                            *hTabsManualMenuRef = CreatePopupMenu();

                            AppendMenu(*hTabsManualMenuRef, MF_BYPOSITION | MF_STRING, IDAddTabManual, Translate::Tr(L" + Add new tab").c_str());
                            int i = 1;
                            for(const std::string& Url: Urls)
                            {

                                std::wstring UrlCopy = s2ws(Url);
                                if(UrlCopy.size() > 50)
                                    UrlCopy = UrlCopy.substr(0,50) + L" ...";

                                std::wstring Text1,Text2;
                                Text1 += Translate::Tr(L"Select tab");
                                Text2 += Translate::Tr(L"Remove tab");
                                Text1 += L" #";
                                Text2 += L" #";
                                Text1 += std::to_wstring(i);
                                Text2 += std::to_wstring(i);

                                Text1 += L" (";
                                Text2 += L" (";

                                Text1 += UrlCopy;
                                Text2 += UrlCopy;

                                Text1 += L")";
                                Text2 += L")";

                                AppendMenu(*hTabsManualMenuRef,MF_SEPARATOR,NULL,L"Separator");

                                UINT Flags = MF_BYPOSITION | MF_STRING;

                                if(i - 1 == SelectIndex)
                                {
                                    Flags |= MFS_GRAYED;
                                }

                                AppendMenu(*hTabsManualMenuRef, Flags, IDManualTabSwitch + i - 1, Text1.c_str());

                                if(i > 1)
                                    AppendMenu(*hTabsManualMenuRef, MF_BYPOSITION | MF_STRING, IDManualTabClose + i - 1, Text2.c_str());

                                i++;
                            }

                            POINT p;
                            p.x = 0;
                            p.y = Layout->Scale(20) + 1;
                            ClientToScreen(TabsButton,&p);
                            TrackPopupMenu(*hTabsManualMenuRef, TPM_TOPALIGN | TPM_LEFTALIGN, p.x, p.y, 0, hwnd, NULL);
                            if(*hTabsManualMenuRef)
                            {
                                DestroyMenu(*hTabsManualMenuRef);
                                *hTabsManualMenuRef = 0;
                            }
                        });




                        return 0;
                    }
                    break;
                    case IDButtonMenu:
                    {
                        CreateHMenu();
                        return 0;
                    }
                    break;
                    case IDButtonTerminate:
                        app->Terminate();
                        return 0;
                    break;
                    case IDButtonQuit:
                        app->Hide();
                        Client->Write("<Minimized>1</Minimized>");
                        return 0;
                    break;
                    case IDButtonLoadUrl:
                    {
                        app->SetFocusOnNextLoad();
                        app->DirectControl()->Load(GetUrl());
                    }
                    break;
                    case IDAddTabManual:
                    {

                        app->DirectControl()->StartAddTab();
                        SetFocusForUrl = true;
                    }
                    break;
                    case IDButtonBackUrl:
                    {
                        app->SetFocusOnNextLoad();
                        app->DirectControl()->GoBack();
                    }
                    break;
                    case IDButtonUp:
                        app->ScrollUp();
                    break;
                    case IDButtonDown:
                        app->ScrollDown();
                    break;
                    case IDButtonRight:
                        app->ScrollRight();
                    break;
                    case IDButtonLeft:
                        app->ScrollLeft();
                    break;
                    case IDButtonUpUp:
                        app->ScrollUpUp();
                    break;
                    case IDButtonDownDown:
                        app->ScrollDownDown();
                    break;
                    case IDButtonRightRight:
                        app->ScrollRightRight();
                    break;
                    case IDButtonLeftLeft:
                        app->ScrollLeftLeft();
                    break;
                    case IDButtonMinimizeMaximize:
                        Layout->MinimizeOrMaximize(hwnd,app->GetData()->_ParentWindowHandle);
                        app->ForceUpdateWindowPositionWithParent();
                        Settings.SetMaximized(!Layout->IsMinimized);
                    break;
                    case IDRecordHttpRequests:
                    {
                        app->GetData()->IsRecordHttp = !app->GetData()->IsRecordHttp;
                        if(app->GetDevToolsReqest2Action())
                            app->GetDevToolsReqest2Action()->Reset();
                        if(app->GetData()->IsRecordHttp)
                        {
                            MessageBox(
                                        hwnd,
                                        (TCHAR *)Translate::Tr(L"Http request recorder is activated.\nAll requests which browser does will be converted to actions with http client and added to script editor.\nStart interacting with browser to see result.").data(),
                                        (TCHAR *)Translate::Tr(L"Request recorder").data(),
                                        MB_OK | MB_ICONINFORMATION
                            );
                        }

                    }
                    break;
                    case IDShowFingerprintDetector:
                    {
                        Layout->SetLabelTop(Translate::Tr(L"Fingerprint detector"));

                        app->CreateDetectorBrowser();
                        Layout->UpdateTabs(MainLayout::Detector);
                    }
                    break;
                    case IDShowDevtools:
                        /*Layout->SetLabelTop(Translate::Tr(L"Chrome developer tools"));
                        Layout->UpdateTabs(MainLayout::Devtools);
                        app->ToggleDevTools();*/
                        {
                            app->GetData()->Connector->OpenDevTools();
                        }
                    break;
                    case IDShowScenario:
                        Layout->SetLabelTop(Translate::Tr(L"Script editor"));
                        Layout->UpdateTabs(MainLayout::Scenario);
                        app->HideActionUpdater();
                    break;
                    case IDShowUpdater:
                        Layout->SetLabelTop(Translate::Tr(L"Script editor"));
                        Layout->UpdateTabs(MainLayout::Scenario);
                        app->ShowActionUpdater();
                    break;
                    case IDButtonSettings:
                        app->LoadSettingsPage();
                        {
                            LOCK_BROWSER_DATA
                            app->GetData()->_Inspect.label.clear();
                        }
                    break;
                    case IDButtonIndirect:
                    case IDButtonDirectNoRecord:
                    case IDButtonDirectRecord:
                        Layout->ShowManualSelect();
                        app->LoadManualSelect();
                    break;

                }
            }
            SetFocus(NULL);
        break;
        case WM_TIMER:
            {
                if(SetFocusForUrl)
                {
                    Layout->FocusUrl();
                    SetFocusForUrl = false;
                }
                if(app->GetData()->IsRecord)
                {
                    std::pair<std::string, bool> MenuRequest = app->GetMenuSelected();
                    if(MenuRequest.second)
                       ProcessMenu(MenuRequest.first) ;
                }
                if(app->GetData()->IsRecord)
                {
                    TimerLoop++;
                    TimerLoop %= 10;
                    //Skip 1 frame out of 10, this is required in order to avoid stack overflow in case if CefDoMessageLoopWork will dispatch message
                    if(TimerLoop != 0)
                    {
                        CefDoMessageLoopWork();
                    }
                }

                std::string Xml = Client->Read();
                if(!Xml.empty())
                {
                    /*if(Xml.find("<Visible") == std::string::npos && Xml.find("<SetWindow") == std::string::npos)
                        app->ClearElementCommand();*/
                    Parser->Parse(Xml);
                }

                app->Timer();
                
                _PopupEmulation->Timer();

                Layout->Timer(app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll);

                if(app->GetData()->ManualControl == BrowserData::DirectRecord)
                    app->DirectControl()->Timer();

                app->DirectControl()->ProcessPendingTouchEndEvent();

                app->GetData()->Connector->Timer();

            }
        break;
        case WM_SYSCHAR:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_CHAR:
            app->DirectControl()->Key(msg, wParam, lParam);
            if(app->GetData()->ManualControl != BrowserData::Indirect && !app->GetData()->IsRecord && msg == WM_KEYUP)
            {
                //Shortcuts in non record mode manual control
                if(wParam == 0x46 /* F */ && (GetKeyState(VK_CONTROL) & 0x8000) != 0 /* Is control pressed */)
                {
                    app->GetData()->_BrowserContextMenu.ShowFindDialog(hwnd);
                }

                if(wParam == VK_F5)
                {
                    app->Reload();
                }

                if(wParam == VK_F12)
                {
                    app->ShowDevTools();
                }
            }


            if(app->GetData()->ManualControl == BrowserData::Indirect && msg == WM_KEYUP)
            {
                if(wParam == VK_UP)
                {
                    ElementTweakUpOrDownUsed ++;
                    app->IncreaseInspectPosition();
                }else if(wParam == VK_DOWN)
                {
                    ElementTweakUpOrDownUsed ++;
                    app->DecreaseInspectPosition();
                }else if(wParam == VK_RETURN)
                {
                    ElementTweakReturnUsed = true;
                    PostMessage(hwnd, WM_LBUTTONDOWN, 1,MAKELPARAM(LastMousePositionRawX,LastMousePositionRawY));
                    PostMessage(hwnd, WM_LBUTTONUP, 0,MAKELPARAM(LastMousePositionRawX,LastMousePositionRawY));
                }
            }

        break;

        case WM_PAINT:

            {

                if(!Layout->IsToolboxMaximized)
                {
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    RECT br = Layout->GetBrowserRectangle(app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll);

                    Layout->CustomDraw(hdc,app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll);



                    if(!Layout->SplitterIsChangingSize())
                    {

                        std::pair<int, int> size = app->GetImageSize();

                        if(size.first > 0 && size.second > 0 && !Layout->IsCentralShown)
                        {
                            char * data = app->GetImageData();

                            BITMAPINFO info;
                            ZeroMemory(&info, sizeof(BITMAPINFO));
                            info.bmiHeader.biBitCount = 32;
                            info.bmiHeader.biWidth = size.first;
                            info.bmiHeader.biHeight = size.second;
                            info.bmiHeader.biPlanes = 1;
                            info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                            info.bmiHeader.biSizeImage = size.first * size.second * 4;
                            info.bmiHeader.biCompression = BI_RGB;

                            if(br.right > br.left && br.bottom > br.top)
                            {
                                if(!app->GetData()->IsRecord)
                                {
                                    StretchDIBits(hdc, br.left, br.bottom, br.right - br.left, br.top - br.bottom - 1, 0, 0, size.first, size.second, data, &info, DIB_RGB_COLORS, SRCCOPY);
                                }else
                                {

                                    HDC hdcTemp = CreateCompatibleDC(hdc);
                                    HBITMAP hbmMem = CreateCompatibleBitmap(hdc, br.right - br.left, br.bottom - br.top);
                                    HANDLE hOld   = SelectObject(hdcTemp, hbmMem);

                                    StretchDIBits(hdcTemp, 0, br.bottom - br.top, br.right - br.left, br.top - br.bottom - 1, 0, 0, size.first, size.second, data, &info, DIB_RGB_COLORS, SRCCOPY);


                                    /* Clear mouse over type if selected highlight area or include/exclude */
                                    if(
                                            app->GetData()->_MultiSelectData.MouseOverType != MouseOverNone
                                            && app->GetData()->_MultiSelectData.MouseOverType != MouseOverInspect
                                      )
                                    {
                                        LOCK_BROWSER_DATA
                                        app->GetData()->_MultiSelectData.MouseOverType = MouseOverNone;
                                    }

                                    MouseOverMultiSelect _MouseOverMultiSelect;

                                    {
                                        LOCK_BROWSER_DATA
                                        bool IsIndirect = app->GetData()->ManualControl == BrowserData::Indirect;
                                        app->GetData()->_Highlight.Paint(hdcTemp,_MouseOverMultiSelect,IsIndirect,LastMousePositionX,LastMousePositionY,app->GetData()->MultiselectMode,app->GetData()->_MultiSelectData,app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,br.right - br.left,br.bottom - br.top,app->GetData()->ScrollX,app->GetData()->ScrollY,0,0);
                                    }

                                    if(app->GetData()->MultiselectMode)
                                    {
                                        LOCK_BROWSER_DATA
                                        bool IsIndirect = app->GetData()->ManualControl == BrowserData::Indirect;
                                        app->GetData()->_MultiSelectData.Paint(hdcTemp,_MouseOverMultiSelect,IsIndirect,LastMousePositionX,LastMousePositionY,app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,br.right - br.left,br.bottom - br.top,app->GetData()->ScrollX,app->GetData()->ScrollY,0,0);
                                    }


                                    bool DrawInspect = true;
                                    {
                                        LOCK_BROWSER_DATA
                                        bool IsIndirect = app->GetData()->ManualControl == BrowserData::Indirect;
                                        _MouseOverMultiSelect.Paint(hdcTemp, IsIndirect, LastMousePositionX, LastMousePositionY, 0, 0, app->GetData()->MultiselectMode, &app->GetData()->_MultiSelectData);
                                        if(app->GetData()->_MultiSelectData.MouseOverType != MouseOverNone && app->GetData()->_MultiSelectData.MouseOverType != MouseOverInspect)
                                        {
                                            DrawInspect = false;
                                        }
                                    }


                                    bool ImageSelect = Layout->DrawImageSelect(hdcTemp);


                                    if(DrawInspect && !ImageSelect && (app->GetData()->ManualControl == BrowserData::Indirect || Layout->IsContextMenuShown))
                                    {
                                        LOCK_BROWSER_DATA
                                        bool SimplifiedText = (ElementTweakUpOrDownUsed > 10) || (ElementTweakUpOrDownUsed > 0 && ElementTweakReturnUsed);
                                        app->GetData()->_Inspect.Paint(hdcTemp,Layout->Scale(14),InspectFont,!Layout->IsContextMenuShown,SimplifiedText,app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,br.right - br.left,br.bottom - br.top,app->GetData()->ScrollX,app->GetData()->ScrollY,0,0);
                                    }

                                    if(Layout->IsContextMenuShown)
                                    {
                                        if(Layout->ContextMenuIsRight)
                                        {
                                            BitBlt(hdc, br.left, br.top, br.right - br.left - Layout->ContextMenuWidth, br.bottom - br.top, hdcTemp, 0, 0, SRCCOPY);
                                        }else
                                        {
                                            BitBlt(hdc, br.left + Layout->ContextMenuWidth, br.top, br.right - br.left - Layout->ContextMenuWidth, br.bottom - br.top, hdcTemp, Layout->ContextMenuWidth, 0, SRCCOPY);
                                        }
                                    }else
                                    {
                                        BitBlt(hdc, br.left, br.top, br.right - br.left, br.bottom - br.top, hdcTemp, 0, 0, SRCCOPY);
                                    }



                                    SelectObject(hdcTemp, hOld);
                                    DeleteObject(hbmMem);
                                    DeleteDC(hdcTemp);

                                }


                            }
                        }

                        /*if(app->GetData()->IsRecord)
                        {
                            RECT r = Layout->GetStateIndicatorRectangle(app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll);
                        }*/

                        HCURSOR CurrentCursor = 0;

                        if(app->GetData()->IsTouchScreen)
                        {
                            if(app->GetData()->IsTouchPressedAutomation)
                            {
                                CurrentCursor = HCursorTouch;
                            }else
                            {
                                CurrentCursor = 0;
                            }

                        }else
                        {
                            CurrentCursor = HCursor;
                        }

                        if(!Layout->IsCentralShown && CurrentCursor && !Layout->IsContextMenuShown && (app->GetData()->ManualControl == BrowserData::Indirect || app->GetData()->IsRecord))
                        {
                            LOCK_BROWSER_DATA
                            BrowserData * d = app->GetData();
                            if(d->CursorX >= 0 && d->CursorX <= d->WidthBrowser && d->CursorY >= 0 && d->CursorY <= d->HeightBrowser)
                                DrawIcon(hdc, br.left + (float)(d->CursorX) * (float)(br.right - br.left) / (float)(d->WidthBrowser) , br.top + (float)(d->CursorY) * (float)(br.bottom - br.top) / (float)(d->HeightBrowser), CurrentCursor);
                        }

                        if(Layout->IsManualControlAction && !Layout->IsCentralShown)
                        {

                            RECT r = Layout->GetManualControlIndicatorRectangle(app->GetData()->WidthBrowser,app->GetData()->HeightBrowser,app->GetData()->WidthAll,app->GetData()->HeightAll);
                            BITMAP bitmap;

                            HBITMAP b = Layout->GetManualAnimationButton();

                            HDC hdcMem = CreateCompatibleDC(hdc);
                            HGDIOBJ oldBitmap = SelectObject(hdcMem, b);
                            GetObject(b, sizeof(bitmap), &bitmap);
                            BitBlt(hdc, r.left, r.top, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
                            SelectObject(hdcMem, oldBitmap);
                            DeleteDC(hdcMem);
                        }


                    }



                    EndPaint(hwnd, &ps);
                }

                /* Update label */
                if(!Layout->IsManualControlAction)
                {
                    std::string label;
                    {
                        LOCK_BROWSER_DATA
                        label = app->GetData()->_Inspect.label;
                    }
                    if(app->GetData()->MultiselectMode)
                    {

                        LOCK_BROWSER_DATA
                        if(
                                    app->GetData()->_MultiSelectData.MouseOverType == MouseOverInclude
                                    ||
                                    app->GetData()->_MultiSelectData.MouseOverType == MouseOverExclude
                           )
                        {

                            label = app->GetData()->_MultiSelectData.MouseOverDataItem.label;
                        }

                        if(
                                    app->GetData()->_MultiSelectData.MouseOverType == MouseOverHighlight
                           )
                        {
                            label = app->GetData()->_MultiSelectData.MouseOverHighlightSelector;
                        }
                    }


                    if(LastLabel != label)
                    {
                        Layout->SetBrowserLabel(label);
                        LastLabel = label;
                    }
                }

                return DefWindowProc(hwnd, msg, wParam, lParam);
            }
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


LRESULT __stdcall KeyHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{

    if (nCode >= 0)
    {
        if (wParam == WM_KEYDOWN)
        {
            KBDLLHOOKSTRUCT kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
            bool IsChanged = false;
            if (kbdStruct.vkCode == VK_F1)
            {
                IsChanged = true;
                app->GetData()->ManualControl = BrowserData::Indirect;
            }
            if (kbdStruct.vkCode == VK_F2)
            {
                IsChanged = true;
                app->GetData()->ManualControl = BrowserData::DirectNoRecord;
            }
            if (kbdStruct.vkCode == VK_F3)
            {
                IsChanged = true;
                app->GetData()->ManualControl = BrowserData::DirectRecord;
            }
            if(IsChanged)
            {
                app->UpdateManualControl(true);
            }
        }
    }



    return CallNextHookEx(KeyHook, nCode, wParam, lParam);
}

void OnBrowserCreated()
{
    Client->Start(KeyGlobal,PidGlobal);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{


    #if defined(BAS_DEBUG)
        CCrashHandler ch;
        ch.SetProcessExceptionHandlers();
        ch.SetThreadExceptionHandlers();
    #else
        SetErrorMode(SetErrorMode(0) | SEM_NOGPFAULTERRORBOX);
    #endif

    CefEnableHighDPISupport();

    {
        std::string CurrentProcessId = std::string("BASProcess") + std::to_string(GetCurrentProcessId());
        HANDLE HandleMutex = CreateMutexA(0,false,CurrentProcessId.c_str());
        WaitForSingleObject(HandleMutex,INFINITE);
    }


    std::srand(MixNumbers(clock(), time(NULL), GetCurrentProcessId()));

    CefMainArgs main_args(hInstance);

    std::string ProcessType;
    std::string SandboxType;
    std::string ParentProcessId;
    std::string UniqueProcessId;
    double DeviceScaleFactor = 1.0;
    int PcapPort = 29815;


    //Parse command line
    std::vector<std::wstring> Arguments;
    {
        int NumberArgs;
        LPWSTR *Arglist;
        Arglist = CommandLineToArgvW(GetCommandLineW(), &NumberArgs);
        for(int i = 0;i<NumberArgs;i++)
        {
            std::wstring Item(Arglist[i]);
            if(Item.find(L"--type=") == 0)
            {
                ProcessType = ws2s(Item);
                ProcessType.erase(0,7);
            }

            if(Item.find(L"--service-sandbox-type=") == 0)
            {
                SandboxType = ws2s(Item);
                SandboxType.erase(0,23);
            }

            if(Item.find(L"--parent-process-id=") == 0)
            {
                ParentProcessId = ws2s(Item);
                ParentProcessId.erase(0,20);
            }

            if(Item.find(L"--unique-process-id=") == 0)
            {
                UniqueProcessId = ws2s(Item);
                UniqueProcessId.erase(0,20);
            }

            if(Item.find(L"--interface-scale-factor=") == 0)
            {
                std::string DeviceScaleFactorString = ws2s(Item);
                DeviceScaleFactorString.erase(0,25);
                DeviceScaleFactor = std::stoi(DeviceScaleFactorString) * 0.01;
                if(DeviceScaleFactor <= 0.0001)
                    DeviceScaleFactor = 1.0;

                //Don't add to arguments list
                continue;
            }

            if(Item.find(L"--PcapPort=") == 0)
            {
                std::string PcapPortString = ws2s(Item);
                PcapPortString.erase(0,11);
                PcapPort = std::stoi(PcapPortString);

                //Don't add to arguments list
                continue;
            }

            Arguments.push_back(Item);
        }
        LocalFree(Arglist);
    }


    if(ProcessType == "renderer")
    {
        if(!ParentProcessId.empty())
            new std::thread(TerminateOnCloseMutex, std::string("BASProcess") + ParentProcessId, false, false);

        // Start renderer process
        CefRefPtr<CefApp> app = new RenderApp();
        int exit_code = CefExecuteProcess(main_args, app, NULL);
        return exit_code;
    }else if (ProcessType == "gpu-process")
    {
        if(!ParentProcessId.empty())
            new std::thread(TerminateOnCloseMutex, std::string("BASProcess") + ParentProcessId, false, false);

        // Start gpu process
        CefRefPtr<CefApp> app = new EmptyApp();
        int exit_code = CefExecuteProcess(main_args, app, NULL);
        return exit_code;
    }else if(!ProcessType.empty())
    {
        if(!ParentProcessId.empty())
            new std::thread(TerminateOnCloseMutex, std::string("BASProcess") + ParentProcessId, false, false);

        // Other process type, for exampleflash plugin
        CefRefPtr<CefApp> app = new EmptyApp();
        int exit_code = CefExecuteProcess(main_args, app, NULL);
        return exit_code;
    }

    //Start browser main process


    Settings.ParseCommandLine(Arguments);

    int NumberArgs = Arguments.size();

    std::string Pid;
    if(NumberArgs>4)
    {
        Pid = ws2s(Arguments[4]);
        WORKER_LOG(std::string("Pid : ") + Pid);

        new std::thread(TerminateOnCloseMutex, std::string("BASProcess") + Pid, true, true);
    }

    std::string Lang = ws2s(Arguments[1]);
    std::string Key = ws2s(Arguments[2]);


    std::ofstream *outfile = new std::ofstream();
    outfile->open(std::wstring(L"s/") + s2ws(Settings.UniqueProcessId()) + std::wstring(L".lock"), std::ofstream::out);


    Layout = new MainLayout(Settings.ToolboxHeight(),Settings.ScenarioWidth(),DeviceScaleFactor);
    hInst = hInstance;

    app = new MainApp();
    app->SetSettings(&Settings);
    app->SetLayout(Layout);

    BrowserData * Data = new BrowserData();
    PostManager * _PostManager = new PostManager();

    {
        Data->IsRecord = Arguments.size() == 6;
        Data->Saver.IsRecord = Data->IsRecord;
        Layout->IsRecord = Data->IsRecord;
        worker_log_init(Data->IsRecord);
    }

    DevToolsReqest2Action * _DevToolsReqest2Action = 0;

    if(Data->IsRecord)
    {
        _DevToolsReqest2Action = new DevToolsReqest2Action();
    }

    if(Data->IsRecord)
    {
        app->Detector.Init(Settings.UniqueProcessId());
        app->Notifications.Init(Settings.UniqueProcessId());
    }


    WORKER_LOG(std::string("IsRecord<<") + std::to_string(Data->IsRecord));

    Data->IsRecordHttp = false;
    Data->IsTouchScreen = false;
    Data->HasHcaptchaModule = true;
    Data->HasRecaptchaModule = true;
    Data->HasFuncaptchaModule = true;
    Data->IsTouchPressedDirectControl = false;
    Data->IsTouchPressedAutomation = false;
    Data->TouchEventId = 1;
    Data->OldestRequestTime = 0;
    Data->_MainWindowHandle = 0;
    Data->_ParentWindowHandle = 0;
    Data->CursorX = rand()%20 + 30;
    Data->CursorY = rand()%20 + 30;
    Data->DirectControlOrAutomationCursorX = Data->CursorX;
    Data->DirectControlOrAutomationCursorY = Data->CursorY;
    Data->ScrollX = 0;
    Data->ScrollY = 0;
    Data->_Inspect.active = false;
    Data->_Inspect.position = 0;
    Data->IsReset = false;
    Data->IsAboutBlankLoaded = false;
    Data->IsCreatingNewPopup = false;
    Data->TimezoneSelected = false;
    Data->IsDrag = false;
    Data->IsTesing = false;
    Data->IsMousePress = false;
    Data->MultiselectMode = false;
    Data->SetIndirectControlOnNext = false;
    Data->MultiselectIsInsideElementLoop = false;
    Data->_AcceptLanguagePattern = "de-DE,de;q=0.9,en-US;q=0.8,en;q=0.7";
    Data->_UniqueProcessId = Settings.UniqueProcessId();
    Data->RemoteDebuggingPort = 10000 + rand()%55000;
    Data->LastClearRequest = 0;
    Data->ManualControl = BrowserData::Indirect;
    Data->Saver.FilePath = std::string("s/") + Settings.UniqueProcessId() + std::string(".ini");
    Data->Saver.Detector = Settings.Detector();
    Data->Saver.TemporaryDisableDetector = false;
    Data->UrlHandler = 0;
    Data->LastClickIsFromIndirectControl = true;
    Data->IsProxySet = false;

    //Create profile fast if it is empty
    if(Settings.ProfilesCaching())
        CreateEmptyProfile(Settings.Profile());

    //Ensure that profile is not busy
    {
        std::wstring LockPath = Settings.Profile() + std::wstring(L"/lockfile");
        DeleteFile(LockPath.c_str());
        while(FileExists(LockPath))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            DeleteFile(LockPath.c_str());
        }
    }

    if(Data->IsRecord)
        Data->_ModulesData = LoadModulesData(Lang, Pid, Data->_UnusedModulesData);

    Data->HasHcaptchaModule = IsModuleEnabled("HCaptcha");
    Data->HasRecaptchaModule = IsModuleEnabled("ReCaptcha");
    Data->HasFuncaptchaModule = IsModuleEnabled("FunCaptcha");

    Data->BrowserCode = ReadAllString("browser_code.txt");
    if(!Data->BrowserCode.empty())
        Data->BrowserCode = base64_decode(Data->BrowserCode);

    if(Settings.UseWidevine())
    {
        InstallWidevine(Settings.Profile(), GetBrowserVersionIdFromPath());
    }else
    {
        DeinstallWidevine(Settings.Profile());
    }

    if(Settings.UseSafeBrowsing())
    {
        InstallSafeBrowsing(Settings.Profile(), GetBrowserVersionIdFromPath());
    }else
    {
        DeinstallSafeBrowsing(Settings.Profile());
    }

    if(Settings.UseComponents())
    {
        InstallComponents(Settings.Profile(), GetBrowserVersionIdFromPath());
    }else
    {
        DeinstallComponents(Settings.Profile());
    }


    if(HasWorkerArgument("--mock-connector"))
    {
        // Initialize dummy connector.
        Data->Connector = new NoneConnector();
    }else
    {
        // Initialize default connector.
        Data->Connector = new DevToolsConnector();
    }
    Data->Results = new ResultManager();
    Data->Results->Init(Data->Connector);
    Data->Connector->OnPaint.push_back(std::bind(&MainApp::OnPaint,app.get()));
    Data->Connector->OnResize.push_back(std::bind(&MainApp::OnResize,app.get()));
    Data->Connector->OnScroll.push_back(std::bind(&MainApp::OnScroll,app.get()));
    Data->Connector->OnRequestStart.push_back(std::bind(&MainApp::OnRequestStart,app.get(),_1));
    Data->Connector->OnRequestStop.push_back(std::bind(&MainApp::OnRequestStop,app.get(),_1));
    if(Data->IsRecord)
    {
        Data->Connector->OnRequestDataMain.push_back(std::bind(&MainApp::OnRequestDataMain,app.get(),_1));
        Data->Connector->OnRequestDataAdditional.push_back(std::bind(&MainApp::OnRequestDataAdditional,app.get(),_1));
    }
    Data->Connector->OnLoadStart.push_back(std::bind(&MainApp::OnLoadStart,app.get()));
    Data->Connector->OnLoadStop.push_back(std::bind(&MainApp::OnLoadStop,app.get()));
    Data->Connector->OnAddressChanged.push_back(std::bind(&MainApp::OnAddressChanged,app.get(),_1));
    Data->Connector->OnNativeDialog.push_back(std::bind(&MainApp::OnNativeDialog,app.get(),_1));
    Data->Connector->OnDownloadStarted.push_back(std::bind(&MainApp::OnDownloadStarted,app.get(),_1));
    Data->Connector->OnBrowserCreated.push_back(OnBrowserCreated);
    Data->MainRemoteDebuggingPort = 10000 + rand()%55000;
    std::shared_ptr<IWebSocketClientFactory> WebScoketFactory = std::make_shared<RawCppWebSocketClientFactory>();
    if(Settings.SaveBrowserLog())
    {
        WebScoketFactory->SetLogPath(Settings.Profile() + std::wstring(L"/cdp.txt"));
    }
    Data->Connector->Initialize(
                    std::make_shared<RawCppHttpClientFactory>(),
                    WebScoketFactory,
                    Data->MainRemoteDebuggingPort, Settings.UniqueProcessId(), std::to_string(GetCurrentProcessId()),
                    PrepareConstantStartupScript(Data),
                    ParseChromeCommandLine(Settings.AdditionalCommandLine()),
                    Settings.InitialProxy(), PcapPort
                    );
    WebScoketFactory.reset();
    if(Data->IsRecord)
    {
        Data->Connector->DisableBackgroundMode();
    }else
    {
        Data->Connector->EnableBackgroundMode();
    }
    Data->Connector->SetProfilePath(Settings.Profile());
    Data->Connector->SetExtensionList(Settings.Extensions());
    Data->Connector->StartScreenCast();
    Data->Connector->StartProcess();

    app->SetData(Data);
    app->SetPostManager(_PostManager);
    if(Data->IsRecord)
    {
        app->SetDevToolsReqest2Action(_DevToolsReqest2Action);
    }
    app->InitNetworkProcessIPC();
    app->WriteBrowserData();

    int exit_code = CefExecuteProcess(main_args, app.get(), NULL);

    if (exit_code >= 0)
      return exit_code;

    CefSettings settings;
    settings.background_color = CefColorSetARGB(255, 255, 255, 255);
    settings.no_sandbox = true;
    settings.ignore_certificate_errors = true;

    if(Data->IsRecord)
    {
    settings.remote_debugging_port = Data->RemoteDebuggingPort;
    }

    if(Data->IsRecord)
    {
        WriteStringToFile("remote_debugging_port.txt", std::to_string(Data->RemoteDebuggingPort));
    }


    /*if(Data->IsRecord)
    {
        settings.log_severity = LOGSEVERITY_VERBOSE;
    }else*/
    {
        settings.log_severity = LOGSEVERITY_DISABLE;
    }
    settings.windowless_rendering_enabled = true;
    settings.multi_threaded_message_loop = false;
    settings.persist_user_preferences = 0;
    settings.persist_session_cookies = 0;
    std::wstring cache = L"cache";
    cef_string_utf16_set(cache.data(),cache.size(),&settings.user_data_path,true);



    _PopupEmulation = new PopupEmulation();

    Client = new PipesClient();
    Parser = new CommandParser();
    Translate::SetLanguage(Lang);
    app->SetInitialStateCallback(Lang);

    WORKER_LOG(Key);
    Arguments.clear();

    KeyGlobal = Key;
    PidGlobal = Pid;
    _PopupEmulation->EventPopupShown.push_back(std::bind(&MainApp::ComboboxOpened,app.get()));
    Layout->EventLoadNoDataPage.push_back(std::bind(&MainApp::LoadNoDataCallback,app.get()));
    Parser->EventLoad.push_back(std::bind(&MainApp::LoadCallback,app.get(),_1));
    Parser->EventLoad2.push_back(std::bind(&MainApp::Load2Callback,app.get(),_1,_2,_3));
    Parser->EventView.push_back(std::bind(&MainApp::ViewCallback,app.get(),_1));
    Parser->EventGetTabs.push_back(std::bind(&MainApp::GetTabsCallback,app.get(),_1));
    Parser->EventManualBrowserControl.push_back(std::bind(&MainApp::ManualBrowserControlCallback,app.get(),_1));

    Parser->EventDisableBorwser.push_back(std::bind(&MainApp::DisableBrowserCallback,app.get()));
    Parser->EventVisible.push_back(std::bind(&MainApp::VisibleCallback,app.get(),_1));
    Parser->EventFlush.push_back(std::bind(&MainApp::FlushCallback,app.get()));
    Parser->EventSetProxy.push_back(std::bind(&MainApp::SetProxyCallback,app.get(),_1,_2,_3,_4,_5,_6));
    Parser->EventAddHeader.push_back(std::bind(&MainApp::AddHeaderCallback,app.get(),_1,_2,_3));
    Parser->EventSetHeaderList.push_back(std::bind(&MainApp::SetHeaderListCallback,app.get(),_1));
    Parser->EventSetAcceptLanguagePattern.push_back(std::bind(&MainApp::SetAcceptLanguagePatternCallback,app.get(),_1));
    Parser->EventSetUserAgentData.push_back(std::bind(&MainApp::SetUserAgentDataCallback,app.get(),_1));
    Parser->EventCleanHeader.push_back(std::bind(&MainApp::CleanHeaderCallback,app.get()));
    Parser->EventSetUserAgent.push_back(std::bind(&MainApp::SetUserAgentCallback,app.get(),_1));
    Parser->EventPrepareFunction.push_back(std::bind(&MainApp::PrepareFunctionCallback,app.get(),_1));
    Parser->EventBackupDone.push_back(std::bind(&MainApp::BackupDoneCallback,app.get(),_1));
    Parser->EventRecaptchaV3List.push_back(std::bind(&MainApp::RecaptchaV3ListCallback,app.get(),_1));
    Parser->EventClickExtensionButton.push_back(std::bind(&MainApp::ClickExtensionButton,app.get(),_1));
    Parser->EventRecaptchaV3Result.push_back(std::bind(&MainApp::RecaptchaV3ResultCallback,app.get(),_1,_2));
    Parser->EventGetUrl.push_back(std::bind(&MainApp::GetUrlCallback,app.get()));
    Parser->EventGetBrowserScreenSettings.push_back(std::bind(&MainApp::GetBrowserScreenSettingsCallback,app.get()));
    Parser->EventResize.push_back(std::bind(&MainApp::ResizeCallback,app.get(),_1,_2));
    Parser->EventTimezone.push_back(std::bind(&MainApp::TimezoneCallback,app.get(),_1));
    Parser->EventSetWindow.push_back(std::bind(&MainApp::SetWindowCallback,app.get(),_1,_2));
    Parser->EventHighlightAction.push_back(std::bind(&MainApp::HighlightActionCallback,app.get(),_1));
    Parser->EventMouseClick.push_back(std::bind(&MainApp::MouseClickCallback,app.get(),_1,_2));
    Parser->EventMouseClickUp.push_back(std::bind(&MainApp::MouseClickUpCallback,app.get(),_1,_2));
    Parser->EventMouseClickDown.push_back(std::bind(&MainApp::MouseClickDownCallback,app.get(),_1,_2));
    Parser->EventPopupClose.push_back(std::bind(&MainApp::PopupCloseCallback,app.get(),_1));
    Parser->EventPopupSelect.push_back(std::bind(&MainApp::PopupSelectCallback,app.get(),_1));
    Parser->EventPopupCreate.push_back(std::bind(&MainApp::PopupCreateCallback,app.get(),_1,_2));
    Parser->EventPopupCreate2.push_back(std::bind(&MainApp::PopupCreate2Callback,app.get(),_1,_2,_3,_4));
    Parser->EventSetComboboxIndex.push_back(std::bind(&MainApp::SetComboboxIndexCallback,app.get(),_1));
    Parser->EventPopupInfo.push_back(std::bind(&MainApp::PopupInfoCallback,app.get()));
    Parser->EventMouseMove.push_back(std::bind(&MainApp::MouseMoveCallback,app.get(),_1,_2,_3,_4,_5,_6,_7,_8,_9,_10));
    Parser->EventSetDeviceScaleFactor.push_back(std::bind(&MainApp::SetDeviceScaleFactorCallback,app.get(),_1));
    Parser->EventScroll.push_back(std::bind(&MainApp::ScrollCallback,app.get(),_1,_2));
    Parser->EventRender.push_back(std::bind(&MainApp::RenderCallback,app.get(),_1,_2,_3,_4));
    Parser->EventSetOpenFileName.push_back(std::bind(&MainApp::SetOpenFileNameCallback,app.get(),_1));
    Parser->EventDragFile.push_back(std::bind(&MainApp::DragFileCallback,app.get(),_1));
    Parser->EventSetStartupScript.push_back(std::bind(&MainApp::SetStartupScriptCallback,app.get(),_1,_2,_3));
    Parser->EventRunTask.push_back(std::bind(&MainApp::RunTaskCallback,app.get(),_1,_2,_3));
    Parser->EventCheckResult.push_back(std::bind(&MainApp::CheckResultCallback,app.get(),_1,_2,_3));
    Parser->EventSendWorkerSettings.push_back(std::bind(&MainApp::SetWorkerSettingsCallback,app.get(),_1,_2,_3,_4,_5,_6,_7,_8,_9,_10));

    Parser->EventSetPromptResult.push_back(std::bind(&MainApp::SetPromptResultCallback,app.get(),_1));
    Parser->EventSetHttpAuthResult.push_back(std::bind(&MainApp::SetHttpAuthResultCallback,app.get(),_1,_2));
    Parser->EventGetCookiesForUrl.push_back(std::bind(&MainApp::GetCookiesForUrlCallback,app.get(),_1));
    Parser->EventSaveCookies.push_back(std::bind(&MainApp::SaveCookiesCallback,app.get()));
    Parser->EventRestoreCookies.push_back(std::bind(&MainApp::RestoreCookiesCallback,app.get(),_1));
    Parser->EventRestoreLocalStorage.push_back(std::bind(&MainApp::RestoreLocalStorageCallback,app.get(),_1));
    Parser->EventWaitCode.push_back(std::bind(&MainApp::WaitCodeCallback,app.get()));
    Parser->EventStartSection.push_back(std::bind(&MainApp::StartSectionCallback,app.get(),_1));
    Parser->EventSetFontList.push_back(std::bind(&MainApp::SetFontListCallback,app.get(),_1));
    Parser->EventScriptFinished.push_back(std::bind(&MainApp::ScriptFinishedCallback,app.get()));
    Parser->EventSetCode.push_back(std::bind(&MainApp::SetCodeCallback,app.get(),_1,_2,_3,_4,_5,_6,_7));
    Parser->EventSetResources.push_back(std::bind(&MainApp::SetResourceCallback,app.get(),_1));
    Parser->EventNavigateBack.push_back(std::bind(&MainApp::NavigateBackCallback,app.get(), _1));
    Parser->EventIsChanged.push_back(std::bind(&MainApp::IsChangedCallback,app.get()));
    Parser->EventSetNextAction.push_back(std::bind(&MainApp::SetNextActionCallback,app.get(),_1));
    Parser->EventCrush.push_back(std::bind(&MainApp::CrushCallback,app.get()));


    Parser->EventAddCacheMaskAllow.push_back(std::bind(&MainApp::AddCacheMaskAllowCallback,app.get(),_1));
    Parser->EventAddCacheMaskDeny.push_back(std::bind(&MainApp::AddCacheMaskDenyCallback,app.get(),_1));
    Parser->EventAddRequestMaskAllow.push_back(std::bind(&MainApp::AddRequestMaskAllowCallback,app.get(),_1));
    Parser->EventAddRequestMaskDeny.push_back(std::bind(&MainApp::AddRequestMaskDenyCallback,app.get(),_1));
    Parser->EventClearCacheMask.push_back(std::bind(&MainApp::ClearCacheMaskCallback,app.get()));
    Parser->EventClearRequestMask.push_back(std::bind(&MainApp::ClearRequestMaskCallback,app.get()));
    Parser->EventAllowPopups.push_back(std::bind(&MainApp::AllowPopups,app.get()));
    Parser->EventRestrictPopups.push_back(std::bind(&MainApp::RestrictPopups,app.get()));
    Parser->EventAllowDownloads.push_back(std::bind(&MainApp::AllowDownloads,app.get()));
    Parser->EventRestrictDownloads.push_back(std::bind(&MainApp::RestrictDownloads,app.get()));
    Parser->EventClearLoadedUrl.push_back(std::bind(&MainApp::ClearLoadedUrlCallback,app.get()));
    Parser->EventClearCachedData.push_back(std::bind(&MainApp::ClearCachedDataCallback,app.get()));
    Parser->EventClearAll.push_back(std::bind(&MainApp::ClearAllCallback,app.get()));
    Parser->EventClearMasks.push_back(std::bind(&MainApp::ClearMasksCallback,app.get()));
    Parser->EventClearData.push_back(std::bind(&MainApp::ClearDataCallback,app.get()));
    Parser->EventFindCacheByMaskBase64.push_back(std::bind(&MainApp::FindCacheByMaskBase64Callback,app.get(),_1));
    Parser->EventFindStatusByMask.push_back(std::bind(&MainApp::FindStatusByMaskCallback,app.get(),_1));
    Parser->EventFindCacheByMaskString.push_back(std::bind(&MainApp::FindCacheByMaskStringCallback,app.get(),_1));
    Parser->EventFindAllCache.push_back(std::bind(&MainApp::FindAllCacheCallback,app.get(),_1));
    Parser->EventIsUrlLoadedByMask.push_back(std::bind(&MainApp::IsUrlLoadedByMaskCallback,app.get(),_1));
    Parser->EventGetLoadStats.push_back(std::bind(&MainApp::GetLoadStatsCallback,app.get()));
    Parser->EventElementCommand.push_back(std::bind(&MainApp::ElementCommandCallback,app.get(),_1));
    Parser->EventRequestVariablesResult.push_back(std::bind(&MainApp::RequestVariablesResultCallback,app.get(),_1));
    Parser->EventDebugVariablesResult.push_back(std::bind(&MainApp::DebugVariablesResultCallback,app.get(),_1));
    Parser->EventDebugCallstackResult.push_back(std::bind(&MainApp::DebugCallstackResultCallback,app.get(),_1));
    Parser->EventRestoreOriginalStage.push_back(RestoreOriginalStage);
    Parser->EventSetMode.push_back(SetMode);

    Parser->EventClearImageData.push_back(std::bind(&MainApp::ClearImageDataCallback,app.get()));
    Parser->EventSetImageData.push_back(std::bind(&MainApp::SetImageDataCallback,app.get(),_1));
    Parser->EventFindImage.push_back(std::bind(&MainApp::FindImageCallback,app.get()));
    Parser->EventFindUrlByMask.push_back(std::bind(&MainApp::FindUrlByMaskCallback,app.get(),_1));

    app->EventSendTextResponce.push_back(std::bind(&PipesClient::Write,Client,_1));

    app->EventHighlightMenu.push_back(HighlightHMenu);

    WORKER_LOG("Start Main Loop");

    WNDCLASSEX wc;
    MSG Msg;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = L"WorkerWindowClass";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
        return 0;

    std::pair<int,int> BrowserSize = Layout->GetDefaultBrowserSize();
    std::pair<int,int> WindowSize = Layout->GetDefaultWindowSize();
    Data->WidthBrowser = BrowserSize.first;
    Data->HeightBrowser = BrowserSize.second;
    Data->WidthAll = WindowSize.first;
    Data->HeightAll = WindowSize.second;

    hwnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        L"WorkerWindowClass",
        Translate::Tr(L"Browser").c_str(),
        WS_OVERLAPPEDWINDOW,
        //WS_POPUP,
        0, 0, Data->WidthAll, Data->HeightAll,
        NULL, NULL, hInstance, NULL);
    if(hwnd == NULL)
        return 0;

    _PopupEmulation->Init(Data, IDPopupEmulation, hwnd, Layout, Data->Connector->GetProxyConfigFolder());
    app->SetPopupEmulation(_PopupEmulation);

    Data->_MainWindowHandle = hwnd;
    Layout->MainWindowHandle = hwnd;


    if(Data->IsRecord)
    {
        KeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyHookCallback, NULL, 0);
    }

    //Set control type to direct
    /*if(Data->IsRecord)
    {
        Data->ManualControl = BrowserData::DirectRecord;
        app->UpdateManualControl();
        Layout->ChangeManualBrowserControlType(Data->ManualControl, true);
    }*/


    SetTimer(hwnd, 0, 5, (TIMERPROC) NULL);

    ShowWindow(hwnd, SW_HIDE);
    UpdateWindow(hwnd);

    if(app->GetData()->IsRecord)
        CefInitialize(main_args, settings, app.get(), NULL);

    thread_setup();

    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        // Allow processing of dialog messages.
        if (Data->_BrowserContextMenu.find_hwnd_ && IsDialogMessage(Data->_BrowserContextMenu.find_hwnd_, &Msg))
          continue;

        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    thread_cleanup();

    WORKER_LOG("End Main Loop");

    if(app->GetData()->IsRecord)
        CefShutdown();

    WORKER_LOG("Exit");

    return Msg.wParam;
}

