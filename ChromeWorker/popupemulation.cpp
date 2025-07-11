#include "popupemulation.h"
#include "JsonParser.h"
#include "JsonSerializer.h"
#include "converter.h"

void PopupEmulation::Init(BrowserData *Data, int FirstIndex, HWND hwnd, MainLayout* Layout, const std::wstring& ProxyConfigFolder)
{
    this->Data = Data;
    this->hwnd = hwnd;
    this->FirstIndex = FirstIndex;
    this->Layout = Layout;
    this->ProxyConfigFolder = ProxyConfigFolder;
    SelectElementIPC.Init(ProxyConfigFolder + std::wstring(L"inselect"));
}

bool PopupEmulation::GetMenuVisibility()
{
    return (Data->ManualControl != BrowserData::Indirect) || Data->IsRecord;
}

void PopupEmulation::CloseMenu(bool ForceClose)
{
    if(hMenu)
    {
        if(ForceClose)
        {
            SendMessage(hwnd, WM_CANCELMODE, (WPARAM)0, (LPARAM)0);
        }
        DestroyMenu(hMenu);
        hMenu = 0;
    }
}

void PopupEmulation::ShowMenu(int X, int Y, int Height, std::vector<std::string> Options)
{
    if(!GetMenuVisibility())
        return;

    CloseMenu();

    hMenu = CreatePopupMenu();

    int i = 0;
    for(std::string Option: Options)
    {
        AppendMenu(hMenu, MF_BYPOSITION | MF_STRING, FirstIndex + i, s2ws(Option).c_str());
        i++;
    }

    RECT BrowserRectRelative = Layout->GetBrowserRectangle(Data->WidthBrowser,Data->HeightBrowser,Data->WidthAll,Data->HeightAll);

    POINT p;
    p.x = BrowserRectRelative.left + Data->DirectControlOrAutomationCursorX * (float)(BrowserRectRelative.right - BrowserRectRelative.left) / (float)(Data->WidthBrowser);
    p.y = BrowserRectRelative.top + Data->DirectControlOrAutomationCursorY * (float)(BrowserRectRelative.bottom - BrowserRectRelative.top) / (float)(Data->HeightBrowser);
    ClientToScreen(Data->_MainWindowHandle, &p);

    POINT p2;
    p2.x = BrowserRectRelative.right;
    p2.y = BrowserRectRelative.bottom;
    ClientToScreen(Data->_MainWindowHandle, &p2);

    POINT p1;
    p1.x = BrowserRectRelative.left;
    p1.y = BrowserRectRelative.top;
    ClientToScreen(Data->_MainWindowHandle, &p1);


    if(p.x < p1.x)
        p.x = p1.x;

    if(p.x > p2.x)
        p.x = p2.x;

    if(p.y < p1.y)
        p.y = p1.y;

    if(p.y > p2.y)
        p.y = p2.y;


    int Result = TrackPopupMenu(hMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD, p.x, p.y, 0, hwnd, NULL) - FirstIndex;

    if(Result < 0)
        Result = -1;

    CloseMenu();

    if(CurrentElementId.empty())
    {
        return;
    }

    JsonSerializer Serializer;
    std::map<std::string, Variant> SendObject;
    SendObject["type"] = Variant(std::string("popup_result"));
    SendObject["index"] = Variant(Result);
    SendObject["element_id"] = Variant(CurrentElementId);
    std::string SendData = Serializer.SerializeObjectToString(SendObject);

    IPCWithFile::Write(ProxyConfigFolder + std::wstring(L"outselect") + s2ws(CurrentElementId), SendData);

}

void PopupEmulation::SetIndex(int Index)
{
    CloseMenu(true);

    if(CurrentElementId.empty())
    {
        return;
    }



    JsonSerializer Serializer;
    std::map<std::string, Variant> SendObject;
    SendObject["type"] = Variant(std::string("popup_result"));
    SendObject["index"] = Variant(Index);
    SendObject["element_id"] = Variant(CurrentElementId);
    std::string SendData = Serializer.SerializeObjectToString(SendObject);

    IPCWithFile::Write(ProxyConfigFolder + std::wstring(L"outselect") + s2ws(CurrentElementId), SendData);

}

void PopupEmulation::Timer()
{
    /*if(hMenu && Data->ManualControl == BrowserData::Indirect)
    {
        CloseMenu();
    }*/

    if(SelectElementIPC.Peek())
    {
        std::vector<std::string> DataAll = SelectElementIPC.Read();

        for(std::string& CurrentData:DataAll)
        {
            JsonParser Parser;
            if(Parser.GetStringFromJson(CurrentData,"type") == std::string("close_popup"))
            {
                CloseMenu(true);
            }
            if(Parser.GetStringFromJson(CurrentData,"type") == std::string("open_popup"))
            {
                for(auto f:EventPopupShown)
                    f();

                /*if(Data->ManualControl == BrowserData::Indirect)
                    return;*/

                int X = Parser.GetFloatFromJson(CurrentData,"left");
                int Y = Parser.GetFloatFromJson(CurrentData,"top");
                int Height = Parser.GetFloatFromJson(CurrentData,"height");

                std::vector<std::string> Options;

                picojson::value OptionsValue;
                bool ParsedCorrectly = true;
                try
                {
                    picojson::parse(OptionsValue, CurrentData);
                }
                catch (...)
                {
                    ParsedCorrectly = false;
                }
                if(ParsedCorrectly && OptionsValue.is<picojson::object>())
                {
                    picojson::object OptionsObject = OptionsValue.get<picojson::object>();
                    if(OptionsObject.count("options") > 0 && OptionsObject["options"].is<picojson::array>())
                    {
                        picojson::array OptionsArray = OptionsObject["options"].get<picojson::array>();

                        for(picojson::value& OptionValue: OptionsArray)
                        {
                            if(OptionValue.is<std::string>())
                            {
                                std::string Option = OptionValue.get<std::string>();

                                Options.push_back(Option);
                            }
                        }
                    }
                }


                if(Options.empty())
                    return;

                CurrentElementId = Parser.GetStringFromJson(CurrentData,"element_id");
                ShowMenu(X, Y, Height, Options);
                return;
            }
        }
    }
}
