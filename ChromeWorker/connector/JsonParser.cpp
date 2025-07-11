#include "JsonParser.h"
#include <sstream>

void JsonParser::GetValueFromJson(const std::string& Json, const std::string& Path, picojson::value& Value, bool& Found)
{
    Found = false;

    std::vector<std::string> Items;
    std::stringstream ss(Path);
    std::string item;
    while (std::getline(ss, item, '.'))
    {
        Items.push_back(item);
    }

    if (Items.size() == 0)
        return;

    picojson::value CurrentValue;
    try
    {
        picojson::parse(CurrentValue, Json);
    }
    catch (...)
    {
        return;
    }


    while (!Items.empty())
    {
        std::string Item = Items.front();
        Items.erase(Items.begin(), Items.begin() + 1);
        if (CurrentValue.is<picojson::value::object>())
        {
            picojson::value::object CurrentObject = CurrentValue.get<picojson::value::object>();
            if (CurrentObject.count(Item) > 0)
            {
                CurrentValue = CurrentObject[Item];
            }
            else
                return;
        }
        else
            return;
    }

    Value = CurrentValue;
    Found = true;

}

std::string JsonParser::GetStringFromJson(const std::string& Json, const std::string& Path, const std::string& Default)
{
    picojson::value Result;
    bool ResultFound = false;
    GetValueFromJson(Json, Path, Result, ResultFound);
    if (!ResultFound)
        return Default;
    if (!Result.is<std::string>())
        return Default;
    return Result.get<std::string>();
}

bool JsonParser::GetBooleanFromJson(const std::string& Json, const std::string& Path, bool Default)
{
    picojson::value Result;
    bool ResultFound = false;
    GetValueFromJson(Json, Path, Result, ResultFound);
    if (!ResultFound)
        return Default;
    if (!Result.is<bool>())
        return Default;
    return Result.get<bool>();
}

double JsonParser::GetFloatFromJson(const std::string& Json, const std::string& Path, float Default)
{
    picojson::value Result;
    bool ResultFound = false;
    GetValueFromJson(Json, Path, Result, ResultFound);
    if (!ResultFound)
        return Default;
    if (!Result.is<double>())
        return Default;
    return Result.get<double>();
}
