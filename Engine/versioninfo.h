#ifndef VERSIONINFO_H
#define VERSIONINFO_H
#include "engine_global.h"

#include <QObject>
#include "iversioninfo.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT VersionInfo : public IVersionInfo
    {
        Q_OBJECT
        QString ServerName;
        bool IsPremium;
    public:
        explicit VersionInfo(QObject *parent = 0);
        virtual int MajorVersion();
        virtual int MinorVersion();
        virtual int BuildVersion();
        virtual QString VersionString();
        virtual void ShowAboutWindow(const QStringList& BrowserVersionList);
        void SetServerName(const QString& ServerName);
        void SetIsPremium();


    signals:

    public slots:

    };
}

#endif // VERSIONINFO_H
