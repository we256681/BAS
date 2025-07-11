#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <vector>

class FileSystem
{
    public:
        std::string ReadFile(const std::wstring& Path, const std::string& Encoding);
        std::vector<char> ReadFileAsBinary(const std::wstring& Path);
        void WriteFileAsBinary(const std::wstring& Path, const std::vector<char>& Data, bool IsAppend);
        void WriteFile(const std::wstring& Path, const std::string& Data, const std::string& Encoding, bool IsAppend);
        bool FileExists(const std::wstring& Path);
        unsigned long long FileSize(const std::wstring& Path);
        unsigned long long FileLastWriteTime(const std::wstring& Path);
        std::wstring ParentFolder(const std::wstring& Path);
        bool IsFolder(const std::wstring& Path);
        void CreateFolder(const std::wstring& Path);
        void RemoveFileOrFolder(const std::wstring& Path);
        void MoveFileOrFolder(const std::wstring& Source, const std::wstring& Destanation);
        void CopyFileOrFolder(const std::wstring& Source, const std::wstring& Destanation);
        std::vector<std::wstring> FolderContent(const std::wstring& Path);

    private:


};

#endif // FILESYSTEM_H
