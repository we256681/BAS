#include "processjavascript.h"

std::string ProcessJavaScript(const std::string& Code, const std::string& Id)
{
    std::string Result = Code;
    try{
        static std::regex SafePattern("_BAS_SAFE\\(([^\\)]+)\\)");
        static std::regex HidePattern("_BAS_HIDE\\(([^\\)]+)\\)");
        Result = std::regex_replace(Result, SafePattern, std::string("_BAS_HIDE(BrowserAutomationStudio_SafeData)[\"$1\"]"));
        Result = std::regex_replace(Result, HidePattern, std::string("location.reload['_bas_hide_") + Id + std::string("'][\"$1\"]"));
    }catch (...)
    {
    }
    return Result;
}
