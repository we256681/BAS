#include "browserextensionmanager.h"
#include <QDirIterator>
#include <QDir>
#include <QRegularExpression>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include "JlCompress.h"
#include <bitset>
#include <QCryptographicHash>

#include "browserversionselector.h"

#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    BrowserExtensionManager::BrowserExtensionManager(QObject *parent) : IBrowserExtensionManager(parent)
    {
        QDir("x").mkpath(".");
        ClearCache();
    }

    qint64 BrowserExtensionManager::CurrentTime()
    {
        return QDateTime::currentDateTime().toMSecsSinceEpoch();
    }

    qint64 BrowserExtensionManager::GetCachedFolderDontStartTime(const QString& Folder)
    {
        QString TimeFilePath = QString("x/") + Folder + QString("/time.txt");
        QFile TimeFile(TimeFilePath);

        if(TimeFile.exists())
        {
            QStringList TimeFileContent = ReadFile(TimeFilePath).split("-");
            if(TimeFileContent.size() == 3)
            {
                QString Data = TimeFileContent.at(0);
                return Data.toLongLong();
            }

        }
        return 0;
    }

    qint64 BrowserExtensionManager::GetCachedFolderDeleteTime(const QString& Folder)
    {
        QString TimeFilePath = QString("x/") + Folder + QString("/time.txt");
        QFile TimeFile(TimeFilePath);

        if(TimeFile.exists())
        {
            QStringList TimeFileContent = ReadFile(TimeFilePath).split("-");
            if(TimeFileContent.size() == 3)
            {
                QString Data = TimeFileContent.at(1);
                return Data.toLongLong();
            }

        }
        return 0;
    }

    qint64 BrowserExtensionManager::GetCachedFolderManifestMissingDeleteDeleteTime(const QString& Folder)
    {
        QString TimeFilePath = QString("x/") + Folder + QString("/time.txt");
        QFile TimeFile(TimeFilePath);

        if(TimeFile.exists())
        {
            QStringList TimeFileContent = ReadFile(TimeFilePath).split("-");
            if(TimeFileContent.size() == 3)
            {
                QString Data = TimeFileContent.at(2);
                return Data.toLongLong();
            }

        }
        return 0;
    }

    void BrowserExtensionManager::SaveCachedFolderTime(const QString& Folder)
    {
        qint64 ManifestMissingDeleteDeleteTime = CurrentTime() + 10 * 60 * 1000;
        qint64 DeleteTime = CurrentTime() + 2 * 24 * 60 * 60 * 1000;
        qint64 DontStartTime = CurrentTime() + 24 * 60 * 60 * 1000;
        QString TimeFilePath = QString("x/") + Folder + QString("/time.txt");
        WriteFile(TimeFilePath, QString::number(DontStartTime) + QString("-") + QString::number(DeleteTime) + QString("-") + QString::number(ManifestMissingDeleteDeleteTime));
    }

    QStringList BrowserExtensionManager::GetCachedFolders(const QString& Mask)
    {
        QStringList Result;
        QDirIterator it("x", QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
        while (it.hasNext())
        {
            QString FilePath = QFileInfo(it.next()).fileName();
            if(Mask.isEmpty() || FilePath.toLower().startsWith(Mask.toLower()))
            {
                Result.push_back(FilePath);
            }
        }
        return Result;
    }

    QString BrowserExtensionManager::ReadFile(const QString& Path)
    {
        QFile f(Path);
        if (!f.open(QFile::ReadOnly))
            return QString();
        QString res = QString::fromUtf8(f.readAll());
        f.close();
        return res;
    }

    void BrowserExtensionManager::WriteFile(const QString& Path,const QString& Data)
    {
        QFile f(Path);
        if(f.open(QIODevice::WriteOnly))
        {
            f.write(Data.toUtf8());
        }
        f.close();
    }

    void BrowserExtensionManager::ClearCache()
    {
        qint64 Now = CurrentTime();

        for(const QString& Folder: GetCachedFolders())
        {
            qint64 DeleteTime = GetCachedFolderDeleteTime(Folder);
            qint64 ManifestMissingDeleteDeleteTime = GetCachedFolderManifestMissingDeleteDeleteTime(Folder);

            QString ManifestFilePath = QString("x/") + Folder + QString("/manifest.json");
            bool IsManifestMissing = !QFile(ManifestFilePath).exists();

            if(
                    //Folder exists and delete time
                    (DeleteTime > 0 && Now > DeleteTime) ||
                    //Probably bad install
                    (IsManifestMissing && Now > ManifestMissingDeleteDeleteTime)
            )
            {
                QDir(QString("x/") + Folder).removeRecursively();
            }
        }
    }

    QString BrowserExtensionManager::InitCacheFolder(const QString &ExtensionId)
    {
        QString FolderName = ExtensionId.mid(0,6) + QString(".") + RandomString(6);
        QString FolderPath = QString("x/") + FolderName;
        QDir(FolderPath).mkpath(".");
        SaveCachedFolderTime(FolderName);
        return FolderPath;
    }


    QString BrowserExtensionManager::Require(const QString& Extension, bool& IsFinishedInstantly, bool& IsInstantFailure)
    {
        QList<PendingRequest> ExtensionRequest = ParseExtension(Extension);

        if(ExtensionRequest.isEmpty())
        {
            //Nothing to parse - return empty string
            IsFinishedInstantly = true;
            IsInstantFailure = false;
            return QString();
        }

        //If at least one error during parse - fail
        for(PendingRequest& Item: ExtensionRequest)
        {
            if(Item.Type == Unknown)
            {
                IsFinishedInstantly = true;
                IsInstantFailure = true;
                return tr("Failed to parse extension data");
            }
        }

        //Process pending requests
        Request Req;
        Req.Id = RandomString(8);
        bool IsFinishedInstant = true;
        for(PendingRequest& Item: ExtensionRequest)
        {
            ResultItem Res;
            Res.ExtensionId = Item.ExtensionId;
            if(Item.Type == FileName)
            {
                Res.ExtensionPath = Item.ExtensionPath;
                Res.IsFinished = true;
            }else if(Item.Type == ExtensionId)
            {
                QString MaybePathToExtension = TryToGetExtensionFromCache(Item.ExtensionId);
                if(MaybePathToExtension.isEmpty())
                {
                    //No extension in cache, need to make request
                    Res.IsFinished = false;
                    IsFinishedInstant = false;
                    AddExtensionToQueue(Item);
                }else
                {
                    //Request found in cache, don't need to make request
                    Res.ExtensionPath = MaybePathToExtension;
                    Res.IsFinished = true;
                }

            }
            Req.Items.append(Res);
        }

        if(IsFinishedInstant)
        {
            //No need to make any request, can return instantly
            QStringList ResultPathList;
            for(ResultItem& Item: Req.Items)
            {
                ResultPathList.append(Item.ExtensionPath);
            }
            IsFinishedInstantly = true;
            IsInstantFailure = false;
            return ResultPathList.join("\r\n");
        }else
        {
            //Request will be made, return id
            IsFinishedInstantly = false;
            IsInstantFailure = false;
            Requests.append(Req);
            return Req.Id;
        }

        return QString();
    }


    void BrowserExtensionManager::SetHttpClientFactory(IHttpClientFactory *HttpClientFactory)
    {
        this->HttpClientFactory = HttpClientFactory;
    }

    void BrowserExtensionManager::OnTimer()
    {
        //Interrupt request if it takes too long
        QDateTime Now = QDateTime::currentDateTime();
        if(!PendingRequestQueue.isEmpty() && StartedAt.secsTo(Now) > 60)
        {
            if(HttpClient)
            {
                HttpClient->deleteLater();
                HttpClient = 0;
            }

            PendingRequest& Request = PendingRequestQueue[0];

            QDir(Request.ExtensionPath).removeRecursively();
            FailExtensionById(Request.ExtensionId,tr("Request timeout"));
            StartNextExtensionDownload();
        }
    }

    QString BrowserExtensionManager::RandomString(int size)
    {
        const QString possibleCharacters("abcdefghijklmnopqrstuvwxyz");
        const int randomStringLength = size;

        QString randomString;
        for(int i=0; i<randomStringLength; ++i)
        {
            int index = qrand() % possibleCharacters.length();
            QChar nextChar = possibleCharacters.at(index);
            randomString.append(nextChar);
        }
        return randomString;
    }

    QString BrowserExtensionManager::TryToGetExtensionFromCache(const QString &ExtensionId)
    {
        for(QString &Folder: GetCachedFolders(ExtensionId.mid(0,6) + QString(".")))
        {
            QString TimeFilePath = QString("x/") + Folder + QString("/time.txt");
            QFile TimeFile(TimeFilePath);

            QString ManifestFilePath = QString("x/") + Folder + QString("/manifest.json");
            QFile ManifestFile(ManifestFilePath);


            //Delete file time and manifest exists
            if(TimeFile.exists() && ManifestFile.exists() && CurrentTime() < GetCachedFolderDontStartTime(Folder))
            {
                return QFileInfo(QString("x/") + Folder).absoluteFilePath();
            }
        }
        return QString();
    }
    QList<BrowserExtensionManager::PendingRequest> BrowserExtensionManager::ParseExtension(const QString &AllExtensionData)
    {
        QList<BrowserExtensionManager::PendingRequest> All;

        QRegularExpression RegexpId("^[a-z]{32}$");
        QRegularExpression RegexpPath("^[a-zA-Z]:");

        QStringList ExtensionDataList = AllExtensionData.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
        QStringList ExtensionsIds;
        for(QString &ExtensionData: ExtensionDataList)
        {
            ExtensionData = ExtensionData.trimmed();
            if(ExtensionData.isEmpty())
                continue;
            PendingRequest Result;
            Result.Type = Unknown;
            Result.ExtensionData = ExtensionData;

            if(ExtensionData.startsWith("https://chrome.google.com"))
            {
                QRegularExpression Regexp("https:\\/\\/chrome\\.google\\.com\\/webstore\\/.+?\\/([a-z]{32})");
                QRegularExpressionMatch Match = Regexp.match(ExtensionData);
                if(Match.hasMatch())
                {
                    Result.Type = ExtensionId;
                    Result.ExtensionId = Match.captured(1);
                }
            }else if(ExtensionData.size() == 32 && RegexpId.match(ExtensionData).hasMatch())
            {
                Result.Type = ExtensionId;
                Result.ExtensionId = ExtensionData;

            }else if(RegexpPath.match(ExtensionData).hasMatch())
            {
                Result.Type = FileName;
                Result.ExtensionPath = ExtensionData;
            }

            if(Result.Type != ExtensionId || !ExtensionsIds.contains(Result.ExtensionId))
            {
                All.append(Result);

                if(Result.Type == ExtensionId)
                {
                    ExtensionsIds.append(Result.ExtensionId);
                }
            }
        }
        return All;
    }
    void BrowserExtensionManager::DownloadExtension()
    {
        if(PendingRequestQueue.empty())
            return;

        //Start timer to interrupt on timeout
        if(!Timer)
        {
            Timer = new QTimer(this);
            Timer->setSingleShot(false);
            Timer->setInterval(10000);
            connect(Timer,SIGNAL(timeout()),this,SLOT(OnTimer()));
            Timer->start();
        }

        //Using only latest browser version to download extension
        //Practically using different version will still give same file
        //TODO: use only current browser version.
        QString DefaultVersionFull = QString("111.0.5563.65");
        BrowserVersionSelector _BrowserVersionSelector;
        IBrowserVersionSelector::BrowserItem _BrowserItem = _BrowserVersionSelector.GetDefaultBrowserItem();
        if(!_BrowserItem.IsNull)
        {
            DefaultVersionFull = _BrowserItem.VersionFull;
        }

        //Get http client
        if(HttpClient)
        {
            HttpClient->deleteLater();
        }
        HttpClient = HttpClientFactory->GetHttpClient();

        StartedAt = QDateTime::currentDateTime();

        PendingRequest& Request = PendingRequestQueue[0];

        Request.ExtensionPath = InitCacheFolder(Request.ExtensionId);

        HttpClient->Connect(this,SLOT(OnDoneHttpRequest()));

        QString ExtensionUrl = QString("https://clients2.google.com/service/update2/crx?response=redirect&prodversion=") + DefaultVersionFull + QString("&acceptformat=crx2,crx3&x=id%3D") + Request.ExtensionId + QString("%26uc&acceptformat=crx2,crx3");
        QString ExtensionPath = Request.ExtensionPath + QString("/temp.crx");
        HttpClient->Download(ExtensionUrl, ExtensionPath);
    }

    void BrowserExtensionManager::AddExtensionToQueue(const PendingRequest& Extension)
    {
        for(PendingRequest& Request: PendingRequestQueue)
        {
            //Extension is already scheduled to download
            if(Request.ExtensionId == Extension.ExtensionId)
                return;
        }

        PendingRequestQueue.append(Extension);
        if(PendingRequestQueue.size() == 1)
        {
            DownloadExtension();
        }
    }

    void BrowserExtensionManager::StartNextExtensionDownload()
    {
        if(PendingRequestQueue.empty())
            return;
        PendingRequestQueue.removeFirst();
        if(!PendingRequestQueue.isEmpty())
        {
            DownloadExtension();
        }
    }

    void BrowserExtensionManager::OnDoneHttpRequest()
    {
        bool WasError = HttpClient->WasError();
        QString ErrorString;
        if(WasError)
        {
            ErrorString = HttpClient->GetErrorString();
        }
        int Status = HttpClient->GetStatus();
        QString Location = HttpClient->GetHeader("Location");

        HttpClient->deleteLater();
        HttpClient = 0;

        if(PendingRequestQueue.empty())
        {
            return;
        }

        PendingRequest& Request = PendingRequestQueue[0];

        if(WasError)
        {
            QDir(Request.ExtensionPath).removeRecursively();
            FailExtensionById(Request.ExtensionId,ErrorString);
            StartNextExtensionDownload();
            return;
        }

        //Follow redirect
        if(Status >= 300 && Status < 400 && !Location.isEmpty())
        {
            HttpClient = HttpClientFactory->GetHttpClient();

            QString CrxPath = Request.ExtensionPath + QString("/temp.crx");

            QFile(CrxPath).remove();

            HttpClient->Connect(this,SLOT(OnDoneHttpRequest()));
            HttpClient->Download(Location, CrxPath);
            return;
        }

        QByteArray Header;

        bool IsSuccess = RemoveHeaderFromCRX(Header);
        if(!IsSuccess)
        {
            QDir(Request.ExtensionPath).removeRecursively();
            FailExtensionById(Request.ExtensionId, tr("Failed to remove header from crx file"));
            StartNextExtensionDownload();
            return;
        }

        QString PublicKey = ExtractPublicKeyFromCRX(Header);
        if(PublicKey.isEmpty())
        {
            QDir(Request.ExtensionPath).removeRecursively();
            FailExtensionById(Request.ExtensionId, tr("Failed to extract public key from crx file"));
            StartNextExtensionDownload();
            return;
        }

        IsSuccess = ExtractZip();
        if(!IsSuccess)
        {
            QDir(Request.ExtensionPath).removeRecursively();
            FailExtensionById(Request.ExtensionId, tr("Failed to extract zip file"));
            StartNextExtensionDownload();
            return;
        }

        QString ManifestFilePath = Request.ExtensionPath + QString("/manifest.json");
        if(!(QFile(ManifestFilePath).exists()))
        {
            QDir(Request.ExtensionPath).removeRecursively();
            FailExtensionById(Request.ExtensionId, tr("Manifest file doesn't exist"));
            StartNextExtensionDownload();
            return;
        }

        QString ManifestFileContent = ReadFile(ManifestFilePath);

        QJsonParseError err;

        QJsonDocument InputObject = QJsonDocument::fromJson(ManifestFileContent.toUtf8(),&err);
        if(err.error || !InputObject.isObject())
        {
            QDir(Request.ExtensionPath).removeRecursively();
            FailExtensionById(Request.ExtensionId, tr("Failed to parse manifest file"));
            StartNextExtensionDownload();
            return;
        }

        QJsonObject Obj = InputObject.object();
        if(!Obj.contains("key"))
        {
            Obj["key"] = PublicKey;
            InputObject.setObject(Obj);
            ManifestFileContent = QString::fromUtf8(InputObject.toJson(QJsonDocument::Indented));

            WriteFile(ManifestFilePath, ManifestFileContent);
        }

        SuccessExtensionById(Request.ExtensionId, QFileInfo(Request.ExtensionPath).absoluteFilePath());
        StartNextExtensionDownload();
    }

    QString BrowserExtensionManager::ExtractPublicKeyFromCRX(QByteArray& Header)
    {
        QDataStream Stream(&Header, QIODevice::ReadOnly);
        bool IsError = false;
        QList<QByteArray> Keys;
        QByteArray ExtensionId;
        while(true)
        {
            if(Stream.atEnd())
                break;

            unsigned int FieldId = CRXReadFieldId(Stream, IsError);
            if(IsError)
                return QString();

            if(FieldId == 2)
            {
                QByteArray KeyMessage = CRXReadData(Stream,IsError);
                if(IsError)
                    return QString();

                QDataStream KeyStream(&KeyMessage, QIODevice::ReadOnly);
                QByteArray Key;

                while(true)
                {
                    if(KeyStream.atEnd())
                        break;

                    unsigned int KeyFieldId = CRXReadFieldId(KeyStream, IsError);
                    if(IsError)
                        return QString();

                    if(KeyFieldId == 1)
                    {
                        Key = CRXReadData(KeyStream,IsError);
                        if(IsError)
                            return QString();
                    }else
                    {
                        CRXReadData(KeyStream,IsError);
                        if(IsError)
                            return QString();
                    }
                }

                if(Key.isEmpty())
                    return QString();

                Keys.append(Key);
            }else if(FieldId == 3)
            {
                CRXReadData(Stream,IsError);
                if(IsError)
                    return QString();
            }else if(FieldId == 10000)
            {

                QByteArray ExtensionIdMessage = CRXReadData(Stream,IsError);
                if(IsError)
                    return QString();
                QDataStream ExtensionIdStream(&ExtensionIdMessage, QIODevice::ReadOnly);
                unsigned int ExtensionIdFieldId = CRXReadFieldId(ExtensionIdStream, IsError);
                if(IsError)
                    return QString();
                if(ExtensionIdFieldId == 1)
                {
                    ExtensionId = CRXReadData(ExtensionIdStream,IsError);
                    if(IsError)
                        return QString();
                }else
                {
                    return QString();
                }


            }else
            {
                CRXReadData(Stream,IsError);
                if(IsError)
                    return QString();
            }

        }

        for(QByteArray& Key: Keys)
        {
            QCryptographicHash Hash(QCryptographicHash::Sha256);
            Hash.addData(Key);
            QByteArray CandidateSignature = Hash.result().mid(0,16);
            if(CandidateSignature == ExtensionId && !ExtensionId.isEmpty())
                return QString::fromUtf8(Key.toBase64());
        }

        return QString();
    }

    unsigned int BrowserExtensionManager::CRXReadFieldId(QDataStream& Header, bool& IsError)
    {
        IsError = false;
        unsigned int FieldData = CRXReadVarint(Header, IsError);
        if(IsError)
            return 0;

        std::bitset<32> FieldDataBits(FieldData);
        std::bitset<32> ResultBits(0);
        for(int i = 3;i<32;i++)
        {
            ResultBits[i-3] = FieldDataBits[i];
        }
        return ResultBits.to_ulong();
    }

    unsigned int BrowserExtensionManager::CRXReadVarint(QDataStream& Header, bool& IsError)
    {
        IsError = false;
        std::bitset<32> Res(0);
        int ResIndex = 0;

        while(true)
        {
            if(Header.atEnd())
            {
                IsError = true;
                return 0;
            }
            unsigned char FirstByte = 0;
            Header>>FirstByte;
            std::bitset<8> Bits(FirstByte);
            bool MSB = Bits[7];
            for(int i = 0;i<7;i++)
            {
                Res[ResIndex] = Bits[i];
                ResIndex++;
                if(ResIndex >= 32)
                {
                    IsError = true;
                    return 0;
                }
            }
            if(!MSB)
                break;

        }

        return Res.to_ulong();
    }

    QByteArray BrowserExtensionManager::CRXReadData(QDataStream& Header, bool& IsError)
    {
        IsError = false;
        unsigned int Length = CRXReadVarint(Header, IsError);
        if(IsError)
            return QByteArray();

        QByteArray Data;

        Data.resize(Length);

        int ActualLength = Header.readRawData(Data.data(),Length);

        if(ActualLength != Length)
        {
            IsError = true;
            return QByteArray();
        }

        return Data;
    }

    bool BrowserExtensionManager::RemoveHeaderFromCRX(QByteArray& Header)
    {
        Header.clear();

        if(PendingRequestQueue.empty())
            return false;

        PendingRequest& Request = PendingRequestQueue[0];

        QString CrxPath = Request.ExtensionPath + QString("/temp.crx");
        QString ZipPath = Request.ExtensionPath + QString("/temp.zip");

        QFile f(CrxPath);
        if (!f.open(QFile::ReadOnly))
        {
            f.close();
            return false;
        }
        QByteArray Info = f.read(12);
        if(Info.size() != 12)
        {
            f.close();
            return false;
        }

        unsigned char * InfoUnsigned = (unsigned char *)Info.data();

        if(!(InfoUnsigned[0] == 67 && InfoUnsigned[1] == 114 && InfoUnsigned[2] == 50 && InfoUnsigned[3] == 52))
            return false;

        int HeaderLength = InfoUnsigned[8] + InfoUnsigned[9] * 256 + InfoUnsigned[10] * 65536 + InfoUnsigned[11] * 16777216;

        Header = f.read(HeaderLength);

        if(Header.size() != HeaderLength)
        {
            f.close();
            return false;
        }

        QFile fz(ZipPath);
        if (!fz.open(QFile::WriteOnly))
        {
            fz.close();
            f.close();
            return false;
        }

        int ChunkSize = 10 * 1024;
        while(!f.atEnd())
        {
            QByteArray Chunk = f.read(ChunkSize);
            fz.write(Chunk);
        }

        fz.close();
        f.close();


        return true;
    }

    bool BrowserExtensionManager::ExtractZip()
    {
        if(PendingRequestQueue.empty())
            return false;

        PendingRequest& Request = PendingRequestQueue[0];

        QString ZipPath = Request.ExtensionPath + QString("/temp.zip");

        QString CrxPath = Request.ExtensionPath + QString("/temp.crx");

        if(JlCompress::extractDir(QFileInfo(ZipPath).absoluteFilePath(), QFileInfo(Request.ExtensionPath).absoluteFilePath()).isEmpty())
        {
            return false;
        }

        QFile(ZipPath).remove();
        QFile(CrxPath).remove();

        return true;
    }


    void BrowserExtensionManager::FailExtensionById(const QString &ExtensionId, const QString &Error)
    {
        QList<QPair<QString,QString> > Items;

        QMutableListIterator<Request> i(Requests);
        while (i.hasNext())
        {
            Request& Req = i.next();
            bool HasItem = false;
            for(ResultItem& Res: Req.Items)
            {
                if(Res.ExtensionId == ExtensionId)
                {
                    HasItem = true;
                    break;
                }
            }
            if(HasItem)
            {
                QPair<QString,QString> Item;
                Item.first = Req.Id;
                Item.second = Error;
                Items.append(Item);
                i.remove();
            }
        }
        for(QPair<QString,QString>& Item: Items)
            emit RequireRequestDone(Item.first, Item.second, true);
    }

    void BrowserExtensionManager::SuccessExtensionById(const QString &ExtensionId, const QString &Path)
    {
        QList<QPair<QString,QString> > Items;
        QMutableListIterator<Request> i(Requests);
        while (i.hasNext())
        {
            Request& Req = i.next();

            bool AllFinished = true;
            for(ResultItem& Res: Req.Items)
            {
                if(Res.ExtensionId == ExtensionId)
                {
                    Res.ExtensionPath = Path;
                    Res.IsFinished = true;
                }
                if(!Res.IsFinished)
                {
                    AllFinished = false;
                }
            }
            if(AllFinished)
            {
                QStringList ResultList;
                for(ResultItem& Res: Req.Items)
                {
                    ResultList.append(Res.ExtensionPath);
                }
                QPair<QString,QString> Item;
                Item.first = Req.Id;
                Item.second = ResultList.join("\r\n");
                Items.append(Item);
                i.remove();
            }
        }
        for(QPair<QString,QString>& Item: Items)
            emit RequireRequestDone(Item.first, Item.second, false);
    }

}
