#ifndef ILANGUAGECONNECTOR_H
#define ILANGUAGECONNECTOR_H
#include "engine_global.h"

#include <QObject>
namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT ILanguageConnector : public QObject
    {
        Q_OBJECT
    public:
        explicit ILanguageConnector(QObject *parent = 0);

    public:

        /* Language Settings*/
        virtual QString GetLanguageName() = 0;
        virtual void SetLanguageVersion(const QString& Version) = 0;
        virtual QString GetLanguageVersion() = 0;
        virtual void AddModule(const QString& Name,const QString& Version) = 0;
        virtual QHash<QString,QString> GetModules() = 0;
        virtual void AddFunction(const QString& Name,const QString& Code) = 0;
        virtual QHash<QString,QString> GetFunctions() = 0;
        virtual void AddFile(const QString& Path,const QString& Code) = 0;
        virtual QHash<QString,QString> GetFiles() = 0;
        virtual void ClearLanguageSettings() = 0;


    /* API */
    signals:
        /* IsError, ErrorMessage, AllowRetry */
        void Started(bool, QString, bool);
        /* ErrorMessage */
        void Stopped(QString);
        /* Function finished, id, variables json, is success, error message */
        void ReceivedResultData(quint64, QString, bool, QString);
        /* Need to call api, id, variables, api string */
        void ReceivedApiData(quint64, QString, QString);
        /* Number of functions working, number of api pending*/
        void ReceivedStats(int, int);
        /* Log */
        void Log(QString);
        /* LogConsole */
        void LogConsole(QString, quint64);
        /* Log to file, containes raw messages, not recomended to show in ui */
        void LogFile(QString);

    public slots:
        /* Start process */
        virtual void Start() = 0;
        /* Stop process */
        virtual void Stop() = 0;
        /* Execute function */
        virtual quint64 SendFunction(const QString& Variables, const QString& Function) = 0;
        /* Send api result */
        virtual void SendApiResponce(const QString& Variables, quint64 Id) = 0;
        /* Kill task, in case if thread ended */
        virtual void KillTask(quint64 Id) = 0;
        /* Get statistic*/
        virtual void GetStats() = 0;
        virtual void EnableLog(bool LogEnabled) = 0;

    };
}

#endif // ILANGUAGECONNECTOR_H

