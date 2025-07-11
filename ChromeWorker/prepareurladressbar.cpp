#include "prepareurladressbar.h"
#include <network/uri.hpp>
#include <network/uri/uri_builder.hpp>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

string url_encode(const string &value)
{
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;

    for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << uppercase;
        escaped << '%' << setw(2) << int((unsigned char) c);
        escaped << nouppercase;
    }

    return escaped.str();
}

std::string prepare_url_adressbar(const std::string& UrlOriginal)
{
    //Check if need to do Google search.
    bool DoGoogleSearch = false;

    bool IsSchemePresent = false;
    std::string UrlToCheck = UrlOriginal;
    for(int i = 0;i<UrlOriginal.size();i++)
    {
        char c = UrlOriginal.at(i);
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
        UrlToCheck = std::string("http://") + UrlOriginal;
    }

    std::string SchemeString;
    std::string HostString;

    try
    {
        network::uri ParsedUrl(UrlToCheck);
        SchemeString = ParsedUrl.scheme().to_string();
        HostString = ParsedUrl.host().to_string();
    } catch (...)
    {
        DoGoogleSearch = true;
    }


    std::transform(SchemeString.begin(), SchemeString.end(), SchemeString.begin(), ::tolower);
    std::transform(HostString.begin(), HostString.end(), HostString.begin(), ::tolower);


    if((SchemeString == std::string("http") || SchemeString == std::string("https")) && HostString.find(".") == std::string::npos)
    {
        DoGoogleSearch = true;
    }

    if((SchemeString == std::string("http") || SchemeString == std::string("https")) && (HostString.find(" ") != std::string::npos || HostString.find("%20") != std::string::npos))
    {
        DoGoogleSearch = true;
    }

    std::string Url;

    if(DoGoogleSearch)
    {
        Url = std::string("https://www.google.com/search?q=") + url_encode(UrlOriginal);
    }else
    {
        Url = UrlToCheck;
    }

    return Url;
}
