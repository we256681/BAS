#ifndef DEVICESCALEMANAGER_H
#define DEVICESCALEMANAGER_H

#include <QObject>
#include "engine_global.h"
#include "idevicescalemanager.h"


namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT DeviceScaleManager : public IDeviceScaleManager
    {
        Q_OBJECT
    public:
        explicit DeviceScaleManager(QObject *parent = 0);
        virtual double GetScaleFactor();
        virtual void Autoscale();
    };
}

#endif // DEVICESCALEMANAGER_H
