#ifndef IWORKERSETTINGS_H
#define IWORKERSETTINGS_H

#include "engine_global.h"

#include "ibrowserversionselector.h"

#include <QObject>
#include <QSettings>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT IWorkerSettings : public QObject
    {
        Q_OBJECT
    public:
        explicit IWorkerSettings(QObject *parent = 0);

        virtual void SetBrowserVersionSelector(IBrowserVersionSelector * _BrowserVersionSelector) = 0;
        virtual QString GetAllBrowserVersionInfo(const QString& Format) = 0;
        virtual QString GetCurrentBrowserVersionInfo(const QString& Format) = 0;
        virtual int FindBrowserVersionId(const QString& VersionString) = 0;

        virtual void SetWorkerPathSafe(const QString& PathSafe) = 0;
        virtual void SetWorkerPathNotSafe(const QString& PathNotSafe) = 0;
        virtual void SetUseWidevine(bool UseWidevine) = 0;
        virtual void SetUseSafeBrowsing(bool UseSafeBrowsing) = 0;
        virtual void SetUseComponents(bool UseComponents) = 0;
        virtual void SetProfile(const QString& Profile) = 0;
        virtual void SetExtensions(const QString& Extensions) = 0;
        virtual void SetCommandLine(const QString& CommandLine) = 0;
        virtual void SetProxyForNextProfile(const QString& ProxyForNextProfile) = 0;
        virtual void SetBrowserEngine(const QString& BrowserEngine) = 0;
        virtual void SetTimezone(int Timezone) = 0;
        virtual int GetTimezone() = 0;
        virtual void SetTimezoneName(const QString& TimezoneName) = 0;
        virtual QString GetTimezoneName() = 0;

        virtual QString Get(const QString& Key) = 0;
        virtual void Set(const QString& Key,const QString& Value) = 0;
        virtual QStringList Keys() = 0;

        virtual int GetPcapPort() = 0;
        virtual void SetPcapPort(int PcapPort) = 0;

        virtual QString GetWorkerPathSafe() = 0;
        virtual QString GetWorkerPathNotSafe() = 0;
        virtual QString GetWorkerPath() = 0;
        virtual bool GetUseWidevine() = 0;
        virtual bool GetUseSafeBrowsing() = 0;
        virtual bool GetUseComponents() = 0;
        virtual QString GetProfile() = 0;
        virtual QString GetRealProfile() = 0;
        virtual bool IsTemporaryProfile() = 0;
        virtual QString GetExtensions() = 0;
        virtual QString GetCommandLineAdditional() = 0;
        virtual QString GetProxyForNextProfile() = 0;
        virtual QString GetBrowserEngine() = 0;
        virtual QString GetBrowserEngineVirtual() = 0;
        virtual void SetBrowserEngineVirtual(const QString& BrowserEngine) = 0;

        virtual QString GetProxyServer() = 0;
        virtual void SetProxyServer(const QString& ProxyServer) = 0;

        virtual int GetProxyPort() = 0;
        virtual void SetProxyPort(int ProxyPort) = 0;

        virtual bool GetProxyIsHttp() = 0;
        virtual void SetProxyIsHttp(bool ProxyIsHttp) = 0;

        virtual QString GetProxyName() = 0;
        virtual void SetProxyName(const QString& ProxyName) = 0;

        virtual QString GetProxyPassword() = 0;
        virtual void SetProxyPassword(const QString& ProxyPassword) = 0;

        virtual QString GetProxyTarget() = 0;
        virtual void SetProxyTarget(const QString& ProxyTarget) = 0;

        virtual IWorkerSettings* Clone() = 0;
        virtual void ParseFromSettings(QSettings& Settings) = 0;
        virtual void Change(const QString& JsonString, bool& NeedRestart,bool& NeedSend, bool &NeedToRestartVirtual, bool &NeedToCloseVirtual, bool IsRecord) = 0;
        virtual QStringList GetCommandLineParameters(const QString &Language, bool IsVirtual) = 0;
        virtual void Clear() = 0;

        virtual QString GetWebrtc() = 0;
        virtual QString GetWebrtcIps() = 0;
        virtual QString GetCanvas() = 0;
        virtual QString GetCanvasNoise() = 0;
        virtual QString GetWebgl() = 0;
        virtual QString GetWebglNoise() = 0;
        virtual void SetWebrtc(const QString& Webrtc) = 0;
        virtual void SetWebrtcIps(const QString& WebrtcIps) = 0;
        virtual void SetCanvas(const QString& Canvas) = 0;
        virtual void SetCanvasNoise(const QString& CanvasNoise) = 0;

        virtual QString GetAudio() = 0;
        virtual QString GetAudioNoise() = 0;
        virtual QString GetQUIC() = 0;
        virtual int GetMaxFPS() = 0;
        virtual void SetMaxFPS(int MaxFPS) = 0;
        virtual void SetAudio(const QString& Audio) = 0;
        virtual void SetAudioNoise(const QString& AudioNoise) = 0;
        virtual void SetQUIC(const QString& QUIC) = 0;
        virtual void SetWebgl(const QString& Webgl) = 0;
        virtual void SetWebglNoise(const QString& WebglNoise) = 0;

        virtual QString GetUniqueProcessId() = 0;


    signals:

    public slots:

    };
}

#endif // IWORKERSETTINGS_H
