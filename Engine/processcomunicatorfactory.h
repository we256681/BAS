#ifndef PROCESSCOMUNICATORFACTORY_H
#define PROCESSCOMUNICATORFACTORY_H
#include "engine_global.h"

#include "iprocesscomunicatorfactory.h"
#include "pipesprocesscomunicatorfactory.h"
#include "webdriverprocesscomunicatorfactory.h"
#include "ihttpclientfactory.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT ProcessComunicatorFactory : public IProcessComunicatorFactory
    {
        Q_OBJECT
        PipesProcessComunicatorFactory *_PipesProcessComunicatorFactory;
        WebDriverProcessComunicatorFactory *_WebDriverProcessComunicatorFactory;
        QStringList PipesParams;
        IHttpClientFactory * HttpClientFactory;
        bool IsRecord;
        QString RecordId;
    public:
        explicit ProcessComunicatorFactory(QObject *parent = 0);
        virtual IProcessComunicator * CreateProcessComunicator(const QString& BrowserEngine);
        virtual void SetLogger(ILogger * Logger);
        //Should contain %keyin% %keyout% %pid%
        void SetCommandLineParamsForPipes(const QStringList& Params);
        void SetHttpClientFactoryForWebDriver(IHttpClientFactory * HttpClientFactory);
        void SetIsRecord();
        void SetRecordId(const QString& RecordId);

    signals:
        void ProcessComunicatorCreated(IProcessComunicator *);
        void Received(const QString& key, const QString& data);
        void KeyStart(const QString& key);
        void SocketStoped(const QString& key);
        void WebDriverStart(IProcessComunicatorFactory * Factory);

    };
}

#endif // PROCESSCOMUNICATORFACTORY_H
