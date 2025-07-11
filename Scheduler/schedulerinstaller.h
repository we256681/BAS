#ifndef SCHEDULERINSTALLER_H
#define SCHEDULERINSTALLER_H

#include <QObject>
#include <QFile>

class SchedulerInstaller : public QObject
{
    Q_OBJECT
    QString InstallDir;
    QString RunnningVersion;
    QString ConsoleEncoding;
    public:
        explicit SchedulerInstaller(QObject *parent = 0);

    public:
        virtual void Install();
        virtual void Uninstall();
        virtual void Restart(QFile * LockFile);
        virtual void Run(const QString& StartType,
                         const QString& IniFileLocation = QString(),
                         const QString& ScriptData = QString(),
                         const QString& ScriptName = QString(),
                         const QString& ScriptLogin = QString(),
                         const QString& ScriptPass = QString());

        virtual QString GetDirectory();
        virtual QString GetVersion();

        void RemoveFromScheduler();


    private:
        void CopyFilesAndInstall();
        void UninstallInternal();


        void RemoveAllCorruptedVersions();
        // If empty string - no installation needed.
        QString FindInstallPath();
        QString FindLatestVersion();
        QString GetExeFolder();
        int VersionToInt(const QString& Version);
        void CopyFiles(const QString& From,const QString& To);
        void CopyFilesInThread(const QString& From,const QString& To);
        void ScheduleIfNeeded(const QString& LatestVersion = QString());
        bool CopyFileInternal(const QString& From,const QString& To,const QString& Filename);
        QString RunSchtasksAndGetResult(const QStringList& Params);
        void Log(const QString& Data);
        void Log(const QString& Text, const QString& Data);
        void Log(const QString& Text, int Data);
        void Log(const QString& Text, bool Data);
        void InitLog();
        QString GenerateXml(const QString& ExeLocation);
        void PreserveStartData(const QString& WorkingDir,
                               const QString& StartType,
                               const QString& IniFileLocation,
                               const QString& ScriptData,
                               const QString& ScriptName,
                               const QString& ScriptLogin,
                               const QString& ScriptPass);
        void DetectConsoleEncoding();
        QString EncodingIdToEncodingName(int EncodingId);


};


#endif // SCHEDULERINSTALLER_H
