#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <string>
#include <vector>

struct AcceptLanguageCombineResult
{
    std::string Header;
    std::string NavigatorLanguage;
    std::vector<std::string> NavigatorLanguages;
};

AcceptLanguageCombineResult CombineAcceptLanguageWithPattern(const std::string & HeaderOriginal, const std::string & Pattern);
std::string CountryToLanguage(const std::string& CountryOriginal);
std::string LanguageToCountry(const std::string& LanguageOriginal);


#endif // LANGUAGEMANAGER_H
