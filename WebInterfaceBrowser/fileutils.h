#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>

std::wstring GetRelativePathToExe(const std::wstring& RelativePath);
std::wstring GetRelativePathToParentFolder(const std::wstring& RelativePath);
std::wstring ExtractFilenameFromPath(const std::wstring& Path);
bool IsAbsolutePath(const std::wstring& Path);


#endif // FILEUTILS_H
