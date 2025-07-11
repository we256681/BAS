#include "popup.h"
#include "log.h"

Popup::Popup()
{

}

void Popup::SetPopupVisibility(bool IsPopupVisible)
{
    if(this->IsPopupVisible == IsPopupVisible)
        return;
    this->IsPopupVisible = IsPopupVisible;
    IsPopupPositionSet = false;
    PoupupData.clear();

}

void Popup::SetPopupPosition(int PopupX, int PopupY)
{
    IsPopupPositionSet = true;
    this->PopupX = PopupX;
    this->PopupY = PopupY;
}

void Popup::SetPopupData(const void* PopupBuffer, int PopupWidth, int PopupHeight)
{

    this->PopupHeight = PopupHeight;
    this->PopupWidth = PopupWidth;
    char * BufferChar = (char *)PopupBuffer;

    PoupupData.assign(BufferChar, BufferChar + PopupWidth * PopupHeight * 4);

}

void Popup::SetBrowserData(const void* BrowserBuffer, int BrowserWidth, int BrowserHeight)
{
    this->BrowserHeight = BrowserHeight;
    this->BrowserWidth = BrowserWidth;
    char * BufferChar = (char *)BrowserBuffer;

    BrowserData.assign(BufferChar, BufferChar + BrowserWidth * BrowserHeight * 4);
}

int Popup::GetBrowserWidth()
{
    return BrowserWidth;
}

int Popup::GetBrowserHeight()
{
    return BrowserHeight;
}

char * Popup::PrepareData()
{
    if(IsPopupVisible && IsPopupPositionSet && !PoupupData.empty() && !BrowserData.empty())
    {
        SendData = BrowserData;
        for(int j = 0;j<PopupHeight;j++)
        {
            for(int i = 0;i<PopupWidth;i++)
            {
                if(i + PopupX < BrowserWidth && j + PopupY < BrowserHeight && i + PopupX >= 0 && j + PopupY >= 0)
                {
                    for(int k = 0;k<4;k++)
                    {
                        SendData[(i + PopupX)*4+(j + PopupY)*BrowserWidth*4 + k] = PoupupData[i*4+j*PopupWidth*4 + k];
                    }
                }
            }

        }
        return SendData.data();

    }else if(!BrowserData.empty())
    {
        SendData = BrowserData;
        return SendData.data();

    }


    return 0;
}
