#ifndef IPROFILERDATAVISUALIZER_H
#define IPROFILERDATAVISUALIZER_H
#include "engine_global.h"

#include <QObject>
#include "iprofilerdata.h"


namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT IProfilerDataVisualizer : public QObject
    {

        Q_OBJECT
    public:
        explicit IProfilerDataVisualizer(QObject *parent = 0);
        virtual void SetProfilerData(IProfilerData* ProfilerData) = 0;
    signals:

    public slots:
        virtual void ShowReport() = 0;
    };
}

#endif // IPROFILERDATAVISUALIZER_H
