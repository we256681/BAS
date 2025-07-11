#include "processdeleter.h"
#include <windows.h>

ProcessDeleter::ProcessDeleter(bool IsTemporaryProfile)
{
    Process = 0;
    this->IsTemporaryProfile = IsTemporaryProfile;
}

void ProcessDeleter::Start(QProcess * Process)
{
    Process->setParent(0);
    this->Process = Process;
    int DelayTime = qrand() % 5000;
    DelayTime += 10000;
    QTimer::singleShot(DelayTime, this, SLOT(Timer()));
}

void ProcessDeleter::Timer()
{
    if(Process)
    {
        Process->kill();
        Process->deleteLater();
    }
    deleteLater();
}
