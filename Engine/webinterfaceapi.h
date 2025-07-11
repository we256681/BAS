#ifndef WEBINTERFACEAPI_H
#define WEBINTERFACEAPI_H
#include "engine_global.h"
#include "iwebinterface.h"
#include "imultiworker.h"
#include "ibrowserfactory.h"
#include "ireportdatavisualizer.h"
#include "iresourcesvisualizer.h"
#include "iprocesscomunicator.h"
#include "idatabasestate.h"
#include "idatabaseconnector.h"
#include <QObject>
#include <QMap>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT WebInterfaceApi : public QObject
    {
        Q_OBJECT
        IWebInterface* WebInterface;
        IMultiWorker *Worker;
        IReportDataVisualizer * ReportDataVisualizer;
        IResourcesVisualizer * ResourcesVisualizer;
        QString LastLogPath;
        QStringList LastResultPathes;
        IDatabaseState *DatabaseState;
        IDatabaseConnector *DatabaseConnector;
        bool SendHideBrowserEvent = false;
        bool IsFirstThreadStarted = false;
        bool IsPendingScriptStart = false;
        struct BrowserViewData
        {
          int RequestId = -1;
          int GetTabsRequestId = -1;
          int BrowserId = -1;
          IProcessComunicator *Communicator = 0;
          bool Active = false;
          bool GetTabsActive = false;
        };
        QMap<IProcessComunicator*, BrowserViewData> BrowserViewMap;
        QMap<int, IProcessComunicator *> CommunicatorMap;
        QWidget * MainWindow = 0;
        QString LastFolder = QString("../../../../");
    public:
        explicit WebInterfaceApi(QObject *parent = nullptr);
        void Init(IWebInterface *WebInterface);
        void SetMainWindow(QWidget * MainWindow);
        void ShowMainWindow();
        void SetWorker(IMultiWorker *Worker);
        void SetReportDataVisualizer(IReportDataVisualizer *ReportDataVisualizer);
        void SetResourcesVisualizer(IResourcesVisualizer *ResourcesVisualizer);
        void SetBrowserFactory(IBrowserFactory *BrowserFactory);
        void SetDatabaseState(IDatabaseState *DatabaseState);
        void SetDatabaseConnector(IDatabaseConnector *DatabaseConnector);
        QString GetDatabaseStructure();
        void Reset();
        void SetSendHideBrowserEvents();

    public slots:
        void NotifyAboutNewDatabaseStructure();
        void ScriptStarted();
    signals:
        void Restart();
        void AbortInstant();
        void AbortNotInstant();
        void ShowDatabase();
        void ShowScheduler();
        void InstallScheduler();
        void StartThread(int);
        void StopThread(int);
        void RunTaskInThread(int,int,QString,QString);
    private slots:
        void WebInterfaceEvalSignal(QString);
        void ReceivedMessage(QString, QVariantMap,bool,int);
        void ThreadStarted();
        void ThreadSuccess();
        void ThreadFail(const QString& Message);
        void BrowserStart(IProcessComunicator* Comunicator, int Index);
        void BrowserStop(IProcessComunicator* Comunicator, int Index);
        void Received(const QString& value);
        void ShowBrowser();
        void HideBrowser();
        void RunTaskResult(int Id, QString Result);
        void FileDialogFinished(int Result);

    };
}

#endif // WEBINTERFACEAPI_H
