#include "commandparser.h"
#include "rapidxml.hpp"
#include "picojson.h"
#include "log.h"
#include "split.h"
#include "replaceall.h"
#include "snappy.h"
#include "base64.h"


CommandParser::CommandParser()
{

}

void CommandParser::Parse(const std::string& Xml)
{
    rapidxml::xml_document<> doc;
    AllXml += Xml;
    std::string FindTag("</Messages>");


    while(true)
    {
        std::size_t pos = AllXml.find(FindTag);

        if(pos == std::string::npos)
            break;

        std::string CurrentXml = AllXml.substr(0,pos + FindTag.length());

        AllXml.erase(0,pos + FindTag.length());

        ReplaceAllInPlace(CurrentXml,"</Messages>","");
        ReplaceAllInPlace(CurrentXml,"<Messages>","");

        std::string Uncompressed;
        CurrentXml = base64_decode(CurrentXml);
        snappy::Uncompress(CurrentXml.data(),CurrentXml.length(),&Uncompressed);
        CurrentXml = std::string("<Messages>") + Uncompressed + std::string("</Messages>");

        //WORKER_LOG(std::string("Found xml : ") + CurrentXml);

        std::vector<char> data(CurrentXml.begin(), CurrentXml.end());
        data.push_back(0);
        try
        {
            doc.parse<0>(data.data());
        }catch(...)
        {
            //Parsing error, this should not happen.
            //In case of error, skip this entry, action will end with timeout.
            continue;
        }

        rapidxml::xml_node<> *MessagesNode = doc.first_node("Messages");
        if(!MessagesNode)
        {
            WORKER_LOG("Not found Messages element");
            continue;
        }

        rapidxml::xml_node<> *CommandNode = 0;

        CommandNode = MessagesNode->first_node("SetNextAction");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("SetNextAction");
            for(auto f:EventSetNextAction)
                f(value);
        }

        CommandNode = MessagesNode->first_node("Load");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("Load");
            for(auto f:EventLoad)
                f(value);
        }

        CommandNode = MessagesNode->first_node("View");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("View");
            for(auto f:EventView)
                f(value);
        }

        CommandNode = MessagesNode->first_node("GetTabs");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("GetTabs");
            for(auto f:EventGetTabs)
                f(value);
        }

        CommandNode = MessagesNode->first_node("ManualBrowserControl");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("EventManualBrowserControl");
            for(auto f:EventManualBrowserControl)
                f(value);
        }

        CommandNode = MessagesNode->first_node("DisableBrowser");
        if(CommandNode)
        {
            WORKER_LOG("EventDisableBorwser");
            for(auto f:EventDisableBorwser)
                f();
        }

        CommandNode = MessagesNode->first_node("SetCode");
        if(CommandNode)
        {
            bool is_testing = false;

            std::string schema;
            for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                if(std::string(attr->name()) == std::string("Schema"))
                {
                    schema = attr->value();
                }

                if(std::string(attr->name()) == std::string("IsTesting"))
                {
                    is_testing = std::string(attr->value()) == std::string("1");
                }
            }
            std::string code;
            rapidxml::xml_node<> *CommandNodeInternal = CommandNode->first_node("Code");
            if(CommandNodeInternal)
            {
                code = CommandNodeInternal->value();
            }

            std::string embedded;
            CommandNodeInternal = CommandNode->first_node("EmbeddedLanguageData");
            if(CommandNodeInternal)
            {
                embedded = CommandNodeInternal->value();
            }

            std::string application_engine_version;
            CommandNodeInternal = CommandNode->first_node("ApplicationEngineVersion");
            if(CommandNodeInternal)
            {
                application_engine_version = CommandNodeInternal->value();
            }

            std::string script_engine_version;
            CommandNodeInternal = CommandNode->first_node("ScriptEngineVersion");
            if(CommandNodeInternal)
            {
                script_engine_version = CommandNodeInternal->value();
            }

            std::string interface_state;
            CommandNodeInternal = CommandNode->first_node("InterfaceState");
            if(CommandNodeInternal)
            {
                interface_state = CommandNodeInternal->value();
            }

            WORKER_LOG("SetCode");
            for(auto f:EventSetCode)
                f(code,embedded,schema,is_testing, interface_state, script_engine_version, application_engine_version);
        }

        CommandNode = MessagesNode->first_node("SetResources");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG(std::string("SetResources<<") + value);
            for(auto f:EventSetResources)
                f(value);
        }

        CommandNode = MessagesNode->first_node("Visible");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("Visible");
            for(auto f:EventVisible)
                f(std::stoi(value));
        }

        CommandNode = MessagesNode->first_node("Flush");
        if(CommandNode)
        {
            WORKER_LOG("Flush");
            for(auto f:EventFlush)
                f();
        }

        CommandNode = MessagesNode->first_node("FindUrlByMask");
        if(CommandNode)
        {
            WORKER_LOG("FindUrlByMask");
            std::string value = CommandNode->value();
            for(auto f:EventFindUrlByMask)
                f(value);
        }

        CommandNode = MessagesNode->first_node("NavigateBack");
        if(CommandNode)
        {
            WORKER_LOG("EventNavigateBack");
            std::string value = CommandNode->value();
            for(auto f:EventNavigateBack)
                f(value == "true");
        }

        CommandNode = MessagesNode->first_node("IsChanged");
        if(CommandNode)
        {
            WORKER_LOG("IsChanged");
            for(auto f:EventIsChanged)
                f();
        }

        CommandNode = MessagesNode->first_node("SetUserAgent");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("EventSetUserAgent");
            for(auto f:EventSetUserAgent)
                f(value);
        }

        CommandNode = MessagesNode->first_node("PrepareFunction");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("EventPrepareFunction");
            for(auto f:EventPrepareFunction)
                f(value);
        }

        CommandNode = MessagesNode->first_node("BackupDone");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("EventBackupDone");
            for(auto f:EventBackupDone)
                f(value);
        }

        CommandNode = MessagesNode->first_node("RecaptchaV3List");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("EventRecaptchaV3List");
            for(auto f:EventRecaptchaV3List)
                f(value);
        }

        CommandNode = MessagesNode->first_node("ClickExtensionButton");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("ClickExtensionButton");
            for(auto f:EventClickExtensionButton)
                f(value);
        }


        CommandNode = MessagesNode->first_node("RecaptchaV3Result");
        if(CommandNode)
        {
            WORKER_LOG("EventRecaptchaV3Result");

            std::string action,id;

            for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                if(std::string(attr->name()) == std::string("Id"))
                {
                    id = attr->value();
                }

                if(std::string(attr->name()) == std::string("Result"))
                {
                    action = attr->value();
                }
            }

            for(auto f:EventRecaptchaV3Result)
            {
                f(id, action);
            }
        }

        CommandNode = MessagesNode->first_node("SetOpenFileName");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("EventSetOpenFileName");
            for(auto f:EventSetOpenFileName)
                f(value);
        }

        CommandNode = MessagesNode->first_node("DragFile");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("EventDragFile");
            for(auto f:EventDragFile)
                f(value);
        }

        CommandNode = MessagesNode->first_node("SetStartupScript");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("EventSetStartupScript");
            std::string target,script_id;

            for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                if(std::string(attr->name()) == std::string("target"))
                {
                    target = attr->value();
                }

                if(std::string(attr->name()) == std::string("script_id"))
                {
                    script_id = attr->value();
                }
            }

            for(auto f:EventSetStartupScript)
            {
                f(value, target, script_id);
            }

        }


        CommandNode = MessagesNode->first_node("RunTask");
        if(CommandNode)
        {
            WORKER_LOG("RunTask");
            std::string function_name,params,result_id;

            for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                if(std::string(attr->name()) == std::string("FunctionName"))
                {
                    function_name = attr->value();
                }

                if(std::string(attr->name()) == std::string("Parameters"))
                {
                    params = attr->value();
                }

                if(std::string(attr->name()) == std::string("ResultId"))
                {
                    result_id = attr->value();
                }
            }

            for(auto f:EventRunTask)
            {
                f(function_name, params, result_id);
            }

        }

        CommandNode = MessagesNode->first_node("CheckResult");
        if(CommandNode)
        {
            WORKER_LOG("EventCheckResult");
            std::string check_id,is_success,error_string;

            for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                if(std::string(attr->name()) == std::string("CheckId"))
                {
                    check_id = attr->value();
                }

                if(std::string(attr->name()) == std::string("IsSuccess"))
                {
                    is_success = attr->value();
                }

                if(std::string(attr->name()) == std::string("ErrorString"))
                {
                    error_string = attr->value();
                }
            }

            for(auto f:EventCheckResult)
            {
                f(check_id, is_success == std::string("true"), error_string);
            }

        }

        CommandNode = MessagesNode->first_node("SendWorkerSettings");
        if(CommandNode)
        {
            WORKER_LOG("EventSendWorkerSettings");
            bool EncodeUtf8 = true;
            bool RefreshConnections = false;
            std::string ProxyServer;
            int ProxyPort;
            bool ProxyIsHttp;
            std::string ProxyName;
            std::string ProxyPassword;
            std::string ProxyTarget;
            std::string BrowserEngine;
            std::string RecordId;

            for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                if(std::string(attr->name()) == std::string("BrowserEngine"))
                {
                    BrowserEngine = attr->value();
                }

                if(std::string(attr->name()) == std::string("RecordId"))
                {
                    RecordId = attr->value();
                }

                if(std::string(attr->name()) == std::string("ProxyServer"))
                {
                    ProxyServer = attr->value();
                }

                if(std::string(attr->name()) == std::string("ProxyPort"))
                {
                    ProxyPort = std::stoi(attr->value());
                }

                if(std::string(attr->name()) == std::string("ProxyIsHttp"))
                {
                    ProxyIsHttp = std::stoi(attr->value());
                }

                if(std::string(attr->name()) == std::string("ProxyName"))
                {
                    ProxyName = attr->value();
                }

                if(std::string(attr->name()) == std::string("ProxyPassword"))
                {
                    ProxyPassword = attr->value();
                }

                if(std::string(attr->name()) == std::string("ProxyTarget"))
                {
                    ProxyTarget = attr->value();
                }

                if(std::string(attr->name()) == std::string("EncodeUtf8"))
                {
                    EncodeUtf8 = std::stoi(attr->value());
                }

                if(std::string(attr->name()) == std::string("RefreshConnections"))
                {
                    RefreshConnections = std::stoi(attr->value());
                }

            }

            for(auto f:EventSendWorkerSettings)
            {
                f(EncodeUtf8, RefreshConnections, ProxyServer, ProxyPort, ProxyIsHttp, ProxyName, ProxyPassword, ProxyTarget, BrowserEngine, RecordId);
            }

        }

        CommandNode = MessagesNode->first_node("SetPromptResult");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("EventSetPromptResult");
            for(auto f:EventSetPromptResult)
                f(value);
        }

        CommandNode = MessagesNode->first_node("SetHttpAuthResult");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            std::vector<std::string> s = split(value,':');
            if(s.size() == 2)
            {
                std::string login = s[0];
                std::string password = s[1];
                WORKER_LOG("EventSetHttpAuthResult");
                for(auto f:EventSetHttpAuthResult)
                    f(login,password);
            }
        }


        CommandNode = MessagesNode->first_node("GetCookiesForUrl");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("EventGetCookiesForUrl");
            for(auto f:EventGetCookiesForUrl)
                f(value);
        }

        CommandNode = MessagesNode->first_node("SaveCookies");
        if(CommandNode)
        {
            WORKER_LOG("EventSaveCookies");
            for(auto f:EventSaveCookies)
                f();
        }

        CommandNode = MessagesNode->first_node("ClearImageData");
        if(CommandNode)
        {
            WORKER_LOG("EventClearImageData");
            for(auto f:EventClearImageData)
                f();
        }

        CommandNode = MessagesNode->first_node("SetImageData");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("EventSetImageData");
            for(auto f:EventSetImageData)
                f(value);
        }

        CommandNode = MessagesNode->first_node("FindImage");
        if(CommandNode)
        {
            WORKER_LOG("EventFindImage");
            for(auto f:EventFindImage)
                f();
        }

        CommandNode = MessagesNode->first_node("RestoreCookies");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("EventRestoreCookies");
            for(auto f:EventRestoreCookies)
                f(value);
        }

        CommandNode = MessagesNode->first_node("RestoreLocalStorage");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("EventRestoreLocalStorage");
            for(auto f:EventRestoreLocalStorage)
                f(value);
        }

        CommandNode = MessagesNode->first_node("RequestVariablesResult");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("RequestVariablesResult");
            for(auto f:EventRequestVariablesResult)
                f(value);
        }
 
        CommandNode = MessagesNode->first_node("DebugVariablesResult");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("DebugVariablesResult");
            for(auto f:EventDebugVariablesResult)
                f(value);
        }

        CommandNode = MessagesNode->first_node("DebugCallstackResult");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("DebugCallstackResult");
            for(auto f:EventDebugCallstackResult)
                f(value);
        }

        CommandNode = MessagesNode->first_node("ScriptFinished");
        if(CommandNode)
        {
            WORKER_LOG("ScriptFinished");
            for(auto f:EventScriptFinished)
                f();
        }

        CommandNode = MessagesNode->first_node("WaitCode");
        if(CommandNode)
        {
            WORKER_LOG("WaitCode");
            for(auto f:EventWaitCode)
                f();
        }

        CommandNode = MessagesNode->first_node("StartSection");
        if(CommandNode)
        {
            WORKER_LOG("StartSection");
            int value = std::stoi(CommandNode->value());
            for(auto f:EventStartSection)
                f(value);
        }

        CommandNode = MessagesNode->first_node("SetFontList");
        if(CommandNode)
        {
            WORKER_LOG("SetFontList");
            std::string value = CommandNode->value();
            for(auto f:EventSetFontList)
                f(value);
        }

        CommandNode = MessagesNode->first_node("GetUrl");
        if(CommandNode)
        {
            WORKER_LOG("GetUrl");
            for(auto f:EventGetUrl)
                f();
        }

        CommandNode = MessagesNode->first_node("GetBrowserScreenSettings");
        if(CommandNode)
        {
            WORKER_LOG("GetBrowserScreenSettings");
            for(auto f:EventGetBrowserScreenSettings)
                f();
        }

        CommandNode = MessagesNode->first_node("Crush");
        if(CommandNode)
        {
            WORKER_LOG("EventCrush");
            for(auto f:EventCrush)
                f();
        }

        CommandNode = MessagesNode->first_node("Resize");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("Resize");
            std::size_t pos = value.find(",");
            if(pos != std::string::npos)
            {
                std::string width = value.substr(0,pos);
                std::string height = value.substr(pos + 1,value.length() - pos - 1);
                for(auto f:EventResize)
                    f(std::stoi(width),std::stoi(height));

            }
        }

        CommandNode = MessagesNode->first_node("SetWindow");
        if(CommandNode)
        {
            bool is_play = false;
            for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                if(std::string(attr->name()) == std::string("is_play"))
                {
                    is_play = std::string(attr->value()) == std::string("1");
                }
            }

            std::string value = CommandNode->value();
            
            for(auto f:EventSetWindow)
                f(value, is_play);
        }

        CommandNode = MessagesNode->first_node("HighlightAction");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("EventHighlightAction");

            for(auto f:EventHighlightAction)
                f(value);
        }

        CommandNode = MessagesNode->first_node("MouseClick");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("MouseClick");
            std::size_t pos = value.find(",");
            if(pos != std::string::npos)
            {
                std::string x = value.substr(0,pos);
                std::string y = value.substr(pos + 1,value.length() - pos - 1);
                for(auto f:EventMouseClick)
                    f(std::stoi(x),std::stoi(y));

            }
        }

        CommandNode = MessagesNode->first_node("SetMode");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("SetMode");
            for(auto f:EventSetMode)
                f(value);
        }


        CommandNode = MessagesNode->first_node("RestoreOriginalStage");
        if(CommandNode)
        {
            WORKER_LOG("RestoreOriginalStage");
            for(auto f:EventRestoreOriginalStage)
                f();
        }



        CommandNode = MessagesNode->first_node("Timezone");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("Timezone");
            for(auto f:EventTimezone)
                f(std::stoi(value));
        }

        CommandNode = MessagesNode->first_node("MouseClickUp");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("MouseClickUp");
            std::size_t pos = value.find(",");
            if(pos != std::string::npos)
            {
                std::string x = value.substr(0,pos);
                std::string y = value.substr(pos + 1,value.length() - pos - 1);
                for(auto f:EventMouseClickUp)
                    f(std::stoi(x),std::stoi(y));

            }
        }

        CommandNode = MessagesNode->first_node("MouseClickDown");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("MouseClickDown");
            std::size_t pos = value.find(",");
            if(pos != std::string::npos)
            {
                std::string x = value.substr(0,pos);
                std::string y = value.substr(pos + 1,value.length() - pos - 1);
                for(auto f:EventMouseClickDown)
                    f(std::stoi(x),std::stoi(y));

            }
        }


        CommandNode = MessagesNode->first_node("PopupClose");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("PopupClose");
            for(auto f:EventPopupClose)
                f(std::stoi(value));
        }

        CommandNode = MessagesNode->first_node("PopupSelect");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("PopupSelect");
            for(auto f:EventPopupSelect)
                f(std::stoi(value));
        }

        CommandNode = MessagesNode->first_node("PopupInfo");
        if(CommandNode)
        {
            WORKER_LOG("PopupInfo");
            for(auto f:EventPopupInfo)
                f();
        }

        CommandNode = MessagesNode->first_node("MouseMove");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("MouseMove");
            std::size_t pos = value.find(",");
            if(pos != std::string::npos)
            {
                std::string x = value.substr(0,pos);
                std::string y = value.substr(pos + 1,value.length() - pos - 1);
                float speed = -1.0;
                float gravity = -1.0;
                float deviation = -1.0;
                bool iscoordinates = false;
                bool domouseup = false;
                double releaseradius = 0.0;
                bool relativecoordinates = false;
                bool trackscroll = false;
                for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
                {
                    if(std::string(attr->name()) == std::string("params"))
                    {
                        picojson::value val;
                        picojson::parse(val,attr->value());
                        picojson::object obj = val.get<picojson::object>();
                        if(obj.count("speed")>0)
                        {
                            speed = obj["speed"].get<double>();
                        }
                        if(obj.count("gravity")>0)
                        {
                            gravity = obj["gravity"].get<double>();
                        }
                        if(obj.count("deviation")>0)
                        {
                            deviation = obj["deviation"].get<double>();
                        }
                        if(obj.count("iscoordinates")>0)
                        {
                            iscoordinates = obj["iscoordinates"].get<std::string>() == std::string("true");
                        }
                        if(obj.count("do_mouse_up")>0)
                        {
                            domouseup = obj["do_mouse_up"].get<std::string>() == std::string("true");
                        }
                        if(obj.count("release_radius")>0)
                        {
                            releaseradius = obj["release_radius"].get<double>();
                        }
                        if(obj.count("relative_coordinates")>0)
                        {
                            relativecoordinates = obj["relative_coordinates"].get<std::string>() == std::string("true");
                        }
                        if(obj.count("track_scroll")>0)
                        {
                            trackscroll = obj["track_scroll"].get<std::string>() == std::string("true");
                        }
                    }
                }

                for(auto f:EventMouseMove)
                    f(std::stoi(x),std::stoi(y),speed,gravity,deviation,iscoordinates,domouseup,releaseradius,relativecoordinates,trackscroll);

            }
        }

        CommandNode = MessagesNode->first_node("SetDeviceScaleFactor");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("SetDeviceScaleFactor");

            for(auto f:EventSetDeviceScaleFactor)
                f(std::stod(value));
        }

        CommandNode = MessagesNode->first_node("PopupCreate");
        if(CommandNode)
        {
            WORKER_LOG("PopupCreate");
            bool is_silent = false;
            std::string url;
            for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                if(std::string(attr->name()) == std::string("is_silent"))
                {
                    is_silent = std::string(attr->value()) == std::string("1");
                }
                if(std::string(attr->name()) == std::string("url"))
                {
                    url = std::string(attr->value());
                }
            }

            for(auto f:EventPopupCreate)
                f(is_silent,url);


        }

        CommandNode = MessagesNode->first_node("PopupCreate2");
        if(CommandNode)
        {
            WORKER_LOG("PopupCreate2");
            bool is_silent = false;
            std::string url;
            bool is_instant = false;
            std::string referrer;
            for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                if(std::string(attr->name()) == std::string("is_silent"))
                {
                    is_silent = std::string(attr->value()) == std::string("1");
                }
                if(std::string(attr->name()) == std::string("is_instant"))
                {
                    is_instant = std::string(attr->value()) == std::string("1");
                }
                if(std::string(attr->name()) == std::string("url"))
                {
                    url = std::string(attr->value());
                }
                if(std::string(attr->name()) == std::string("referrer"))
                {
                    referrer = std::string(attr->value());
                }
            }

            for(auto f:EventPopupCreate2)
                f(is_silent,url,referrer,is_instant);


        }

        CommandNode = MessagesNode->first_node("SetComboboxIndex");
        if(CommandNode)
        {
            WORKER_LOG("SetComboboxIndex");
            int index = -1;
            for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                if(std::string(attr->name()) == std::string("index"))
                {
                    index = std::stoi(std::string(attr->value()));
                }
            }

            for(auto f:EventSetComboboxIndex)
                f(index);

        }

        CommandNode = MessagesNode->first_node("Scroll");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("Scroll");
            std::size_t pos = value.find(",");
            if(pos != std::string::npos)
            {
                std::string x = value.substr(0,pos);
                std::string y = value.substr(pos + 1,value.length() - pos - 1);
                for(auto f:EventScroll)
                    f(std::stoi(x),std::stoi(y));

            }
        }

        CommandNode = MessagesNode->first_node("Render");
        if(CommandNode)
        {
            std::string value = CommandNode->value();
            WORKER_LOG("Render");
            int x=-1,y=-1,width=-1,height=-1;

            std::size_t pos = value.find(",");
            if(pos != std::string::npos)
            {
                x = std::stoi(value.substr(0,pos));
                value = value.substr(pos + 1,value.length() - pos - 1);
            }

            pos = value.find(",");
            if(pos != std::string::npos)
            {
                y = std::stoi(value.substr(0,pos));
                value = value.substr(pos + 1,value.length() - pos - 1);
            }

            pos = value.find(",");
            if(pos != std::string::npos)
            {
                width = std::stoi(value.substr(0,pos));
                value = value.substr(pos + 1,value.length() - pos - 1);
            }

            height = std::stoi(value);

            for(auto f:EventRender)
                f(x,y,width,height);
        }

        CommandNode = MessagesNode->first_node("AddHeader");
        if(CommandNode)
        {
            WORKER_LOG("AddHeader");

            std::string key,value;
            std::string target;
            for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                WORKER_LOG(attr->name());
                if(std::string(attr->name()) == std::string("name"))
                {
                    WORKER_LOG(attr->value());
                    key = attr->value();
                }else if(std::string(attr->name()) == std::string("value"))
                {
                    WORKER_LOG(attr->value());
                    value = attr->value();
                }else if(std::string(attr->name()) == std::string("target"))
                {
                    target = attr->value();
                }
            }

            for(auto f:EventAddHeader)
                f(key,value,target);
        }

        CommandNode = MessagesNode->first_node("SetHeaderList");
        if(CommandNode)
        {
            WORKER_LOG("SetHeaderList");

            std::string json;
            for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                WORKER_LOG(attr->name());
                if(std::string(attr->name()) == std::string("json"))
                {
                    WORKER_LOG(attr->value());
                    json = attr->value();
                }
            }

            for(auto f:EventSetHeaderList)
                f(json);
        }


        CommandNode = MessagesNode->first_node("SetAcceptLanguagePattern");
        if(CommandNode)
        {
            WORKER_LOG("SetAcceptLanguagePattern");

            std::string pattern;
            for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                if(std::string(attr->name()) == std::string("pattern"))
                {
                    pattern = attr->value();
                }
            }

            for(auto f:EventSetAcceptLanguagePattern)
                f(pattern);
        }

        CommandNode = MessagesNode->first_node("SetUserAgentData");
        if(CommandNode)
        {
            WORKER_LOG("SetUserAgentData");

            std::string data;
            for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                if(std::string(attr->name()) == std::string("data"))
                {
                    data = attr->value();
                }
            }

            for(auto f:EventSetUserAgentData)
                f(data);
        }


        CommandNode = MessagesNode->first_node("CleanHeader");
        if(CommandNode)
        {
            WORKER_LOG("CleanHeader");
            for(auto f:EventCleanHeader)
                f();
        }

        CommandNode = MessagesNode->first_node("AddCacheMaskAllow");
        if(CommandNode)
        {
            WORKER_LOG("AddCacheMaskAllow");
            std::string value = CommandNode->value();
            for(auto f:EventAddCacheMaskAllow)
                f(value);
        }



        CommandNode = MessagesNode->first_node("AddCacheMaskDeny");
        if(CommandNode)
        {
            WORKER_LOG("AddCacheMaskDeny");
            std::string value = CommandNode->value();
            for(auto f:EventAddCacheMaskDeny)
                f(value);
        }

        CommandNode = MessagesNode->first_node("AddRequestMaskAllow");
        if(CommandNode)
        {
            WORKER_LOG("AddRequestMaskAllow");
            std::string value = CommandNode->value();
            for(auto f:EventAddRequestMaskAllow)
                f(value);
        }

        CommandNode = MessagesNode->first_node("AddRequestMaskDeny");
        if(CommandNode)
        {
            WORKER_LOG("AddRequestMaskDeny");
            std::string value = CommandNode->value();
            for(auto f:EventAddRequestMaskDeny)
                f(value);
        }

        CommandNode = MessagesNode->first_node("ClearCacheMask");
        if(CommandNode)
        {
            WORKER_LOG("ClearCacheMask");
            for(auto f:EventClearCacheMask)
                f();
        }

        CommandNode = MessagesNode->first_node("ClearRequestMask");
        if(CommandNode)
        {
            WORKER_LOG("ClearRequestMask");
            for(auto f:EventClearRequestMask)
                f();
        }

        CommandNode = MessagesNode->first_node("RestrictPopups");
        if(CommandNode)
        {
            WORKER_LOG("RestrictPopups");
            for(auto f:EventRestrictPopups)
                f();
        }

        CommandNode = MessagesNode->first_node("AllowPopups");
        if(CommandNode)
        {
            WORKER_LOG("AllowPopups");
            for(auto f:EventAllowPopups)
                f();
        }

        CommandNode = MessagesNode->first_node("RestrictDownloads");
        if(CommandNode)
        {
            WORKER_LOG("RestrictDownloads");
            for(auto f:EventRestrictDownloads)
                f();
        }

        CommandNode = MessagesNode->first_node("AllowDownloads");
        if(CommandNode)
        {
            WORKER_LOG("AllowDownloads");
            for(auto f:EventAllowDownloads)
                f();
        }

        CommandNode = MessagesNode->first_node("ClearLoadedUrl");
        if(CommandNode)
        {
            WORKER_LOG("ClearLoadedUrl");
            for(auto f:EventClearLoadedUrl)
                f();
        }

        CommandNode = MessagesNode->first_node("ClearCachedData");
        if(CommandNode)
        {
            WORKER_LOG("ClearCachedData");
            for(auto f:EventClearCachedData)
                f();
        }

        CommandNode = MessagesNode->first_node("ClearAll");
        if(CommandNode)
        {
            WORKER_LOG("ClearAll");
            for(auto f:EventClearAll)
                f();
        }

        CommandNode = MessagesNode->first_node("ClearMasks");
        if(CommandNode)
        {
            WORKER_LOG("ClearMasks");
            for(auto f:EventClearMasks)
                f();
        }

        CommandNode = MessagesNode->first_node("ClearData");
        if(CommandNode)
        {
            WORKER_LOG("ClearData");
            for(auto f:EventClearData)
                f();
        }

        CommandNode = MessagesNode->first_node("FindCacheByMaskBase64");
        if(CommandNode)
        {
            WORKER_LOG("FindCacheByMaskBase64");
            std::string value = CommandNode->value();
            for(auto f:EventFindCacheByMaskBase64)
                f(value);
        }

        CommandNode = MessagesNode->first_node("FindStatusByMask");
        if(CommandNode)
        {
            WORKER_LOG("FindStatusByMask");
            std::string value = CommandNode->value();
            for(auto f:EventFindStatusByMask)
                f(value);
        }

        CommandNode = MessagesNode->first_node("FindCacheByMaskString");
        if(CommandNode)
        {
            WORKER_LOG("FindCacheByMaskString");
            std::string value = CommandNode->value();
            for(auto f:EventFindCacheByMaskString)
                f(value);
        }

        CommandNode = MessagesNode->first_node("FindAllCache");
        if(CommandNode)
        {
            WORKER_LOG("FindAllCache");
            std::string value = CommandNode->value();
            for(auto f:EventFindAllCache)
                f(value);
        }

        CommandNode = MessagesNode->first_node("IsUrlLoadedByMask");
        if(CommandNode)
        {
            WORKER_LOG("IsUrlLoadedByMask");
            std::string value = CommandNode->value();
            for(auto f:EventIsUrlLoadedByMask)
                f(value);
        }

        CommandNode = MessagesNode->first_node("GetLoadStats");
        if(CommandNode)
        {
            WORKER_LOG("GetLoadStats");
            for(auto f:EventGetLoadStats)
                f();
        }


        CommandNode = MessagesNode->first_node("Element");
        if(CommandNode)
        {
            WORKER_LOG("Element");
            ElementCommand send;
            CommandNode = CommandNode->first_node();
            send.CommandName = CommandNode->name();
            WORKER_LOG(std::string("command : ") + send.CommandName);
            for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                if(std::string(attr->name()) == std::string("ID"))
                {
                    send.CommandId = attr->value();
                    WORKER_LOG(std::string("id : ") + send.CommandId);
                }
                if(std::string(attr->name()) == std::string("NoWait"))
                {
                    send.IsNoWait = std::stoi(attr->value());
                    WORKER_LOG(std::string("nowait : ") + std::to_string(send.IsNoWait));
                }
            }
            rapidxml::xml_node<> *A = CommandNode->first_node("A");
            if(A)
            {
                send.CommandParam1 = A->first_attribute("value")->value();
                WORKER_LOG(std::string("param1 : ") + send.CommandParam1);
            }

            rapidxml::xml_node<> *B = CommandNode->first_node("B");
            if(B)
            {
                send.CommandParam2 = B->first_attribute("value")->value();
                WORKER_LOG(std::string("param2 : ") + send.CommandParam2);
            }

            rapidxml::xml_node<> *Path = CommandNode->first_node("Path");
            if(Path)
            {
                for (rapidxml::xml_node<> *path_part = Path->first_node(); path_part; path_part = path_part->next_sibling())
                {
                    std::pair<std::string,std::string> p;
                    p.first = path_part->name();
                    p.second = path_part->value();
                    WORKER_LOG(std::string("path1 : ") + p.first);
                    WORKER_LOG(std::string("path2 : ") + p.second);
                    if(p.first == "frame")
                    {
                        send.FrameUrl = p.second;
                    }else
                        send.Path.push_back(p);
                }
            }
            for(auto f:EventElementCommand)
                f(send);
        }

        CommandNode = MessagesNode->first_node("Load2");
        if(CommandNode)
        {
            WORKER_LOG("EventLoad2");

            std::string url,referrer,instant;
            for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                if(std::string(attr->name()) == std::string("url"))
                {
                    url = attr->value();
                }else if(std::string(attr->name()) == std::string("referrer"))
                {
                    referrer = attr->value();
                }else if(std::string(attr->name()) == std::string("instant"))
                {
                    instant = attr->value();
                }
            }


            for(auto f:EventLoad2)
                f(url,referrer,instant == "true");
        }



        CommandNode = MessagesNode->first_node("SetProxy");
        if(CommandNode)
        {
            WORKER_LOG("SetProxy");

            std::string server,name,password;
            int Port;
            bool IsHttp;
            std::string target;
            for (rapidxml::xml_attribute<> *attr = CommandNode->first_attribute(); attr; attr = attr->next_attribute())
            {
                WORKER_LOG(attr->name());
                if(std::string(attr->name()) == std::string("server"))
                {
                    WORKER_LOG(attr->value());
                    server = attr->value();
                }else if(std::string(attr->name()) == std::string("Port"))
                {
                    WORKER_LOG(attr->value());
                    Port = std::stoi(attr->value());
                }else if(std::string(attr->name()) == std::string("IsHttp"))
                {
                    WORKER_LOG(attr->value());
                    IsHttp = std::stoi(attr->value());
                }else if(std::string(attr->name()) == std::string("name"))
                {
                    WORKER_LOG(attr->value());
                    name = attr->value();
                }else if(std::string(attr->name()) == std::string("password"))
                {
                    WORKER_LOG(attr->value());
                    password = attr->value();
                }else if(std::string(attr->name()) == std::string("target"))
                {
                    target = attr->value();
                }
            }



            for(auto f:EventSetProxy)
                f(server,Port,IsHttp,name,password,target);
        }
    }

}
