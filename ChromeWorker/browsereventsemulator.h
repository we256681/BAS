#ifndef BROWSEREVENTSEMULATOR_H
#define BROWSEREVENTSEMULATOR_H

#include "include/cef_app.h"
#include <atomic>
#include "idevtoolsconnector.h"


struct KeyState
{
    bool IsShift;
    bool IsAlt;
    bool IsCtrl;

    bool IsPresingKey;
    std::string PresingString;
    clock_t PresingKeyNext;

    int MouseUpX;
    int MouseUpY;
    bool MouseUpIsRight;
    bool IsClickingMouse;

    KeyState();
    void Clear();
    bool IsClear();
    bool IsPresingCharacter();
};

class BrowserEventsEmulator
{

public:
    BrowserEventsEmulator();
    static void SetFocus(CefRefPtr<CefBrowser> Browser);
    /*
     * @type == 0 - up and down
     * @type == 1 - up
     * @type == 2 - down
     * */
    static Async MouseClick(IDevToolsConnector* Connector, int x, int y, const std::pair<int,int> scroll, int type, bool& IsMousePress, bool& IsDrag, bool IsTouch, std::atomic_int& TouchId, std::atomic_bool& IsTouchPressedAutomation, KeyState& State);

    static void StartDrag(CefRefPtr<CefBrowser> Browser, CefRefPtr<CefDragData> drag_data,CefBrowserHost::DragOperationsMask allowed_ops, int x, int y, bool IsTouch, std::atomic_int& TouchId, std::atomic_bool& IsTouchPressedAutomation);

    static void MouseMove(IDevToolsConnector* Connector,
                          bool & IsMouseMoveSimulation,
                          int MouseStartX, int MouseStartY,
                          int MouseEndX, int MouseEndY,
                          int& MouseCurrentX, int& MouseCurrentY,
                          float Speed,
                          int BrowserWidth, int BrowserHeight,
                          float Gravity, float Wind, float TargetArea,
                          bool IsInit, bool IsDouble,
                          bool IsMousePress, bool IsDrag, bool IsTouch, std::atomic_int& TouchId, std::atomic_bool& IsTouchPressedAutomation,
                          KeyState& State
                          );
    static void MouseMoveLine(IDevToolsConnector* Connector, bool & IsMouseMoveSimulation, int MouseStartX, int MouseStartY, int MouseEndX, int MouseEndY , int& MouseCurrentX, int& MouseCurrentY, float Speed, int BrowserWidth, int BrowserHeight,bool IsMousePress, bool IsDrag, bool IsTouch, std::atomic_int& TouchId, std::atomic_bool& IsTouchPressedAutomation, KeyState& State);
    static bool IsPointOnScreen(int PointX, int PointY, int ScrollX, int ScrollY, int BrowserWidth, int BrowserHeight);
    static void Key(IDevToolsConnector* Connector, std::string & text, KeyState& State, int mousex, int mousey, bool IsTouch);
};

#endif // BROWSEREVENTSEMULATOR_H
