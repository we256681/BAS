#include "versioninfo.h"
#include "aboutbrowserstudio.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    VersionInfo::VersionInfo(QObject *parent) :
        IVersionInfo(parent)
    {
        IsPremium = false;
    }

    void VersionInfo::SetIsPremium()
    {
        IsPremium = true;
    }

    int VersionInfo::MajorVersion()
    {
        return 25;
    }

    int VersionInfo::MinorVersion()
    {
        return 8;
    }

    int VersionInfo::BuildVersion()
    {
        return 0;
    }

    QString VersionInfo::VersionString()
    {
        return QString("%1.%2.%3").arg(MajorVersion()).arg(MinorVersion()).arg(BuildVersion());
    }

    void VersionInfo::SetServerName(const QString& ServerName)
    {
        this->ServerName = ServerName;
    }

    void VersionInfo::ShowAboutWindow(const QStringList& BrowserVersionList)
    {

        AboutBrowserStudio about(VersionString(), BrowserVersionList, ServerName);
        if(IsPremium)
            about.SetIsPremium();
        about.exec();
    }
}
