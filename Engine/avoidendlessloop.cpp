#include "avoidendlessloop.h"

AvoidEndlessLoop::AvoidEndlessLoop()
{
    MaxItems = 30;
    PeriodMSeconds = 1000;
}

void AvoidEndlessLoop::Init(int PeriodMSeconds,int MaxItems)
{
    this->PeriodMSeconds = PeriodMSeconds;
    this->MaxItems = MaxItems;
}

//false - no sleep
//true - sleep
bool AvoidEndlessLoop::Run()
{
    bool HasLastRun = false;
    bool IsOverflow = false;
    QDateTime LastRun;
    QDateTime Now = QDateTime::currentDateTime();
    while(Usages.length() > MaxItems)
    {
        IsOverflow = true;
        Usages.removeFirst();
    }

    if(!Usages.empty())
    {
        HasLastRun = true;
        LastRun = Usages.first();
    }

    Usages.append(Now);

    if(!IsOverflow)
    {
        return false;
    }

    bool Res = LastRun > Now.addMSecs(-PeriodMSeconds);

    if(Res)
    {
        Usages.clear();
    }

    return Res;
}
