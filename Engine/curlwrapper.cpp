#include "curlwrapper.h"
#include "every_cpp.h"


namespace BrowserAutomationStudioFramework
{
    CurlWrapper::CurlWrapper(QObject *parent) :
        QObject(parent)
    {
        cUrl = 0;
        DoTrace = false;
    }

    void CurlWrapper::SetFilter(const QString& Filter)
    {
        this->Filter = Filter;
    }

    void CurlWrapper::SetTraceFilter(const QString& TraceFilter)
    {
        this->TraceFilter = TraceFilter;
    }

    void CurlWrapper::SetFileName(const QString& FileName)
    {
        this->FileName = FileName;
    }

    void CurlWrapper::SetDoTrace()
    {
        this->DoTrace = true;
    }


    void CurlWrapper::SetOptions(const QtCUrl::Options& Options)
    {
        this->Options = Options;
    }

    CurlWrapper::~CurlWrapper()
    {

    }

    void CurlWrapper::Stop()
    {
        if(cUrl)
            cUrl->MarkAutoDelete();
    }


    void CurlWrapper::Start()
    {
        AlreadyDeleted = false;
        cUrl = new QtCUrl();

        if(DoTrace)
        {
            cUrl->SetDoTrace();
            cUrl->SetTraceFilter(TraceFilter);
        }

        cUrl->SetFileName(FileName);

        QtCUrl *cUrlCopy = cUrl;
        cUrl->SetFilter(Filter);

        cUrl->exec(Options);
        cUrl = 0;

        bool result = cUrlCopy->lastError().code() == CURLE_OK;

        QByteArray Data = (result)?cUrlCopy->GetDataByteArray():cUrlCopy->errorBuffer().toUtf8();
        QByteArray Trace = cUrlCopy->GetTrace();
        QByteArray Log = cUrlCopy->GetLogByteArray();
        delete cUrlCopy;

        emit Done(result, Data, Log, Trace);



    }


}

