#include "devtoolsactionsleep.h"
#include <chrono>

using namespace std::chrono;

void DevToolsActionSleep::Run()
{
    FinishActionTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() + Params["time"].Number;
    Params.erase("time");

    SubscribbedEvents.push_back("Timer");
    State = Running;

}


void DevToolsActionSleep::OnWebSocketEvent(const std::string& Method, const std::string& Message)
{

    if(Method == "Timer")
    {
        long long Now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        if(Now > FinishActionTime)
        {
            State = Finished;
            Result->Success();
        }
    }

}
