#include "readallfile.h"
#include <fstream>
#include <windows.h>
#include "converter.h"

std::string ReadAllString(const std::string& filename)
{
    return ReadAllString(s2ws(filename));
}

std::string ReadAllString(const std::wstring& filename)
{
    std::string result;

    try
    {
        std::ifstream ifs(filename, std::ios::binary|std::ios::ate);
        std::ifstream::pos_type pos = ifs.tellg();

        result.resize(pos);

        ifs.seekg(0, std::ios::beg);
        ifs.read(&result[0], pos);
    }
    catch(...)
    {

    }

    return result;
}

std::vector<char> ReadAllBytes(const std::string& filename)
{
    std::vector<char> result;
    try
    {
        std::ifstream ifs(s2ws(filename), std::ios::binary|std::ios::ate);
        std::ifstream::pos_type pos = ifs.tellg();

        result.resize(pos);

        ifs.seekg(0, std::ios::beg);
        ifs.read(&result[0], pos);
    }catch(...)
    {

    }

    return result;
}

void ReadAllBytes(const std::string& filename,std::vector<char>& result)
{
    //std::vector<char> result;
    try
    {
        std::ifstream ifs(s2ws(filename), std::ios::binary|std::ios::ate);
        std::ifstream::pos_type pos = ifs.tellg();

        int len = result.size();
        result.resize(len + pos);

        ifs.seekg(0, std::ios::beg);
        ifs.read(result.data() + len, pos);
    }catch(...)
    {

    }

    //return result;
}

std::vector<FileEntry> GetFilesInDirectory(const std::wstring &Directory)
{
    std::vector<FileEntry> out;
    HANDLE dir;
    WIN32_FIND_DATA file_data;

    if ((dir = FindFirstFile((Directory + std::wstring(L"/*")).c_str(), &file_data)) == INVALID_HANDLE_VALUE)
        return out;

    do {
        const std::string file_name = ws2s(file_data.cFileName);
        const std::string full_file_name = ws2s(Directory) + "/" + file_name;

        const std::wstring file_name_wstring = file_data.cFileName;
        const std::wstring full_file_name_wstring = Directory + L"/" + file_name_wstring;

        const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (file_name[0] == '.')
            continue;

        /*if (is_directory)
            continue;*/

        FileEntry entry;
        entry.FileName = file_name;
        entry.Path = full_file_name;
        entry.FileNameWString = file_name_wstring;
        entry.PathWString = full_file_name_wstring;
        entry.IsDirectory = is_directory;

        out.push_back(entry);
    } while (FindNextFile(dir, &file_data));

    FindClose(dir);

    return out;
}

std::vector<FileEntry> GetFilesInDirectory(const std::string &Directory)
{
    return GetFilesInDirectory(s2ws(Directory));
}
