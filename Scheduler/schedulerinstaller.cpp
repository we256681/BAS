#include "schedulerinstaller.h"
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QDirIterator>
#include <QRegularExpression>
#include <QCoreApplication>
#include <QProcess>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QFutureWatcher>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <Windows.h>


SchedulerInstaller::SchedulerInstaller(QObject *parent) :
    QObject(parent)
{
    InstallDir = QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QString("/TaskScheduler/"));
}

void SchedulerInstaller::RemoveAllCorruptedVersions()
{
    Log("Removing corrupted versions started ...");

    QDirIterator DirIterator(GetDirectory(), QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    while (DirIterator.hasNext())
    {
        QString Dir = DirIterator.next();
        QFileInfo DirInfo(Dir);
        QString DirString = DirInfo.fileName();
        if(!QFileInfo(QDir(Dir).filePath("installed.txt")).exists() && VersionToInt(DirString) > 0)
        {
            Log("Removing corrupted version", DirString);
            Log("Result", QDir(GetDirectory() + "/" + DirString).removeRecursively());
        }
    }

    Log("Removing corrupted versions finished");
}

QString SchedulerInstaller::FindInstallPath()
{
    int CurrentVersion = VersionToInt(GetVersion());
    QString InstallVersion = QDir::cleanPath(GetDirectory() + "/" + GetVersion());
    QDirIterator DirIterator(GetDirectory(), QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    while (DirIterator.hasNext())
    {
        QString Dir = DirIterator.next();
        QFileInfo DirInfo(Dir);
        QString DirString = DirInfo.fileName();
        int Version = VersionToInt(DirString);

        if(Version >= CurrentVersion && QFileInfo(QDir(Dir).filePath("installed.txt")).exists())
        {
            return QString();
        }

    }
    return InstallVersion;
}

QString SchedulerInstaller::FindLatestVersion()
{
    QString LatestVersion;
    QDirIterator DirIterator(GetDirectory(), QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    while (DirIterator.hasNext())
    {
        QString Dir = DirIterator.next();
        QFileInfo DirInfo(Dir);
        QString DirString = DirInfo.fileName();

        if(VersionToInt(DirString) > VersionToInt(LatestVersion) && QFileInfo(QDir(Dir).filePath("installed.txt")).exists())
        {
            LatestVersion = DirString;
        }

    }
    return LatestVersion;
}

void SchedulerInstaller::CopyFiles(const QString& From,const QString& To)
{
    //Root files
    CopyFileInternal(From,To,"Qt5Core.dll");
    CopyFileInternal(From,To,"zlib1.dll");
    CopyFileInternal(From,To,"ssleay32.dll");
    CopyFileInternal(From,To,"Qt5WebSockets.dll");
    CopyFileInternal(From,To,"Qt5Network.dll");
    CopyFileInternal(From,To,"opengl32sw.dll");
    CopyFileInternal(From,To,"msvcr120.dll");
    CopyFileInternal(From,To,"msvcp120.dll");
    CopyFileInternal(From,To,"libGLESV2.dll");
    CopyFileInternal(From,To,"libeay32.dll");
    CopyFileInternal(From,To,"libEGL.dll");
    CopyFileInternal(From,To,"D3Dcompiler_47.dll");

    //Plugins
    CopyFileInternal(From,To,"bearer/qnativewifibearer.dll");
    CopyFileInternal(From,To,"bearer/qgenericbearer.dll");
    CopyFileInternal(From,To,"iconengines/qsvgicon.dll");
    CopyFileInternal(From,To,"imageformats/qwebp.dll");
    CopyFileInternal(From,To,"imageformats/qwbmp.dll");
    CopyFileInternal(From,To,"imageformats/qtiff.dll");
    CopyFileInternal(From,To,"imageformats/qtga.dll");
    CopyFileInternal(From,To,"imageformats/qsvg.dll");
    CopyFileInternal(From,To,"imageformats/qjpeg.dll");
    CopyFileInternal(From,To,"imageformats/qico.dll");
    CopyFileInternal(From,To,"imageformats/qicns.dll");
    CopyFileInternal(From,To,"imageformats/qgif.dll");
    CopyFileInternal(From,To,"imageformats/qdds.dll");
    CopyFileInternal(From,To,"platforms/qwindows.dll");

    //Scheduler.exe
    CopyFileInternal(From,To,"Scheduler.exe");


    //RemoteExecuteScript Root files
    CopyFileInternal(From,To + "/app","ssleay32.dll");
    CopyFileInternal(From,To + "/app","Qt5Widgets.dll");
    CopyFileInternal(From,To + "/app","Qt5Svg.dll");
    CopyFileInternal(From,To + "/app","Qt5Network.dll");
    CopyFileInternal(From,To + "/app","Qt5Gui.dll");
    CopyFileInternal(From,To + "/app","Qt5Core.dll");
    CopyFileInternal(From,To + "/app","opengl32sw.dll");
    CopyFileInternal(From,To + "/app","msvcr120.dll");
    CopyFileInternal(From,To + "/app","msvcp120.dll");
    CopyFileInternal(From,To + "/app","libGLESV2.dll");
    CopyFileInternal(From,To + "/app","libeay32.dll");
    CopyFileInternal(From,To + "/app","libEGL.dll");
    CopyFileInternal(From,To + "/app","D3Dcompiler_47.dll");
    CopyFileInternal(From,To + "/app","zlib1.dll");


    //RemoteExecuteScript Plugins
    CopyFileInternal(From,To + "/app","bearer/qnativewifibearer.dll");
    CopyFileInternal(From,To + "/app","bearer/qgenericbearer.dll");
    CopyFileInternal(From,To + "/app","iconengines/qsvgicon.dll");
    CopyFileInternal(From,To + "/app","imageformats/qwebp.dll");
    CopyFileInternal(From,To + "/app","imageformats/qwbmp.dll");
    CopyFileInternal(From,To + "/app","imageformats/qtiff.dll");
    CopyFileInternal(From,To + "/app","imageformats/qtga.dll");
    CopyFileInternal(From,To + "/app","imageformats/qsvg.dll");
    CopyFileInternal(From,To + "/app","imageformats/qjpeg.dll");
    CopyFileInternal(From,To + "/app","imageformats/qico.dll");
    CopyFileInternal(From,To + "/app","imageformats/qicns.dll");
    CopyFileInternal(From,To + "/app","imageformats/qgif.dll");
    CopyFileInternal(From,To + "/app","imageformats/qdds.dll");
    CopyFileInternal(From,To + "/app","platforms/qwindows.dll");

    //RemoteExecuteScript.exe
    CopyFileInternal(From,To + "/app","RemoteExecuteScript.exe");

    //Gui
    CopyFileInternal(From + "/Worker",To + "/gui","zlib1.dll");
    CopyFileInternal(From + "/Worker",To + "/gui","v8_context_snapshot.bin");
    CopyFileInternal(From + "/Worker",To + "/gui","swiftshader/libEGL.dll");
    CopyFileInternal(From + "/Worker",To + "/gui","swiftshader/libGLESv2.dll");
    CopyFileInternal(From + "/Worker",To + "/gui","snapshot_blob.bin");
    CopyFileInternal(From + "/Worker",To + "/gui","SchedulerGui.exe");
    CopyFileInternal(From + "/Worker",To + "/gui","natives_blob.bin");
    CopyFileInternal(From + "/Worker",To + "/gui","libGLESv2.dll");
    CopyFileInternal(From + "/Worker",To + "/gui","libEGL.dll");
    CopyFileInternal(From + "/Worker",To + "/gui","icudtl.dat");
    CopyFileInternal(From + "/Worker",To + "/gui","libcef.dll");
    CopyFileInternal(From + "/Worker",To + "/gui","devtools_resources.pak");
    CopyFileInternal(From + "/Worker",To + "/gui","d3dcompiler_47.dll");
    CopyFileInternal(From + "/Worker",To + "/gui","d3dcompiler_43.dll");
    CopyFileInternal(From + "/Worker",To + "/gui","shedulericonbig.ico");
    CopyFileInternal(From + "/Worker",To + "/gui","schedulericonsmall.ico");
    CopyFileInternal(From + "/Worker",To + "/gui","chrome_elf.dll");
    CopyFileInternal(From + "/Worker",To + "/gui","cef_extensions.pak");
    CopyFileInternal(From + "/Worker",To + "/gui","cef_200_percent.pak");
    CopyFileInternal(From + "/Worker",To + "/gui","cef_100_percent.pak");
    CopyFileInternal(From + "/Worker",To + "/gui","cef.pak");

    //Locales
    {
        QString FromPath = From + "/Worker/locales";
        QDirIterator it(FromPath, QStringList() << "*.*", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            QString AbsolutePath = it.next();
            QString RelativePath = QDir(FromPath).relativeFilePath(AbsolutePath);

            CopyFileInternal(FromPath,To + QString("/gui/locales"),RelativePath);
        }
    }


    //Web application
    {
        QString WebApplicationFromPath = From+"/html/scheduler";
        QDirIterator it(WebApplicationFromPath, QStringList() << "*.*", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            QString AbsolutePath = it.next();
            QString RelativePath = QDir(WebApplicationFromPath).relativeFilePath(AbsolutePath);

            CopyFileInternal(WebApplicationFromPath,To + QString("/gui/html"),RelativePath);
        }
    }

    //Create installed.txt file to show that works correctly
    {
        QFile file(To + QString("/installed.txt"));
        file.open(QIODevice::WriteOnly);
        file.close();
    }
}

void SchedulerInstaller::CopyFilesInThread(const QString& From,const QString& To)
{
    QFutureWatcher<void> watcher;
    QFuture<void> future;
    QEventLoop loop;
    connect(&watcher, SIGNAL(finished()), &loop, SLOT(quit()));


    future = QtConcurrent::run(this, &SchedulerInstaller::CopyFiles, From, To);
    watcher.setFuture(future);
    loop.exec();

}

void SchedulerInstaller::ScheduleIfNeeded(const QString& LatestVersion)
{
    Log("Schedule started ...");

    Log("Checking if any version is scheduled ...");

    QStringList Params;
    Params.append("/Query");
    Params.append("/FO");
    Params.append("CSV");
    Params.append("/tn");
    Params.append("TaskSchedulerApplication");
    Params.append("/V");
    QString Result = RunSchtasksAndGetResult(Params);

    QRegularExpression re("[\\\\\\/](\\d+\\.\\d+\\.\\d+)[\\\\\\/]Scheduler.exe");
    re.setPatternOptions(QRegularExpression::MultilineOption);
    QRegularExpressionMatch match = re.match(Result);
    RunnningVersion.clear();
    if(match.hasMatch())
        RunnningVersion = match.captured(1);


    if(RunnningVersion == "1.0.0")
    {
        Log("Scheduled version is 1.0.0. It was buggy, so removing it");
        UninstallInternal();
        RunnningVersion.clear();
    }

    if(RunnningVersion.isEmpty())
    {
        Log("No version is scheduled");
        //Task is not scheduled, schedule it
        QString Version = LatestVersion;
        if(Version.isEmpty())
            Version = FindLatestVersion();
        QString Exe = GetDirectory() + QString("/") + Version + QString("/Scheduler.exe");
        QString Path = GenerateXml(Exe);
        RunSchtasksAndGetResult(QStringList()<<"/Create"<<"/tn"<<"TaskSchedulerApplication"<<"/xml"<<Path);
    }else
    {
        Log("Scheduled version", RunnningVersion);
    }

    Log("Schedule finished ...");
}

QString SchedulerInstaller::GenerateXml(const QString& ExeLocation)
{
    QString Xml = R"XML(<?xml version="1.0" encoding="UTF-16"?>
<Task version="1.2" xmlns="http://schemas.microsoft.com/windows/2004/02/mit/task">
<Triggers>
<LogonTrigger>
  <Enabled>true</Enabled>
  <UserId>BAS_USER_ID</UserId>
</LogonTrigger>
</Triggers>
<Principals>
<Principal id="Author">
  <RunLevel>LeastPrivilege</RunLevel>
</Principal>
</Principals>
<Settings>
<MultipleInstancesPolicy>Parallel</MultipleInstancesPolicy>
<DisallowStartIfOnBatteries>false</DisallowStartIfOnBatteries>
<StopIfGoingOnBatteries>false</StopIfGoingOnBatteries>
<AllowHardTerminate>true</AllowHardTerminate>
<StartWhenAvailable>false</StartWhenAvailable>
<RunOnlyIfNetworkAvailable>false</RunOnlyIfNetworkAvailable>
<IdleSettings>
  <StopOnIdleEnd>true</StopOnIdleEnd>
  <RestartOnIdle>false</RestartOnIdle>
</IdleSettings>
<AllowStartOnDemand>true</AllowStartOnDemand>
<Enabled>true</Enabled>
<Hidden>false</Hidden>
<RunOnlyIfIdle>false</RunOnlyIfIdle>
<WakeToRun>false</WakeToRun>
<ExecutionTimeLimit>PT0S</ExecutionTimeLimit>
<Priority>7</Priority>
</Settings>
<Actions Context="Author">
<Exec>
  <Command>BAS_EXE_LOCATION</Command>
</Exec>
</Actions>
</Task>
)XML";

    QString UserName;

    {
        std::vector<wchar_t> DataArray;
        DWORD DataSize = 4096;
        DataArray.resize(DataSize);
        if(GetComputerName( DataArray.data(), &DataSize))
        {
            std::wstring UserNameItem(DataArray.begin(),DataArray.begin() + DataSize);
            UserName += QString::fromStdWString(UserNameItem);
        }
    }

    if(!UserName.isEmpty())
        UserName += "\\";

    {
        std::vector<wchar_t> DataArray;
        DWORD DataSize = 4096;
        DataArray.resize(DataSize);
        if(GetUserName(DataArray.data(), &DataSize))
        {
            std::wstring UserNameItem(DataArray.begin(),DataArray.begin() + DataSize - 1);
            UserName += QString::fromStdWString(UserNameItem);
        }
    }

    Log("Current username", UserName);
    Log("Scheduler.exe location", ExeLocation);

    Xml = Xml.replace("BAS_EXE_LOCATION",ExeLocation.toHtmlEscaped());
    Xml = Xml.replace("BAS_USER_ID",UserName.toHtmlEscaped());

    QFile outFile("Scheduler.xml");
    outFile.open(QIODevice::WriteOnly);
    QTextStream ts(&outFile);
    ts.setCodec("UTF-16");
    ts << bom << Xml;
    outFile.close();


    return QFileInfo(outFile).absoluteFilePath();
}



void SchedulerInstaller::CopyFilesAndInstall()
{
    Log("Install started ...");

    //Create scheduler working dir
    Log("Making root directory", QDir(GetDirectory()).mkpath("."));

    //Remove corrupted versions
    RemoveAllCorruptedVersions();

    //Find install path
    QString InstallPath = FindInstallPath();

    if(!InstallPath.isEmpty())
    {
        Log("Install to",InstallPath);
        //Install path not empty, so need to install
        CopyFilesInThread(GetExeFolder(), InstallPath);
    }else
    {
        Log("No installation needed, already installed");
    }

    QString LatestVersion = FindLatestVersion();

    Log("Latest version", LatestVersion);

    //If any version is installed
    if(!LatestVersion.isEmpty())
    {
        //Schedule task, skip if already scheduled
        //If scheduled previous version, reschedule
        ScheduleIfNeeded(LatestVersion);
    }

    Log("Install finished");

}

void SchedulerInstaller::UninstallInternal()
{
    Log("Uninstall started ...");

    RunSchtasksAndGetResult(QStringList()<<"/End"<<"/tn"<<"TaskSchedulerApplication");
    RunSchtasksAndGetResult(QStringList()<<"/Delete"<<"/tn"<<"TaskSchedulerApplication"<<"/f");

    Log("Uninstall finished");
}

void SchedulerInstaller::DetectConsoleEncoding()
{
    ConsoleEncoding = "utf-8";
    QProcess p;
    p.start("cmd",QStringList()<<"/c"<<"chcp");
    while(p.state() != QProcess::NotRunning)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    QString Result = p.readAllStandardOutput() + "\r\n" + p.readAllStandardError();
    QRegularExpression re("\\d+");
    QRegularExpressionMatch match = re.match(Result);
    if(!match.hasMatch())
        return;

    int ConsoleEncodingInt = match.captured(0).toInt();


    ConsoleEncoding = EncodingIdToEncodingName(ConsoleEncodingInt);
}


void SchedulerInstaller::Install()
{

    InitLog();

    DetectConsoleEncoding();

    Log("Working dir",GetDirectory());
    Log("Version",GetVersion());
    Log("Console encoding",ConsoleEncoding);


    ScheduleIfNeeded();

    Log("Start scheduler");

    RunSchtasksAndGetResult(QStringList()<<"/Run"<<"/tn"<<"TaskSchedulerApplication");

}

void SchedulerInstaller::Uninstall()
{
    InitLog();

    DetectConsoleEncoding();

    Log("Working dir",GetDirectory());
    Log("Version",GetVersion());
    Log("Console encoding",ConsoleEncoding);

    UninstallInternal();

}

void SchedulerInstaller::RemoveFromScheduler()
{
    InitLog();

    DetectConsoleEncoding();

    Log("Working dir",GetDirectory());
    Log("Version",GetVersion());
    Log("Console encoding",ConsoleEncoding);

    RunSchtasksAndGetResult(QStringList()<<"/Delete"<<"/tn"<<"TaskSchedulerApplication"<<"/f");
}

void SchedulerInstaller::Restart(QFile * LockFile)
{
    InitLog();

    DetectConsoleEncoding();

    Log("Working dir",GetDirectory());
    Log("Version",GetVersion());
    Log("Console encoding",ConsoleEncoding);

    //Find latest version
    QString LatestVersion = FindLatestVersion();

    Log("Latest version",LatestVersion);

    //Delete old schedule
    Log("Delete old schedule");
    RunSchtasksAndGetResult(QStringList()<<"/Delete"<<"/tn"<<"TaskSchedulerApplication"<<"/f");

    //Create new schedule
    Log("Create new schedule");
    QString Exe = GetDirectory() + QString("/") + LatestVersion + QString("/Scheduler.exe");
    QString Path = GenerateXml(Exe);
    RunSchtasksAndGetResult(QStringList()<<"/Create"<<"/tn"<<"TaskSchedulerApplication"<<"/xml"<<Path);

    //Release lock to be able new process to start
    if(LockFile)
        LockFile->close();

    //Run new schedule
    RunSchtasksAndGetResult(QStringList()<<"/Run"<<"/tn"<<"TaskSchedulerApplication");

}


void SchedulerInstaller::Run(const QString& StartType,
                             const QString& IniFileLocation,
                             const QString& ScriptData,
                             const QString& ScriptName,
                             const QString& ScriptLogin,
                             const QString& ScriptPass)
{
    InitLog();

    DetectConsoleEncoding();

    Log("Run started ...");
    Log("Installing into",GetDirectory());
    Log("Version",GetVersion());
    Log("Console encoding",ConsoleEncoding);

    RunnningVersion.clear();

    CopyFilesAndInstall();

    Log("Started scheduler");

    //Start scheduler
    RunSchtasksAndGetResult(QStringList()<<"/Run"<<"/tn"<<"TaskSchedulerApplication");

    //Start viewer
    QString StartedVersion = RunnningVersion;
    if(StartedVersion.isEmpty())
        StartedVersion = FindLatestVersion();
    QString WorkingDir = GetDirectory() + QString("/") + StartedVersion + QString("/");
    PreserveStartData(WorkingDir,StartType,IniFileLocation,ScriptData,ScriptName,ScriptLogin,ScriptPass);
    QString StartBrowser = WorkingDir + QString("Scheduler.exe");
    Log("Start browser",StartBrowser + QString(" --browser"));

    QProcess::startDetached(StartBrowser, QStringList()<<"--browser"<<"--input-done", WorkingDir);

    Log("Run finished");

}

void SchedulerInstaller::PreserveStartData(const QString& WorkingDir,
                                           const QString& StartType,
                                           const QString& IniFileLocation,
                                           const QString& ScriptData,
                                           const QString& ScriptName,
                                           const QString& ScriptLogin,
                                           const QString& ScriptPass)
{
    QString Filename = WorkingDir + QString("/gui/html/js/inputs.js");

    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("method", StartType);
    obj.insert("script_data", ScriptData);
    obj.insert("script_name", ScriptName);
    obj.insert("script_login", ScriptLogin);
    obj.insert("script_pass", ScriptPass);
    obj.insert("ini", IniFileLocation);
    doc.setObject(obj);
    QString AllDataString = QString("window.SchedulerInputs = ") + QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    QFile ResultFile(Filename);
    ResultFile.open(QIODevice::WriteOnly);
    ResultFile.write(AllDataString.toUtf8());
    ResultFile.close();
}

QString SchedulerInstaller::GetDirectory()
{
    return InstallDir;
}

QString SchedulerInstaller::GetVersion()
{
    return "1.6.0";
}

QString SchedulerInstaller::GetExeFolder()
{
    return QCoreApplication::applicationDirPath();
}

int SchedulerInstaller::VersionToInt(const QString& Version)
{
    QStringList Split = Version.split(".");
    QRegularExpression RegexpVersion("^\\d+$");
    if(Split.size() == 3)
    {
        if(RegexpVersion.match(Split[0]).hasMatch() && RegexpVersion.match(Split[1]).hasMatch() && RegexpVersion.match(Split[2]).hasMatch())
        {
            return 100 * 100 * Split[0].toInt() + 100 * Split[1].toInt() + Split[2].toInt();
        }
    }
    return 0;
}

QString SchedulerInstaller::RunSchtasksAndGetResult(const QStringList& Params)
{
    Log(QString("Running schtasks.exe ") + Params.join(" "));
    QProcess p;
    p.start("schtasks.exe", Params);
    while(p.state() != QProcess::NotRunning)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    QTextCodec * Codec = QTextCodec::codecForName(ConsoleEncoding.toUtf8().data());
    QString Result;
    if(Codec)
    {
        Result = Codec->toUnicode(p.readAllStandardOutput()) + "\r\n" + Codec->toUnicode(p.readAllStandardError());
    }else
    {
        Result = p.readAllStandardOutput() + "\r\n" + p.readAllStandardError();
    }

    Log(QString("Result "), Result);
    return Result;
}

bool SchedulerInstaller::CopyFileInternal(const QString& From,const QString& To,const QString& Filename)
{
    QFileInfo(To + QString("/") + Filename).dir().mkpath(".");
    QString CopyFrom = From + QString("/") + Filename;
    QString CopyTo = To + QString("/") + Filename;
    bool Res = QFile::copy(CopyFrom, CopyTo);
    Log(QString("Copy file ") + CopyFrom + QString(" -> ") + CopyTo,Res);
    return Res;
}


void SchedulerInstaller::InitLog()
{
    QFile(GetDirectory() + QString("/scheduler_install_log.txt")).remove();
}

void SchedulerInstaller::Log(const QString& Data)
{
    QString DateString = QTime::currentTime().toString("hh:mm:ss");
    QFile OutFile(GetDirectory() + QString("/scheduler_install_log.txt"));
    OutFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream Stream(&OutFile);
    Stream.setCodec("UTF-8");
    Stream << "[" << DateString << "] " << Data << endl;
    OutFile.close();
}

void SchedulerInstaller::Log(const QString& Text, const QString& Data)
{
    Log(Text + QString(" : ") + Data);
}

void SchedulerInstaller::Log(const QString& Text, int Data)
{
    Log(Text + QString(" : ") + QString::number(Data));
}

void SchedulerInstaller::Log(const QString& Text, bool Data)
{
    Log(Text + QString(" : ") + ((Data) ? "success" : "fail"));
}

QString SchedulerInstaller::EncodingIdToEncodingName(int EncodingId)
{
    switch(EncodingId)
    {
        case 37: return "IBM037";
        case 437: return "IBM437";
        case 500: return "IBM500";
        case 708: return "ASMO-708";
        case 720: return "DOS-720";
        case 737: return "ibm737";
        case 775: return "ibm775";
        case 850: return "ibm850";
        case 852: return "ibm852";
        case 855: return "IBM855";
        case 857: return "ibm857";
        case 858: return "IBM00858";
        case 860: return "IBM860";
        case 861: return "ibm861";
        case 862: return "DOS-862";
        case 863: return "IBM863";
        case 864: return "IBM864";
        case 865: return "IBM865";
        case 866: return "cp866";
        case 869: return "ibm869";
        case 870: return "IBM870";
        case 874: return "windows-874";
        case 875: return "cp875";
        case 932: return "shift_jis";
        case 936: return "gb2312";
        case 949: return "ks_c_5601-1987";
        case 950: return "big5";
        case 1026: return "IBM1026";
        case 1047: return "IBM01047";
        case 1140: return "IBM01140";
        case 1141: return "IBM01141";
        case 1142: return "IBM01142";
        case 1143: return "IBM01143";
        case 1144: return "IBM01144";
        case 1145: return "IBM01145";
        case 1146: return "IBM01146";
        case 1147: return "IBM01147";
        case 1148: return "IBM01148";
        case 1149: return "IBM01149";
        case 1200: return "utf-16";
        case 1201: return "unicodeFFFE";
        case 1250: return "windows-1250";
        case 1251: return "windows-1251";
        case 1252: return "windows-1252";
        case 1253: return "windows-1253";
        case 1254: return "windows-1254";
        case 1255: return "windows-1255";
        case 1256: return "windows-1256";
        case 1257: return "windows-1257";
        case 1258: return "windows-1258";
        case 1361: return "Johab";
        case 10000: return "macintosh";
        case 10001: return "x-mac-japanese";
        case 10002: return "x-mac-chinesetrad";
        case 10003: return "x-mac-korean";
        case 10004: return "x-mac-arabic";
        case 10005: return "x-mac-hebrew";
        case 10006: return "x-mac-greek";
        case 10007: return "x-mac-cyrillic";
        case 10008: return "x-mac-chinesesimp";
        case 10010: return "x-mac-romanian";
        case 10017: return "x-mac-ukrainian";
        case 10021: return "x-mac-thai";
        case 10029: return "x-mac-ce";
        case 10079: return "x-mac-icelandic";
        case 10081: return "x-mac-turkish";
        case 10082: return "x-mac-croatian";
        case 12000: return "utf-32";
        case 12001: return "utf-32BE";
        case 20000: return "x-Chinese_CNS";
        case 20001: return "x-cp20001";
        case 20002: return "x_Chinese-Eten";
        case 20003: return "x-cp20003";
        case 20004: return "x-cp20004";
        case 20005: return "x-cp20005";
        case 20105: return "x-IA5";
        case 20106: return "x-IA5-German";
        case 20107: return "x-IA5-Swedish";
        case 20108: return "x-IA5-Norwegian";
        case 20127: return "us-ascii";
        case 20261: return "x-cp20261";
        case 20269: return "x-cp20269";
        case 20273: return "IBM273";
        case 20277: return "IBM277";
        case 20278: return "IBM278";
        case 20280: return "IBM280";
        case 20284: return "IBM284";
        case 20285: return "IBM285";
        case 20290: return "IBM290";
        case 20297: return "IBM297";
        case 20420: return "IBM420";
        case 20423: return "IBM423";
        case 20424: return "IBM424";
        case 20833: return "x-EBCDIC-KoreanExtended";
        case 20838: return "IBM-Thai";
        case 20866: return "koi8-r";
        case 20871: return "IBM871";
        case 20880: return "IBM880";
        case 20905: return "IBM905";
        case 20924: return "IBM00924";
        case 20932: return "EUC-JP";
        case 20936: return "x-cp20936";
        case 20949: return "x-cp20949";
        case 21025: return "cp1025";
        case 21866: return "koi8-u";
        case 28591: return "iso-8859-1";
        case 28592: return "iso-8859-2";
        case 28593: return "iso-8859-3";
        case 28594: return "iso-8859-4";
        case 28595: return "iso-8859-5";
        case 28596: return "iso-8859-6";
        case 28597: return "iso-8859-7";
        case 28598: return "iso-8859-8";
        case 28599: return "iso-8859-9";
        case 28603: return "iso-8859-13";
        case 28605: return "iso-8859-15";
        case 29001: return "x-Europa";
        case 38598: return "iso-8859-8-i";
        case 50220: return "iso-2022-jp";
        case 50221: return "csISO2022JP";
        case 50222: return "iso-2022-jp";
        case 50225: return "iso-2022-kr";
        case 50227: return "x-cp50227";
        case 51932: return "euc-jp";
        case 51936: return "EUC-CN";
        case 51949: return "euc-kr";
        case 52936: return "hz-gb-2312";
        case 54936: return "GB18030";
        case 57002: return "x-iscii-de";
        case 57003: return "x-iscii-be";
        case 57004: return "x-iscii-ta";
        case 57005: return "x-iscii-te";
        case 57006: return "x-iscii-as";
        case 57007: return "x-iscii-or";
        case 57008: return "x-iscii-ka";
        case 57009: return "x-iscii-ma";
        case 57010: return "x-iscii-gu";
        case 57011: return "x-iscii-pa";
        case 65000: return "utf-7";
        case 65001: return "utf-8";

    }
    return "utf-8";
}



