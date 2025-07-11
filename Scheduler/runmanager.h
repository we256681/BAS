#ifndef RUNMANAGER_H
#define RUNMANAGER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QRegularExpression>

class ProjectVersion
{
    int ToNumber() const
    {
        return 100 * 100 * Major + 100 * Minor + Patch;
    }

    int Major = 0;
    int Minor = 0;
    int Patch = 0;

public:

    ProjectVersion(const QString& Data)
    {
        QStringList Split = Data.split(".");
        QRegularExpression RegexpVersion("^\\d+$");
        if(Split.size() == 3)
        {
            if(RegexpVersion.match(Split[0]).hasMatch() && RegexpVersion.match(Split[1]).hasMatch() && RegexpVersion.match(Split[2]).hasMatch())
            {
                Major = Split[0].toInt();
                Minor = Split[1].toInt();
                Patch = Split[2].toInt();
            }
        }
    }

    ProjectVersion()
    {
    }

    bool IsNull()
    {
        return Major == 0 && Minor == 0 && Patch == 0;
    }

    QString ToString()
    {
        return QString::number(Major) + QString(".") + QString::number(Minor) + QString(".") + QString::number(Patch);
    }

    bool operator <(const ProjectVersion& Other)
    {
        return ToNumber() < Other.ToNumber();
    }
    bool operator >(const ProjectVersion& Other)
    {
        return ToNumber() > Other.ToNumber();
    }
    bool operator <=(const ProjectVersion& Other)
    {
        return ToNumber() <= Other.ToNumber();
    }
    bool operator >=(const ProjectVersion& Other)
    {
        return ToNumber() >= Other.ToNumber();
    }
    bool operator ==(const ProjectVersion& Other)
    {
        return ToNumber() == Other.ToNumber();
    }
    bool operator !=(const ProjectVersion& Other)
    {
        return ToNumber() != Other.ToNumber();
    }

};

class RunManager : public QObject
{
    Q_OBJECT
    QFile * LockFile = 0;
public:
    explicit RunManager(QObject *parent = nullptr);
    bool OnStart(const QStringList& Params);
    //Empty string if no update needed, project version otherwise
    QString CheckUpdateReady();
    void Restart();
    void Uninstall();
private:



public slots:
};

#endif // RUNMANAGER_H
