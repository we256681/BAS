#include "copyfolder.h"
#include <windows.h>


void CopyFolder(const std::wstring& From, const std::wstring& To)
{
    SHFILEOPSTRUCTW s = { 0 };
    s.wFunc = FO_COPY;
    s.fFlags =  FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
    std::wstring FromFinal = From;
    std::wstring ToFinal = To;
    ToFinal.push_back(L'\0');
    FromFinal.push_back(L'\0');
    s.pTo = ToFinal.c_str();
    s.pFrom = FromFinal.c_str();
    SHFileOperationW(&s);
}
