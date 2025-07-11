#include "PrepareUrl.h"

std::string PrepareUrl(const std::string& Url)
{
    bool IsSchemePresent = false;
    std::string UrlToCheck = Url;
    for(int i = 0; i < Url.size(); i++)
    {
        char c = Url.at(i);
        if(c == ':')
        {
            IsSchemePresent = i > 0;
            break;
        }
        if(!(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9' || c == '+' || c == '.' || c == '-'))
        {
            //Not allowed char for scheme
            IsSchemePresent = false;
            break;
        }
    }


    if(!IsSchemePresent)
    {
        UrlToCheck = std::string("http://") + Url;
    }

    return UrlToCheck;
}