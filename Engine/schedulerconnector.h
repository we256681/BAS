#ifndef SCHEDULERCONNECTOR_H
#define SCHEDULERCONNECTOR_H
#include "engine_global.h"

#include <QObject>
#include <QLocalSocket>
#include <QEventLoop>

#include "ischedulerconnector.h"
#include "imultiworker.h"
#include "ireportdatavisualizer.h"
#include <QTranslator>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT SchedulerConnector : public ISchedulerConnector
    {
        Q_OBJECT
        QLocalSocket *Socket = 0;
        bool IsActive = false;
        QByteArray Data;
        QEventLoop ConnectLoop;
        IMultiWorker *Worker = 0;
        IReportDataVisualizer *ReportDataVisualizer = 0;
        QMap<int, IProcessComunicator *> CommunicatorMap;

    public:
        explicit SchedulerConnector(QObject *parent = nullptr);
        virtual void StartIfNeeded(const QStringList& Params);
        virtual void Start(const QString& Id);
        virtual bool GetIsActive();
        virtual void MessageSend(const QString& Message);
        virtual void MessageSendFormat(const QString& TagName, const QString& Content = QString(),
                                       const QString& AttributeName1 = QString(), const QString& AttributeData1 = QString(),
                                       const QString& AttributeName2 = QString(), const QString& AttributeData2 = QString(),
                                       const QString& AttributeName3 = QString(), const QString& AttributeData3 = QString());

        void SetWorker(IMultiWorker *Worker);
        void SetBrowserFactory(IBrowserFactory *BrowserFactory);
        void SetReportDataVisualizer(IReportDataVisualizer *ReportDataVisualizer);

        void Reset();

    signals:

    public slots:
    private slots:
        void Income();
        void ConnectDelayed();
        void Connected();
        void ConnectTimeout();
        void MessageReceived(QString TagName, QString Content, QMap<QString,QString> Params);

        //WORKER
        void ThreadStarted();
        void ThreadFail();
        void ThreadSuccess();
        void StageStatistic(int MaxSuccess, int MaxFails);

        //BROWSER FACTORY
        void BrowserStart(IProcessComunicator*,int);
        void BrowserStop(IProcessComunicator*,int);
        void Received(const QString& value);

    private:
        void ParseMessage(const QString& Message);

    };
}

#endif // SCHEDULERCONNECTOR_H
