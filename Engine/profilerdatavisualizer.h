#ifndef PROFILERDATAVISUALIZER_H
#define PROFILERDATAVISUALIZER_H
#include "engine_global.h"

#include <QObject>
#include "iprofilerdatavisualizer.h"
#include "reportdatavisualizerdialog.h"


namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT ProfilerDataVisualizer : public IProfilerDataVisualizer
    {
        Q_OBJECT
        IProfilerData* ProfilerData;
        ReportDataVisualizerDialog *VisualizerDialog;

    public:
        explicit ProfilerDataVisualizer(QObject *parent = 0);
        virtual void SetProfilerData(IProfilerData* ProfilerData);
    signals:

    public slots:
        virtual void ShowReport();
        void DisableProfiler();
    };
}

#endif // PROFILERDATAVISUALIZER_H
