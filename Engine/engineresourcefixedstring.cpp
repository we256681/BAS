#include "engineresourcefixedstring.h"
#include "stringresourcehandler.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    EngineResourceFixedString::EngineResourceFixedString(QObject *parent) :
        EngineResourceAbstract(parent),Active(true)
    {
    }

    void EngineResourceFixedString::SetValue(const QString& Value)
    {
        this->Value = Value;
    }

    void EngineResourceFixedString::RemoveIndex(int index)
    {

    }
    int EngineResourceFixedString::Find(const QString& value)
    {
        return -1;
    }

    QList<int> EngineResourceFixedString::FindByKeys(const QStringList& keys)
    {
        return QList<int>();
    }


    IResourceHandler* EngineResourceFixedString::GetHandler(const QSet<QString>& refuse)
    {
        StringResourceHandler *handler = new StringResourceHandler();
        if(Active)
        {
            handler->SetString(Value);
            connect(handler,SIGNAL(DieSignal()),this,SLOT(Die()));
            connect(handler,SIGNAL(SuccessSignal()),this,SLOT(Success()));
            connect(handler,SIGNAL(FailSignal()),this,SLOT(Fail()));
            handler->SetHandlerStatus(IResourceHandler::Ready);
        }else
        {
            handler->SetHandlerStatus(IResourceHandler::NotAvailable);
        }
        return handler;
    }
    QList<QScriptValue> EngineResourceFixedString::GetAllData()
    {
        return QList<QScriptValue>() << QScriptValue(Value);
    }

    QJsonValue EngineResourceFixedString::GetDescription()
    {
        QJsonValue Res = Value;
        return Res;
    }
    QList<QScriptValue> EngineResourceFixedString::GetRandomSubarrayData(int size)
    {
        if(size > 0)
            return QList<QScriptValue>() << QScriptValue(Value);
        return QList<QScriptValue>();
    }
    QScriptValue EngineResourceFixedString::GetAtIndex(int index)
    {
        if(index == 0)
        {
            return QScriptValue(Value);
        }
        return QScriptValue(QScriptValue::NullValue);
    }
    int EngineResourceFixedString::GetTotalLength()
    {
        return 1;
    }

    void EngineResourceFixedString::Success()
    {

    }
    void EngineResourceFixedString::Fail()
    {

    }
    void EngineResourceFixedString::Die()
    {
        //Active = false;
    }
    void EngineResourceFixedString::Reload()
    {
        Active = true;
    }
    void EngineResourceFixedString::Clear()
    {
    }
    void EngineResourceFixedString::Insert(const QString& value, bool onlywrite)
    {
        this->Value = value;
    }

    void EngineResourceFixedString::Sync()
    {

    }

    void EngineResourceFixedString::SyncNotForce()
    {

    }

    void EngineResourceFixedString::SetAtIndex(int index, const QString& val)
    {

    }
}
