#ifndef IEMBEDDEDLANGUAGEMANAGER_H
#define IEMBEDDEDLANGUAGEMANAGER_H

#include "engine_global.h"

#include <QObject>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "idiffpatcher.h"
#include "imodulemanager.h"
#include "EmbeddedDefinitions.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT IEmbeddedLanguageManager : public QObject
    {
        Q_OBJECT

    public:


        explicit IEmbeddedLanguageManager(QObject *parent = 0);
        virtual void ReadLanguageList(bool WriteLogs, bool IsSilent, bool HideInterface, QList<EmbeddedLanguage>& Languages, QList<EmbeddedModule>& Modules) = 0;
        virtual void ReadLanguageListLocal(QList<EmbeddedLanguage>& Languages, QList<EmbeddedModule>& Modules) = 0;
        virtual QList<EmbeddedCodeItem> GetEmbeddedCode() = 0;
        virtual void SetModuleCode(QList<EmbeddedCodeItem> AdditionalData) = 0;


        virtual void ReadData(const QString& EmbeddedData) = 0;
        virtual QString WriteData() = 0;

        virtual QString SerializeData() = 0;

        virtual void Stop() = 0;

        virtual bool WasError() = 0;
        virtual QString ErrorString() = 0;
        virtual bool IsRunWithoutLanguages() = 0;
        virtual void SetDiffPatcher(IDiffPatcher *DiffPatcher) = 0;
        virtual void ContinueEmbeddedLanguages() = 0;
        virtual void SetIsRecord(bool IsRecord) = 0;

        virtual QList<EmbeddedLanguage> GetMainLanguages() = 0;
        virtual QList<EmbeddedModule> GetMainModules() = 0;


    signals:
        void Started();
        /*Id,Success,ErrorString*/
        void CheckResult(QString,bool,QString);


        /* Function finished, id, variables json, is success, error message */
        void ReceivedResultData(quint64, QString, bool, QString);
        /* Need to call api, id, variables, api string */
        void ReceivedApiData(quint64, QString, QString);
        /* ConsoleLog */
        void ConsoleLog(const QString& Text,quint64 Id);

        /* Events, which is triggered during initialization */
        void InitializationShow();
        void InitializationHide();
        void InitializationLog(QString);
        void InitializationFail(QString);
        void InitializationTitle(QString);

    public slots:
        virtual void Start() = 0;

        virtual void DeserializeData(const QString& Data) = 0;

        /* Execute function */
        virtual quint64 SendFunction(const QString& Variables, const QString& Function, const QString& LanguageName, const QString& LanguageVersion, QString& StartErrorString) = 0;
        /* Send api result */
        virtual void SendApiResponce(const QString& Variables, quint64 Id, const QString& LanguageName, const QString& LanguageVersion) = 0;
        /* Send api result */
        virtual void KillTask(quint64 Id, const QString& LanguageName, const QString& LanguageVersion) = 0;

    };
}

#endif // IEMBEDDEDLANGUAGEMANAGER_H
