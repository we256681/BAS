#include "JsonSerializer.h"
#include <sstream>

std::string JsonSerializer::SerializeObjectToString(const std::map<std::string, Variant>& Params)
{
    picojson::object ParamsObject = SerializeObjectToObject(Params);
    return picojson::value(ParamsObject).serialize();
}

picojson::object JsonSerializer::SerializeObjectToObject(const std::map<std::string, Variant>& Params)
{
    picojson::value::object ParamsObject;

    for(const std::pair<std::string, Variant>& Param : Params)
    {
        std::vector<std::string> Items;
        std::stringstream ss(Param.first);
        std::string item;
        while(std::getline(ss, item, '.'))
        {
            Items.push_back(item);
        }

        picojson::value::object* Target = &ParamsObject;

        while(Items.size() > 1)
        {
            std::string Item = Items.front();
            Items.erase(Items.begin(), Items.begin() + 1);
            if(Target->count(Item) == 0)
            {
                picojson::value::object NewItem;
                Target->operator[](Item) = picojson::value(NewItem);
            }
            bool IsObject = Target->operator[](Item).is<picojson::value::object>();
            if(IsObject)
            {

                Target = &(Target->operator[](Item).get<picojson::value::object>());
            }
        }

        if(Items.size() == 1)
        {
            Target->operator[](Items.front()) = SerializeVariant(Param.second);
        }
    }
    return ParamsObject;
}

picojson::value JsonSerializer::SerializeVariant(const Variant& Target)
{
    if(Target.IsBoolean)
        return picojson::value(Target.Boolean);
    else if(Target.IsFloat)
        return picojson::value(Target.Float);
    else if(Target.IsNumber)
        return picojson::value((double)Target.Number);
    else if(Target.IsString)
        return picojson::value(Target.String);
    else if(Target.IsList)
    {
        picojson::array arr;

        for(const auto& i : Target.List)
        {
            arr.push_back(picojson::value(SerializeVariant(i)));
        }

        return picojson::value(arr);
    } else if(Target.IsMap)
    {
        picojson::value::object map;

        for(const auto& i : Target.Map)
        {
            map[i.first] = picojson::value(SerializeVariant(i.second));
        }

        return picojson::value(map);
    }
}
