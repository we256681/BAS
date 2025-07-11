#ifndef FILEEXISTS_H
#define FILEEXISTS_H

#include <string>

bool FileExists(const std::string& Path);
bool FileExists(const std::wstring& Path);

bool FolderExists(const std::string& Path);
bool FolderExists(const std::wstring& Path);

#endif // FILEEXISTS_H
