#ifndef IPROCESSCOMUNICATOR_H
#define IPROCESSCOMUNICATOR_H
#include "engine_global.h"

#include <QObject>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT IProcessComunicator : public QObject
    {
        Q_OBJECT

    public:
        explicit IProcessComunicator(QObject *parent = 0);
        virtual void SetLocation(const QString& Location) = 0;
        virtual void SetRecord(bool IsRecord) = 0;
        virtual void CreateProcess(const QStringList& arguments, bool IsTemporaryProfile) = 0;
        virtual void ConnectToProcess(const QString& key_in, const QString& key_out) = 0;
        virtual void Send(const QString& value) = 0;
        virtual void Abort() = 0;
        virtual void AbortByUser() = 0;
        virtual bool IsError() = 0;
        virtual QString ErrorString() = 0;

        //Should thread fail
        virtual bool IsFail() = 0;
        virtual QString FailString() = 0;

        //Should script die
        virtual bool IsDie() = 0;
        virtual QString DieString() = 0;

        virtual bool HasProcess() = 0;
        virtual qint64 GetPID() = 0;
        virtual void SetGeneralTimeout(int Timeout) = 0;

        //Profiles
        virtual bool GetIsOnlineProfile() = 0;
        virtual QString GetProfileId() = 0;

        virtual void OnShowBrowser() = 0;
        virtual void OnHideBrowser() = 0;

    signals:
        void Received(const QString& value);
        void ShowBrowser();
        void HideBrowser();
        void ProcessStarted();
        void ProcessFinished();
        void Error();
        void OnSuspend();
        void ProcessRestored(IProcessComunicator *);

    public slots:
        virtual void Suspend() = 0;
    };
}

#endif // IPROCESSCOMUNICATOR_H
