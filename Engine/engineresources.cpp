#include "engineresources.h"
#include "resourcemodelscript.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    EngineResources::EngineResources(QObject *parent) :
        IEngineResources(parent)
    {
    }

    void EngineResources::SetEngineResourcesController(IResourceController* EngineResourcesController)
    {
        this->EngineResourcesController = EngineResourcesController;
    }

    void EngineResources::SetResources(IResources* Resources)
    {
        this->Resources = Resources;
    }

    void EngineResources::Clear()
    {
        int len = Data.count();
        QList<IEngineResource *> values = Data.values();
        for(int i = 0;i<len;i++)
        {
            values.at(i)->deleteLater();
        }
        Data.clear();
    }

    void EngineResources::Add(const QString& Name, IEngineResource * Res)
    {
        Data[Name] = Res;
    }

    void EngineResources::Reload(const QString & name)
    {
        if(!Data.contains(name))
        {
            return;
        }else
        {
            Data[name]->Reload();
        }
    }

    void EngineResources::Clear(const QString & name)
    {
        if(!Data.contains(name))
        {
            return;
        }else
        {
            Data[name]->Clear();
        }
    }

    void EngineResources::Remove(const QString & name)
    {
        if(!Data.contains(name))
        {
            return;
        }else
        {
            Data[name]->deleteLater();
            Data.remove(name);
        }
    }

    void EngineResources::RemoveIndex(const QString & name,int index)
    {
        if(!Data.contains(name))
        {
            return;
        }else
        {
            Data[name]->RemoveIndex(index);
        }
    }

    int EngineResources::Find(const QString & name,const QString& value)
    {
        if(!Data.contains(name))
        {
            return -1;
        }else
        {
            return Data[name]->Find(value);
        }
    }

    QList<int> EngineResources::FindByKeys(const QString & name,const QStringList& keys)
    {
        if(!Data.contains(name))
        {
            return QList<int>();
        }else
        {
            return Data[name]->FindByKeys(keys);
        }
    }

    QList<QScriptValue> EngineResources::GetAllData(const QString & name)
    {
        if(!Data.contains(name))
        {
            return QList<QScriptValue>();
        }else
        {
            return Data[name]->GetAllData();
        }

    }

    QJsonValue EngineResources::GetDescription(const QString & name)
    {
        if(!Data.contains(name))
        {
            return QJsonValue::Null;
        }else
        {
            return Data[name]->GetDescription();
        }

    }
    QList<QScriptValue> EngineResources::GetRandomSubarrayData(const QString& name, int size)
    {
        if(!Data.contains(name))
        {
            return QList<QScriptValue>();
        }else
        {
            return Data[name]->GetRandomSubarrayData(size);
        }
    }
    QScriptValue EngineResources::GetAtIndex(const QString& name, int index)
    {
        if(!Data.contains(name))
        {
            return QString();
        }else
        {
            return Data[name]->GetAtIndex(index);
        }
    }

    void EngineResources::SetAtIndex(const QString& name, int index, const QString& val)
    {
        if(!Data.contains(name))
        {
            return;
        }else
        {
            Data[name]->SetAtIndex(index,val);
        }
    }

    int EngineResources::GetTotalLength(const QString& name)
    {
        if(!Data.contains(name))
        {
            return 0;
        }else
        {
            return Data[name]->GetTotalLength();
        }
    }

    QString EngineResources::GetInfo(const QString& Name, const QString& Key)
    {
        if(!Data.contains(Name))
        {
            return QString();
        }else
        {
            return Data[Name]->GetInfo(Key);
        }
    }

    IResourceHandler* EngineResources::GetHandler(const QString & name,const QSet<QString>& refuse)
    {
        if(!Data.contains(name))
        {
            return 0;
        }else
        {
            return Data[name]->GetHandler(refuse);
        }
    }

    QStringList EngineResources::GetNames()
    {
        return Data.keys();
    }

    void EngineResources::Insert(const QString & name,const QString & value, bool onlywrite)
    {
        if(!Data.contains(name))
        {
            return;
        }else
        {
            Data[name]->Insert(value, onlywrite);
        }
    }

    void EngineResources::Sync(const QString & name)
    {
        if(!Data.contains(name))
        {
            return;
        }else
        {
            Data[name]->Sync();
        }
    }

    void EngineResources::SyncNotForce()
    {
        for(IEngineResource * Res:Data.values())
        {
            Res->SyncNotForce();
        }
    }

    void EngineResources::Create(const QString& name, int successes, int fails, int simultaneous, int interval, bool greedy, bool dont_give_up)
    {
        QList<IResourceModel * > * list = Resources->GetModelList();
        QList<IResourceModel * > copy;
        copy.append(*list);
        list->clear();
        ResourceModelScript m;
        m.SetName(name);
        m.SetSuccessAttempts(successes);
        m.SetFailAttempts(fails);
        m.SetNumberSimultaneousUse(simultaneous);
        m.SetIntervalBetweenUsage(interval);
        m.SetGreedy(greedy);
        m.SetDontGiveUp(dont_give_up);
        list->append(&m);
        EngineResourcesController->FromModelToView(Resources, false);
        list->clear();
        list->append(copy);
    }
}
