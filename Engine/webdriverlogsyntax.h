#ifndef WEBDRIVERLOGSYNTAX_H
#define WEBDRIVERLOGSYNTAX_H

#include "engine_global.h"
#include "ilogsyntax.h"
#include <QObject>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT WebDriverLogSyntax : public ILogSyntax
    {
        Q_OBJECT
    public:
        explicit WebDriverLogSyntax(QObject *parent = 0);
        virtual QString Process(const QString& Line);

    };

}
#endif // WEBDRIVERLOGSYNTAX_H

