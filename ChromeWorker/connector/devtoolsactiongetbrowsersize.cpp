#include "devtoolsactiongetbrowsersize.h"

void DevToolsActionGetBrowserSize::Run()
{
    State = Running;
    SendWebSocket("Browser.getWindowForTarget",Params);
}

void DevToolsActionGetBrowserSize::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    int Width = GetFloatFromJson(Message, "bounds.width");
    int Height = GetFloatFromJson(Message, "bounds.height");

    std::vector<std::string> ResultList;
    ResultList.push_back(std::to_string(Width));
    ResultList.push_back(std::to_string(Height));
    Result->Success(ResultList);
    State = Finished;
}
