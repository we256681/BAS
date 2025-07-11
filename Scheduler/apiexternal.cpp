#include "apiexternal.h"
#include <QDebug>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStorageInfo>


ApiExternal::ApiExternal(QObject *parent) : HttpRequestHandler(parent)
{
    Settings = new QSettings("settings.ini", QSettings::IniFormat, this);
}

/*void ApiExternal::SetTaskPersist(TaskPersist *_TaskPersist)
{
    this->_TaskPersist = _TaskPersist;
}

void ApiExternal::SetManualActions(ManualActions *_ManualActions)
{
    this->_ManualActions = _ManualActions;
}

void ApiExternal::SetLogger(Logger *_Logger)
{
    this->_Logger = _Logger;
}*/

void ApiExternal::SetSchedulePredict(SchedulePredict *_SchedulePredict)
{
    this->_SchedulePredict = _SchedulePredict;
}

void ApiExternal::SetRunManager(RunManager * _RunManager)
{
    this->_RunManager = _RunManager;
}

void ApiExternal::service(HttpRequest &request, HttpResponse &response)
{
    QString Path = QString::fromUtf8(request.getPath());
    QString Data = QString::fromUtf8(request.getBody());
    QString Method = QString::fromUtf8(request.getMethod()).toLower();

    response.setHeader("Access-Control-Allow-Origin", "*");
    response.setHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, PATCH, OPTIONS");
    response.setHeader("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept-Datetime, Upgrade-Insecure-Requests, Authorization, Cache-Control, If-Match, If-Modified-Since, If-None-Match, If-Range, If-Unmodified-Since, Max-Forwards, Range, Pragma, X-Requested-With, DNT, X-HTTP-Method-Override, X-Csrf-Token, X-Request-ID");

    if(Path.startsWith("/tasks/insertarchived") && Method == "post")
    {
        emit TaskInsert(Data, true);
        response.write("true",true);
        return;
    }else if(Path.startsWith("/check_update") && Method == "get")
    {
        response.write(_RunManager->CheckUpdateReady().toUtf8(),true);
        return;
    }else if(Path.startsWith("/restart") && Method == "get")
    {
        _RunManager->Restart();
        return;
    }else if(Path.startsWith("/uninstall") && Method == "get")
    {
        _RunManager->Uninstall();
        return;
    }else if(Path.startsWith("/tasks/insert") && Method == "post")
    {
        emit TaskInsert(Data, false);
        response.write("true",true);
        return;
    }else if(Path.startsWith("/manual/answer") && Method == "post")
    {
        QString Id = request.getParameter("id");
        QString Data = request.getParameter("data");
        QString Success = request.getParameter("success");
        emit ManualAnswer(Id,Data,Success == "true");
        response.write("true",true);
        return;
    }else if(Path.startsWith("/tasks/updatesoft") && Method == "post")
    {
        QString Id = Path.replace("/tasks/updatesoft/","");
        emit TaskUpdate(Id,Data,false);
        response.write("true",true);
        return;
    }else if(Path.startsWith("/tasks/updatehard") && Method == "post")
    {
        QString Id = Path.replace("/tasks/updatehard/","");
        emit TaskUpdate(Id,Data,true);
        response.write("true",true);
        return;
    }
    else if(Path.startsWith("/tasks/predict") && Method == "post")
    {
        QJsonObject ScheduleObject = QJsonDocument::fromJson(request.getBody()).object();
        PredictData Data = _SchedulePredict->Predict(ScheduleObject);
        QJsonObject ResultObject;
        QJsonArray DateList;
        for(QString& DateString:Data.Data)
        {
            DateList.append(DateString);
        }

        ResultObject.insert("list", DateList);
        ResultObject.insert("has_more", Data.HasMore);
        ResultObject.insert("success", Data.IsSuccess);
        ResultObject.insert("message", Data.Message);

        QJsonDocument doc(ResultObject);
        response.write(doc.toJson(),true);
        return;
    }
    else if(Path.startsWith("/tasks/stop") && Method == "get")
        {
            QString Id = Path.replace("/tasks/stop/","");
            emit TaskStop(Id);
            response.write("true",true);
            return;
        }
    else if(Path.startsWith("/tasks/cancel") && Method == "get")
        {
            QString Id = Path.replace("/tasks/cancel/","");
            emit TaskCancel(Id);
            response.write("true",true);
            return;
        }
    else if(Path.startsWith("/tasks/start") && Method == "get")
        {
            QString Id = Path.replace("/tasks/start/","");
            emit TaskStart(Id);
            response.write("true",true);
            return;
        }
    else if(Path.startsWith("/tasks/resources_edit_local") && Method == "post")
        {
            QString Id = Path.replace("/tasks/resources_edit_local/","");
            QString Script = request.getParameter("script");
            QString Actual = request.getParameter("actual");
            emit StartResourceEditLocal(Id,Script,Actual);
            response.write("true",true);
            return;
        }
    else if(Path.startsWith("/tasks/resources_edit_remote") && Method == "post")
        {
            QString Id = Path.replace("/tasks/resources_edit_remote/","");
            QString ScriptName = request.getParameter("script_name");
            QString ScriptLogin = request.getParameter("script_login");
            QString ScriptPass = request.getParameter("script_pass");
            QString Actual = request.getParameter("actual");
            emit StartResourceEditRemote(Id,ScriptName,ScriptLogin,ScriptPass,Actual);
            response.write("true",true);
            return;
        }
    else if(Path.startsWith("/tasks/togglevisibility") && Method == "get")
        {
            QString Id = Path.replace("/tasks/togglevisibility/","");
            emit TaskToggleVisibility(Id);
            response.write("true",true);
            return;
        }
    else if(Path.startsWith("/tasks/archive") && Method == "get")
        {
            QString Id = Path.replace("/tasks/archive/","");
            emit TaskArchive(Id);
            response.write("true",true);
            return;
        }
    else if(Path.startsWith("/tasks/restore") && Method == "get")
        {
            QString Id = Path.replace("/tasks/restore/","");
            emit TaskRestore(Id);
            response.write("true",true);
            return;
        }
    else if(Path.startsWith("/tasks/delete") && Method == "get")
        {
            QString Id = Path.replace("/tasks/delete/","");
            emit TaskDelete(Id);
            response.write("true",true);
            return;
        }

    else if(Path.startsWith("/tasks/report") && Method == "get")
    {
        QString Id = Path.replace("/tasks/report/","");
        emit TaskReport(Id);
        response.write("true",true);
        return;
    }else if(Path.startsWith("/tasks/clearall") && Method == "get")
    {
        emit TaskClearAll();
        response.write("true",true);
        return;
    }else if(Path.startsWith("/tasks/remove_archived") && Method == "get")
    {
        emit TaskRemoveArchived();
        response.write("true",true);
        return;
    }else if(Path.startsWith("/scheduler_log/clear") && Method == "get")
    {
        //_Logger->ClearAll();
        emit ClearSchedulerLog();
        response.write("true",true);
        return;
    }else if(Path.startsWith("/application_log/clear") && Method == "get")
    {
        //_Logger->ClearAllApplication();
        emit ClearApplicationLog();
        response.write("true",true);
        return;
    }/*else if(Path.startsWith("/summary") && Method == "get")
    {
        QString Res = "{\"tasks\": [\n";
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
        Res += "}";

        response.setHeader(QString("Content-Type").toUtf8(),QString("application/json; charset=utf-8").toUtf8());

        response.write(Res.toUtf8(),true);
        return;
    }*/else if(Path.startsWith("/download") && Method == "get")
    {
        QString FilePath = request.getParameter(QString("file").toUtf8());
        QFile File(FilePath);

        if(!File.open(QIODevice::ReadOnly))
        {
            response.write("File not found",true);
            return;
        }
        QByteArray Data = File.readAll();
        File.close();
        //QString Name = QFileInfo(FilePath).fileName();
        //response.setHeader(QString("Content-Type").toUtf8(),QString("application/octet-stream").toUtf8());
        //response.setHeader(QString("Content-Disposition").toUtf8(),(QString("attachment; filename=\"") + Name + QString("\"")).toUtf8());
        response.write(Data, true);
        return;

    }else if(Path.startsWith("/get_scheduler_log") && Method == "get")
    {
        QStringList Content;
        Logger _L;
        _L.LoadAll(false);

        for(LogItem & I: _L.FindAll())
        {
            Content.append(I.Param2 + QString(" [") + I.Date.toString("dd.MM.yyyy hh:mm:ss") + QString("] ") + I.Message);
        }


        /*response.setHeader(QString("Content-Type").toUtf8(),QString("application/octet-stream").toUtf8());
        response.setHeader(QString("Content-Disposition").toUtf8(),(QString("attachment; filename=\"log.txt\"")).toUtf8());*/
        QString Res = Content.join("\n");
        response.write(Res.toUtf8(), true);

        return;

    }else if(Path.startsWith("/filesystem") && Method == "post")
    {
        QStringList PathList;
        if(!request.getParameter("path").isEmpty())
            PathList = QString::fromUtf8(request.getParameter("path")).split("|");

        QJsonArray Result;

        if(PathList.isEmpty())
        {
            for(const QStorageInfo &storage: QStorageInfo::mountedVolumes())
            {
                if (storage.isValid() && storage.isReady() && !storage.isReadOnly())
                {
                    QJsonObject ToAdd;
                    ToAdd.insert("is_folder", true);
                    ToAdd.insert("name", storage.rootPath().split("/").first());
                    Result.append(ToAdd);
                }
            }
        }else
        {
            QString Folder = PathList.join("/") + QString("/");
            //qDebug()<<Folder;
            QDir directory(Folder);

            QStringList items = directory.entryList(QDir::AllDirs | QDir::NoDot | QDir::NoDotDot);
            for(QString& item: items)
            {
                QJsonObject ToAdd;
                ToAdd.insert("is_folder", true);
                ToAdd.insert("name", item);
                Result.append(ToAdd);
            }
            items = directory.entryList(QDir::Files | QDir::NoDot | QDir::NoDotDot);
            for(QString& item: items)
            {
                QJsonObject ToAdd;
                ToAdd.insert("is_folder", false);
                ToAdd.insert("name", item);
                Result.append(ToAdd);
            }
        }

        QJsonDocument doc(Result);
        response.write(doc.toJson(),true);

        return;
    }




    response.write("false",true);
}

void ApiExternal::Start(int Port)
{
    Settings->setValue("port", Port);
    new HttpListener(Settings,this,this);
}
