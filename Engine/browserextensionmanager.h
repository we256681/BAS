#ifndef BROWSEREXTENSIONMANAGER_H
#define BROWSEREXTENSIONMANAGER_H

#include "engine_global.h"

#include "ibrowserextensionmanager.h"

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QDataStream>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT BrowserExtensionManager : public IBrowserExtensionManager
    {
        Q_OBJECT

        QDateTime StartedAt;

        enum RequestType
        {
          FileName,
          ExtensionId,
          Unknown
        };

        struct PendingRequest
        {
            RequestType Type;
            QString ExtensionData;
            QString ExtensionId;
            QString ExtensionPath;
        };

        struct ResultItem
        {
            QString ExtensionId;
            QString ExtensionPath;
            bool IsFinished = false;
        };

        struct Request
        {
            QList<ResultItem> Items;
            QString Id;
        };

        QList<PendingRequest> PendingRequestQueue;
        QList<Request> Requests;



        IHttpClientFactory *HttpClientFactory;
        IHttpClient *HttpClient = 0;
        QTimer* Timer = 0;
    public:
        explicit BrowserExtensionManager(QObject *parent = 0);
        void SetHttpClientFactory(IHttpClientFactory *HttpClientFactory);
        QString Require(const QString& Extension, bool& IsFinishedInstantly, bool& IsInstantFailure);
    signals:

    private slots:
        //For timeout calculation
        void OnTimer();
        void OnDoneHttpRequest();
    private:

        //Clear cached items


        //Helpers
        QStringList GetCachedFolders(const QString& Mask = QString());
        qint64 CurrentTime();
        qint64 GetCachedFolderDontStartTime(const QString& Folder);
        qint64 GetCachedFolderDeleteTime(const QString& Folder);
        qint64 GetCachedFolderManifestMissingDeleteDeleteTime(const QString& Folder);
        QString ReadFile(const QString& Path);
        void WriteFile(const QString& Path,const QString& Data);
        QString RandomString(int size);

        void ClearCache();
        QString InitCacheFolder(const QString &ExtensionId);
        void SaveCachedFolderTime(const QString& Folder);
        QString TryToGetExtensionFromCache(const QString &ExtensionId);
        QList<PendingRequest> ParseExtension(const QString &AllExtensionData);
        void DownloadExtension();
        void AddExtensionToQueue(const PendingRequest& Extension);
        void StartNextExtensionDownload();
        void FailExtensionById(const QString &ExtensionId, const QString &Error);
        void SuccessExtensionById(const QString &ExtensionId, const QString &Path);

        //Parse crx3.proto https://github.com/chromium/chromium/blob/e15cfb65a092b26224d6bdc685ed2aa9f41f7eb6/components/crx_file/crx3.proto
        QString ExtractPublicKeyFromCRX(QByteArray& Header);
        bool RemoveHeaderFromCRX(QByteArray& Header);
        bool ExtractZip();

        //Protobuf very simple, based on https://developers.google.com/protocol-buffers/docs/encoding#packed
        unsigned int CRXReadFieldId(QDataStream& Header, bool& IsError);
        unsigned int CRXReadVarint(QDataStream& Header, bool& IsError);
        QByteArray CRXReadData(QDataStream& Header, bool& IsError);
    };
}


#endif // BROWSEREXTENSIONMANAGER_H
