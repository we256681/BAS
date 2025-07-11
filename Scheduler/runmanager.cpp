#include "runmanager.h"
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QProcess>
#include <QLockFile>
#include <QFile>
#include <QLocale>
#include "schedulerinstaller.h"

RunManager::RunManager(QObject *parent) : QObject(parent)
{

}

QString RunManager::CheckUpdateReady()
{
    QString CurrentFolder = QDir::currentPath();
    QString ParentPath = QDir::cleanPath(CurrentFolder + "/../");

    QDirIterator DirIterator(ParentPath, QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);

    QString CurrentDirString = QFileInfo(CurrentFolder).fileName();
    ProjectVersion LatestProject;
    while (DirIterator.hasNext())
    {
        QString Dir = DirIterator.next();
        QFileInfo DirInfo(Dir);
        QString DirString = DirInfo.fileName();
        ProjectVersion CurrentProject(DirString);
        if(CurrentProject > LatestProject && QFileInfo(QDir(Dir).filePath("installed.txt")).exists())
        {
            LatestProject = CurrentProject;
        }
    }
    if(LatestProject > ProjectVersion(CurrentDirString))
        return LatestProject.ToString();

    return QString();
}

void RunManager::Restart()
{
    SchedulerInstaller Installer;
    Installer.Restart(LockFile);
    LockFile = 0;
    qApp->quit();
}

void RunManager::Uninstall()
{
    SchedulerInstaller Installer;
    Installer.RemoveFromScheduler();
    qApp->quit();
}


bool RunManager::OnStart(const QStringList& Params)
{

    if(Params.contains("--install"))
    {
        SchedulerInstaller Installer;
        Installer.Install();
        return false;
    }

    if(Params.contains("--uninstall"))
    {
        SchedulerInstaller Installer;
        Installer.Uninstall();
        return false;
    }

    QString CurrentFolder = QDir::currentPath();
    QString ParentPath = QDir::cleanPath(CurrentFolder + "/../");

    QString CurrentDirString = QFileInfo(CurrentFolder).fileName();
    ProjectVersion LatestProject(CurrentDirString);

    if(LatestProject.IsNull())
    {
        //Stop current process if latest dir has bad format
        return false;
    }

    QString SchedulerGuiExeLatest = QDir::cleanPath(ParentPath + "/" + LatestProject.ToString() + "/gui/SchedulerGui.exe");
    QString LatestProjectGuiPath = QDir::cleanPath(ParentPath + "/" + LatestProject.ToString() + "/gui");


    if(Params.contains("--browser"))
    {
        //Delete input params if clear run
        if(!Params.contains("--input-done"))
        {
            QFile::remove(LatestProjectGuiPath + "/html/js/inputs.js");
        }

        //Start browser and close current project
        QString Language = QString("--lang-") + ((QLocale::system().name().split("_").first().toLower() == "ru") ? "ru" : "en");
        QProcess::startDetached(SchedulerGuiExeLatest,QStringList()<<Language,LatestProjectGuiPath);
        return false;
    }

    //Current project is latest, just run it
    //Allow to run only single instance
    QString LockPath = QDir::cleanPath(ParentPath + "/instance.lock");
    LockFile = new QFile(LockPath);
    LockFile->remove();
    if(!LockFile->exists() && LockFile->open(QIODevice::WriteOnly))
    {
        return true;
    }
    return false;

}

