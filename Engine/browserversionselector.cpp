#include "browserversionselector.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QFileInfo>

#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{

    BrowserVersionSelector::BrowserVersionSelector(QObject *parent) : IBrowserVersionSelector(parent)
    {
    }

    bool ItemMajorVersionLessThan(const IBrowserVersionSelector::BrowserItem &Item1, const BrowserVersionSelector::BrowserItem &Item2)
    {
        QStringList Item1Split = Item1.VersionFull.split(".");
        QStringList Item2Split = Item2.VersionFull.split(".");

        int MaxLen = qMax(Item1Split.length(), Item2Split.length());

        for(int i = 0;i < MaxLen;i++)
        {
            int Item1Part = 0;
            int Item2Part = 0;

            if(i < Item1Split.length())
            {
                Item1Part = Item1Split[i].toInt();
            }

            if(i < Item2Split.length())
            {
                Item2Part = Item2Split[i].toInt();
            }

            if(Item1Part < Item2Part)
                return false;

            if(Item1Part > Item2Part)
                return true;
        }

        return 0;
    }

    QList<IBrowserVersionSelector::BrowserItem> BrowserVersionSelector::GetAllBrowserItems()
    {
        //Use cache if possible
        if(IsCacheGenerated)
        {
            return CachedBrowserItems;
        }

        //Read file
        QByteArray BrowserVersionFileData;
        QFile File("browser_versions.json");
        if (File.open(QFile::ReadOnly | QFile::Text))
        {
            BrowserVersionFileData = File.readAll();
            File.close();
        }

        //Parse json browser_versions.json, save to ResultRaw

        QList<BrowserItem> ResultRaw;

        if(!BrowserVersionFileData.isEmpty())
        {
            QJsonParseError JsonParseError;
            QJsonDocument BrowserVersionDocument = QJsonDocument::fromJson(BrowserVersionFileData,&JsonParseError);
            if(!JsonParseError.error && BrowserVersionDocument.isArray())
            {
                QJsonArray BrowserVersionArray = BrowserVersionDocument.array();
                for(const QJsonValue& BrowserItemValue: BrowserVersionArray)
                {
                    if(BrowserItemValue.isObject())
                    {
                        QJsonObject BrowserItemObject = BrowserItemValue.toObject();

                        if(
                                BrowserItemObject.contains("id") && BrowserItemObject["id"].isDouble() &&
                                BrowserItemObject.contains("browser_version") && BrowserItemObject["browser_version"].isString()
                          )
                        {
                            BrowserItem Item;
                            Item.Id = BrowserItemObject["id"].toInt();
                            Item.VersionFull = BrowserItemObject["browser_version"].toString();

                            QStringList VersionSplit = Item.VersionFull.split(".");
                            Item.VersionMajorString = VersionSplit.first();
                            Item.VersionMajorNumber = Item.VersionMajorString.toInt();

                            if(BrowserItemObject.contains("browser_type") && BrowserItemObject["browser_type"].isString())
                            {
                                Item.BrowserType = BrowserItemObject["browser_type"].toString();
                            }

                            if(BrowserItemObject.contains("bas_version") && BrowserItemObject["bas_version"].isString())
                            {
                                Item.BasVersion = BrowserItemObject["bas_version"].toString();
                            }

                            if(BrowserItemObject.contains("description") && BrowserItemObject["description"].isString())
                            {
                                Item.Description = BrowserItemObject["description"].toString();
                            }

                            if(BrowserItemObject.contains("architecture") && BrowserItemObject["architecture"].isString())
                            {
                                Item.Architecture = BrowserItemObject["architecture"].toString();
                            }

                            Item.WorkerFolderName = QString("Worker.") + QString::number(Item.Id);
                            Item.WorkerExeRelativePath = Item.WorkerFolderName + QString("/Worker.exe");
                            Item.WorkerExeFullPath = QFileInfo(Item.WorkerExeRelativePath).absoluteFilePath();

                            ResultRaw.append(Item);
                        }

                    }
                }
            }
        }

        //Filter non existing items
        CachedBrowserItems.clear();

        for(BrowserItem& Item: ResultRaw)
        {
            if(QFileInfo(Item.WorkerExeFullPath).exists())
            {
                Item.IsNull = false;
                CachedBrowserItems.append(Item);
            }
        }

        //Sort
        qSort(CachedBrowserItems.begin(), CachedBrowserItems.end(), ItemMajorVersionLessThan);

        IsCacheGenerated = true;
        return CachedBrowserItems;
    }

    QStringList BrowserVersionSelector::GetAllBrowserFullVersions()
    {
        QStringList Result;
        for(BrowserItem& Item: GetAllBrowserItems())
        {
            Result.append(Item.VersionFull);
        }

        return Result;
    }


    IBrowserVersionSelector::BrowserItem BrowserVersionSelector::GetDefaultBrowserItem()
    {
        QList<BrowserVersionSelector::BrowserItem> AllBrowserItems = GetAllBrowserItems();
        if(AllBrowserItems.isEmpty())
        {
            BrowserItem Item;
            return Item;
        }

        return AllBrowserItems.first();
    }

    IBrowserVersionSelector::BrowserItem BrowserVersionSelector::GetBrowserItemById(int Id)
    {
        if(Id == 0)
        {
            return GetDefaultBrowserItem();
        }

        for(BrowserItem& Item: GetAllBrowserItems())
        {
            if(Item.Id == Id)
                return Item;
        }

        BrowserItem Item;
        return Item;
    }

    IBrowserVersionSelector::BrowserItem BrowserVersionSelector::GetBrowserItemByVersion(const QString& Version)
    {
        if(Version.isEmpty() || Version == QString("0") || Version.toLower() == "default")
        {
            return GetDefaultBrowserItem();
        }

        QList<IBrowserVersionSelector::BrowserItem> AllItems = GetAllBrowserItems();

        if(AllItems.isEmpty())
        {
            BrowserItem Item;
            return Item;
        }

        if(Version == "random")
        {
            BrowserItem Item = AllItems.at(qrand() % AllItems.size());
            return Item;
        }

        for(BrowserItem& Item: AllItems)
        {
            if(Item.VersionFull == Version)
                return Item;
        }

        for(BrowserItem& Item: AllItems)
        {
            if(Item.VersionMajorString == Version)
                return Item;
        }

        for(BrowserItem& Item: AllItems)
        {
            if(QString::number(Item.Id) == Version)
                return Item;
        }

        BrowserItem Item;
        return Item;
    }

}
