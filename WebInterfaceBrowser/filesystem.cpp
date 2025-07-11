#include "filesystem.h"
#include <fstream>
#include <string>
#include "convertencoding.h"
#include "boost/filesystem.hpp"

std::string FileSystem::ReadFile(const std::wstring& Path, const std::string& Encoding)
{
    std::vector<char> DataRaw = ReadFileAsBinary(Path);

    if(DataRaw.size() >= 3 && DataRaw[0] == -17 && DataRaw[1] == -69 && DataRaw[2] == -65)
    {
        //Utf-8, strip BOM and return
        return std::string(DataRaw.begin() + 3, DataRaw.end());
    }else if(DataRaw.size() >= 2 && DataRaw[0] == -2 && DataRaw[1] == -1)
    {
        //Utf-16, big endian
        std::string DataString(DataRaw.begin() + 2,DataRaw.end());
        ConverterResult Result = convert_to_utf8(DataString,"UTF-16BE");
        if(Result->WasSuccess)
        {
            return Result->Result;
        }else
        {
            throw std::exception("Failed to change encoding");
        }
        return std::string();
    }else if(DataRaw.size() >= 2 && DataRaw[0] == -1 && DataRaw[1] == -2)
    {
        //Utf-16, little endian
        std::string DataString(DataRaw.begin() + 2,DataRaw.end());
        ConverterResult Result = convert_to_utf8(DataString,"UTF-16LE");
        if(Result->WasSuccess)
        {
            return Result->Result;
        }else
        {
            throw std::exception("Failed to change encoding");
        }
        return std::string();
    }else
    {
        //No BOM, use default encoding
        std::string DataString(DataRaw.begin(),DataRaw.end());
        ConverterResult Result = convert_to_utf8(DataString,Encoding);
        if(Result->WasSuccess)
        {
            return Result->Result;
        }else
        {
            throw std::exception("Failed to change encoding");
        }
        return std::string();
    }
}

std::vector<char> FileSystem::ReadFileAsBinary(const std::wstring& Path)
{
    if(!FileExists(Path))
    {
        throw std::exception("File doesn't exist");
    }

    if(IsFolder(Path))
    {
        throw std::exception("Reading folder instead of file");
    }

    std::vector<char> result;
    std::ifstream ifs(Path, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    result.resize(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&result[0], pos);

    return result;
}

void FileSystem::WriteFileAsBinary(const std::wstring& Path, const std::vector<char>& Data, bool IsAppend)
{
    CreateFolder(ParentFolder(Path));

    if(IsFolder(Path))
    {
        throw std::exception("Writing folder instead of file");
    }

    std::ios_base::openmode AccessType;
    if(IsAppend)
    {
        AccessType = std::ios::binary | std::ios::app;
    }else
    {
        AccessType = std::ios::binary;
    }
    std::ofstream outfile(Path, AccessType);
    if(outfile.is_open())
    {
        std::string DataString(Data.begin(),Data.end());

        outfile<<DataString;
    }else
    {
        throw std::exception("Failed to open file");
    }
    outfile.flush();
    outfile.close();

}

void FileSystem::WriteFile(const std::wstring& Path, const std::string& Data, const std::string& Encoding, bool IsAppend)
{
    ConverterResult Result = convert(Data,"UTF-8",Encoding);
    if(!Result->WasSuccess)
    {
        throw std::exception("Failed to change encoding");
        return;
    }

    WriteFileAsBinary(Path,std::vector<char>(Result->Result.begin(),Result->Result.end()), IsAppend);
}

bool FileSystem::FileExists(const std::wstring& Path)
{
    try
    {
        return boost::filesystem::exists(Path);
    }catch(...)
    {

    }
    return false;
}

unsigned long long FileSystem::FileSize(const std::wstring& Path)
{
    try
    {
        return boost::filesystem::file_size(Path);
    }catch(...)
    {

    }
    return 0;
}

unsigned long long FileSystem::FileLastWriteTime(const std::wstring& Path)
{
    try
    {
        return boost::filesystem::last_write_time(Path);
    }catch(...)
    {

    }
    return 0;
}


std::wstring FileSystem::ParentFolder(const std::wstring& Path)
{
    try
    {
        boost::filesystem::path PathParsed = Path;
        return PathParsed.parent_path().wstring();
    }catch(...)
    {

    }
    return std::wstring();
}

bool FileSystem::IsFolder(const std::wstring& Path)
{
    try
    {
        return boost::filesystem::is_directory(Path);
    }catch(...)
    {

    }
    return false;
}

void FileSystem::CreateFolder(const std::wstring& Path)
{
    if(IsFolder(Path))
    {
        return;
    }

    if(FileExists(Path))
    {
        throw std::exception("Failed to create directory");
    }

    if(!boost::filesystem::create_directories(Path))
    {
        throw std::exception("Failed to create directory");
    }

}

void FileSystem::RemoveFileOrFolder(const std::wstring& Path)
{
    boost::filesystem::remove_all(Path);
}

void FileSystem::MoveFileOrFolder(const std::wstring& Source, const std::wstring& Destanation)
{
    if(FileExists(Source))
    {
        CreateFolder(ParentFolder(Destanation));
        RemoveFileOrFolder(Destanation);
    }else
    {
        throw std::exception("File doesn't exist");
    }

    boost::filesystem::rename(Source, Destanation);

}

void RecursiveCopyFolder(const boost::filesystem::path &src, const boost::filesystem::path &dst)
{
    if(boost::filesystem::is_directory(src))
    {
        boost::filesystem::create_directories(dst);
        for(boost::filesystem::directory_entry& item : boost::filesystem::directory_iterator(src))
        {
            RecursiveCopyFolder(item.path(), dst/item.path().filename());
        }
    }else if (boost::filesystem::is_regular_file(src))
    {
        boost::filesystem::copy(src, dst);
    }
}

void FileSystem::CopyFileOrFolder(const std::wstring& Source, const std::wstring& Destanation)
{
    if(FileExists(Source))
    {
        CreateFolder(ParentFolder(Destanation));
        RemoveFileOrFolder(Destanation);
    }else
    {
        throw std::exception("File doesn't exist");
    }

    if(IsFolder(Source))
        RecursiveCopyFolder(Source, Destanation);
    else
        boost::filesystem::copy_file(Source, Destanation);
}

std::vector<std::wstring> FileSystem::FolderContent(const std::wstring& Path)
{
    if(!FileExists(Path))
    {
        throw std::exception("File doesn't exist");
    }

    if(!IsFolder(Path))
    {
        throw std::exception("Reading file instead of folder");
    }

    std::vector<std::wstring> Res;
    for(boost::filesystem::directory_entry& item : boost::filesystem::directory_iterator(Path))
    {
        Res.push_back(item.path().wstring());
    }

    return Res;
}

