#include "subprocessbrowserfactory.h"
#include "subprocessbrowser.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    SubprocessBrowserFactory::SubprocessBrowserFactory(QObject *parent) :
        IBrowserFactory(parent)
    {
        Widget = 0;
        Language = "en";
        IsUIDisabled = false;
    }

    void SubprocessBrowserFactory::DisableUI()
    {
        IsUIDisabled = true;
    }


    SubprocessListWidget* SubprocessBrowserFactory::GetOrCreateWidget()
    {
        if(!Widget)
            Widget = new SubprocessListWidget();

        return Widget;
    }

    QWidget *SubprocessBrowserFactory::GetWidget(bool IsRecord)
    {
        if(IsUIDisabled)
            return 0;
        GetOrCreateWidget()->SetAutoStart(IsRecord);
        GetOrCreateWidget()->SetBrowsersHidden(IsRecord);
        return GetOrCreateWidget();
    }

    void SubprocessBrowserFactory::ClearAll()
    {
        foreach(IBrowser* b,BrowserList)
            b->deleteLater();

        BrowserList.clear();
        BrowserListUnused.clear();
        if(!IsUIDisabled)
            GetOrCreateWidget()->Clear();
    }

    void SubprocessBrowserFactory::Clear(QList<IBrowser*>* Browsers)
    {
        for(IBrowser *Browser:*Browsers)
        {
            if(!BrowserListUnused.contains(Browser))
                BrowserListUnused.append(Browser);
        }
    }


    QList<IBrowser*> SubprocessBrowserFactory::Create(int size)
    {
        QList<IBrowser*> res;

        for(int i = 0;i<size;i++)
        {
            if(!BrowserListUnused.empty())
            {
                res.append(BrowserListUnused.first());
                BrowserListUnused.removeFirst();
                continue;
            }
            SubprocessBrowser * o = new SubprocessBrowser(this);
            o->SetPCResourcesSmoothUsage(PCResourcesSmoothUsage);
            o->SetLanguage(Language);
            o->SetRecordId(RecordId);
            connect(o,SIGNAL(ProcessCreated(IProcessComunicator*)), this, SLOT(ProcessCreated(IProcessComunicator*)));
            o->SetNetworkAccessManagerFactory(NetworkAccessManagerFactory);
            o->SetProcessComunicatorFactory(ProcessComunicatorFactory);
            BrowserList.append(o);
            res.append(o);
        }
        return res;
    }

    void SubprocessBrowserFactory::SetLanguage(const QString & Language)
    {
        this->Language = Language;
    }

    void SubprocessBrowserFactory::SetRecordId(const QString & RecordId)
    {
        this->RecordId = RecordId;
    }

    void SubprocessBrowserFactory::ProcessCreated(IProcessComunicator* Comunicator)
    {
        disconnect(Comunicator,SIGNAL(OnSuspend()),this,SLOT(CommunicatorDestroyed()));
        connect(Comunicator,SIGNAL(OnSuspend()),this,SLOT(CommunicatorDestroyed()));
        disconnect(Comunicator,SIGNAL(ProcessRestored(IProcessComunicator*)),this,SLOT(ProcessCreated(IProcessComunicator*)));
        connect(Comunicator,SIGNAL(ProcessRestored(IProcessComunicator*)),this,SLOT(ProcessCreated(IProcessComunicator*)));

        emit OnBrowserCreated();

        int index = -1;
        if(qobject_cast<IBrowser*>(sender()))
        {
            index = BrowserList.indexOf(qobject_cast<IBrowser*>(sender()));
            emit BrowserStart(Comunicator, index);

            Comunicator->setProperty("ThreadNumber", index);
        }else
        {
            index = Comunicator->property("ThreadNumber").toInt();
            emit BrowserStart(Comunicator, index);
        }

        if(!IsUIDisabled)
            GetOrCreateWidget()->Add(Comunicator,index);
    }

    void SubprocessBrowserFactory::CommunicatorDestroyed()
    {
        IProcessComunicator* Comunicator = qobject_cast<IProcessComunicator*>(sender());
        if(!Comunicator)
            return;
        int index = Comunicator->property("ThreadNumber").toInt();
        emit BrowserStop(Comunicator, index);

    }


    void SubprocessBrowserFactory::SetNetworkAccessManager(INetworkAccessManagerFactory *NetworkAccessManagerFactory)
    {
        this->NetworkAccessManagerFactory = NetworkAccessManagerFactory;
    }

    void SubprocessBrowserFactory::SetPCResourcesSmoothUsage(IPCResourcesSmoothUsage *PCResourcesSmoothUsage)
    {
        this->PCResourcesSmoothUsage = PCResourcesSmoothUsage;
    }

    IPCResourcesSmoothUsage *SubprocessBrowserFactory::GetPCResourcesSmoothUsage()
    {
        return PCResourcesSmoothUsage;
    }

    INetworkAccessManagerFactory * SubprocessBrowserFactory::GetNetworkAccessManager()
    {
        return NetworkAccessManagerFactory;
    }

    void SubprocessBrowserFactory::SetProcessComunicatorFactory(IProcessComunicatorFactory *ProcessComunicatorFactory)
    {
        this->ProcessComunicatorFactory = ProcessComunicatorFactory;
    }

    IProcessComunicatorFactory * SubprocessBrowserFactory::GetProcessComunicatorFactory()
    {
        return ProcessComunicatorFactory;
    }

}
