#ifndef ANTIGATECAPTCHASOLVER_H
#define ANTIGATECAPTCHASOLVER_H
#include "engine_global.h"


#include "isolver.h"
#include "ihttpclientfactory.h"
#include "posttoantigate.h"
#include <QTimer>
#include <QMap>

namespace BrowserAutomationStudioFramework
{

    class SingleIdWorker : public QObject
    {
      Q_OBJECT
      public:
          QString id;
          QString antigate_id;
          IHttpClient * client;
          QTimer* timer;
          bool is_recaptcha_v3;
          bool is_json_interface;
    };

    class ENGINESHARED_EXPORT AntigateCaptchaSolver : public ISolver
    {
        Q_OBJECT
        QString key;
        IHttpClientFactory * HttpClientFactory;
        IHttpClient * HttpClient;
        QHash<QString,QString> IdToAntigateIdList;
        QList<SingleIdWorker*> Workers;
        QList<QString> RecaptchaV3Ids;
        bool StartedMonitor;
        int Iterator;
        QTimer timer;
        bool StartImmediate;
        int timeout; // Time in milliseconds between each new check of the task result
        int delay; // Time in milliseconds between task creation and the first result check
        QString Server;
        QString SoftId;
        void StartMonitor();
        bool MultipleIds;
    public:
        explicit AntigateCaptchaSolver(QObject *parent = 0);

    signals:
    public slots:
        void SetServer(const QString& Server);
        void SetSoftId(const QString& SoftId);
        void SetTimeout(int timeout);
        void SetDelay(int delay);
        void SetStartImmediate(bool StartImmediate);
        virtual QString Solve(const QString& base64,const QStringList& props);
        virtual void ReportBad(const QString& id);
        virtual bool TimeLimited();
        void SetHttpClientFactory(IHttpClientFactory * HttpClientFactory);
        void SetMultipleIds(bool MultipleIds);
        IHttpClientFactory * GetHttpClientFactory();

    private slots:
        void DeleteSender();
        void StartIteration();
        void StartSingleIteration();
        void DoneIteration();
        void DoneSignleIteration();
        void PostedToAntigate(const QString& antigate_id, const QString& id, bool res, bool is_recaptcha_v3, bool is_json_interface);
    };
}
#endif // ANTIGATECAPTCHASOLVER_H
