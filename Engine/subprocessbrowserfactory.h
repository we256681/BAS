#ifndef SUBPROCESSBROWSERFACTORY_H
#define SUBPROCESSBROWSERFACTORY_H
#include "engine_global.h"

#include "ibrowserfactory.h"
#include "iprocesscomunicatorfactory.h"
#include "subprocesslistwidget.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT SubprocessBrowserFactory : public IBrowserFactory
    {
        Q_OBJECT

        bool IsUIDisabled;
        QList<IBrowser*> BrowserList;
        QList<IBrowser*> BrowserListUnused;

        INetworkAccessManagerFactory *NetworkAccessManagerFactory;
        IProcessComunicatorFactory *ProcessComunicatorFactory;
        SubprocessListWidget *Widget;
        QString Language;
        QString RecordId;
        IPCResourcesSmoothUsage *PCResourcesSmoothUsage;
    public:
        void SetLanguage(const QString & Language);
        void SetRecordId(const QString & RecordId);
        explicit SubprocessBrowserFactory(QObject *parent = 0);
        virtual void ClearAll();
        virtual void Clear(QList<IBrowser*>* Browsers);
        virtual QList<IBrowser*> Create(int size);
        virtual void SetNetworkAccessManager(INetworkAccessManagerFactory *NetworkAccessManagerFactory);
        virtual void SetPCResourcesSmoothUsage(IPCResourcesSmoothUsage *PCResourcesSmoothUsage);
        virtual IPCResourcesSmoothUsage *GetPCResourcesSmoothUsage();
        virtual INetworkAccessManagerFactory * GetNetworkAccessManager();
        virtual void DisableUI();
        void SetProcessComunicatorFactory(IProcessComunicatorFactory *ProcessComunicatorFactory);
        IProcessComunicatorFactory * GetProcessComunicatorFactory();
        QWidget *GetWidget(bool IsRecord);
    signals:

    private slots:
        void ProcessCreated(IProcessComunicator* Comunicator);
        void CommunicatorDestroyed();
    private:
        SubprocessListWidget* GetOrCreateWidget();

    };
}

#endif // SUBPROCESSBROWSERFACTORY_H
