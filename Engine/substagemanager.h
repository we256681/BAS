#ifndef SUBSTAGEMANAGER_H
#define SUBSTAGEMANAGER_H

#include <QHash>
#include "iworker.h"
#include "ibrowser.h"

namespace BrowserAutomationStudioFramework
{
    class IWorker;
    class IBrowser;

}

class SubstageManager
{
    struct Substage
    {
        int Id;
        qint64 MaxSuccessLeft;
        qint64 MaxFailureLeft;
        qint64 ThreadsWorking;
        int MaxIndex;
        bool IsDead = false;
        bool IsDeadInstant = false;



        QList<BrowserAutomationStudioFramework::IWorker *> Workers;
        QList<BrowserAutomationStudioFramework::IBrowser *> Browsers;

    };
    struct SubstageInfo
    {
        int Id;

        /* Custom threads */
        bool CustomThreadsIsSuccess = false;
        bool CustomThreadsIsRunning = true;
        bool CustomThreadsWasRunningAtLeastOnce = false;
        QString CustomThreadsResultJson;
        QString CustomThreadsErrorMesage;
        bool CustomThreadsStopThreadAfterTaskFinish = false;
        bool CustomThreadsStartPostponedOnIdle;
    };
    struct SubstageQueue
    {
        int Id;

        /* Custom threads */
        QString FunctionName;
        QString Parameters;
        bool StopThreadAfterTaskFinish;
        int MaxThreads;
        bool StartNewCustomThread;
        bool StartPostponedOnIdle;
        int StartIndex;
    };
    QHash<int, Substage> Stages;
    QHash<int, SubstageInfo> StagesInfo;
    QList<SubstageQueue> StagesQueue;
    int StartIndex = 0;
public:


    SubstageManager();
    void AddSubstage(int Id, qint64 MaxSuccessLeft, qint64 MaxFailureLeft, qint64 ThreadsWorking, int MaxIndex, QList<BrowserAutomationStudioFramework::IBrowser *> Browsers);
    void RemoveStage(int Id);
    QList<BrowserAutomationStudioFramework::IWorker *> * GetWorkers(int Id);
    QList<BrowserAutomationStudioFramework::IBrowser *> * GetBrowsers(int Id);
    QList<BrowserAutomationStudioFramework::IWorker *> GetAllWorkers();
    QList<BrowserAutomationStudioFramework::IBrowser *> GetAllBrowsers();
    int MaxIndex();
    int TotalThreads();
    int TotalStages();
    int TotalQueued();
    void Success(int Id);
    void Failure(int Id);
    bool IsDeadInstant(int Id);
    void Die(int Id, bool Instant);
    void EndThread(int Id);
    bool CreateThread(int Id);
    bool NeedToFinishStage(int Id);
    void Clear();
    bool HasStage(int Id);

    /* Custom threads */
    void SetCustomThreadInfo(int Id, bool IsSuccess, bool IsRunning, QString ResultJson, QString ErrorMesage);
    bool CustomThreadGetIsRunning(int ThreadId);
    bool CustomThreadGetIsSuccess(int ThreadId);
    QString CustomThreadGetResult(int ThreadId);
    QString CustomThreadGetError(int ThreadId);
    bool CustomThreadGetWasRunningAtLeastOnce(int ThreadId);
    bool CustomThreadGetStopThreadAfterTaskFinish(int ThreadId);
    void CustomThreadSetStopThreadAfterTaskFinish(int ThreadId, bool StopThreadAfterTaskFinish);
    bool CustomThreadGetStartPostponedOnIdle(int ThreadId);
    void CustomThreadSetStartPostponedOnIdle(int ThreadId, bool StartPostponedOnIdle);
    QList<int> CustomThreadGetAllNotRunning();

    /* Custom threads queue */
    void CustomThreadQueueFunction(int ThreadId, QString FunctionName, QString Parameters, bool StopThreadAfterTaskFinish, bool StartPostponedOnIdle, int MaxThreads, bool StartNewCustomThread);
    int CustomThreadsTotal();
    bool CustomThreadsNextQueuedFunction(int& ThreadId, QString& FunctionName, QString& Parameters, bool& StopThreadAfterTaskFinish, bool &StartPostponedOnIdle, int& MaxThreads, bool StartNewCustomThread, int InputThreadId);
    void CustomThreadRemoveQueue(int ThreadId);
    bool CustomThreadIsQueue(int ThreadId);
    bool CustomThreadCanStartInIddleThread();
    int CustomThreadGetIddleThread();

};

#endif // SUBSTAGEMANAGER_H
