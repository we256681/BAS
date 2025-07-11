#ifndef POSTPREPARESTRATEGYCUSTOMENCODE_H
#define POSTPREPARESTRATEGYCUSTOMENCODE_H

#include <QString>
#include "engine_global.h"
#include "ipostpreparestrategy.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT PostPrepareStrategyCustomEncode : public IPostPrepareStrategy
    {
        QString ContentType;
        QString Boundary;
        void ParseBoundary(const QString& Data);
        QByteArray ConvertBase64(const QString& Data);

    public:
        PostPrepareStrategyCustomEncode();
        virtual QByteArray GenerateData(const QHash<QString,ContentData> & params);
        virtual QByteArray GetContentType();
        void SetContentType(const QString& ContentType);
    };
}

#endif // POSTPREPARESTRATEGYCUSTOMENCODE_H
