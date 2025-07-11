#include "idevtoolsaction.h"
#include <sstream>

void IDevToolsAction::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{

}

void IDevToolsAction::OnHttpClientMessage(bool IsSuccess,int StatusCode,std::string& Data)
{

}

void IDevToolsAction::OnWebSocketEvent(const std::string& Method,const std::string& Message)
{

}

void IDevToolsAction::Initialize(const std::string& TypeName)
{
    SetTypeName(TypeName);
    this->Result = std::make_shared<AsyncResult>();
}

Async IDevToolsAction::GetResult()
{
    return Result;
}

void IDevToolsAction::SetResult(Async Result)
{
    this->Result = Result;
}

std::map<std::string, Variant> IDevToolsAction::GetParams()
{
    return Params;
}

void IDevToolsAction::SetParams(const std::map<std::string, Variant>& Params)
{
    this->Params = Params;
}

std::vector<std::string> IDevToolsAction::GetSubscribbedEvents()
{
    return SubscribbedEvents;
}

void IDevToolsAction::SetSubscribbedEvents(const std::vector<std::string>& SubscribbedEvents)
{
    this->SubscribbedEvents = SubscribbedEvents;
}

int IDevToolsAction::GetId()
{
    return Id;
}

void IDevToolsAction::SetId(int Id)
{
    this->Id = Id;
}

int IDevToolsAction::GetUniqueId()
{
    return UniqueId;
}

void IDevToolsAction::SetUniqueId(int UniqueId)
{
    this->UniqueId = UniqueId;
}

std::string IDevToolsAction::GetOverrideDefaultTabId()
{
    return OverrideDefaultTabId;
}

void IDevToolsAction::SetOverrideDefaultTabId(const std::string& OverrideDefaultTabId)
{
    this->OverrideDefaultTabId = OverrideDefaultTabId;
}

DevToolsGlobalState* IDevToolsAction::GetGlobalState()
{
    return GlobalState;
}

void IDevToolsAction::SetGlobalState(DevToolsGlobalState* GlobalState)
{
    this->GlobalState = GlobalState;
}

bool IDevToolsAction::GetRunnningForAllRunActiveTab()
{
    return RunnningForAllRunActiveTab;
}

void IDevToolsAction::SetRunnningForAllRunActiveTab(bool RunnningForAllRunActiveTab)
{
    this->RunnningForAllRunActiveTab = RunnningForAllRunActiveTab;
}

int IDevToolsAction::GetGroupId()
{
    return GroupId;
}

void IDevToolsAction::SetGroupId(int GroupId)
{
    this->GroupId = GroupId;
}

IDevToolsAction::StateEnum IDevToolsAction::GetState()
{
    return State;
}

void IDevToolsAction::SetState(StateEnum State)
{
    this->State = State;
}

std::string IDevToolsAction::GetTypeName()
{
    return TypeName;
}

void IDevToolsAction::SetTypeName(const std::string& TypeName)
{
    this->TypeName = TypeName;
}

int IDevToolsAction::GetTimeout()
{
    return Timeout;
}

void IDevToolsAction::SetTimeout(int Timeout)
{
    this->Timeout = Timeout;
}

long long IDevToolsAction::GetTimeStarted()
{
    return TimeStarted;
}

void IDevToolsAction::SetTimeStarted(long long TimeStarted)
{
    this->TimeStarted = TimeStarted;
}

long long IDevToolsAction::GetDeadline()
{
    return Deadline;
}

void IDevToolsAction::SetDeadline(long long Deadline)
{
    this->Deadline = Deadline;
}


bool IDevToolsAction::UsesHttpClient()
{
    return false;
}

bool IDevToolsAction::FilterActionSaver(std::shared_ptr<IDevToolsAction> Action)
{
    return true;
}

IDevToolsAction::ActionSaverBehavior IDevToolsAction::GetActionSaverBehavior()
{
    return DontSave;
}

bool IDevToolsAction::IsNeedToRunForAllActiveTabs()
{
    return false;
}

bool IDevToolsAction::IsNeedToRunForAllActiveFrames()
{
    return false;
}

void IDevToolsAction::OnTabCreation()
{
    Run();
}

void IDevToolsAction::OnTabSwitching()
{
    Run();
}

void IDevToolsAction::OnRunnningForAllRunActiveTab()
{
    Run();
}

void IDevToolsAction::CloneTo(IDevToolsAction* Object)
{
    Object->Params = Params;
    Object->Timeout = Timeout;
}

std::string IDevToolsAction::GetStringFromJson(const std::string& Json, const std::string& Path, const std::string& Default)
{
    return Parser.GetStringFromJson(Json, Path, Default);
}

bool IDevToolsAction::GetBooleanFromJson(const std::string& Json, const std::string& Path, bool Default)
{
    return Parser.GetBooleanFromJson(Json, Path, Default);
}

double IDevToolsAction::GetFloatFromJson(const std::string& Json, const std::string& Path, float Default)
{
    return Parser.GetFloatFromJson(Json, Path, Default);
}

std::string IDevToolsAction::GetActualTabId()
{
    if(!OverrideDefaultTabId.empty())
    {
        return OverrideDefaultTabId;
    }

    return GlobalState->TabId;
}

std::shared_ptr<TabData> IDevToolsAction::FindTabById(const std::string& TabId)
{
    for(std::shared_ptr<TabData> Tab : GlobalState->Tabs)
    {
        if(Tab->TabId == TabId)
        {
            return Tab;
        }
    }
    return nullptr;
}

std::string IDevToolsAction::GetDefaultTabId()
{
    if(!OverrideDefaultTabId.empty())
    {
        return OverrideDefaultTabId;
    }

    return GlobalState->TabId;
}


void IDevToolsAction::SendWebSocket(const std::string& Method,const std::map<std::string,Variant>& Params, std::string TabId)
{
    picojson::value::object Data;
    picojson::value::object ParamsObject = Serializer.SerializeObjectToObject(Params);

    Data["id"] = picojson::value((double)Id);
    Data["method"] = picojson::value(Method);
    Data["params"] = picojson::value(ParamsObject);
    if(TabId == "CurrentTab")
    {
        Data["sessionId"] = picojson::value(GetDefaultTabId());
    } else
    {
        Data["sessionId"] = picojson::value(TabId);
    }

    std::string DataString = picojson::value(Data).serialize();

    GlobalState->WebSocketClient->Send(DataString);
}

