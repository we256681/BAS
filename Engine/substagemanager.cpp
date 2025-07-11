#include "substagemanager.h"

SubstageManager::SubstageManager()
{

}

void SubstageManager::AddSubstage(int Id, qint64 MaxSuccessLeft, qint64 MaxFailureLeft, qint64 ThreadsWorking, int MaxIndex, QList<BrowserAutomationStudioFramework::IBrowser *> Browsers)
{
    Substage s;
    s.Id = Id;
    s.MaxFailureLeft = MaxFailureLeft;
    s.MaxSuccessLeft = MaxSuccessLeft;
    s.ThreadsWorking = ThreadsWorking;
    s.MaxIndex = MaxIndex;
    s.Browsers = Browsers;
    Stages[Id] = s;
}

bool SubstageManager::HasStage(int Id)
{
   return Stages.contains(Id);
}

QList<BrowserAutomationStudioFramework::IWorker *> * SubstageManager::GetWorkers(int Id)
{
    if(!Stages.contains(Id))
        return 0;

    Substage &s = Stages[Id];
    return &s.Workers;

}

QList<BrowserAutomationStudioFramework::IBrowser *> *SubstageManager::GetBrowsers(int Id)
{
    if(!Stages.contains(Id))
        return 0;

    Substage &s = Stages[Id];
    return &s.Browsers;

}


QList<BrowserAutomationStudioFramework::IWorker *> SubstageManager::GetAllWorkers()
{
    QList<BrowserAutomationStudioFramework::IWorker *> res;
    for(Substage s:Stages)
    {
        for(BrowserAutomationStudioFramework::IWorker * w:s.Workers)
        {
            res.append(w);
        }
    }
    return res;
}

int SubstageManager::MaxIndex()
{
    int MaxIndex = 0;
    for(Substage s:Stages)
    {
        if(s.MaxIndex > MaxIndex)
            MaxIndex = s.MaxIndex;
    }
    return MaxIndex;
}

int SubstageManager::TotalThreads()
{
    int res = 0;
    for(Substage s:Stages)
    {
        res += s.ThreadsWorking;
    }
    return res;
}

int SubstageManager::TotalStages()
{
    return Stages.size();
}

int SubstageManager::TotalQueued()
{
    return StagesQueue.size();
}

QList<BrowserAutomationStudioFramework::IBrowser *> SubstageManager::GetAllBrowsers()
{
    QList<BrowserAutomationStudioFramework::IBrowser *> res;
    for(Substage s:Stages)
    {
        for(BrowserAutomationStudioFramework::IBrowser * b:s.Browsers)
        {
            res.append(b);
        }
    }
    return res;
}


void SubstageManager::Success(int Id)
{
    if(!Stages.contains(Id))
        return;

    Substage &s = Stages[Id];
    s.MaxSuccessLeft --;
}

void SubstageManager::Failure(int Id)
{
    if(!Stages.contains(Id))
        return;

    Substage &s = Stages[Id];
    s.MaxFailureLeft --;
}

bool SubstageManager::IsDeadInstant(int Id)
{
    if(!Stages.contains(Id))
        return false;

    Substage &s = Stages[Id];
    return s.IsDeadInstant;
}

void SubstageManager::Die(int Id, bool Instant)
{
    if(!Stages.contains(Id))
        return;

    Substage &s = Stages[Id];
    s.IsDead = true;

    s.IsDeadInstant = Instant;

    if(Instant)
    {
        QList<BrowserAutomationStudioFramework::IWorker *> * Workers = GetWorkers(Id);
        for(int i = 0;i<Workers->length();i++)
        {
            BrowserAutomationStudioFramework::IWorker * w = Workers->at(i);
            if(w)
            {
                w->Abort(true);
            }
        }

        QList<BrowserAutomationStudioFramework::IBrowser *> * Browsers = GetBrowsers(Id);
        for(int i = 0;i<Browsers->length();i++)
        {
            BrowserAutomationStudioFramework::IBrowser * b = Browsers->at(i);

            b->SetWorker(0);
        }
    }


}


bool SubstageManager::CreateThread(int Id)
{
    if(!Stages.contains(Id))
        return false;

    Substage &s = Stages[Id];
    bool CanCreateThread = s.MaxSuccessLeft > 0 && s.MaxFailureLeft > 0 && !s.IsDead;
    if(!CanCreateThread)
    {
        s.ThreadsWorking --;
    }

    return CanCreateThread;
}

void SubstageManager::EndThread(int Id)
{
    if(!Stages.contains(Id))
        return;
    Substage &s = Stages[Id];
    s.ThreadsWorking --;
}

void SubstageManager::RemoveStage(int Id)
{
    if(!Stages.contains(Id))
        return;

    Stages.remove(Id);
}


bool SubstageManager::NeedToFinishStage(int Id)
{
    if(!Stages.contains(Id))
        return true;

    Substage &s = Stages[Id];
    bool NeedToFinish = s.ThreadsWorking <= 0;
    /*if(NeedToFinish)
    {
        Stages.remove(Id);
    }*/

    //qDebug()<<s.ThreadsWorking<<NeedToFinish;

    return NeedToFinish;
}

void SubstageManager::Clear()
{
    Stages.clear();
}

void SubstageManager::SetCustomThreadInfo(int Id, bool IsSuccess, bool IsRunning, QString ResultJson, QString ErrorMesage)
{
    if(!StagesInfo.contains(Id))
    {
        SubstageInfo s;
        s.Id = Id;
        StagesInfo[Id] = s;
    }

    SubstageInfo &s = StagesInfo[Id];
    s.CustomThreadsIsSuccess = IsSuccess;
    s.CustomThreadsIsRunning = IsRunning;
    s.CustomThreadsResultJson = ResultJson;
    s.CustomThreadsErrorMesage = ErrorMesage;

    if(IsRunning)
    {
        s.CustomThreadsWasRunningAtLeastOnce = true;
    }
}

bool SubstageManager::CustomThreadGetIsRunning(int ThreadId)
{
    if(!StagesInfo.contains(ThreadId))
        return false;

    SubstageInfo &s = StagesInfo[ThreadId];
    return s.CustomThreadsIsRunning;
}

bool SubstageManager::CustomThreadGetWasRunningAtLeastOnce(int ThreadId)
{
    if(!StagesInfo.contains(ThreadId))
        return false;

    SubstageInfo &s = StagesInfo[ThreadId];
    return s.CustomThreadsWasRunningAtLeastOnce;
}

bool SubstageManager::CustomThreadGetIsSuccess(int ThreadId)
{
    if(!StagesInfo.contains(ThreadId))
        return false;

    SubstageInfo &s = StagesInfo[ThreadId];
    return s.CustomThreadsIsSuccess;
}

QString SubstageManager::CustomThreadGetResult(int ThreadId)
{
    if(!StagesInfo.contains(ThreadId))
        return "null";

    SubstageInfo &s = StagesInfo[ThreadId];
    return s.CustomThreadsResultJson;
}

QString SubstageManager::CustomThreadGetError(int ThreadId)
{
    if(!StagesInfo.contains(ThreadId))
        return QString();

    SubstageInfo &s = StagesInfo[ThreadId];
    return s.CustomThreadsErrorMesage;
}

bool SubstageManager::CustomThreadGetStopThreadAfterTaskFinish(int ThreadId)
{
    if(!StagesInfo.contains(ThreadId))
        return false;

    SubstageInfo &s = StagesInfo[ThreadId];
    return s.CustomThreadsStopThreadAfterTaskFinish;
}

void SubstageManager::CustomThreadSetStopThreadAfterTaskFinish(int ThreadId, bool StopThreadAfterTaskFinish)
{
    if(!StagesInfo.contains(ThreadId))
    {
        SubstageInfo s;
        s.Id = ThreadId;
        StagesInfo[ThreadId] = s;
    }

    SubstageInfo &s = StagesInfo[ThreadId];
    s.CustomThreadsStopThreadAfterTaskFinish = StopThreadAfterTaskFinish;
}

bool SubstageManager::CustomThreadGetStartPostponedOnIdle(int ThreadId)
{
    if(!StagesInfo.contains(ThreadId))
        return false;

    SubstageInfo &s = StagesInfo[ThreadId];
    return s.CustomThreadsStartPostponedOnIdle;
}

void SubstageManager::CustomThreadSetStartPostponedOnIdle(int ThreadId, bool StartPostponedOnIdle)
{
    if(!StagesInfo.contains(ThreadId))
    {
        SubstageInfo s;
        s.Id = ThreadId;
        StagesInfo[ThreadId] = s;
    }

    SubstageInfo &s = StagesInfo[ThreadId];
    s.CustomThreadsStartPostponedOnIdle = StartPostponedOnIdle;
}


QList<int> SubstageManager::CustomThreadGetAllNotRunning()
{
    QList<int> Result;
    for(Substage s:Stages)
    {
        if(StagesInfo.contains(s.Id))
        {
            SubstageInfo& i = StagesInfo[s.Id];
            if(!i.CustomThreadsIsRunning)
            {
                Result.append(s.Id);
            }
        }
    }
    return Result;
}

void SubstageManager::CustomThreadQueueFunction(int ThreadId, QString FunctionName, QString Parameters, bool StopThreadAfterTaskFinish, bool StartPostponedOnIdle, int MaxThreads, bool StartNewCustomThread)
{
    SubstageQueue s;
    s.Id = ThreadId;
    s.FunctionName = FunctionName;
    s.Parameters = Parameters;
    s.StopThreadAfterTaskFinish = StopThreadAfterTaskFinish;
    s.MaxThreads = MaxThreads;
    s.StartNewCustomThread = StartNewCustomThread;
    s.StartIndex = StartIndex;
    s.StartPostponedOnIdle = StartPostponedOnIdle;
    StartIndex++;
    StagesQueue.append(s);
}

int SubstageManager::CustomThreadsTotal()
{
    int Total = 0;
    for(Substage s:Stages)
    {
        if(StagesInfo.contains(s.Id))
        {
            Total++;
        }
    }
    return Total;
}

bool SubstageManager::CustomThreadsNextQueuedFunction(int& ThreadId, QString& FunctionName, QString& Parameters, bool& StopThreadAfterTaskFinish, bool &StartPostponedOnIdle, int& MaxThreads, bool StartNewCustomThread, int InputThreadId)
{
    //Remove queue items, which parent thread is closed
    {
        QList<SubstageQueue>::iterator i = StagesQueue.begin();
        while (i != StagesQueue.end())
        {
            SubstageQueue s = *i;
            if(!s.StartNewCustomThread && !Stages.contains(s.Id))
            {
                i = StagesQueue.erase(i);
            }
            else
                ++i;
        }
    }


    if(StartNewCustomThread)
    {
        int TotalThreads = CustomThreadsTotal();
        QList<SubstageQueue>::iterator i = StagesQueue.begin();
        while (i != StagesQueue.end())
        {
            SubstageQueue s = *i;
            if(s.MaxThreads > TotalThreads)
            {
                ThreadId = i->Id;
                FunctionName = i->FunctionName;
                Parameters = i->Parameters;
                StopThreadAfterTaskFinish = i->StopThreadAfterTaskFinish;
                MaxThreads = i->MaxThreads;
                StartPostponedOnIdle = i->StartPostponedOnIdle;
                i = StagesQueue.erase(i);
                return true;
            }
            else
                ++i;
        }
        return false;
    }else
    {
        //Find min start index
        int MinIndex = -1;
        bool FoundAtLeastOne = false;
        QList<SubstageQueue>::iterator i = StagesQueue.begin();
        while (i != StagesQueue.end())
        {
            SubstageQueue s = *i;
            if(InputThreadId == s.Id && ( s.StartIndex < MinIndex || !FoundAtLeastOne ) )
            {
                FoundAtLeastOne = true;
                MinIndex = s.StartIndex;
            }
            ++i;
        }

        //Remove it and set result
        if(FoundAtLeastOne)
        {
            QList<SubstageQueue>::iterator i = StagesQueue.begin();
            while (i != StagesQueue.end())
            {
                SubstageQueue s = *i;
                if(InputThreadId == s.Id && s.StartIndex == MinIndex)
                {
                    ThreadId = i->Id;
                    FunctionName = i->FunctionName;
                    Parameters = i->Parameters;
                    StopThreadAfterTaskFinish = i->StopThreadAfterTaskFinish;
                    MaxThreads = i->MaxThreads;
                    StartPostponedOnIdle = i->StartPostponedOnIdle;
                    i = StagesQueue.erase(i);
                    return true;
                }
                else
                    ++i;
            }
        }else if(CustomThreadGetStartPostponedOnIdle(InputThreadId))
        {
            //This thread can run postponded tasks

            QList<SubstageQueue>::iterator i = StagesQueue.begin();
            while (i != StagesQueue.end())
            {
                SubstageQueue s = *i;
                if(s.StartNewCustomThread)
                {
                    ThreadId = i->Id;
                    FunctionName = i->FunctionName;
                    Parameters = i->Parameters;
                    StopThreadAfterTaskFinish = i->StopThreadAfterTaskFinish;
                    MaxThreads = i->MaxThreads;
                    StartPostponedOnIdle = i->StartPostponedOnIdle;
                    i = StagesQueue.erase(i);
                    return true;
                }
                else
                    ++i;
            }
        }
        return false;
    }
}

void SubstageManager::CustomThreadRemoveQueue(int ThreadId)
{
    QList<SubstageQueue>::iterator i = StagesQueue.begin();
    while (i != StagesQueue.end())
    {
        SubstageQueue s = *i;
        if(s.Id == ThreadId)
        {
            i = StagesQueue.erase(i);
        }
        else
            ++i;
    }
}

bool SubstageManager::CustomThreadIsQueue(int ThreadId)
{
    QList<SubstageQueue>::iterator i = StagesQueue.begin();
    while (i != StagesQueue.end())
    {
        SubstageQueue s = *i;
        if(s.Id == ThreadId)
        {
            return true;
        }
        else
            ++i;
    }

    return false;
}

bool SubstageManager::CustomThreadCanStartInIddleThread()
{
    for(Substage s:Stages)
    {
        if(StagesInfo.contains(s.Id))
        {
            SubstageInfo& i = StagesInfo[s.Id];
            if(!i.CustomThreadsIsRunning && i.CustomThreadsStartPostponedOnIdle)
            {
                return true;
            }
        }
    }
    return false;
}

int SubstageManager::CustomThreadGetIddleThread()
{
    for(Substage s:Stages)
    {
        if(StagesInfo.contains(s.Id))
        {
            SubstageInfo& i = StagesInfo[s.Id];
            if(!i.CustomThreadsIsRunning && i.CustomThreadsStartPostponedOnIdle)
            {
                return i.Id;
            }
        }
    }
    return 0;
}

