#ifndef ENGINERESOURCEFIXEDSTRING_H
#define ENGINERESOURCEFIXEDSTRING_H
#include "engine_global.h"

#include "engineresourceabstract.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT EngineResourceFixedString : public EngineResourceAbstract
    {
        Q_OBJECT
        QString Value;
        bool Active;
    public:
        explicit EngineResourceFixedString(QObject *parent = 0);
        void SetValue(const QString& Value);


    signals:

    public slots:
        virtual void Success();
        virtual void Fail();
        virtual void Die();
        virtual IResourceHandler* GetHandler(const QSet<QString>& refuse);
        virtual QList<QScriptValue> GetAllData();
        virtual QJsonValue GetDescription();

        virtual QList<QScriptValue> GetRandomSubarrayData(int size);
        virtual QScriptValue GetAtIndex(int index);
        virtual void SetAtIndex(int index, const QString& val);

        virtual int GetTotalLength();


        virtual void Reload();
        virtual void Insert(const QString& value, bool onlywrite);
        virtual void Sync();
        virtual void SyncNotForce();
        virtual void Clear();
        virtual void RemoveIndex(int index);
        virtual int Find(const QString& value);
        virtual QList<int> FindByKeys(const QStringList& keys);

    };
}

#endif // ENGINERESOURCEFIXEDSTRING_H
