#include "taskpersist.h"
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QTextStream>

TaskPersist::TaskPersist(QObject *parent) : QObject(parent)
{

}

void TaskPersist::SetStream(Stream* _Stream)
{
    this->_Stream = _Stream;
}

void TaskPersist::SetManualActions(ManualActions *_ManualActions)
{
    this->_ManualActions = _ManualActions;
}

Task& TaskPersist::Find(const QString& Id)
{
    return Tasks[Id];
}

bool TaskPersist::Contains(const QString& Id)
{
    return Tasks.contains(Id);
}

void TaskPersist::Insert(Task T)
{
    Tasks.insert(T.GetString("id"),T);
    SaveAll();
}

void TaskPersist::Remove(const QString& Id)
{
    Tasks.remove(Id);
    SaveAll();
    QJsonObject Params;
    Params.insert("id",Id);
    _Stream->Send("remove_task",Params);
    _ManualActions->ScriptFinished(Id);

}

void TaskPersist::ClearAll()
{
    for(QString & Id: Tasks.keys())
    {
        QJsonObject Params;
        Params.insert("id",Id);
        _Stream->Send("remove_task",Params);
        _ManualActions->ScriptFinished(Id);
    }
    Tasks.clear();
    SaveAll();
}

void TaskPersist::ClearArchived()
{
    for (auto it = Tasks.begin(); it != Tasks.end();)
        if (it.value().GetString("status") == "archived")
        {
            QJsonObject Params;
            Params.insert("id",it.key());
            _Stream->Send("remove_task",Params);
            _ManualActions->ScriptFinished(it.key());
            it = Tasks.erase(it);
        }
        else
            ++it;
    SaveAll();
}

QList<QString> TaskPersist::FindAllIds()
{
    return Tasks.keys();
}

/*void TaskPersist::Update(const QString& Id, Task T)
{
    T.Set("id",Id);
    if(Contains(Id))
        Find(Id).UpdateWith(T);
    SaveAll();
}*/

void TaskPersist::SaveAll()
{
    QDir("../data").mkpath(".");

    QDirIterator it("../data", QDir::Files | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    while(it.hasNext())
    {
        QString file = it.next();
        QString Id = QFileInfo(file).baseName();
        if(!Contains(Id))
        {
            QFile(file).remove();
        }
    }

    QMap<QString,Task>::iterator i;
    for (i = Tasks.begin(); i != Tasks.end(); ++i)
    {
        Task &t = i.value();
        if(t.GetIsDirty())
        {
            QString Data = t.Serialize();
            QFile file("../data/" + t.GetString("id") + QString(".json"));
            if(file.open(QIODevice::WriteOnly))
            {
                file.write(Data.toUtf8());
                file.close();
            }
        }

        if(t.GetIsNew())
        {
            _Stream->Send("add_task",t.GetData());
        }else if(t.GetIsDirty())
        {
            _Stream->Send("update_task",t.PrepareDirtyParams());
        }
        t.ClearDirtyAndNew();
    }

}

void TaskPersist::LoadAll()
{
    Tasks.clear();
    QDir("../data").mkpath(".");

    QDirIterator it("../data", QDir::Files | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    while(it.hasNext())
    {
        QString file = it.next();
        QFile File(file);
        if(File.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream in(&File);
            in.setCodec("UTF-8");
            QString Data = in.readAll();
            File.close();
            Task T = Task::Parse(Data, false);
            Tasks.insert(T.GetString("id"),T);
        }

    }

}

