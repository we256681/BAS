#include "devtoolsactionopenfile.h"
#include <chrono>
#include <windows.h>
#include "converter.h"

using namespace std::chrono;

void DevToolsActionOpenFile::Run()
{
    //Parse params
    NodeId = Params["node_id"].Float;
    Params.erase("node_id");

    IsMultiple = Params["is_multiple"].Boolean;
    Params.erase("is_multiple");

    TargetIdForElement = Params["target_id"].String;
    Params.erase("target_id");

    OpenFileDialogIsManual = GlobalState->OpenFileDialogIsManual;
    OpenFileDialogResult = GlobalState->OpenFileDialogResult;

    if(OpenFileDialogIsManual)
    {
       State = Finished;
       Result->Success();
    }else
    {
        if (!OpenFileDialogResult.empty())
        {
            FinishActionTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() + 1000;
            SubscribbedEvents.push_back("Timer");
            State = Running;
        }else
        {
            State = Finished;
            Result->Success();
        }
    }

}


void DevToolsActionOpenFile::OnWebSocketEvent(const std::string& Method, const std::string& Message)
{
    if(OpenFileDialogIsManual)
    {

    }else
    {
        if(Method == "Timer")
        {
            long long Now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
            if(Now > FinishActionTime)
            {

                std::vector<Variant> Files;
                picojson::value Json;

                std::string Error = picojson::parse(Json, OpenFileDialogResult);

                if (Error.empty() && Json.is<picojson::array>())
                {
                    for (const auto& File : Json.get<picojson::array>())
                    {
                        if (File.is<std::string>())
                        {
                            Files.push_back(File.get<std::string>());
                            if(!IsMultiple)
                                break;
                        }
                    }
                }
                else
                {
                    Files.push_back(OpenFileDialogResult);
                }

                std::map<std::string, Variant> CurrentParams;

                CurrentParams["backendNodeId"] = Variant(NodeId);
                CurrentParams["files"] = Variant(Files);


                if(TargetIdForElement.empty())
                {
                    SendWebSocket("DOM.setFileInputFiles", CurrentParams);
                }else
                {
                    SendWebSocket("DOM.setFileInputFiles", CurrentParams, TargetIdForElement);
                }

                State = Finished;
                Result->Success();
            }
        }
    }
}
