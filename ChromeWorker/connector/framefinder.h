#ifndef FRAMEFINDER_H
#define FRAMEFINDER_H

#include <string>
#include <vector>

class FrameFinder
{
    void Cleanup();
    long long Now();

    struct DataItem
    {
        long long DateCreated = 0;
        bool HasSessionId = false;
        std::string SessionId;
        std::string ActionId;
    };

    std::vector<DataItem> Data;


public:

    struct StatusInfo
    {
        enum StatusEnum
        {
            Success,
            Wait,
            DoRequest,
            IgnoreEvent,
            CloseHangingTouch,
        }Status;

        std::string ActionId;

        std::string SessionId;
    };

    StatusInfo GetSessionId(const std::string& ActionId, const std::string& Type);
    void SetSessionId(const std::string& ActionId, const std::string& SessionId);

    std::string GetReport();

};

#endif // FRAMEFINDER_H
