#include "inspectresult.h"
#include "log.h"
#include "picojson.h"
#include "fillalpharectangle.h"
#include "translate.h"
#include <string>

void InspectResult::Paint(HDC hdc, int TextSize, HFONT InspectFont, bool ShowText, bool SimplifiedText, int BrowserRealWidth, int BrowserRealHeight, int BrowserDrawWidth, int BrowserDrawHeight, int BrowserScrollX, int BrowserScrollY, int BrowserLeft, int BrowserTop)
{

    if(!active)
        return;

    POINT pt;


    int     x1 = (float)x * (float)BrowserDrawWidth / (float)BrowserRealWidth
            ,y1 = (float)y * (float)BrowserDrawHeight / (float)BrowserRealHeight
            ,width1 = (float)width* (float)BrowserDrawWidth / (float)BrowserRealWidth
            ,height1 = (float)height* (float)BrowserDrawHeight / (float)BrowserRealHeight;


    if(x1<=0)
    {
       width1 += x1;
       x1 = 1;
    }

    if(y1<=0)
    {
       height1 += y1;
       y1 = 1;
    }

    if(x1 + width1 >= BrowserDrawWidth)
       width1 = BrowserDrawWidth - x1 - 1;

    if(y1 + height1 >= BrowserDrawHeight)
       height1 = BrowserDrawHeight - y1 - 1;


    int htext = TextSize;

    bool is_text = true;
    std::wstring text;

    int width_tooltip1 = 20;
    int width_tooltip2 = 90;
    int width_tooltip3 = 320;
    int width_tooltip4 = 480;

    if(Translate::GetLanguage() == "en")
    {
        width_tooltip1 = 20;
        width_tooltip2 = 70;
        width_tooltip3 = 190;
        width_tooltip4 = 320;
    }

    if(SimplifiedText)
    {
        if(width1 < width_tooltip1)
        {
            is_text = false;
        }else if(position)
        {
            text = std::wstring(L" ") + std::to_wstring(position) + std::wstring(L" ");
        }
    }else
    {
        if(width1 < width_tooltip1)
        {
            is_text = false;
        }else if(width1 < width_tooltip2)
        {
            text = std::wstring(L" ") + std::to_wstring(position) + std::wstring(L" ");
        }else if(width1 < width_tooltip3)
        {
            text = std::wstring(L" ") + Translate::Tr(L"Position: ") + std::to_wstring(position) + std::wstring(L" ");
        }else if(width1 < width_tooltip4)
        {
            htext = TextSize * 3;
            text = std::wstring(L" ") + Translate::Tr(L"Position: ") + std::to_wstring(position) + std::wstring(L". \r\n ") + Translate::Tr(L"Key up / key down - tweak selection.") +  std::wstring(L" \r\n ") + Translate::Tr(L"Enter - select.") + std::wstring(L" ");
        }else
        {
            text = std::wstring(L" ") + Translate::Tr(L"Position: ") + std::to_wstring(position) + std::wstring(L". ") + Translate::Tr(L"Key up / key down - tweak selection.") + std::wstring(L" ") + Translate::Tr(L"Enter - select.") + std::wstring(L" ");
        }
    }


    int y_text = 0;
    if(y1 + height1 / 2 > BrowserDrawHeight / 2)
    {
        y_text = y1 - htext;
    }else
    {
        y_text = y1 + height1;
    }

    int     x_text = x1
            ,width_text = width1
            ,height_text = htext;


    if(!ShowText)
    {
        is_text = false;
    }

    if(is_text)
    {
        RECT rect;
        rect.left = BrowserLeft + x_text;
        rect.top = BrowserTop + y_text;
        rect.bottom = BrowserTop + y_text + height_text;
        rect.right = BrowserLeft + x_text + width_text;
        //SetBkMode(hdc,TRANSPARENT);
        HFONT hFontOld = (HFONT)SelectObject( hdc, InspectFont );
        SetTextColor(hdc,RGB(100,100,100));
        DrawText(hdc,text.c_str(),text.length(),&rect,DT_RIGHT | DT_VCENTER | DT_WORDBREAK);
        SelectObject( hdc, hFontOld );
    }

    //WORKER_LOG(std::string("Paint<<") + std::to_string(x1) + std::string("<<") + std::to_string(y1)+ std::string("<<") + std::to_string(width1)+ std::string("<<") + std::to_string(height1));

    HPEN hPen = CreatePen(PS_SOLID,2,RGB(250,50,50));
    SelectObject(hdc, hPen);

        MoveToEx(hdc, BrowserLeft + x1, BrowserTop + y1, &pt);
        LineTo(hdc, BrowserLeft + x1, BrowserTop + y1 + height1);

        MoveToEx(hdc, BrowserLeft + x1, BrowserTop + y1, &pt);
        LineTo(hdc, BrowserLeft + x1 + width1, BrowserTop + y1);

        MoveToEx(hdc, BrowserLeft + x1, BrowserTop + y1 + height1, &pt);
        LineTo(hdc, BrowserLeft + x1 + width1, BrowserTop + y1 + height1);

        MoveToEx(hdc, BrowserLeft + x1 + width1, BrowserTop + y1, &pt);
        LineTo(hdc, BrowserLeft + x1 + width1, BrowserTop + y1 + height1);

    DeleteObject(hPen);

    /*hPen = CreatePen(PS_SOLID,1,RGB(250,50,50));
    SelectObject(hdc, hPen);

        MoveToEx(hdc, BrowserLeft, BrowserTop + y1 + height1/2, &pt);
        LineTo(hdc, BrowserLeft + x1, BrowserTop + y1 + height1/2);

        MoveToEx(hdc, BrowserLeft + x1 + width1, BrowserTop + y1 + height1/2, &pt);
        LineTo(hdc, BrowserLeft + BrowserDrawWidth, BrowserTop + y1 + height1/2);

        MoveToEx(hdc, BrowserLeft + x1 + width1/2, BrowserTop, &pt);
        LineTo(hdc, BrowserLeft + x1 + width1/2, BrowserTop + y1);

        MoveToEx(hdc, BrowserLeft + x1 + width1/2, BrowserTop + y1 + height1, &pt);
        LineTo(hdc, BrowserLeft + x1 + width1/2, BrowserTop + BrowserDrawHeight);


    DeleteObject(hPen);*/


    //HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));

    /*{
        RECT r;
        r.top = BrowserTop;
        r.bottom = BrowserTop + y1;

        r.left = BrowserLeft;
        r.right = BrowserLeft + BrowserDrawWidth;

        FillAlphaRect(hdc,&r,brush,20);
    }

    {
        RECT r;
        r.top = BrowserTop + y1 + height1;
        r.bottom = BrowserTop + BrowserDrawHeight;

        r.left = BrowserLeft;
        r.right = BrowserLeft + BrowserDrawWidth;

        FillAlphaRect(hdc,&r,brush,20);
    }

    {
        RECT r;
        r.top = BrowserTop + y1-1;
        r.bottom = BrowserTop + y1 + height1+1;

        r.left = BrowserLeft;
        r.right = BrowserLeft + x1;

        FillAlphaRect(hdc,&r,brush,20);
    }

    {
        RECT r;
        r.top = BrowserTop + y1-1;
        r.bottom = BrowserTop + y1 + height1+1;

        r.left = BrowserLeft + x1 + width1;
        r.right = BrowserLeft + BrowserDrawWidth;

        FillAlphaRect(hdc,&r,brush,20);
    }*/

    /*{
        RECT r;
        r.top = BrowserTop + y1;
        r.bottom = BrowserTop + y1 + height1;

        r.left = BrowserLeft + x1;
        r.right = BrowserLeft + x1 + width1;

        FillAlphaRect(hdc,&r,brush,20);
    }


    DeleteObject(brush);*/



    /*MoveToEx(hdc, 0, 30, &pt);
    LineTo(hdc, 500, 30);

    MoveToEx(hdc, 502, 221, &pt);
    LineTo(hdc, WindowWidth, 221);*/
}

std::string InspectResult::Serialize()
{
    picojson::value::object res;

    if(isimage)
    {
        std::string res = imagedata;
        res.insert(res.begin(),'"');
        res.push_back('"');
        imagedata.clear();
        return res;
    }else
    {
        res["css"] = picojson::value(css);
        res["css1"] = picojson::value(css);
        if(css2 != css)
            res["css2"] = picojson::value(css2);
        if(css3 != css && css3 != css2)
            res["css3"] = picojson::value(css3);
        res["match"] = picojson::value(match);
        res["xpath"] = picojson::value(xpath);
        res["mousex"] = picojson::value(std::to_string(mousex));
        res["mousey"] = picojson::value(std::to_string(mousey));

        return picojson::value(res).serialize();
    }
}

void MultiSelectData::Paint(HDC hdc, MouseOverMultiSelect& _MouseOverMultiSelect, bool IsIndirect, int MouseX, int MouseY, int BrowserRealWidth, int BrowserRealHeight, int BrowserDrawWidth, int BrowserDrawHeight, int BrowserScrollX, int BrowserScrollY, int BrowserLeft, int BrowserTop)
{
    for(MultiSelectDataItem &Inspect: Items)
    {

        if(!Inspect.size_calculated)
            continue;

        int x = Inspect.x;
        int y = Inspect.y;

        int     x1 = (float)x * (float)BrowserDrawWidth / (float)BrowserRealWidth
                ,y1 = (float)y * (float)BrowserDrawHeight / (float)BrowserRealHeight
                ,width1 = (float)Inspect.width* (float)BrowserDrawWidth / (float)BrowserRealWidth
                ,height1 = (float)Inspect.height* (float)BrowserDrawHeight / (float)BrowserRealHeight;



        if(x1 + width1 <=0 || width1<=1)
        {
            continue;
        }

        if(y1 + height1 <=0 || height1<=1)
        {
            continue;
        }

        if(x1 >= BrowserDrawWidth)
        {
            continue;
        }

        if(y1 >= BrowserDrawHeight)
        {
            continue;
        }



        if(x1<=0)
        {
           width1 += x1;
           x1 = 1;
        }

        if(y1<=0)
        {
           height1 += y1;
           y1 = 1;
        }

        if(x1 + width1 >= BrowserDrawWidth)
           width1 = BrowserDrawWidth - x1 - 1;

        if(y1 + height1 >= BrowserDrawHeight)
           height1 = BrowserDrawHeight - y1 - 1;


        if(
                (MouseOverType == MouseOverNone || MouseOverType == MouseOverInspect)
                &&
                MouseX != -1 && MouseY != -1 && MouseX >= x1 && MouseX <= x1 + width1 && MouseY >= y1 && MouseY <= y1 + height1)
        {

            RectMouseOverMultiSelect MouseOverItem;
            MouseOverItem.X = x1;
            MouseOverItem.Y = y1;
            MouseOverItem.Width = width1;
            MouseOverItem.Height = height1;
            MouseOverItem.MouseOverType = Inspect.IsInclude ? MouseOverInclude : MouseOverExclude;
            MouseOverItem.MouseOverHighlightSelector.clear();
            MouseOverItem.MouseOverDataItem = Inspect;
            _MouseOverMultiSelect.Rectangles.push_back(MouseOverItem);


        }


        RECT r;
        r.top = BrowserTop + y1;
        r.bottom = BrowserTop + y1 + height1;

        r.left = BrowserLeft + x1;
        r.right = BrowserLeft + x1 + width1;



        HBRUSH brush;

        if(Inspect.IsInclude)
            brush = CreateSolidBrush(RGB(0, 200, 0));
        else
            brush = CreateSolidBrush(RGB(200, 0, 0));

        FillAlphaRect(hdc,&r,brush,100);

        DeleteObject(brush);

    }
}

void MouseOverMultiSelect::Paint(HDC hdc, bool IsIndirect, int MouseX, int MouseY, int BrowserLeft, int BrowserTop, bool MultiselectMode, MultiSelectData* Multiselect)
{
    int IndexPaint = -1;
    int AreaMax = -1;
    int Index = 0;
    for(RectMouseOverMultiSelect& Over:Rectangles)
    {
        int x1 = Over.X;
        int y1 = Over.Y;
        int width1 = Over.Width;
        int height1 = Over.Height;

        int Area = width1 * height1;

        if(AreaMax < 0 || Area < AreaMax)
        {
            AreaMax = Area;
            IndexPaint = Index;
        }

        Index++;

    }

    if(IndexPaint >= 0)
    {

        RectMouseOverMultiSelect& Over = Rectangles[IndexPaint];

        int x1 = Over.X;
        int y1 = Over.Y;
        int width1 = Over.Width;
        int height1 = Over.Height;

        Multiselect->MouseOverType = Over.MouseOverType;
        Multiselect->MouseOverHighlightSelector = Over.MouseOverHighlightSelector;
        Multiselect->MouseOverDataItem = Over.MouseOverDataItem;

        if(IsIndirect)
        {
            POINT pt;

            HPEN hPen = CreatePen(PS_DOT,1,RGB(250,50,50));
            SelectObject(hdc, hPen);

                MoveToEx(hdc, BrowserLeft + x1, BrowserTop + y1, &pt);
                LineTo(hdc, BrowserLeft + x1, BrowserTop + y1 + height1);
                MoveToEx(hdc, BrowserLeft + x1 - 1, BrowserTop + y1, &pt);
                LineTo(hdc, BrowserLeft + x1 - 1, BrowserTop + y1 + height1);


                MoveToEx(hdc, BrowserLeft + x1, BrowserTop + y1, &pt);
                LineTo(hdc, BrowserLeft + x1 + width1, BrowserTop + y1);
                MoveToEx(hdc, BrowserLeft + x1, BrowserTop + y1 - 1, &pt);
                LineTo(hdc, BrowserLeft + x1 + width1, BrowserTop + y1 - 1);


                MoveToEx(hdc, BrowserLeft + x1, BrowserTop + y1 + height1, &pt);
                LineTo(hdc, BrowserLeft + x1 + width1, BrowserTop + y1 + height1);
                MoveToEx(hdc, BrowserLeft + x1, BrowserTop + y1 + height1 - 1, &pt);
                LineTo(hdc, BrowserLeft + x1 + width1, BrowserTop + y1 + height1 - 1);


                MoveToEx(hdc, BrowserLeft + x1 + width1, BrowserTop + y1, &pt);
                LineTo(hdc, BrowserLeft + x1 + width1, BrowserTop + y1 + height1);
                MoveToEx(hdc, BrowserLeft + x1 + width1 - 1, BrowserTop + y1, &pt);
                LineTo(hdc, BrowserLeft + x1 + width1 - 1, BrowserTop + y1 + height1);

            DeleteObject(hPen);
        }
    }

}


void MultiSelectData::Clear()
{
    Items.clear();
}

std::string MultiSelectData::Serialize()
{
    picojson::value::array ResArray;
    for(MultiSelectDataItem& Item: Items)
    {
        picojson::value::object ResObject;
        ResObject["id"] = picojson::value(Item.id);
        ResObject["selector"] = picojson::value(Item.selector);
        ResObject["include"] = picojson::value(Item.IsInclude ? (double)1 : (double)0);
        ResArray.push_back(picojson::value(ResObject));
    }
    return picojson::value(ResArray).serialize();
}

void MultiSelectData::UpdatePositions(const std::string& Data, int FrameOffsetX, int FrameOffsetY)
{
    picojson::value v;
    std::string err = picojson::parse(v, Data);
    if(!err.empty())
        return;
    if(!v.is<picojson::value::array>())
    {
        return;
    }
    for(MultiSelectDataItem& Item: Items)
    {
        Item.size_calculated = false;
    }
    picojson::value::array a = v.get<picojson::value::array>();
    for(picojson::value& Item: a)
    {
        picojson::value::object ItemObject = Item.get<picojson::value::object>();
        std::string Id = ItemObject["id"].get<std::string>();
        int X = ItemObject["x"].get<double>() + FrameOffsetX;
        int Y = ItemObject["y"].get<double>() + FrameOffsetY;
        int Width = ItemObject["width"].get<double>();
        int Height = ItemObject["height"].get<double>();


        for(MultiSelectDataItem& Item: Items)
        {
            if(Item.id == Id)
            {
                Item.size_calculated = true;
                Item.x = X;
                Item.y = Y;
                Item.width = Width;
                Item.height = Height;
            }
        }
    }
}


MultiSelectDataItem::MultiSelectDataItem()
{
    id = std::to_string(rand()%100000 + 1000);
}


bool MultiSelectData::MouseClick()
{
    if(MouseOverType == MouseOverInclude || MouseOverType == MouseOverExclude)
    {
        for(std::vector<MultiSelectDataItem>::iterator it = Items.begin() ; it != Items.end(); )
        {
            if(it->id == MouseOverDataItem.id)
            {
                it = Items.erase(it);
            }else
            {
                ++it;
            }
        }
        IsDirty = true;
        return true;
    }
    if(MouseOverType == MouseOverHighlight)
    {

        bool IsGood = true;
        for(MultiSelectDataItem &Item:Items)
        {
            if(Item.selector == MouseOverHighlightSelector)
            {
                IsGood = false;
                break;
            }
        }
        if(IsGood)
        {
            if(!IsSelectorsInSameFrame(OriginalSelector,MouseOverHighlightSelector))
            {
                ReportAboutDifferentFrame();
                return false;
            }
        }
        if(IsGood)
        {
            MultiSelectDataItem NewItem;
            NewItem.selector = MouseOverHighlightSelector;
            NewItem.label = MouseOverHighlightSelector;
            NewItem.IsInclude = false;
            Items.push_back(NewItem);
            IsDirty = true;
        }
    }
    if(MouseOverType == MouseOverInspect)
    {
        bool IsGood = true;
        for(MultiSelectDataItem &Item:Items)
        {
            if(Item.selector == MouseOverInspectData.css)
            {
                IsGood = false;
                break;
            }
        }
        if(IsGood)
        {
            if(!IsSelectorsInSameFrame(OriginalSelector,MouseOverInspectData.css))
            {
                ReportAboutDifferentFrame();
                return false;
            }
        }
        if(IsGood)
        {
            MultiSelectDataItem NewItem;
            NewItem.selector = MouseOverInspectData.css;
            NewItem.label = MouseOverInspectData.label;
            NewItem.IsInclude = true;
            Items.push_back(NewItem);
            IsDirty = true;
        }
    }
    return false;
}

bool MultiSelectData::IsSelectorsInSameFrame(const std::string& Selector1, const std::string& Selector2)
{
    std::size_t Index1 = Selector1.rfind(">FRAME>");
    std::size_t Index2 = Selector2.rfind(">FRAME>");
    if(Index1 == std::string::npos && Index2 == std::string::npos)
    {
        return true;
    }

    if(Index1 == std::string::npos && Index2 != std::string::npos)
    {
        return false;
    }

    if(Index1 != std::string::npos && Index2 == std::string::npos)
    {
        return false;
    }

    if(Index1 != Index2)
    {
        return false;
    }

    std::string Prefix1 = Selector1.substr(0,Index1);
    std::string Prefix2 = Selector1.substr(0,Index2);

    return Prefix1 == Prefix2;
}

void MultiSelectData::ReportAboutDifferentFrame()
{
    MessageBox(NULL,Translate::Tr(L"Can't select element from different frame.").c_str(),Translate::Tr(L"Error").c_str(),MB_OK | MB_ICONSTOP);
}


