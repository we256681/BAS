#include "devtoolsactionsavecookies.h"

void DevToolsActionSaveCookies::Run()
{
    State = Running;
    SendWebSocket("Network.getAllCookies", Params);
}

void DevToolsActionSaveCookies::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    std::string ResultString("[]");

    picojson::value AllValue;
    std::string err = picojson::parse(AllValue, Message);
    if(err.empty() && AllValue.is<picojson::object>())
    {
        picojson::object AllObject = AllValue.get<picojson::object>();
        if(AllObject.count("cookies") == 1 && AllObject["cookies"].is<picojson::array>())
        {
            picojson::array AllEntries = AllObject["cookies"].get<picojson::array>();
            ResultString = picojson::value(AllEntries).serialize();
        }
    }
    Result->Success(ResultString);
    State = Finished;
}
