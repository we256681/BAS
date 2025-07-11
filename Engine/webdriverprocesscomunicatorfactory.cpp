#include "webdriverprocesscomunicatorfactory.h"
#include "webdriveprocesscomunicator.h"
#include <QFile>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSettings>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QStandardPaths>
#include <QDirIterator>
#include <Windows.h>
#include "every_cpp.h"


namespace BrowserAutomationStudioFramework
{
    WebDriverProcessComunicatorFactory::WebDriverProcessComunicatorFactory(QObject *parent) :
        IProcessComunicatorFactory(parent)
    {
        Logger = 0;
        IsRecord = false;

        QSettings Settings("settings_multilogin.ini",QSettings::IniFormat);
        MLALocation = Settings.value("ExeLocation","C:/Program Files (x86)/Multilogin/multilogin.exe").toString();
        ApiLocation = Settings.value("ApiLocation","https://api.multiloginapp.com/").toString();
        IsCustomDriver = Settings.value("CustomDriver", false).toBool();
        SessionIdParameterName = Settings.value("SessionIdParameterName","sessionId").toString();
        PortParameterName = Settings.value("PortParameterName","url").toString();
        MLAPort = Settings.value("MultiloginPort",35000).toInt();
        Log("Created");

    }

    QString WebDriverProcessComunicatorFactory::GetRandomString()
    {
       const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
       const int randomStringLength = 12;

       QString randomString;
       for(int i=0; i<randomStringLength; ++i)
       {
           int index = qrand() % possibleCharacters.length();
           QChar nextChar = possibleCharacters.at(index);
           randomString.append(nextChar);
       }
       return randomString;
    }

    bool WebDriverProcessComunicatorFactory::IsProcessRunning(int pid)
    {
        HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
        bool exists = (process != NULL);
        CloseHandle(process);
        return exists;
    }

    void WebDriverProcessComunicatorFactory::Log(const QString& Text,const QString& Arg1,const QString& Arg2,const QString& Arg3)
    {

        QString Res;

        bool c1 = Text.contains("%1");
        bool c2 = Text.contains("%2");
        bool c3 = Text.contains("%3");
        if(c1 && c2 && c3)
        {
            Res = Text.arg(Arg1).arg(Arg2).arg(Arg3);
        }else if(c1 && c2)
        {
            Res = Text.arg(Arg1).arg(Arg2);
        }else if(c1)
        {
            Res = Text.arg(Arg1);
        }else
        {
            Res = Text;
        }

        if(!MLAToken.isEmpty())
            Res = Res.replace(MLAToken,MLAToken.mid(0,5) + QString("..."));

        if(Logger)
        {
            bool IsMultilogin = QRegExp("^\\s*(\\<\\-|\\-\\>)").indexIn(Res) == 0;
            LogLevel Level;
            if(IsMultilogin)
                Level = LogUser;
            else
                Level = LogInfo;


            QString ResHtml = Res;
            ResHtml = QString("<span style='color:gray'>CommunicatorFactory</span> ") + ResHtml.replace("<","&lt;").
                    replace("<","&lt;").
                    replace(">","&gt;").
                    replace(QRegExp("^\\s*(\\-\\&gt\\;|\\&lt\\;\\-)\\s*\\[([^\\]]*)\\]"),"\\1 [<span style='text-decoration: underline;'>\\2</span>]").
                    replace(QRegExp("^\\s*\\&lt\\;\\-"),"<span style='color:#41f48f;'>mla received</span> ").
                    replace(QRegExp("^\\s*\\-\\&gt\\;"),"<span style='color:#4292f4;'>mla sent</span> ");

            QString ResText = Res;
            ResText = QString("CommunicatorFactory ") + ResText.
                    replace(QRegExp("^\\s*\\<\\-"),"mla received").
                    replace(QRegExp("^\\s*\\-\\>"),"mla sent");


            Logger->WriteHtml(ResHtml,ResText,Level);
        }
    }


    WebDriverProcessComunicatorFactory::~WebDriverProcessComunicatorFactory()
    {
        Logger = 0;
        for(WebDriverUnit &Unit:Drivers)
        {
            if(Unit.WebDriverProcess)
            {
                QStringList args;
                args.append("/F");
                args.append("/T");
                args.append("/PID");
                args.append(QString::number(Unit.WebDriverProcess->processId()));
                Log("Stopping process %1", QString::number(Unit.WebDriverProcess->processId()));

                QProcess * Process = new QProcess();
                connect(Process,SIGNAL(finished(int)),Process,SLOT(deleteLater()));
                Process->start("taskkill.exe",args);

            }
        }
    }

    void WebDriverProcessComunicatorFactory::SetHttpClientFactory(IHttpClientFactory *HttpClientFactory)
    {
        this->HttpClientFactory = HttpClientFactory;
    }

    void WebDriverProcessComunicatorFactory::SetLogger(ILogger * Logger)
    {
        this->Logger = Logger;
    }

    void WebDriverProcessComunicatorFactory::SetIsRecord()
    {
        this->IsRecord = true;
    }


    void WebDriverProcessComunicatorFactory::SetRecordId(const QString& RecordId)
    {
        this->RecordId = RecordId;
    }

    IProcessComunicator * WebDriverProcessComunicatorFactory::CreateProcessComunicator(const QString& BrowserEngine)
    {
        WebDriveProcessComunicator * Comunicator = new WebDriveProcessComunicator();
        if(IsRecord)
        {
            Comunicator->SetIsRecord();
            Comunicator->SetRecordId(RecordId);
        }
        Comunicator->SetLogger(Logger);
        Comunicator->SetBrowserEngine(BrowserEngine);
        Comunicator->SetHttpClientFactory(HttpClientFactory);

        connect(Comunicator, SIGNAL(NewSession(QStringList,QString,int)), this, SLOT(NewSession(QStringList,QString,int)));
        connect(this, SIGNAL(SessionCreated(int,QString,int,bool,QString,bool,QString)), Comunicator, SLOT(SessionCreated(int,QString,int,bool,QString,bool,QString)));
        connect(this, SIGNAL(LockProfile(int,QString)), Comunicator, SLOT(LockProfile(int,QString)));

        return Comunicator;
    }

    void WebDriverProcessComunicatorFactory::NewSession(QStringList arguments, QString BrowserEngine, int CommunicatorId)
    {
        Log("New session. BrowserEngine = %1, Communicator Id = %2, Arguments = %3",BrowserEngine,QString::number(CommunicatorId),arguments.join(" "));
        WebDriverUnit &Unit = Drivers[BrowserEngine];
        WebDriverUnit::ProcessStateEnum OriginalState = Unit.ProcessState;

        if(OriginalState == WebDriverUnit::NotStarted)
        {
            if(BrowserEngine.startsWith("WebDriver"))
            {
                Unit.ProcessState = WebDriverUnit::Starting;
                Unit.BrowserEngine = BrowserEngine;
                Unit.WebDriverProcess = new QProcess();
                connect(Unit.WebDriverProcess,SIGNAL(started()),this,SLOT(ProcessStarted()));
                QString BrowserEngineProcessPath = QString("webdriver/");
                QStringList args;
                Unit.Port = qrand() % 10000 + 10000;
                args.append(QString("--port=%1").arg(QString::number(Unit.Port)));


                if(BrowserEngine == "WebDriverChrome")
                {
                    BrowserEngineProcessPath += "chromedriver.exe";
                }else if(BrowserEngine == "WebDriverFirefox")
                {
                    BrowserEngineProcessPath += "geckodriver.exe";
                }

                Log("WebDriver process not started. Starting... Port = %1, Path = %2",QString::number(Unit.Port), BrowserEngineProcessPath);

                WebDriverUnit::PendingUnit P;
                P.Id = CommunicatorId;
                P.Arguments = arguments;
                P.Type = WebDriverUnit::NewSession;
                Unit.Pending.append(P);

                Unit.WebDriverProcess->start(BrowserEngineProcessPath,args);
            }else if(BrowserEngine.startsWith("MLA"))
            {
                Unit.ProcessState = WebDriverUnit::Starting;
                Unit.BrowserEngine = BrowserEngine;
                Unit.WebDriverProcess = 0;
                Unit.Port = MLAPort;

                WebDriverUnit::PendingUnit P;
                P.Id = CommunicatorId;
                P.Arguments = arguments;
                P.Type = WebDriverUnit::NewSession;
                Unit.Pending.append(P);

                UpdateDriverPath();
            }


        }

        if(OriginalState == WebDriverUnit::Starting)
        {
            Log("WebDriver process is starting right now. Adding pending task.");

            WebDriverUnit::PendingUnit P;
            P.Id = CommunicatorId;
            P.Arguments = arguments;
            P.Type = WebDriverUnit::NewSession;
            Unit.Pending.append(P);
        }

        if(OriginalState == WebDriverUnit::Working)
        {
            Log("WebDriver process is already started. Starting new session.");
            if(BrowserEngine.startsWith("MLA"))
            {
                MlaCreateProfile(arguments, BrowserEngine, CommunicatorId);
            }else
                CreateNewSessionFor(arguments, BrowserEngine, CommunicatorId);
        }
    }



    void WebDriverProcessComunicatorFactory::ProcessStarted()
    {

        QProcess * Process = qobject_cast<QProcess*>(sender());
        Log("WebDriver process started. Process id = %1.",QString::number(Process->processId()));
        QMutableMapIterator<QString,WebDriverUnit> i(Drivers);
        while (i.hasNext())
        {
            i.next();
            WebDriverUnit &Unit = i.value();
            if(Unit.WebDriverProcess == Process)
            {
                Unit.ProcessState = WebDriverUnit::Working;
                for(WebDriverUnit::PendingUnit& p:Unit.Pending)
                {
                    Log("Start pending task for communicator %1.",QString::number(p.Id));
                    CreateNewSessionFor(p.Arguments,Unit.BrowserEngine,p.Id);
                }
                Unit.Pending.clear();
            }
        }
    }

    void WebDriverProcessComunicatorFactory::CreateNewSessionFor(const QStringList& Arguments, const QString& BrowserEngine, int CommunicatorId)
    {
        Log("Create new session. BrowserEngine = %1, CommunicatorId = %2, Arguments = %3",BrowserEngine, QString::number(CommunicatorId), Arguments.join(" "));


        IHttpClient * HttpClient = HttpClientFactory->GetHttpClient(false);
        HttpClient->setParent(this);
        HttpClient->Connect(this,SLOT(SessionCreateResult()));
        HttpClient->setProperty("CommunicatorId",CommunicatorId);
        HttpClient->setProperty("Port",Drivers[BrowserEngine].Port);


        QVariantList ArgsList;
        for(const QString& Arg:Arguments)
        {
            ArgsList.append(Arg);
        }
        QVariantMap ArgsMap;
        ArgsMap["args"] = ArgsList;
        QJsonObject object = QJsonObject::fromVariantMap(ArgsMap);

        QJsonDocument document;
        document.setObject(object);

        QString ArgumentString = QString::fromUtf8(document.toJson());



        QHash<QString,QString> p;
        if(BrowserEngine == "WebDriverChrome")
        {
            p["data"] = QString("{\"desiredCapabilities\":{\"browserName\":\"chrome\",\"goog:chromeOptions\":") + ArgumentString + QString("}}");
            //p["data"] = "{\"desiredCapabilities\":{\"browserName\":\"chrome\",\"goog:chromeOptions\":{\"args\":[\"--headless-none\"]}}}";
        }else
        {
            p["data"] = "{\"desiredCapabilities\":{\"browserName\":\"firefox\",\"moz:firefoxOptions\":{}},\"capabilities\":{\"firstMatch\":[{\"browserName\":\"firefox\",\"moz:firefoxOptions\":{}}]}}";
        }

        PostOptions Options;
        Options.PrepareStrategy = "custom";
        HttpClient->AddHeader("Content-Type","application/json");

        QString url = QString("http://127.0.0.1:%1/session").arg(QString::number(Drivers[BrowserEngine].Port));
        Log("-> [%1] %2", url, p["data"]);
        HttpClient->Post(url, p, Options);

    }

    void WebDriverProcessComunicatorFactory::SessionCreateResult()
    {
        sender()->deleteLater();
        IHttpClient * HttpClient = qobject_cast<IHttpClient*>(sender());
        Log("<- [%1] %2", HttpClient->GetLastUrl(), HttpClient->GetContent());

        QJsonDocument JsonResponse = QJsonDocument::fromJson(HttpClient->GetPageData());
        QJsonObject JsonObj = JsonResponse.object();
        QString SessionId;
        if(JsonObj.contains("sessionId"))
            SessionId = JsonObj["sessionId"].toString();
        else
            SessionId = JsonObj["value"].toObject()["sessionId"].toString();
        int CommunicatorId = HttpClient->property("CommunicatorId").toInt();
        int Port = HttpClient->property("Port").toInt();
        Log("SessionCreated. CommunicatorId = %1, SessionId = %2", QString::number(CommunicatorId), SessionId);
        emit SessionCreated(CommunicatorId, SessionId, Port, false, QString(), false, QString());
    }

    void WebDriverProcessComunicatorFactory::ScheduleCheckIfMetainfoIsUpdated()
    {
        QTimer::singleShot(1000,this,SLOT(CheckIfMetainfoIsUpdated()));
        TimesCheckedMetainfoIsUpdated ++;
    }

    void WebDriverProcessComunicatorFactory::CheckIfMetainfoIsUpdated()
    {
        Log("Check if metainfo is updated ... ");

        bool IsCanConnect;
        bool IsNoInfo;

        if(!SearchForMultiloginMetainfo(IsCanConnect,IsNoInfo))
        {
            if(IsCanConnect)
            {
                ScheduleCheckIfMLAPortOpen();
            }else
            {
                if(TimesCheckedMetainfoIsUpdated > 30)
                {
                    FailMLAProcessStart("Metainfo is not updated for a long time. This may happend because of using old Multilogin version");
                    return;
                }
                ScheduleCheckIfMetainfoIsUpdated();
            }
        }
    }

    void WebDriverProcessComunicatorFactory::ScheduleCheckIfMLAPortOpen()
    {
        QTimer::singleShot(1000,this,SLOT(CheckIfMLAPortOpen()));
        TimesCheckedMlaPort ++;
    }

    void WebDriverProcessComunicatorFactory::CheckIfMLAPortOpen()
    {
        Log("Check if %1 port open ... ", QString::number(MLAPort));

        IHttpClient * HttpClient = HttpClientFactory->GetHttpClient(false);
        HttpClient->setParent(this);
        HttpClient->Connect(this,SLOT(CheckMLAPortResult()));

        QString url = QString("http://127.0.0.1:%1/api/v1/webdriver/status").arg(QString::number(MLAPort));
        Log("-> [%1]", url);
        HttpClient->Get(url);
    }

    void WebDriverProcessComunicatorFactory::CheckMLAPortResult()
    {
        sender()->deleteLater();
        IHttpClient * HttpClient = qobject_cast<IHttpClient*>(sender());
        Log("<- [%1] %2", HttpClient->GetLastUrl(), HttpClient->GetContent());

        if(HttpClient->WasError())
        {
            if(TimesCheckedMlaPort > 30)
            {
                FailMLAProcessStart("Failed to start MLA for a long time");
                return;
            }
            ScheduleCheckIfMLAPortOpen();
        }else
        {
            CheckStatusResponce(HttpClient);
        }
    }

    void WebDriverProcessComunicatorFactory::CheckStatusResponce(IHttpClient * HttpClient)
    {
        QJsonDocument JsonResponse = QJsonDocument::fromJson(HttpClient->GetPageData());
        QJsonObject JsonObj = JsonResponse.object();

        if(!JsonObj.contains("state") || JsonObj["state"].toString() != "success")
        {
            FailMLAProcessStart(QString("http://127.0.0.1:%1/api/v1/webdriver/status returned strange result: ").arg(QString::number(MLAPort)) + HttpClient->GetContent());
            return;
        }
        ExtractMLAToken();

    }

    void WebDriverProcessComunicatorFactory::UpdateDriverPath()
    {
        if(IsCustomDriver)
        {
            QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
            if(!env.contains("HOMEDRIVE") || !env.contains("HOMEPATH"))
            {
                FailMLAProcessStart("No %HOMEDRIVE% or %HOMEPATH% variable");
                return;
            }
            QString SettingsPath = env.value("HOMEDRIVE") + env.value("HOMEPATH") + QString("/.multiloginapp.com/app.properties");
            if(!QFile(SettingsPath).exists())
            {
                FailMLAProcessStart(QString("File %1 not exists").arg(SettingsPath));
                return;
            }

            QSettings Settings(SettingsPath, QSettings::IniFormat);

            QString ChromeDriverPath = QFileInfo("webdriver/chromedriver.exe").absoluteFilePath();
            if(Settings.value("mimic.driver_path_manual","").toString() != ChromeDriverPath)
            {
                Log("Writting mimic.driver_path_manual as %1", ChromeDriverPath);
                Settings.setValue("mimic.driver_path_manual",ChromeDriverPath);
                Settings.sync();
            }
        }

        DetectMLAPort();
    }

    void WebDriverProcessComunicatorFactory::ExtractMLAToken()
    {
        Log("Extracting MLA token");

        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        if(!env.contains("HOMEDRIVE") || !env.contains("HOMEPATH"))
        {
            FailMLAProcessStart("No %HOMEDRIVE% or %HOMEPATH% variable");
            return;
        }
        QString SettingsPath = env.value("HOMEDRIVE") + env.value("HOMEPATH") + QString("/.multiloginapp.com/app.properties");
        if(!QFile(SettingsPath).exists())
        {
            FailMLAProcessStart(QString("File %1 not exists").arg(SettingsPath));
            return;
        }

        QSettings Settings(SettingsPath, QSettings::IniFormat);
        MLAToken = Settings.value("token",QString()).toString();
        if(MLAToken.isEmpty())
        {
            FailMLAProcessStart("MLA token is empty");
            return;
        }

        if(!IsCustomDriver && Settings.contains("mimic.driver_path_manual"))
        {
            Log("Removing mimic.driver_path_manual");
            Settings.remove("mimic.driver_path_manual");
            Settings.sync();
        }

        Log("Found token %1...", MLAToken);

        MLAAutoDeleteOldProfiles();
    }

    void WebDriverProcessComunicatorFactory::MLAAutoDeleteOldProfiles()
    {
        IHttpClient * HttpClient = HttpClientFactory->GetHttpClient(false);
        HttpClient->setParent(this);
        HttpClient->Connect(this,SLOT(MLAAutoDeleteOldProfilesResult()));

        QVariantMap ParamsMap;
        QVariantList Fingerprints;

        QString ProfilePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QString("/MultiloginProfiles/");
        QDirIterator directories(ProfilePath, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
        while(directories.hasNext())
        {
            directories.next();
            QString dir = directories.filePath();
            QString LockFilePath = QDir::cleanPath(dir + QDir::separator() + "lock.txt");
            QFile LockFile(LockFilePath);
            LockFile.remove();
            if(!LockFile.exists())
            {
                QString FingerprintIdPath = QDir::cleanPath(dir + QDir::separator() + "fingerprint_id.txt");
                QFile FingerprintFile(FingerprintIdPath);
                FingerprintFile.open(QIODevice::ReadOnly);
                QString FingerprintId = QString::fromUtf8(FingerprintFile.readAll());
                FingerprintFile.close();
                QDir(dir).removeRecursively();
                Log("Removing folder %1", dir);
                if(!FingerprintId.isEmpty())
                    Fingerprints.append(FingerprintId);
            }


        }

        if(Fingerprints.empty())
        {
            CreateMLASessions();
            return;
        }

        ParamsMap["fingerprintIds"] = Fingerprints;

        QJsonObject object = QJsonObject::fromVariantMap(ParamsMap);
        QJsonDocument document;
        document.setObject(object);

        QString Params = QString::fromUtf8(document.toJson());


        QHash<QString,QString> p;
        p["data"] = Params;
        PostOptions Options;
        Options.PrepareStrategy = "custom";
        HttpClient->AddHeader("Content-Type","application/json");

        QString url = QString("http://localhost:%1/api/v1/localprofile/remove").arg(QString::number(MLAPort));
        Log("-> [%1] %2", url, p["data"]);
        HttpClient->Post(url, p, Options);



    }

    void WebDriverProcessComunicatorFactory::MLAAutoDeleteOldProfilesResult()
    {
        sender()->deleteLater();
        IHttpClient * HttpClient = qobject_cast<IHttpClient*>(sender());

        Log("<- [%1] %2", HttpClient->GetLastUrl(), HttpClient->GetContent());

        CreateMLASessions();

    }

    bool WebDriverProcessComunicatorFactory::SearchForMultiloginMetainfo(bool& IsCanConnect,bool& IsNoInfo)
    {
        IsCanConnect = false;
        IsNoInfo = false;
        bool IsOldVersion = false;
        QString OldVersionString;

        Log("Reading Multilogin meta");

        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        if(!env.contains("HOMEDRIVE") || !env.contains("HOMEPATH"))
        {
            FailMLAProcessStart("No %HOMEDRIVE% or %HOMEPATH% variable");
            return true;
        }

        QDir dir(env.value("HOMEDRIVE") + env.value("HOMEPATH") + QString("/.multiloginapp.com/metainfo"));
        dir.setNameFilters(QStringList() << "*.*");
        dir.setFilter(QDir::Files);
        foreach(QString dirFile, dir.entryList())
        {
            dir.remove(dirFile);
            QString MultiloginFile = dir.absoluteFilePath(dirFile);

            QFile File(MultiloginFile);
            if(File.open(QIODevice::ReadOnly))
            {
                QByteArray json = File.readAll();
                QJsonDocument JsonResponse = QJsonDocument::fromJson(json);
                QJsonObject JsonObj = JsonResponse.object();
                int Port = JsonObj["port"].toInt();
                QString VersionString = JsonObj["version"].toString().split(" ").first();

                QStringList Version = VersionString.split(".");
                if(Version.size() == 3 && (Version[0].toInt() > 2 || (Version[0].toInt() == 2 && Version[1].toInt() > 1 ) || (Version[0].toInt() == 2 && Version[1].toInt() == 1 && Version[2].toInt() >= 4)))
                {
                    Log("Found running instance with open port %1",QString::number(Port));
                    MLAPort = Port;
                    IsCanConnect = true;
                    IsNoInfo = false;
                    return false;
                }else
                {
                    OldVersionString = VersionString;
                    IsOldVersion = true;
                }
            }else
            {
                MLAPort = 35000;
                IsCanConnect = true;
                IsNoInfo = false;
                return false;
            }


        }
        if(IsOldVersion)
        {
            FailMLAProcessStart(QString("You are using old Multilogin version %1, please update.").arg(OldVersionString));
            return true;
        }
        Log("No running instance found");

        IsNoInfo = true;
        return false;
    }

    void WebDriverProcessComunicatorFactory::StartMLAProcess()
    {
        Log("MLA is not running, starting ... ");

        if(!QFile(MLALocation).exists())
        {
            FailMLAProcessStart(MLALocation + QString(" doesn't exist, is MLA installed?"));
            return;
        }

        QProcess * Process = new QProcess();
        connect(Process,SIGNAL(finished(int)),Process,SLOT(deleteLater()));
        Process->start(MLALocation,QStringList());

        TimesCheckedMlaPort = 0;
        TimesCheckedMetainfoIsUpdated = 0;
        ScheduleCheckIfMetainfoIsUpdated();
    }


    void WebDriverProcessComunicatorFactory::DetectMLAPort()
    {
        bool IsCanConnect;
        bool IsNoInfo;
        if(!SearchForMultiloginMetainfo(IsCanConnect,IsNoInfo))
        {
            if(IsCanConnect)
            {
                Log("Found Multilogin running on %1 port. Trying to connect.",QString::number(MLAPort));
                CheckIfMLAPortOpenFirstTimeAfterCanConnect();
            }
            if(IsNoInfo)
            {
                CheckIfMLAPortOpenFirstTimeAfterNoInfo();
            }
        }
    }

    void WebDriverProcessComunicatorFactory::CheckIfMLAPortOpenFirstTimeAfterCanConnect()
    {
        Log("Check if %1 port open ... ",QString::number(MLAPort));

        IHttpClient * HttpClient = HttpClientFactory->GetHttpClient(false);
        HttpClient->setParent(this);
        HttpClient->Connect(this,SLOT(CheckMLAPortFirstTimeResultAfterCanConnect()));

        QString url = QString("http://127.0.0.1:%1/api/v1/webdriver/status").arg(QString::number(MLAPort));
        Log("-> [%1]", url);
        HttpClient->Get(url);
    }

    void WebDriverProcessComunicatorFactory::CheckMLAPortFirstTimeResultAfterCanConnect()
    {
        sender()->deleteLater();
        IHttpClient * HttpClient = qobject_cast<IHttpClient*>(sender());
        Log("<- [%1] %2", HttpClient->GetLastUrl(), HttpClient->GetContent());

        if(HttpClient->WasError())
        {
            FailMLAProcessStart("Multilogin port is closed, but there is information in metainfo, something goes wrong.");
        }else
        {
            CheckStatusResponce(HttpClient);
        }
    }

    void WebDriverProcessComunicatorFactory::CheckIfMLAPortOpenFirstTimeAfterNoInfo()
    {
        Log("Check if %1 port open ... ",QString::number(MLAPort));

        IHttpClient * HttpClient = HttpClientFactory->GetHttpClient(false);
        HttpClient->setParent(this);
        HttpClient->Connect(this,SLOT(CheckMLAPortFirstTimeResultAfterNoInfo()));

        QString url = QString("http://127.0.0.1:%1/api/v1/webdriver/status").arg(QString::number(MLAPort));
        Log("-> [%1]", url);
        HttpClient->Get(url);
    }

    void WebDriverProcessComunicatorFactory::CheckMLAPortFirstTimeResultAfterNoInfo()
    {
        sender()->deleteLater();
        IHttpClient * HttpClient = qobject_cast<IHttpClient*>(sender());
        Log("<- [%1] %2", HttpClient->GetLastUrl(), HttpClient->GetContent());

        if(HttpClient->WasError())
        {
            StartMLAProcess();
        }else
        {
            FailMLAProcessStart("Multilogin port is open, but no information in metainfo, so you are using old Multilogin version, please update ...");
        }
    }

    void WebDriverProcessComunicatorFactory::CreateMLASessions()
    {

        Log("Creating profiles and start sessions ... ");

        QMutableMapIterator<QString,WebDriverUnit> i(Drivers);
        while (i.hasNext())
        {
            i.next();
            WebDriverUnit &Unit = i.value();
            if(Unit.BrowserEngine.startsWith("MLA"))
            {
                Unit.ProcessState = WebDriverUnit::Working;
                for(WebDriverUnit::PendingUnit& p:Unit.Pending)
                {
                    Log("Start pending task for communicator %1.",QString::number(p.Id));
                    MlaCreateProfile(p.Arguments,Unit.BrowserEngine,p.Id);
                }
                Unit.Pending.clear();
            }
        }
    }

    void WebDriverProcessComunicatorFactory::MlaCreateProfile(const QStringList& Arguments, const QString& BrowserEngine, int CommunicatorId)
    {
        bool NeedUpdateFingerprintFactors = false;
        QString ProfilePath;
        QStringList ArgumentsSorted;
        bool LoadFingerprintFromProfileFolder = false;

        //Find profile
        for(const QString& arg:Arguments)
        {
            if(arg.startsWith("Profile="))
            {
                ProfilePath = arg;

                ProfilePath = ProfilePath.mid(8);
                Log("Creating profile for %1", ProfilePath);
            }else if(arg.startsWith("LoadFingerprintFromProfileFolder="))
            {
                QString LoadFingerprintFromProfileFolderString = arg;
                LoadFingerprintFromProfileFolder = LoadFingerprintFromProfileFolderString.mid(33) == "true";
            }else
            {
                if(arg.startsWith("proxyHost=") || arg.startsWith("proxyType=") || arg.startsWith("proxyUser=") || arg.startsWith("proxyPass=") || arg.startsWith("proxyPort="))
                    continue;

                ArgumentsSorted.append(arg.trimmed());
            }
        }
        ArgumentsSorted.sort();

        if(ProfilePath.isEmpty())
        {
            ProfilePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QString("/MultiloginProfiles/");
            QDir dir(ProfilePath);
            ProfilePath = dir.absoluteFilePath(GetRandomString());
            QDir(ProfilePath).mkpath(".");
            Log("Create new temporary local profile at %1", ProfilePath);
            emit LockProfile(CommunicatorId, ProfilePath);
            NeedUpdateFingerprintFactors = true;
            MlaCreateLocalFingerprint(Arguments, BrowserEngine, CommunicatorId, ProfilePath);

        }else if(ProfilePath.startsWith("mla_id:"))
        {
            QString OnlineProfileId = ProfilePath.mid(7);
            Log("Use existing profile and find it by id &1", OnlineProfileId);
            CreateNewMLASessionFor(BrowserEngine, CommunicatorId, OnlineProfileId);
        }else if(ProfilePath.startsWith("mla_name:"))
        {
            QString OnlineProfileName = ProfilePath.mid(9);
            Log("Create new online profile with name %1", OnlineProfileName);
            MlaCreateOnlineProfile(Arguments, BrowserEngine, CommunicatorId, OnlineProfileName);
        }else
        {
            NeedUpdateFingerprintFactors = true;
            if(QDir(ProfilePath).exists())
            {
                emit LockProfile(CommunicatorId, ProfilePath);
                QString FingerprintIdPath = QDir::cleanPath(ProfilePath + QDir::separator() + "fingerprint_id.txt");
                if(QFile(FingerprintIdPath).exists())
                {
                    bool IsFactorSame = true;

                    if(!LoadFingerprintFromProfileFolder)
                    {
                        Log("Fingerprint exists, check if fingerprint factors has changed");
                        QFile FingerpintsFactorFile(QDir::cleanPath(ProfilePath + QDir::separator() + "fingerprint_factors.txt"));
                        FingerpintsFactorFile.open(QIODevice::ReadOnly);
                        QTextStream in(&FingerpintsFactorFile);
                        int index = 0;
                        while (!in.atEnd())
                        {
                            QString line = in.readLine().trimmed();
                            if(ArgumentsSorted.length() <= index || ArgumentsSorted[index] != line)
                            {
                                IsFactorSame = false;
                                break;
                            }
                            index++;
                        }
                        FingerpintsFactorFile.close();
                    }else
                    {
                        //If LoadFingerprintFromProfileFolder is set, than always load existing fingerprint
                        IsFactorSame = true;
                    }

                    if(IsFactorSame)
                    {
                        Log("Fingerprints is the same, just start profile");
                        QFile FingerprintFile(FingerprintIdPath);
                        FingerprintFile.open(QIODevice::ReadOnly);
                        QString FingerprintId = QString::fromUtf8(FingerprintFile.readAll());
                        FingerprintFile.close();
                        MlaCreateLocalProfile(Arguments, BrowserEngine, CommunicatorId, ProfilePath, FingerprintId);
                    }else
                    {
                        Log("Regenerating fingerprint");
                        MlaCreateLocalFingerprint(Arguments, BrowserEngine, CommunicatorId, ProfilePath);
                    }



                }else
                {
                    Log("Fingerprint not exists, generate it and start profile");
                    MlaCreateLocalFingerprint(Arguments, BrowserEngine, CommunicatorId, ProfilePath);
                }
            }else
            {
                Log("Directory not exists, generate it and create new profile");
                QDir(ProfilePath).mkpath(".");
                emit LockProfile(CommunicatorId, ProfilePath);
                MlaCreateLocalFingerprint(Arguments, BrowserEngine, CommunicatorId, ProfilePath);
            }

        }

        if(NeedUpdateFingerprintFactors)
        {
            Log("Updating fingerprint factors");
            QFile FingerpintsFactorFile(ProfilePath + QString("/fingerprint_factors.txt"));
            FingerpintsFactorFile.open(QIODevice::WriteOnly);
            QTextStream ts(&FingerpintsFactorFile);
            for(const QString& arg:ArgumentsSorted)
            {

                if(!arg.startsWith("Profile=") && !arg.startsWith("LoadFingerprintFromProfileFolder="))
                {
                    ts<<arg<<"\n";
                }
            }
            FingerpintsFactorFile.close();
        }
    }

    void WebDriverProcessComunicatorFactory::MlaCreateLocalFingerprint(const QStringList& Arguments, const QString& BrowserEngine, int CommunicatorId, const QString& ProfilePath)
    {
        IHttpClient * HttpClient = HttpClientFactory->GetHttpClient(false);
        HttpClient->setParent(this);
        HttpClient->Connect(this,SLOT(MlaCreateLocalFingerprintResult()));
        HttpClient->setProperty("CommunicatorId",CommunicatorId);
        HttpClient->setProperty("BrowserEngine",BrowserEngine);
        HttpClient->setProperty("ProfilePath",ProfilePath);
        HttpClient->setProperty("Arguments",Arguments);

        QVariantMap ParamsMap = GenerateMlaSettings(Arguments, false);

        QJsonObject object = QJsonObject::fromVariantMap(ParamsMap);
        QJsonDocument document;
        document.setObject(object);

        QString Params = QString::fromUtf8(document.toJson());


        QHash<QString,QString> p;
        p["data"] = Params;
        PostOptions Options;
        Options.PrepareStrategy = "custom";
        HttpClient->AddHeader("Content-Type","application/json");

        QString url = QString("http://localhost:%1/api/v1/localprofile/generate").arg(QString::number(MLAPort));
        Log("-> [%1] %2", url, p["data"]);
        HttpClient->Post(url, p, Options);
    }

    void WebDriverProcessComunicatorFactory::MlaCreateLocalFingerprintResult()
    {
        sender()->deleteLater();
        IHttpClient * HttpClient = qobject_cast<IHttpClient*>(sender());
        int CommunicatorId = HttpClient->property("CommunicatorId").toInt();
        QString BrowserEngine = HttpClient->property("BrowserEngine").toString();
        QString ProfilePath = HttpClient->property("ProfilePath").toString();
        QStringList Arguments = HttpClient->property("Arguments").toStringList();

        Log("<- [%1] %2", HttpClient->GetLastUrl(), HttpClient->GetContent());

        if(HttpClient->WasError())
        {
            FailMLASessionStart(QString("This may problem may appear because your license don't allow automation. Http error \"%1\" during http://localhost:%2/api/v1/localprofile/generate").arg(HttpClient->GetErrorString()).arg(QString::number(MLAPort)), CommunicatorId);
            return;
        }else
        {
            QJsonDocument JsonResponse = QJsonDocument::fromJson(HttpClient->GetPageData());
            QJsonObject JsonObj = JsonResponse.object();

            if(!JsonObj.contains("status") || !JsonObj.contains("value"))
            {
                FailMLASessionStart(QString("http://localhost:%1/api/v1/localprofile/generate returned strange result: ").arg(QString::number(MLAPort)) + HttpClient->GetContent(), CommunicatorId);
                return;
            }

            if(JsonObj["status"].toString().toUpper() != QString("OK"))
            {
                FailMLASessionStart(QString("Failed to create ingerprint with error \"%1\"").arg(JsonObj["value"].toString()), CommunicatorId);
                return;
            }
            QString FingerprintId = JsonObj["value"].toString();
            QDir dir(ProfilePath);
            QFile OutFile(dir.absoluteFilePath("fingerprint_id.txt"));
            OutFile.open(QIODevice::WriteOnly);
            QTextStream ts(&OutFile);
            ts << FingerprintId;
            OutFile.close();
            QDir(ProfilePath).mkpath("profile");
            MlaCreateLocalProfile(Arguments, BrowserEngine, CommunicatorId, ProfilePath, FingerprintId);
        }
    }

    void WebDriverProcessComunicatorFactory::MlaCreateLocalProfile(const QStringList& Arguments, const QString& BrowserEngine, int CommunicatorId, const QString& ProfilePath, const QString& FingerprintId)
    {
        Log("Create new local mla profile. BrowserEngine = %1, CommunicatorId = %2, Arguments = %3",BrowserEngine, QString::number(CommunicatorId), Arguments.join(" "));
        Log("ProfilePath = %1, FingerprintId = %2",ProfilePath, FingerprintId);

        IHttpClient * HttpClient = HttpClientFactory->GetHttpClient(false);
        HttpClient->setParent(this);
        HttpClient->Connect(this,SLOT(MlaCreateLocalProfileResult()));
        HttpClient->setProperty("CommunicatorId",CommunicatorId);
        HttpClient->setProperty("BrowserEngine",BrowserEngine);
        HttpClient->setProperty("ProfilePath",ProfilePath);
        HttpClient->setProperty("FingerprintId",FingerprintId);


        QByteArray ProfilePathEncodedBytes = QUrl::toPercentEncoding(QDir(ProfilePath).absoluteFilePath("profile"));
        QString ProfilePathEncoded = QString::fromUtf8(ProfilePathEncodedBytes);

        QString url = QString("http://localhost:%1/api/v1/localprofile/start?fingerprintId=").arg(QString::number(MLAPort)) + FingerprintId + QString("&profilePath=") + ProfilePathEncoded;

        QVariantMap ParamsMap;

        ParamsMap["ipValidation"] = false;

        for(const QString& Argument:Arguments)
        {
            if(!(Argument.startsWith("proxyHost=") || Argument.startsWith("proxyType=") || Argument.startsWith("proxyUser=") || Argument.startsWith("proxyPass=") || Argument.startsWith("proxyPort=")))
                continue;

            int index = Argument.indexOf('=');
            if(index < 0)
                continue;
            QString Key = Argument.mid(0,index);
            if(Key == "proxyHost")
                Key = "host";
            if(Key == "proxyType")
                Key = "type";
            if(Key == "proxyUser")
                Key = "user";
            if(Key == "proxyPass")
                Key = "pass";
            if(Key == "proxyPort")
                Key = "port";
            QString Value = Argument.mid(index + 1);
            QJsonDocument JsonResponse = QJsonDocument::fromJson((QString("{\"data\":") + Value + QString("}")).toUtf8());

            QJsonObject JsonObj = JsonResponse.object();
            QJsonValue JsonValue = JsonObj["data"];
            QVariant VariantValue = JsonValue.toVariant();
            if(Key == "type")
            {
                VariantValue = VariantValue.toString().toUpper();
            }

            ParamsMap.insert(Key,VariantValue);
        }

        QJsonObject object = QJsonObject::fromVariantMap(ParamsMap);
        QJsonDocument document;
        document.setObject(object);

        QString Params = QString::fromUtf8(document.toJson());


        QHash<QString,QString> p;
        p["data"] = Params;
        PostOptions Options;
        Options.PrepareStrategy = "custom";
        HttpClient->AddHeader("Content-Type","application/json");

        Log(QString("-> [") + url + QString("] ") + p["data"]);
        HttpClient->Post(url, p, Options);

    }

    void WebDriverProcessComunicatorFactory::MlaCreateLocalProfileResult()
    {
        sender()->deleteLater();
        IHttpClient * HttpClient = qobject_cast<IHttpClient*>(sender());
        int CommunicatorId = HttpClient->property("CommunicatorId").toInt();
        QString BrowserEngine = HttpClient->property("BrowserEngine").toString();
        QString ProfilePath = HttpClient->property("ProfilePath").toString();
        QString FingerprintId = HttpClient->property("FingerprintId").toString();

        Log(QString("<- [") + HttpClient->GetLastUrl() + QString("] ") + HttpClient->GetContent());


        if(HttpClient->WasError())
        {
            FailMLASessionStart(QString("Http error \"%1\" during MLA create local profile").arg(HttpClient->GetErrorString()), CommunicatorId);
            return;
        }else
        {
            QJsonDocument JsonResponse = QJsonDocument::fromJson(HttpClient->GetPageData());
            QJsonObject JsonObj = JsonResponse.object();

            if(!JsonObj.contains("status"))
            {
                FailMLASessionStart(QString("http://localhost:%1/api/v1/localprofile/start returned strange result: ").arg(QString::number(MLAPort)) + HttpClient->GetContent(), CommunicatorId);
                return;
            }

            if(JsonObj["status"].toString().toUpper() != QString("OK"))
            {
                FailMLASessionStart(QString("Failed to start profile with error \"%1\"").arg(JsonObj["value"].toString()), CommunicatorId);
                return;
            }
            QString ProfileId = FingerprintId;
            MLAGetMetaForLocalProfile(ProfileId, CommunicatorId, ProfilePath, FingerprintId);
        }
    }

    void WebDriverProcessComunicatorFactory::MLAGetMetaForLocalProfile(const QString& ProfileId, int CommunicatorId, const QString& ProfilePath, const QString& FingerprintId)
    {
        Log("Getting ProfileMeta for local profile. CommunicatorId = %1, ProfileId = %2", QString::number(CommunicatorId), ProfileId);
        Log("ProfilePath = %1, FingerprintId = %2", ProfilePath, FingerprintId);

        IHttpClient * HttpClient = HttpClientFactory->GetHttpClient(false);
        HttpClient->setParent(this);
        HttpClient->Connect(this,SLOT(MLAGetMetaForLocalProfileResult()));
        HttpClient->setProperty("CommunicatorId",CommunicatorId);
        HttpClient->setProperty("ProfileId",ProfileId);
        HttpClient->setProperty("ProfilePath",ProfilePath);
        HttpClient->setProperty("FingerprintId",FingerprintId);


        QString url = QString("http://127.0.0.1:%2/api/v1/start-profile/%1").arg(ProfileId).arg(QString::number(MLAPort));
        Log("-> [%1]", url);
        HttpClient->Get(url);
    }

    void WebDriverProcessComunicatorFactory::MLAGetMetaForLocalProfileResult()
    {
        sender()->deleteLater();
        IHttpClient * HttpClient = qobject_cast<IHttpClient*>(sender());
        Log("<- [%1] %2", HttpClient->GetLastUrl(), HttpClient->GetContent());

        int CommunicatorId = HttpClient->property("CommunicatorId").toInt();
        QString ProfileId = HttpClient->property("ProfileId").toString();
        QString ProfilePath = HttpClient->property("ProfilePath").toString();
        QString FingerprintId = HttpClient->property("FingerprintId").toString();

        QJsonDocument JsonResponse = QJsonDocument::fromJson(HttpClient->GetPageData());
        QJsonObject JsonObj = JsonResponse.object();
        QString SessionId;
        int Port = 0;

        if(JsonObj.contains(SessionIdParameterName))
            SessionId = JsonObj[SessionIdParameterName].toString();

        if(JsonObj.contains(PortParameterName))
            Port = JsonObj[PortParameterName].toString().split(":").last().toInt();


        if(SessionId.isEmpty())
        {
            FailMLASessionStart(QString("Can't find session id in meta responce \"%1\"").arg(HttpClient->GetContent()), CommunicatorId);
            return;
        }
        if(Port <= 0)
        {
            FailMLASessionStart(QString("Can't find port in meta responce \"%1\"").arg(HttpClient->GetContent()), CommunicatorId);
            return;
        }

        Log("Meta obtained. SessionId = %1, ProfileId = %2, Port = %3", SessionId, ProfileId, QString::number(Port));
        Log("ProfilePath = %1, FingerprintId = %2, Port = %3", ProfilePath, FingerprintId, QString::number(Port));

        emit SessionCreated(CommunicatorId, SessionId + QString("|") + ProfilePath + QString("|") + MLAToken, Port, false, QString(), false, QString());
    }

    QVariantMap WebDriverProcessComunicatorFactory::GenerateMlaSettings(const QStringList& Arguments, bool WithProxy)
    {
        QVariantMap Raw;

        if(WithProxy)
            Raw["proxyIpValidation"] = false;

        Raw["forbidConcurrentExecution"] = true;

        Raw["generateZeroFingerprintsData"] = true;


        bool SetsGeolocation = false;
        bool SetsTimezone = false;
        for(const QString& Argument:Arguments)
        {
            if(!WithProxy)
            {
                if(Argument.startsWith("proxyHost=") || Argument.startsWith("proxyType=") || Argument.startsWith("proxyUser=") || Argument.startsWith("proxyPass=") || Argument.startsWith("proxyPort="))
                    continue;
            }
            if(Argument.startsWith("Profile=") || Argument.startsWith("LoadFingerprintFromProfileFolder="))
                continue;
            int index = Argument.indexOf('=');
            if(index < 0)
                continue;
            QString Key = Argument.mid(0,index);

            if(Key == "timeZoneFillOnStart" || Key == "geolocation.fillOnStart")
                continue;

            if(Key == "timeZone")
            {
                SetsGeolocation = true;
            }

            if(Key == "geolocation.latitude")
            {
                SetsTimezone = true;
            }


            QString Value = Argument.mid(index + 1);
            QJsonDocument JsonResponse = QJsonDocument::fromJson((QString("{\"data\":") + Value + QString("}")).toUtf8());

            QJsonObject JsonObj = JsonResponse.object();
            QJsonValue JsonValue = JsonObj["data"];
            QVariant VariantValue = JsonValue.toVariant();

            if(Key == "proxyType" && VariantValue.toString() == "none")
            {
                continue;
            }

            Raw.insert(Key,VariantValue);
        }

        Raw.insert("timeZoneFillOnStart", !SetsGeolocation);
        Raw.insert("geolocation.fillOnStart", !SetsTimezone);



        while(true)
        {
            int LongestChain = 0;
            for(QVariantMap::const_iterator iter = Raw.begin(); iter != Raw.end(); ++iter)
            {
                QString key = iter.key();
                int Level = key.split(".").length();
                if(Level > LongestChain)
                    LongestChain = Level;
            }

            if(LongestChain <= 1)
            {
                break;
            }

            QString Prefix;
            for(QVariantMap::const_iterator iter = Raw.begin(); iter != Raw.end(); ++iter)
            {
                QString key = iter.key();
                QStringList split = key.split(".");
                int Level = split.length();
                if(Level == LongestChain)
                {
                    LongestChain = Level;
                    split.removeLast();
                    Prefix = split.join(".");
                    break;
                }
            }

            QVariantMap NewItem;
            QMutableMapIterator<QString, QVariant> i(Raw);
            while (i.hasNext())
            {
                i.next();
                QString key = i.key();
                QStringList split = key.split(".");

                if(key.startsWith(Prefix + QString(".")))
                {
                    NewItem[split.last()] = i.value();
                    i.remove();
                }
            }
            Raw[Prefix] = NewItem;

        }

        return Raw;
    }


    void WebDriverProcessComunicatorFactory::MlaCreateOnlineProfile(const QStringList& Arguments, const QString& BrowserEngine, int CommunicatorId, const QString& ProfileName)
    {
        Log("Create new mla profile. BrowserEngine = %1, CommunicatorId = %2, Arguments = %3",BrowserEngine, QString::number(CommunicatorId), Arguments.join(" "));

        IHttpClient * HttpClient = HttpClientFactory->GetHttpClient(false);
        HttpClient->setParent(this);
        HttpClient->Connect(this,SLOT(MlaCreateOnlineProfileResult()));
        HttpClient->setProperty("CommunicatorId",CommunicatorId);
        HttpClient->setProperty("BrowserEngine",BrowserEngine);

        QVariantMap ParamsMap = GenerateMlaSettings(Arguments, true);
        if(BrowserEngine == QString("MLAMimic"))
        {
            ParamsMap["browserType"] = "mimic";
        }

        ParamsMap["name"] = ProfileName;

        QJsonObject object = QJsonObject::fromVariantMap(ParamsMap);
        QJsonDocument document;
        document.setObject(object);

        QString Params = QString::fromUtf8(document.toJson());


        QHash<QString,QString> p;
        p["data"] = Params;
        PostOptions Options;
        Options.PrepareStrategy = "custom";
        HttpClient->AddHeader("Content-Type","application/json");

        QString url = ApiLocation + QString("v1/profile/create?token=%1").arg(MLAToken);
        Log("-> [%1] %2", url, p["data"]);
        HttpClient->Post(url, p, Options);
    }


    void WebDriverProcessComunicatorFactory::MlaCreateOnlineProfileResult()
    {
        sender()->deleteLater();
        IHttpClient * HttpClient = qobject_cast<IHttpClient*>(sender());
        int CommunicatorId = HttpClient->property("CommunicatorId").toInt();
        QString BrowserEngine = HttpClient->property("BrowserEngine").toString();
        Log("<- [%1] %2", HttpClient->GetLastUrl(), HttpClient->GetContent());

        if(HttpClient->WasError())
        {
            FailMLASessionStart(QString("Http error \"%1\" during MLA session start").arg(HttpClient->GetErrorString()), CommunicatorId);
            return;
        }else
        {
            QJsonDocument JsonResponse = QJsonDocument::fromJson(HttpClient->GetPageData());
            QJsonObject JsonObj = JsonResponse.object();

            if(!JsonObj.contains("status") || !JsonObj.contains("value"))
            {
                FailMLASessionStart(ApiLocation + QString("v1/profile/create returned strange result: ") + HttpClient->GetContent(), CommunicatorId);
                return;
            }

            if(JsonObj["status"].toString().toUpper() != QString("OK"))
            {
                FailMLASessionStart(QString("Failed to create profile with error \"%1\"").arg(JsonObj["value"].toString()), CommunicatorId);
                return;
            }
            QString ProfileId = JsonObj["value"].toString();
            CreateNewMLASessionFor(BrowserEngine, CommunicatorId, ProfileId);
        }
    }




    void WebDriverProcessComunicatorFactory::CreateNewMLASessionFor(const QString& BrowserEngine, int CommunicatorId, const QString& ProfileId)
    {
        Log("Create new MLA session. BrowserEngine = %1, CommunicatorId = %2, ProfileId = %3",BrowserEngine, QString::number(CommunicatorId), ProfileId);

        IHttpClient * HttpClient = HttpClientFactory->GetHttpClient(false);
        HttpClient->setParent(this);
        HttpClient->Connect(this,SLOT(MLASessionCreateResult()));
        HttpClient->setProperty("CommunicatorId",CommunicatorId);
        HttpClient->setProperty("BrowserEngine",BrowserEngine);
        HttpClient->setProperty("ProfileId",ProfileId);

        QHash<QString,QString> p;
        p["data"] = QString("{\"desiredCapabilities\":{\"multiloginapp-profileId\":\"") + ProfileId + QString("\"}}");

        PostOptions Options;
        Options.PrepareStrategy = "custom";
        HttpClient->AddHeader("Content-Type","application/json");

        QString url = QString("http://127.0.0.1:%1/api/v1/webdriver/session").arg(QString::number(Drivers[BrowserEngine].Port));
        Log("-> [%1] %2", url, p["data"]);
        HttpClient->Post(url, p, Options);

    }

    void WebDriverProcessComunicatorFactory::MLASessionCreateResult()
    {
        sender()->deleteLater();
        IHttpClient * HttpClient = qobject_cast<IHttpClient*>(sender());
        Log("<- [%1] %2", HttpClient->GetLastUrl(), HttpClient->GetContent());

        int CommunicatorId = HttpClient->property("CommunicatorId").toInt();
        QString ProfileId = HttpClient->property("ProfileId").toString();

        QJsonDocument JsonResponse = QJsonDocument::fromJson(HttpClient->GetPageData());
        QJsonObject JsonObj = JsonResponse.object();
        QString SessionId;
        if(JsonObj.contains("sessionId"))
            SessionId = JsonObj["sessionId"].toString();
        else
            SessionId = JsonObj["value"].toObject()["sessionId"].toString();
        if(SessionId.isEmpty())
        {
            FailMLASessionStart(QString("Can't find session id in responce \"%1\"").arg(HttpClient->GetContent()), CommunicatorId);
            return;
        }

        Log("MLA session created! CommunicatorId = %1, SessionId = %2, ProfileId = %3", QString::number(CommunicatorId), SessionId, ProfileId);

        MLAGetMeta(ProfileId, CommunicatorId);

    }

    void WebDriverProcessComunicatorFactory::MLAGetMeta(const QString& ProfileId, int CommunicatorId)
    {
        Log("Getting ProfileMeta. CommunicatorId = %1, ProfileId = %2", QString::number(CommunicatorId), ProfileId);

        IHttpClient * HttpClient = HttpClientFactory->GetHttpClient(false);
        HttpClient->setParent(this);
        HttpClient->Connect(this,SLOT(MLAGetMetaResult()));
        HttpClient->setProperty("CommunicatorId",CommunicatorId);
        HttpClient->setProperty("ProfileId",ProfileId);


        QString url = QString("http://127.0.0.1:%2/api/v1/start-profile/%1").arg(ProfileId).arg(QString::number(MLAPort));
        Log("-> [%1]", url);
        HttpClient->Get(url);
    }

    void WebDriverProcessComunicatorFactory::MLAGetMetaResult()
    {
        sender()->deleteLater();
        IHttpClient * HttpClient = qobject_cast<IHttpClient*>(sender());
        Log("<- [%1] %2", HttpClient->GetLastUrl(), HttpClient->GetContent());

        int CommunicatorId = HttpClient->property("CommunicatorId").toInt();
        QString ProfileId = HttpClient->property("ProfileId").toString();

        QJsonDocument JsonResponse = QJsonDocument::fromJson(HttpClient->GetPageData());
        QJsonObject JsonObj = JsonResponse.object();
        QString SessionId;
        int Port = 0;

        if(JsonObj.contains(SessionIdParameterName))
            SessionId = JsonObj[SessionIdParameterName].toString();

        if(JsonObj.contains(PortParameterName))
            Port = JsonObj[PortParameterName].toString().split(":").last().toInt();


        if(SessionId.isEmpty())
        {
            FailMLASessionStart(QString("Can't find session id in meta responce \"%1\"").arg(HttpClient->GetContent()), CommunicatorId);
            return;
        }
        if(Port <= 0)
        {
            FailMLASessionStart(QString("Can't find port in meta responce \"%1\"").arg(HttpClient->GetContent()), CommunicatorId);
            return;
        }

        Log("Meta obtained. SessionId = %1, ProfileId = %2, Port = %3", SessionId, ProfileId, QString::number(Port));

        emit SessionCreated(CommunicatorId, SessionId + QString("|mla_id:") + ProfileId + QString("|") + MLAToken, Port, false, QString(), false, QString());
    }


    void WebDriverProcessComunicatorFactory::FailMLAProcessStart(const QString& Message)
    {
        Log("Fail to start MLA process with message %1.", Message);
        QMutableMapIterator<QString,WebDriverUnit> i(Drivers);
        while (i.hasNext())
        {
            i.next();
            WebDriverUnit &Unit = i.value();
            if(Unit.BrowserEngine.startsWith("MLA"))
            {
                Unit.ProcessState = WebDriverUnit::Working;
                for(WebDriverUnit::PendingUnit& p:Unit.Pending)
                {
                    emit SessionCreated(p.Id, "", MLAPort, true, Message, false, QString());
                }
                Unit.Pending.clear();
                i.remove();
            }
        }
    }

    void WebDriverProcessComunicatorFactory::FailMLASessionStart(const QString& Message, int CommunicatorId)
    {
        Log("Fail to start MLA session for communicator %1 with message %2.", QString::number(CommunicatorId), Message);
        emit SessionCreated(CommunicatorId, "", MLAPort, false, QString(), true, Message);
    }

}
