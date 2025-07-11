#ifndef ENGINERESOURCEFIXEDINTEGER_H
#define ENGINERESOURCEFIXEDINTEGER_H
#include "engine_global.h"


#include "engineresourceabstract.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT EngineResourceFixedInteger : public EngineResourceAbstract
    {
        Q_OBJECT
        int Value;
        bool Active;
    public:
        explicit EngineResourceFixedInteger(QObject *parent = 0);
        void SetValue(int Value);


    signals:

    public slots:
        virtual IResourceHandler* GetHandler(const QSet<QString>& refuse);
        virtual QList<QScriptValue> GetAllData();
        virtual QJsonValue GetDescription();
        virtual QList<QScriptValue> GetRandomSubarrayData(int size);
        virtual QScriptValue GetAtIndex(int index);
        virtual void SetAtIndex(int index, const QString& val);
        virtual int GetTotalLength();
        virtual void Reload();

        virtual void Success();
        virtual void Fail();
        virtual void Die();
        virtual void Clear();
        virtual void Insert(const QString& value, bool onlywrite);
        virtual void Sync();
        virtual void SyncNotForce();
        virtual void RemoveIndex(int index);
        virtual int Find(const QString& value);
        virtual QList<int> FindByKeys(const QStringList& keys);

    };
}

#endif // ENGINERESOURCEFIXEDINTEGER_H
