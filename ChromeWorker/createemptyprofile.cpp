#include "createemptyprofile.h"
#include "fileexists.h"
#include "copyfolder.h"
#include "fileutils.h"
#include "log.h"
#include "converter.h"
#include "replaceall.h"
#include <windows.h>

void CreateEmptyProfile(const std::wstring& ProfilePath)
{
    //Detect if profile already exists
    if(FolderExists(ProfilePath) && FolderExists(ProfilePath + L"/Default") + FileExists(ProfilePath + L"/Login Data"))
        return;

    std::wstring From = GetRelativePathToExe(L"ep\\*");
    std::wstring To = ProfilePath;

    ReplaceAllInPlace(From, L"/", L"\\");
    ReplaceAllInPlace(To, L"/", L"\\");

    CreateDirectoryW(To.c_str(), NULL);

    CopyFolder(From,To);

}
