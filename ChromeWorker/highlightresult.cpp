#include "highlightresult.h"
#include "log.h"
#include "fillalpharectangle.h"




void HighlightResult::Paint(HDC hdc, MouseOverMultiSelect& _MouseOverMultiSelect, bool IsIndirect, int MouseX, int MouseY, bool MultiselectMode, MultiSelectData& Multiselect, int BrowserRealWidth, int BrowserRealHeight, int BrowserDrawWidth, int BrowserDrawHeight, int BrowserScrollX, int BrowserScrollY, int BrowserLeft, int BrowserTop)
{

    for(rect& r: highlights)
    {
        bool is_alternative = r.is_alternative;
        int x = r.x;
        int y = r.y;

        int x1 = (float)(x) * (float)BrowserDrawWidth / (float)BrowserRealWidth + 1
            ,y1 = (float)(y) * (float)BrowserDrawHeight / (float)BrowserRealHeight + 1
            ,width1 = (float)r.width* (float)BrowserDrawWidth / (float)BrowserRealWidth + 1
            ,height1 = (float)r.height* (float)BrowserDrawHeight / (float)BrowserRealHeight + 1;

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
                !is_alternative
                &&
                MultiselectMode && (Multiselect.MouseOverType == MouseOverNone || Multiselect.MouseOverType == MouseOverInspect)
                &&
                MouseX != -1 && MouseY != -1 && MouseX >= x1 && MouseX <= x1 + width1 && MouseY >= y1 && MouseY <= y1 + height1
          )
        {

            RectMouseOverMultiSelect MouseOverItem;
            MouseOverItem.X = x1;
            MouseOverItem.Y = y1;
            MouseOverItem.Width = width1;
            MouseOverItem.Height = height1;
            MouseOverItem.MouseOverType = MouseOverHighlight;
            MouseOverItem.MouseOverHighlightSelector = r.selector + std::string(">AT> ") + std::to_string(r.index);
            _MouseOverMultiSelect.Rectangles.push_back(MouseOverItem);


        }



        RECT r;
        r.top = BrowserTop + y1;
        r.bottom = BrowserTop + y1 + height1;

        r.left = BrowserLeft + x1;
        r.right = BrowserLeft + x1 + width1;

        HBRUSH brush;

        if(is_alternative)
        {
            brush = CreateSolidBrush(RGB(200, 200, 0));
        }else
        {
            brush = CreateSolidBrush(RGB(0, 153, 204));
        }


        FillAlphaRect(hdc,&r,brush,100);

        DeleteObject(brush);
    }

}
