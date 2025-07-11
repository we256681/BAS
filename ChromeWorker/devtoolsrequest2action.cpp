#include "devtoolsrequest2action.h"
#include "readallfile.h"
#include "picojson.h"
#include "base64.h"
#include "checkvalidutf8.h"
#include "JsonParser.h"
#include "startwith.h"
#include <chrono>

using namespace std::chrono;


std::string DevToolsReqest2Action::JsonEscape(const std::string& Text)
{
    return picojson::value(Text).serialize();
}

std::string DevToolsReqest2Action::JsonEscapeInsideString(const std::string & Text)
{
    std::string res = JsonEscape(Text);
    if(!res.empty())
    {
        res.erase(0,1);
    }
    if(!res.empty())
    {
        res.erase(res.length() - 1,1);
    }
    return res;
}

long long DevToolsReqest2Action::Now()
{
    return duration_cast< milliseconds >( system_clock::now().time_since_epoch() ).count();
}

void DevToolsReqest2Action::Process(std::shared_ptr<RequestItem> CurrentItem)
{
    if(!CurrentItem)
        return;

    bool NeedToRemove = false;

    //Deside if we are ready to use this item
    if(CurrentItem->IsFinished || (CurrentItem->IsAdditional && CurrentItem->IsMain) || (Now() - CurrentItem->StartedAt > 10000))
    {
        NeedToRemove = true;
    }

    //Item must be removed, but not sure that event must be generated
    if(NeedToRemove)
    {
        //WORKER_LOG("!!!!!!!!!!!!!! Remove " + CurrentItem->Id);
        RemoveRequest(CurrentItem->Id);
    }

    //Item is not ready yet, waiting for additional data
    if(!NeedToRemove)
    {
        return;
    }

    //Item is ready, but there is no main data, skip
    if(!CurrentItem->IsMain)
    {
        return;
    }


    //WORKER_LOG("!!!!!!!!!!!!!! Generate event " + CurrentItem->Id);


    //Generate event

    std::string Res;

    // Collect headers
    std::string HeadersRaw;
    for (std::map<std::string, std::string>::iterator it=CurrentItem->HeaderMap.begin();it!=CurrentItem->HeaderMap.end();it++)
    {
        std::string key = it->first;
        std::string value = it->second;

        std::string KeyLow = key;
        try{std::transform(KeyLow.begin(), KeyLow.end(), KeyLow.begin(), ::tolower);}catch(...){}


        if(KeyLow == std::string("accept-encoding"))
        {
            value = "gzip, deflate";
        }
        if(KeyLow == std::string("connection"))
        {
            continue;
        }
        if(KeyLow == std::string("host"))
        {
            continue;
        }
        if(KeyLow == std::string("content-type"))
        {
            continue;
        }
        if(KeyLow == std::string("content-length"))
        {
            continue;
        }
        if(starts_with(KeyLow, ":"))
        {
            continue;
        }



        if(!HeadersRaw.empty())
            HeadersRaw += std::string("\n");
        HeadersRaw += key;
        HeadersRaw += std::string(": ");
        HeadersRaw += value;

    }




    if(CurrentItem->IsPost)
    {
        // Adding post action
        std::string PostDataString(CurrentItem->PostRawData.data(),CurrentItem->PostRawData.size());
        if(!utf8_check_is_valid(PostDataString))
        {
            PostDataString = "BINARY DATA";
        }

        std::string code = std::string("\n_switch_http_client_main()\n") +
        std::string("http_client_post_no_redirect(") + JsonEscape(CurrentItem->Url) + std::string(", [\"data\", ") + JsonEscape(PostDataString)
                + std::string("],{method:(") + JsonEscape(CurrentItem->Method)
                + std::string("), encoding:\"UTF-8\", \"content-type\":(") + JsonEscape(std::string("custom/") + CurrentItem->ContentType)
                + std::string("), headers:(") + JsonEscape(HeadersRaw)
                + std::string(")})!");
        std::string json =
                std::string("{\"s\":\"httpclientpost\",\"v\":1,\"f\":[],\"uw\":\"0\",\"ut\":\"0\""
                            ",\"uto\":\"0\",\"um\":\"0\",\"d\":[{\"id\":\"Value\",\"type\":\"constr\""
                            ",\"data\":") + JsonEscape(CurrentItem->Url) + std::string(",\"class\":\"string\"},"
                            "{\"id\":\"Check2\",\"type\":\"check\",\"data\":false},{\"id\":\"PostName\""
                            ",\"type\":\"constr\",\"data\":\"\",\"class\":\"string\"},{\"id\":\"PostValue\""
                            ",\"type\":\"constr\",\"data\":\"\",\"class\":\"string\"},{\"id\":\"PostDataArray\""
                            ",\"type\":\"constr\",\"data\":\"\",\"class\":\"expression\"},{\"id\":\"ContentType\""
                            ",\"type\":\"constr\",\"data\":\"urlencode\",\"class\":\"string\"}"
                            ",{\"id\":\"PostDataRaw\",\"type\":\"constr\",\"data\":") + JsonEscape(PostDataString)
                            + std::string(",\"class\":\"string\"},{\"id\":\"ContentTypeRaw\",\"type\""
                            ":\"constr\",\"data\":") + JsonEscape(CurrentItem->ContentType) + std::string(",\"class\":\"string\"}"
                            ",{\"id\":\"Encoding\",\"type\":\"constr\",\"data\":\"UTF-8\",\"class\":\"string\"}"
                            ",{\"id\":\"Method\",\"type\":\"constr\",\"data\":") + JsonEscape(CurrentItem->Method)
                            + std::string(",\"class\":\"string\"},{\"id\":\"Check\",\"type\":\"check\",\"data\":false},"
                            "{\"id\":\"Headers\",\"type\":\"constr\",\"data\":") + JsonEscape(HeadersRaw)   + std::string(",\"class\":\"string\"}]}");


        std::string base64_json = base64_encode((unsigned char const *)json.data(),json.size());

        std::string task = std::string("BrowserAutomationStudio_AddTask(\"\",\"\\/*Dat:") + base64_json + std::string("*\\/") + JsonEscapeInsideString(code) + std::string("\",0, true);");

        Res+=std::string(";");
        Res+=task;
    }else
    {
        // Adding get action
        std::string code = std::string("\n_switch_http_client_main()\n") +
        std::string("http_client_get_no_redirect2(") + JsonEscape(CurrentItem->Url) + std::string(",{method:(") + JsonEscape(CurrentItem->Method) + std::string("), headers:(") + JsonEscape(HeadersRaw) + std::string(")})!");
        std::string json =
                std::string("{\"s\":\"httpclientget\",\"v\":1,\"f\":[],\"uw\":\"0\",\"ut\":\"0\",\"uto\":\"0\",\"um\":\"0\",\"d\":[{\"id\":\"Value\",\"type\":\"constr\",\"data\":")
                + JsonEscape(CurrentItem->Url) + std::string(",\"class\":\"string\"},{\"id\":\"Method\",\"type\":\"constr\",\"data\":") + JsonEscape(CurrentItem->Method) + std::string(",\"class\":\"string\"},{\"id\":\"Check\",\"type\":\"check\",\"data\":false},"
                "{\"id\":\"Headers\",\"type\":\"constr\",\"data\":") + JsonEscape(HeadersRaw)   + std::string(",\"class\":\"string\"}]}");


        std::string base64_json = base64_encode((unsigned char const *)json.data(),json.size());

        std::string task = std::string("BrowserAutomationStudio_AddTask(\"\",\"\\/*Dat:") + base64_json + std::string("*\\/") + JsonEscapeInsideString(code) + std::string("\",0,true);");

        Res+=std::string(";");
        Res+=task;
    }

    if(!Res.empty())
    {
        for(auto f:OnDataReady)
            f(Res);
    }
}

std::shared_ptr<DevToolsReqest2Action::RequestItem> DevToolsReqest2Action::FindRequestOrCreateNew(const std::string& RequestId)
{
    std::shared_ptr<RequestItem> CurrentItem;
    for(std::shared_ptr<RequestItem> Item: Items)
    {
        if(Item->Id == RequestId)
        {
            CurrentItem = Item;
            break;
        }
    }

    if(!CurrentItem)
    {
        CurrentItem = std::make_shared<RequestItem>();
        Items.push_back(CurrentItem);
        //WORKER_LOG("!!!!!!!!!!!!!! +++++ " + std::to_string(Items.size()));
    }

    CurrentItem->Id = RequestId;
    CurrentItem->StartedAt = Now();

    return CurrentItem;
}

void DevToolsReqest2Action::RemoveRequest(const std::string& RequestId)
{
    for(std::vector<std::shared_ptr<RequestItem>>::iterator it = Items.begin() ; it != Items.end(); )
    {
        std::shared_ptr<RequestItem> Item = *it;
        if(Item->Id == RequestId)
        {
            it = Items.erase(it);
        }else
        {
            ++it;
        }
    }

    //WORKER_LOG("!!!!!!!!!!!!!! ----- " + std::to_string(Items.size()));
}

std::shared_ptr<DevToolsReqest2Action::RequestItem> DevToolsReqest2Action::FindRequestOrReturnNull(const std::string& RequestId)
{
    std::shared_ptr<RequestItem> CurrentItem;
    for(std::shared_ptr<RequestItem> Item: Items)
    {
        if(Item->Id == RequestId)
        {
            CurrentItem = Item;
            break;
        }
    }

    return CurrentItem;
}

void DevToolsReqest2Action::RemoveKeyIgnoreCase(std::map<std::string, std::string>& Headers, const std::string& Key)
{
    std::string KeyOriginal = Key;
    try{std::transform(KeyOriginal.begin(), KeyOriginal.end(), KeyOriginal.begin(), ::tolower);}catch(...){}

    for (std::map<std::string, std::string>::iterator it=Headers.begin();it!=Headers.end();)
    {
        std::string KeyCurrent = it->first;
        try{std::transform(KeyCurrent.begin(), KeyCurrent.end(), KeyCurrent.begin(), ::tolower);}catch(...){}

        if(KeyCurrent == KeyOriginal)
            it = Headers.erase(it);
        else
        {
            ++it;
        }

    }
}

void DevToolsReqest2Action::ConvertStop(const std::string& RequestId)
{
    //WORKER_LOG("!!!!!!!!!!!!!! Stop request for " + RequestId);

    std::shared_ptr<RequestItem> CurrentItem = FindRequestOrReturnNull(RequestId);
    if(!CurrentItem)
    {
        return;
    }

    CurrentItem->IsFinished = true;
    Process(CurrentItem);
}


void DevToolsReqest2Action::ConvertAdditional(const std::string& RequestString)
{
    JsonParser Parser;

    //Find request or create new

    std::string RequestId = Parser.GetStringFromJson(RequestString, "requestId");

    //WORKER_LOG("!!!!!!!!!!!!!! Additional data for " + RequestId);

    std::shared_ptr<RequestItem> CurrentItem = FindRequestOrCreateNew(RequestId);

    CurrentItem->IsAdditional = true;

    //Get headers map
    {
        picojson::value AllValue;
        bool ParsedCorrectly = true;
        try
        {
            picojson::parse(AllValue, RequestString);
        }
        catch (...)
        {
            ParsedCorrectly = false;
        }
        if(ParsedCorrectly && AllValue.is<picojson::object>())
        {
            picojson::object AllObject = AllValue.get<picojson::object>();

            if(AllObject.count("headers") > 0 && AllObject["headers"].is<picojson::object>())
            {
                picojson::object HeadersObject = AllObject["headers"].get<picojson::object>();

                for(std::pair<std::string, picojson::value> Header: HeadersObject)
                {
                    if(Header.second.is<std::string>())
                    {
                        std::pair<std::string, std::string> HeaderItem;
                        HeaderItem.first = Header.first;
                        HeaderItem.second = Header.second.get<std::string>();
                        RemoveKeyIgnoreCase(CurrentItem->HeaderMap,HeaderItem.first);

                        CurrentItem->HeaderMap.insert(CurrentItem->HeaderMap.end(), HeaderItem);
                    }
                }
            }

        }
    }

    Process(CurrentItem);
}

void DevToolsReqest2Action::Timer()
{
    for(std::shared_ptr<RequestItem> Item: Items)
    {
        Process(Item);
    }
}

void DevToolsReqest2Action::ConvertMain(const std::string& RequestString)
{
    JsonParser Parser;

    //Find request or create new

    std::string RequestId = Parser.GetStringFromJson(RequestString, "requestId");

    //WORKER_LOG("!!!!!!!!!!!!!! Main data for " + RequestId);
    std::shared_ptr<RequestItem> CurrentItem = FindRequestOrCreateNew(RequestId);

    CurrentItem->IsMain = true;

    CurrentItem->Method = Parser.GetStringFromJson(RequestString, "request.method");
    try{std::transform(CurrentItem->Method.begin(), CurrentItem->Method.end(), CurrentItem->Method.begin(), ::toupper);}catch(...){}

    CurrentItem->Url = Parser.GetStringFromJson(RequestString, "request.url");
    CurrentItem->IsPost = false;

    CurrentItem->PostRawData.clear();

    if(CurrentItem->Method == "OPTIONS")
        return;


    //Get headers map
    {
        picojson::value AllValue;
        bool ParsedCorrectly = true;
        try
        {
            picojson::parse(AllValue, RequestString);
        }
        catch (...)
        {
            ParsedCorrectly = false;
        }
        if(ParsedCorrectly && AllValue.is<picojson::object>())
        {
            picojson::object AllObject = AllValue.get<picojson::object>();
            if(AllObject.count("request") > 0 && AllObject["request"].is<picojson::object>())
            {
                picojson::object RequestObject = AllObject["request"].get<picojson::object>();

                if(RequestObject.count("headers") > 0 && RequestObject["headers"].is<picojson::object>())
                {
                    picojson::object HeadersObject = RequestObject["headers"].get<picojson::object>();

                    for(std::pair<std::string, picojson::value> Header: HeadersObject)
                    {
                        if(Header.second.is<std::string>())
                        {
                            std::pair<std::string, std::string> HeaderItem;
                            HeaderItem.first = Header.first;
                            HeaderItem.second = Header.second.get<std::string>();
                            RemoveKeyIgnoreCase(CurrentItem->HeaderMap,HeaderItem.first);

                            CurrentItem->HeaderMap.insert(CurrentItem->HeaderMap.end(), HeaderItem);
                        }
                    }
                }
            }
        }
    }



    //Parse request.postDataEntries entries
    std::vector<std::string> PostDataItems;
    if(Parser.GetBooleanFromJson(RequestString, "request.hasPostData", false))
    {
        picojson::value AllValue;
        bool ParsedCorrectly = true;
        try
        {
            picojson::parse(AllValue, RequestString);
        }
        catch (...)
        {
            ParsedCorrectly = false;
        }
        if(ParsedCorrectly && AllValue.is<picojson::object>())
        {
            picojson::object AllObject = AllValue.get<picojson::object>();
            if(AllObject.count("request") > 0 && AllObject["request"].is<picojson::object>())
            {
                picojson::object RequestObject = AllObject["request"].get<picojson::object>();

                if(RequestObject.count("postDataEntries") > 0 && RequestObject["postDataEntries"].is<picojson::array>())
                {
                    picojson::array PostDataArray = RequestObject["postDataEntries"].get<picojson::array>();

                    for(picojson::value& PostDataValue: PostDataArray)
                    {
                        if(PostDataValue.is<picojson::object>())
                        {
                            picojson::object PostDataObject = PostDataValue.get<picojson::object>();

                            if(PostDataObject.count("bytes") > 0 && PostDataObject["bytes"].is<std::string>())
                            {
                                std::string BytesString = PostDataObject["bytes"].get<std::string>();
                                PostDataItems.push_back(BytesString);
                            }
                        }
                    }
                }
            }
        }
    }

    //Concat all POST data to PostRawData
    for(std::string& DataBase64: PostDataItems)
    {
        std::string RawPostData = base64_decode(DataBase64);
        CurrentItem->PostRawData.insert(CurrentItem->PostRawData.end(), RawPostData.begin(), RawPostData.end());
    }

    CurrentItem->IsPost = !CurrentItem->PostRawData.empty();

    // Get content type for POST requests
    if(CurrentItem->IsPost)
    {
    for (std::map<std::string, std::string>::iterator it=CurrentItem->HeaderMap.begin();it!=CurrentItem->HeaderMap.end();it++)
    {
        std::string header = it->first;
        try{std::transform(header.begin(), header.end(), header.begin(), ::tolower);}catch(...){}
        if(header == "content-type")
        {
            CurrentItem->ContentType = it->second;
            break;
        }
    }
    }

    Process(CurrentItem);
}

void DevToolsReqest2Action::Reset()
{
    Items.clear();
}
