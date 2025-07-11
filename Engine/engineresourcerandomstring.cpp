#include "engineresourcerandomstring.h"
#include "stringresourcehandler.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    EngineResourceRandomString::EngineResourceRandomString(QObject *parent) :
        EngineResourceAbstract(parent)
    {
    }
    void EngineResourceRandomString::SetStringBuilder(IStringBuilder *Builder)
    {
        this->Builder = Builder;
    }

    void EngineResourceRandomString::SetValue(const QString& Value)
    {
        this->Value = Value;
    }


    void EngineResourceRandomString::RemoveIndex(int index)
    {

    }
    int EngineResourceRandomString::Find(const QString& value)
    {
        return -1;
    }

    QList<int> EngineResourceRandomString::FindByKeys(const QStringList& keys)
    {
        return QList<int>();
    }



    IResourceHandler* EngineResourceRandomString::GetHandler(const QSet<QString>& refuse)
    {
        StringResourceHandler *handler = new StringResourceHandler();
        handler->SetString(Builder->Expand(Value));
        connect(handler,SIGNAL(DieSignal()),this,SLOT(Die()));
        connect(handler,SIGNAL(SuccessSignal()),this,SLOT(Success()));
        connect(handler,SIGNAL(FailSignal()),this,SLOT(Fail()));
        handler->SetHandlerStatus(IResourceHandler::Ready);

        return handler;
    }

    QList<QScriptValue> EngineResourceRandomString::GetAllData()
    {
        return QList<QScriptValue>();
    }
    QJsonValue EngineResourceRandomString::GetDescription()
    {
        QJsonValue Res = Value;
        return Res;
    }
    QList<QScriptValue> EngineResourceRandomString::GetRandomSubarrayData(int size)
    {
        return QList<QScriptValue>();
    }
    QScriptValue EngineResourceRandomString::GetAtIndex(int index)
    {
        return QScriptValue(QScriptValue::NullValue);
    }
    int EngineResourceRandomString::GetTotalLength()
    {
        return 0;
    }

    void EngineResourceRandomString::Success()
    {

    }
    void EngineResourceRandomString::Fail()
    {

    }
    void EngineResourceRandomString::Die()
    {

    }
    void EngineResourceRandomString::Reload()
    {

    }
    void EngineResourceRandomString::Clear()
    {
    }
    void EngineResourceRandomString::Insert(const QString& value, bool onlywrite)
    {

    }
    void EngineResourceRandomString::Sync()
    {

    }

    void EngineResourceRandomString::SyncNotForce()
    {

    }
    void EngineResourceRandomString::SetAtIndex(int index, const QString& val)
    {

    }
}
