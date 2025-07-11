#ifndef HIGHLIGHTRESULT_H
#define HIGHLIGHTRESULT_H

#include <vector>
#include <windows.h>
#include "inspectresult.h"

struct HighlightResult
{
    struct rect
    {
        int x;
        int y;
        int width;
        int height;
        int index;
        bool is_alternative;
        std::string selector;
    };

    std::vector<rect> highlights;

    void Paint(HDC hdc, MouseOverMultiSelect& _MouseOverMultiSelect, bool IsIndirect, int MouseX, int MouseY, bool MultiselectMode, MultiSelectData& Multiselect, int BrowserRealWidth, int BrowserRealHeight, int BrowserDrawWidth, int BrowserDrawHeight, int BrowserScrollX, int BrowserScrollY, int BrowserLeft, int BrowserTop);

};

#endif // HIGHLIGHTRESULT_H
