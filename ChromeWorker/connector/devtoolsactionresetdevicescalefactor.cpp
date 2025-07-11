#include "devtoolsactionresetdevicescalefactor.h"
#include <windows.h>
#include <fstream>
#include <chrono>
#include "fileexists.h"

using namespace std::chrono;

void DevToolsActionResetDeviceScaleFactor::Run()
{
    GlobalState->SaveProxy->ResetDPI();
    Path = GlobalState->SaveProxy->GetConfigFilePath("dpi");

    //Wait 3 seconds
    NextCheck = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() + 1000;
    SubscribbedEvents.push_back("Timer");
    State = Running;
}


void DevToolsActionResetDeviceScaleFactor::OnWebSocketEvent(const std::string& Method, const std::string& Message)
{
    if(Method == "Timer" && NextCheck > 0)
    {
        long long Now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        if(Now > NextCheck)
        {
            if(!FileExists(Path))
            {
                NextCheck = 0;
                State = Finished;
                Result->Success();
                return;
            }
            {
                NextCheck = Now + 1000;
            }
        }
    }
}
