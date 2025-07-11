#include "converter.h"
#include <regex>

std::string ws2s(const std::wstring& wstr)
{
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}

std::wstring s2ws(const std::string& str)
{
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

std::string GetFirstUtf8Char(std::string& String)
{
    if(String.empty())
    {
        String.clear();
        return std::string();
    }

    int ByteNumber = 0;

    unsigned char FirstByte = (unsigned char)String[0];

    if(FirstByte <= 0x7F)
    {
        ByteNumber = 1;
    }
    else
    {
        ByteNumber =
            ((FirstByte & 240) == 240) ? 4 : (
            ((FirstByte & 224) == 224) ? 3 : (
            ((FirstByte & 192) == 192) ? 2 : -1
        ));
        if (ByteNumber <= 0)
        {
            String.clear();
            return std::string();
        }
    }

    if(String.length() < ByteNumber)
    {
        String.clear();
        return std::string();
    }

    std::string FirstUtf8Char = String.substr(0, ByteNumber);

    String.erase(0, ByteNumber);

    return FirstUtf8Char;
}

