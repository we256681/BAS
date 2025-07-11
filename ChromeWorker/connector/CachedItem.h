#ifndef CACHEDITEM_H
#define CACHEDITEM_H

#include "json/picojson.h"
#include <string>
#include <map>

struct CachedItem
{
    CachedItem();
    std::map<std::string, std::string> ResponseHeaders;
    std::map<std::string, std::string> RequestHeaders;
    bool IsFinished = false;
    bool IsError = false;
    std::string PostData;
    std::string Error;
    std::string Body;
    std::string Url;
    std::string TabId;
    int Status = 0;
    long long CreatedAt = 0;

    void UpdateResponseHeaders(const std::string& Result, const std::string& ObjectName, const std::string& PropertyName);
    void UpdateRequestHeaders(const std::string& Result, const std::string& ObjectName, const std::string& PropertyName);
    picojson::value Serialize();
};

#endif // CACHEDITEM_H
