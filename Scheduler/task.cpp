#include "task.h"
#include <QJsonDocument>
#include <QJsonArray>

Schedule::Schedule()
{

}


void Schedule::Set(const QString& Key, int Value)
{
    Data.insert(Key,QJsonValue(Value));
}

void Schedule::Set(const QString& Key, const QString& Value)
{
    Data.insert(Key,QJsonValue(Value));
}

void Schedule::Set(const QString& Key, QDateTime Value)
{
    Data.insert(Key,QJsonValue(QString::number(Value.toMSecsSinceEpoch())));
}

int Schedule::GetInt(const QString& Key, int Default)
{
    if(!Data.contains(Key))
        return Default;
    return Data.value(Key).toInt();
}

QString Schedule::GetString(const QString& Key, const QString& Default)
{
    if(!Data.contains(Key))
        return Default;
    return Data.value(Key).toString();
}

QDateTime Schedule::GetDate(const QString& Key, QDateTime Default)
{
    if(!Data.contains(Key))
        return Default;
    return QDateTime::fromMSecsSinceEpoch(Data.value(Key).toString().toLongLong());
}

bool Schedule::IsDateInvalid(const QString& Key)
{
    return GetString(Key,QString()) == QString();
}

bool Schedule::GetBool(const QString& Key, bool Default)
{
    if(!Data.contains(Key))
        return Default;
    return Data.value(Key).toBool();
}

QJsonArray Schedule::GetArray(const QString& Key)
{
    return Data.value(Key).toArray();
}

QString Schedule::Serialize()
{
    QJsonDocument doc(Data);
    return QString::fromUtf8(doc.toJson());
}

Schedule Schedule::Parse(const QString& Text)
{
    Schedule Res;
    Res.Data = QJsonDocument::fromJson(Text.toUtf8()).object();
    return Res;
}



Task::Task()
{
    DirtyListAll.clear();
    DirtyListAll.insert("run_last_date");
    DirtyListAll.insert("end_last_date");
    DirtyListAll.insert("schedule");
    DirtyListAll.insert("run_next_date");
    DirtyListAll.insert("status");
    DirtyListAll.insert("threads");
    DirtyListAll.insert("number_execute");
    DirtyListAll.insert("success_total");
    DirtyListAll.insert("fail_total");
    DirtyListAll.insert("success_current");
    DirtyListAll.insert("fail_current");
    DirtyListAll.insert("number_execute");
    DirtyListAll.insert("log_path");
    DirtyListAll.insert("report");
    DirtyListAll.insert("browsers_count");
    DirtyListAll.insert("script_name");
    DirtyListAll.insert("script_data");
    DirtyListAll.insert("script_login");
    DirtyListAll.insert("script_pass");
    DirtyListAll.insert("settings");
    DirtyListAll.insert("name");

    DirtyListAll.insert("result1_path");
    DirtyListAll.insert("result1_name");
    DirtyListAll.insert("result1_active");
    DirtyListAll.insert("result1_number");

    DirtyListAll.insert("result2_path");
    DirtyListAll.insert("result2_name");
    DirtyListAll.insert("result2_active");
    DirtyListAll.insert("result2_number");

    DirtyListAll.insert("result3_path");
    DirtyListAll.insert("result3_name");
    DirtyListAll.insert("result3_active");
    DirtyListAll.insert("result3_number");

    DirtyListAll.insert("result4_path");
    DirtyListAll.insert("result4_name");
    DirtyListAll.insert("result4_active");
    DirtyListAll.insert("result4_number");

    DirtyListAll.insert("result5_path");
    DirtyListAll.insert("result5_name");
    DirtyListAll.insert("result5_active");
    DirtyListAll.insert("result5_number");

    DirtyListAll.insert("result6_path");
    DirtyListAll.insert("result6_name");
    DirtyListAll.insert("result6_active");
    DirtyListAll.insert("result6_number");

    DirtyListAll.insert("result7_path");
    DirtyListAll.insert("result7_name");
    DirtyListAll.insert("result7_active");
    DirtyListAll.insert("result7_number");

    DirtyListAll.insert("result8_path");
    DirtyListAll.insert("result8_name");
    DirtyListAll.insert("result8_active");
    DirtyListAll.insert("result8_number");

    DirtyListAll.insert("result9_path");
    DirtyListAll.insert("result9_name");
    DirtyListAll.insert("result9_active");
    DirtyListAll.insert("result9_number");


}

QList<Schedule> Task::GetScheduleList()
{
    return ScheduleList;
}

QString Task::GetRandomString()
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

void Task::SetDirtyAll()
{
    DirtyList.clear();
    for(QString& Str: Data.keys())
    {
        DirtyList.insert(Str);
    }
}

void Task::GenerateNewId()
{
    Set("id", GetRandomString());
}

void Task::Set(const QString& Key, int Value)
{
    Data.insert(Key,QJsonValue(Value));
    DirtyList.insert(Key);
}

void Task::Increase(const QString& Key)
{
    Set(Key, GetInt(Key) + 1);
}

void Task::Decrease(const QString& Key)
{
    Set(Key, GetInt(Key) - 1);
}


void Task::Set(const QString& Key, const QString& Value)
{
    Data.insert(Key,QJsonValue(Value));
    DirtyList.insert(Key);
}

void Task::Set(const QString& Key, QDateTime Value)
{
    Data.insert(Key,QJsonValue(QString::number(Value.toMSecsSinceEpoch())));
    DirtyList.insert(Key);
}

void Task::SetDateInvalid(const QString& Key)
{
    Set(Key, QString());
}

int Task::GetInt(const QString& Key, int Default)
{
    if(!Data.contains(Key))
        return Default;
    return Data.value(Key).toInt();
}

QString Task::GetString(const QString& Key, const QString& Default)
{
    if(!Data.contains(Key))
        return Default;
    return Data.value(Key).toString();
}

QDateTime Task::GetDate(const QString& Key, QDateTime Default)
{
    if(!Data.contains(Key))
        return Default;
    return QDateTime::fromMSecsSinceEpoch(Data.value(Key).toString().toLongLong());
}

bool Task::IsDateInvalid(const QString& Key)
{
    return GetString(Key,QString()) == QString();
}


QString Task::Serialize()
{
    QJsonObject DataCopy = Data;

    if(!ScheduleList.isEmpty())
    {
        DataCopy.insert("schedule", ScheduleList.at(0).Data);
    }

    QJsonDocument doc(DataCopy);
    return QString::fromUtf8(doc.toJson());
}

Task Task::Parse(const QString& Text, bool IsNew)
{
    Task Res;
    Res.Data = QJsonDocument::fromJson(Text.toUtf8()).object();
    if(Res.Data.contains("schedule"))
    {
        QJsonValue ScheduleObject = Res.Data.value("schedule");
        Res.Data.remove("schedule");
        if(ScheduleObject.isObject())
        {
            Schedule schedule;
            schedule.Data = ScheduleObject.toObject();
            Res.ScheduleList.append(schedule);
        }
    }
    if(IsNew)
    {
        Res.IsNew = IsNew;
    }
    return Res;
}

bool Task::GetIsDirty()
{
    return !DirtyList.isEmpty();
}

bool Task::GetIsNew()
{
    return IsNew;
}

QJsonObject Task::PrepareDirtyParams()
{
    QJsonObject Res;

    for(const QString Key: DirtyList)
    {
        if(DirtyListAll.contains(Key))
        {
            if(Key == "schedule")
            {
                if(ScheduleList.size() == 1)
                    Res.insert("schedule", ScheduleList.at(0).Data);
            }else
                Res.insert(Key,Data.value(Key));
        }
    }

    Res.insert("id", Data.value("id").toString());

    return Res;

}

void Task::ClearDirtyAndNew()
{
    DirtyList.clear();
    IsNew = false;
}

QJsonObject Task::GetData()
{
    QJsonObject DataCopy = Data;

    if(!ScheduleList.isEmpty())
    {
        DataCopy.insert("schedule", ScheduleList.at(0).Data);
    }

    return DataCopy;
}


void Task::UpdateWith(const Task& T)
{
    for(const QString& Key: T.Data.keys())
    {
        if(
           Key == "script_name" ||
           Key == "script_login" ||
           Key == "script_pass" ||
           Key == "script_data" ||
           Key == "settings" ||
           Key == "name"
        )
        {
            Data.insert(Key,T.Data.value(Key));
            DirtyList.insert(Key);
        }
    }
    if(T.ScheduleList.size() > 0)
    {
        ScheduleList = T.ScheduleList;
        DirtyList.insert("schedule");
    }
}
