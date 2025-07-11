#include "replaceall.h"

void ReplaceAllInPlace(std::string & str, const std::string& from, const std::string& to)
{
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}


void ReplaceAllInPlace(std::wstring & str, const std::wstring& from, const std::wstring& to)
{
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::wstring::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

std::string ReplaceAll(const std::string & str, const std::string& from, const std::string& to)
{
    std::string res = str;
    ReplaceAllInPlace(res,from,to);
    return res;
}


std::wstring ReplaceAll(const std::wstring & str, const std::wstring& from, const std::wstring& to)
{
    std::wstring res = str;
    ReplaceAllInPlace(res,from,to);
    return res;
}
