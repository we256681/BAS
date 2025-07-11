#ifndef POPUP_H
#define POPUP_H

#include <vector>

class Popup
{
    bool IsPopupVisible = false;
    bool IsPopupPositionSet = false;
    int PopupX = 0;
    int PopupY = 0;
    int PopupHeight = 0;
    int PopupWidth = 0;
    int BrowserHeight = 0;
    int BrowserWidth = 0;
    std::vector<char> PoupupData;
    std::vector<char> BrowserData;
    std::vector<char> SendData;
public:
    Popup();
    void SetPopupVisibility(bool IsPopupVisible);
    void SetPopupPosition(int PopupX, int PopupY);
    void SetPopupData(const void* PopupBuffer, int PopupWidth, int PopupHeight);
    void SetBrowserData(const void* BrowserBuffer, int BrowserWidth, int BrowserHeight);
    int GetBrowserWidth();
    int GetBrowserHeight();
    char * PrepareData();

};

#endif // POPUP_H
