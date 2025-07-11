#include "taskmanager.h"
#include <QDir>
#include <QJsonArray>

TaskManager::TaskManager(QObject *parent) : QObject(parent)
{
    QTimer * Timer = new QTimer(this);
    Timer->setSingleShot(false);
    Timer->setInterval(1000);
    connect(Timer,SIGNAL(timeout()),this,SLOT(Process()));
    connect(Timer,SIGNAL(timeout()),this,SIGNAL(Iddle()));

    Timer->start();
}

int TaskManager::ObjectToInterval(const QJsonObject& Object)
{
    int res = 0;
    if(Object.contains("minute"))
    {
        res += 60 * Object.value("minute").toInt();
    }

    if(Object.contains("hour"))
    {
        res += 60 * 60 * Object.value("hour").toInt();
    }

    if(Object.contains("day"))
    {
        res += 24 * 60 * 60 * (Object.value("day").toInt() - 1);
    }

    if(Object.contains("week"))
    {
        res += 24 * 60 * 60 * (Object.value("week").toInt() - 1);
    }
    return res;
}

bool TaskManager::CheckProbability(int Probability)
{
    return qrand() % 100 < Probability;
}

int TaskManager::GetRanomBetween(int Start, int End)
{
    if(Start == End)
        return Start;
    int Big;
    int Small;
    if(Start > End)
    {
        Big = Start;
        Small = End;
    }else
    {
        Small = Start;
        Big = End;
    }
    return qrand() % (Big - Small + 1) + Small ;

}

void TaskManager::SetApiExternal(ApiExternal *_ApiExternal)
{
    this->_ApiExternal = _ApiExternal;
    connect(_ApiExternal,SIGNAL(TaskInsert(QString,bool)),this,SLOT(TaskInsert(QString,bool)));
    connect(_ApiExternal,SIGNAL(TaskUpdate(QString,QString,bool)),this,SLOT(TaskUpdate(QString,QString,bool)));
    connect(_ApiExternal,SIGNAL(StartResourceEditLocal(QString,QString,QString)),this,SLOT(StartResourceEditLocal(QString,QString,QString)));
    connect(_ApiExternal,SIGNAL(StartResourceEditRemote(QString,QString,QString,QString,QString)),this,SLOT(StartResourceEditRemote(QString,QString,QString,QString,QString)));
    connect(_ApiExternal,SIGNAL(TaskReport(QString)),this,SLOT(TaskReport(QString)));
    connect(_ApiExternal,SIGNAL(TaskClearAll()),this,SLOT(TaskClearAll()));
    connect(_ApiExternal,SIGNAL(TaskRemoveArchived()),this,SLOT(TaskRemoveArchived()));
    connect(_ApiExternal,SIGNAL(ManualAnswer(QString,QString,bool)),this,SLOT(ManualAnswer(QString,QString,bool)));

    connect(_ApiExternal,SIGNAL(ClearApplicationLog()),this,SLOT(ClearApplicationLog()));
    connect(_ApiExternal,SIGNAL(ClearSchedulerLog()),this,SLOT(ClearSchedulerLog()));

    connect(_ApiExternal,SIGNAL(TaskStop(QString)),this,SLOT(TaskStop(QString)));
    connect(_ApiExternal,SIGNAL(TaskCancel(QString)),this,SLOT(TaskCancel(QString)));
    connect(_ApiExternal,SIGNAL(TaskStart(QString)),this,SLOT(TaskStart(QString)));
    connect(_ApiExternal,SIGNAL(TaskArchive(QString)),this,SLOT(TaskArchive(QString)));
    connect(_ApiExternal,SIGNAL(TaskRestore(QString)),this,SLOT(TaskRestore(QString)));
    connect(_ApiExternal,SIGNAL(TaskDelete(QString)),this,SLOT(TaskDelete(QString)));
    connect(_ApiExternal,SIGNAL(TaskToggleVisibility(QString)),this,SLOT(TaskToggleVisibility(QString)));
}

void TaskManager::ManualAnswer(QString ActionId,QString Result,bool IsSuccess)
{
    _ManualActions->Answer(ActionId,Result,IsSuccess);
}

void TaskManager::ClearApplicationLog()
{
    this->_Logger->ClearAllApplication();
}

void TaskManager::ClearSchedulerLog()
{
    this->_Logger->ClearAll();
}

void TaskManager::SetManualActions(ManualActions *_ManualActions)
{
    this->_ManualActions = _ManualActions;
}

void TaskManager::SetApiScript(ApiScript *_ApiScript)
{
    this->_ApiScript = _ApiScript;
    connect(_ApiScript,SIGNAL(OnStarted(QString)),this,SLOT(OnStarted(QString)));
    connect(_ApiScript,SIGNAL(OnFinished(QString,QString,bool)),this,SLOT(OnFinished(QString,QString,bool)));
    connect(_ApiScript,SIGNAL(OnMessage(QString,QString,QString,QMap<QString,QString>)),this,SLOT(OnMessage(QString,QString,QString,QMap<QString,QString>)));
    connect(this,SIGNAL(Iddle()),_ApiScript,SLOT(Timer()));
}

void TaskManager::OnMessage(QString Id,QString TagName, QString Content, QMap<QString,QString> Params)
{
    if(TagName == "ResourcesEdit")
    {
        QJsonObject Result;
        Result.insert("id",Id);
        Result.insert("data",Content);
        Result.insert("message",QString());
        Result.insert("success",true);
        _Stream->Send("resources_edit",Result);
        return;
    }


    if(!_TaskPersist->Contains(Id))
        return;

    Task &_Task = _TaskPersist->Find(Id);

    bool Updated = false;

    if(TagName == "ThreadStarted")
    {
        Updated = true;
        _Task.Increase("threads");
    }else if(TagName == "Manual")
    {
        if(Params["Type"] == "BrowserStop")
        {
            _ManualActions->BrowserStop(Params["Id"]);
        }else
        {
            _ManualActions->Add(Params["Id"],Id,Params["Type"],Content);
        }
    }else if(TagName == "ThreadFail")
    {
        Updated = true;
        _Task.Decrease("threads");
        _Task.Increase("fail_current");
    }else if(TagName == "ThreadSuccess")
    {
        Updated = true;
        _Task.Decrease("threads");
        _Task.Increase("success_current");
    }else if(TagName == "BrowserStart")
    {
        Updated = true;
        _Task.Increase("browsers_count");
    }else if(TagName == "BrowserStop")
    {
        Updated = true;
        _Task.Decrease("browsers_count");
    }else if(TagName == "StageStatistic")
    {
        Updated = true;
        _Task.Set("threads", 0);
        _Task.Set("browsers_count", 0);
        _Task.Set("success_total", Params["MaxSuccess"].toInt());
        _Task.Set("fail_total", Params["MaxFails"].toInt());
    }else if(TagName == "LogInfo")
    {
        Updated = true;
        _Task.Set("log_path", Content);
    }else if(TagName == "Log")
    {
        _Logger->AddApplicationLog(Content, Params["ActionId"],_Task.GetString("name") ,Params["Type"] , QString());
    }else if(TagName == "ResultInfo")
    {
        Updated = true;
        QString Index = Params["Index"];
        QString Name = Params["Name"];
        QString Visibility = Params["Visibility"];
        _Task.Set(QString("result") + Index + QString("_path"), Content);
        _Task.Set(QString("result") + Index + QString("_name"), Name);
        _Task.Set(QString("result") + Index + QString("_active"), Visibility);
    }else if(TagName == "Report")
    {
        Updated = true;
        _Task.Set("report", Content);
    }else if(TagName == "Result")
    {
        Updated = true;
        int Index = Params["Index"].toInt() + 1;
        QString Name = QString("result") + QString::number(Index) + QString("_number");
        _Task.Increase(Name);

        _Logger->AddApplicationLog(Content,
                                   _Task.GetString(QString("result") + QString::number(Params["Index"].toInt() + 1) + QString("_name")),
                                   _Task.GetString("name"),
                                   QString(),
                                   Params["Index"]
                                );
    }
    if(Updated)
    {
        _TaskPersist->SaveAll();
    }
}


void TaskManager::OnFinished(QString Id,QString Log, bool IsSuccess)
{
    if(Id.startsWith("resources_"))
    {
        QJsonObject Result;
        Result.insert("id",Id);
        Result.insert("data",QString());
        Result.insert("message",Log);
        Result.insert("success",IsSuccess);
        _Stream->Send("resources_edit",Result);
        return;
    }
    if(!_TaskPersist->Contains(Id))
        return;

    Task &_Task = _TaskPersist->Find(Id);


    if(IsSuccess)
    {
        _Logger->Add(
                    QString("Finished"),
                    QString("Normal"),
                    QString("Success"),
                    QString("TaskFinished"),
                    _Task.GetString("id"),
                    "task_id",
                    _Task.GetString("name"),
                    "task_name"
                    );
    }else
    {
        _Logger->Add(
                    QString("<a data-fail>Failed to start</a>"),
                    QString("Normal"),
                    QString("Error"),
                    QString("TaskFinished"),
                    _Task.GetString("id"),
                    "task_id",
                    _Task.GetString("name"),
                    "task_name",
                    Log,
                    "scheduler_log");
    }


    UpdateTaskAfterFinished(_Task);
    _TaskPersist->SaveAll();


    Process();
}

void TaskManager::UpdateTaskAfterFinished(Task &T)
{
    if(T.GetScheduleList().size() != 1)
        return;

    if(T.GetString("status") != "executing" && T.GetString("status") != "downloading")
        return;

    Schedule  _Schedule = T.GetScheduleList().at(0);
    QString ScheduleType = _Schedule.GetString("type");

    _ManualActions->ScriptFinished(T.GetString("id"));

    //Update all tasks which launch is depends on this
    QString Name = T.GetString("name");
    for(QString& Id : _TaskPersist->FindAllIds())
    {
        Task &_Task = _TaskPersist->Find(Id);
        if(_Task.GetScheduleList().length() != 1)
        {
            continue;
        }
        Schedule _ScheduleCurrent = _Task.GetScheduleList().at(0);
        if(
                _ScheduleCurrent.GetString("type") == "event" &&
                Name == _ScheduleCurrent.GetString("run_other_task_name") &&
                _Task.GetString("status") == "need_schedule" &&
                Id != T.GetString("id")
          )
        {
            QDateTime ScheduledDate;
            if(_ScheduleCurrent.GetBool("consider_execution_time", false))
            {
                if(_Task.IsDateInvalid("run_last_date"))
                    ScheduledDate = QDateTime::currentDateTime();
                else
                    ScheduledDate = _Task.GetDate("run_last_date").addSecs(_ScheduleCurrent.GetInt("interval_event"));

            }else
            {
                if(_Task.IsDateInvalid("end_last_date"))
                    ScheduledDate = QDateTime::currentDateTime();
                else
                    ScheduledDate = _Task.GetDate("end_last_date").addSecs(_ScheduleCurrent.GetInt("interval_event"));

            }
            _Task.Set("run_next_date",ScheduledDate);
            _Task.Set("status","scheduled_wait_for_execution");

            _Logger->Add(
                        QString(QString("Scheduled at <date>")),
                        QString("Normal"),
                        QString("Normal"),
                        QString("TaskScheduled"),
                        _Task.GetString("id"),
                        "task_id",
                        _Task.GetString("name"),
                        "task_name",
                        QString::number(ScheduledDate.toMSecsSinceEpoch()),
                        "date_replace");

        }

    }

    T.Set("end_last_date",QDateTime::currentDateTime());

    bool IsArchived = false;

    if(T.GetInt("start_after_stop",0))
    {
        RunTask(T);
        T.Set("start_after_stop",0);
    }else if(T.GetInt("archive_after_stop",0))
    {
        T.Increase("number_execute");
        IsArchived = true;
        T.Set("status","archived");
        T.Set("archive_after_stop",0);
    }else
    {
        T.Increase("number_execute");
        if(ScheduleType == "now" || ScheduleType == "at")
        {
            IsArchived = true;
            T.Set("status","archived");
        }else if(ScheduleType == "list" || ScheduleType == "event")
        {
            T.Set("status","need_schedule");
        }else if(ScheduleType == "interval" || ScheduleType == "hour" || ScheduleType == "day" || ScheduleType == "week" || ScheduleType == "month")
        {
            if(T.GetInt("number_execute", 0) >= _Schedule.GetInt("max_start_number", 100000))
            {
                IsArchived = true;
                T.Set("status","archived");
            }else
            {
                T.Set("status","need_schedule");
            }
        }
    }

    if(IsArchived)
    {
        _Logger->Add(
                    QString("Archived"),
                    QString("Normal"),
                    QString("Normal"),
                    QString("TaskArchived"),
                    T.GetString("id"),
                    "task_id",
                    T.GetString("name"),
                    "task_name");
    }


}

void TaskManager::OnStarted(QString Id)
{
    if(!_TaskPersist->Contains(Id))
        return;

    Task &_Task = _TaskPersist->Find(Id);
    _Task.Set("status","executing");
    _TaskPersist->SaveAll();
    _Logger->Add(
                QString("Started"),
                QString("Normal"),
                QString("Success"),
                QString("TaskStarted"),
                _Task.GetString("id"),
                "task_id",
                _Task.GetString("name"),
                "task_name");
}

void TaskManager::TaskReport(QString Id)
{
    _ApiScript->SendMessageFormat(Id,"GetReport");
}

void TaskManager::TaskStop(QString Id)
{
    if(!_TaskPersist->Contains(Id))
        return;

    Task &_Task = _TaskPersist->Find(Id);


    _Logger->Add(
                QString("Stopping manually ..."),
                QString("Normal"),
                QString("Normal"),
                QString("TaskStarted"),
                _Task.GetString("id"),
                "task_id",
                _Task.GetString("name"),
                "task_name");


    //Validation
    if(_Task.GetString("status") != "executing" && _Task.GetString("status") != "downloading")
        return;

    _ApiScript->TerminateProcess(Id);

}

void TaskManager::TaskCancel(QString Id)
{
    _ApiScript->TerminateProcess(Id);
}

void TaskManager::TaskToggleVisibility(QString Id)
{
    if(!_TaskPersist->Contains(Id))
        return;

    Task &_Task = _TaskPersist->Find(Id);


    if(_Task.GetString("status") != "executing")
        return;

    _ApiScript->SendMessageFormat(Id,"VisibilityToggle");

}
void TaskManager::TaskStart(QString Id)
{
    if(!_TaskPersist->Contains(Id))
        return;

    Task &_Task = _TaskPersist->Find(Id);

    //Validation
    if(_Task.GetString("status") != "archived" && _Task.GetString("status") != "scheduled_wait_for_execution")
        return;

    if(_Task.GetString("status") == "archived")
    {
        _Task.Set("archive_after_stop",1);
    }
    _Logger->Add(
        QString("Starting manually ..."),
        QString("Normal"),
        QString("Normal"),
        QString("TaskStarting"),
        _Task.GetString("id"),
        "task_id",
        _Task.GetString("name"),
        "task_name");

    if(_Task.GetScheduleList().size() == 1)
    {
        Schedule _Schedule = _Task.GetScheduleList().at(0);
        QString Type = _Schedule.GetString("type");
        if(Type == "hour" || Type == "day" || Type == "week" || Type == "month")
        {
            _Task.SetDateInvalid("current_interval_start");
            _Task.SetDateInvalid("current_interval_end");
            _Task.Set("run_interval_list","");
        }
    }

    RunTask(_Task);

    _TaskPersist->SaveAll();
}

void TaskManager::TaskArchive(QString Id)
{
    if(!_TaskPersist->Contains(Id))
        return;

    Task &_Task = _TaskPersist->Find(Id);

    //Validation
    if(_Task.GetString("status") == "archived")
        return;

    if(_Task.GetString("status") == "executing" || _Task.GetString("status") == "downloading")
    {
        _Task.Set("archive_after_stop",1);
        _ApiScript->TerminateProcess(Id);
        _TaskPersist->SaveAll();
    }

    if(_Task.GetString("status") == "need_schedule" || _Task.GetString("status") == "scheduled_wait_for_execution")
    {
        _Task.Set("status","archived");
        _Task.SetDateInvalid("run_next_date");
        _Task.Set("archive_after_stop",0);

        _TaskPersist->SaveAll();
    }

}

void TaskManager::TaskRestore(QString Id)
{
    if(!_TaskPersist->Contains(Id))
        return;

    Task &_Task = _TaskPersist->Find(Id);

    _Logger->Add(
                QString("Restored from archive"),
                QString("Normal"),
                QString("Normal"),
                QString("TaskRestoredManually"),
                _Task.GetString("id"),
                "task_id",
                _Task.GetString("name"),
                "task_name");

    //Validation
    if(_Task.GetString("status") != "archived")
        return;

    _Task.Set("number_execute",0);
    _Task.SetDateInvalid("run_last_date");
    _Task.SetDateInvalid("end_last_date");
    if(_Task.GetScheduleList().size() == 1)
    {
        Schedule _Schedule = _Task.GetScheduleList().at(0);
        QString Type = _Schedule.GetString("type");
        if(Type == "hour" || Type == "day" || Type == "week" || Type == "month")
        {
            _Task.SetDateInvalid("current_interval_start");
            _Task.SetDateInvalid("current_interval_end");
            _Task.Set("run_interval_list","");
        }
    }
    _Task.Set("status","need_schedule");
    Process();
    _TaskPersist->SaveAll();


}

void TaskManager::TaskDelete(QString Id)
{
    if(!_TaskPersist->Contains(Id))
        return;

    Task &_Task = _TaskPersist->Find(Id);

    _Logger->Add(
                QString("Deleted"),
                QString("Normal"),
                QString("Error"),
                QString("TaskDeleted"),
                _Task.GetString("id"),
                "task_id",
                _Task.GetString("name"),
                "task_name");


    _ApiScript->TerminateProcess(Id);

    _TaskPersist->Remove(Id);


}

void TaskManager::TaskUpdate(QString Id, QString Text, bool IsHard)
{
    if(!_TaskPersist->Contains(Id))
        return;

    Task Update = Task::Parse(Text, false);

    Task &_Task = _TaskPersist->Find(Id);

    _Task.UpdateWith(Update);

    _Logger->Add(
                QString("Manually edited"),
                QString("Normal"),
                QString("Normal"),
                QString("TaskEdited"),
                _Task.GetString("id"),
                "task_id",
                _Task.GetString("name"),
                "task_name");

    if(IsHard)
    {
        QString Status = _Task.GetString("status");
        if(Status == "scheduled_wait_for_execution")
        {
            _Task.Set("status", "need_schedule");
            if(_Task.GetScheduleList().size() == 1)
            {
                Schedule _Schedule = _Task.GetScheduleList().at(0);
                QString Type = _Schedule.GetString("type");
                if(Type == "hour" || Type == "day" || Type == "week" || Type == "month")
                {
                    _Task.SetDateInvalid("current_interval_start");
                    _Task.SetDateInvalid("current_interval_end");
                    _Task.Set("run_interval_list","");
                }
            }
        }else if(Status == "downloading" || Status == "executing")
        {
            _Logger->Add(
                        QString("Restarting task ..."),
                        QString("Normal"),
                        QString("Normal"),
                        QString("TaskRestarting"),
                        _Task.GetString("id"),
                        "task_id",
                        _Task.GetString("name"),
                        "task_name");

            _Task.Set("start_after_stop", 1);


            _ApiScript->TerminateProcess(Id);
        }

    }

    Process();

    _TaskPersist->SaveAll();

}


void TaskManager::TaskInsert(QString Text, bool IsArchive)
{
    Task T = Task::Parse(Text, true);
    T.GenerateNewId();
    if(IsArchive)
        T.Set("status","archived");
    else
        T.Set("status","need_schedule");

    T.SetDateInvalid("run_next_date");
    T.SetDateInvalid("run_last_date");
    T.SetDateInvalid("end_last_date");
    T.Set("success_total", 0);
    T.Set("fail_total", 0);
    T.Set("success_current", 0);
    T.Set("fail_current", 0);
    T.Set("number_execute", 0);
    T.Set("threads", 0);
    T.Set("browsers_count", 0);
    T.Set("archive_after_stop", 0);
    T.Set("start_after_stop", 0);
    T.Set("log_path","");
    for(int i = 1;i<=9;i++)
    {
        T.Set(QString("result") + QString::number(i) + QString("_path"), "");
        T.Set(QString("result") + QString::number(i) + QString("_name"), "");
        T.Set(QString("result") + QString::number(i) + QString("_active"), "");
        T.Set(QString("result") + QString::number(i) + QString("_number"), 0);
    }
    T.Set("report","");

    T.Set("date_created",QDateTime::currentDateTime());

    if(T.GetString("script_name").isEmpty() && T.GetString("script_data").isEmpty())
    {
        _Logger->Add(
                    QString("No script_name or script_data parameter, don't know how to start task"),
                    QString("Normal"),
                    QString("Error"),
                    QString("TaskInvalid"),
                    T.GetString("id"),
                    "task_id",
                    T.GetString("name"),
                    "task_name");
        return;
    }

    if(T.GetScheduleList().size() != 1)
    {
        _Logger->Add(
                    QString("No information about task schedule"),
                    QString("Normal"),
                    QString("Error"),
                    QString("TaskInvalid"),
                    T.GetString("id"),
                    "task_id",
                    T.GetString("name"),
                    "task_name");
        return;
    }

    _TaskPersist->Insert(T);
    _Logger->Add(
                QString("New task created"),
                QString("Normal"),
                QString("Normal"),
                QString("TaskCreated"),
                T.GetString("id"),
                "task_id",
                T.GetString("name"),
                "task_name");

    Process();


}

void TaskManager::StartResourceEditRemote(QString Id,QString Name,QString Login,QString Pass,QString Actual)
{
    //Sending remote script

    QDir("temp").mkpath(".");


    QString ActualPath;

    if(!Actual.isEmpty())
    {
        QCryptographicHash hash(QCryptographicHash::Sha256);
        hash.addData(Actual.toUtf8());
        QString HashString = QString::fromUtf8(hash.result().toHex());

        QFile Data(QString("temp/") + HashString.mid(0,8) + QString(".actual.xml"));
        if(!Data.exists())
        {
            if(Data.open(QIODevice::WriteOnly))
            {
                Data.write(Actual.toUtf8());
                Data.close();
            }
        }
        ActualPath = QFileInfo(Data).absoluteFilePath();
    }

    QStringList Params;

    Params<<"--resources"<<"--remote"<<"--script"<<Name<<"--username"<<Login<<"--password"<<Pass<<"--scheduler-key"<<Id;
    if(!ActualPath.isEmpty())
    {
        Params<<"--actual"<<ActualPath;
    }
    _ApiScript->RunProcess(Id,Params);

}

void TaskManager::StartResourceEditLocal(QString Id,QString Text,QString Actual)
{
    //Sending local script

    QDir("temp").mkpath(".");

    QString ScriptPath;
    QString ActualPath;


    {
        QCryptographicHash hash(QCryptographicHash::Sha256);
        hash.addData(Text.toUtf8());
        QString HashString = QString::fromUtf8(hash.result().toHex());

        QFile Data(QString("temp/") + HashString.mid(0,8) + QString(".script.xml"));
        if(!Data.exists())
        {
            if(Data.open(QIODevice::WriteOnly))
            {
                Data.write(Text.toUtf8());
                Data.close();
            }
        }
        ScriptPath = QFileInfo(Data).absoluteFilePath();
    }

    if(!Actual.isEmpty())
    {
        QCryptographicHash hash(QCryptographicHash::Sha256);
        hash.addData(Actual.toUtf8());
        QString HashString = QString::fromUtf8(hash.result().toHex());

        QFile Data(QString("temp/") + HashString.mid(0,8) + QString(".actual.xml"));
        if(!Data.exists())
        {
            if(Data.open(QIODevice::WriteOnly))
            {
                Data.write(Actual.toUtf8());
                Data.close();
            }
        }
        ActualPath = QFileInfo(Data).absoluteFilePath();
    }

    QStringList Params;


    Params<<"--resources"<<"--script"<<ScriptPath<<"--scheduler-key"<<Id;
    if(!ActualPath.isEmpty())
    {
        Params<<"--actual"<<ActualPath;
    }
    _ApiScript->RunProcess(Id,Params);

}


void TaskManager::Process()
{
    bool IsUpdated = false;
    QDateTime now = QDateTime::currentDateTime();
    for(QString& Id : _TaskPersist->FindAllIds())
    {
        Task &_Task = _TaskPersist->Find(Id);
        if(_Task.GetScheduleList().length() != 1)
            continue;
        Schedule _Schedule = _Task.GetScheduleList().at(0);
        QString TaskStatus = _Task.GetString("status");
        if(TaskStatus == "need_schedule")
        {
            QString ScheduleType = _Schedule.GetString("type");
            bool IsScheduled = false;
            bool IsArchived = false;
            QDateTime ScheduledDate;
            if(ScheduleType == "now")
            {
                IsUpdated = true;
                IsScheduled = true;
                ScheduledDate = now;
                _Task.Set("status","scheduled_wait_for_execution");
                _Task.Set("run_next_date",now);
            }else if(ScheduleType == "list")
            {
                IsUpdated = true;

                QJsonArray JsonDates = _Schedule.GetArray("date_start_list");
                QList<QDateTime> Dates;
                for(QJsonValueRef DateValue:JsonDates)
                {
                    Dates.append(QDateTime::fromMSecsSinceEpoch(DateValue.toString().toLongLong()));
                }
                std::sort(Dates.begin(),Dates.end());
                for(QDateTime Date:Dates)
                {
                    if(now <= Date)
                    {
                        _Task.Set("status","scheduled_wait_for_execution");
                        _Task.Set("run_next_date",Date);
                        ScheduledDate = Date;
                        IsScheduled = true;
                        break;
                    }
                }
                if(!IsScheduled)
                {
                    IsArchived = true;
                    _Task.Set("status","archived");
                }

            }else if(ScheduleType == "at")
            {
                IsUpdated = true;
                IsScheduled = true;
                ScheduledDate = _Schedule.GetDate("date_start");
                _Task.Set("status","scheduled_wait_for_execution");
                _Task.Set("run_next_date",_Schedule.GetDate("date_start"));
            }else if(ScheduleType == "interval")
            {
                IsUpdated = true;

                if(_Schedule.GetBool("consider_execution_time", false))
                {
                    if(_Task.IsDateInvalid("run_last_date"))
                        ScheduledDate = now;
                    else
                        ScheduledDate = _Task.GetDate("run_last_date").addSecs(_Schedule.GetInt("interval"));

                }else
                {
                    if(_Task.IsDateInvalid("end_last_date"))
                        ScheduledDate = now;
                    else
                        ScheduledDate = _Task.GetDate("end_last_date").addSecs(_Schedule.GetInt("interval"));

                }


                if(!_Schedule.IsDateInvalid("date_start") && ScheduledDate < _Schedule.GetDate("date_start"))
                {
                    ScheduledDate = _Schedule.GetDate("date_start");
                }


                if(!_Schedule.IsDateInvalid("date_end") && ScheduledDate > _Schedule.GetDate("date_end"))
                {
                    IsArchived = true;
                    _Task.Set("status","archived");
                }else
                {
                    IsScheduled = true;
                    _Task.Set("run_next_date",ScheduledDate);
                    _Task.Set("status","scheduled_wait_for_execution");

                }
            }else if(ScheduleType == "hour" || ScheduleType == "day" || ScheduleType == "week" || ScheduleType == "month")
            {
                bool IsHour = ScheduleType == "hour";
                bool IsDay = ScheduleType == "day";
                bool IsWeek = ScheduleType == "week";
                bool IsMonth = ScheduleType == "month";
                IsUpdated = true;
                QDateTime CurrentIntervalStart;
                QDateTime CurrentIntervalEnd;
                QList<QDateTime> Items;
                bool CanGenerateItemsForCurrentInterval = false;
                if(
                        _Task.IsDateInvalid("current_interval_start") ||
                        _Task.IsDateInvalid("current_interval_end") ||
                        now > _Task.GetDate("current_interval_end")
                  )
                {
                    //Interval is outdated
                    if(IsHour)
                    {
                        CurrentIntervalStart = QDateTime(now.date(),QTime(now.time().hour(),0));
                        CurrentIntervalEnd = CurrentIntervalStart.addSecs(60 * 60);
                    }

                    if(IsDay)
                    {
                        CurrentIntervalStart = QDateTime(now.date(),QTime(0,0));
                        CurrentIntervalEnd = CurrentIntervalStart.addSecs(24 * 60 * 60);
                    }

                    if(IsWeek)
                    {
                        int Offset = now.date().dayOfWeek() - 1;
                        CurrentIntervalStart = QDateTime(now.date().addDays(-Offset),QTime(0,0));
                        CurrentIntervalEnd = CurrentIntervalStart.addSecs(7 * 24 * 60 * 60);
                    }

                    if(IsMonth)
                    {
                        CurrentIntervalStart = QDateTime(QDate(now.date().year(),now.date().month(),1),QTime(0,0));
                        CurrentIntervalEnd = CurrentIntervalStart.addMonths(1);
                    }

                    CanGenerateItemsForCurrentInterval = true;


                }else
                {
                    CurrentIntervalEnd = _Task.GetDate("current_interval_end");
                    CurrentIntervalStart = _Task.GetDate("current_interval_start");
                    for(QString ItemString : _Task.GetString("run_interval_list").split(","))
                    {
                        Items.append(QDateTime::fromMSecsSinceEpoch(ItemString.toLongLong()));
                    }
                    CanGenerateItemsForCurrentInterval = false;
                }
                int Index = 0;
                bool JobDone = false;
                while(true)
                {
                    //Check if too many intervals and no success
                    //or if current interval is greater then allowed
                    if(
                            Index > 10000 ||
                            (!_Schedule.IsDateInvalid("date_end") && CurrentIntervalStart > _Schedule.GetDate("date_end"))
                       )
                    {
                        IsArchived = true;
                        _Task.Set("status","archived");
                        _Task.Set("run_interval_list","");
                        _Task.SetDateInvalid("current_interval_start");
                        _Task.SetDateInvalid("current_interval_end");
                        break;
                    }




                    if(CanGenerateItemsForCurrentInterval)
                    {
                        //Generate items
                        QJsonArray JsonSections = _Schedule.GetArray(QString("sections_") + ScheduleType);

                        for(QJsonValueRef SectionValue:JsonSections)
                        {
                            QJsonObject SectionObject = SectionValue.toObject();
                            int StartOffset = ObjectToInterval(SectionObject.value("start").toObject());
                            int EndOffset = ObjectToInterval(SectionObject.value("end").toObject());
                            int Probability = SectionObject.value("probability").toInt();
                            if(CheckProbability(Probability))
                            {
                                int Value = GetRanomBetween(StartOffset, EndOffset);
                                QDateTime InsertDate = CurrentIntervalStart.addSecs(Value);
                                if(InsertDate >= CurrentIntervalStart && InsertDate <= CurrentIntervalEnd)
                                {
                                   /*Items.append(CurrentIntervalStart.addSecs(StartOffset));
                                   Items.append(CurrentIntervalStart.addSecs(EndOffset));*/
                                   Items.append(InsertDate);
                                }
                            }

                        }

                        CanGenerateItemsForCurrentInterval = false;
                        std::sort(Items.begin(),Items.end());
                        continue;
                    }


                    while(!Items.isEmpty())
                    {
                        /*QDateTime ItemStart = Items.takeFirst();
                        QDateTime ItemEnd = Items.takeFirst();*/
                        QDateTime ItemDate = Items.takeFirst();
                        if(
                                now <= ItemDate &&
                                (_Schedule.IsDateInvalid("date_start") || ItemDate >= _Schedule.GetDate("date_start")) &&
                                (_Schedule.IsDateInvalid("date_end") || ItemDate <= _Schedule.GetDate("date_end"))
                           )
                        {
                            //Item match
                            JobDone = true;

                            QStringList ItemsToSave;
                            for(QDateTime ItemDate: Items)
                            {
                                ItemsToSave.append(QString::number(ItemDate.toMSecsSinceEpoch()));
                            }
                            _Task.Set("run_interval_list",ItemsToSave.join(","));
                            _Task.Set("current_interval_start", CurrentIntervalStart);
                            _Task.Set("current_interval_end", CurrentIntervalEnd);

                            IsScheduled = true;
                            ScheduledDate = ItemDate;
                            _Task.Set("run_next_date",ItemDate);
                            _Task.Set("status","scheduled_wait_for_execution");
                            break;
                        }
                    }

                    if(JobDone)
                        break;
                    else
                    {
                        while(true)
                        {
                            //Get next interval
                            if(IsHour)
                            {
                                CurrentIntervalStart = CurrentIntervalStart.addSecs(60 * 60);
                                CurrentIntervalEnd = CurrentIntervalEnd.addSecs(60 * 60);
                            }

                            if(IsDay)
                            {
                                CurrentIntervalStart = CurrentIntervalStart.addSecs(24 * 60 * 60);
                                CurrentIntervalEnd = CurrentIntervalEnd.addSecs(24 * 60 * 60);
                            }

                            if(IsWeek)
                            {
                                CurrentIntervalStart = CurrentIntervalStart.addSecs(7 * 24 * 60 * 60);
                                CurrentIntervalEnd = CurrentIntervalEnd.addSecs(7 * 24 * 60 * 60);
                            }

                            if(IsMonth)
                            {
                                CurrentIntervalStart = CurrentIntervalStart.addMonths(1);
                                CurrentIntervalEnd = CurrentIntervalEnd.addMonths(1);
                            }

                            if(_Schedule.IsDateInvalid("date_start") || CurrentIntervalEnd >= _Schedule.GetDate("date_start"))
                            {
                                break;
                            }
                        }


                        Index++;
                        CanGenerateItemsForCurrentInterval = true;

                    }

                }
            }




            if(IsArchived)
            {
                _Logger->Add(
                            QString("Archived"),
                            QString("Normal"),
                            QString("Normal"),
                            QString("TaskArchived"),
                            _Task.GetString("id"),
                            "task_id",
                            _Task.GetString("name"),
                            "task_name");
            }

            if(IsScheduled)
            {
                _Logger->Add(
                            QString(QString("Scheduled at <date>")),
                            QString("Normal"),
                            QString("Normal"),
                            QString("TaskScheduled"),
                            _Task.GetString("id"),
                            "task_id",
                            _Task.GetString("name"),
                            "task_name",
                            QString::number(ScheduledDate.toMSecsSinceEpoch()),
                            "date_replace");
            }
        }
        TaskStatus = _Task.GetString("status");
        if(TaskStatus == "scheduled_wait_for_execution")
        {
            QDateTime WhenStart = _Task.GetDate("run_next_date");
            if(now >= WhenStart)
            {
                IsUpdated = true;
                _Logger->Add(
                    QString("Starting ..."),
                    QString("Normal"),
                    QString("Normal"),
                    QString("TaskStarting"),
                    _Task.GetString("id"),
                    "task_id",
                    _Task.GetString("name"),
                    "task_name");

                RunTask(_Task);
            }
        }
        TaskStatus = _Task.GetString("status");
        if(TaskStatus == "executing")
        {
            if(_Task.GetScheduleList().length() != 1)
                continue;

            IsUpdated = true;
            Schedule _Schedule = _Task.GetScheduleList().at(0);

            QDateTime StartTime = _Task.GetDate("run_last_date");
            int MaxRunningTime = _Schedule.GetInt("max_running_time",0);
            if(MaxRunningTime > 0 && StartTime.addSecs(MaxRunningTime) < now)
            {
                _Logger->Add(
                            QString("Timed out, stopping ..."),
                            QString("Normal"),
                            QString("Error"),
                            QString("TaskTimeout"),
                            _Task.GetString("id"),
                            "task_id",
                            _Task.GetString("name"),
                            "task_name");
                _ApiScript->TerminateProcess(Id);
            }

        }
    }
    if(IsUpdated)
    {
        _TaskPersist->SaveAll();
    }
}

void TaskManager::RunTask(Task &T)
{
    QDateTime now = QDateTime::currentDateTime();

    T.Set("status","downloading");
    T.Set("run_last_date", now);
    T.SetDateInvalid("end_last_date");
    T.SetDateInvalid("run_next_date");
    T.Set("success_total", 0);
    T.Set("fail_total", 0);
    T.Set("success_current", 0);
    T.Set("fail_current", 0);
    T.Set("threads", 0);
    T.Set("browsers_count", 0);
    T.Set("log_path", "");
    for(int i = 1;i<=9;i++)
    {
        T.Set(QString("result") + QString::number(i) + QString("_path"), "");
        T.Set(QString("result") + QString::number(i) + QString("_name"), "");
        T.Set(QString("result") + QString::number(i) + QString("_active"), "");
        T.Set(QString("result") + QString::number(i) + QString("_number"), 0);
    }


    T.Set("report", "");



    QStringList Params;
    if(!T.GetString("script_name").isEmpty())
    {
        //Sending remote script
        Params<<"--silent"<<"--remote"<<"--script"<<T.GetString("script_name")<<"--username"<<T.GetString("script_login")<<"--password"<<T.GetString("script_pass")<<"--scheduler-key"<<T.GetString("id");
    }else if(!T.GetString("script_data").isEmpty())
    {
        //Sending local script

        QDir("temp").mkpath(".");
        QCryptographicHash hash(QCryptographicHash::Sha256);
        hash.addData(T.GetString("script_data").toUtf8());
        QString HashString = QString::fromUtf8(hash.result().toHex());

        QFile Data(QString("temp/") + HashString.mid(0,8) + QString(".data.xml"));
        if(!Data.exists())
        {
            if(Data.open(QIODevice::WriteOnly))
            {
                Data.write(T.GetString("script_data").toUtf8());
                Data.close();
            }
        }


        Params<<"--silent"<<"--script"<<QFileInfo(Data).absoluteFilePath()<<"--scheduler-key"<<T.GetString("id");
    }

    if(!T.GetString("settings").isEmpty())
    {
        QDir("temp").mkpath(".");
        QFile Actual(QString("temp/") + T.GetString("id") + QString(".xml"));
        if(Actual.open(QIODevice::WriteOnly))
        {
            Actual.write(T.GetString("settings").toUtf8());
            Actual.close();
        }
        Params<<"--actual"<<QFileInfo(Actual).absoluteFilePath();
    }

   _ApiScript->RunProcess(T.GetString("id"),Params);
}

void TaskManager::TaskRemove(QString Id)
{
    _TaskPersist->Remove(Id);
}

void TaskManager::TaskClearAll()
{
    _Logger->Add(
                QString("Removed all"),
                QString("Normal"),
                QString("Error"),
                QString("ArchivedDeleted"));

    for(QString& Id : _TaskPersist->FindAllIds())
    {
        _ApiScript->TerminateProcess(Id);
    }
    _TaskPersist->ClearAll();
}

void TaskManager::TaskRemoveArchived()
{
    _Logger->Add(
                QString("Removed all archived"),
                QString("Normal"),
                QString("Error"),
                QString("ArchivedDeleted"));

    _TaskPersist->ClearArchived();
}

void TaskManager::SetTaskPersist(TaskPersist *_TaskPersist)
{
    this->_TaskPersist = _TaskPersist;
}

void TaskManager::SetStream(Stream *_Stream)
{
    this->_Stream = _Stream;
    connect(_Stream,SIGNAL(OnConnection(QWebSocket*)),this,SLOT(OnConnection(QWebSocket*)));

}

void TaskManager::OnConnection(QWebSocket * Socket)
{
    QString Res = "{\"type\": \"summary\", \"params\": {\"tasks\": [\n";
    QStringList Content;
    for(QString& Id: _TaskPersist->FindAllIds())
    {
        Task & T = _TaskPersist->Find(Id);
        Content.append(T.Serialize());
    }
    Res += Content.join(",\n");
    Res += "\n],\"scheduler_log\": [\n";
    Content.clear();
    for(LogItem & I: _Logger->FindAll())
    {
        Content.append(I.Serialize());
    }
    Res += Content.join(",\n");
    Res += "\n],\"application_log\": [\n";
    Content.clear();
    for(LogItemApplication & I: _Logger->FindAllApplication())
    {
        Content.append(I.Serialize());
    }
    Res += Content.join(",\n");
    Res += "\n],\"manual_actions\": ";
    Res += _ManualActions->Serialize();
    Res += "}}";

    this->_Stream->SendSocketRaw(Socket,Res);
}

void TaskManager::SetLogger(Logger *_Logger)
{
    this->_Logger = _Logger;
}

void TaskManager::SetWebApplication(WebApplication *_WebApplication)
{
    this->_WebApplication = _WebApplication;
}

void TaskManager::Start()
{
    QDir("temp").removeRecursively();
    _TaskPersist->LoadAll();
    _Logger->LoadAll(true);
    _ApiExternal->Start(11293);
    _Stream->Start(11294);
    _WebApplication->Start(11295);
    _ApiScript->Start();

    for(QString& Id : _TaskPersist->FindAllIds())
    {
        Task &_Task = _TaskPersist->Find(Id);
        UpdateTaskAfterFinished(_Task);
    }
    _TaskPersist->SaveAll();

}
