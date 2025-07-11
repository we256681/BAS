#ifndef PROFILERDATA_H
#define PROFILERDATA_H
#include "engine_global.h"

#include <QObject>
#include <QDateTime>
#include <QMap>
#include "iprofilerdata.h"

namespace BrowserAutomationStudioFramework
{

    struct ThreadData
    {
          QDateTime StartTime;
          qint64 ActionId;
          bool IsCPU;
    };

    class ENGINESHARED_EXPORT ProfilerData : public IProfilerData
    {
        Q_OBJECT

        bool IsProfilerEnabled;
        bool IsTraceEnabled;
        QDateTime StartTime;
        QMap<qint64, ThreadData> Threads;
        QMap<qint64, qint64> TimeAllReport;
        QMap<qint64, qint64> TimeCPUReport;

    public:
        explicit ProfilerData(QObject *parent = 0);

        virtual void SetIsProfilerEnabled(bool IsProfilerEnabled);
        virtual void SetIsTraceEnabled(bool IsTraceEnabled);
        virtual bool GetIsTraceEnabled();
        virtual bool GetIsProfilerEnabled();
        virtual QString GenerateReport();
        virtual void SwitchCurrentActionToAsync(qint64 ThreadId);
        virtual void SwitchCurrentActionToSync(qint64 ThreadId);
        virtual void Start();
        virtual void RunAction(qint64 ThreadId,qint64 ActionId);

    };
}
#endif // PROFILERDATA_H
