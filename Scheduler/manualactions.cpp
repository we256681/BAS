#include "manualactions.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

ManualActions::ManualActions(QObject *parent) : QObject(parent)
{

}

void ManualActions::SetStream(Stream * _Stream)
{
    this->_Stream = _Stream;
}

void ManualActions::SetApiScript(ApiScript * _ApiScript)
{
    this->_ApiScript = _ApiScript;
}

void ManualActions::Add(
        const QString& ActionId,
        const QString& ScriptId,
        const QString& Type,
        const QString& Data
     )
{
    ManualAction Action;
    Action.ActionId = ActionId;
    Action.ScriptId = ScriptId;
    Action.Type = Type;
    Action.Data = Data;
    this->Data.insert(ActionId, Action);
    QJsonObject Object;
    Object.insert("id", ActionId);
    Object.insert("type", Type);
    Object.insert("data", Data);
    _Stream->Send("manual_add", Object);
}

void ManualActions::ScriptFinished(const QString& ScriptId)
{
    for(auto it = Data.begin(); it != Data.end();)
    {
        if(it.value().ScriptId == ScriptId)
        {
            QJsonObject SendManualRemove;
            SendManualRemove.insert("id",it.value().ActionId);
            _Stream->Send("manual_remove", SendManualRemove);
            it = Data.erase(it);
        }
        else
            ++it;
    }
}

void ManualActions::Answer(const QString& ActionId,const QString& Result, bool IsSuccess)
{
    if(!Data.contains(ActionId))
        return;

    //Send result to script

    QString Type = Data[ActionId].Type;
    QString ScriptId = Data[ActionId].ScriptId;
    if(Type == "Browser")
    {
        _ApiScript->SendMessageFormat(ScriptId,"ShowBrowser",QString(),"Id",ActionId);
    }else
    {
        _ApiScript->SendMessageFormat(ScriptId,"Manual",Result,"Type",Type,"Success",(IsSuccess) ? "true" : "false","Id",ActionId);
    }

    if(Type == "Browser")
    {
        //If manual control, wait while it will finish
    }else
    {
        //Send stream notification
        QJsonObject SendManualRemove;
        SendManualRemove.insert("id",ActionId);
        _Stream->Send("manual_remove", SendManualRemove);


        //Remove from data
        this->Data.remove(ActionId);
    }
}

void ManualActions::BrowserStop(const QString& ActionId)
{
    //Send stream notification
    QJsonObject SendManualRemove;
    SendManualRemove.insert("id",ActionId);
    _Stream->Send("manual_remove", SendManualRemove);


    //Remove from data
    this->Data.remove(ActionId);
}


QString ManualActions::Serialize()
{
    QJsonArray Result;

    for(ManualAction & Action:Data)
    {
        QJsonObject Item;
        Item.insert("id", Action.ActionId);
        Item.insert("type", Action.Type);
        Item.insert("data", Action.Data);
        Result.append(Item);
    }


    QJsonDocument doc(Result);
    return QString::fromUtf8(doc.toJson());
}
