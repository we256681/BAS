#ifndef VARIANT_H
#define VARIANT_H

#include <string>
#include <vector>
#include <map>

struct Variant
{
    Variant(){}
    Variant(int Number){this->Number = Number;IsNumber = true;}
    Variant(double Float){this->Float = Float;IsFloat = true;}
    Variant(bool Boolean){this->Boolean = Boolean;IsBoolean = true;}
    Variant(const std::string& String){this->String = String;IsString = true;}
    Variant(const std::vector<Variant>& List){this->List = List;IsList = true;}
    Variant(const std::map<std::string, Variant>& Map){this->Map = Map;IsMap = true;}
    int Number = 0;
    bool IsNumber = false;
    double Float = 0.0;
    bool IsFloat = false;
    bool Boolean = false;
    bool IsBoolean = false;
    std::string String;
    bool IsString = false;
    std::vector<Variant> List;
    bool IsList = false;
    std::map<std::string, Variant> Map;
    bool IsMap = false;
};



#endif // VARIANT_H
