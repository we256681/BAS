#include "multitimer.h"
#include <QSettings>
#include "every_cpp.h"


namespace BrowserAutomationStudioFramework
{
    MultiTimer::MultiTimer(QObject *parent) :
        QObject(parent)
    {
        Timer = new QTimer(this);
        Timer->setSingleShot(false);
        QSettings Settings("settings.ini",QSettings::IniFormat);
        Timer->setInterval(Settings.value("MultiTimerTimeout",100).toInt());
        connect(Timer,SIGNAL(timeout()),this,SLOT(TimeoutSlot()));
        Timer->start();
    }

    void MultiTimer::TimeoutSlot()
    {
        QDateTime now = QDateTime::currentDateTime();
        emit Timeout(now);
    }

}
