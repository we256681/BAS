#include "logger.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>
#include <QDir>


Logger::Logger()
{

}

QString GetRandomString()
{
   const QString possibleCharacters("abcdefghijklmnopqrstuvwxyz");
   const int randomStringLength = 10;

   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString;
}

void Logger::SetStream(Stream *_Stream)
{
    this->_Stream = _Stream;
}

QJsonObject LogItem::SerializeToObject()
{
    QJsonObject Data;

    Data.insert("date",QJsonValue(QString::number(Date.toMSecsSinceEpoch())));
    Data.insert("message",Message);
    Data.insert("priority",Priority);
    Data.insert("status",Status);
    Data.insert("type",Type);
    Data.insert("param1",Param1Name);
    Data.insert("param2",Param2Name);
    Data.insert("param3",Param3Name);
    Data.insert("param4",Param4Name);
    Data.insert("param5",Param5Name);
    Data.insert("id",Id);


    if(!Param1Name.isEmpty())
        Data.insert(Param1Name,Param1);
    if(!Param2Name.isEmpty())
        Data.insert(Param2Name,Param2);
    if(!Param3Name.isEmpty())
        Data.insert(Param3Name,Param3);
    if(!Param4Name.isEmpty())
        Data.insert(Param4Name,Param4);
    if(!Param5Name.isEmpty())
        Data.insert(Param5Name,Param5);

    return Data;

}

QJsonObject LogItemApplication::SerializeToObject()
{
    QJsonObject Data;
    Data.insert("message",Message);
    Data.insert("action_id",ActionId);
    Data.insert("task_name",TaskName);
    Data.insert("type",Type);
    Data.insert("result_index",ResultIndex);
    Data.insert("id",Id);
    return Data;
}


QString LogItem::Serialize()
{
    QJsonDocument doc(SerializeToObject());
    return QString::fromUtf8(doc.toJson());
}

QString LogItemApplication::Serialize()
{
    QJsonDocument doc(SerializeToObject());
    return QString::fromUtf8(doc.toJson());
}


QList<LogItem> Logger::FindAll()
{
    return Items;
}

QList<LogItemApplication> Logger::FindAllApplication()
{
    return ApplicationItems;
}


void Logger::ClearAll()
{
    Items.clear();
    QFile inputFile("../logs/log.txt");
    inputFile.remove();
}

void Logger::ClearAllApplication()
{
    ApplicationItems.clear();
}

LogItem LogItem::Parse(const QString& Text)
{
    LogItem Res;
    QJsonObject Object = QJsonDocument::fromJson(Text.toUtf8()).object();
    Res.Date = QDateTime::fromMSecsSinceEpoch(Object.value("date").toString().toLongLong());
    Res.Message = Object.value("message").toString();
    Res.Priority = Object.value("priority").toString();
    Res.Status = Object.value("status").toString();
    Res.Type = Object.value("type").toString();
    Res.Id = Object.value("id").toString();
    Res.Param1Name = Object.value("param1").toString();
    Res.Param2Name = Object.value("param2").toString();
    Res.Param3Name = Object.value("param3").toString();
    Res.Param4Name = Object.value("param4").toString();
    Res.Param5Name = Object.value("param5").toString();

    Res.Param1 = Object.value(Res.Param1Name).toString();
    Res.Param2 = Object.value(Res.Param2Name).toString();
    Res.Param3 = Object.value(Res.Param3Name).toString();
    Res.Param4 = Object.value(Res.Param4Name).toString();
    Res.Param5 = Object.value(Res.Param5Name).toString();

    return Res;
}


void Logger::Add(
        const QString& Message,
        const QString& Priority,
        const QString& Status,
        const QString& Type,
        const QString& Param1,
        const QString& Param1Name,
        const QString& Param2,
        const QString& Param2Name,
        const QString& Param3,
        const QString& Param3Name,
        const QString& Param4,
        const QString& Param4Name,
        const QString& Param5,
        const QString& Param5Name
     )
{
    LogItem Item;

    Item.Date = QDateTime::currentDateTime();
    if(LastDateTime.isValid() && Item.Date <= LastDateTime)
    {
        Item.Date = LastDateTime.addMSecs(1);
    }
    LastDateTime = Item.Date;
    Item.Message = Message;
    Item.Priority = Priority;
    Item.Status = Status;
    Item.Type = Type;
    Item.Param1 = Param1;
    Item.Param2 = Param2;
    Item.Param3 = Param3;
    Item.Param4 = Param4;
    Item.Param5 = Param5;
    Item.Param1Name = Param1Name;
    Item.Param2Name = Param2Name;
    Item.Param3Name = Param3Name;
    Item.Param4Name = Param4Name;
    Item.Param5Name = Param5Name;
    Item.Id = GetRandomString();
    Items.push_back(Item);
    QDir("../logs").mkpath(".");
    QFile File("../logs/log.txt");
    QFileInfo Info("../logs/log.txt");

    if(Items.size() > MaxItems)
        Items.erase(Items.begin() ,Items.begin() + Items.size() - MaxItems);


    if(File.open(QFile::WriteOnly | QFile::Text | QFile::Append))
    {
        QTextStream out(&File);
        out.setCodec("UTF-8");
        if(Info.size() > 0)
        {
            out<<"\n\n"<<Separator<<"\n\n";
        }
        out<<Item.Serialize().toUtf8();
        File.close();
    }

    _Stream->Send("add_scheduler_log",Item.SerializeToObject());
}

void Logger::AddApplicationLog(
        const QString& Message,
        const QString& ActionId,
        const QString& TaskName,
        const QString& Type,
        const QString& ResultIndex
     )
{
    LogItemApplication Item;
    Item.Message = Message;
    Item.ActionId = ActionId;
    Item.TaskName = TaskName;
    Item.Type = Type;
    Item.ResultIndex = ResultIndex;
    Item.Id = GetRandomString();


    if(ApplicationItems.size() > 10000)
        ApplicationItems.erase(ApplicationItems.begin() + 5000 ,ApplicationItems.end());

    ApplicationItems.prepend(Item);

    _Stream->Send("add_application_log",Item.SerializeToObject());

}


void Logger::LoadAll(bool ChangeFile)
{
    Items.clear();
    QDir("../logs").mkpath(".");

    QFile inputFile("../logs/log.txt");
    QString Collect;
    if(inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        in.setCodec("UTF-8");

        while (!in.atEnd())
        {
            QString line = in.readLine();
            if(line == Separator)
            {
                Collect = Collect.trimmed();
                if(!Collect.isEmpty())
                    Items.append(LogItem::Parse(Collect));
                Collect.clear();
            }else
            {
                Collect.append("\n");
                Collect.append(line);
            }
        }
        inputFile.close();
    }

    Collect = Collect.trimmed();
    if(!Collect.isEmpty())
        Items.append(LogItem::Parse(Collect));

    if(Items.size() > MaxItems)
        Items.erase(Items.begin() ,Items.begin() + Items.size() - MaxItems);

    if(ChangeFile)
    {
        QFile File("../logs/log.txt");
        File.remove();
        if(File.open(QFile::WriteOnly | QFile::Text | QFile::Append))
        {
            QTextStream out(&File);
            out.setCodec("UTF-8");
            int Iterator = 0;

            for(LogItem& Item: Items)
            {
                if(Iterator)
                {
                    out<<"\n\n"<<Separator<<"\n\n";
                }
                Iterator++;
                out<<Item.Serialize().toUtf8();
            }

            File.close();
        }
    }
}
