#ifndef ICHECKSCRIPT_H
#define ICHECKSCRIPT_H

#include "engine_global.h"

#include <QObject>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT ICheckScript : public QObject
    {
        Q_OBJECT
    public:
        explicit ICheckScript(QObject *parent = nullptr);
        virtual bool Check(const QString& Script) = 0;
    signals:

    };

}

#endif // ICHECKSCRIPT_H
