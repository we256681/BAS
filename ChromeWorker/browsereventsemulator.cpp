#include "browsereventsemulator.h"
#include <windows.h>
#include <bitset>
#include "converter.h"
#include "include/cef_app.h"
#include "log.h"
#include <math.h>



CefBrowserHost::DragOperationsMask allowedops;


KeyState::KeyState()
{
    Clear();
}

void KeyState::Clear()
{
    IsShift = false;
    IsAlt = false;
    IsCtrl = false;
    IsPresingKey = false;
    IsClickingMouse = false;
}

bool KeyState::IsClear()
{
    return !IsShift && !IsAlt && !IsCtrl;
}

bool KeyState::IsPresingCharacter()
{
    return IsPresingKey;
}


void BrowserEventsEmulator::StartDrag(CefRefPtr<CefBrowser> Browser, CefRefPtr<CefDragData> drag_data,CefBrowserHost::DragOperationsMask allowed_ops, int x, int y, bool IsTouch, std::atomic_int& TouchId, std::atomic_bool& IsTouchPressedAutomation)
{
    if(IsTouch)
    {
        CefTouchEvent Event;
        Event.id = TouchId;
        Event.x = x;
        Event.y = y;
        Event.radius_x = 11.5;
        Event.radius_y = 11.5;
        Event.rotation_angle = 0.0;
        Event.pressure = 1.0;
        Event.pointer_type = CEF_POINTER_TYPE_TOUCH;
        Event.modifiers = EVENTFLAG_NONE;
        Event.type = CEF_TET_PRESSED;
        IsTouchPressedAutomation = true;

        Browser->GetHost()->SendTouchEvent(Event);
    }else
    {
        CefMouseEvent e;
        e.modifiers = EVENTFLAG_LEFT_MOUSE_BUTTON;

        e.x = x;
        e.y = y;
        allowedops = allowed_ops;
        Browser->GetHost()->DragTargetDragEnter(drag_data,e,allowed_ops);
        Browser->GetHost()->DragTargetDragOver(e,allowedops);
    }

}


BrowserEventsEmulator::BrowserEventsEmulator()
{

}

void BrowserEventsEmulator::SetFocus(CefRefPtr<CefBrowser> Browser)
{
    /*if(Browser)
        Browser->GetHost()->SendFocusEvent(true);*/
}

bool BrowserEventsEmulator::IsPointOnScreen(int PointX, int PointY, int ScrollX, int ScrollY, int BrowserWidth, int BrowserHeight)
{
    WORKER_LOG(
                std::string("IsPointOnScreen<<PointX") + std::to_string(PointX)
                + std::string("<<PointY") + std::to_string(PointY)
                + std::string("<<ScrollX") + std::to_string(ScrollX)
                + std::string("<<ScrollY") + std::to_string(ScrollY)
                +std::string("<<BrowserWidth") + std::to_string(BrowserWidth)
                + std::string("<<BrowserHeight") + std::to_string(BrowserHeight)
                );
    return PointX>=ScrollX && PointX<=ScrollX + BrowserWidth && PointY>=ScrollY && PointY<=ScrollY + BrowserHeight;
}

int random(int max)
{
    if(max <= 0)
        return 0;
    return rand() % (max);
}

void BrowserEventsEmulator::MouseMoveLine(IDevToolsConnector* Connector, bool & IsMouseMoveSimulation, int MouseStartX, int MouseStartY, int MouseEndX, int MouseEndY , int& MouseCurrentX, int& MouseCurrentY, float Speed, int BrowserWidth, int BrowserHeight, bool IsMousePress, bool IsDrag, bool IsTouch, std::atomic_int& TouchId, std::atomic_bool& IsTouchPressedAutomation, KeyState& State)
{
    if(!IsMouseMoveSimulation)
        return;

    MouseButton CurrentMouseState = MouseButtonNone;

    if(IsMousePress)
    {
        CurrentMouseState = MouseButtonLeft;
    }

    int CurrentKeyState = KeyboardModifiersNone;

    if(State.IsShift)
    {
        CurrentKeyState |= KeyboardModifiersShift;
    }
    if(State.IsAlt)
    {
        CurrentKeyState |= KeyboardModifiersAlt;
    }
    if(State.IsCtrl)
    {
        CurrentKeyState |= KeyboardModifiersCtrl;
    }

    float DistanceSquareAll = (MouseStartX - MouseEndX) * (MouseStartX - MouseEndX) + (MouseStartY - MouseEndY) * (MouseStartY - MouseEndY);
    float DistanceSquareCurrent = (MouseStartX - MouseCurrentX) * (MouseStartX - MouseCurrentX) + (MouseStartY - MouseCurrentY) * (MouseStartY - MouseCurrentY);

    float DistanceAll = sqrtf(DistanceSquareAll);
    float DistanceCurrent = sqrtf(DistanceSquareCurrent);

    DistanceCurrent += (Speed * 15.0) / 100.0;

    if(DistanceCurrent >= DistanceAll)
    {
        IsMouseMoveSimulation = false;
        MouseCurrentX = MouseEndX;
        MouseCurrentY = MouseEndY;
        if(IsTouch)
        {
            if(IsTouchPressedAutomation)
            {
                Connector->Touch(TouchEventMove,MouseCurrentX,MouseCurrentY,TouchId);
            }
        }else
        {
            Connector->Mouse(MouseEventMove,MouseCurrentX,MouseCurrentY,CurrentMouseState,CurrentMouseState,CurrentKeyState);

            if(IsDrag)
            {
                //Browser->GetHost()->DragTargetDragOver(e,allowedops);
            }

        }
        return;
    }

    float k = DistanceCurrent / (DistanceAll - DistanceCurrent);

    MouseCurrentX = ((float)MouseStartX + k * (float)MouseEndX) / (1.0 + k);
    MouseCurrentY = ((float)MouseStartY + k * (float)MouseEndY) / (1.0 + k);


    if(IsTouch)
    {
        if(IsTouchPressedAutomation)
        {
            Connector->Touch(TouchEventMove,MouseCurrentX,MouseCurrentY,TouchId);
        }
    }else
    {
        Connector->Mouse(MouseEventMove,MouseCurrentX,MouseCurrentY,CurrentMouseState,CurrentMouseState,CurrentKeyState);
        if(IsDrag)
        {
            //Browser->GetHost()->DragTargetDragOver(e,allowedops);
        }
    }
}

void BrowserEventsEmulator::MouseMove(IDevToolsConnector* Connector,
                                      bool & IsMouseMoveSimulation,
                                      int MouseStartX, int MouseStartY,
                                      int MouseEndX, int MouseEndY,
                                      int& MouseCurrentX, int& MouseCurrentY,
                                      float Speed,
                                      int BrowserWidth, int BrowserHeight,
                                      float Gravity, float Wind, float TargetArea,
                                      bool IsInit, bool IsDouble, bool IsMousePress, bool IsDrag,
                                      bool IsTouch, std::atomic_int& TouchId, std::atomic_bool& IsTouchPressedAutomation,
                                      KeyState& State)
{

    MouseButton CurrentMouseState = MouseButtonNone;

    if(IsMousePress)
    {
        CurrentMouseState = MouseButtonLeft;
    }

    int CurrentKeyState = KeyboardModifiersNone;

    if(State.IsShift)
    {
        CurrentKeyState |= KeyboardModifiersShift;
    }
    if(State.IsAlt)
    {
        CurrentKeyState |= KeyboardModifiersAlt;
    }
    if(State.IsCtrl)
    {
        CurrentKeyState |= KeyboardModifiersCtrl;
    }

    static float veloX,veloY,windX,windY,veloMag,dist,randomDist,lastDist,D;
    static int lastX,lastY,MSP,W,TDist;
    static int T;
    static float sqrt2,sqrt3,sqrt5,PDist,maxStep,dModA,dModB,nModA,nModB;

    if(IsInit)
    {
        veloX = 0;
        veloY = 0;
        windX = 0;
        windY = 0;
        veloMag = 0;
        dist = 0;
        randomDist = 0;
        lastDist = 0;
        D = 0;
        lastX = 0;
        lastY = 0;
        MSP = 0;
        W = 0;
        TDist = 0;
        T = 0;
        PDist = 0;
        maxStep = 0;

        MSP = 20;
        sqrt2 = sqrtf(2.0f);
        sqrt3 = sqrtf(3.0f);
        sqrt5 = sqrt(5.0f);

        TDist = hypot(MouseStartX-MouseEndX, MouseStartY-MouseEndY);
        if (TDist < 1)
            TDist = 1;

        dModA = 0.88;
        dModB = 0.95;

        if(TDist > 220)
        {
            nModA = 0.08;
            nModB = 0.04;
        }else
        {
            nModA = 0.20;
            nModB = 0.10;
        }
    }else
    {
        //WORKER_LOG(std::to_string(hypot(MouseCurrentX-MouseEndX, MouseCurrentY-MouseEndY)));

        //WORKER_LOG(std::to_string(MouseCurrentX-MouseEndX));
        //WORKER_LOG(std::to_string(MouseCurrentY-MouseEndY));

        if(hypot(MouseCurrentX-MouseEndX, MouseCurrentY-MouseEndY) <= 3)
        {
            IsMouseMoveSimulation = false;
            MouseCurrentX = MouseEndX;
            MouseCurrentY = MouseEndY;

            //if(MouseCurrentX >= 0 && MouseCurrentX <= BrowserWidth && MouseCurrentY >= 0 && MouseCurrentY <= BrowserHeight)
            if(IsTouch)
            {
                if(IsTouchPressedAutomation)
                {
                    Connector->Touch(TouchEventMove,MouseCurrentX,MouseCurrentY,TouchId);
                }
            }else
            {
                Connector->Mouse(MouseEventMove,MouseCurrentX,MouseCurrentY,CurrentMouseState,CurrentMouseState,CurrentKeyState);

                if(IsDrag)
                {
                    //Browser->GetHost()->DragTargetDragOver(e,allowedops);
                }

            }
            return;
        }

        dist = hypot(MouseCurrentX-MouseEndX, MouseCurrentY-MouseEndY);

        if(Wind > dist)
            Wind = dist;

        if(dist < 1)
        {
            dist = 1;
        }

        PDist = dist/TDist;
        if (PDist < 0.01)
        {
            PDist = 0.01;
        }

        if(IsDouble)
        {
            if (PDist <= dModA)
            {
                D = (round((round(dist)*0.3))/5);
                if (D < ((TDist > 220) ? 20 : 3))
                    D = ((TDist > 220) ? 20 : 3);

            } else if (PDist > dModA)
            {
                if (PDist < dModB)
                {
                    D = 5 + rand() % 4;
                }else if (PDist >= dModB)
                {
                    D = 3 + rand() % 2;
                }
            }
        }

        if (PDist >= nModA)
        {
            D = (round((round(dist)*0.3))/5);
            if (D < ((TDist > 220) ? 15 : 3))
            {
                D = ((TDist > 220) ? 15 : 3);
            }
        }else if (PDist < nModA)
        {
            if (PDist >= nModB)
            {
                D = 5 + rand() % 4;
            }
            else if (PDist < nModB)
            {
                D = 3 + rand() % 2;
            }
        }

        D *= Speed/100.0;
        if(D<3)
            D = 5;

        if (D <= round(dist))
            maxStep = D;
        else
            maxStep = round(dist);

        //maxStep = 4;

        if(dist >= TargetArea)
        {
            windX = (8.0 * windX / sqrt3 + 2.0 * (random((int)round(Wind) * 2 + 1) - Wind) / sqrt5) / 5.0;
            windY = (8.0 * windY / sqrt3 + 2.0 * (random((int)round(Wind) * 2 + 1) - Wind) / sqrt5) / 5.0;
        }else
        {
            windX = windX / sqrt2;
            windY = windY / sqrt2;
        }

        veloX = veloX + windX;
        veloY = veloY + windY;
        veloX = veloX + Gravity * (MouseEndX - MouseCurrentX) / dist;
        veloY = veloY + Gravity * (MouseEndY - MouseCurrentY) / dist;

        if (hypot(veloX, veloY) > maxStep)
        {
            randomDist = maxStep / 2.0 + random((int)round(maxStep) / 2);
            veloMag = sqrtf(veloX * veloX + veloY * veloY);
            veloX = (veloX / veloMag) * randomDist;
            veloY = (veloY / veloMag) * randomDist;
        }

        lastX = round(MouseCurrentX);
        lastY = round(MouseCurrentY);
        MouseCurrentX += veloX;
        MouseCurrentY += veloY;

        if((lastX != round(MouseCurrentX)) || (lastY != round(MouseCurrentY)))
        {
            //if(MouseCurrentX >= 0 && MouseCurrentX <= BrowserWidth && MouseCurrentY >= 0 && MouseCurrentY <= BrowserHeight)
            if(IsTouch)
            {
                if(IsTouchPressedAutomation)
                {
                    Connector->Touch(TouchEventMove,MouseCurrentX,MouseCurrentY,TouchId);
                }
            }else
            {
                Connector->Mouse(MouseEventMove,MouseCurrentX,MouseCurrentY,CurrentMouseState,CurrentMouseState,CurrentKeyState);
                if(IsDrag)
                {
                    //Browser->GetHost()->DragTargetDragOver(e,allowedops);
                }

            }
        }

        W = (random((int)round(100/MSP)))*6;
        if (W < 5)
        {
            W = 5;
        }

        if(IsDouble)
        {
            if (PDist > dModA)
                W = round(W*2.5);
        }else
        {
            W = round(W*1.2);
        }

        lastDist = dist;

    }

    /*if(!IsMouseMoveSimulation)
        return;

    float DistanceSquareAll = (MouseStartX - MouseEndX) * (MouseStartX - MouseEndX) + (MouseStartY - MouseEndY) * (MouseStartY - MouseEndY);
    float DistanceSquareCurrent = (MouseStartX - MouseCurrentX) * (MouseStartX - MouseCurrentX) + (MouseStartY - MouseCurrentY) * (MouseStartY - MouseCurrentY);

    float DistanceAll = sqrtf(DistanceSquareAll);
    float DistanceCurrent = sqrtf(DistanceSquareCurrent);

    DistanceCurrent += Speed;

    if(DistanceCurrent >= DistanceAll)
    {
        IsMouseMoveSimulation = false;
        MouseCurrentX = MouseEndX;
        MouseCurrentY = MouseEndY;
        return;
    }

    float k = DistanceCurrent / (DistanceAll - DistanceCurrent);

    MouseCurrentX = ((float)MouseStartX + k * (float)MouseEndX) / (1.0 + k);
    MouseCurrentY = ((float)MouseStartY + k * (float)MouseEndY) / (1.0 + k);


    if(MouseCurrentX >= 0 && MouseCurrentX <= BrowserWidth && MouseCurrentY >= 0 && MouseCurrentY <= BrowserHeight)
    {
        CefMouseEvent e;
        e.x = MouseCurrentX;
        e.y = MouseCurrentY;

        Browser->GetHost()->SendMouseMoveEvent(e,false);
    }*/


}

Async BrowserEventsEmulator::MouseClick(IDevToolsConnector* Connector, int x, int y, const std::pair<int,int> scroll, int type, bool& IsMousePress, bool& IsDrag, bool IsTouch, std::atomic_int& TouchId, std::atomic_bool& IsTouchPressedAutomation, KeyState& State)
{
    int X = x - scroll.first;
    int Y = y - scroll.second;

    int CurrentMouseState = MouseButtonNone;

    if(IsMousePress)
    {
        CurrentMouseState |= MouseButtonLeft;
    }

    if(type == 2)
    {
        IsMousePress = true;
    }

    if(type == 1)
    {
        IsMousePress = false;
    }

    int CurrentKeyState = KeyboardModifiersNone;

    if(State.IsShift)
    {
        CurrentKeyState |= KeyboardModifiersShift;
    }
    if(State.IsAlt)
    {
        CurrentKeyState |= KeyboardModifiersAlt;
    }
    if(State.IsCtrl)
    {
        CurrentKeyState |= KeyboardModifiersCtrl;
    }


    if(type == 1 && IsDrag)
    {
        /*Browser->GetHost()->DragTargetDragOver(event,allowedops);
        Browser->GetHost()->DragSourceEndedAt(event.x,event.y,allowedops);


        Browser->GetHost()->DragTargetDrop(event);
        Browser->GetHost()->DragSourceSystemDragEnded();*/
        IsDrag = false;
    }

    Async Result;

    if(type != 1)
    {
        if(IsTouch)
        {
            IsTouchPressedAutomation = true;
            Result = Connector->Touch(TouchEventDown,X,Y,TouchId);
        }else
        {
            Connector->Mouse(MouseEventDown,X,Y,MouseButtonLeft,CurrentMouseState,CurrentKeyState);
        }
    }

    if(type != 2)
    {
        if(IsTouch)
        {
            IsTouchPressedAutomation = false;
            Result = Connector->Touch(TouchEventUp,X,Y,TouchId);
            TouchId++;
        }else
        {
            Connector->Mouse(MouseEventUp,X,Y,MouseButtonLeft,CurrentMouseState,CurrentKeyState);
        }
    }

    return Result;
}

void BrowserEventsEmulator::Key(IDevToolsConnector *Connector, std::string & text, KeyState& State, int mousex, int mousey, bool IsTouch)
{
    MouseButton CurrentMouseState = MouseButtonNone;

    int CurrentKeyState = KeyboardModifiersNone;

    if(State.IsShift)
    {
        CurrentKeyState |= KeyboardModifiersShift;
    }
    if(State.IsAlt)
    {
        CurrentKeyState |= KeyboardModifiersAlt;
    }
    if(State.IsCtrl)
    {
        CurrentKeyState |= KeyboardModifiersCtrl;
    }

    if(State.IsPresingKey)
    {
        if(State.IsClickingMouse)
        {
            CurrentMouseState = (State.MouseUpIsRight) ? MouseButtonRight : MouseButtonLeft;
            Connector->Mouse(MouseEventUp,State.MouseUpX,State.MouseUpY,CurrentMouseState,CurrentMouseState,CurrentKeyState);
        }else
        {
            Connector->Key(KeyEventUp,State.PresingString,CurrentKeyState);
        }
        State.IsPresingKey = false;
        State.IsClickingMouse = false;
        return;
    }

    bool AdditionalIsShift = false;
    bool AdditionalIsAlt = false;
    bool AdditionalIsCtrl = false;

    std::string text_copy = text;
    while(true)
    {
        if(text_copy.rfind("<SHIFT>", 0) == 0)
        {
            AdditionalIsShift = true;
            text_copy.erase(text_copy.begin(),text_copy.begin() + 7);
            continue;
        }
        if(text_copy.rfind("<MENU>", 0) == 0)
        {
            AdditionalIsAlt = true;
            text_copy.erase(text_copy.begin(),text_copy.begin() + 5);
            continue;
        }
        if(text_copy.rfind("<CONTROL>", 0) == 0)
        {
            AdditionalIsCtrl = true;
            text_copy.erase(text_copy.begin(),text_copy.begin() + 9);
            continue;
        }
        break;
    }

    if(text_copy.empty() && State.IsClear())
        return;

    std::wstring text_whcar = s2ws(text_copy);
    if(text_whcar.empty() && State.IsClear())
        return;

    bool IsKeyParsed = false;
    unsigned char key = 0;
    char state = -1;
    wchar_t letter_wchar = 0;
    std::string InputString;
    bool ismouse = false;
    bool ismouseup = false;
    bool ismouseleft = false;
    bool ismouseright = false;

    bool is_special_letter = true;
    int character_length = 1;
    int MouseClickTimes = 1;


    if(!text_copy.empty())
    {
        std::string CANCEL = "<CANCEL>";
        std::string BACK = "<BACK>";
        std::string TAB = "<TAB>";
        std::string CLEAR = "<CLEAR>";
        std::string RETURN = "<RETURN>";
        std::string CAPITAL = "<CAPITAL>";
        std::string ESCAPE = "<ESCAPE>";
        std::string PRIOR = "<PRIOR>";
        std::string NEXT = "<NEXT>";
        std::string END = "<END>";
        std::string HOME = "<HOME>";
        std::string LEFT = "<LEFT>";
        std::string UP = "<UP>";
        std::string RIGHT = "<RIGHT>";
        std::string DOWN = "<DOWN>";
        std::string SELECT = "<SELECT>";
        std::string PRINT = "<PRINT>";
        std::string EXECUTE = "<EXECUTE>";
        std::string SNAPSHOT = "<SNAPSHOT>";
        std::string INSERT = "<INSERT>";
        std::string _DELETE = "<DELETE>";
        std::string MOUSESCROLLUP = "<MOUSESCROLLUP>";
        std::string MOUSESCROLLDOWN = "<MOUSESCROLLDOWN>";
        std::string MOUSELEFT = "<MOUSELEFT>";
        std::string MOUSERIGHT = "<MOUSERIGHT>";
        std::string MOUSEDOUBLE = "<MOUSEDOUBLE>";

        if(text_copy.rfind(MOUSESCROLLUP, 0) == 0)
        {
            character_length = MOUSESCROLLUP.length();
            ismouse = true;
            ismouseup = true;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(MOUSESCROLLDOWN, 0) == 0)
        {
            character_length = MOUSESCROLLDOWN.length();
            ismouse = true;
            ismouseup = false;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(MOUSELEFT, 0) == 0)
        {
            character_length = MOUSELEFT.length();
            ismouseleft = true;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(MOUSERIGHT, 0) == 0)
        {
            character_length = MOUSERIGHT.length();
            ismouseright = true;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(MOUSEDOUBLE, 0) == 0)
        {
            character_length = MOUSEDOUBLE.length();
            ismouseleft = true;
            MouseClickTimes = 2;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(CANCEL, 0) == 0)
        {
            character_length = CANCEL.length();
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(BACK, 0) == 0)
        {
            character_length = BACK.length();
            InputString = CharacterBackspace;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(TAB, 0) == 0)
        {
            character_length = TAB.length();
            InputString = CharacterTab;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(CLEAR, 0) == 0)
        {
            character_length = CLEAR.length();
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(RETURN, 0) == 0)
        {
            character_length = RETURN.length();
            InputString = CharacterEnter;
            is_special_letter = false;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(CAPITAL, 0) == 0)
        {
            character_length = CAPITAL.length();
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(ESCAPE, 0) == 0)
        {
            character_length = ESCAPE.length();
            InputString = CharacterEscape;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(PRIOR, 0) == 0)
        {
            character_length = PRIOR.length();
            InputString = CharacterPageUp;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(NEXT, 0) == 0)
        {
            character_length = NEXT.length();
            InputString = CharacterPageDown;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(END, 0) == 0)
        {
            character_length = END.length();
            InputString = CharacterEnd;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(HOME, 0) == 0)
        {
            character_length = HOME.length();
            InputString = CharacterHome;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(LEFT, 0) == 0)
        {
            character_length = LEFT.length();
            InputString = CharacterLeft;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(UP, 0) == 0)
        {
            character_length = UP.length();
            InputString = CharacterUp;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(RIGHT, 0) == 0)
        {
            character_length = RIGHT.length();
            InputString = CharacterRight;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(DOWN, 0) == 0)
        {
            character_length = DOWN.length();
            InputString = CharacterDown;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(SELECT, 0) == 0)
        {
            character_length = SELECT.length();
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(PRINT, 0) == 0)
        {
            character_length = PRINT.length();
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(EXECUTE, 0) == 0)
        {
            character_length = EXECUTE.length();
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(SNAPSHOT, 0) == 0)
        {
            character_length = SNAPSHOT.length();
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(INSERT, 0) == 0)
        {
            character_length = INSERT.length();
            InputString = CharacterInsert;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else if(text_copy.rfind(_DELETE, 0) == 0)
        {
            character_length = _DELETE.length();
            InputString = CharacterDelete;
            text_copy.erase(text_copy.begin(),text_copy.begin() + character_length);
        }else
        {
            is_special_letter = false;
            if(!text_whcar.empty())
                letter_wchar = text_whcar.at(0);

            InputString = GetFirstUtf8Char(text_copy);
            std::vector<HKL> Locales;
            int max_locales = 300;
            Locales.resize(max_locales);
            max_locales = GetKeyboardLayoutList(max_locales,Locales.data());

            int index = 0;
            while((!IsKeyParsed || state == -1) && index < max_locales)
            {
                short c = VkKeyScanEx(letter_wchar,Locales[index]);
                key = c & 0xFF;
                IsKeyParsed = true;
                state = c >> 8;
                index ++;
            }



        }
    }

    bool IsShift = false;
    bool IsAlt = false;
    bool IsCtrl = false;

    if(state != -1)
    {
        IsShift = state&1;
        IsCtrl = state&2;
        IsAlt = state&4;
    }

    if(AdditionalIsShift)
    {
        IsShift = true;
    }

    if(AdditionalIsAlt)
    {
        IsAlt = true;
    }

    if(AdditionalIsCtrl)
    {
        IsCtrl = true;
    }


    CurrentKeyState = KeyboardModifiersNone;

    if(IsShift)
    {
        CurrentKeyState |= KeyboardModifiersShift;
    }
    if(IsAlt)
    {
        CurrentKeyState |= KeyboardModifiersAlt;
    }
    if(IsCtrl)
    {
        CurrentKeyState |= KeyboardModifiersCtrl;
    }

    if(!ismouse)
    {
        if(IsShift != State.IsShift)
        {
            State.IsShift = IsShift;
            Connector->Key((IsShift) ? KeyEventDown : KeyEventUp,CharacterShift,CurrentKeyState);
            return;
        }

        if(IsAlt != State.IsAlt)
        {
            State.IsAlt = IsAlt;
            Connector->Key((IsAlt) ? KeyEventDown : KeyEventUp,CharacterAlt,CurrentKeyState);
            return;
        }

        if(IsCtrl != State.IsCtrl)
        {
            State.IsCtrl = IsCtrl;
            Connector->Key((IsCtrl) ? KeyEventDown : KeyEventUp,CharacterCtrl,CurrentKeyState);
            return;
        }

        if(ismouseleft)
        {
            CurrentMouseState = MouseButtonLeft;

            Connector->Mouse(MouseEventDown,mousex,mousey,CurrentMouseState,CurrentMouseState,CurrentKeyState,MouseClickTimes);

            if(MouseClickTimes == 1)
            {
                //Postpond mouse up
                State.IsClickingMouse = true;
                State.MouseUpIsRight = false;
                State.MouseUpX = mousex;
                State.MouseUpY = mousey;
                State.IsPresingKey = true;

            }else
            {
                //Mouse up immediatelly                
                Connector->Mouse(MouseEventUp,mousex,mousey,CurrentMouseState,CurrentMouseState,CurrentKeyState,MouseClickTimes);
            }


        }
        else if(ismouseright)
        {
            CurrentMouseState = MouseButtonRight;

            Connector->Mouse(MouseEventDown,mousex,mousey,CurrentMouseState,CurrentMouseState,CurrentKeyState,MouseClickTimes);

            if(MouseClickTimes == 1)
            {
                //Postpond mouse up
                State.IsClickingMouse = true;
                State.MouseUpIsRight = true;
                State.MouseUpX = mousex;
                State.MouseUpY = mousey;
                State.IsPresingKey = true;

            }else
            {
                //Mouse up immediatelly                
                Connector->Mouse(MouseEventUp,mousex,mousey,CurrentMouseState,CurrentMouseState,CurrentKeyState,MouseClickTimes);
            }
        }else
        {

            //Main key down
            {
                Connector->Key(KeyEventDown,InputString,CurrentKeyState);
            }
            //Main key press
            if(!is_special_letter && ((!IsCtrl && !IsAlt) || (IsAlt && IsCtrl)))
            {
                Connector->Key(KeyEventCharacter,InputString,CurrentKeyState);
            }
            //Main key up delay


            State.IsClickingMouse = false;
            State.IsPresingKey = true;
            State.PresingString = InputString;
        }
    }
    else
    {
        Connector->Wheel(mousex, mousey, ismouseup);
    }

    text = text_copy;
}

