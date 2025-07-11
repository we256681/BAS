#include "remote.h"
#include <QDir>
#include <QProcess>
#include <QDebug>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>
#include <QXmlStreamReader>
#include <QDirIterator>
#include <QStandardPaths>
#include "oldversionremover.h"
#include "JlCompress.h"

Remote::Remote(QObject *parent) :
    QObject(parent)
{
    Client = new HttpClient(this);
    ClientForDownloader = new HttpClient(this);
    Downloader = new ResumeDownloader(this);
    Downloader->Init(ClientForDownloader);
    connect(Downloader,SIGNAL(Finished()),this,SLOT(EngineDownloaded()));
    connect(Downloader,SIGNAL(Log(QString)),this,SLOT(DownloadLog(QString)));
    connect(Downloader,SIGNAL(DownloadProgress(qint64,qint64)),this,SLOT(DownloadProgress(qint64,qint64)));

    ForceFree = false;
    UI = 0;
}

void Remote::Exit()
{
    if(UI)
    {
        UI->deleteLater();
        UI = 0;
    }
    qDebug()<<"Exit";
    QTimer::singleShot(0,[](){
         QCoreApplication::exit(0);
    });
}

QString Remote::GetGlobalDir(const QString& path)
{
    if(IsEnginesInAppData)
    {
        QDir dir(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/BasEngines/");
        return dir.absoluteFilePath(path);
    }
    return path;
}

void Remote::DetectSettings()
{
    IsValid = true;

    IsX64 = QSysInfo::currentCpuArchitecture() == "x86_64";
    IsSilent = Arguments.contains("--silent");
    #ifdef SILENT
        IsSilent = true;
    #endif
    Arguments.removeAll("--silent");
    QSettings Settings("data/remote_settings.ini",QSettings::IniFormat);
    IsEnginesInAppData = Settings.value("IsEnginesInAppData",false).toBool();
    qDebug()<<"IsEnginesInAppData"<<IsEnginesInAppData;
    KeepVersionNumber = Settings.value("KeepVersionNumber",100).toInt();
    qDebug()<<"KeepVersionNumber"<<KeepVersionNumber;
    qDebug()<<"InstallerVersion 3.0";

    IsRemote = Arguments.contains("--remote");
    Arguments.removeAll("--remote");

    IsResources = Arguments.contains("--resources");
    Arguments.removeAll("--resources");


    QString PrevKey;
    for(QString Key:Arguments)
    {
        if(PrevKey == "--script")
        {
            ScriptName = Key;
        }else if(PrevKey == "--server")
        {
            Server = Key;
        }else if(PrevKey == "--username")
        {
            Username = Key;
        }else if(PrevKey == "--password")
        {
            Password = Key;
        }else if(PrevKey == "--scheduler-key")
        {
            SchedulerKey = Key;
        }else if(PrevKey == "--actual")
        {
            ActualPath = Key;
        }
        PrevKey = Key;
    }
    if(!IsRemote)
    {
        ScriptPath = ScriptName;
        if(ScriptPath.isEmpty())
        {
            ScriptPath = "data/project.xml";
        }
        QCryptographicHash hash(QCryptographicHash::Sha256);
        hash.addData(ScriptName.toUtf8());
        QString HashString = QString::fromUtf8(hash.result().toHex());
        ScriptName = HashString.mid(0,8);
    }
    if(Server.isEmpty())
    {
         Server = Settings.value("Server","https://bablosoft.com/").toString();
         //qDebug()<<"Server from settings"<<Server;
    }

    if(IsRemote && (ScriptName.isEmpty() || Server.isEmpty()))
    {
        ErrorMessage = "No script name or server name";
        IsValid = false;
    }

    qDebug()<<"IsValid"<<IsValid<<"IsX64"<<IsX64<<"IsSilent"<<IsSilent<<"IsRemote"<<IsRemote<<"ScriptName"<<ScriptName<<"Server"<<Server<<"Username"<<Username<<"PasswordLength"<<Password.length();

}

void Remote::DetectScriptProperties()
{
    if(IsRemote)
    {
        if(UI)
            UI->Show();
        Client->Connect(this,SLOT(ScriptPropertiesHttpClientResp()));
        qDebug()<<"GET"<<Server + QString("scripts/") + ScriptName + QString("/properties");
        Client->Get(Server + QString("scripts/") + ScriptName + QString("/properties"));
    }else
    {
        //Parse project.xml
        {
            QFileInfo info(ScriptPath);
            if(!info.exists())
            {
                if(IsSilent)
                {
                    Exit();
                }else
                {
                    UI->SetError("project.xml not found");
                }
                return;
            }
        }

        QString HashString, EngineVersion;
        {
            QFile f(ScriptPath);
            if (f.open(QFile::ReadOnly))
            {
                QCryptographicHash hash(QCryptographicHash::Sha256);
                if (hash.addData(&f))
                {
                     HashString = QString::fromUtf8(hash.result().toHex());
                     HashString = HashString.mid(0,8);
                }
            }
            f.close();
        }

        if(HashString.isEmpty())
        {
            if(IsSilent)
            {
                Exit();
            }else
            {
                UI->SetError("failed to compute hash");
            }
            return;
        }

        {
            QFile file(ScriptPath);
            QXmlStreamReader xmlReader;
            if (!file.open(QIODevice::ReadOnly| QIODevice::Text))
            {
                if(IsSilent)
                {
                    Exit();
                }else
                {
                    UI->SetError("failed to read file");
                }
                return;
            }

            xmlReader.setDevice(&file);

            while(!xmlReader.atEnd() && !xmlReader.hasError())
            {
                QXmlStreamReader::TokenType token = xmlReader.readNext();
                if(xmlReader.name() == "EngineVersion" && token == QXmlStreamReader::StartElement)
                {
                    xmlReader.readNext();
                    EngineVersion = xmlReader.text().toString();
                }

                if(xmlReader.name() == "Remote" && token == QXmlStreamReader::StartElement)
                {
                    foreach(QXmlStreamAttribute attr,xmlReader.attributes())
                    {
                        if(attr.name() == "ScriptName")
                        {
                            this->ScriptName = attr.value().toString();
                        }
                        if(attr.name() == "Server")
                        {
                            this->Server = attr.value().toString();
                        }
                        if(attr.name() == "Username")
                        {
                            this->Username = attr.value().toString();
                        }
                        if(attr.name() == "Password")
                        {
                            this->Password = attr.value().toString();
                        }
                    }
                    qDebug()<<"Remote project.xml"<<"ScriptName"<<ScriptName<<"Server"<<Server<<"Username"<<Username<<"Password"<<Password;
                    IsRemote = true;
                    DetectScriptProperties();
                    return;
                }

            }


        }

        if(EngineVersion.isEmpty())
        {
            if(IsSilent)
            {
                Exit();
            }else
            {
                UI->SetError("failed to get engine version");
            }
            return;
        }

        emit ScriptPropertiesDetected(HashString, EngineVersion);


    }
}

void Remote::ScriptPropertiesHttpClientResp()
{
    qDebug()<<Client->GetContent();

    if(Client->WasError())
    {
        if(IsSilent)
        {
            Exit();
        }else
        {
            UI->SetError(Client->GetErrorString().replace("bablosoft","server"));
        }
        return;
    }
    QJsonDocument JsonResponse = QJsonDocument::fromJson(Client->GetContent().toUtf8());
    QJsonObject JsonObj = JsonResponse.object();
    if(!JsonObj["success"].toBool())
    {
        if(IsSilent)
        {
            Exit();
        }else
        {
            UI->SetError(JsonObj["message"].toString());
        }
        return;
    }
    if(JsonObj["free"].toBool())
    {
        ForceFree = true;
    }

    emit ScriptPropertiesDetected(JsonObj["hash"].toString(),JsonObj["engversion"].toString());
}

void Remote::ScriptPropertiesDetected(const QString& ScriptHash, const QString& EngineVersion)
{
    QString ScriptHashUpdated = QString::fromUtf8(QCryptographicHash::hash(EngineVersion.toUtf8(), QCryptographicHash::Sha256).toHex());
    ScriptHashUpdated = ScriptHashUpdated.mid(0,8);

    this->ScriptHash = "SID" + ScriptHashUpdated;
    this->EngineVersion = EngineVersion;
    this->IsValid = IsValid;
    this->ErrorMessage = ErrorMessage;

    if(UI)
        UI->SetProgress(10);
    qDebug()<<"ScriptHash"<<ScriptHashUpdated<<"EngineVersion"<<EngineVersion;

    //Check if script is already installed
    {
        QString GlobalFolder = (!IsRemote) ? "appslocal" : "appsremote";
        QString FullPath = GlobalFolder + QString("/") + ScriptName + QString("/SID") + ScriptHashUpdated + QString("/engine/FastExecuteScript.exe");
        qDebug()<<"AppPath"<<FullPath<<GlobalFolder + QString("/") + ScriptName + QString("/SID") + ScriptHashUpdated + QString("/engine/");
        QFileInfo info(FullPath);
        if(info.exists())
        {

            if(IsRemote)
            {
                QString FullPath = GlobalFolder + QString("/") + ScriptName + QString("/SID") + ScriptHashUpdated;

                QFile FileProjectXml(FullPath + "/engine/project.xml");
                if(FileProjectXml.open(QIODevice::WriteOnly))
                {
                    QXmlStreamWriter xmlWriter(&FileProjectXml);
                    xmlWriter.setAutoFormatting(true);
                    xmlWriter.writeStartDocument();

                    xmlWriter.writeStartElement("Remote");
                    xmlWriter.writeAttribute("ScriptName", ScriptName);
                    xmlWriter.writeAttribute("Version", "last");
                    xmlWriter.writeAttribute("Server", Server);
                    xmlWriter.writeAttribute("Username", Username);
                    xmlWriter.writeAttribute("Password", Password);
                    if(ForceFree)
                        xmlWriter.writeAttribute("Free", "true");
                    xmlWriter.writeAttribute("Mode", "1");

                    xmlWriter.writeEndElement();
                }

                FileProjectXml.close();
            }


            QStringList params;
            if(IsSilent)
                params.append("--silent");

            if(!SchedulerKey.isEmpty())
            {
                params.append("--scheduler-key");
                params.append(SchedulerKey);
            }

            if(IsResources)
            {
                params.append("--resources");
            }

            QString FolderToStart = GlobalFolder + QString("/") + ScriptName + QString("/SID") + ScriptHashUpdated + QString("/engine/");
            if(!ActualPath.isEmpty())
            {
                QString FullPathCopy = FullPath;
                QString ActualName = QString("Actual.") + ((IsRemote) ? (ScriptName + QString(".")):"") + QString("xml");
                FullPathCopy.replace("FastExecuteScript.exe",ActualName);
                QFile::remove(FullPathCopy);
                QFile::copy(ActualPath, FullPathCopy);

                QFile::remove(ActualPath);
            }
            {
                QProcess *p = new QProcess();
                p->setWorkingDirectory(FolderToStart);
                p->start(FullPath,params);
                while(true)
                {
                    if(p->state() == QProcess::NotRunning)
                        break;

                    if(p->pid())
                    {
                        qDebug()<<"PID="<<p->processId();
                        break;
                    }

                    QCoreApplication::processEvents(QEventLoop::AllEvents, 30);
                }
            }

            if(UI)
                UI->SetProgress(100);

            qDebug()<<"Run!";

            Exit();
            return;
        }
    }

    //Check if engine already downloaded
    {
        QString EnginesDir;
        EnginesDir = GetGlobalDir((IsRemote) ? "enginesprotected" : "engines");
        QString ProtectedAdd = (IsRemote) ? "Protected" : "";

        QString FullPath = EnginesDir + QString("/") + EngineVersion + QString("/engine.zip");
        qDebug()<<"EnginePath"<<FullPath;
        QFileInfo info(FullPath);
        if(info.exists())
        {
            emit EnginePrepared();
        }else
        {
            if(UI)
                UI->Show();

            QString Arch = (IsX64) ? "64" : "32";
            QString EngineUrl = Server + QString("distr/FastExecuteScript") + ProtectedAdd + Arch + QString("/") + EngineVersion + QString("/FastExecuteScript") + ProtectedAdd + QString(".x") + Arch + QString(".zip");
            qDebug()<<"Downloading"<<EngineUrl;
            Downloader->Get(EngineUrl);
        }
    }
}

void Remote::DownloadProgress(qint64 BytesReceived, qint64 BytesTotal)
{
    if(BytesTotal && UI)
        UI->SetProgress(10 + (80 * BytesReceived) / BytesTotal);
}

void Remote::DownloadLog(QString Text)
{
    qDebug()<<Text;
}

void Remote::EngineDownloaded()
{
    if(Downloader->WasError())
    {
        if(IsSilent)
        {
            Exit();
        }else
        {
            UI->SetError(Downloader->GetErrorString().replace("bablosoft","server"));
        }
        return;
    }

    QString EnginesDir;
    EnginesDir = GetGlobalDir((IsRemote) ? "enginesprotected" : "engines");

    QDir dir(EnginesDir + QString("/") + EngineVersion);
    if(!dir.exists())
    {
        dir.mkpath(".");
    }

    QFile file(EnginesDir + QString("/") + EngineVersion + QString("/engine.zip"));
    if(!file.open(QIODevice::WriteOnly))
    {
        if(IsSilent)
        {
            Exit();
        }else
        {
            UI->SetError("Failed to write engine file");
        }
        return;
    }

    QList<QByteArray>* AllData = Downloader->GetPageData();
    int Length = AllData->size();

    for(int i = 0;i<Length;i++)
    {
        file.write(AllData->at(i));
    }

    file.close();

    OldVersionRemover Remover;
    Remover.Remove((IsRemote) ? "enginesprotected" : "engines", EngineVersion, KeepVersionNumber);

    emit EnginePrepared();
}


void Remote::EnginePrepared()
{
    qDebug()<<"EnginePrepared";
    if(UI)
    {
        UI->SetProgress(90);
        UI->hide();
    }



    //Remove temp file
    QString GlobalFolder = (!IsRemote) ? "appslocal" : "appsremote";
    QString FullPath = GlobalFolder + QString("/") + ScriptName + QString("/temp");
    if(IsRemote)
    {
        QDir dir(FullPath);
        if(!dir.removeRecursively())
        {
            if(IsSilent)
            {
                Exit();
            }else
            {
                UI->SetError("Failed to remove temp directory");
            }
        }
    }

    QString EnginesDir;
    EnginesDir = GetGlobalDir((IsRemote) ? "enginesprotected" : "engines");

    //Uncompress
    QStringList AllFiles = JlCompress::extractDir(EnginesDir + QString("/") + EngineVersion + QString("/engine.zip"),FullPath + "/engine");


    //Copy from prev install
    QString PreviousInstallation;
    {
        QDir dir = QDir(GlobalFolder + QString("/") + ScriptName);

        QStringList list = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
        foreach(QString d, list)
        {
            if(d.startsWith("SID"))
            {
                PreviousInstallation = dir.absoluteFilePath(d);
                break;
            }

        }
    }

    if(!PreviousInstallation.isEmpty())
    {
        qDebug()<<"Copy files from "<<PreviousInstallation;

        QStringList FileList;
        QFile FileListFile(PreviousInstallation + "/engine/filelist.txt");
        if (FileListFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&FileListFile);
           while (!in.atEnd())
           {
              QString line = in.readLine();
              FileList.append(line);
           }
           FileListFile.close();
        }
        FileListFile.close();

        QDir dir = QDir(PreviousInstallation + "/engine");
        QDirIterator it(dir.absolutePath(), QDir::NoDotAndDotDot | QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            QString f = it.next();
            QString relative = f;
            relative.remove(dir.absolutePath());

            bool Exclude = false;

            //Don't copy node.js files to new folder
            if(relative.startsWith("/e/"))
            {
                Exclude = true;
            }else
            {
            for(QString ExcludeFile: FileList)
            {
                if(f.endsWith(ExcludeFile))
                {
                    Exclude = true;
                    break;
                }
            }
            }
            if(!Exclude)
            {
                QString fa = f;
                QString fr = f.replace(dir.absolutePath(),"");
                fr = FullPath + "/engine" + fr;
                qDebug() << fa << "->" << fr;
                {
                    QDir d(fr);
                    d.mkpath("..");
                }
                QFile::copy(fa, fr);
            }
        }

    }

    //Save filelist.txt
    {
        QFile file(FullPath + "/engine/filelist.txt");
        if(!file.open(QIODevice::WriteOnly))
        {
            if(IsSilent)
            {
                Exit();
            }else
            {
                UI->SetError("Failed to write engine file");
            }
            return;
        }

        QStringList AllFilesRelative;
        for(QString s: AllFiles)
        {
            if(QFileInfo(s).isFile())
            {
                AllFilesRelative.append(s.replace(QDir(FullPath + "/engine").absolutePath(),""));
            }
        }
        AllFilesRelative.append("/filelist.txt");
        AllFilesRelative.append("/project.xml");



        QString allfiles = AllFilesRelative.join("\r\n");

        file.write(allfiles.toUtf8());

        file.close();
    }

    //Safe project.xml
    if(IsRemote)
    {
        QFile FileProjectXml(FullPath + "/engine/project.xml");
        if(FileProjectXml.open(QIODevice::WriteOnly))
        {
            QXmlStreamWriter xmlWriter(&FileProjectXml);
            xmlWriter.setAutoFormatting(true);
            xmlWriter.writeStartDocument();

            xmlWriter.writeStartElement("Remote");
            xmlWriter.writeAttribute("ScriptName", ScriptName);
            xmlWriter.writeAttribute("Version", "last");
            xmlWriter.writeAttribute("Server", Server);
            xmlWriter.writeAttribute("Username", Username);
            xmlWriter.writeAttribute("Password", Password);
            if(ForceFree)
                xmlWriter.writeAttribute("Free", "true");
            xmlWriter.writeAttribute("Mode", "1");

            xmlWriter.writeEndElement();
        }

        FileProjectXml.close();
    }else
    {
        QFile::copy(ScriptPath, FullPath + "/engine/project.xml");
    }

    //Rename dir
    {
        QDir dir;
        if(!dir.rename(FullPath, GlobalFolder + QString("/") + ScriptName + QString("/") + ScriptHash))
        {
            if(IsSilent)
            {
                Exit();
            }else
            {
                UI->SetError("Failed to rename dir");
            }
        }
    }

    //Remove old
    if(!PreviousInstallation.isEmpty())
    {
        qDebug()<<"Remove old";
        QDir dir(PreviousInstallation);
        dir.removeRecursively();
    }

    //Run exe
    QStringList params;
    if(IsSilent)
        params.append("--silent");
    QString FolderToStart = GlobalFolder + QString("/") + ScriptName + QString("/") + ScriptHash + QString("/engine/");

    if(!SchedulerKey.isEmpty())
    {
        params.append("--scheduler-key");
        params.append(SchedulerKey);
    }

    if(IsResources)
    {
        params.append("--resources");
    }
    if(!ActualPath.isEmpty())
    {
        QString ActualName = QString("Actual.") + ((IsRemote) ? (ScriptName + QString(".")):"") + QString("xml");
        QString FullPathCopy = GlobalFolder + QString("/") + ScriptName + QString("/") + ScriptHash + QString("/engine/") + ActualName;
        QFile::remove(FullPathCopy);
        QFile::copy(ActualPath, FullPathCopy);

        QFile::remove(ActualPath);
    }
    {
        QProcess *p = new QProcess();
        p->setWorkingDirectory(FolderToStart);
        p->start(GlobalFolder + QString("/") + ScriptName + QString("/") + ScriptHash + "/engine/FastExecuteScript.exe",params);
        while(true)
        {
            if(p->state() == QProcess::NotRunning)
                break;

            if(p->pid())
            {
                qDebug()<<"PID="<<p->processId();
                break;
            }

            QCoreApplication::processEvents(QEventLoop::AllEvents, 30);
        }
    }

    if(UI)
        UI->SetProgress(100);

    qDebug()<<"Run!";
    Exit();
    return;
}

bool Remote::Start(const QStringList& Arguments)
{
    this->Arguments = Arguments;

    DetectSettings();
    if(!IsSilent)
    {
        UI = new DownloadingWidget();
        connect(UI,SIGNAL(Cancel()),this,SLOT(Exit()));

        UI->hide();
    }
    if(!IsValid)
    {
        if(IsSilent)
        {
            return false;
        }else
        {
            UI->SetError(ErrorMessage);
            return true;
        }
    }


    DetectScriptProperties();

    return true;
}

