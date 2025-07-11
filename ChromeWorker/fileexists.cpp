#include "fileexists.h"
#include "windows.h"

bool FileExists(const std::string& Path)
{
  DWORD dwAttrib = GetFileAttributesA(Path.c_str());

  return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
         !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool FileExists(const std::wstring& Path)
{
  DWORD dwAttrib = GetFileAttributesW(Path.c_str());

  return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
         !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool FolderExists(const std::string& Path)
{
  DWORD dwAttrib = GetFileAttributesA(Path.c_str());

  return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
         (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool FolderExists(const std::wstring& Path)
{
  DWORD dwAttrib = GetFileAttributesW(Path.c_str());

  return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
         (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
