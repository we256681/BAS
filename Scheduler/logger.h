#ifndef LOGGER_H
#define LOGGER_H

#include <QDateTime>
#include "stream.h"

struct LogItem
{
    QString Id;
    QDateTime Date;
    QString Message;
    QString Priority;
    QString Status;
    QString Type;
    QString Param1;
    QString Param2;
    QString Param3;
    QString Param4;
    QString Param5;

    QString Param1Name;
    QString Param2Name;
    QString Param3Name;
    QString Param4Name;
    QString Param5Name;
    QString Serialize();
    QJsonObject SerializeToObject();
    static LogItem Parse(const QString& Text);
};

struct LogItemApplication
{
    QString Id;
    QString Message;
    QString ActionId;
    QString TaskName;
    QString Type;
    QString ResultIndex;
    QString Serialize();
    QJsonObject SerializeToObject();
};

class Logger
{
    QList<LogItem> Items;
    QList<LogItemApplication> ApplicationItems;
    static const int MaxItems = 10000;
    QString Separator = "-----------------------";
    Stream *_Stream = 0;
    QDateTime LastDateTime;
public:
    Logger();
    void SetStream(Stream *_Stream);
    void Add(
            const QString& Message,
            const QString& Priority,
            const QString& Status,
            const QString& Type,
            const QString& Param1 = QString(),
            const QString& Param1Name = QString(),
            const QString& Param2 = QString(),
            const QString& Param2Name = QString(),
            const QString& Param3 = QString(),
            const QString& Param3Name = QString(),
            const QString& Param4 = QString(),
            const QString& Param4Name = QString(),
            const QString& Param5 = QString(),
            const QString& Param5Name = QString()
         );

    void AddApplicationLog(
            const QString& Message,
            const QString& ActionId,
            const QString& TaskName,
            const QString& Type,
            const QString& ResultIndex
         );
    void LoadAll(bool ChangeFile);

    QList<LogItem> FindAll();
    QList<LogItemApplication> FindAllApplication();

    void ClearAll();
    void ClearAllApplication();
};

#endif // LOGGER_H
