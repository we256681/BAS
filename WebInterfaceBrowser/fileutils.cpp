#include "fileutils.h"
#include <vector>
#include <windows.h>
#include <algorithm>

std::wstring GetRelativePathToExe(const std::wstring& RelativePath)
{
    std::vector<wchar_t> buffer(MAX_PATH);
    int size = GetModuleFileName(NULL, buffer.data(), MAX_PATH);
    buffer.resize(size);
    wchar_t separator[] = {L'\\'};
    std::vector<wchar_t>::iterator it = std::find_end(buffer.begin(), buffer.end(), separator, separator + 1);
    size = it - buffer.begin() + 1;
    buffer.resize(size);

    buffer.insert(buffer.end(), RelativePath.begin(), RelativePath.end());
    //buffer.push_back(0);

    return std::wstring(buffer.data(),buffer.size());
}

std::wstring GetRelativePathToParentFolder(const std::wstring& RelativePath)
{
    std::vector<wchar_t> buffer(MAX_PATH);
    int size = GetModuleFileName(NULL, buffer.data(), MAX_PATH);
    buffer.resize(size);
    wchar_t separator[] = {L'\\'};
    std::vector<wchar_t>::iterator it = std::find_end(buffer.begin(), buffer.end(), separator, separator + 1);
    size = it - buffer.begin();
    buffer.resize(size);
    it = std::find_end(buffer.begin(), buffer.end(), separator, separator + 1);
    size = it - buffer.begin() + 1;
    buffer.resize(size);

    buffer.insert(buffer.end(), RelativePath.begin(), RelativePath.end());
    //buffer.push_back(0);

    return std::wstring(buffer.data(),buffer.size());
}

std::wstring ExtractFilenameFromPath(const std::wstring& Path)
{
    std::vector<wchar_t> buffer(Path.begin(), Path.end());
    wchar_t separator[] = {L'\\'};
    std::vector<wchar_t>::iterator it = std::find_end(buffer.begin(), buffer.end(), separator, separator + 1);

    if(it == buffer.end())
    {
        return Path;
    }

    std::wstring Res(it + 1, buffer.end());

    return Res;
}

bool IsAbsolutePath(const std::wstring& Path)
{
    return Path.size() >= 2 && Path[1] == L':';
}
