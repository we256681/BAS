#ifndef REMOTE_H
#define REMOTE_H

#include <QObject>
#include <QTimer>
#include "httpclient.h"
#include "resumedownloader.h"
#include "downloadingwidget.h"

class Remote : public QObject
{
    Q_OBJECT
    HttpClient *Client;
    HttpClient *ClientForDownloader;
    ResumeDownloader *Downloader;
    QStringList Arguments;
    DownloadingWidget *UI;

    int KeepVersionNumber = 100;

    //Errors
    bool IsValid;
    QString ErrorMessage;

    //Initial params
    bool IsX64;
    bool IsSilent;
    bool IsResources;
    bool IsRemote;
    bool IsEnginesInAppData;
    QString ScriptName;
    QString ScriptPath;
    QString Server;
    QString Username;
    QString Password;
    QString SchedulerKey;
    QString ActualPath;


    //Script params
    QString ScriptHash;
    QString EngineVersion;
    bool ForceFree;

    QString GetGlobalDir(const QString& path);


public:
    explicit Remote(QObject *parent = 0);
    bool Start(const QStringList& Arguments);
    void DetectSettings();
    void DetectScriptProperties();
    void EnginePrepared();
signals:

public slots:
private slots:
    void Exit();
    void ScriptPropertiesHttpClientResp();
    void ScriptPropertiesDetected(const QString& ScriptHash, const QString& EngineVersion);
    void DownloadProgress(qint64 BytesReceived, qint64 BytesTotal);
    void DownloadLog(QString Text);
    void EngineDownloaded();

};

#endif // REMOTE_H
