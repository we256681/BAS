#include "browserdirectcontrol.h"
#include "handlersmanager.h"
#include "base64.h"
#include <chrono>
#include "converter.h"
#include "startwith.h"
#include "multithreading.h"
#include "KeyboardEmulation.h"


using namespace std::chrono;



//
//  Helpers
//

std::string BrowserDirectControl::JsonEscape(const std::string& Text)
{
    return picojson::value(Text).serialize();
}

std::string BrowserDirectControl::JsonEscapeInsideString(const std::string & Text)
{
    std::string res = JsonEscape(Text);
    if(!res.empty())
    {
        res.erase(0,1);
    }
    if(!res.empty())
    {
        res.erase(res.length() - 1,1);
    }
    return res;
}

int64 BrowserDirectControl::Now()
{
    return duration_cast< milliseconds >( system_clock::now().time_since_epoch() ).count();
}

bool BrowserDirectControl::IsKeyDown(WPARAM wparam)
{
    return (GetKeyState(wparam) & 0x8000) != 0;
}


//
//  Mouse clicks
//


BrowserDirectControl::InspectTask BrowserDirectControl::GetInspectTask()
{
    //TID_UI

    InspectTask Res;
    Res.IsNull = true;

    if(_BrowserData->ManualControl != BrowserData::DirectRecord)
    {
        IsInspecting = false;
        MouseClicks.clear();
        return Res;
    }

    if(IsInspecting || MouseClicks.empty())
    {
        return Res;
    }

    Res.IsNull = false;

    Res.X = MouseClicks.at(0).X;
    Res.Y = MouseClicks.at(0).Y;

    //WORKER_LOG("!!!!!! Start inspect mouse" + std::to_string(Res.X) + ", " + std::to_string(Res.Y));

    IsInspecting = true;

    return Res;
}

void BrowserDirectControl::TimeoutLastInspect()
{
    //TID_UI

    if(_BrowserData->ManualControl != BrowserData::DirectRecord)
    {
        IsInspecting = false;
        MouseClicks.clear();
        return;
    }

    if(!IsInspecting)
        return;

    if(MouseClicks.empty())
        return;

    //WORKER_LOG("!!!!!! Timeout inspect mouse");


    MouseClicks.erase(MouseClicks.begin(),MouseClicks.begin() + 1);
}

void BrowserDirectControl::ApplyInspectResult(MouseClickItem Item, InspectResult Inspect)
{
    if(_BrowserData->ManualControl != BrowserData::DirectRecord)
    {
        IsInspecting = false;
        MouseClicks.clear();
        return;
    }

    //TID_UI
    bool IsDownOrUp = Item.IsDownOrUp;
    bool IsDrop = Item.IsDrop;
    bool IsDoubleClick = Item.IsDoubleClick;


    //Add action to scenario
    std::string code;
    std::string json;

    if(Inspect.css.empty())
    {
        if(IsDownOrUp)
            PreviousMouseInspect.css.clear();
        return;
    }

    if(Inspect.css3 == Inspect.css2 || Inspect.css3 == Inspect.css)
    {
        Inspect.css3.clear();
    }

    if(Inspect.css2 == Inspect.css3 || Inspect.css2 == Inspect.css)
    {
        Inspect.css2.clear();
        Inspect.css2 = Inspect.css3;
    }


    //Mouse down, maybe drag and drop
    if(IsDownOrUp)
    {
        //Drag start
        PreviousMouseInspect = Inspect;
        PreviousMouseCode = std::string("_SELECTOR = ") + JsonEscape(Inspect.css) + std::string(";\n") +
                std::string("wait_element_visible(_SELECTOR)!\n") +
                std::string("_call(_random_point, {})!\n") +
                std::string("_if(_result().length > 0, function(){\n") +
                std::string("move( {} )!\n") +
                std::string("get_element_selector(_SELECTOR, false).clarify(X,Y)!\n") +
                std::string("_call(_clarify, {} )!\n") +
                std::string("mouse_down(X,Y)!\n") +
                std::string("})!");

        PreviousMouseMeta = std::string("{\"s\":\"dragelement\",\"v\":1,\"f\":[],\"uw\":\"0\",\"ut\":\"0\",\"uto\":\"0\",\"um\":\"0\",\"d\":[{\"id\":\"Speed\",\"type\":\"constr\",\"data\":\"100\",\"class\":\"expression\"},{\"id\":\"Gravity\",\"type\":\"constr\",\"data\":\"6\",\"class\":\"expression\"},{\"id\":\"Deviation\",\"type\":\"constr\",\"data\":\"2.5\",\"class\":\"expression\"}],\"p\":{\"is_image\":false,\"css\":") +
                JsonEscape(Inspect.css) +
                std::string(",\"version\":\"1.0\",\"css1\":") +
                JsonEscape(Inspect.css) +
                std::string(",\"css2\":") +
                JsonEscape(Inspect.css2) +
                std::string(",\"css3\":") +
                JsonEscape(Inspect.css3) +
                std::string(",\"current\":\"css\",\"match\":") +
                JsonEscape(Inspect.match) +
                std::string(",\"xpath\":") +
                JsonEscape(Inspect.xpath) +
                std::string(",\"at\":\"") +
                std::to_string(Inspect.x) +
                std::string(", ") +
                std::to_string(Inspect.y) +
                std::string("\",\"we\":true,\"fa\":true}}");

        return;
    }

    if(IsDrop)
    {
        //Drop

        code = std::string("_SELECTOR = ") + JsonEscape(Inspect.css) + std::string(";\n") +
                std::string("wait_element_visible(_SELECTOR)!\n") +
                std::string("_call(_random_point, {})!\n") +
                std::string("_if(_result().length > 0, function(){\n") +
                std::string("move( {} )!\n") +
                std::string("get_element_selector(_SELECTOR, false).clarify(X,Y)!\n") +
                std::string("_call(_clarify, {} )!\n") +
                std::string("mouse_up(X,Y)!\n") +
                std::string("})!");

        json = std::string("{\"s\":\"dropelement\",\"v\":1,\"f\":[],\"uw\":\"0\",\"ut\":\"0\",\"uto\":\"0\",\"um\":\"0\",\"d\":[{\"id\":\"Speed\",\"type\":\"constr\",\"data\":\"100\",\"class\":\"expression\"},{\"id\":\"Gravity\",\"type\":\"constr\",\"data\":\"6\",\"class\":\"expression\"},{\"id\":\"Deviation\",\"type\":\"constr\",\"data\":\"2.5\",\"class\":\"expression\"}],\"p\":{\"is_image\":false,\"css\":") +
                JsonEscape(Inspect.css) +
                std::string(",\"version\":\"1.0\",\"css1\":") +
                JsonEscape(Inspect.css) +
                std::string(",\"css2\":") +
                JsonEscape(Inspect.css2) +
                std::string(",\"css3\":") +
                JsonEscape(Inspect.css3) +
                std::string(",\"current\":\"css\",\"match\":") +
                JsonEscape(Inspect.match) +
                std::string(",\"xpath\":") +
                JsonEscape(Inspect.xpath) +
                std::string(",\"at\":\"") +
                std::to_string(Inspect.x) +
                std::string(", ") +
                std::to_string(Inspect.y) +
                std::string("\",\"we\":true,\"fa\":true}}");

        AddItem(json, code, std::string(), DragEndType, Inspect);
    }else
    {
        if(!IsDoubleClick)
        {
            //Single click
            code = std::string("_SELECTOR = ") + JsonEscape(Inspect.css) + std::string(";\n") +
                               std::string("wait_element_visible(_SELECTOR)!\n") +
                               std::string("_call(_random_point, {})!\n") +
                               std::string("_if(_result().length > 0, function(){\n") +
                               std::string("move( {} )!\n") +
                               std::string("get_element_selector(_SELECTOR, false).clarify(X,Y)!\n") +
                               std::string("_call(_clarify, {} )!\n") +
                               std::string("mouse(X,Y)!\n") +
                               std::string("})!");
            json =
                std::string("{\"s\":\"moveandclickelement\",\"v\":1,\"f\":[],\"uw\":\"0\",\"ut\":\"0\",\"uto\":\"0\",\"um\":\"0\",\"d\":[{\"id\":\"Check\",\"type\":\"check\",\"data\":false},{\"id\":\"Select\",\"type\":\"select\",\"data\":\"left\"},{\"id\":\"Speed\",\"type\":\"constr\",\"data\":\"100\",\"class\":\"expression\"},{\"id\":\"Gravity\",\"type\":\"constr\",\"data\":\"6\",\"class\":\"expression\"},{\"id\":\"Deviation\",\"type\":\"constr\",\"data\":\"2.5\",\"class\":\"expression\"}],\"p\":{\"is_image\":false,\"css\":") +
                JsonEscape(Inspect.css) +
                std::string(",\"version\":\"1.0\",\"css1\":") +
                JsonEscape(Inspect.css) +
                std::string(",\"css2\":") +
                JsonEscape(Inspect.css2) +
                std::string(",\"css3\":") +
                JsonEscape(Inspect.css3) +
                std::string(",\"current\":\"css\",\"match\":") +
                JsonEscape(Inspect.match) +
                std::string(",\"xpath\":") +
                JsonEscape(Inspect.xpath) +
                std::string(",\"at\":\"") +
                std::to_string(Inspect.x) +
                std::string(", ") +
                std::to_string(Inspect.y) +
                std::string("\",\"we\":true,\"fa\":true}}");



            AddItem(json, code, std::string(), ClickType, Inspect);
        }else
        {
            //Double click

            code = std::string("_SELECTOR = ") + JsonEscape(Inspect.css) + std::string(";\n") +
                               std::string("wait_element_visible(_SELECTOR)!\n") +
                               std::string("_call(_random_point, {})!\n") +
                               std::string("_if(_result().length > 0, function(){\n") +
                               std::string("move( {} )!\n") +
                               std::string("get_element_selector(_SELECTOR, false).clarify(X,Y)!\n") +
                               std::string("_call(_clarify, {} )!\n") +
                               std::string("_type(\"<MOUSEDOUBLE>\",100)!\n") +
                               std::string("})!");


            json =
                std::string("{\"s\":\"moveandclickelement\",\"v\":1,\"f\":[],\"uw\":\"0\",\"ut\":\"0\",\"uto\":\"0\",\"um\":\"0\",\"d\":[{\"id\":\"Check\",\"type\":\"check\",\"data\":false},{\"id\":\"Select\",\"type\":\"select\",\"data\":\"double\"},{\"id\":\"Speed\",\"type\":\"constr\",\"data\":\"100\",\"class\":\"expression\"},{\"id\":\"Gravity\",\"type\":\"constr\",\"data\":\"6\",\"class\":\"expression\"},{\"id\":\"Deviation\",\"type\":\"constr\",\"data\":\"2.5\",\"class\":\"expression\"}],\"p\":{\"is_image\":false,\"css\":") +
                JsonEscape(Inspect.css) +
                std::string(",\"version\":\"1.0\",\"css1\":") +
                JsonEscape(Inspect.css) +
                std::string(",\"css2\":") +
                JsonEscape(Inspect.css2) +
                std::string(",\"css3\":") +
                JsonEscape(Inspect.css3) +
                std::string(",\"current\":\"css\",\"match\":") +
                JsonEscape(Inspect.match) +
                std::string(",\"xpath\":") +
                JsonEscape(Inspect.xpath) +
                std::string(",\"at\":\"") +
                std::to_string(Inspect.x) +
                std::string(", ") +
                std::to_string(Inspect.y) +
                std::string("\",\"we\":true,\"fa\":true}}");

            AddItem(json, code, std::string(), DoubleClickType, Inspect);

        }
    }

}

void BrowserDirectControl::SetInspectResult(InspectResult Inspect)
{
    //TID_UI

    if(_BrowserData->ManualControl != BrowserData::DirectRecord)
    {
        IsInspecting = false;
        MouseClicks.clear();
        return;
    }

    if(!IsInspecting)
        return;

    if(MouseClicks.empty())
        return;




    IsInspecting = false;
    MouseClickItem Item = MouseClicks.at(0);

    MouseClicks.erase(MouseClicks.begin(),MouseClicks.begin() + 1);

    //WORKER_LOG("!!!!!! Inspect mouse result " + std::to_string(Item.X) + ", " + std::to_string(Item.Y) + ". " + Inspect.css);

    DoMouseEvent(Item);
    ApplyInspectResult(Item, Inspect);
}


//
//  Working with sequence
//

void BrowserDirectControl::SendCode(const std::string& Meta,const std::string& Code, bool DontSendNextWaitFullPageLoaded, bool IsWaitFullPageLoaded)
{
    if(this->DontSendNextWaitFullPageLoaded && IsWaitFullPageLoaded)
        return;

    this->DontSendNextWaitFullPageLoaded = DontSendNextWaitFullPageLoaded;

    std::string base64_json = base64_encode((unsigned char const *)Meta.data(),Meta.size());

    std::string task = std::string(";var NewId = Math.floor(Math.random() * (1000000000 - 100)) + 100;BrowserAutomationStudio_AddTask(\"\",\"\\/*Dat:") + base64_json + std::string("*\\/\\n\\/*Browser*\\/\\n") + JsonEscapeInsideString(Code) + std::string("\",NewId,true,true);BrowserAutomationStudio_FocusActionFast(NewId);");

    for(auto f:EventExecuteScenarioCode)
        f(task);

}

void BrowserDirectControl::SendDetector(const std::string& Name,const std::string& Description)
{
    if(!_BrowserData->IsRecord)
        return;

    std::string Code = std::string("BrowserAutomationStudio_SendDetectorBrowserData(JSON.stringify({type: \"browser\", id: \"manual\", name: tr(") +
            JsonEscape(Name) +
        std::string("), is_image: false, description: ") +
            JsonEscape(Description) +
        std::string(", comment: \"\", timestamp: ") +
            std::to_string(Now()) +
        std::string("}))");

    for(auto f:EventExecuteScenarioCode)
        f(Code);
}

void BrowserDirectControl::SendSequenceItem(const SequenceItem& Item)
{
    std::string Meta;
    std::string Code;
    bool DontSendNextWaitFullPageLoaded;
    bool IsWaitFullPageLoaded;

    Meta = Item.Meta;
    Code = Item.Code;
    IsWaitFullPageLoaded = Item.Type == WaitFullPageLoadType;
    DontSendNextWaitFullPageLoaded = Item.Type == PopupSelect || Item.Type == PopupClose || Item.Type == LoadType || Item.Type == WaitFullPageLoadType || Item.Type == GoBackType;

    SendCode(Meta, Code, DontSendNextWaitFullPageLoaded, IsWaitFullPageLoaded);
}

void BrowserDirectControl::SendSequenceItems(std::vector<SequenceItem>& Items)
{
    SequenceItem Main;
    bool IsMain = false;
    SequenceItem Wait;
    bool IsWait = false;

    //Get main item
    for(SequenceItem& Item: Items)
    {
        if(Item.Type != WaitFullPageLoadType)
        {
            IsMain = true;
            Main = Item;
            break;
        }
    }

    //Get wait item
    for(SequenceItem& Item: Items)
    {
        if(Item.Type == WaitFullPageLoadType)
        {
            IsWait = true;
            Wait = Item;
            break;
        }
    }

    if(IsMain)
    {
        //Get text
        bool IsFirst = true;
        for(SequenceItem& Item: Items)
        {
            if(Item.Type != WaitFullPageLoadType)
            {
                if(!IsFirst)
                    Main.Text += Item.Text;
                IsFirst = false;
            }
        }

        //Update main type
        if(!Main.Text.empty())
        {
            if(Main.Type == ClickType)
            {
                Main.Code = std::string("_SELECTOR = ") + JsonEscape(Main.Inspect.css) + std::string(";\n") +
                                    std::string("wait_element_visible(_SELECTOR)!\n") +
                                    std::string("_call(_random_point, {})!\n") +
                                    std::string("_if(_result().length > 0, function(){\n") +
                                    std::string("move( {} )!\n") +
                                    std::string("get_element_selector(_SELECTOR, false).clarify(X,Y)!\n") +
                                    std::string("_call(_clarify, {} )!\n") +
                                    std::string("mouse(X,Y)!\n") +
                                    std::string("_type(") + JsonEscape(Main.Text) + std::string(",100)!\n") +
                                    std::string("})!");




                Main.Meta =
                        std::string("{\"s\":\"typeelement\",\"v\":1,\"f\":[],\"uw\":\"0\",\"ut\":\"0\",\"uto\":\"0\",\"um\":\"0\",\"d\":[{\"id\":\"TypeData\",\"type\":\"constr\",\"data\":") +
                        JsonEscape(Main.Text) +
                        std::string(",\"class\":\"string\"},{\"id\":\"TypeInterval\",\"type\":\"constr\",\"data\":\"100\",\"class\":\"int\"},{\"id\":\"Check\",\"type\":\"check\",\"data\":false},{\"id\":\"Speed\",\"type\":\"constr\",\"data\":\"100\",\"class\":\"expression\"},{\"id\":\"Gravity\",\"type\":\"constr\",\"data\":\"6\",\"class\":\"expression\"},{\"id\":\"Deviation\",\"type\":\"constr\",\"data\":\"2.5\",\"class\":\"expression\"}],\"p\":{\"is_image\":false,\"css\":") +
                        JsonEscape(Main.Inspect.css) +
                        std::string(",\"version\":\"1.0\",\"css1\":") +
                        JsonEscape(Main.Inspect.css) +
                        std::string(",\"css2\":") +
                        JsonEscape(Main.Inspect.css2) +
                        std::string(",\"css3\":") +
                        JsonEscape(Main.Inspect.css3) +
                        std::string(",\"current\":\"css\",\"match\":") +
                        JsonEscape(Main.Inspect.match) +
                        std::string(",\"xpath\":") +
                        JsonEscape(Main.Inspect.xpath) +
                        std::string(",\"at\":\"") +
                        std::to_string(Main.Inspect.x) +
                        std::string(", ") +
                        std::to_string(Main.Inspect.y) +
                        std::string("\",\"we\":true,\"fa\":true}}");

                SendDetector("Type Text",Main.Text + std::string(" -> ") + Main.Inspect.css);


            }else if(Main.Type == KeyType)
            {
                Main.Code = std::string("_type(") + JsonEscape(Main.Text) + std::string(",100)!");
                Main.Meta = std::string("{\"s\":\"type\",\"v\":1,\"f\":[],\"uw\":\"0\",\"ut\":\"0\",\"uto\":\"0\",\"um\":\"0\",\"d\":[{\"id\":\"TypeData\",\"type\":\"constr\",\"data\":") +
                        JsonEscape(Main.Text) +
                        std::string(",\"class\":\"string\"},{\"id\":\"TypeInterval\",\"type\":\"constr\",\"data\":\"100\",\"class\":\"int\"}]}");

                SendDetector("Type",Main.Text);
            }
        }
        SendSequenceItem(Main);
    }

    if(IsWait)
    {
        SendSequenceItem(Wait);
    }
}

void BrowserDirectControl::ClearSequence()
{
    Sequence.clear();
}

void BrowserDirectControl::ProcessSequence()
{
    if(Sequence.empty())
        return;
    std::vector<SequenceItem> SubSequence;
    int CurrentIndex = 0;
    int64 CurrentTime = Now();

    while(true)
    {
        //Everything is removed, break
        if(Sequence.empty())
            break;

        //We are in a sequence and reached end of the list
        if(CurrentIndex >= Sequence.size())
        {

            int64 LastItemTime = Sequence.at(Sequence.size() - 1).Time;


            if(CurrentTime - LastItemTime > MaxDelay)
            {
                //Last item is old, can send all
                SendSequenceItems(SubSequence);
                Sequence.clear();
            }else
            {
                //Need to wait if there is more events
            }

            //All data is processed, can exit loop
            break;
        }

        SequenceItem& CurrentItem = Sequence.at(CurrentIndex);

        if(CurrentIndex > 0)
        {

            //We are in a sub sequence
            if((CurrentItem.Type == KeyType || CurrentItem.Type == WaitFullPageLoadType) && CurrentItem.Time - Sequence.at(CurrentIndex - 1).Time <= MaxDelay)
            {
                //Continue sub sequence
                SubSequence.push_back(CurrentItem);
                CurrentIndex++;
            }else
            {

                //Break sub sequence - send all items and remove elements
                if(CurrentItem.Type != DoubleClickType)
                {
                    SendSequenceItems(SubSequence);
                }
                Sequence.erase(Sequence.begin(),Sequence.begin() + CurrentIndex);
                CurrentIndex = 0;
            }
        }else
        {
            //This element is first
            if(CurrentItem.Type == KeyType || CurrentItem.Type == ClickType)
            {
                //Start new sub sequence
                SubSequence.push_back(CurrentItem);
                CurrentIndex++;
            }else
            {
                //Send single item, don't start sub sequence
                SendSequenceItem(CurrentItem);
                Sequence.erase(Sequence.begin(),Sequence.begin() + 1);
                CurrentIndex = 0;
            }
        }

    }
}

void BrowserDirectControl::AddItem(const std::string& Meta,const std::string& Code, const std::string& Text, SequenceItemType Type, InspectResult Inspect)
{
    ProcessSequence();
    SequenceItem NewItem;
    NewItem.Time = Now();
    NewItem.Code = Code;
    NewItem.Meta = Meta;
    NewItem.Type = Type;
    NewItem.Text = Text;
    NewItem.Inspect = Inspect;
    Sequence.push_back(NewItem);
    ProcessSequence();
}

//
//  Main methods
//

BrowserDirectControl::BrowserDirectControl()
{

}

void BrowserDirectControl::Init(BrowserData *_BrowserData)
{
    this->_BrowserData = _BrowserData;
    IsDrag = false;
    LastClickTime = -1;
    DontSendNextWaitFullPageLoaded = true;
    IsPageLoaded = false;
    IsInspecting = false;
}

void BrowserDirectControl::Timer()
{
    // TID_UI
    ProcessSequence();

    PageLoadedInternal();

    ProcessSequence();
}

//
//  Browser events
//

void BrowserDirectControl::ProcessPendingTouchEndEvent()
{
    if(!TouchEndIsPending)
        return;

    if(!TouchStartTask)
        return;

    if(!TouchStartTask->GetIsFinished())
        return;

    if(TouchStartTime == 0)
        TouchStartTime = Now();

    std::string ResultDataRaw = TouchStartTask->GetString();
    JsonParser Parser;
    bool IsInIsolatedFrame = Parser.GetBooleanFromJson(ResultDataRaw, "is_in_isolated_frame", false);

    if(!IsInIsolatedFrame || Now() > TouchStartTime + 500)
    {

        //Need to send event

        TouchEndIsPending = false;
        TouchStartTask.reset();
        TouchStartTime = 0;

        _BrowserData->Connector->Touch(TouchEventUp, TouchEndPendingX, TouchEndPendingY, TouchEndPendingId);
    }
}


void BrowserDirectControl::StartDrag(CefRefPtr<CefBrowser> Browser, CefRefPtr<CefDragData> drag_data,CefBrowserHost::DragOperationsMask allowed_ops, int x, int y)
{
    //TID_UI

    CefMouseEvent e;
    e.modifiers = EVENTFLAG_LEFT_MOUSE_BUTTON;

    e.x = x;
    e.y = y;
    allowedops = allowed_ops;
    Browser->GetHost()->DragTargetDragEnter(drag_data,e,allowed_ops);
    Browser->GetHost()->DragTargetDragOver(e,allowedops);

    IsDrag = true;

    //Check if recording mode is on
    if(_BrowserData->ManualControl != BrowserData::DirectRecord)
        return;

    AddItem(PreviousMouseMeta, PreviousMouseCode, std::string(), DragStartType, PreviousMouseInspect);

    LastAddTab = false;
}


void BrowserDirectControl::Load(const std::string& Url)
{
    //TID_UI

    //Check if control is indirect
    if(_BrowserData->ManualControl == BrowserData::Indirect)
        return;

    std::string UrlCopy = Url;

    IsDrag = false;

    //Load url
    if(LastAddTab)
    {
        _BrowserData->Connector->CreateTab(Url, true);
    }else
        _BrowserData->Connector->Load(Url, true);


    SendDetector("Load",Url);

    //Check if recording mode is on
    if(_BrowserData->ManualControl != BrowserData::DirectRecord)
        return;

    if(LastAddTab)
    {
        //Add action to scenario
        std::string code = std::string("_popupcreate2(false, ") + JsonEscape(UrlCopy) + std::string(", \"\", false)!");

        std::string json = std::string("{\"s\":\"addtab\",\"v\":1,\"f\":[],\"uw\":\"1\",\"ut\":\"0\",\"uto\":\"0\",\"um\":\"0\",\"d\":[{\"id\":\"Url\",\"type\":\"constr\",\"data\":") + JsonEscape(UrlCopy) + std::string(",\"class\":\"string\"},{\"id\":\"IsSilent\",\"type\":\"constr\",\"data\":\"false\",\"class\":\"string\"},{\"id\":\"Referrer\",\"type\":\"constr\",\"data\":\"\",\"class\":\"string\"}]}");
        AddItem(json, code, std::string(), LoadType);
    }else
    {
        //Add action to scenario
        std::string code = std::string("_load(") + JsonEscape(Url) + std::string(", \"\", false)!");
        std::string json = std::string("{\"s\":\"load\",\"v\":1,\"f\":[],\"uw\":\"1\",\"ut\":\"0\",\"uto\":\"0\",\"um\":\"0\",\"d\":[{\"id\":\"LoadUrl\",\"type\":\"constr\",\"data\":") + JsonEscape(Url) + std::string(",\"class\":\"string\"},{\"id\":\"Referrer\",\"type\":\"constr\",\"data\":\"\",\"class\":\"string\"}]}");

        AddItem(json, code, std::string(), LoadType);
    }

    LastAddTab = false;
}

void BrowserDirectControl::StartAddTab()
{
    //TID_UI

    //Check if control is indirect
    if(_BrowserData->ManualControl == BrowserData::Indirect)
        return;

    LastAddTab = true;
}

void BrowserDirectControl::StopAddTab()
{
    //TID_UI

    //Check if control is indirect
    if(_BrowserData->ManualControl == BrowserData::Indirect)
        return;

    LastAddTab = false;
}

void BrowserDirectControl::SelectTab(int i)
{
    //TID_UI

    //Check if control is indirect
    if(_BrowserData->ManualControl == BrowserData::Indirect)
        return;

    _BrowserData->Connector->SwitchToTab(i);

    //Check if recording mode is on
    if(_BrowserData->ManualControl != BrowserData::DirectRecord)
        return;

    //Add action to scenario
    std::string code = std::string("popupselect(") + std::to_string(i) + std::string(")!");
    std::string json = std::string("{\"s\":\"selecttab\",\"v\":1,\"f\":[],\"uw\":\"0\",\"ut\":\"0\",\"uto\":\"0\",\"um\":\"0\",\"d\":[{\"id\":\"Index\",\"type\":\"constr\",\"data\":\"") + std::to_string(i) + std::string("\",\"class\":\"int\"}]}");

    AddItem(json, code, std::string(), PopupSelect);

    LastAddTab = false;
}

void BrowserDirectControl::CloseTab(int i)
{
    //TID_UI

    //Check if control is indirect
    if(_BrowserData->ManualControl == BrowserData::Indirect)
        return;

    _BrowserData->Connector->CloseTab(i);

    //Check if recording mode is on
    if(_BrowserData->ManualControl != BrowserData::DirectRecord)
        return;


    //Add action to scenario
    std::string code = std::string("popupclose(") + std::to_string(i) + std::string(")!");
    std::string json = std::string("{\"s\":\"closetab\",\"v\":1,\"f\":[],\"uw\":\"0\",\"ut\":\"0\",\"uto\":\"0\",\"um\":\"0\",\"d\":[{\"id\":\"Index\",\"type\":\"constr\",\"data\":\"") + std::to_string(i) + std::string("\",\"class\":\"int\"}]}");

    AddItem(json, code, std::string(), PopupClose);

    LastAddTab = false;
}

void BrowserDirectControl::PageLoadedInternal()
{
    // TID_UI

    if(!IsPageLoaded)
        return;

    IsPageLoaded = false;

    //Check if control is indirect
    if(_BrowserData->ManualControl == BrowserData::Indirect)
        return;

    IsDrag = false;

    //Check if recording mode is on
    if(_BrowserData->ManualControl != BrowserData::DirectRecord)
        return;


    //Add action to scenario
    std::string code = std::string("wait_async_load()!");
    std::string json = std::string("{\"s\":\"waitfullload\",\"v\":1,\"f\":[],\"uw\":\"0\",\"ut\":\"0\",\"uto\":\"0\",\"um\":\"0\",\"d\":[]}");

    AddItem(json, code, std::string(), WaitFullPageLoadType);

}

void BrowserDirectControl::PageLoaded()
{
    // TID_IO

    IsPageLoaded = true;
}

void BrowserDirectControl::GoBack()
{
    //TID_UI

    //Check if control is indirect
    if(_BrowserData->ManualControl == BrowserData::Indirect)
        return;

    _BrowserData->Connector->NavigateBack(true);

    //Check if recording mode is on
    if(_BrowserData->ManualControl != BrowserData::DirectRecord)
        return;

    //Add action to scenario
    std::string code = std::string("navigate_back(false)!\nwait_async_load()!");
    std::string json = std::string("{\"s\":\"navigateback\",\"v\":1,\"f\":[],\"uw\":\"1\",\"ut\":\"0\",\"uto\":\"0\",\"um\":\"0\",\"d\":[]}");

    AddItem(json, code, std::string(), GoBackType);

    LastAddTab = false;
}

void BrowserDirectControl::ScrollUp()
{
    //TID_UI

    //Check if control is indirect
    if(_BrowserData->ManualControl == BrowserData::Indirect)
        return;

    //No mouse wheele in touch mode
    if(_BrowserData->IsTouchScreen)
        return;

    _BrowserData->Connector->Wheel(LastMoveX, LastMoveY, true);
}

void BrowserDirectControl::ScrollDown()
{
    //TID_UI

    //Check if control is indirect
    if(_BrowserData->ManualControl == BrowserData::Indirect)
        return;

    //No mouse wheele in touch mode
    if(_BrowserData->IsTouchScreen)
        return;

    _BrowserData->Connector->Wheel(LastMoveX, LastMoveY, false);
}



void BrowserDirectControl::MouseMove(int X, int Y, bool IsMousePressed, bool IsCtrlPressed, bool IsShiftPressed)
{
    //TID_UI

    _BrowserData->DirectControlOrAutomationCursorX = X;
    _BrowserData->DirectControlOrAutomationCursorY = Y;

    //Check if control is indirect
    if(_BrowserData->ManualControl == BrowserData::Indirect)
        return;


    if(_BrowserData->IsTouchScreen)
    {
        if(_BrowserData->IsTouchPressedDirectControl)
        {
            _BrowserData->Connector->Touch(TouchEventMove, X, Y, _BrowserData->TouchEventId);
        }

        return;
    }

    LastMoveX = X;
    LastMoveY = Y;

    MouseButton PressedMouse = MouseButtonNone;

    int PressedKey = KeyboardModifiersNone;

    if(IsMousePressed)
    {
        PressedMouse = MouseButtonLeft;
    }

    if(IsCtrlPressed)
        PressedKey |= KeyboardModifiersCtrl;

    if(IsShiftPressed)
        PressedKey |= KeyboardModifiersShift;


    _BrowserData->Connector->Mouse(MouseEventMove, X, Y, PressedMouse, PressedMouse, PressedKey);

    /*if(IsDrag)
    {
        Pointer->GetBrowser()->GetHost()->DragTargetDragOver(e,allowedops);
    }*/

}



void BrowserDirectControl::Key(UINT msg, WPARAM wParam, LPARAM lParam)
{
    //TID_UI

    //Check if control is indirect
    if(_BrowserData->ManualControl == BrowserData::Indirect)
        return;

    IsDrag = false;

    KeyEvent Event;

    if (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN)
        Event = KeyEventDown;
    else if(msg == WM_KEYUP || msg == WM_SYSKEYUP)
        Event = KeyEventUp;
    else
        Event = KeyEventCharacter;

    int Modifiers = KeyboardModifiersNone;

    if (IsKeyDown(VK_SHIFT))
      Modifiers |= KeyboardModifiersShift;
    if (IsKeyDown(VK_CONTROL))
      Modifiers |= KeyboardModifiersCtrl;
    if (IsKeyDown(VK_MENU))
      Modifiers |= KeyboardModifiersAlt;

    if(IsKeyDown(VK_RMENU) || !((IsKeyDown(VK_CONTROL) | IsKeyDown(VK_MENU)) && Event == KeyEventCharacter))
        _BrowserData->Connector->KeyRaw(Event, wParam, lParam, Modifiers);

    //event.is_system_key = msg == WM_SYSCHAR || msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP;

    //Check if recording mode is on
    if(_BrowserData->ManualControl != BrowserData::DirectRecord)
        return;

    if(msg == WM_CHAR)
    {
        std::wstring Str;

        if(wParam == VK_BACK)
        {
            Str = L"<BACK>";
        }else if(wParam == VK_ESCAPE)
        {
            Str = L"<ESCAPE>";
        }else if(wParam == VK_TAB)
        {
            Str = L"<TAB>";
        }else if(IsKeyDown(VK_CONTROL))
        {
            if(wParam == 1)
            {
                Str = L"a";
            }
            if(wParam == 24)
            {
                Str = L"x";
            }
            if(wParam == 3)
            {
                Str = L"c";
            }
            if(wParam == 22)
            {
                Str = L"v";
            }
            if(wParam == 25)
            {
                Str = L"y";
            }
            if(wParam == 26)
            {
                Str = L"z";
            }
        }else
        {
            Str.push_back((wchar_t)wParam);
        }
        std::string Str2 = ws2s(Str);

        if(!Str2.empty())
        {
            if(IsKeyDown(VK_CONTROL))
                Str2 = std::string("<CONTROL>") + Str2;


            AddItem(std::string(), std::string(), Str2, KeyType);
        }
    }
    if(msg == WM_KEYDOWN)
    {
        std::string Text;


        if(wParam == VK_RETURN)
        {
            Text = "<RETURN>";
        }

        if(wParam == VK_PRIOR)
        {
            Text = "<PRIOR>";
        }
        if(wParam == VK_NEXT)
        {
            Text = "<NEXT>";
        }
        if(wParam == VK_END)
        {
            Text = "<END>";
        }
        if(wParam == VK_HOME)
        {
            Text = "<HOME>";
        }
        if(wParam == VK_LEFT)
        {
            Text = "<LEFT>";
        }
        if(wParam == VK_UP)
        {
            Text = "<UP>";
        }
        if(wParam == VK_RIGHT)
        {
            Text = "<RIGHT>";
        }
        if(wParam == VK_DOWN)
        {
            Text = "<DOWN>";
        }
        if(wParam == VK_INSERT)
        {
            Text = "<INSERT>";
        }
        if(wParam == VK_DELETE)
        {
            Text = "<DELETE>";
        }

        if(!Text.empty())
            AddItem(std::string(), std::string(), Text, KeyType);

    }

}

void BrowserDirectControl::DoMouseEvent(MouseClickItem Item)
{
    if(_BrowserData->IsTouchScreen)
    {
        return;
    }

    MouseEvent Event;
    if(Item.IsDownOrUp)
    {
        Event = MouseEventDown;
    }else
    {
        Event = MouseEventUp;
    }

    MouseButton Button;
    if(Item.IsLeftMouseButton)
    {
        Button = MouseButtonLeft;
    }else
    {
        Button = MouseButtonRight;
    }

    int PressedMouse = MouseButtonNone;

    if(Item.IsLeftMousePressed)
        PressedMouse |= MouseButtonLeft;

    if(Item.IsRightMousePressed)
        PressedMouse |= MouseButtonRight;

    int PressedKeyboard = KeyboardModifiersNone;

    bool IsOpenNewTab = false;

    if(Item.IsCtrlPressed)
    {
        PressedKeyboard |= KeyboardModifiersCtrl;
        IsOpenNewTab = true;
    }

    if(Item.IsShiftPressed)
    {
        PressedKeyboard |= KeyboardModifiersShift;
        IsOpenNewTab = true;
    }

    //Replace Ctrl-Click or Shift-Click with Shift-Ctrl-Click in order to always switch to new tab
    if(IsOpenNewTab)
    {
        PressedKeyboard |= KeyboardModifiersShift;
        PressedKeyboard |= KeyboardModifiersCtrl;
    }

    _BrowserData->Connector->Mouse(Event, Item.X, Item.Y, Button, PressedMouse, PressedKeyboard, Item.IsDoubleClick ? 2 : 1);


    /*if(Item.IsDrop)
    {
        Pointer->GetBrowser()->GetHost()->DragTargetDragOver(e,allowedops);
        Pointer->GetBrowser()->GetHost()->DragSourceEndedAt(e.x,e.y,allowedops);

        Pointer->GetBrowser()->GetHost()->DragTargetDrop(e);
        Pointer->GetBrowser()->GetHost()->DragSourceSystemDragEnded();
    }*/

}

void BrowserDirectControl::MouseClick(int X, int Y, bool IsDownOrUp, bool IsLeftMousePressed, bool IsRightMousePressed, bool IsCtrlPressed, bool IsShiftPressed, bool IsLeftMouseButton)
{
    //TID_UI
    _BrowserData->DirectControlOrAutomationCursorX = X;
    _BrowserData->DirectControlOrAutomationCursorY = Y;

    //Check if control is indirect
    if(_BrowserData->ManualControl == BrowserData::Indirect)
        return;

    if(_BrowserData->IsTouchScreen)
    {
        TouchEvent EventType;
        bool SendEvent = false;
        int CurrentTouchId = _BrowserData->TouchEventId;
        if(_BrowserData->IsTouchPressedDirectControl && !IsDownOrUp)
        {
            EventType = TouchEventUp;
            SendEvent = true;
            _BrowserData->TouchEventId++;
            _BrowserData->IsTouchPressedDirectControl = false;

            //Check if click was used
            int64 now = Now();

            if(LastClickTime >= 0 &&
                    (
                            abs(LastClickX - X) <= (GetSystemMetrics(SM_CXDOUBLECLK) / 2)
                        &&
                            abs(LastClickY - Y) <= (GetSystemMetrics(SM_CYDOUBLECLK) / 2)
                        &&
                            (now - LastClickTime) < GetDoubleClickTime()
                    )
                )
            {
                //Save click to scenario tab
                MouseClickItem NewItem;
                NewItem.X = X;
                NewItem.Y = Y;
                NewItem.IsDrop = false;
                NewItem.IsDownOrUp = false;
                NewItem.IsDoubleClick = false;
                NewItem.IsDrag = false;

                NewItem.IsLeftMousePressed = false;
                NewItem.IsRightMousePressed = false;
                NewItem.IsCtrlPressed = false;
                NewItem.IsShiftPressed = false;
                NewItem.IsLeftMouseButton = false;

                NewItem.AllowedOptions = allowedops;

                ApplyInspectResult(NewItem, PreviousMouseInspect);
            }

        }else if(!_BrowserData->IsTouchPressedDirectControl && IsDownOrUp)
        {
            EventType = TouchEventDown;
            SendEvent = true;
            _BrowserData->IsTouchPressedDirectControl = true;

            //Save info about position and time in order to check if click was performed later.
            int64 now = Now();
            LastClickTime = now;
            LastClickX = X;
            LastClickY = Y;

            //Send item to inspect
            MouseClickItem NewItem;
            NewItem.X = X;
            NewItem.Y = Y;
            NewItem.IsDrop = false;
            NewItem.IsDownOrUp = true;
            NewItem.IsDoubleClick = false;
            NewItem.IsDrag = false;

            NewItem.IsLeftMousePressed = false;
            NewItem.IsRightMousePressed = false;
            NewItem.IsCtrlPressed = false;
            NewItem.IsShiftPressed = false;
            NewItem.IsLeftMouseButton = false;

            NewItem.AllowedOptions = allowedops;

            MouseClicks.push_back(NewItem);
        }

        if(SendEvent)
        {

            if(EventType == TouchEventDown)
            {
                if(TouchStartTask)
                {
                    //ignore event, avoid zoom
                }else
                {
                    TouchStartTask = _BrowserData->Connector->Touch(EventType, X, Y, CurrentTouchId);
                    TouchStartTime = 0;
                    TouchEndIsPending = false;
                }
            }else if(EventType == TouchEventUp)
            {
                TouchEndIsPending = true;
                TouchEndPendingX = X;
                TouchEndPendingY = Y;
                TouchEndPendingId = CurrentTouchId;
                ProcessPendingTouchEndEvent();
            }

        }
        return;
    }



    _BrowserData->LastClickIsFromIndirectControl = false;


    //Detect click number
    int ClickNumber = 1;

    int64 now = Now();

    if(LastClickTime >= 0 &&
            (
                    abs(LastClickX - X) <= (GetSystemMetrics(SM_CXDOUBLECLK) / 2)
                &&
                    abs(LastClickY - Y) <= (GetSystemMetrics(SM_CYDOUBLECLK) / 2)
                &&
                    (now - LastClickTime) < GetDoubleClickTime()
            )
        )
    {
        //Double click
        ClickNumber = 2;

        if(!IsDownOrUp)
            LastClickTime = -1;
    }else if(!IsDownOrUp)
    {
        LastClickTime = now;
        LastClickX = X;
        LastClickY = Y;
    }

    //Create new item
    MouseClickItem NewItem;
    NewItem.X = X;
    NewItem.Y = Y;
    NewItem.IsDrop = !IsDownOrUp && IsDrag;
    NewItem.IsDownOrUp = IsDownOrUp;
    NewItem.IsDoubleClick = ClickNumber != 1;
    NewItem.IsDrag = IsDrag;

    NewItem.IsLeftMousePressed = IsLeftMousePressed;
    NewItem.IsRightMousePressed = IsRightMousePressed;
    NewItem.IsCtrlPressed = IsCtrlPressed;
    NewItem.IsShiftPressed = IsShiftPressed;
    NewItem.IsLeftMouseButton = IsLeftMouseButton;

    NewItem.AllowedOptions = allowedops;

    //Update drag state
    if(NewItem.IsDrop)
    {
        IsDrag = false;
    }

    if(NewItem.IsDrop)
    {

    }else if(!NewItem.IsDownOrUp && !NewItem.IsDoubleClick)
    {
        std::string Selector;
        {
            LOCK_BROWSER_DATA
            Selector = _BrowserData->_Inspect.css;
        }
        SendDetector("Move And Click On Element",Selector);
    }

    if(_BrowserData->ManualControl == BrowserData::DirectNoRecord)
    {
        //No recording mode, send event
        //WORKER_LOG("!!!!!! Send mouse click directly " + std::to_string(X) + ", " + std::to_string(Y));
        DoMouseEvent(NewItem);
    }else
    {
        if(NewItem.IsDownOrUp || NewItem.IsDrop)
        {
            //WORKER_LOG("!!!!!! Send mouse click inspect to queue " + std::to_string(X) + ", " + std::to_string(Y));
            MouseClicks.push_back(NewItem);
        }else
        {
            //WORKER_LOG("!!!!!! Apply last inspect result " + std::to_string(X) + ", " + std::to_string(Y) + ", " + PreviousMouseInspect.css);
            DoMouseEvent(NewItem);
            ApplyInspectResult(NewItem, PreviousMouseInspect);
        }
    }


    LastAddTab = false;

}
