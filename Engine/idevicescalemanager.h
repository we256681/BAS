#ifndef IDEVICESCALEMANAGER_H
#define IDEVICESCALEMANAGER_H

#include "engine_global.h"
#include <QObject>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT IDeviceScaleManager : public QObject
    {
        Q_OBJECT
    public:
        explicit IDeviceScaleManager(QObject *parent = nullptr);
        virtual double GetScaleFactor() = 0;
        virtual void Autoscale() = 0;
    signals:

    };
}

#endif // IDEVICESCALEMANAGER_H
