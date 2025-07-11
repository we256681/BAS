#ifndef INSPECTRESULT
#define INSPECTRESULT

#include <string>
#include <vector>
#include <windows.h>

struct FrameInspectResult
{
    bool is_frame;
    std::string frame_name;
    std::string frame_url;
    std::string frame_tag_html;
    int frame_index;
    int x_with_padding;
    int y_with_padding;
    int frame_depth;
    long long frame_id;
    long long parent_frame_id;
};


struct InspectResult
{
  int x;
  int y;
  int width;
  int height;
  std::string label;
  std::string css;
  std::string css2;
  std::string css3;
  std::string xpath;
  std::string match;
  int position;
  int mousex;
  int mousey;
  bool active;
  bool isimage = false;
  std::string imagedata;
  FrameInspectResult FrameData;
  void Paint(HDC hdc, int TextSize, HFONT InspectFont, bool ShowText, bool SimplifiedText, int BrowserRealWidth, int BrowserRealHeight, int BrowserDrawWidth, int BrowserDrawHeight, int BrowserScrollX, int BrowserScrollY, int BrowserLeft, int BrowserTop);
  std::string Serialize();

};

struct MultiSelectDataItem
{
    int x = -1;
    int y = -1;
    int width = -1;
    int height = -1;
    std::string id;
    bool size_calculated = false;
    std::string selector;
    std::string label;
    bool IsInclude;
    MultiSelectDataItem();
};

enum MouseOverTypeEnum
{
    MouseOverNone,
    MouseOverInclude,
    MouseOverExclude,
    MouseOverHighlight,
    MouseOverInspect
};


struct RectMouseOverMultiSelect
{
    int X;
    int Y;
    int Width;
    int Height;
    MouseOverTypeEnum MouseOverType;
    MultiSelectDataItem MouseOverDataItem;
    std::string MouseOverHighlightSelector;
};

class MultiSelectData;

struct MouseOverMultiSelect
{
    std::vector<RectMouseOverMultiSelect> Rectangles;
    void Paint(HDC hdc, bool IsIndirect, int MouseX, int MouseY, int BrowserLeft, int BrowserTop, bool MultiselectMode, MultiSelectData* Multiselect);

};

struct MultiSelectData
{
    bool IsDirty = false;
    std::vector<MultiSelectDataItem> Items;
    std::string OriginalSelector;
    void Paint(HDC hdc, MouseOverMultiSelect& _MouseOverMultiSelect, bool IsIndirect, int MouseX, int MouseY, int BrowserRealWidth, int BrowserRealHeight, int BrowserDrawWidth, int BrowserDrawHeight, int BrowserScrollX, int BrowserScrollY, int BrowserLeft, int BrowserTop);
    std::string Serialize();
    void UpdatePositions(const std::string& Data, int FrameOffsetX, int FrameOffsetY);
    void Clear();


    /* Mouse over and clicking */
    MouseOverTypeEnum MouseOverType = MouseOverNone;
    MultiSelectDataItem MouseOverDataItem;
    std::string MouseOverHighlightSelector;
    InspectResult MouseOverInspectData;

    bool MouseClick();

    private:
        bool IsSelectorsInSameFrame(const std::string& Selector1, const std::string& Selector2);
        void ReportAboutDifferentFrame();
};






#endif // INSPECTRESULT

