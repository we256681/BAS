#include "devtoolsactiongetnavigationhistory.h"

void DevToolsActionGetNavigationHistory::Run()
{
    State = Running;

    std::map<std::string, Variant> CurrentParams;
    SendWebSocket("Page.getNavigationHistory", CurrentParams);
}

void DevToolsActionGetNavigationHistory::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    picojson::value AllValue;
    picojson::parse(AllValue, Message);
    picojson::array AllEntries = AllValue.get<picojson::object>()["entries"].get<picojson::array>();
    int CurrentIndex = AllValue.get<picojson::object>()["currentIndex"].get<double>();

    Result->SetResult(CurrentIndex);

    std::vector<std::string> Res;

    for(picojson::value& Value: AllEntries)
    {
        std::string Url = Value.get<picojson::object>()["url"].get<std::string>();
        Res.push_back(Url);
    }

    Result->Success(Res);
    State = Finished;

}
