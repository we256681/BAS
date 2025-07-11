#include "devtoolsactionrestorecookies.h"

void DevToolsActionRestoreCookies::Run()
{
    std::string CookiesString = Params["cookies"].String;
    Params.erase("cookies");

    //Parse cookies
    picojson::value AllValue;
    std::string err = picojson::parse(AllValue, CookiesString);
    if(!err.empty() || !AllValue.is<picojson::array>())
    {
        Cookies.clear();
    }else
    {
        picojson::array AllList = AllValue.get<picojson::array>();
        for(picojson::value& Val: AllList)
        {
            if(Val.is<picojson::object>())
            {
                picojson::object Obj = Val.get<picojson::object>();
                std::map<std::string, Variant> CookieItem;
                for(std::map<std::string,picojson::value>::iterator it = Obj.begin(); it != Obj.end(); ++it)
                {
                    std::string Key = it->first;
                    picojson::value Value = it->second;
                    if(Value.is<std::string>())
                    {
                        CookieItem[Key] = Variant(Value.get<std::string>());
                    }

                    if(Value.is<double>())
                    {
                        CookieItem[Key] = Variant(Value.get<double>());
                    }

                    if(Value.is<bool>())
                    {
                        CookieItem[Key] = Variant(Value.get<bool>());
                    }

                }
                Cookies.push_back(Variant(CookieItem));
            }
        }
    }


    State = Running;

    IsDeletingCookies = true;
    IsSettingCookies = false;
    SendWebSocket("Network.clearBrowserCookies", Params);
}

void DevToolsActionRestoreCookies::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    if(IsDeletingCookies)
    {
        if(Cookies.empty())
        {
            IsDeletingCookies = false;
            IsSettingCookies = false;
            Result->Success();
            State = Finished;
            return;
        }else
        {
            IsDeletingCookies = false;
            IsSettingCookies = true;
            std::map<std::string, Variant> CurrentParams;
            CurrentParams["cookies"] = Variant(Cookies);
            SendWebSocket("Network.setCookies", CurrentParams);
            return;
        }
    }

    if(IsSettingCookies)
    {
        IsDeletingCookies = false;
        IsSettingCookies = false;
        Result->Success();
        State = Finished;
        return;
    }

}
