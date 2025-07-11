#include "pcresourcessmoothusage.h"
#include <QSettings>
#include <QDebug>
#include <QMutableHashIterator>
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    PCResourcesSmoothUsage::PCResourcesSmoothUsage(QObject *parent) : IPCResourcesSmoothUsage(parent)
    {
        PdhOpenQuery(NULL, NULL, &cpuQuery);
        PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
        PdhCollectQueryData(cpuQuery);
        MaxBrowserStartSimultaneously = 3;
        MinFreeMemoryToStartBrowser = 1500;
        MinUnusedCpu = 35;
        Timer = new QTimer(this);
        connect(Timer,SIGNAL(timeout()),this,SLOT(TimerSlot()));
        Timer->setSingleShot(false);
        Timer->setInterval(300);
        Timer->start();
        {
            QSettings SettingsWorker("settings_worker.ini",QSettings::IniFormat);
            MinUnusedCpu = SettingsWorker.value("MinUnusedCpu",MinUnusedCpu).toInt();
            MinFreeMemoryToStartBrowser = SettingsWorker.value("MinFreeMemoryToStartBrowser",MinFreeMemoryToStartBrowser).toInt();
            MaxBrowserStartSimultaneously = SettingsWorker.value("MaxBrowserStartSimultaneously",MaxBrowserStartSimultaneously).toInt();
        }

    }

    bool PCResourcesSmoothUsage::CheckCanStartBrowser()
    {
        QDateTime now = QDateTime::currentDateTime();

        if(StartedBrowserAtLeastOnce && LastTimeStartedBrowser.addMSecs(MinimumIntervalBetweenBrowserStart) > now)
            return false;

        int UsedCpu = GetUsedCpu();
        int FreeMem = GetFreeMemory();

        bool CanStart = ((100 - UsedCpu) >= MinUnusedCpu) && (FreeMem >= MinFreeMemoryToStartBrowser);
        if(CanStart)
        {
            LastSystemLoad = QDateTime();
        }else
        {
            if(BrowsersPending.empty())
            {
                LastSystemLoad = QDateTime();
            }else if(!LastSystemLoad.isNull())
            {
                int sec = LastSystemLoad.secsTo(now);
                if(sec > 30)
                {
                    QString log = tr("Failed to start browser, not enough system resources. Free memory %1 mb, processor usage %2 %").arg(QString::number(FreeMem)).arg(QString::number(UsedCpu));
                    emit Log(log);
                    LastSystemLoad = QDateTime();
                }

            }else
            {
                LastSystemLoad = now;
            }
        }


        return CanStart;
    }

    int PCResourcesSmoothUsage::GetUsedCpu()
    {
        QDateTime now = QDateTime::currentDateTime();

        int MaxCPU = 0;

        //Remove CPU measurements older than 10 seconds
        QMutableListIterator<QPair<int, QDateTime> > i(CPUMeasurements);
        while (i.hasNext())
        {
            i.next();
            int diff = i.value().second.msecsTo(now);
            if(diff > CpuMeasurmentInterval)
            {
                i.remove();
            }else
            {
                //Calculate max CPU
                if(i.value().first > MaxCPU)
                    MaxCPU = i.value().first;
            }
        }

        //Get actual CPU usage
        PDH_FMT_COUNTERVALUE counterVal;
        PdhCollectQueryData(cpuQuery);
        PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
        int res = counterVal.doubleValue;
        if(res>100)
            res = 100;
        if(res<0)
            res = 0;

        //Add actual CPU to list
        QPair<int, QDateTime> ResPair;
        ResPair.first = res;
        ResPair.second = now;
        CPUMeasurements.push_back(ResPair);

        //Update MaxCPU with actual value
        if(res > MaxCPU)
            MaxCPU = res;

        return MaxCPU;
    }

    int PCResourcesSmoothUsage::GetFreeMemory()
    {
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        return status.ullAvailPhys / (1024 * 1024);
    }

    void PCResourcesSmoothUsage::TimerSlot()
    {
        RemoveOldStartingBrowsers();
        if(CheckCanStartBrowser() && !BrowsersPending.empty())
            StartPendingBrowsers();
    }

    void PCResourcesSmoothUsage::RemoveOldStartingBrowsers()
    {
        QMutableHashIterator<int,QDateTime> i(BrowsersStarting);
        QDateTime now = QDateTime::currentDateTime();
        while (i.hasNext())
        {
            i.next();
            int diff = i.value().secsTo(now);
            if(diff > 30)
            {
                i.remove();
            }
        }
    }

    void PCResourcesSmoothUsage::StartPendingBrowsers()
    {
        int pending = CalculateNumberOfStartingBrowsers();
        if(MaxBrowserStartSimultaneously > pending)
        {
            if(BrowsersPending.empty())
                return;
            int BrowserId = BrowsersPending.first();
            BrowsersPending.removeFirst();
            QDateTime now = QDateTime::currentDateTime();
            BrowsersStarting[BrowserId] = now;
            StartedBrowserAtLeastOnce = true;
            LastTimeStartedBrowser = QDateTime::currentDateTime();
            emit CanStartBrowser(BrowserId);
        }
    }

    int PCResourcesSmoothUsage::CalculateNumberOfStartingBrowsers()
    {
        return BrowsersStarting.size();
    }

    void PCResourcesSmoothUsage::NeedToStartBrowser(int BrowserId)
    {
        RemoveOldStartingBrowsers();
        if(CalculateNumberOfStartingBrowsers() < MaxBrowserStartSimultaneously && CheckCanStartBrowser())
        {
            QDateTime now = QDateTime::currentDateTime();
            BrowsersStarting[BrowserId] = now;
            StartedBrowserAtLeastOnce = true;
            LastTimeStartedBrowser = QDateTime::currentDateTime();
            emit CanStartBrowser(BrowserId);
        }else
        {
            BrowsersPending.append(BrowserId);
        }

    }

    void PCResourcesSmoothUsage::BrowserStarted(int BrowserId)
    {
        BrowsersStarting.remove(BrowserId);
        RemoveOldStartingBrowsers();
        if(CheckCanStartBrowser())
            StartPendingBrowsers();
    }
}
