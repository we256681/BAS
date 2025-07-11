#include "deletefolder.h"
#include <windows.h>


void DeleteFolder(const std::wstring& Folder)
{
    SHFILEOPSTRUCTW s = { 0 };
    s.wFunc = FO_DELETE;
    s.fFlags =  FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
    std::wstring FolderFinal = Folder;
    FolderFinal.push_back(L'\0');
    s.pFrom = FolderFinal.c_str();
    SHFileOperationW(&s);
}
