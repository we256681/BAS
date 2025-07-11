#include "processcomunicatorfactory.h"
#include "every_cpp.h"



namespace BrowserAutomationStudioFramework
{
    ProcessComunicatorFactory::ProcessComunicatorFactory(QObject *parent) :
        IProcessComunicatorFactory(parent)
    {
       _PipesProcessComunicatorFactory = 0;
       _WebDriverProcessComunicatorFactory = 0;
       IsRecord = false;
    }

    void ProcessComunicatorFactory::SetIsRecord()
    {
        this->IsRecord = true;
    }

    void ProcessComunicatorFactory::SetRecordId(const QString& RecordId)
    {
        this->RecordId = RecordId;
    }

    void ProcessComunicatorFactory::SetCommandLineParamsForPipes(const QStringList& Params)
    {
        PipesParams = Params;
    }

    void ProcessComunicatorFactory::SetHttpClientFactoryForWebDriver(IHttpClientFactory * HttpClientFactory)
    {
        this->HttpClientFactory = HttpClientFactory;
    }

    IProcessComunicator * ProcessComunicatorFactory::CreateProcessComunicator(const QString& BrowserEngine)
    {
        if(BrowserEngine == "BASChrome")
        {
            if(!_PipesProcessComunicatorFactory)
            {
                _PipesProcessComunicatorFactory = new PipesProcessComunicatorFactory(this);
                _PipesProcessComunicatorFactory->SetCommandLineParams(PipesParams);
                connect(_PipesProcessComunicatorFactory,SIGNAL(KeyStart(QString)),this,SIGNAL(KeyStart(QString)));
                connect(_PipesProcessComunicatorFactory,SIGNAL(ProcessComunicatorCreated(IProcessComunicator*)),this,SIGNAL(ProcessComunicatorCreated(IProcessComunicator*)));
                connect(_PipesProcessComunicatorFactory,SIGNAL(Received(QString,QString)),this,SIGNAL(Received(QString,QString)));
                connect(_PipesProcessComunicatorFactory,SIGNAL(SocketStoped(QString)),this,SIGNAL(SocketStoped(QString)));
            }
            return _PipesProcessComunicatorFactory->CreateProcessComunicator(BrowserEngine);
        }else
        {
            if(!_WebDriverProcessComunicatorFactory)
            {
                _WebDriverProcessComunicatorFactory = new WebDriverProcessComunicatorFactory(this);
                _WebDriverProcessComunicatorFactory->SetHttpClientFactory(HttpClientFactory);
                emit WebDriverStart(_WebDriverProcessComunicatorFactory);
            }

            if(IsRecord)
            {
                _WebDriverProcessComunicatorFactory->SetIsRecord();
                _WebDriverProcessComunicatorFactory->SetRecordId(RecordId);
            }

            return _WebDriverProcessComunicatorFactory->CreateProcessComunicator(BrowserEngine);
        }
    }

    void ProcessComunicatorFactory::SetLogger(ILogger * Logger)
    {

    }


}
