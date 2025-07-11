#ifndef WEBDRIVERPROCESSCOMUNICATORFACTORY_H
#define WEBDRIVERPROCESSCOMUNICATORFACTORY_H
#include "engine_global.h"
#include "iprocesscomunicatorfactory.h"
#include "ihttpclientfactory.h"
#include <QProcess>
#include <QMap>


namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT WebDriverProcessComunicatorFactory : public IProcessComunicatorFactory
    {
        Q_OBJECT
        struct WebDriverUnit
        {
            QProcess *WebDriverProcess = 0;
            int Port;
            enum ProcessStateEnum
            {
                NotStarted,
                Starting,
                Working
            }ProcessState = NotStarted;

            enum PendingTypeEnum
            {
                NewSession
            };

            struct PendingUnit
            {
                int Id;
                QStringList Arguments;
                PendingTypeEnum Type;
            };

            QList<PendingUnit> Pending;
            QString BrowserEngine;

        };
        QMap<QString,WebDriverUnit> Drivers;
        IHttpClientFactory *HttpClientFactory;
        QString MLALocation;
        QString MLAToken;
        int TimesCheckedMlaPort;
        int TimesCheckedMetainfoIsUpdated;
        QStringList MlaProfilesAutoclear;
        QString ApiLocation;
        QString SessionIdParameterName;
        QString PortParameterName;
        int MLAPort;
        bool IsCustomDriver;
        ILogger * Logger;
        bool IsRecord;
        QString RecordId;

        bool IsProcessRunning(int pid);
        QString GetRandomString();

    public:
        explicit WebDriverProcessComunicatorFactory(QObject *parent = 0);
        ~WebDriverProcessComunicatorFactory();
        virtual IProcessComunicator * CreateProcessComunicator(const QString& BrowserEngine);
        void SetHttpClientFactory(IHttpClientFactory *HttpClientFactory);
        virtual void SetLogger(ILogger * Logger);
        void SetIsRecord();
        void SetRecordId(const QString& RecordId);



    signals:
        void SessionCreated(int, const QString&, int, bool, const QString&, bool, const QString&);
        void LockProfile(int, const QString&);
    private slots:
        void NewSession(QStringList arguments, QString BrowserEngine, int CommunicatorId);
        void ProcessStarted();
        void CreateNewSessionFor(const QStringList& Arguments, const QString& BrowserEngine, int CommunicatorId);
        void SessionCreateResult();
        void Log(const QString& Text,const QString& Arg1 = QString(),const QString& Arg2 = QString(),const QString& Arg3 = QString());
        void MLAGetMeta(const QString& ProfileId, int CommunicatorId);
        void MLAGetMetaResult();
        void MlaCreateProfile(const QStringList& Arguments, const QString& BrowserEngine, int CommunicatorId);
        QVariantMap GenerateMlaSettings(const QStringList& Arguments, bool WithProxy);

        //Local profile
        void MlaCreateLocalFingerprint(const QStringList& Arguments, const QString& BrowserEngine, int CommunicatorId, const QString& ProfilePath);
        void MlaCreateLocalFingerprintResult();
        void MlaCreateLocalProfile(const QStringList& Arguments, const QString& BrowserEngine, int CommunicatorId, const QString& ProfilePath, const QString& FingerprintId);
        void MlaCreateLocalProfileResult();
        void MLAGetMetaForLocalProfile(const QString& ProfileId, int CommunicatorId, const QString& ProfilePath, const QString& FingerprintId);
        void MLAGetMetaForLocalProfileResult();
        void MLAAutoDeleteOldProfiles();
        void MLAAutoDeleteOldProfilesResult();

        void MlaCreateOnlineProfile(const QStringList& Arguments, const QString& BrowserEngine, int CommunicatorId, const QString& ProfileName);
        void MlaCreateOnlineProfileResult();
        void CreateNewMLASessionFor(const QString& BrowserEngine, int CommunicatorId, const QString& ProfileId);
        void MLASessionCreateResult();
        void ScheduleCheckIfMetainfoIsUpdated();
        void CheckIfMetainfoIsUpdated();
        void ScheduleCheckIfMLAPortOpen();
        void CheckIfMLAPortOpen();
        void CheckMLAPortResult();
        void ExtractMLAToken();
        void DetectMLAPort();
        bool SearchForMultiloginMetainfo(bool& IsCanConnect,bool& IsNoInfo);
        void StartMLAProcess();
        void CheckIfMLAPortOpenFirstTimeAfterNoInfo();
        void CheckMLAPortFirstTimeResultAfterNoInfo();
        void CheckIfMLAPortOpenFirstTimeAfterCanConnect();
        void CheckMLAPortFirstTimeResultAfterCanConnect();
        void CreateMLASessions();
        void CheckStatusResponce(IHttpClient * HttpClient);
        void UpdateDriverPath();

        void FailMLAProcessStart(const QString& Message);
        void FailMLASessionStart(const QString& Message, int CommunicatorId);

    };
}

#endif // WEBDRIVERPROCESSCOMUNICATORFACTORY_H
