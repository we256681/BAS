#ifndef RESUMEDOWNLOADER_H
#define RESUMEDOWNLOADER_H

#include <QObject>
#include <QTimer>
#include "ihttpclient.h"

class ResumeDownloader : public QObject
{
    Q_OBJECT
    IHttpClient *Client = 0;

    /* Settings */
    qint64 ChunkSize = 10485760 /* 10 mb */;
    int RetryCount = 30;
    int RetryInterval = 2000 /* 2 seconds */;
    int MaximumDownloadTimeForOneChunk = 1200000 /* 20 minutes */;
    int MaximumDownloadTimeForSimpleRequest = 10000 /* 10 seconds */;
    int CheckSpeedInterval = 20000 /* 20 seconds */;
    qint64 CheckSpeedMinimumSize = 307200 /* 300 kb */;

    QTimer* CheckSpeedTimer = 0;
    qint64 CheckSpeedLastValue = 0;
    qint64 CheckSpeedCurrentValue = 0;

    bool _WasError = false;
    QString _LastError;
    QString Url;
    QString MetaUrl;

    bool IsMeta = false;
    qint64 TotalSize = 0;
    QStringList Chunks;
    QString Checksum;
    QString AlternativeUrl;

    QList<QByteArray> ResultChunks;
    qint64 CurrentDownloadPosition = 0;
    qint64 CurrentRetryCount = 0;
    qint64 CurrentChunkIndex = 0;


public:
    explicit ResumeDownloader(QObject *parent = nullptr);
    void Init(IHttpClient *Client);
    void Get(const QString &Url);

    bool WasError();
    QString GetErrorString();
    QList<QByteArray>* GetPageData();

private slots:
    void DownloadMetaResult();
    void StartDownloadNextChunk();
    void DownloadNextChunk();
    void FinishDownloadNextChunk();
    void DownloadProgressSlot(qint64 BytesReceived, qint64 BytesTotal);
    void TotalSizeResult();
    void CheckSpeed();

signals:
    void Finished();
    void Log(QString);
    void DownloadProgress(qint64 BytesReceived, qint64 BytesTotal);

};

#endif // RESUMEDOWNLOADER_H
