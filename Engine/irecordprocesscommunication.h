#ifndef IRECORDPROCESSCOMMUNICATION_H
#define IRECORDPROCESSCOMMUNICATION_H

#include "engine_global.h"

#include <QObject>
#include <QList>
#include "iprocesscomunicator.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT IRecordProcessCommunication : public QObject
    {
        Q_OBJECT
    public:
        struct ResourceDescription
        {
          QString Name;
          QString Description;
          QString Type;
        };
        explicit IRecordProcessCommunication(QObject *parent = 0);
    signals:
        void ReceiveCode(QString Code);
        void EmbeddedData(QString Code);
        void SaveInterface(QString Json);
        void LoadScript(QString FileName);
        void NeedRestart();
        void Interrupt();
        void Stop();
        void MaximizeWindow();
        void WindowAttached();
        void StartBackup();
        void RunTaskResult(int ResultId,QString Result);
        void PrepareFunctionResult(const QString& FunctionName, const QString& FunctionData);

    public slots:
        virtual void SendIsChanged() = 0;
        virtual void StopRecorder() = 0;
        virtual void SendCode(const QString& Code,const QString& Schema, const QString& EmbeddedLanguageData, bool IsTesting, const QString& InterfaceState, const QString& ScriptEngineVersion, const QString& ApplicationEngineVersion) = 0;
        virtual void HighlightAction(const QString& ActionId) = 0;
        virtual void SendResources(const QList<ResourceDescription>& Resources) = 0;
        virtual void SetWindow(const QString& Window) = 0;
        virtual void InstallProcessComunicator(IProcessComunicator *Comunicator) = 0;
        virtual void Closed() = 0;
        virtual void RestoreOriginalStage() = 0;
        virtual void CheckResult(QString CheckId,bool IsSuccess,QString ErrorString) = 0;
        virtual void PrepareFunction(QString FunctionName) = 0;
        virtual void OnRecord() = 0;
        virtual void OnRun() = 0;
        virtual void BackupDone(QString FullPath) = 0;

    };
}

#endif // IRECORDPROCESSCOMMUNICATION_H
