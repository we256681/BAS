#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include "Variant.h"
#include "json/picojson.h"

class JsonSerializer
{
public:
    std::string SerializeObjectToString(const std::map<std::string, Variant>& Params);
    picojson::object SerializeObjectToObject(const std::map<std::string, Variant>& Params);
private:
    picojson::value SerializeVariant(const Variant& Target);
    

};

#endif // JSONSERIALIZER_H