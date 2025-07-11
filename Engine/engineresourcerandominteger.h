#ifndef ENGINERESOURCERANDOMINTEGER_H
#define ENGINERESOURCERANDOMINTEGER_H
#include "engine_global.h"

#include "engineresourceabstract.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT EngineResourceRandomInteger : public EngineResourceAbstract
    {
        Q_OBJECT
        int MinValue;
        int MaxValue;
        bool Active;
    public:
        explicit EngineResourceRandomInteger(QObject *parent = 0);

        void SetMinValue(int Value);
        void SetMaxValue(int Value);


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

#endif // ENGINERESOURCERANDOMINTEGER_H
