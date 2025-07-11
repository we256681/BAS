#include "updater.h"
#include <QDir>
#include <QProcess>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>
#include "JlCompress.h"
#include "oldversionremover.h"

Updater::Updater(QObject *parent) :
    QObject(parent)
{
    Client = new HttpClient(this);
    ClientForDownloader = new HttpClient(this);
    Downloader = new ResumeDownloader(this);
    Downloader->Init(ClientForDownloader);
    connect(Downloader,SIGNAL(Finished()),this,SLOT(DownloadFinished()));
    connect(Downloader,SIGNAL(DownloadProgress(qint64,qint64)),this,SLOT(DownloadProgress(qint64,qint64)));

    CurrentVersion = GetLocalVersion();
    CanSkip = !CurrentVersion.isEmpty();
    Progress = 0;

}

QString Updater::GetLocalVersion()
{
    QDir dir = QDir("apps");
    QStringList list = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
    QStringList res;
    foreach(QString dir, list)
    {
        if(dir.startsWith("installing."))
        {
            continue;
        }
        res.append(dir);
    }

    return MaxVersion(res);
}

QString Updater::MaxVersion(const QStringList& Dirs)
{
    QString res;
    foreach(QString Dir, Dirs)
    {
        if(Dir == "temp")
            continue;

        if(res.isEmpty())
        {
            res = Dir;
            continue;
        }

        QStringList ds = Dir.split(".");
        QStringList rs = res.split(".");

        int l = qMin(ds.length(),rs.length());
        for(int i = 0;i<l;i++)
        {
            int di = ds.at(i).toInt();
            int ri = rs.at(i).toInt();
            if(di>ri)
            {
                res = Dir;
                break;
            }else if(di<ri)
            {
                break;
            }

        }

    }
    return res;
}

void Updater::Skip()
{
    qDebug()<<"Arguments Output"<<Arguments;
    OldVersionRemover Remover;
    Remover.Remove("apps");

    QProcess::startDetached(QString("apps/") + CurrentVersion + "/BrowserAutomationStudio.exe", Arguments , QString("./apps/") + CurrentVersion);
    if(Progress)
    {
        Progress->deleteLater();
        QCoreApplication::exit();
    }
}


bool Updater::Start(const QStringList& Arguments)
{
    this->Arguments = Arguments;
    if(!this->Arguments.isEmpty())
        this->Arguments.removeAt(0);
    if(this->Arguments.contains("--close"))
    {
        Skip();
        return false;
    }
    Progress = new UpdateProgress();
    connect(Progress,SIGNAL(Skip()),this,SLOT(Skip()));
    connect(Progress,SIGNAL(Update()),this,SLOT(Update()));
    Progress->SetStageCheckForUpdates();
    Client->Connect(this,SLOT(DoneLatestQuery()));
    QSettings Settings("settings.ini",QSettings::IniFormat);
    Settings.sync();
    QString Server = Settings.value("ApiEndpoint").toString();
    Client->Get(Server);
    return true;
}

void Updater::DoneLatestQuery()
{
    if(Client->WasError())
    {
        Skip();
        return;
    }
    QJsonDocument JsonResponse = QJsonDocument::fromJson(Client->GetPageData());
    QJsonObject JsonObj = JsonResponse.object();
    AppReference = QString("http://bablosoft.com/") + JsonObj["script"].toString();
    RemoteVersion = JsonObj["version"].toString();
    QStringList Changelog;
    if(JsonObj.contains("changelog"))
    {
        QVariantList array = JsonObj["changelog"].toArray().toVariantList();
        foreach(QVariant v,array)
        {
            Changelog.append(v.toString().replace("\n",""));
        }
    }

    if(CurrentVersion.isEmpty())
    {
        Update();
        return;
    }

    if(RemoteVersion == CurrentVersion)
    {
        Skip();
        return;
    }

    if(RemoteVersion == MaxVersion(QStringList()<<RemoteVersion<<CurrentVersion))
    {
        Progress->SetStageNewVersion();
        Progress->SetVersionNumber(RemoteVersion);
        Progress->SetChangelog(Changelog.join("\r\n"));
    }
    else
    {
        Skip();
        return;
    }
}

void Updater::Update()
{
    Progress->SetStageDownloading(CanSkip);
    Downloader->Get(AppReference);
}

void Updater::DownloadProgress(qint64 BytesReceived, qint64 BytesTotal)
{
    if(BytesTotal)
        Progress->SetProgress((100 * BytesReceived) / BytesTotal);
}

void Updater::DownloadFinished()
{
    if(Downloader->WasError())
    {
        Skip();
        return;
    }

    const char alphanum[] = "abcdefghijklmnopqrstuvwxyz";
    QString TempLocation = "apps/installing.";
    for(int i = 0;i<10;i++)
    {
        TempLocation += QString(alphanum[qrand() % (sizeof(alphanum) - 1)]);
    }
    TempLocation += "/";
    QString TempFile = TempLocation + "download.zip";

    QDir dir(TempLocation);
    if (!dir.exists())
    {
        dir.mkpath(".");
    }

    QFile file(TempFile);
    if(!file.open(QIODevice::WriteOnly))
    {
        Skip();
        return;
    }

    QList<QByteArray>* AllData = Downloader->GetPageData();
    int Length = AllData->size();

    for(int i = 0;i<Length;i++)
    {
        file.write(AllData->at(i));
    }
    file.close();

    JlCompress::extractDir(QFileInfo(TempFile).absoluteFilePath(),QFileInfo(TempFile).absoluteDir().absolutePath());
    file.remove();

    QDir dir1;
    if(!dir1.rename(TempLocation, QString("apps/") + RemoteVersion))
    {
        Skip();
        return;
    }

    CurrentVersion = RemoteVersion;

    Skip();
}
