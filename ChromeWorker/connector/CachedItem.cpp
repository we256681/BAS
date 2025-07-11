#include "CachedItem.h"
#include <chrono>

using namespace std::chrono;

CachedItem::CachedItem()
{
    CreatedAt = duration_cast< milliseconds >( system_clock::now().time_since_epoch() ).count();
}

void CachedItem::UpdateResponseHeaders(const std::string& Result, const std::string& ObjectName, const std::string& PropertyName)
{
    picojson::value AllValue;
    picojson::parse(AllValue,Result);
    picojson::object AllObject = AllValue.get<picojson::object>();

    if(AllObject.count(ObjectName) && AllObject[ObjectName].is<picojson::object>())
    {
        picojson::object MainObject = AllObject[ObjectName].get<picojson::object>();

        if(MainObject.count(PropertyName) && MainObject[PropertyName].is<picojson::object>())
        {
            picojson::object HeadersObject = MainObject[PropertyName].get<picojson::object>();
            for(const auto& Header : HeadersObject)
            {
                this->ResponseHeaders[Header.first] = Header.second.get<std::string>();
            }
        }
    }
}

void CachedItem::UpdateRequestHeaders(const std::string& Result, const std::string& ObjectName, const std::string& PropertyName)
{
    picojson::value AllValue;
    picojson::parse(AllValue,Result);
    picojson::object AllObject = AllValue.get<picojson::object>();

    if(AllObject.count(ObjectName) && AllObject[ObjectName].is<picojson::object>())
    {
        picojson::object MainObject = AllObject[ObjectName].get<picojson::object>();

        if(MainObject.count(PropertyName) && MainObject[PropertyName].is<picojson::object>())
        {
            picojson::object HeadersObject = MainObject[PropertyName].get<picojson::object>();
            for(const auto& Header : HeadersObject)
            {
                this->RequestHeaders[Header.first] = Header.second.get<std::string>();
            }
        }
    }

}

picojson::value CachedItem::Serialize() 
{
    picojson::object Item;

    picojson::array ResponseHeaders;
    for (const auto& header : this->ResponseHeaders)
    {
        picojson::array HeaderArray = { picojson::value(header.first), picojson::value(header.second) };
        ResponseHeaders.push_back(picojson::value(HeaderArray));
    }
    Item["response_headers"] = picojson::value(ResponseHeaders);

    picojson::array RequestHeaders;
    for (const auto& header : this->RequestHeaders)
    {
        picojson::array HeaderArray = { picojson::value(header.first), picojson::value(header.second) };
        RequestHeaders.push_back(picojson::value(HeaderArray));
    }
    Item["request_headers"] = picojson::value(RequestHeaders);

    Item["is_finished"] = picojson::value((double)this->IsFinished);
    Item["is_error"] = picojson::value((double)this->IsError);
    Item["status"] = picojson::value((double)this->Status);
    Item["post_data"] = picojson::value(this->PostData);
    Item["error"] = picojson::value(this->Error);
    Item["body"] = picojson::value(this->Body);
    Item["url"] = picojson::value(this->Url);
    
    return picojson::value(Item);
}
