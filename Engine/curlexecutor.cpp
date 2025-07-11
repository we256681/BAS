#include "curlexecutor.h"
#include "every_cpp.h"


namespace BrowserAutomationStudioFramework
{
    CurlExecutor::CurlExecutor(QObject *parent) :
        QObject(parent)
    {
        Curl = 0;
        Thread = 0;
        Result = false;
        Data = "Not Run";
        UseExternalThread = false;
        DoTrace = false;

    }

    void CurlExecutor::SetThread(QThread *Thread)
    {
        this->Thread = Thread;
        UseExternalThread = true;
    }



    CurlExecutor::~CurlExecutor()
    {
        Stop();
    }

    void CurlExecutor::Stop()
    {
        if(Curl)
        {
            emit StopCurl();
            Curl = 0;
        }
    }

    void CurlExecutor::SetFilename(const QString& FileName)
    {
        this->FileName = FileName;
    }

    void CurlExecutor::SetDoTrace()
    {
        this->DoTrace = true;
    }


    void CurlExecutor::Execute()
    {

        if(!UseExternalThread)
        {
            Thread = new QThread();
        }



        Curl = new CurlWrapper();
        if(DoTrace)
        {
            Curl->SetDoTrace();
            Curl->SetTraceFilter(TraceFilter);
        }
        Curl->SetFileName(FileName);
        Curl->SetOptions(Options);
        Curl->SetFilter(Filter);
        Curl->moveToThread(Thread);



        connect(this, SIGNAL(StartCurl()), Curl, SLOT(Start()));
        connect(this, SIGNAL(StopCurl()), Curl, SLOT(Stop()),Qt::DirectConnection);
        connect(Curl, SIGNAL(Done(bool,QByteArray,QByteArray,QByteArray)), this, SLOT(DoneWrapper(bool,QByteArray,QByteArray,QByteArray)));
        connect(Curl, SIGNAL(Done(bool,QByteArray,QByteArray,QByteArray)), Curl, SLOT(deleteLater()));

        if(!UseExternalThread)
        {
            connect(Curl, SIGNAL(Done(bool,QByteArray,QByteArray,QByteArray)), Thread, SLOT(quit()));
            connect(Thread, SIGNAL(finished()), Thread, SLOT(deleteLater()));
        }

        Thread->start();

        emit StartCurl();

    }




    void CurlExecutor::DoneWrapper(bool Result,const QByteArray& Data,const QByteArray& Log,const QByteArray& Trace)
    {
        this->Result = Result;
        this->Data = Data;
        this->Log = Log;
        this->Trace = Trace;
        Curl = 0;
        Options.clear();
        Filter.clear();
        emit Done();

    }

    QtCUrl::Options* CurlExecutor::GetOptions()
    {
        return &Options;
    }

    bool CurlExecutor::GetResult()
    {
        return Result;
    }

    QByteArray CurlExecutor::GetDataByteArray()
    {
        return Data;
    }

    QByteArray CurlExecutor::GetTrace()
    {
        return Trace;
    }

    void CurlExecutor::SetFilter(const QString& Filter)
    {
        this->Filter = Filter;
    }

    void CurlExecutor::SetTraceFilter(const QString& TraceFilter)
    {
        this->TraceFilter = TraceFilter;
    }


    QByteArray CurlExecutor::GetLogByteArray()
    {
        return this->Log;
    }
}
