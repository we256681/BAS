#ifndef IPROFILERDATA_H
#define IPROFILERDATA_H
#include "engine_global.h"

#include <QObject>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT IProfilerData : public QObject
    {

        Q_OBJECT
    public:
        explicit IProfilerData(QObject *parent = 0);
        virtual void SetIsProfilerEnabled(bool IsProfilerEnabled) = 0;
        virtual void SetIsTraceEnabled(bool IsTraceEnabled) = 0;
        virtual bool GetIsTraceEnabled() = 0;
        virtual bool GetIsProfilerEnabled() = 0;

        virtual QString GenerateReport() = 0;
        virtual void Start() = 0;
        virtual void RunAction(qint64 ThreadId,qint64 ActionId) = 0;
        virtual void SwitchCurrentActionToAsync(qint64 ThreadId) = 0;
        virtual void SwitchCurrentActionToSync(qint64 ThreadId) = 0;
    signals:

    };
}

#endif // IPROFILERDATA_H
