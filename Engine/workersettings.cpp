#include "workersettings.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QDirIterator>
#include "devicescalemanager.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{

    const static int TIMEZONE_NONE = 99999;
    WorkerSettings::WorkerSettings(QObject *parent) :
        IWorkerSettings(parent)
    {
        Clear();
        BrowserEngine = "BASChrome";
        BrowserEngineVirtual = "BASChrome";
    }

    QString WorkerSettings::GetUniqueProcessId()
    {
        return UniqueProcessId;
    }

    void WorkerSettings::Clear()
    {
        Webrtc = "enable";
        Canvas = "enable";
        Audio = "enable";
        QUIC = "enable";
        UseWidevine = true;
        UseSafeBrowsing = true;
        UseComponents = true;
        Webgl = "enable";
        ProxyPort = 0;
        ProxyIsHttp = true;
        ProxyServer.clear();
        ProxyName.clear();
        ProxyPassword.clear();
        ProxyTarget.clear();
        Profile.clear();
        CurrentBrowserVersion = 0;
        TempProfile = QString("prof/") + GetRandomString();
        Extensions.clear();
        CommandLine.clear();
        ProxyForNextProfile = "block";
        Timezone = TIMEZONE_NONE;
        TimezoneName = "BAS_NOT_SET";

        //Settings.clear();
        Settings = SettingsPreserved;
    }

    void WorkerSettings::SetBrowserVersionSelector(IBrowserVersionSelector * _BrowserVersionSelector)
    {
        this->_BrowserVersionSelector = _BrowserVersionSelector;
    }


    QString WorkerSettings::Get(const QString& Key)
    {
        if(!Settings.contains(Key))
            return QString();
        return Settings[Key];
    }

    void WorkerSettings::Set(const QString& Key,const QString& Value)
    {
        Settings[Key] = Value;
    }

    QStringList WorkerSettings::Keys()
    {
        return Settings.keys();
    }

    void WorkerSettings::SetTimezone(int Timezone)
    {
        this->Timezone = Timezone;
    }

    int WorkerSettings::GetTimezone()
    {
        return Timezone;
    }

    void WorkerSettings::SetTimezoneName(const QString& TimezoneName)
    {
        this->TimezoneName = TimezoneName;
    }

    QString WorkerSettings::GetTimezoneName()
    {
        return TimezoneName;
    }

    QString WorkerSettings::GetProxyServer()
    {
        return ProxyServer;
    }
    void WorkerSettings::SetProxyServer(const QString& ProxyServer)
    {
        this->ProxyServer = ProxyServer;
    }

    int WorkerSettings::GetProxyPort()
    {
        return ProxyPort;
    }

    void WorkerSettings::SetProxyPort(int ProxyPort)
    {
        this->ProxyPort = ProxyPort;
    }

    int WorkerSettings::GetPcapPort()
    {
        return PcapPort;
    }

    void WorkerSettings::SetPcapPort(int PcapPort)
    {
        this->PcapPort = PcapPort;
    }

    bool WorkerSettings::GetProxyIsHttp()
    {
        return ProxyIsHttp;
    }
    void WorkerSettings::SetProxyIsHttp(bool ProxyIsHttp)
    {
        this->ProxyIsHttp = ProxyIsHttp;
    }

    QString WorkerSettings::GetProxyName()
    {
        return ProxyName;
    }
    void WorkerSettings::SetProxyName(const QString& ProxyName)
    {
        this->ProxyName = ProxyName;
    }

    QString WorkerSettings::GetProxyPassword()
    {
        return ProxyPassword;
    }

    void WorkerSettings::SetProxyPassword(const QString& ProxyPassword)
    {
        this->ProxyPassword = ProxyPassword;
    }

    QString WorkerSettings::GetProxyTarget()
    {
        return ProxyTarget;
    }

    void WorkerSettings::SetProxyTarget(const QString& ProxyTarget)
    {
        this->ProxyTarget = ProxyTarget;
    }

    void WorkerSettings::SetBrowserEngine(const QString& BrowserEngine)
    {
        this->BrowserEngine = BrowserEngine;
    }

    QString WorkerSettings::GetBrowserEngine()
    {
        return BrowserEngine;
    }

    QString WorkerSettings::GetBrowserEngineVirtual()
    {
        return BrowserEngineVirtual;
    }
    void WorkerSettings::SetBrowserEngineVirtual(const QString& BrowserEngine)
    {
        this->BrowserEngineVirtual = BrowserEngine;
    }

    void WorkerSettings::SetWorkerPathSafe(const QString& PathSafe)
    {
        this->PathSafe = PathSafe;
    }
    void WorkerSettings::SetWorkerPathNotSafe(const QString& PathNotSafe)
    {
        this->PathNotSafe = PathNotSafe;
    }
    void WorkerSettings::SetProfile(const QString& Profile)
    {
        this->Profile = Profile;
    }
    void WorkerSettings::SetExtensions(const QString& Extensions)
    {
        this->Extensions = Extensions;
    }
    void WorkerSettings::SetCommandLine(const QString& CommandLine)
    {
        this->CommandLine = CommandLine;
    }
    void WorkerSettings::SetProxyForNextProfile(const QString& ProxyForNextProfile)
    {
        this->ProxyForNextProfile = ProxyForNextProfile;
    }
    void WorkerSettings::SetUseWidevine(bool UseWidevine)
    {
        this->UseWidevine = UseWidevine;
    }
    void WorkerSettings::SetUseSafeBrowsing(bool UseSafeBrowsing)
    {
        this->UseSafeBrowsing = UseSafeBrowsing;
    }

    void WorkerSettings::SetUseComponents(bool UseComponents)
    {
        this->UseComponents = UseComponents;
    }

    QString WorkerSettings::GetWebrtc()
    {
        return Webrtc;
    }
    QString WorkerSettings::GetWebrtcIps()
    {
        return WebrtcIps;
    }
    QString WorkerSettings::GetCanvas()
    {
        return Canvas;
    }
    QString WorkerSettings::GetCanvasNoise()
    {
        return CanvasNoise;
    }
    QString WorkerSettings::GetAudio()
    {
        return Audio;
    }
    QString WorkerSettings::GetAudioNoise()
    {
        return AudioNoise;
    }
    QString WorkerSettings::GetQUIC()
    {
        return QUIC;
    }
    int WorkerSettings::GetMaxFPS()
    {
        return MaxFPS;
    }
    void WorkerSettings::SetAudio(const QString& Audio)
    {
        this->Audio = Audio;
    }
    void WorkerSettings::SetQUIC(const QString& QUIC)
    {
        this->QUIC = QUIC;
    }
    void WorkerSettings::SetAudioNoise(const QString& AudioNoise)
    {
        this->AudioNoise = AudioNoise;
    }
    void WorkerSettings::SetMaxFPS(int MaxFPS)
    {
        if(MaxFPS < 10)
            MaxFPS = 10;
        this->MaxFPS = MaxFPS;
    }

    QString WorkerSettings::GetWebgl()
    {
        return Webgl;
    }
    QString WorkerSettings::GetWebglNoise()
    {
        return WebglNoise;
    }
    void WorkerSettings::SetWebrtc(const QString& Webrtc)
    {
        this->Webrtc = Webrtc;
    }
    void WorkerSettings::SetWebrtcIps(const QString& WebrtcIps)
    {
        this->WebrtcIps = WebrtcIps;
    }
    void WorkerSettings::SetCanvas(const QString& Canvas)
    {
        this->Canvas = Canvas;
    }
    void WorkerSettings::SetCanvasNoise(const QString& CanvasNoise)
    {
        this->CanvasNoise = CanvasNoise;
    }
    void WorkerSettings::SetWebgl(const QString& Webgl)
    {
        this->Webgl = Webgl;
    }
    void WorkerSettings::SetWebglNoise(const QString& WebglNoise)
    {
        this->WebglNoise = WebglNoise;
    }

    QString WorkerSettings::GetWorkerPathSafe()
    {
        return PathSafe;
    }
    QString WorkerSettings::GetWorkerPathNotSafe()
    {
        return PathNotSafe;
    }
    QString WorkerSettings::GetProfile()
    {
        return Profile;
    }
    QString WorkerSettings::GetRealProfile()
    {
        if(!Profile.isEmpty())
            return Profile;
        return QFileInfo(TempProfile).absoluteFilePath();
    }
    bool WorkerSettings::IsTemporaryProfile()
    {
        return Profile.isEmpty();
    }
    QString WorkerSettings::GetWorkerPath()
    {
        IBrowserVersionSelector::BrowserItem Item = _BrowserVersionSelector->GetBrowserItemById(CurrentBrowserVersion);
        if(!Item.IsNull)
        {
            return QString("./") + Item.WorkerExeRelativePath;
        }

        return PathSafe;
    }

    QString WorkerSettings::GetAllBrowserVersionInfo(const QString& Format)
    {
        bool IsExtended = Format.toLower() == "extended";

        QJsonArray ResultArray;

        for(const IBrowserVersionSelector::BrowserItem& Item: _BrowserVersionSelector->GetAllBrowserItems())
        {
            if(IsExtended)
            {
                QJsonObject ResultItem;
                ResultItem["architecture"] = Item.Architecture;
                ResultItem["description"] = Item.Description;
                ResultItem["bas_version"] = Item.BasVersion;
                ResultItem["browser_type"] = Item.BrowserType;
                ResultItem["browser_version"] = Item.VersionFull;
                ResultItem["id"] = Item.Id;
                ResultItem["is_null"] = Item.IsNull;
                ResultArray.append(ResultItem);
            }else
            {
                ResultArray.append(Item.VersionFull);
            }
        }

        QJsonDocument Document;
        Document.setArray(ResultArray);
        return Document.toJson(QJsonDocument::Compact);
    }

    QString WorkerSettings::GetCurrentBrowserVersionInfo(const QString& Format)
    {
        bool IsExtended = Format.toLower() == "extended";

        IBrowserVersionSelector::BrowserItem Item = _BrowserVersionSelector->GetBrowserItemById(CurrentBrowserVersion);

        if(IsExtended)
        {
            QJsonObject ResultItem;

            ResultItem["architecture"] = Item.Architecture;
            ResultItem["description"] = Item.Description;
            ResultItem["bas_version"] = Item.BasVersion;
            ResultItem["browser_type"] = Item.BrowserType;
            ResultItem["browser_version"] = Item.VersionFull;
            ResultItem["id"] = Item.Id;
            ResultItem["is_null"] = Item.IsNull;

            QJsonDocument Document;
            Document.setObject(ResultItem);
            return Document.toJson(QJsonDocument::Compact);
        }else
        {
            QJsonDocument Document;
            QJsonArray ResultArray;
            ResultArray.append(Item.VersionFull);

            Document.setArray(ResultArray);
            QString Result = Document.toJson(QJsonDocument::Compact);

            Result = Result.replace("[", "");
            Result = Result.replace("]", "");

            return Result;
        }

    }

    int WorkerSettings::FindBrowserVersionId(const QString& VersionString)
    {
        IBrowserVersionSelector::BrowserItem Item = _BrowserVersionSelector->GetBrowserItemByVersion(VersionString);

        if(Item.IsNull)
        {
            return -1;
        }

        return Item.Id;
    }

    QString WorkerSettings::GetExtensions()
    {
        return Extensions;
    }
    QString WorkerSettings::GetCommandLineAdditional()
    {
        return CommandLine;
    }
    QString WorkerSettings::GetProxyForNextProfile()
    {
        return ProxyForNextProfile;
    }
    bool WorkerSettings::GetUseWidevine()
    {
        return UseWidevine;
    }
    bool WorkerSettings::GetUseSafeBrowsing()
    {
        return UseSafeBrowsing;
    }
    bool WorkerSettings::GetUseComponents()
    {
        return UseComponents;
    }

    IWorkerSettings* WorkerSettings::Clone()
    {
        WorkerSettings * res = new WorkerSettings();
        res->SetWorkerPathSafe(PathSafe);
        res->SetWorkerPathNotSafe(PathNotSafe);
        res->SetUseWidevine(UseWidevine);
        res->SetUseSafeBrowsing(UseSafeBrowsing);
        res->SetUseComponents(UseComponents);
        res->SetProfile(Profile);
        res->CurrentBrowserVersion = CurrentBrowserVersion;
        res->SetExtensions(Extensions);
        res->SetCommandLine(CommandLine);
        res->SetProxyForNextProfile(ProxyForNextProfile);
        res->SetBrowserEngine(BrowserEngine);
        res->SetProxyServer(ProxyServer);
        res->SetProxyPort(ProxyPort);
        res->SetProxyIsHttp(ProxyIsHttp);
        res->SetProxyName(ProxyName);
        res->SetProxyPassword(ProxyPassword);

        res->SetWebrtc(Webrtc);
        res->SetWebrtcIps(WebrtcIps);
        res->SetTimezone(Timezone);
        res->SetTimezoneName(TimezoneName);
        res->SetCanvas(Canvas);
        res->SetCanvasNoise(CanvasNoise);
        res->SetAudio(Audio);
        res->SetQUIC(QUIC);
        res->SetAudioNoise(AudioNoise);
        res->SetMaxFPS(MaxFPS);
        res->SetWebgl(Webgl);
        res->SetWebglNoise(WebglNoise);

        for(QString& key: Keys())
        {
            res->Set(key,Get(key));
        }
        return res;
    }

    void WorkerSettings::ParseFromSettings(QSettings& Settings)
    {
        if(Settings.contains("IsSafe"))
        {
            if(Settings.value("IsSafe",true).toBool())
            {
                SetWebrtc("disable");
                SetCanvas("disable");
                SetAudio("disable");
            }else
            {
                SetWebrtc("enable");
                SetCanvas("enable");
                SetAudio("enable");
            }
        }

        if(Settings.contains("DisableWebgl"))
        {
            if(Settings.value("DisableWebgl",true).toBool())
            {
                SetWebgl("disable");
            }else
            {
                SetWebgl("enable");
            }
        }

        if(Settings.contains("Webrtc"))
            SetWebrtc(Settings.value("Webrtc","disable").toString());

        if(Settings.contains("Canvas"))
            SetCanvas(Settings.value("Canvas","disable").toString());

        if(Settings.contains("CanvasNoise"))
            SetCanvasNoise(Settings.value("CanvasNoise","").toString());

        if(Settings.contains("Audio"))
            SetAudio(Settings.value("Audio","disable").toString());

        if(Settings.contains("AudioNoise"))
            SetAudioNoise(Settings.value("AudioNoise","").toString());

        if(Settings.contains("QUIC"))
            SetQUIC(Settings.value("QUIC","disable").toString());

        if(Settings.contains("MaxFPS"))
        {
            SetMaxFPS(Settings.value("MaxFPS","30").toInt());
        }

        if(Settings.contains("WebrtcIps"))
            SetWebrtcIps(Settings.value("WebrtcIps","").toString());

        if(Settings.contains("Webgl"))
            SetWebgl(Settings.value("Webgl","disable").toString());

        if(Settings.contains("WebglNoise"))
            SetWebglNoise(Settings.value("WebglNoise","").toString());

        if(Settings.contains("WebglVendor"))
        {
            QString Value = Settings.value("WebglVendor","").toString();
            if(!Value.isEmpty())
                Set("Webgl.unmaskedVendor", Value);
        }

        if(Settings.contains("WebglRenderer"))
        {
            QString Value = Settings.value("WebglRenderer","").toString();
            if(!Value.isEmpty())
                Set("Webgl.unmaskedRenderer", Value);
        }

        if(Settings.contains("EnableWidevine"))
            SetUseWidevine(Settings.value("EnableWidevine",true).toBool());

        if(Settings.contains("EnableSafeBrowsing"))
            SetUseSafeBrowsing(Settings.value("EnableSafeBrowsing",true).toBool());

        if(Settings.contains("EnableComponents"))
            SetUseComponents(Settings.value("EnableComponents",true).toBool());
    }

    void WorkerSettings::SetSettingWhichRestartsBrowser(const QString& Key, QJsonObject& Object, bool& NeedRestart, bool& NeedSend)
    {
        if(Object.contains(Key))
        {
            QString prev = Get(Key);
            QString next = Object[Key].toString();
            if(prev != next)
            {
                Set(Key, next);
                NeedRestart = true;
            }
        }
    }

    void WorkerSettings::SetSettingWhichRestartsVirtualBrowser(const QString& Key, QJsonObject& Object, bool& NeedToRestartVirtual, bool& NeedToCloseVirtual)
    {
        if(Object.contains(Key))
        {
            QString prev = Get(Key);
            QString next = Object[Key].toString();
            if(prev != next)
            {
                Set(Key, next);
                NeedToRestartVirtual = true;
            }
        }
    }

    void WorkerSettings::PreserveDefaultMultiloginSettings(const QJsonObject& Object)
    {
        SettingsPreserved.clear();
        for(QString &Key:Object.keys())
        {
            QString Prefix("multilogin.fingerprintGeneration.");
            if(Key.startsWith(Prefix))
            {
                SettingsPreserved[Key] = Object[Key].toString();
            }
        }
    }


    void WorkerSettings::Change(const QString& JsonString, bool& NeedRestart,bool& NeedSend,bool &NeedToRestartVirtual,bool &NeedToCloseVirtual, bool IsRecord)
    {
        NeedRestart = false;
        NeedSend = false;
        NeedToRestartVirtual = false;
        NeedToCloseVirtual = false;

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(JsonString.toUtf8(), &err);

        if(err.error != QJsonParseError::NoError)
        {
            return;
        }

         QJsonObject object = doc.object();

         if(object.contains("BrowserEngine"))
         {
            QString next = object["BrowserEngine"].toString();

            if(IsRecord)
            {
                QString prev = GetBrowserEngineVirtual();
                if(prev != next)
                {

                    if(!prev.startsWith("BAS") && next.startsWith("BAS"))
                    {
                        //returning to BAS engine in record mode - need to close virtual browser
                        NeedToCloseVirtual = true;
                    }

                    //record mode - nofify script editor about new engine
                    NeedSend = true;
                }
            }else
            {
                QString prev = GetBrowserEngine();
                if(prev != next)
                {
                    //Not a record mode - restart browser
                    NeedRestart = true;
                    SetBrowserEngine(next);
                }
            }

            if(next.startsWith("MLA"))
            {
                PreserveDefaultMultiloginSettings(object);
            }

            SetBrowserEngineVirtual(next);
         }

         bool IsMLAReal = !GetBrowserEngine().startsWith("BAS");
         bool IsMLAVirtual = !GetBrowserEngineVirtual().startsWith("BAS");
         bool IsMLA = IsMLAReal || IsMLAVirtual;

         if(object.contains("server"))
         {
            QString prev = GetProxyServer();
            QString next = object["server"].toString();
            if(prev.isEmpty() && next.isEmpty())
            {
                //Skip if both proxies are empty, this could happen during startup
            }else
            {
                NeedSend = true;
                SetProxyServer(next);
            }
         }

         if(object.contains("Port"))
         {
            int prev = GetProxyPort();
            int next = object["Port"].toInt();
            if(prev == 0 && next == 0)
            {
                //Skip if both proxies are empty, this could happen during startup
            }else
            {
                NeedSend = true;
                SetProxyPort(next);
            }
         }


         if(object.contains("IsHttp"))
         {
            bool prev = GetProxyIsHttp();
            bool next = object["IsHttp"].toBool();
            if(prev == true && next == true)
            {
                //Skip if both proxies are empty, this could happen during startup
            }else
            {
                NeedSend = true;
                SetProxyIsHttp(next);
            }
         }


         if(object.contains("name"))
         {
            QString prev = GetProxyName();
            QString next = object["name"].toString();
            if(prev.isEmpty() && next.isEmpty())
            {
                //Skip if both proxies are empty, this could happen during startup
            }else
            {
                NeedSend = true;
                SetProxyName(next);
            }
         }


         if(object.contains("password"))
         {
            QString prev = GetProxyPassword();
            QString next = object["password"].toString();
            if(prev.isEmpty() && next.isEmpty())
            {
                //Skip if both proxies are empty, this could happen during startup
            }else
            {
                NeedSend = true;
                SetProxyPassword(next);
            }
         }


         if(object.contains("target"))
         {
            QString prev = GetProxyTarget();
            QString next = object["target"].toString();
            if(prev.isEmpty() && next.isEmpty())
            {
                //Skip if both proxies are empty, this could happen during startup
            }else
            {
                NeedSend = true;
                SetProxyTarget(next);
            }
         }

         if(!IsMLA && object.contains("IsSafe"))
         {
            if(object["IsSafe"].toBool())
            {
                SetCanvas("disable");
                SetWebrtc("disable");
                SetAudio("disable");
            }else
            {
                SetCanvas("enable");
                SetWebrtc("enable");
                SetAudio("enable");
            }
            UpdateFingerprintsSettings();
         }


         if(!IsMLA && object.contains("UseWidevine"))
         {
            bool prev = GetUseWidevine();
            bool next = object["UseWidevine"].toBool();
            if(prev != next)
            {
                NeedRestart = true;
                SetUseWidevine(next);
            }
         }

         if(!IsMLA && object.contains("UseSafeBrowsing"))
         {
            bool prev = GetUseSafeBrowsing();
            bool next = object["UseSafeBrowsing"].toBool();
            if(prev != next)
            {
                NeedRestart = true;
                SetUseSafeBrowsing(next);
            }
         }

         if(!IsMLA && object.contains("UseComponents"))
         {
            bool prev = GetUseComponents();
            bool next = object["UseComponents"].toBool();
            if(prev != next)
            {
                NeedRestart = true;
                SetUseComponents(next);
            }
         }

         if(!IsMLA && object.contains("Webrtc"))
         {
            SetWebrtc(object["Webrtc"].toString());
            UpdateFingerprintsSettings();
         }

         if(!IsMLA && object.contains("Canvas"))
         {
            SetCanvas(object["Canvas"].toString());
            UpdateFingerprintsSettings();
         }

         if(!IsMLA && object.contains("Audio"))
         {
            SetAudio(object["Audio"].toString());
            UpdateFingerprintsSettings();
         }

         if(!IsMLA && object.contains("QUIC"))
         {
            SetQUIC(object["QUIC"].toString());
            UpdateFingerprintsSettings();
         }

         if(!IsMLA && object.contains("CanvasNoise"))
         {
            SetCanvasNoise(object["CanvasNoise"].toString());
            UpdateFingerprintsSettings();
         }

         if(!IsMLA && object.contains("AudioNoise"))
         {
            SetAudioNoise(object["AudioNoise"].toString());
            UpdateFingerprintsSettings();
         }

         if(!IsMLA && object.contains("MaxFPS"))
         {
            SetMaxFPS(object["MaxFPS"].toInt());
            UpdateFingerprintsSettings();
         }

         if(!IsMLA && object.contains("WebrtcIps"))
         {
            SetWebrtcIps(object["WebrtcIps"].toString());
            UpdateFingerprintsSettings();
         }

         if(!IsMLA && object.contains("Timezone"))
         {
            SetTimezone(object["Timezone"].toString().toInt());
            UpdateFingerprintsSettings();
         }

         if(!IsMLA && object.contains("TimezoneName"))
         {
            SetTimezoneName(object["TimezoneName"].toString());
            UpdateFingerprintsSettings();
         }

         if(!IsMLA && object.contains("Webgl"))
         {
            SetWebgl(object["Webgl"].toString());
            UpdateFingerprintsSettings();
         }

         if(!IsMLA && object.contains("WebglNoise"))
         {
            SetWebglNoise(object["WebglNoise"].toString());
            UpdateFingerprintsSettings();
         }

         bool UpdatedWebglSettings = false;
         for(QString &Key:object.keys())
         {
             QString Prefix("Webgl.");
             if(Key.startsWith(Prefix))
             {
                 Set(Key,object[Key].toString());
                 UpdatedWebglSettings = true;
             }
         }
         if(UpdatedWebglSettings)
         {
            UpdateFingerprintsSettings();
         }

         bool UpdatedAttributes = false;
         for(QString &Key:object.keys())
         {
             QString Prefix("Attribute.");
             if(Key.startsWith(Prefix))
             {
                 Set(Key,object[Key].toString());
                 UpdatedAttributes = true;
             }
         }
         bool UpdatedFingerprints = false;
         for(QString &Key:object.keys())
         {
             QString Prefix("Fingerprints.");
             if(Key.startsWith(Prefix))
             {
                 Set(Key,object[Key].toString());
                 UpdatedFingerprints = true;
             }
         }
         if(UpdatedAttributes || UpdatedFingerprints)
         {
            UpdateFingerprintsSettings();
         }

         if(object.contains("ProfilePath"))
         {
            QString prev = GetProfile();
            QString next = object["ProfilePath"].toString();
            bool IsNewTempProfile = false;
            if(next == QString("<Incognito>"))
            {
                next.clear();
                TempProfile = QString("prof/") + GetRandomString();
                IsNewTempProfile = true;
            }

            if(prev != next || IsNewTempProfile || (prev.isEmpty() && next.isEmpty() && IsMLA))
            {
                if(IsMLAReal)
                {
                    NeedRestart = true;
                }else if(IsMLAVirtual)
                {
                    NeedToRestartVirtual = true;
                }else
                {
                    NeedRestart = true;
                }
                SetProfile(next);
            }

         }

         if(object.contains("BrowserVersionId"))
         {
            int prev = CurrentBrowserVersion;
            int next = object["BrowserVersionId"].toString().toInt();

            bool PrevIsNull = false;
            bool NextIsNull = false;

            {
                IBrowserVersionSelector::BrowserItem Item = _BrowserVersionSelector->GetBrowserItemById(prev);
                PrevIsNull = Item.IsNull;
                prev = Item.Id;
            }

            {
                IBrowserVersionSelector::BrowserItem Item = _BrowserVersionSelector->GetBrowserItemById(next);
                NextIsNull = Item.IsNull;
                next = Item.Id;
            }

            if(!PrevIsNull && !NextIsNull && prev != next)
            {
                NeedRestart = true;
                CurrentBrowserVersion = next;
            }

         }

         if(object.contains("Extensions"))
         {
            QString prev = GetExtensions();
            QString next = object["Extensions"].toString();

            if(prev != next)
            {
                NeedRestart = true;
                SetExtensions(next);
            }

         }

         if(object.contains("CommandLine"))
         {
            QString prev = GetCommandLineAdditional();
            QString next = object["CommandLine"].toString();

            if(prev != next)
            {
                NeedRestart = true;
                SetCommandLine(next);
            }

         }

         if(object.contains("ProxyForNextProfile"))
         {
            QString prev = GetProxyForNextProfile();
            QString next = object["ProxyForNextProfile"].toString();

            if(prev != next)
            {
                SetProxyForNextProfile(next);
            }

         }

         if(IsMLA && object.contains("LoadFingerprintFromProfileFolder"))
         {
            QString prev = Get("LoadFingerprintFromProfileFolder");
            QString next = object["LoadFingerprintFromProfileFolder"].toString();
            if(prev != next)
            {
                Set("LoadFingerprintFromProfileFolder",next);
            }
         }

         if(IsMLAReal)
         {
             for(QString &Key:object.keys())
             {
                 QString Prefix("multilogin.fingerprintGeneration.");
                 if(Key.startsWith(Prefix))
                 {
                     SetSettingWhichRestartsBrowser(Key, object, NeedRestart, NeedSend);
                 }
             }
         }else if(IsMLAVirtual)
         {
             for(QString &Key:object.keys())
             {
                 QString Prefix("multilogin.fingerprintGeneration.");
                 if(Key.startsWith(Prefix))
                 {
                     SetSettingWhichRestartsVirtualBrowser(Key, object, NeedToRestartVirtual, NeedToCloseVirtual);
                 }
             }
         }
    }

    QString WorkerSettings::GetRandomString()
    {
       const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
       const int randomStringLength = 8;

       QString randomString;
       for(int i=0; i<randomStringLength; ++i)
       {
           int index = qrand() % possibleCharacters.length();
           QChar nextChar = possibleCharacters.at(index);
           randomString.append(nextChar);
       }
       return randomString;
    }

    void WorkerSettings::UpdateFingerprintsSettings()
    {
        RemoveOldFingerprintSettings();

        QString Text;

        if(Webrtc == "enable")
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += "WebrtcType=Enable";
        }

        if(Webrtc == "disable")
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += "WebrtcType=Disable";
        }

        if(Webrtc == "replace")
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += "WebrtcType=Replace";
        }


        if(!WebrtcIps.isEmpty())
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += QString("WebrtcIps=") + WebrtcIps;
        }


        if(Timezone >= TIMEZONE_NONE || Timezone <= -TIMEZONE_NONE)
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += QString("TimezoneReplace=Disable");
        }else
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += QString("TimezoneReplace=Enable");
            Text += "\r\n";
            Text += QString("TimezoneOffset=") + QString::number(Timezone);
        }


        if(TimezoneName != "BAS_NOT_SET")
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += QString("TimezoneName=") + TimezoneName;

            if(!Text.isEmpty())
                Text += "\r\n";
            std::hash<std::string> Hasher;
            unsigned short int hash = Hasher(TimezoneName.toStdString());
            Text += QString("TimezoneHash=") + QString::number(hash);
        }




        if(Canvas == "enable")
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += "CanvasType=Enable";
        }

        if(Canvas == "disable")
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += "CanvasType=Disable";
        }

        if(Canvas == "noise")
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += "CanvasType=Noise";
        }


        if(Audio == "enable")
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += "AudioType=Enable";
        }

        if(Audio == "disable")
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += "AudioType=Disable";
        }

        if(Audio == "noise")
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += "AudioType=Noise";
        }

        if(QUIC == "enable")
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += "QUIC=Enable";
        }

        if(QUIC == "disable")
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += "QUIC=Disable";
        }

        if(Webgl == "enable")
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += "WebglType=Enable";
        }

        if(Webgl == "disable")
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += "WebglType=Disable";
        }

        if(Webgl == "noise")
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += "WebglType=Noise";
        }

        if(!CanvasNoise.isEmpty())
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += QString("CanvasFingerprint=") + CanvasNoise;
        }

        if(!AudioNoise.isEmpty())
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += QString("AudioFingerprint=") + AudioNoise;
        }

        if(MaxFPS > 0)
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += QString("MaxFPS=") + QString::number(MaxFPS);
        }

        if(!WebglNoise.isEmpty())
        {
            if(!Text.isEmpty())
                Text += "\r\n";
            Text += QString("WebglFingerprint=") + WebglNoise;
        }

        QStringList keys = Keys();

        for(const QString& Key: keys)
        {
            if(Key.startsWith("Webgl."))
            {
                if(!Text.isEmpty())
                    Text += "\r\n";
                Text += Key + QString("=") + Get(Key);
            }
        }

        for(const QString& Key: keys)
        {
            if(Key.startsWith("Attribute."))
            {
                if(!Text.isEmpty())
                    Text += "\r\n";
                Text += Key + QString("=") + Get(Key);
            }
        }

        QString ActualProfile = GetProfile();
        if(ActualProfile.isEmpty())
        {
            ActualProfile = TempProfile;
        }
        if(!QFileInfo(ActualProfile).isAbsolute())
        {
            QDir HomeDir(".");
            ActualProfile = HomeDir.absoluteFilePath(ActualProfile);
        }
        ActualProfile += QString("/data.notification");
        ActualProfile = QDir::cleanPath(ActualProfile);
        if(!Text.isEmpty())
            Text += "\r\n";

        Text += "NotificationData" + QString("=") + QString::fromUtf8(ActualProfile.toUtf8().toBase64());


        //Check if fingerprint uses canvas replacement
        bool ReplaceCanvas = keys.contains("Fingerprints.PerfectCanvasDoReplace") && Get("Fingerprints.PerfectCanvasDoReplace") == "Enable";

        for(const QString& Key: keys)
        {
            if(Key.startsWith("Fingerprints."))
            {
                if(Key.startsWith("Fingerprints.PerfectCanvasReplace.") && !ReplaceCanvas)
                {
                    //Disable PerfectCanvas data if no need to replace it
                    continue;
                }
                if(!Text.isEmpty())
                    Text += "\r\n";
                QString KeyUpdated = Key;
                KeyUpdated = KeyUpdated.remove("Fingerprints.");
                Text += KeyUpdated + QString("=") + Get(Key);
            }
        }

        QFile file(QString("s/") + UniqueProcessId + QString("1.ini"));
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(Text.toUtf8());
        }
        file.close();

    }

    void WorkerSettings::RemoveOldFingerprintSettings()
    {
        QDirIterator it("s", QDir::Files | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
        while (it.hasNext())
        {
            QString FilePath = it.next();
            if(FilePath.toLower().endsWith(".lock"))
            {
                QFile(FilePath).remove();

                if(!QFileInfo(FilePath).exists())
                {
                    FilePath.remove(FilePath.size()-5,5);
                    QFile(FilePath + ".txt").remove();
                    QFile(FilePath + ".fonts.txt").remove();
                    QFile(FilePath + ".ini").remove();
                    QFile(FilePath + "1.ini").remove();
                    QFile(FilePath + ".detect").remove();
                    QFile(FilePath + ".notification").remove();
                }

            }
        }
    }

    QStringList WorkerSettings::GetCommandLineParameters(const QString &Language, bool IsVirtual)
    {
        QString Engine;
        if(IsVirtual)
            Engine = GetBrowserEngineVirtual();
        else
            Engine = GetBrowserEngine();
        QStringList res;
        if(Engine == QString("BASChrome"))
        {
            res.append(Language);

            res.append("--UseWidevine");
            res.append(QString::number(GetUseWidevine()));

            res.append("--UseSafeBrowsing");
            res.append(QString::number(GetUseSafeBrowsing()));

            res.append("--UseComponents");
            res.append(QString::number(GetUseComponents()));

            UniqueProcessId = GetRandomString();
            UpdateFingerprintsSettings();
            res.append(QString("--unique-process-id=") + UniqueProcessId);

            {
                DeviceScaleManager Scale;
                QString ScaleFactorParam = QString::number((int)(Scale.GetScaleFactor() * 100.0));
                res.append(QString("--interface-scale-factor=") + ScaleFactorParam);
            }

            res.append("--Profile");
            QString ActualProfile = GetProfile();
            if(ActualProfile.isEmpty())
            {
                ActualProfile = TempProfile;
            }
            res.append(ActualProfile);

            res.append("--Extensions");
            res.append(GetExtensions().split(QRegExp("[\r\n]"),QString::SkipEmptyParts).join(";"));

            res.append("--CommandLine");
            res.append(GetCommandLineAdditional().split(QRegExp("[\r\n]"),QString::SkipEmptyParts).join(";"));

            res.append(QString("--PcapPort=") + QString::number(PcapPort));

            res.append("--InitialProxy");
            res.append(GetProxyForNextProfile());
            SetProxyForNextProfile("block");
        }else if(Engine.startsWith(QString("WebDriver")))
        {
            //res.append("--headless");
            if(!GetProxyServer().isEmpty())
            {
                QString ProxyServerString = "--proxy-server=";
                if(GetProxyIsHttp())
                {
                    ProxyServerString += "http://";
                }else
                {
                    ProxyServerString += "socks5://";
                }
                if(!GetProxyName().isEmpty() && !GetProxyPassword().isEmpty())
                {
                    ProxyServerString += GetProxyName();
                    ProxyServerString += ":";
                    ProxyServerString += GetProxyPassword();
                    ProxyServerString += "@";
                }
                ProxyServerString += GetProxyServer();
                ProxyServerString += ":";
                ProxyServerString += QString::number(GetProxyPort());
                res.append(ProxyServerString);
            }

        }else if(Engine.startsWith(QString("MLA")))
        {
            if(!GetProxyServer().isEmpty())
            {
                if(GetProxyIsHttp())
                {
                    res.append(QString("proxyType=\"http\""));
                }else
                {
                    res.append(QString("proxyType=\"socks5\""));
                }
                if(!GetProxyName().isEmpty() && !GetProxyPassword().isEmpty())
                {
                    res.append(QString("proxyUser=\"") + GetProxyName() + QString("\""));
                    res.append(QString("proxyPass=\"") + GetProxyPassword() + QString("\""));
                }
                res.append(QString("proxyHost=\"") + GetProxyServer() + QString("\""));
                res.append(QString("proxyPort=") + QString::number(GetProxyPort()));
            }else
            {
                res.append(QString("proxyType=\"none\""));
            }
            res.append("Profile=" + GetProfile());
            if(Get("LoadFingerprintFromProfileFolder") == QString("true"))
                res.append("LoadFingerprintFromProfileFolder=true");

            for(QString &Key:Keys())
            {
                QString Prefix("multilogin.fingerprintGeneration.");
                if(Key.startsWith(Prefix))
                {
                    QString KeyCopy = Key;
                    MLAAddCommandLineSettings(res, KeyCopy.mid(Prefix.length()),Prefix);
                }
            }
        }
        return res;
    }

    void WorkerSettings::MLAAddCommandLineSettings(QStringList& Arguments,const QString& SettingKey,const QString& Prefix)
    {
        if(Keys().contains(Prefix + SettingKey))
        {
            Arguments.append(SettingKey + QString("=") + Get(Prefix + SettingKey));
        }
    }

}
