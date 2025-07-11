#ifndef EMBEDDEDLANGUAGEMANAGER_H
#define EMBEDDEDLANGUAGEMANAGER_H
#include "engine_global.h"

#include <QObject>
#include "iembeddedlanguagemanager.h"
#include "embeddedlanguageinstalldialog.h"
#include "ilanguageconnector.h"



namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT EmbeddedLanguageManager : public IEmbeddedLanguageManager
    {
        Q_OBJECT

        QList<EmbeddedCodeItem> AllData;
        QList<EmbeddedCodeItem> AdditionalData;

        QList<EmbeddedLanguage> AllLanguages;

        QList<EmbeddedModule> AllModules;

        IDiffPatcher *DiffPatcher;
        int LanguageStartIterator;
        bool WriteLogs;
        bool IsSilent;
        bool HideInterface;
        QList<ILanguageConnector*> Connectors;

        void StartNextLanguage();
        void MergeLanguage(EmbeddedLanguage& _Language);
        void MergeModule(EmbeddedModule& _Module);
        bool _WasError;
        QString _ErrorString;
        EmbeddedLanguageInstallDialog *_EmbeddedLanguageInstallDialog;
        QString DialogTitle;
        QString DialogLabel;
        bool IsActive;
        bool RunWithoutEmbeddedLanguages;

        /* Checking */
        QString CheckId;
        bool IsChecking;
        QString DataBeforeChecking;
        bool IsCheckingFail;
        QString CheckFailError;

        /* Language start retries */
        int MaxRetryNumber = 1;
        int CurrentRetryNumber = 0;

        bool IsRecord;

    public:
        explicit EmbeddedLanguageManager(QObject *parent = 0);
        ~EmbeddedLanguageManager();

        virtual void ReadLanguageList(bool WriteLogs, bool IsSilent, bool HideInterface, QList<EmbeddedLanguage>& Languages, QList<EmbeddedModule>& Modules);
        virtual void ReadLanguageListLocal(QList<EmbeddedLanguage>& Languages, QList<EmbeddedModule>& Modules);
        virtual QList<EmbeddedCodeItem> GetEmbeddedCode();
        virtual void SetModuleCode(QList<EmbeddedCodeItem> AdditionalData);


        virtual void ReadData(const QString& EmbeddedData);
        virtual QString WriteData();

        virtual QString SerializeData();
        virtual void Stop();


        virtual bool WasError();
        virtual QString ErrorString();
        virtual bool IsRunWithoutLanguages();
        virtual void SetDiffPatcher(IDiffPatcher *DiffPatcher);
        virtual void SetIsRecord(bool IsRecord);

        virtual QList<EmbeddedLanguage> GetMainLanguages();
        virtual QList<EmbeddedModule> GetMainModules();


    public slots:
        virtual void Start();

        virtual void DeserializeData(const QString& Data);
        virtual void NextLanguageStarted(bool IsError, QString ErrorMessage, bool AllowRetry);

        /* Execute function */
        virtual quint64 SendFunction(const QString& Variables, const QString& Function, const QString& LanguageName, const QString& LanguageVersion, QString& StartErrorString);
        /* Send api result */
        virtual void SendApiResponce(const QString& Variables, quint64 Id, const QString& LanguageName, const QString& LanguageVersion);
        /* Kill task, in case if thread ended */
        virtual void KillTask(quint64 Id, const QString& LanguageName, const QString& LanguageVersion);

        virtual void ContinueEmbeddedLanguages();


    private slots:
        void StartInternal();
        void Timeout();
        void ActivateWindow();
        void Cancel();
        void ConnectorStartingLog(QString);
        void CloseDialog();
        void Error(const QString& String);
        void Restart();

    signals:
        void IsRecordChanged(bool IsRecord);



    };
}

#endif // EMBEDDEDLANGUAGEMANAGER_H

