#ifndef IBROWSERVERSIONSELECTOR_H
#define IBROWSERVERSIONSELECTOR_H

#include "engine_global.h"
#include <QObject>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT IBrowserVersionSelector : public QObject
    {
        Q_OBJECT

        public:

        struct BrowserItem
        {
            int Id = 0;
            QString VersionFull;
            int VersionMajorNumber = 0;
            QString VersionMajorString;
            QString WorkerFolderName;
            QString WorkerExeRelativePath;
            QString WorkerExeFullPath;

            QString BasVersion;
            QString BrowserType;
            QString Description;
            QString Architecture;
            bool IsNull = true;
        };
    
        explicit IBrowserVersionSelector(QObject *parent = 0);
    
        virtual QList<BrowserItem> GetAllBrowserItems() = 0;
        virtual BrowserItem GetDefaultBrowserItem() = 0;

        virtual QStringList GetAllBrowserFullVersions() = 0;

        virtual BrowserItem GetBrowserItemById(int Id) = 0;
        virtual BrowserItem GetBrowserItemByVersion(const QString& Version) = 0;
    };
}


#endif // IBROWSERVERSIONSELECTOR_H
