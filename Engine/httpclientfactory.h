#ifndef HTTPCLIENTFACTORY_H
#define HTTPCLIENTFACTORY_H
#include "engine_global.h"

#include <QObject>
#include "ihttpclientfactory.h"
#include "ipostpreparestrategyfactory.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT HttpClientFactory : public IHttpClientFactory
    {
        Q_OBJECT
        bool DoSniff;
        bool ExchangeReports;
        IPostPrepareStrategyFactory *PostPrepareStrategyFactoryObject;
    public:
        explicit HttpClientFactory(QObject *parent = 0);
        virtual IHttpClient * GetHttpClient(bool UseSnifer = true);
        virtual void Sniff(bool DoSniff);
    signals:
        void NewSnifferReport(SnifferReport);

    public slots:
        void SnifferStateChanged(bool State);
    private slots:
        void NewSnifferReportSlot(SnifferReport);
    };
}

#endif // HTTPCLIENTFACTORY_H
