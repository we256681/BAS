#include "profilerdata.h"
#include <QDebug>
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    ProfilerData::ProfilerData(QObject *parent) : IsProfilerEnabled(parent)
    {
        IsProfilerEnabled = false;
        IsTraceEnabled = false;
    }

    void ProfilerData::SetIsProfilerEnabled(bool IsProfilerEnabled)
    {
        this->IsProfilerEnabled = IsProfilerEnabled;
        if(!IsProfilerEnabled)
            Threads.clear();
    }

    void ProfilerData::SetIsTraceEnabled(bool IsTraceEnabled)
    {
        this->IsTraceEnabled = IsTraceEnabled;
    }

    bool ProfilerData::GetIsTraceEnabled()
    {
        return IsTraceEnabled;
    }

    bool ProfilerData::GetIsProfilerEnabled()
    {
        return IsProfilerEnabled;
    }


    QString ProfilerData::GenerateReport()
    {
        if(TimeAllReport.empty())
            return tr("Data will be available after launching script in run mode.");

        QMap<qint64, qint64> TimeReportReverse;
        QMapIterator<qint64, qint64> ItTimeReport(TimeCPUReport);
        while(ItTimeReport.hasNext())
        {
            ItTimeReport.next();
            TimeReportReverse.insertMulti(ItTimeReport.value(),ItTimeReport.key());
        }

        QString Report;
        Report += tr("<table ><tr ><td style=\"text-align:center;padding-left:10px;padding-right:10px;\">Action #</td><td style=\"text-align:center;padding-left:10px;padding-right:10px;\">CPU time mseconds</td><td style=\"text-align:center;padding-left:10px;padding-right:10px;\">Total time mseconds</td></tr>");
        Report += QString("\n");

        QMapIterator<qint64, qint64> ItTimeReportReverse(TimeReportReverse);
        ItTimeReportReverse.toBack();
        while(ItTimeReportReverse.hasPrevious())
        {
            ItTimeReportReverse.previous();
            Report += QString("<tr ><td style=\"text-align:center;padding-left:10px;padding-right:10px;\">") + QString::number(ItTimeReportReverse.value()) + QString("</td><td style=\"text-align:center;padding-left:10px;padding-right:10px;\">") + QString::number(ItTimeReportReverse.key()) + QString("</td><td style=\"text-align:center;padding-left:10px;padding-right:10px;\">") + QString::number(TimeAllReport[ItTimeReportReverse.value()]) + QString("</td></tr>");
        }
        Report += tr("</table>");


        return Report;
    }

    void ProfilerData::Start()
    {
        StartTime = QDateTime::currentDateTime();
        Threads.clear();
        TimeAllReport.clear();
        TimeCPUReport.clear();
    }

    void ProfilerData::SwitchCurrentActionToAsync(qint64 ThreadId)
    {
        if(!IsProfilerEnabled)
            return;

        if(!Threads.contains(ThreadId))
        {
            return;
        }

        ThreadData& Data = Threads[ThreadId];
        if(Data.ActionId == 0)
        {
            return;
        }

        if(!Data.IsCPU)
        {
            return;
        }

        Data.IsCPU = false;

        QDateTime now = QDateTime::currentDateTime();
        qint64 TimeSpent = Data.StartTime.msecsTo(now);

        if(TimeAllReport.contains(Data.ActionId))
        {
            TimeAllReport[Data.ActionId] += TimeSpent;
        }else
        {
            TimeAllReport[Data.ActionId] = TimeSpent;
        }

        if(TimeCPUReport.contains(Data.ActionId))
        {
            TimeCPUReport[Data.ActionId] += TimeSpent;
        }else
        {
            TimeCPUReport[Data.ActionId] = TimeSpent;
        }

        Data.StartTime = now;
    }

    void ProfilerData::SwitchCurrentActionToSync(qint64 ThreadId)
    {
        if(!IsProfilerEnabled)
            return;

        if(!Threads.contains(ThreadId))
        {
            return;
        }

        ThreadData& Data = Threads[ThreadId];
        if(Data.ActionId == 0)
        {
            return;
        }

        if(Data.IsCPU)
        {
            return;
        }

        Data.IsCPU = true;

        QDateTime now = QDateTime::currentDateTime();
        qint64 TimeSpent = Data.StartTime.msecsTo(now);

        if(TimeAllReport.contains(Data.ActionId))
        {
            TimeAllReport[Data.ActionId] += TimeSpent;
        }else
        {
            TimeAllReport[Data.ActionId] = TimeSpent;
        }

        TimeSpent = 0;

        if(TimeCPUReport.contains(Data.ActionId))
        {
            TimeCPUReport[Data.ActionId] += TimeSpent;
        }else
        {
            TimeCPUReport[Data.ActionId] = TimeSpent;
        }

        Data.StartTime = now;


    }

    void ProfilerData::RunAction(qint64 ThreadId,qint64 ActionId)
    {
        if(!IsProfilerEnabled)
            return;

        QDateTime now = QDateTime::currentDateTime();

        if(Threads.contains(ThreadId))
        {
            ThreadData& Data = Threads[ThreadId];

            qint64 TimeSpent = Data.StartTime.msecsTo(now);

            if(Data.ActionId != 0)
            {
                if(TimeAllReport.contains(Data.ActionId))
                {
                    TimeAllReport[Data.ActionId] += TimeSpent;
                }else
                {
                    TimeAllReport[Data.ActionId] = TimeSpent;
                }

                if(!Data.IsCPU)
                {
                    TimeSpent = 0;
                }

                if(TimeCPUReport.contains(Data.ActionId))
                {
                    TimeCPUReport[Data.ActionId] += TimeSpent;
                }else
                {
                    TimeCPUReport[Data.ActionId] = TimeSpent;
                }

            }

            Data.StartTime = now;
            Data.ActionId = ActionId;

        }else
        {
            ThreadData Data;

            Data.StartTime = now;
            Data.ActionId = ActionId;
            Data.IsCPU = true;

            Threads[ThreadId] = Data;
        }
    }
}

