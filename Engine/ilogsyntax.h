#ifndef ILOGSYNTAX_H
#define ILOGSYNTAX_H

#include "engine_global.h"

#include <QObject>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT ILogSyntax : public QObject
    {
        Q_OBJECT
    public:
        explicit ILogSyntax(QObject *parent = 0);
        virtual QString Process(const QString& Line) = 0;
    signals:

    public slots:

    };
}


#endif // ILOGSYNTAX_H
