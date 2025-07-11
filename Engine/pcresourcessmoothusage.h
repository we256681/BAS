#ifndef PCRESOURCESSMOOTHUSAGE_H
#define PCRESOURCESSMOOTHUSAGE_H
#include "engine_global.h"
#include <QHash>
#include <QList>
#include <QDateTime>
#include <QTimer>
#include "pdh.h"
#include "ipcresourcessmoothusage.h"


namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT PCResourcesSmoothUsage : public IPCResourcesSmoothUsage
    {
        Q_OBJECT
        PDH_HQUERY cpuQuery;
        PDH_HCOUNTER cpuTotal;
        QTimer *Timer;
        QDateTime LastSystemLoad;

        QHash<int,QDateTime> BrowsersStarting;
        QList<int> BrowsersPending;
        int MaxBrowserStartSimultaneously;
        int MinFreeMemoryToStartBrowser;
        int MinUnusedCpu;
        int CpuMeasurmentInterval = 2000;
        int MinimumIntervalBetweenBrowserStart = 1000;
        bool StartedBrowserAtLeastOnce = false;
        QDateTime LastTimeStartedBrowser;

        QList<QPair<int, QDateTime> > CPUMeasurements;

        void RemoveOldStartingBrowsers();
        void StartPendingBrowsers();
        int CalculateNumberOfStartingBrowsers();
        bool CheckCanStartBrowser();
        int GetUsedCpu();
        int GetFreeMemory();
    public:
        explicit PCResourcesSmoothUsage(QObject *parent = 0);
    public slots:
        virtual void NeedToStartBrowser(int BrowserId);
        virtual void BrowserStarted(int BrowserId);
    private slots:
        void TimerSlot();
    };
}

#endif // PCRESOURCESSMOOTHUSAGE_H

