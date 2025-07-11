#include "httpclientfactory.h"
#include "httpclient.h"
#include "curlhttpclient.h"

#include "postpreparestrategyfactory.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{

    HttpClientFactory::HttpClientFactory(QObject *parent) :
        IHttpClientFactory(parent)
    {
        PostPrepareStrategyFactoryObject = new PostPrepareStrategyFactory(this);
        DoSniff = false;
        ExchangeReports = true;
    }

    void HttpClientFactory::Sniff(bool DoSniff)
    {
        this->DoSniff = DoSniff;
    }

    void HttpClientFactory::SnifferStateChanged(bool State)
    {
        ExchangeReports = State;
    }

    IHttpClient * HttpClientFactory::GetHttpClient(bool UseSnifer)
    {
        IHttpClient* res = new CurlHttpClient();
        if(DoSniff && UseSnifer)
        {
            res->Sniff();
            connect(res,SIGNAL(NewSnifferReport(SnifferReport)),this,SLOT(NewSnifferReportSlot(SnifferReport)));
        }
        res->SetPostPrepareStrategyFactory(PostPrepareStrategyFactoryObject);
        return res;
    }

    void HttpClientFactory::NewSnifferReportSlot(SnifferReport Report)
    {
        if(ExchangeReports)
            emit NewSnifferReport(Report);
    }

}
