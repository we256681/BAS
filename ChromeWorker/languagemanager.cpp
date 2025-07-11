#include "languagemanager.h"
#include "trim.h"
#include "split.h"
#include "startwith.h"
#include <regex>
#include <algorithm>
#include <iostream>
#include <math.h>


struct AcceptLanguageReplacePattern
{
    enum AcceptLanguageReplaceType
    {
        Same,
        SameExceptLast,
        SameExceptLastAndFirst,
        ArithmeticProgression,
        ArithmeticProgressionExceptLast,
        ArithmeticProgressionExceptLastAndFirst,
        Custom,
    }Type;

    double First;
    double Last;
    double SameValue;
    double Difference;
    std::vector<double> CustomFactors;

};

struct ParsedAcceptLanguageItem
{
    std::string Country;
    std::string Language;
    std::string FactorString;
    std::string Raw;

    bool IsSame(double x,double y)
    {
        return abs(x - y)<0.001f;
    }

    std::string Build()
    {
        std::string Res;
        Res = GetLanguageAndCountry();
        if(!IsSame(1.0,GetFactor()))
        {
            Res+=std::string(";q=") + FactorString;
        }

        return Res;
    }
    std::string GetLanguageAndCountry()
    {
        if(Country.size() > 0)
            return Language + "-" + Country;
        return Language;
    }
    double GetFactor()
    {
        if(FactorString.empty())
            return 1.0;
        std::stringstream ss(FactorString);
        double Res;
        ss >> Res;
        return Res;
    }

    void SetFactor(double Factor)
    {
        if(IsSame(1.0,Factor))
        {
            FactorString.clear();
            return;
        }
        FactorString = std::to_string(Factor);
        FactorString.erase ( FactorString.find_last_not_of('0') + 1, std::string::npos );
    }

    std::string GetFactorString()
    {
        std::string str = std::to_string(GetFactor());
        str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
        return str;
    }

    bool IsLocalLanguage()
    {
        std::string LanguageLower = Language;
        std::transform(LanguageLower.begin(), LanguageLower.end(), LanguageLower.begin(), ::tolower);

        std::string CountryLower = Country;
        std::transform(CountryLower.begin(), CountryLower.end(), CountryLower.begin(), ::tolower);

        return (LanguageLower != "en" && LanguageLower != "*") || (CountryLower != "gb" && CountryLower != "us" && !CountryLower.empty());
    }
};

struct ParsedAcceptLanguage
{
    std::vector<ParsedAcceptLanguageItem> Items;

    bool HasCountries()
    {
        for(ParsedAcceptLanguageItem& Item: Items)
        {
            if(!Item.Country.empty())
            {
                return true;
            }
        }
        return false;
    }

    std::string Build()
    {
        bool Spaces = HasSpaces();
        std::string Res;
        for(ParsedAcceptLanguageItem& Item: Items)
        {
            if(!Res.empty())
            {
                if(Spaces)
                {
                    Res += ", ";
                }else
                {
                    Res += ",";
                }
            }
            Res += Item.Build();
        }
        return Res;
    }

    std::string GetLanguage()
    {
        if(Items.size() == 0)
        {
            return std::string();
        }
        return Items[0].GetLanguageAndCountry();
    }
    std::vector<std::string> GetLanguages()
    {
        std::vector<std::string> Res;
        for(ParsedAcceptLanguageItem& Item: Items)
        {
            if(std::find(Res.begin(), Res.end(), Item.GetLanguageAndCountry()) == Res.end())
            {
                if(Item.GetLanguageAndCountry() != "*")
                    Res.push_back(Item.GetLanguageAndCountry());
            }
        }
        return Res;
    }

    bool IsSame(double x,double y)
    {
        return abs(x - y)<0.001f;
    }

    bool HasSpaces()
    {
        for(ParsedAcceptLanguageItem& Item: Items)
        {
            if(starts_with(Item.Raw," "))
            {
                return true;
            }
        }
        return false;
    }

    bool IsOneItemWithLocalLanguage()
    {
        int LocalLanguageNumber = 0;
        for(ParsedAcceptLanguageItem& Item: Items)
        {
            if(Item.IsLocalLanguage())
            {
                LocalLanguageNumber++;
            }
        }
        return LocalLanguageNumber == 1;

    }

    bool IsTwoItemsWithLocalLanguage()
    {
        int LocalLanguageNumber = 0;
        for(ParsedAcceptLanguageItem& Item: Items)
        {
            if(Item.IsLocalLanguage())
            {
                LocalLanguageNumber++;
            }
        }
        return LocalLanguageNumber == 2;

    }

    bool IsNoLocalLanguage()
    {
        int LocalLanguageNumber = 0;
        for(ParsedAcceptLanguageItem& Item: Items)
        {
            if(Item.IsLocalLanguage())
            {
                LocalLanguageNumber++;
            }
        }
        return LocalLanguageNumber == 0;
    }

    bool IsFactorsSame()
    {
        if(Items.size() == 0)
            return true;
        double SameFactor = Items[0].GetFactor();
        for(ParsedAcceptLanguageItem& Item: Items)
        {
            if(!IsSame(SameFactor, Item.GetFactor()))
            {
                return false;
            }
        }
        return true;
    }

    bool IsFactorsSameExceptLast()
    {
        if(Items.size() < 3)
            return true;
        double SameFactor = Items[0].GetFactor();
        int i = -1;
        for(ParsedAcceptLanguageItem& Item: Items)
        {
            i++;
            if(Items.size() - 1 == i)
            {
                continue;
            }

            if(!IsSame(SameFactor, Item.GetFactor()))
            {
                return false;
            }
        }
        return true;
    }

    bool IsFactorsSameExceptLastAndFirst()
    {
        if(Items.size() < 4)
            return true;
        double SameFactor = Items[1].GetFactor();
        int i = -1;
        for(ParsedAcceptLanguageItem& Item: Items)
        {
            i++;
            if(Items.size() - 1 == i || i == 0)
            {
                continue;
            }

            if(!IsSame(SameFactor, Item.GetFactor()))
            {
                return false;
            }
        }
        return true;
    }

    double IsFactorsArithmeticProgression()
    {
        if(Items.size() < 3)
            return 0.1;
        double Delta = Items[0].GetFactor() - Items[1].GetFactor();
        double FirstFactor = Items[0].GetFactor();

        int i = -1;
        for(ParsedAcceptLanguageItem& Item: Items)
        {
            i++;
            if(i == 1 || i == 0)
            {
                continue;
            }
            if(!IsSame(FirstFactor - i * Delta, Item.GetFactor()))
            {
                return -1.0;
            }
        }

        return Delta;
    }
    double IsFactorsArithmeticProgressionExceptLast()
    {
        if(Items.size() < 4)
            return 0.1;
        double Delta = Items[0].GetFactor() - Items[1].GetFactor();
        double FirstFactor = Items[0].GetFactor();

        int i = -1;
        for(ParsedAcceptLanguageItem& Item: Items)
        {
            i++;
            if(i == 1 || i == 0 || Items.size() - 1 == i)
            {
                continue;
            }
            if(!IsSame(FirstFactor - i * Delta, Item.GetFactor()))
            {
                return -1.0;
            }
        }

        return Delta;
    }
    double IsFactorsArithmeticProgressionExceptLastAndFirst()
    {
        if(Items.size() < 5)
            return 0.1;
        double Delta = Items[1].GetFactor() - Items[2].GetFactor();
        double FirstFactor = Items[1].GetFactor();

        int i = -1;
        for(ParsedAcceptLanguageItem& Item: Items)
        {
            i++;
            if(i == 2 || i == 1 || i == 0 || Items.size() - 1 == i)
            {
                continue;
            }
            if(!IsSame(FirstFactor - i * Delta, Item.GetFactor()))
            {
                return -1.0;
            }
        }

        return Delta;
    }

    void ReplaceFactors(AcceptLanguageReplacePattern Pattern)
    {
        if(Pattern.Type == AcceptLanguageReplacePattern::Same)
        {
            for(ParsedAcceptLanguageItem& Item: Items)
            {
                if(Item.Language != "*")
                    Item.SetFactor(Pattern.SameValue);
            }
        }

        if(Pattern.Type == AcceptLanguageReplacePattern::SameExceptLast)
        {
            for(ParsedAcceptLanguageItem& Item: Items)
            {
                if(Item.Language != "*")
                    Item.SetFactor(Pattern.SameValue);
            }
            if(Items.size() > 0)
            {
                Items[Items.size() - 1].SetFactor(Pattern.Last);
            }
        }

        if(Pattern.Type == AcceptLanguageReplacePattern::SameExceptLastAndFirst)
        {
            for(ParsedAcceptLanguageItem& Item: Items)
            {
                if(Item.Language != "*")
                    Item.SetFactor(Pattern.SameValue);
            }
            if(Items.size() > 0)
            {
                Items[0].SetFactor(Pattern.First);
            }
            if(Items.size() > 1)
            {
                Items[Items.size() - 1].SetFactor(Pattern.Last);
            }
        }

        if(Pattern.Type == AcceptLanguageReplacePattern::ArithmeticProgression)
        {
            double Factor = 1.0f;
            for(ParsedAcceptLanguageItem& Item: Items)
            {
                if(Item.Language != "*")
                {
                    Item.SetFactor(Factor);
                    Factor -= Pattern.Difference;
                }
            }
        }


        if(Pattern.Type == AcceptLanguageReplacePattern::ArithmeticProgressionExceptLast)
        {
            double Factor = 1.0f;
            for(ParsedAcceptLanguageItem& Item: Items)
            {
                if(Item.Language != "*")
                {
                    Item.SetFactor(Factor);
                    Factor -= Pattern.Difference;
                }
            }
            if(Items.size() > 0)
            {
                Items[0].SetFactor(Pattern.First);
            }
        }

        if(Pattern.Type == AcceptLanguageReplacePattern::ArithmeticProgressionExceptLastAndFirst)
        {
            double Factor = 1.0f;
            for(ParsedAcceptLanguageItem& Item: Items)
            {
                if(Item.Language != "*")
                {
                    Item.SetFactor(Factor);
                    Factor -= Pattern.Difference;
                }
            }
            if(Items.size() > 0)
            {
                Items[0].SetFactor(Pattern.First);
            }
            if(Items.size() > 1)
            {
                Items[Items.size() - 1].SetFactor(Pattern.Last);
            }
        }

        if(Pattern.Type == AcceptLanguageReplacePattern::Custom)
        {
            int Index = 0;
            for(ParsedAcceptLanguageItem& Item: Items)
            {
                if(Index < Pattern.CustomFactors.size())
                {
                    if(Item.Language != "*")
                    {
                        Item.SetFactor(Pattern.CustomFactors[Index]);
                        Index++;
                    }
                }
            }
        }


    }


    AcceptLanguageReplacePattern GetReplacePattern()
    {
        AcceptLanguageReplacePattern Result;


        if(Items.size() >= 2 && IsFactorsSame())
        {
            Result.Type = AcceptLanguageReplacePattern::Same;
            Result.SameValue = Items[0].GetFactor();
            return Result;
        }

        if(Items.size() >= 3 && IsFactorsSameExceptLast())
        {
            Result.Type = AcceptLanguageReplacePattern::SameExceptLast;
            Result.SameValue = Items[0].GetFactor();
            Result.Last = Items[Items.size() - 1].GetFactor();
            return Result;
        }

        if(Items.size() >= 4 && IsFactorsSameExceptLastAndFirst())
        {
            Result.Type = AcceptLanguageReplacePattern::SameExceptLastAndFirst;
            Result.SameValue = Items[1].GetFactor();
            Result.Last = Items[Items.size() - 1].GetFactor();
            Result.First = Items[0].GetFactor();
        }

        double Difference = IsFactorsArithmeticProgression();

        if(Items.size() >= 3 && Difference >= 0.0)
        {
            Result.Type = AcceptLanguageReplacePattern::ArithmeticProgression;
            Result.Difference = Difference;
            return Result;
        }

        Difference = IsFactorsArithmeticProgressionExceptLast();

        if(Items.size() >= 4 && Difference >= 0.0)
        {
            Result.Type = AcceptLanguageReplacePattern::ArithmeticProgressionExceptLast;
            Result.Difference = Difference;
            Result.First = Items[0].GetFactor();
            return Result;
        }

        Difference = IsFactorsArithmeticProgressionExceptLastAndFirst();

        if(Items.size() >= 5 && Difference >= 0.0)
        {
            Result.Type = AcceptLanguageReplacePattern::ArithmeticProgressionExceptLastAndFirst;
            Result.Difference = Difference;
            Result.First = Items[0].GetFactor();
            Result.Last = Items[Items.size() - 1].GetFactor();
            return Result;
        }


        Result.Type = AcceptLanguageReplacePattern::Custom;
        for(ParsedAcceptLanguageItem& Item: Items)
        {
            Result.CustomFactors.push_back(Item.GetFactor());
        }


        return Result;
    }


    bool IsEmpty()
    {
        return Items.size() == 0;
    }
};

ParsedAcceptLanguage ParseAcceptLanguageHeader(const std::string & Header)
{
    //Initialization
    ParsedAcceptLanguage Res;


    //Get raw items
    std::string Data = Header;
    Data = trim(Data);
    std::vector<std::string> RawItems = split(Data,',');


    //Parse all
    for(std::string& RawItemNoTrim: RawItems)
    {
        ParsedAcceptLanguageItem Item;
        Item.Raw = RawItemNoTrim;

        std::string RawItem = trim(RawItemNoTrim);
        bool IsGood = true;

        try{
            std::regex Regexp("([^\\;\\-]+)(\\-([^\\;]+))?(\\;\\s*q\\s*\\=\\s*(.+))?");
            std::smatch Match;

            if (std::regex_search(RawItem, Match, Regexp))
            {
                if(Match.size() > 1)
                {
                    Item.Language = Match[1];
                }

                if(Match.size() > 3)
                {
                    Item.Country = Match[3];
                }

                if(Match.size() > 5)
                {
                    Item.FactorString = Match[5];
                }

            }else
                IsGood = false;
        }catch(...)
        {
            IsGood = false;
        }

        if(IsGood)
            Res.Items.push_back(Item);
    }


    return Res;
}

std::string CountryToLanguage(const std::string& CountryOriginal)
{
    std::string Country = CountryOriginal;
    std::transform(Country.begin(), Country.end(), Country.begin(), ::tolower);
    if(Country == "ax")
    {
        return "sv";
    }
    if(Country == "al")
    {
        return "sq";
    }
    if(Country == "dz")
    {
        return "ar";
    }
    if(Country == "ad")
    {
        return "ca";
    }
    if(Country == "ao")
    {
        return "kg";
    }
    if(Country == "ag")
    {
        return "en";
    }
    if(Country == "ar")
    {
        return "es";
    }
    if(Country == "am")
    {
        return "hy";
    }
    if(Country == "aw")
    {
        return "nl";
    }
    if(Country == "au")
    {
        return "en";
    }
    if(Country == "at")
    {
        return "de";
    }
    if(Country == "az")
    {
        return "az";
    }
    if(Country == "bs")
    {
        return "en";
    }
    if(Country == "bh")
    {
        return "ar";
    }
    if(Country == "bd")
    {
        return "bn";
    }
    if(Country == "bb")
    {
        return "en";
    }
    if(Country == "by")
    {
        return "be";
    }
    if(Country == "be")
    {
        return "nl";
    }
    if(Country == "bz")
    {
        return "en";
    }
    if(Country == "bj")
    {
        return "fr";
    }
    if(Country == "bt")
    {
        return "dz";
    }
    if(Country == "bo")
    {
        return "ay";
    }
    if(Country == "ba")
    {
        return "bs";
    }
    if(Country == "bw")
    {
        return "en";
    }
    if(Country == "br")
    {
        return "pt";
    }
    if(Country == "bn")
    {
        return "ms";
    }
    if(Country == "bg")
    {
        return "bg";
    }
    if(Country == "bf")
    {
        return "fr";
    }
    if(Country == "bi")
    {
        return "fr";
    }
    if(Country == "kh")
    {
        return "km";
    }
    if(Country == "cm")
    {
        return "en";
    }
    if(Country == "ca")
    {
        return "en";
    }
    if(Country == "cv")
    {
        return "pt";
    }
    if(Country == "cf")
    {
        return "fr";
    }
    if(Country == "td")
    {
        return "ar";
    }
    if(Country == "cl")
    {
        return "es";
    }
    if(Country == "cn")
    {
        return "zh";
    }
    if(Country == "co")
    {
        return "es";
    }
    if(Country == "km")
    {
        return "ar";
    }
    if(Country == "cg")
    {
        return "fr";
    }
    if(Country == "cd")
    {
        return "fr";
    }
    if(Country == "cr")
    {
        return "es";
    }
    if(Country == "ci")
    {
        return "fr";
    }
    if(Country == "hr")
    {
        return "hr";
    }
    if(Country == "cu")
    {
        return "es";
    }
    if(Country == "cw")
    {
        return "nl";
    }
    if(Country == "cy")
    {
        return "el";
    }
    if(Country == "cz")
    {
        return "cs";
    }
    if(Country == "dk")
    {
        return "da";
    }
    if(Country == "dj")
    {
        return "aa";
    }
    if(Country == "dm")
    {
        return "en";
    }
    if(Country == "do")
    {
        return "es";
    }
    if(Country == "ec")
    {
        return "es";
    }
    if(Country == "eg")
    {
        return "ar";
    }
    if(Country == "sv")
    {
        return "es";
    }
    if(Country == "gq")
    {
        return "fr";
    }
    if(Country == "er")
    {
        return "ar";
    }
    if(Country == "ee")
    {
        return "et";
    }
    if(Country == "et")
    {
        return "am";
    }
    if(Country == "fo")
    {
        return "da";
    }
    if(Country == "fj")
    {
        return "en";
    }
    if(Country == "fi")
    {
        return "fi";
    }
    if(Country == "fr")
    {
        return "fr";
    }
    if(Country == "gf")
    {
        return "fr";
    }
    if(Country == "pf")
    {
        return "fr";
    }
    if(Country == "ga")
    {
        return "fr";
    }
    if(Country == "gm")
    {
        return "en";
    }
    if(Country == "ge")
    {
        return "ka";
    }
    if(Country == "de")
    {
        return "de";
    }
    if(Country == "gh")
    {
        return "ak";
    }
    if(Country == "gr")
    {
        return "el";
    }
    if(Country == "gd")
    {
        return "en";
    }
    if(Country == "gp")
    {
        return "fr";
    }
    if(Country == "gt")
    {
        return "es";
    }
    if(Country == "gn")
    {
        return "fr";
    }
    if(Country == "gw")
    {
        return "pt";
    }
    if(Country == "gy")
    {
        return "en";
    }
    if(Country == "ht")
    {
        return "fr";
    }
    if(Country == "va")
    {
        return "it";
    }
    if(Country == "hn")
    {
        return "es";
    }
    if(Country == "hk")
    {
        return "zh";
    }
    if(Country == "hu")
    {
        return "hu";
    }
    if(Country == "is")
    {
        return "is";
    }
    if(Country == "in")
    {
        return "bn";
    }
    if(Country == "id")
    {
        return "id";
    }
    if(Country == "ir")
    {
        return "fa";
    }
    if(Country == "iq")
    {
        return "ar";
    }
    if(Country == "ie")
    {
        return "en";
    }
    if(Country == "il")
    {
        return "ar";
    }
    if(Country == "it")
    {
        return "fr";
    }
    if(Country == "jm")
    {
        return "en";
    }
    if(Country == "jp")
    {
        return "ja";
    }
    if(Country == "je")
    {
        return "fr";
    }
    if(Country == "jo")
    {
        return "ar";
    }
    if(Country == "kz")
    {
        return "kk";
    }
    if(Country == "ke")
    {
        return "en";
    }
    if(Country == "ki")
    {
        return "en";
    }
    if(Country == "kp")
    {
        return "ko";
    }
    if(Country == "kr")
    {
        return "ko";
    }
    if(Country == "kw")
    {
        return "ar";
    }
    if(Country == "kg")
    {
        return "ky";
    }
    if(Country == "la")
    {
        return "lo";
    }
    if(Country == "lv")
    {
        return "lv";
    }
    if(Country == "lb")
    {
        return "ar";
    }
    if(Country == "ls")
    {
        return "en";
    }
    if(Country == "lr")
    {
        return "en";
    }
    if(Country == "ly")
    {
        return "ar";
    }
    if(Country == "li")
    {
        return "de";
    }
    if(Country == "lt")
    {
        return "lt";
    }
    if(Country == "lu")
    {
        return "fr";
    }
    if(Country == "mo")
    {
        return "zh";
    }
    if(Country == "mk")
    {
        return "mk";
    }
    if(Country == "mg")
    {
        return "fr";
    }
    if(Country == "mw")
    {
        return "ny";
    }
    if(Country == "my")
    {
        return "en";
    }
    if(Country == "mv")
    {
        return "dv";
    }
    if(Country == "ml")
    {
        return "bm";
    }
    if(Country == "mt")
    {
        return "en";
    }
    if(Country == "mh")
    {
        return "en";
    }
    if(Country == "mq")
    {
        return "fr";
    }
    if(Country == "mr")
    {
        return "ar";
    }
    if(Country == "mu")
    {
        return "en";
    }
    if(Country == "yt")
    {
        return "fr";
    }
    if(Country == "mx")
    {
        return "es";
    }
    if(Country == "fm")
    {
        return "en";
    }
    if(Country == "md")
    {
        return "ro";
    }
    if(Country == "mc")
    {
        return "fr";
    }
    if(Country == "mn")
    {
        return "mn";
    }
    if(Country == "ma")
    {
        return "ar";
    }
    if(Country == "mz")
    {
        return "pt";
    }
    if(Country == "mm")
    {
        return "my";
    }
    if(Country == "na")
    {
        return "en";
    }
    if(Country == "nr")
    {
        return "en";
    }
    if(Country == "np")
    {
        return "ne";
    }
    if(Country == "nl")
    {
        return "nl";
    }
    if(Country == "nc")
    {
        return "fr";
    }
    if(Country == "nz")
    {
        return "en";
    }
    if(Country == "ni")
    {
        return "es";
    }
    if(Country == "ne")
    {
        return "ar";
    }
    if(Country == "ng")
    {
        return "en";
    }
    if(Country == "no")
    {
        return "nb";
    }
    if(Country == "om")
    {
        return "ar";
    }
    if(Country == "pk")
    {
        return "en";
    }
    if(Country == "pw")
    {
        return "en";
    }
    if(Country == "pa")
    {
        return "es";
    }
    if(Country == "pg")
    {
        return "en";
    }
    if(Country == "py")
    {
        return "gn";
    }
    if(Country == "pe")
    {
        return "ay";
    }
    if(Country == "ph")
    {
        return "en";
    }
    if(Country == "pl")
    {
        return "pl";
    }
    if(Country == "pt")
    {
        return "pt";
    }
    if(Country == "pr")
    {
        return "es";
    }
    if(Country == "qa")
    {
        return "ar";
    }
    if(Country == "re")
    {
        return "fr";
    }
    if(Country == "ro")
    {
        return "ro";
    }
    if(Country == "ru")
    {
        return "ru";
    }
    if(Country == "rw")
    {
        return "en";
    }
    if(Country == "bl")
    {
        return "fr";
    }
    if(Country == "kn")
    {
        return "en";
    }
    if(Country == "lc")
    {
        return "en";
    }
    if(Country == "pm")
    {
        return "fr";
    }
    if(Country == "vc")
    {
        return "en";
    }
    if(Country == "ws")
    {
        return "en";
    }
    if(Country == "sm")
    {
        return "it";
    }
    if(Country == "st")
    {
        return "pt";
    }
    if(Country == "sa")
    {
        return "ar";
    }
    if(Country == "sn")
    {
        return "fr";
    }
    if(Country == "rs")
    {
        return "sr";
    }
    if(Country == "sc")
    {
        return "en";
    }
    if(Country == "sl")
    {
        return "en";
    }
    if(Country == "sg")
    {
        return "zh";
    }
    if(Country == "sx")
    {
        return "nl";
    }
    if(Country == "sk")
    {
        return "cs";
    }
    if(Country == "si")
    {
        return "it";
    }
    if(Country == "sb")
    {
        return "en";
    }
    if(Country == "so")
    {
        return "ar";
    }
    if(Country == "za")
    {
        return "af";
    }
    if(Country == "ss")
    {
        return "en";
    }
    if(Country == "es")
    {
        return "es";
    }
    if(Country == "lk")
    {
        return "en";
    }
    if(Country == "sd")
    {
        return "ar";
    }
    if(Country == "sr")
    {
        return "nl";
    }
    if(Country == "sz")
    {
        return "en";
    }
    if(Country == "se")
    {
        return "sv";
    }
    if(Country == "ch")
    {
        return "fr";
    }
    if(Country == "sy")
    {
        return "ar";
    }
    if(Country == "tw")
    {
        return "zh";
    }
    if(Country == "tj")
    {
        return "fa";
    }
    if(Country == "tz")
    {
        return "en";
    }
    if(Country == "th")
    {
        return "th";
    }
    if(Country == "tl")
    {
        return "pt";
    }
    if(Country == "tg")
    {
        return "ee";
    }
    if(Country == "to")
    {
        return "en";
    }
    if(Country == "tt")
    {
        return "en";
    }
    if(Country == "tn")
    {
        return "ar";
    }
    if(Country == "tr")
    {
        return "tr";
    }
    if(Country == "tm")
    {
        return "tk";
    }
    if(Country == "tv")
    {
        return "en";
    }
    if(Country == "ug")
    {
        return "en";
    }
    if(Country == "ua")
    {
        return "uk";
    }
    if(Country == "ae")
    {
        return "ar";
    }
    if(Country == "gb")
    {
        return "en";
    }
    if(Country == "us")
    {
        return "en";
    }
    if(Country == "uy")
    {
        return "es";
    }
    if(Country == "uz")
    {
        return "uz";
    }
    if(Country == "vu")
    {
        return "bi";
    }
    if(Country == "ve")
    {
        return "es";
    }
    if(Country == "vn")
    {
        return "vi";
    }
    if(Country == "wf")
    {
        return "fr";
    }
    if(Country == "eh")
    {
        return "es";
    }
    if(Country == "ye")
    {
        return "ar";
    }
    if(Country == "zm")
    {
        return "en";
    }
    if(Country == "zw")
    {
        return "en";
    }
    return Country;
}
std::string LanguageToCountry(const std::string& LanguageOriginal)
{
    std::string Language = LanguageOriginal;
    std::transform(Language.begin(), Language.end(), Language.begin(), ::tolower);
    //Afar
    if(Language == "aa")
    {
        return "dj";
    }
    //Afrikaans
    if(Language == "af")
    {
        return "za";
    }
    //Akan
    if(Language == "ak")
    {
        return "gh";
    }
    //Albanian
    if(Language == "sq")
    {
        return "al";
    }
    //Amharic
    if(Language == "am")
    {
        return "et";
    }
    //Arabic
    if(Language == "ar")
    {
        return "eg";
    }
    //Armenian
    if(Language == "hy")
    {
        return "am";
    }
    //Aymara
    if(Language == "ay")
    {
        return "bo";
    }
    //Azerbaijani
    if(Language == "az")
    {
        return "az";
    }
    //Bambara
    if(Language == "bm")
    {
        return "ml";
    }
    //Belarusian
    if(Language == "be")
    {
        return "by";
    }
    //Bengali,Bangla
    if(Language == "bn")
    {
        return "in";
    }
    //Bislama
    if(Language == "bi")
    {
        return "vu";
    }
    //Bosnian
    if(Language == "bs")
    {
        return "ba";
    }
    //Bulgarian
    if(Language == "bg")
    {
        return "bg";
    }
    //Burmese
    if(Language == "my")
    {
        return "mm";
    }
    //Catalan,Valencian
    if(Language == "ca")
    {
        return "ad";
    }
    //Chichewa,Chewa,Nyanja
    if(Language == "ny")
    {
        return "mw";
    }
    //Chinese
    if(Language == "zh")
    {
        return "cn";
    }
    //Croatian
    if(Language == "hr")
    {
        return "hr";
    }
    //Czech
    if(Language == "cs")
    {
        return "cz";
    }
    //Danish
    if(Language == "da")
    {
        return "dk";
    }
    //Divehi,Dhivehi,Maldivian
    if(Language == "dv")
    {
        return "mv";
    }
    //Dutch
    if(Language == "nl")
    {
        return "nl";
    }
    //Dzongkha
    if(Language == "dz")
    {
        return "bt";
    }
    //English
    if(Language == "en")
    {
        return "us";
    }
    //Estonian
    if(Language == "et")
    {
        return "ee";
    }
    //Ewe
    if(Language == "ee")
    {
        return "gh";
    }
    //Fijian
    if(Language == "fj")
    {
        return "fj";
    }
    //Finnish
    if(Language == "fi")
    {
        return "fi";
    }
    //French
    if(Language == "fr")
    {
        return "fr";
    }
    //Fula,Fulah,Pulaar,Pular
    if(Language == "ff")
    {
        return "bj";
    }
    //Georgian
    if(Language == "ka")
    {
        return "ge";
    }
    //German
    if(Language == "de")
    {
        return "de";
    }
    //Greek
    if(Language == "el")
    {
        return "gr";
    }
    //Guaraní
    if(Language == "gn")
    {
        return "bo";
    }
    //Gujarati
    if(Language == "gu")
    {
        return "in";
    }
    //Haitian,Haitian Creole
    if(Language == "ht")
    {
        return "ht";
    }
    //Hausa
    if(Language == "ha")
    {
        return "ng";
    }
    //Hebrew
    if(Language == "he")
    {
        return "il";
    }
    //Hindi
    if(Language == "hi")
    {
        return "in";
    }
    //Hiri Motu
    if(Language == "ho")
    {
        return "pg";
    }
    //Hungarian
    if(Language == "hu")
    {
        return "hu";
    }
    //Indonesian
    if(Language == "id")
    {
        return "id";
    }
    //Irish
    if(Language == "ga")
    {
        return "ie";
    }
    //Igbo
    if(Language == "ig")
    {
        return "ng";
    }
    //Icelandic
    if(Language == "is")
    {
        return "is";
    }
    //Italian
    if(Language == "it")
    {
        return "it";
    }
    //Japanese
    if(Language == "ja")
    {
        return "jp";
    }
    //Kanuri
    if(Language == "kr")
    {
        return "ne";
    }
    //Kazakh
    if(Language == "kk")
    {
        return "kz";
    }
    //Khmer
    if(Language == "km")
    {
        return "kh";
    }
    //Kinyarwanda
    if(Language == "rw")
    {
        return "rw";
    }
    //Kyrgyz
    if(Language == "ky")
    {
        return "kg";
    }
    //Kongo
    if(Language == "kg")
    {
        return "cd";
    }
    //Korean
    if(Language == "ko")
    {
        return "kp";
    }
    //Kurdish
    if(Language == "ku")
    {
        return "iq";
    }
    //Kwanyama,Kuanyama
    if(Language == "kj")
    {
        return "ao";
    }
    //Latin
    if(Language == "la")
    {
        return "va";
    }
    //Luxembourgish,Letzeburgesch
    if(Language == "lb")
    {
        return "lu";
    }
    //Lingala
    if(Language == "ln")
    {
        return "cd";
    }
    //Lao
    if(Language == "lo")
    {
        return "la";
    }
    //Lithuanian
    if(Language == "lt")
    {
        return "lt";
    }
    //Luba-Katanga
    if(Language == "lu")
    {
        return "cd";
    }
    //Latvian
    if(Language == "lv")
    {
        return "lv";
    }
    //Macedonian
    if(Language == "mk")
    {
        return "mk";
    }
    //Malagasy
    if(Language == "mg")
    {
        return "mg";
    }
    //Malay
    if(Language == "ms")
    {
        return "id";
    }
    //Maltese
    if(Language == "mt")
    {
        return "mt";
    }
    //Māori
    if(Language == "mi")
    {
        return "nz";
    }
    //Marshallese
    if(Language == "mh")
    {
        return "mh";
    }
    //Mongolian
    if(Language == "mn")
    {
        return "mn";
    }
    //Norwegian Bokmål
    if(Language == "nb")
    {
        return "no";
    }
    //Northern Ndebele
    if(Language == "nd")
    {
        return "za";
    }
    //Nepali
    if(Language == "ne")
    {
        return "np";
    }
    //Norwegian Nynorsk
    if(Language == "nn")
    {
        return "no";
    }
    //Norwegian
    if(Language == "no")
    {
        return "no";
    }
    //Southern Ndebele
    if(Language == "nr")
    {
        return "za";
    }
    //Panjabi,Punjabi
    if(Language == "pa")
    {
        return "in";
    }
    //Persian,Farsi
    if(Language == "fa")
    {
        return "ir";
    }
    //Polish
    if(Language == "pl")
    {
        return "pl";
    }
    //Pashto,Pushto
    if(Language == "ps")
    {
        return "af";
    }
    //Portuguese
    if(Language == "pt")
    {
        return "pt";
    }
    //Quechua
    if(Language == "qu")
    {
        return "pe";
    }
    //Romansh
    if(Language == "rm")
    {
        return "ch";
    }
    //Kirundi
    if(Language == "rn")
    {
        return "bi";
    }
    //Romanian
    if(Language == "ro")
    {
        return "ro";
    }
    //Russian
    if(Language == "ru")
    {
        return "ru";
    }
    //Sango
    if(Language == "sg")
    {
        return "cf";
    }
    //Serbian
    if(Language == "sr")
    {
        return "rs";
    }
    //Shona
    if(Language == "sn")
    {
        return "zw";
    }
    //Sinhala,Sinhalese
    if(Language == "si")
    {
        return "lk";
    }
    //Slovak
    if(Language == "sk")
    {
        return "sk";
    }
    //Slovene
    if(Language == "sl")
    {
        return "si";
    }
    //Somali
    if(Language == "so")
    {
        return "so";
    }
    //Southern Sotho
    if(Language == "st")
    {
        return "za";
    }
    //Spanish,Castilian
    if(Language == "es")
    {
        return "es";
    }
    //Swahili
    if(Language == "sw")
    {
        return "cd";
    }
    //Swati
    if(Language == "ss")
    {
        return "sz";
    }
    //Swedish
    if(Language == "sv")
    {
        return "se";
    }
    //Tamil
    if(Language == "ta")
    {
        return "in";
    }
    //Telugu
    if(Language == "te")
    {
        return "in";
    }
    //Tajik
    if(Language == "tg")
    {
        return "tj";
    }
    //Thai
    if(Language == "th")
    {
        return "th";
    }
    //Tigrinya
    if(Language == "ti")
    {
        return "er";
    }
    //Turkmen
    if(Language == "tk")
    {
        return "tm";
    }
    //Tagalog
    if(Language == "tl")
    {
        return "ph";
    }
    //Tswana
    if(Language == "tn")
    {
        return "bw";
    }
    //Turkish
    if(Language == "tr")
    {
        return "tr";
    }
    //Tsonga
    if(Language == "ts")
    {
        return "za";
    }
    //Ukrainian
    if(Language == "uk")
    {
        return "ua";
    }
    //Urdu
    if(Language == "ur")
    {
        return "pk";
    }
    //Uzbek
    if(Language == "uz")
    {
        return "uz";
    }
    //Venda
    if(Language == "ve")
    {
        return "za";
    }
    //Vietnamese
    if(Language == "vi")
    {
        return "vn";
    }
    //Welsh
    if(Language == "cy")
    {
        return "gb";
    }
    //Wolof
    if(Language == "wo")
    {
        return "sn";
    }
    //Xhosa
    if(Language == "xh")
    {
        return "za";
    }
    //Yoruba
    if(Language == "yo")
    {
        return "ng";
    }
    //Zulu
    if(Language == "zu")
    {
        return "za";
    }

    return Language;
}


AcceptLanguageCombineResult CombineAcceptLanguageWithPattern(const std::string & HeaderOriginal, const std::string & Pattern)
{
    std::string Header = HeaderOriginal;
    if(Header.empty())
        Header = "en-US";
    else
        Header = HeaderOriginal;
    AcceptLanguageCombineResult Result;
    ParsedAcceptLanguage ParsedPattern = ParseAcceptLanguageHeader(Pattern);
    ParsedAcceptLanguage ParsedHeader = ParseAcceptLanguageHeader(Header);
    bool HasCountries = ParsedPattern.HasCountries();
    //Header is empty - return empty.
    if(ParsedHeader.Items.size() == 0)
    {
        Result.NavigatorLanguage.clear();
        Result.NavigatorLanguages.clear();
        Result.Header.clear();
        return Result;
    }
    //Header is complex or no pattern - return original header
    if(ParsedHeader.Items.size() > 1 || ParsedPattern.Items.size() == 0)
    {
        Result.NavigatorLanguage = ParsedHeader.GetLanguage();
        Result.NavigatorLanguages = ParsedHeader.GetLanguages();
        Result.Header = Header;
        return Result;
    }
    //At this point header items list is not empty
    bool IsHeaderEn = !ParsedHeader.Items[0].IsLocalLanguage();
    ParsedAcceptLanguageItem HeaderItem = ParsedHeader.Items[0];

    //No local language - return original header
    if(ParsedPattern.IsNoLocalLanguage())
    {
        std::string HeaderCountry = HeaderItem.Country;
        if(IsHeaderEn && !HeaderCountry.empty() && HeaderCountry != "US")
        {
            for(ParsedAcceptLanguageItem& Item: ParsedPattern.Items)
            {
                if(!Item.Country.empty())
                {
                    Item.Country = HeaderCountry;
                }
            }
            Result.NavigatorLanguage = ParsedPattern.GetLanguage();
            Result.NavigatorLanguages = ParsedPattern.GetLanguages();
            Result.Header = ParsedPattern.Build();
            return Result;
        }else
        {
            Result.NavigatorLanguage = ParsedPattern.GetLanguage();
            Result.NavigatorLanguages = ParsedPattern.GetLanguages();
            Result.Header = Pattern;
            return Result;
        }
    }

    //One local language in pattern, replace it with value from header
    if(ParsedPattern.IsOneItemWithLocalLanguage() && !IsHeaderEn)
    {
        for(ParsedAcceptLanguageItem& Item: ParsedPattern.Items)
        {
            if(Item.IsLocalLanguage())
            {
                Item.Language = HeaderItem.Language;
                if(Item.Country.size() > 0)
                {
                    if(HeaderItem.Country.empty())
                    {
                        std::string NewValue = LanguageToCountry(HeaderItem.Language);
                        std::transform(NewValue.begin(), NewValue.end(), NewValue.begin(), ::toupper);
                        Item.Country = NewValue;
                    }
                    else
                        Item.Country = HeaderItem.Country;
                }
            }
        }

        Result.NavigatorLanguage = ParsedPattern.GetLanguage();
        Result.NavigatorLanguages = ParsedPattern.GetLanguages();
        Result.Header = ParsedPattern.Build();
        return Result;
    }


    //Two local languages in pattern, replace it with values from header(with and without country)
    if(ParsedPattern.IsTwoItemsWithLocalLanguage() && !IsHeaderEn && HasCountries)
    {
        bool WasOnlyLanguage = false;
        bool WasCountryAndLanguage = false;
        for(ParsedAcceptLanguageItem& Item: ParsedPattern.Items)
        {
            if(Item.IsLocalLanguage())
            {
                Item.Language = HeaderItem.Language;


                if(Item.Country.empty() && !WasOnlyLanguage || (!Item.Country.empty() && !WasOnlyLanguage && WasCountryAndLanguage))
                {
                    WasOnlyLanguage = true;
                    Item.Country.clear();
                }else if(!Item.Country.empty() && !WasCountryAndLanguage || (Item.Country.empty() && WasOnlyLanguage && !WasCountryAndLanguage))
                {
                    WasCountryAndLanguage = true;
                    if(HeaderItem.Country.empty())
                    {
                        std::string NewValue = LanguageToCountry(HeaderItem.Language);
                        std::transform(NewValue.begin(), NewValue.end(), NewValue.begin(), ::toupper);
                        Item.Country = NewValue;
                    }else
                    {
                        Item.Country = HeaderItem.Country;
                    }
                }
            }
        }

        Result.NavigatorLanguage = ParsedPattern.GetLanguage();
        Result.NavigatorLanguages = ParsedPattern.GetLanguages();
        Result.Header = ParsedPattern.Build();
        return Result;
    }

    //There are several local languages and maybe english or *, need to remove all locals.
    if(IsHeaderEn)
    {
        AcceptLanguageReplacePattern ReplacePattern = ParsedPattern.GetReplacePattern();
        std::vector<ParsedAcceptLanguageItem> NewItems;
        std::string HeaderCountry = HeaderItem.Country;
        for(ParsedAcceptLanguageItem& Item: ParsedPattern.Items)
        {
            if(!Item.IsLocalLanguage())
            {
                if(!HeaderCountry.empty() && !Item.Country.empty())
                {
                    Item.Country = HeaderCountry;
                }
                NewItems.push_back(Item);
            }
        }
        if(NewItems.size() == 0)
        {
            HeaderItem.FactorString.clear();

            if(HasCountries)
            {
                if(HeaderItem.Country.empty())
                {
                    std::string NewValue = LanguageToCountry(HeaderItem.Language);
                    std::transform(NewValue.begin(), NewValue.end(), NewValue.begin(), ::toupper);
                    HeaderItem.Country = NewValue;
                }
            }else
            {
                HeaderItem.Country.clear();
            }

            NewItems.push_back(HeaderItem);
        }

        ParsedPattern.Items = NewItems;

        ParsedPattern.ReplaceFactors(ReplacePattern);

        Result.NavigatorLanguage = ParsedPattern.GetLanguage();
        Result.NavigatorLanguages = ParsedPattern.GetLanguages();
        Result.Header = ParsedPattern.Build();
        return Result;
    }


    //There are several local languages and maybe english or *, need replace first local with header.
    AcceptLanguageReplacePattern ReplacePattern = ParsedPattern.GetReplacePattern();
    bool WasOnlyLanguage = false;
    bool WasCountryAndLanguage = false;
    std::vector<ParsedAcceptLanguageItem> NewItems;
    for(ParsedAcceptLanguageItem& Item: ParsedPattern.Items)
    {
        if(Item.IsLocalLanguage())
        {
            if(Item.Country.empty() && !WasOnlyLanguage || (!Item.Country.empty() && !WasOnlyLanguage && WasCountryAndLanguage))
            {
                WasOnlyLanguage = true;
                Item.Language = HeaderItem.Language;
                Item.Country.clear();
                NewItems.push_back(Item);

            }else if((!Item.Country.empty() && !WasCountryAndLanguage || (Item.Country.empty() && WasOnlyLanguage && !WasCountryAndLanguage)) && HasCountries)
            {
                WasCountryAndLanguage = true;
                Item.Language = HeaderItem.Language;
                if(HeaderItem.Country.empty())
                {
                    std::string NewValue = LanguageToCountry(HeaderItem.Language);
                    std::transform(NewValue.begin(), NewValue.end(), NewValue.begin(), ::toupper);
                    Item.Country = NewValue;
                }else
                {
                    Item.Country = HeaderItem.Country;
                }
                NewItems.push_back(Item);

            }
        }else
        {
            //Keep all english items
            NewItems.push_back(Item);
        }
    }
    if(NewItems.size() == 0)
    {
        HeaderItem.FactorString.clear();
        NewItems.push_back(HeaderItem);
    }

    ParsedPattern.Items = NewItems;
    ParsedPattern.ReplaceFactors(ReplacePattern);

    Result.NavigatorLanguage = ParsedPattern.GetLanguage();
    Result.NavigatorLanguages = ParsedPattern.GetLanguages();
    Result.Header = ParsedPattern.Build();
    return Result;

    //Result.Header = "???";


    /*AcceptLanguageCombineResult Result;
    Result.Header =
            Pattern;

    for(ParsedAcceptLanguageItem& Item: ParsedPattern.Items)
    {
        Result.Header += std::string("\n") + Item.Raw + std::string(" [Country = ") + (Item.Country) +
                std::string(", Language = ") + (Item.Language) +
                std::string(", FactorString = ") + (Item.FactorString) +
                std::string(", FactorFloat = ") + std::to_string(Item.GetFactor())+ "]";
    }
    Result.Header += std::string("\n") + std::string("HasSpaces = ") + ((ParsedPattern.HasSpaces())?"true":"false")
            + std::string("\n") + std::string("IsNoLocalLanguage = ") + ((ParsedPattern.IsNoLocalLanguage())?"true":"false")
            + std::string("\n") + std::string("IsOneItemWithLocalLanguage = ") + ((ParsedPattern.IsOneItemWithLocalLanguage())?"true":"false")
            + std::string("\n") + std::string("IsTwoItemsWithLocalLanguage = ") + ((ParsedPattern.IsTwoItemsWithLocalLanguage())?"true":"false")
            + std::string("\n") + std::string("IsFactorsSame = ") + ((ParsedPattern.IsFactorsSame())?"true":"false")
            + std::string("\n") + std::string("IsFactorsSameExceptLast = ") + ((ParsedPattern.IsFactorsSameExceptLast())?"true":"false")
            + std::string("\n") + std::string("IsFactorsSameExceptLastAndFirst = ") + ((ParsedPattern.IsFactorsSameExceptLastAndFirst())?"true":"false")
            + std::string("\n") + std::string("IsFactorsArithmeticProgression = ") + ((ParsedPattern.IsFactorsArithmeticProgression() >= 0.0)?"true, " + std::to_string(ParsedPattern.IsFactorsArithmeticProgression()):"false")
            + std::string("\n") + std::string("IsFactorsArithmeticProgressionExceptLast = ") + ((ParsedPattern.IsFactorsArithmeticProgressionExceptLast() >= 0.0)?"true, " + std::to_string(ParsedPattern.IsFactorsArithmeticProgressionExceptLast()):"false")
            + std::string("\n") + std::string("IsFactorsArithmeticProgressionExceptLastAndFirst = ") + ((ParsedPattern.IsFactorsArithmeticProgressionExceptLastAndFirst() >= 0.0)?"true, " + std::to_string(ParsedPattern.IsFactorsArithmeticProgressionExceptLastAndFirst()):"false")

            + std::string("\n\n");*/

    //return Result;
}
