#ifndef PIPESPROCESSCOMUNICATOR_H
#define PIPESPROCESSCOMUNICATOR_H
#include "engine_global.h"
#include "iprocesscomunicator.h"
#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QProcess>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT PipesProcessComunicator : public IProcessComunicator
    {
        Q_OBJECT
        QString key;
        QString Location;
        QProcess *Process;
        bool isError;
        QString errorString;
        QStringList Params;
        QStringList Arguments;
        bool IsRecord;
        bool IsTemporaryProfile;

    public:
        explicit PipesProcessComunicator(QObject *parent = 0);
        virtual void SetLocation(const QString& Location);
        virtual void SetRecord(bool IsRecord);
        void SetCommandLineParams(const QStringList& Params);

        virtual void CreateProcess(const QStringList& arguments, bool IsTemporaryProfile);
        virtual void ConnectToProcess(const QString& key_in, const QString& key_out);
        virtual void Send(const QString& value);
        virtual void Abort();
        virtual void AbortByUser();
        virtual bool IsError();
        virtual QString ErrorString();
        virtual bool IsFail();
        virtual QString FailString();
        virtual bool IsDie();
        virtual QString DieString();
        virtual bool HasProcess();
        virtual qint64 GetPID();
        virtual void SetGeneralTimeout(int Timeout);

        virtual bool GetIsOnlineProfile();
        virtual QString GetProfileId();

        virtual void OnShowBrowser();
        virtual void OnHideBrowser();

    public slots:
        virtual void Suspend();
        void ReceivedAll(const QString& key, const QString& data);
        void ProcessStartedAll(const QString& key);
        void ProcessStoppedAll(const QString& key);
    signals:
        void SendSignal(const QString& key, const QString& data);
    private slots:
        void UnexpectedFinished();

    };
}

#endif // PIPESPROCESSCOMUNICATOR_H
