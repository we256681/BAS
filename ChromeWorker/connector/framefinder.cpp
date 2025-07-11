#include "framefinder.h"
#include <chrono>
#include "log.h"

using namespace std::chrono;

long long FrameFinder::Now()
{
    return duration_cast< milliseconds >( system_clock::now().time_since_epoch() ).count();
}

void FrameFinder::Cleanup()
{
    long long CurrentTime = Now();
    for(auto it = Data.begin(); it != Data.end(); )
    {
        if(CurrentTime - it->DateCreated > 60000)
        {
            it = Data.erase(it);
        } else
        {
            ++it;
        }
    }

}

std::string FrameFinder::GetReport()
{
    std::string Result;

    for(DataItem& Item: Data)
    {
        if(!Result.empty())
        {
            Result += std::string(",");
        }

        Result += Item.ActionId;
    }

    return Result;
}

FrameFinder::StatusInfo FrameFinder::GetSessionId(const std::string& ActionId, const std::string& Type)
{
    Cleanup();

    StatusInfo Result;

    for(auto it = Data.begin(); it != Data.end(); )
    {
        if(it->ActionId == ActionId)
        {
            if(it->HasSessionId)
            {
                if(Type == "touchStart")
                {
                    Result.Status = StatusInfo::IgnoreEvent;
                    return Result;
                }else if(Type == "touchEnd")
                {
                    Result.Status = StatusInfo::Success;
                    Result.SessionId = it->SessionId;
                    it = Data.erase(it);
                    return Result;
                }if(Type == "touchMove")
                {
                    Result.Status = StatusInfo::Success;
                    Result.SessionId = it->SessionId;
                    return Result;
                }

            }else
            {
                if(Type == "touchStart")
                {
                    Result.Status = StatusInfo::IgnoreEvent;
                    return Result;
                }else if(Type == "touchEnd")
                {
                    Result.Status = StatusInfo::Wait;
                    return Result;
                }if(Type == "touchMove")
                {
                    Result.Status = StatusInfo::Wait;
                    return Result;
                }
            }
        }

        ++it;
    }

    if(Type == "touchStart")
    {
        if(Data.empty())
        {
            DataItem NewItem;
            NewItem.DateCreated = Now();
            NewItem.HasSessionId = false;
            NewItem.ActionId = ActionId;
            Data.push_back(NewItem);

            Result.Status = StatusInfo::DoRequest;
            return Result;
        }else
        {
            Result.Status = StatusInfo::CloseHangingTouch;
            Result.SessionId = Data[0].SessionId;
            Result.ActionId = Data[0].ActionId;
            Data.erase(Data.begin());
            return Result;
        }

    }else if(Type == "touchEnd")
    {
        Result.Status = StatusInfo::IgnoreEvent;
        return Result;
    }if(Type == "touchMove")
    {
        Result.Status = StatusInfo::IgnoreEvent;
        return Result;
    }

    return Result;
}

void FrameFinder::SetSessionId(const std::string& ActionId, const std::string& SessionId)
{
    Cleanup();


    for(DataItem& Item: Data)
    {
        if(Item.ActionId == ActionId)
        {
            Item.SessionId = SessionId;
            Item.HasSessionId = true;
            break;
        }
    }
}
