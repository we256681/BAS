#include "profilebackgroundremover.h"
#include <QDir>
#include <QFileInfo>


ProfileBackgroundRemover::ProfileBackgroundRemover(QObject *parent) : QObject(parent)
{
    _Timer = new QTimer(this);
}

void RemoveOldTempProfiles()
{
    QDirIterator it("prof", QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    while (it.hasNext())
    {
        QString dir = it.next();
        QString FilePath = dir + "/lockfile";

        QFile(FilePath).remove();

        if(!QFileInfo(FilePath).exists())
        {
            QDir(dir).removeRecursively();
        }
    }
}

void ProfileBackgroundRemover::Timer()
{
    QDateTime Now = QDateTime::currentDateTime();

    if(IsRunning)
    {
        if(!Future.isRunning())
        {
            IsRunning = false;
            EndedAtLeastOnce = true;
            LastEndTime = Now;
        }
    }

    bool CanStart = true;

    if(IsRunning)
        CanStart = false;

    if(EndedAtLeastOnce && LastEndTime.secsTo(Now) < 10)
        CanStart = false;

    if(CanStart)
    {
        Future = QtConcurrent::run(RemoveOldTempProfiles);
        IsRunning = true;
    }
}

void ProfileBackgroundRemover::Run(bool immediate)
{
    _Timer->setInterval(10000);
    _Timer->setSingleShot(false);
    connect(_Timer,SIGNAL(timeout()),this,SLOT(Timer()));
    if (immediate) Timer();
    _Timer->start();
}
