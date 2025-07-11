#ifndef BROWSERVERSIONSELECTOR_H
#define BROWSERVERSIONSELECTOR_H

#include "engine_global.h"

#include "ibrowserversionselector.h"

#include <QObject>
#include <QString>
#include <QList>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT BrowserVersionSelector : public IBrowserVersionSelector
    {
        Q_OBJECT


        QList<BrowserItem> CachedBrowserItems;
        bool IsCacheGenerated = false;

    public:

        explicit BrowserVersionSelector(QObject *parent = 0);

        QList<BrowserItem> GetAllBrowserItems();
        BrowserItem GetDefaultBrowserItem();

        QStringList GetAllBrowserFullVersions();

        BrowserItem GetBrowserItemById(int Id);
        BrowserItem GetBrowserItemByVersion(const QString& Version);

    };
}

#endif // BROWSERVERSIONSELECTOR_H
