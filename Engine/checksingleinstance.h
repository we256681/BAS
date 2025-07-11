#ifndef CHECKSINGLEINSTANCE_H
#define CHECKSINGLEINSTANCE_H

#include "engine_global.h"
#include <QObject>
#include <QSharedMemory>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT CheckSingleInstance : public QObject
    {
        Q_OBJECT
        QSharedMemory sharedMemory;
    public:
        explicit CheckSingleInstance(QObject *parent = nullptr);
        bool Check(const QString& Key);
    signals:

    public slots:
    };
}

#endif // CHECKSINGLEINSTANCE_H
