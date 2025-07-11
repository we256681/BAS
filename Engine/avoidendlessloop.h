#ifndef AVOIDENDLESSLOOP_H
#define AVOIDENDLESSLOOP_H

#include <QList>
#include <QDateTime>

class AvoidEndlessLoop
{
    QList<QDateTime> Usages;
    int PeriodMSeconds;
    int MaxItems;
public:
    AvoidEndlessLoop();
    void Init(int PeriodMSeconds,int MaxItems);
    bool Run();
};

#endif // AVOIDENDLESSLOOP_H
