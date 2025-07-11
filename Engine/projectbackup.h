#ifndef PROJECTBACKUP_H
#define PROJECTBACKUP_H
#include "engine_global.h"
#include <QTimer>

namespace BrowserAutomationStudioFramework
{

    class ENGINESHARED_EXPORT ProjectBackup : public QObject
    {
        Q_OBJECT
        int milliseconds;
        QString DestFolder;
        QTimer* Timer;
        QString CurrentFileName;
    public:
        explicit ProjectBackup(QObject *parent = 0);

        void SetPeriod(int milliseconds);
        void SetDestFolder(const QString& DestFolder);

        void Start();
    signals:
        void Backup(QString);
    private slots:
        void DoBackups();
        QString DoBackupsInternal();


    public slots:
        //Start backup manually and report in BackupDone signal
        void StartBackup();
        void CurrentFileNameHasChanged(QString Filename);

    signals:
        //BackupDone is called only if backup was initiated by StartBackup slot
        void BackupDone(QString);
    };
}

#endif // PROJECTBACKUP_H
