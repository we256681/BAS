#include "DevToolsActionSetProxy.h"
#include "converter.h"
#include "readallfile.h"
#include <windows.h>
#include <fstream>
#include <chrono>

using namespace std::chrono;

void DevToolsActionSetProxy::Run()
{
    //This action will do following steps:
    // 1) Stop new reqeusts from being send.
    // 2) Wait 3 seconds.
    // 3) Close all connections.
    // 4) Wait 3 seconds.
    // 5) Make all connection run through proxy.
    // 4) Wait 0.5 seconds.

    //Init folder

    //Check if same proxy already applied.
    {
        std::string Server = Params["server"].String;
        int Port = Params["port"].Number;
        bool IsHttp = Params["is_http"].Boolean;
        std::string Login = Params["login"].String;
        std::string Password = Params["password"].String;

        if(GlobalState->SaveProxy->IsCurrentProxyConfigEquals(Server, Port, IsHttp, Login, Password))
        {
            //We are using same proxy, immediatelly return
            State = Finished;
            Result->Success();
            return;
        }
    }


    //Initialization

    WaitingForSetProxy = false;
    WaitingForResetAllConnections = false;
    WaitingForStopNetworkActivity = false;

    //Stop new reqeusts from being send
    GlobalState->SaveProxy->WriteProxyConfig("127.0.0.1", 0, true, std::string(), std::string());

    //Wait 3 seconds
    FinishActionTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() + 3000;
    WaitingForStopNetworkActivity = true;
    SubscribbedEvents.push_back("Timer");
    State = Running;
}


void DevToolsActionSetProxy::OnWebSocketEvent(const std::string& Method, const std::string& Message)
{
    if(Method == "Timer")
    {
        long long Now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        if(Now > FinishActionTime)
        {
            if(WaitingForStopNetworkActivity)
            {
                //Initialization
                WaitingForSetProxy = false;
                WaitingForResetAllConnections = false;
                WaitingForStopNetworkActivity = false;

                GlobalState->SaveProxy->ResetAllConnections();

                FinishActionTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() + 3000;
                WaitingForResetAllConnections = true;
                return;
            }

            if(WaitingForResetAllConnections)
            {
                //Initialization
                WaitingForSetProxy = false;
                WaitingForResetAllConnections = false;
                WaitingForStopNetworkActivity = false;

                //Parse params
                std::string Server = Params["server"].String;
                Params.erase("server");

                int Port = Params["port"].Number;
                Params.erase("port");

                bool IsHttp = Params["is_http"].Boolean;
                Params.erase("is_http");

                std::string Login = Params["login"].String;
                Params.erase("login");

                std::string Password = Params["password"].String;
                Params.erase("password");

                //Generate proxy data
                GlobalState->SaveProxy->WriteProxyConfig(Server, Port, IsHttp, Login, Password);

                FinishActionTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() + 500;
                WaitingForSetProxy = true;
                return;
            }

            if(WaitingForSetProxy)
            {
                WaitingForSetProxy = false;
                WaitingForResetAllConnections = false;
                WaitingForStopNetworkActivity = false;

                State = Finished;
                Result->Success();
                return;
            }



        }
    }
}
