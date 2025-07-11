#ifndef FILLALPHARECTANGLE_H
#define FILLALPHARECTANGLE_H

#include <windows.h>


BOOL FillAlphaRect(HDC hdc, const RECT* lpcrect, HBRUSH brush, BYTE alpha);

#endif // FILLALPHARECTANGLE_H
