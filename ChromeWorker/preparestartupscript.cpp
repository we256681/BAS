#include "preparestartupscript.h"
#include "javascriptextensions.h"
#include "modulesdata.h"
#include "multithreading.h"
#include "languagemanager.h"


std::string PrepareConstantStartupScript(BrowserData* Data)
{
    JavaScriptExtensions Extensions;
    std::string extension = Extensions.GetBasicExtension(Data->IsRecord);

    std::string modules;
    modules += Data->BrowserCode;

    std::string hide;
    hide = Extensions.GetHideExtension(Data->_UniqueProcessId);


    std::string AllScript = hide + std::string(";") + extension + std::string(";") + modules + std::string(";");
    AllScript = Extensions.ProcessJs(std::string(";(function () {") + AllScript + std::string("})();"), Data->_UniqueProcessId);
    return AllScript;
}

std::string PrepareMutableStartupScript(BrowserData* Data)
{
    std::string jscode;

    for (auto& Startup : Data->_StartupScript)
    {
         std::string StartupScript = Startup.second.Last();
         if(!StartupScript.empty())
         {
             if(!jscode.empty())
                 jscode += ";";
             jscode += "try{";
             jscode += StartupScript;
             jscode += "}catch(e){};";
         }
    }

    if(!jscode.empty())
        jscode += ";";
    jscode += "try{";
    jscode += PrepareConstantStartupScript(Data);
    jscode += "}catch(e){};";

    jscode += std::string(";_BAS_HIDE(BrowserAutomationStudio_RecaptchaV3ActionList) = ") + picojson::value(Data->_RecaptchaV3List).serialize() + std::string(";");

    JavaScriptExtensions Extensions;
    jscode = Extensions.ProcessJs(jscode,Data->_UniqueProcessId);
    return jscode;
}

std::vector<std::pair<std::string,std::string> > PrepareHeaders(BrowserData* Data)
{
    std::vector<std::pair<std::string,std::string> >HeadersCopy;
    for(std::pair<std::string, std::string>& Header:Data->_Headers)
    {
        if(Header.first == "Accept-Language")
        {
            AcceptLanguageCombineResult CombineAcceptLanguage = CombineAcceptLanguageWithPattern(Header.second,Data->_AcceptLanguagePattern);
            std::pair<std::string, std::string> AcceptLanguage;
            AcceptLanguage.first = Header.first;
            AcceptLanguage.second = CombineAcceptLanguage.Header;
            HeadersCopy.push_back(AcceptLanguage);
        }else
        {
            HeadersCopy.push_back(Header);
        }
    }
    return HeadersCopy;
}

void UpdateBrowserData(BrowserData* Data)
{
    std::string UserAgent;
    std::string AcceptLanguage;

    for(const auto& Header: Data->_Headers)
    {
        if(Header.first == "User-Agent")
        {
            UserAgent = Header.second;
        }else if(Header.first == "Accept-Language")
        {
            AcceptLanguage = Header.second;
        }
    }

    Data->Saver.IsProxySet = Data->IsProxySet;

    Data->Saver.RequestMask = Data->_RequestMask;

    Data->Saver.Headers = Data->_Headers;

    Data->Saver.UserAgent = UserAgent;

    Data->Saver.UserAgentDataBase64 = Data->_UserAgentDataBase64;

    Data->Saver.DeviceScaleFactor = Data->DeviceScaleFactor;

    Data->Saver.Languages = CombineAcceptLanguageWithPattern(AcceptLanguage,Data->_AcceptLanguagePattern).NavigatorLanguages;

    Data->Saver.LanguagesHeader = CombineAcceptLanguageWithPattern(AcceptLanguage,Data->_AcceptLanguagePattern).Header;

    Data->Saver.StartupScript = PrepareMutableStartupScript(Data);

    Data->Saver.Save();
}

