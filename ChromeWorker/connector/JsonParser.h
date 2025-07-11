#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "json/picojson.h"
#include <string>

class JsonParser
{
    public:
        void GetValueFromJson(const std::string& Json, const std::string& Path, picojson::value& Value, bool& Found);
        std::string GetStringFromJson(const std::string& Json, const std::string& Path, const std::string& Default = std::string());
        bool GetBooleanFromJson(const std::string& Json, const std::string& Path, bool Default = false);
        double GetFloatFromJson(const std::string& Json, const std::string& Path, float Default = 0.0);
};

#endif // JSONPARSER_H