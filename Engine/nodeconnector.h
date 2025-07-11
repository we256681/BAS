#ifndef NODECONNECTOR_H
#define NODECONNECTOR_H
#include "engine_global.h"
#include "ilanguageconnector.h"
#include <QLocalServer>
#include <QLocalSocket>
#include <QSharedPointer>
#include <QProcess>
#include <QFile>
#include "httpclient.h"
#include <QObject>
#include <QTimer>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT NodeConnector : public ILanguageConnector
    {
        Q_OBJECT

        QSharedPointer<QLocalServer> Server;
        QSharedPointer<QProcess> Process;
        QSharedPointer<QProcess> NpmInstallProcess;
        QLocalSocket *Client;
        bool IsActive = false;
        bool IsProcessRestart = false;
        bool NoNeedRestartProcess = false;
        QList<QString> SendQueue;
        QString DataAll;
        QSharedPointer<QFile> NodeExeLock;
        bool LogEnabled = false;
        bool IsRecord = false;
        bool HasPipeVersion = false;
        bool AutoCleanWhenFinishedSuccess = false;
        bool AutoCleanWhenFinishedFail = false;

        QStringList DistrAlreadyTried;

        QString Suffix;

        /* Language Settings*/
        QHash<QString,QString> Modules;
        QHash<QString,QString> Functions;
        QHash<QString,QString> Files;
        QString LanguageVersion;

        QSharedPointer<HttpClient> _HttpClient;

        /* FinalizeInstall */
        void FinalizeInstall(bool IsError, const QString& Message, bool RemoveZip = true, const QString& IntefaceMessage = QString(), bool AllowRetry = true);
        bool FinalizeInstallIsError;
        bool FinalizeInstallAllowRetry;
        QString FinalizeInstallMessage;
        QString FinalizeInstallInterfaceMessage;

        /* Timeout */
        QTimer *Timer = 0;
        bool NewProcessWaitTimeout = false;
    public:
        explicit NodeConnector(QObject *parent = 0);
        ~NodeConnector();

    private slots:
        void NewConnection();
        void ProcessFinished();
        void StartProcess();
        void StartPipes();
        void ReadData();
        void Write(const QString& Text);
        void SendRaw(const QString& Text);
        quint64 GenerateId();
        QString GetLanguageSettingsHash();
        QString GetExecutableLocationForNode();
        QString GetExecutableLocationForMain();
        void StartInternal();
        void DistrDownloaded();
        void ExtractDistr();
        bool IsX64();
        void NpmInstalled(int StatusCode);
        /*Returns is success*/
        bool DeleteFunctionsAndFiles(const QString& NodePath);
        bool InstallFunctionsAndFiles(const QString& NodePath);
        QString FindInstalledDistr();
        void LOG(const QString& Text);
        void LOGINTERFACE(const QString& Text);
        QStringList AutoCleanPrepare();
        QString GetRandomString();
        QString A(const QString& RelativePath);
        void OnDistrExtracted();
        void OnFolderMoved();
        void OnFinalizeInstall();
        void OnChecksumsCalculated();
        void OnInstallDirChecksum();
        void FailedToStartNpm(QProcess::ProcessError error);
        void FailedToStartNode(QProcess::ProcessError error);
        void InstalledDirFinishedSearch();
        void InstalledDirSearchIteration();
        void StartProcessTimeout();

    public:

        /* Language Settings*/
        virtual QString GetLanguageName();
        virtual void SetLanguageVersion(const QString& Version);
        virtual QString GetLanguageVersion();
        virtual void AddModule(const QString& Name,const QString& Version);
        virtual QHash<QString,QString> GetModules();
        virtual void AddFunction(const QString& Name,const QString& Code);
        virtual QHash<QString,QString> GetFunctions();
        virtual void AddFile(const QString& Path,const QString& Code);
        virtual QHash<QString,QString> GetFiles();
        virtual void ClearLanguageSettings();

        /* Other settings*/
        void SetHasPipeVersion();
        void SetAutoCleanWhenFinishedSuccess();
        void SetAutoCleanWhenFinishedFail();
    signals:

    public slots:
        /* Start process */
        virtual void Start();
        /* Stop process */
        virtual void Stop();
        /* Execute function */
        virtual quint64 SendFunction(const QString& Variables, const QString& Function);
        /* Send api result */
        virtual void SendApiResponce(const QString& Variables, quint64 Id);
        /* Kill task, in case if thread ended */
        virtual void KillTask(quint64 Id);
        /* Get statistic*/
        virtual void GetStats();
        virtual void EnableLog(bool LogEnabled);
        virtual void SetIsRecord(bool IsRecord);



    };
}

#endif // NODECONNECTOR_H
