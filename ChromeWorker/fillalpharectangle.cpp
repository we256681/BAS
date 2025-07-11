#include "fillalpharectangle.h"

BOOL FillAlphaRect(HDC hdc, const RECT* lpcrect, HBRUSH brush, BYTE alpha)
{
    SIZE_T bmWidth = lpcrect->right - lpcrect->left;
    SIZE_T bmHeight = lpcrect->bottom - lpcrect->top;
    SIZE_T byteCount = bmWidth * bmHeight * 4;
    BITMAPINFO bmi;
    void* pvBits;

    ZeroMemory(&bmi, 0);
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = bmWidth;
    bmi.bmiHeader.biHeight = bmHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = byteCount;

    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBM = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0x0);
    HBITMAP oldBM = (HBITMAP)SelectObject(memDC, memBM);

    for (SIZE_T i = 0, size = bmWidth * bmHeight; i < size; ++i)
        ((UINT32*)pvBits)[i] = 0xff000000;

    SetBkMode(memDC, GetBkMode(hdc));
    SetBkColor(memDC, GetBkColor(hdc));
    HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, brush);
    HPEN oldPen = (HPEN)SelectObject(memDC, GetStockObject(NULL_PEN));
    Rectangle(memDC, 0, 0, bmWidth, bmHeight);
    SelectObject(memDC, oldBrush);
    SelectObject(memDC, oldPen);

    for (SIZE_T i = 3; i < byteCount; i += 4)
            ((BYTE*)pvBits)[i] = 0xff - ((BYTE*)pvBits)[i];

    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = alpha;
    bf.AlphaFormat = AC_SRC_ALPHA;
    BOOL res = AlphaBlend(
        hdc,
        lpcrect->left,
        lpcrect->top,
        bmWidth,
        bmHeight,
        memDC,
        0,
        0,
        bmWidth,
        bmHeight,
        bf);

    SelectObject(memDC, oldBM);
    DeleteDC(memDC);
    DeleteObject(memBM);
    return res;
}
