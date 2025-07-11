#ifndef IDEVTOOLSACTION_H
#define IDEVTOOLSACTION_H

#include "asyncresult.h"
#include "DevToolsGlobalState.h"
#include "Variant.h"
#include "json/picojson.h"
#include "JsonParser.h"
#include "JsonSerializer.h"


class IDevToolsAction
{
    protected:
        JsonParser Parser;
        JsonSerializer Serializer;
        std::string GetDefaultTabId();
        void SendWebSocket(const std::string& Method,const std::map<std::string,Variant>& Params, std::string TabId = std::string("CurrentTab"));
        std::string GetActualTabId();
        std::shared_ptr<TabData> FindTabById(const std::string& TabId);
        std::string GetStringFromJson(const std::string& Json, const std::string& Path, const std::string& Default = std::string());
        bool GetBooleanFromJson(const std::string& Json, const std::string& Path, bool Default = false);
        double GetFloatFromJson(const std::string& Json, const std::string& Path, float Default = 0.0);
    public:
        enum StateEnum
        {
            NotStarted,
            Running,
            Finished
        };

        enum ActionSaverBehavior
        {
            DontSave,
            SaveAccumulateSameType,
            SaveClearSameType,
            SaveAndCustomFilterFunction,
            DontSaveAndCustomFilterFunction
        };

    protected:
        //Inputs
        Async Result;
        std::map<std::string,Variant> Params;
        std::vector<std::string> SubscribbedEvents;
        int Id = 0;
        //Unique id is used to find/interrupt action by using result
        int UniqueId = 0;
        std::string OverrideDefaultTabId;
        DevToolsGlobalState* GlobalState;
        bool RunnningForAllRunActiveTab = false;
        int GroupId;

        //Outputs
        StateEnum State = NotStarted;
        std::string TypeName;

        //Timeout
        int Timeout = -1;
        long long TimeStarted = 0;
        long long Deadline = 0;

    public:
        //Getters/setters

        void Initialize(const std::string& TypeName);

        virtual Async GetResult();
        virtual void SetResult(Async Result);
        virtual std::map<std::string, Variant> GetParams();
        virtual void SetParams(const std::map<std::string, Variant>& Params);
        virtual std::vector<std::string> GetSubscribbedEvents();
        virtual void SetSubscribbedEvents(const std::vector<std::string>& SubscribbedEvents);
        virtual int GetId();
        virtual void SetId(int Id);
        virtual int GetUniqueId();
        virtual void SetUniqueId(int UniqueId);
        virtual std::string GetOverrideDefaultTabId();
        virtual void SetOverrideDefaultTabId(const std::string& OverrideDefaultTabId);
        virtual DevToolsGlobalState* GetGlobalState();
        virtual void SetGlobalState(DevToolsGlobalState* GlobalState);
        virtual bool GetRunnningForAllRunActiveTab();
        virtual void SetRunnningForAllRunActiveTab(bool RunnningForAllRunActiveTab);
        virtual int GetGroupId();
        virtual void SetGroupId(int GroupId);
        virtual StateEnum GetState();
        virtual void SetState(StateEnum State);
        virtual std::string GetTypeName();
        virtual void SetTypeName(const std::string& TypeName);
        virtual int GetTimeout();
        virtual void SetTimeout(int Timeout);
        virtual long long GetTimeStarted();
        virtual void SetTimeStarted(long long TimeStarted);
        virtual long long GetDeadline();
        virtual void SetDeadline(long long Deadline);


        //ActionSaver
        virtual ActionSaverBehavior GetActionSaverBehavior();
        virtual bool FilterActionSaver(std::shared_ptr<IDevToolsAction> Action);
        virtual bool IsNeedToRunForAllActiveTabs();
        virtual bool IsNeedToRunForAllActiveFrames();
        virtual void OnTabCreation();
        virtual void OnTabSwitching();
        virtual void OnRunnningForAllRunActiveTab();

        //Helper to clone action
        void CloneTo(IDevToolsAction* Object);

        virtual void Run() = 0;
        virtual bool UsesHttpClient();
        virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
        virtual void OnWebSocketEvent(const std::string& Method,const std::string& Message);
        virtual void OnHttpClientMessage(bool IsSuccess,int StatusCode,std::string& Data);
};

#endif // IDEVTOOLSACTION_H
