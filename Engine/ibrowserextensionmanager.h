#ifndef IBROWSEREXTENSIONMANAGER_H
#define IBROWSEREXTENSIONMANAGER_H

#include "engine_global.h"
#include "ihttpclientfactory.h"
#include <QObject>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT IBrowserExtensionManager : public QObject
    {
        Q_OBJECT
    public:
        explicit IBrowserExtensionManager(QObject *parent = 0);
        virtual QString Require(const QString& Extension, bool& IsFinishedInstantly, bool& IsInstantFailure) = 0;
        virtual void SetHttpClientFactory(IHttpClientFactory *HttpClientFactory) = 0;
    signals:
        void RequireRequestDone(QString Id, QString Path, bool IsFailure);
    };
}


#endif // IBROWSEREXTENSIONMANAGER_H
