#ifndef RECORDPROCESSCOMMUNICATION_H
#define RECORDPROCESSCOMMUNICATION_H

#include "engine_global.h"

#include <QObject>
#include "irecordprocesscommunication.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT RecordProcessCommunication : public IRecordProcessCommunication
    {
        Q_OBJECT
        IProcessComunicator *Comunicator;
        bool CanSend;
        bool CanSendIsChanged;
        QString SendData;
        QString SendDataSchema;
        QString SendEmbeddedData;
        QString SendInterfaceState;
        QString SendScriptEngineVersion;
        QString SendApplicationEngineVersion;
        bool IsPlayingScript;

        QString SendResourcesString;
        QString Window;
        bool IsTesting;

        int RunTaskResultId = -1;
        QString RunTaskFunction;
        QString RunTaskParameters;
        QString ConvertResources(const QList<ResourceDescription>& Resources);
    public:

        explicit RecordProcessCommunication(QObject *parent = 0);

    signals:

    private slots:
        void ReceivedData(const QString& text);
        void SetCanSendDataTrue();
        void SetCanSendDataFalse();
    public slots:
        virtual void SendIsChanged();
        virtual void StopRecorder();
        virtual void SendCode(const QString& Code,const QString& Schema, const QString& EmbeddedLanguageData, bool IsTesting, const QString& InterfaceState, const QString& ScriptEngineVersion, const QString& ApplicationEngineVersion);
        virtual void HighlightAction(const QString& ActionId);
        virtual void SetWindow(const QString& Window);
        virtual void SendResources(const QList<ResourceDescription>& Resources);
        virtual void InstallProcessComunicator(IProcessComunicator *Comunicator);
        virtual void Closed();
        virtual void RestoreOriginalStage();
        virtual void CheckResult(QString CheckId,bool IsSuccess,QString ErrorString);
        void RunTaskInRecordMode(int ResultId, QString Function, QString Parameters);
        virtual void PrepareFunction(QString FunctionName);
        virtual void OnRecord();
        virtual void OnRun();
        virtual void BackupDone(QString FullPath);
    };
}

#endif // RECORDPROCESSCOMMUNICATION_H
