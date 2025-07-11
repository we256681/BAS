#include "projectbackup.h"
#include <QDateTime>
#include <QDir>
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    ProjectBackup::ProjectBackup(QObject *parent) : QObject(parent)
    {
        milliseconds = 5 * 60000;
        Timer = 0;
        DestFolder = "../../projectbackups";

    }

    void ProjectBackup::SetPeriod(int milliseconds)
    {
        this->milliseconds = milliseconds;
    }
    void ProjectBackup::SetDestFolder(const QString& DestFolder)
    {
        this->DestFolder = DestFolder;
    }

    void ProjectBackup::Start()
    {
        Timer = new QTimer(this);
        Timer->setInterval(milliseconds);
        Timer->setSingleShot(false);
        connect(Timer,SIGNAL(timeout()),this,SLOT(DoBackups()));
        Timer->start();
    }

    QString ProjectBackup::DoBackupsInternal()
    {
        QDateTime CurrentDateTime = QDateTime::currentDateTime();
        QDir dir(DestFolder + QDir::separator() + CurrentDateTime.toString("yyyy.MM.dd"));
        if(!dir.exists())
            dir.mkpath(".");
        QString FilePart;
        if(!CurrentFileName.isEmpty())
            FilePart = QFileInfo(CurrentFileName).baseName() + QString(".");
        QString path = dir.absoluteFilePath(FilePart + QString("%1.xml").arg(CurrentDateTime.toString("hh.mm.ss")));
        emit Backup(path);
        return path;
    }

    void ProjectBackup::CurrentFileNameHasChanged(QString Filename)
    {
        CurrentFileName = Filename;
    }

    void ProjectBackup::DoBackups()
    {
        DoBackupsInternal();
    }

    void ProjectBackup::StartBackup()
    {
        QString Res = DoBackupsInternal();
        emit BackupDone(Res);
    }



}
