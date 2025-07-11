#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QJsonObject>
#include <QDateTime>
#include <QSet>

class Schedule
{
    QJsonObject Data;
public:
    Schedule();
    void Set(const QString& Key, int Value);
    void Set(const QString& Key, const QString& Value);
    void Set(const QString& Key, QDateTime Value);

    int GetInt(const QString& Key, int Default = 0);
    QString GetString(const QString& Key, const QString& Default = QString());
    QDateTime GetDate(const QString& Key, QDateTime Default = QDateTime());
    QJsonArray GetArray(const QString& Key);
    bool IsDateInvalid(const QString& Key);
    bool GetBool(const QString& Key, bool Default);
    QString Serialize();
    static Schedule Parse(const QString& Text);

    friend class Task;

};

class Task
{

    QJsonObject Data;
    QSet<QString> DirtyList;
    QSet<QString> DirtyListAll;
    bool IsNew = false;
    QList<Schedule> ScheduleList;
public:
    Task();
    void GenerateNewId();
    void Set(const QString& Key, int Value);
    void Set(const QString& Key, const QString& Value);
    void Set(const QString& Key, QDateTime Value);
    void SetDateInvalid(const QString& Key);
    void Increase(const QString& Key);
    void Decrease(const QString& Key);
    int GetInt(const QString& Key, int Default = 0);
    QString GetString(const QString& Key, const QString& Default = QString());
    QDateTime GetDate(const QString& Key, QDateTime Default = QDateTime());

    bool IsDateInvalid(const QString& Key);

    QString Serialize();
    static Task Parse(const QString& Text, bool IsNew);

    void UpdateWith(const Task& T);

    bool GetIsDirty();
    bool GetIsNew();
    QJsonObject PrepareDirtyParams();
    void ClearDirtyAndNew();
    QJsonObject GetData();

    QList<Schedule> GetScheduleList();
private:
    QString GetRandomString();
    void SetDirtyAll();


};

#endif // TASK_H
