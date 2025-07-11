#include "oldversionremover.h"
#include <QDir>
#include <QRegularExpression>

OldVersionRemover::OldVersionRemover(QObject *parent) : QObject(parent)
{

}

bool CompareVersions(const QString& Version1,const QString& Version2)
{
    QStringList Version1Split = Version1.split(".");
    QStringList Version2Split = Version2.split(".");

    if(Version1Split.size() == 3 && Version2Split.size() == 3)
    {
        int Version1X = 0;
        int Version1Y = 0;
        int Version1Z = 0;
        int Version2X = 0;
        int Version2Y = 0;
        int Version2Z = 0;

        Version1X = Version1Split.at(0).toInt();
        Version1Y = Version1Split.at(1).toInt();
        Version1Z = Version1Split.at(2).toInt();

        Version2X = Version2Split.at(0).toInt();
        Version2Y = Version2Split.at(1).toInt();
        Version2Z = Version2Split.at(2).toInt();


        return  (Version1X > Version2X
                || (Version1X == Version2X && Version1Y > Version2Y)
                || (Version1X == Version2X && Version1Y == Version2Y && Version1Z > Version2Z));

    }

    return true;
}

bool CheckIfVersion(const QString& Text)
{
    QRegularExpression re("^\\d+\\.\\d+\\.\\d+$");
    re.setPatternOptions(QRegularExpression::MultilineOption);
    QRegularExpressionMatch match = re.match(Text);
    return !match.hasMatch();
}

void OldVersionRemover::Remove(const QString& Dir, int NumberOfFoldersToKeep)
{
    QDir dir(Dir);
    QStringList list = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
    list.erase(std::remove_if(list.begin(), list.end(), CheckIfVersion), list.end());
    std::sort(list.begin(), list.end(), CompareVersions);
    for(int i = 0;i<NumberOfFoldersToKeep;i++)
    {
        if(list.isEmpty())
        {
            break;
        }
        list.removeFirst();
    }
    //list.erase(list.begin(),list.begin() + NumberOfFoldersToKeep);
    for(QString& d:list)
    {
        QDir parent(Dir);
        parent.cd(d);
        parent.removeRecursively();
    }
}

