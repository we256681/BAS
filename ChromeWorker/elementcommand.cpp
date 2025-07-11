#include "elementcommand.h"
#include "picojson.h"


std::string ElementCommand::SerializePathRaw()
{
    picojson::array res;
    for(std::pair<std::string,std::string> p:Path)
    {
        res.push_back(picojson::value(p.first));
        res.push_back(picojson::value(p.second));
    }

    return picojson::value(res).serialize();
}


std::string ElementCommand::SerializePath()
{
    return SerializePathRaw();
}

