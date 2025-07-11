#ifndef DEVTOOLSREQUEST2ACTION_H
#define DEVTOOLSREQUEST2ACTION_H

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <functional>

class DevToolsReqest2Action
{
    //std::map<std::string, std::string> Headers;
    std::string JsonEscape(const std::string & Text);
    std::string JsonEscapeInsideString(const std::string & Text);
    struct RequestItem
    {
        //State
        bool IsMain = false;
        bool IsAdditional = false;
        bool IsFinished = false;

        //Data
        bool IsPost = false;
        long long StartedAt = 0;
        std::string Method;
        std::string Url;
        std::string ContentType;
        std::vector<char> PostRawData;
        std::map<std::string, std::string> HeaderMap;
        std::string Id;
    };

    void RemoveKeyIgnoreCase(std::map<std::string, std::string>& Headers, const std::string& Key);

    std::vector<std::shared_ptr<RequestItem>> Items;

    std::shared_ptr<RequestItem> FindRequestOrCreateNew(const std::string& RequestId);
    std::shared_ptr<RequestItem> FindRequestOrReturnNull(const std::string& RequestId);
    void RemoveRequest(const std::string& RequestId);

    long long Now();
    void Process(std::shared_ptr<RequestItem> CurrentItem);
public:

    std::vector<std::function<void(std::string)> > OnDataReady;

    //Request started
    void ConvertMain(const std::string& RequestString);

    //Request additional info
    void ConvertAdditional(const std::string& RequestString);

    //Request stopped
    void ConvertStop(const std::string& RequestId);

    //Finish hanging requests if needed
    void Timer();

    //Clear all collected data
    void Reset();

};

#endif // DEVTOOLSREQUEST2ACTION_H
