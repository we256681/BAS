#ifndef ENGINERESOURCESTRINGBOX_H
#define ENGINERESOURCESTRINGBOX_H
#include "engine_global.h"

#include "engineresourceabstract.h"
#include "istringbox.h"
#include "istringboxproxy.h"
#include "idatabaseconnector.h"
#include "icsvhelper.h"


namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT EngineResourceStringBox : public EngineResourceAbstract
    {
        Q_OBJECT
        IStringBox* Box;
        IStringBoxProxy* StringBoxProxy;
        IDatabaseConnector* DatabaseConnector;
        int TableId;
        ICsvHelper *_CsvHelper;
        QList<int> ColumnIds;
    public:
        explicit EngineResourceStringBox(QObject *parent = 0);
        void SetValue(IStringBox* Box);
        void SetStringBoxProxy(IStringBoxProxy* StringBoxProxy);
        void SetDatabase(IDatabaseConnector* DatabaseConnector, int TableId, ICsvHelper *_CsvHelper, const QList<int>& ColumnIds);


    signals:
        void Synced();
        void SyncedNotForce();
    public slots:
        virtual IResourceHandler* GetHandler(const QSet<QString>& refuse);
        virtual QList<QScriptValue> GetAllData();
        virtual QJsonValue GetDescription();
        virtual QList<QScriptValue> GetRandomSubarrayData(int size);
        virtual QScriptValue GetAtIndex(int index);
        virtual void SetAtIndex(int index, const QString& val);

        virtual int GetTotalLength();

        virtual void Success();
        virtual void Fail();
        virtual void Die();
        virtual void Purge();
        virtual void Clear();
        virtual void Update(const QStringList &);
        virtual void Reload();
        virtual void Insert(const QString& value, bool onlywrite);
        virtual void Sync();
        virtual void SyncNotForce();

        virtual void RemoveIndex(int index);
        virtual int Find(const QString& value);
        virtual QList<int> FindByKeys(const QStringList& keys);


    };
}

#endif // ENGINERESOURCESTRINGBOX_H
