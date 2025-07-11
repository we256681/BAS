#include "apiscript.h"
#include <QXmlStreamReader>
#include <QRegularExpression>
#include <windows.h>

ApiScript::ApiScript(QObject *parent) : QObject(parent)
{

}


void ApiScript::Start()
{
    Server = new QLocalServer(this);
    connect(Server,SIGNAL(newConnection()),this,SLOT(NewConnection()));
    Server->listen(QString("\\\\.\\pipe\\basscheduler"));
}

void ApiScript::RunProcess(QString Id,QStringList CommandLine)
{
    TaskStartQueue[Id] = CommandLine;
}

void ApiScript::SetLogger(Logger * _Logger)
{
    this->_Logger = _Logger;
}

void ApiScript::Timer()
{
    if(!TaskStartingId.isEmpty())
    {
        QDateTime TaskStartingDateCopy = TaskStartingDate;
        TaskStartingDateCopy = TaskStartingDateCopy.addSecs(60 * 5);
        if(QDateTime::currentDateTime() > TaskStartingDateCopy)
        {
            TaskStartingId.clear();
        }
    }
    if(TaskStartingId.isEmpty() && !TaskStartQueue.isEmpty())
    {
        QString Id = TaskStartQueue.firstKey();
        QStringList CommandLine = TaskStartQueue.first();
        TaskStartingId = Id;
        TaskStartingDate = QDateTime::currentDateTime();
        TaskStartQueue.remove(Id);

        //Start task
        QProcess *Process = new QProcess(this);
        Process->setProperty("id", Id);
        connect(Process,SIGNAL(finished(int)),this,SLOT(ProcessFinished()));
        connect(Process,SIGNAL(readyRead()),this,SLOT(ReadProcessOutput()));
        connect(Process,SIGNAL(finished(int)),Process,SLOT(deleteLater()));
        Process->setWorkingDirectory("app");
        StartingProcesses.append(Process);
        Process->start("app/RemoteExecuteScript.exe",CommandLine);
    }
}

void ApiScript::ReadProcessOutput()
{
    QProcess *Process = qobject_cast<QProcess *>(sender());
    QString Id = Process->property("id").toString();
    QString Data = QString::fromUtf8(Process->peek(10 * 1024 * 1024));
    if(Data.contains("PID="))
    {
        QRegularExpression RegexpPid("PID\\=\\s*(\\d+)");
        QRegularExpressionMatch Match = RegexpPid.match(Data);
        if(Match.hasMatch())
        {
            WorkingProcessesPids[Id] = Match.captured(1);
        }

    }
}

void ApiScript::RemoveFromStartQueue(const QString& Id)
{
    TaskStartQueue.remove(Id);
    if(TaskStartingId == Id)
    {
        TaskStartingId.clear();
    }
}


void ApiScript::ProcessFinished()
{
    QProcess *Process = qobject_cast<QProcess *>(sender());
    StartingProcesses.removeAll(Process);
    QString Id = Process->property("id").toString();
    QString Output = QString::fromUtf8(Process->readAll());
    if(!Output.contains("Run!"))
    {
        emit OnFinished(Id,Output,false);
        RemoveFromStartQueue(Id);
    }
}

void ApiScript::TerminateProcess(QString Id)
{
    SendMessageFormat(Id,"Terminate");
    RemoveFromStartQueue(Id);
    if(WorkingProcessesPids.contains(Id))
    {
        QString Pid = WorkingProcessesPids[Id];
        std::wstring arg = L"/F /PID ";
        arg  += Pid.toStdWString();

        ShellExecute(
          NULL,
          NULL,
          L"taskkill",
          (wchar_t *)arg.data(),
          NULL,SW_HIDE
        );
    }
    for(QProcess* Process: StartingProcesses)
    {
        if(Id == Process->property("id").toString())
        {
            Process->kill();
        }
    }
    if(!FindLocalServerByKey(Id))
    {
        emit OnFinished(Id,QString(),true);
        WorkingProcessesPids.remove(Id);
    }
}

void ApiScript::SendMessageRaw(QString Id,QString Message)
{
    QLocalSocket *Client = FindLocalServerByKey(Id);
    if(Client)
    {
        Client->write(QString("<Messages>" + Message + "</Messages>").toUtf8());
        Client->flush();
    }
}

void ApiScript::SendMessageFormat(QString Id,const QString& TagName, const QString& Content,
                       const QString& AttributeName1, const QString& AttributeData1,
                       const QString& AttributeName2, const QString& AttributeData2,
                       const QString& AttributeName3, const QString& AttributeData3)
{
    QString WriteString;
    QXmlStreamWriter xmlWriter(&WriteString);
    xmlWriter.writeStartElement(TagName);
    if(!AttributeName1.isEmpty())
        xmlWriter.writeAttribute(AttributeName1,AttributeData1);
    if(!AttributeName2.isEmpty())
        xmlWriter.writeAttribute(AttributeName2,AttributeData2);
    if(!AttributeName3.isEmpty())
        xmlWriter.writeAttribute(AttributeName3,AttributeData3);

    if(!Content.isEmpty())
        xmlWriter.writeCharacters(Content);
    xmlWriter.writeEndElement();

    SendMessageRaw(Id,WriteString);
}

QLocalSocket * ApiScript::FindLocalServerByKey(const QString& Key)
{
    QHashIterator<QLocalSocket *,LocalServerData> i(Sockets);
    while (i.hasNext())
    {
        i.next();
        if(i.value()->key == Key)
            return i.key();
    }
    return 0;
}

void ApiScript::NewConnection()
{
    QLocalSocket *Client = Server->nextPendingConnection();

    if(!Client)
        return;

    connect(Client,SIGNAL(disconnected()),this,SLOT(Finished()));

    LocalServerData Data = std::make_shared<ApiScript::LocalServerDataClass>();

    Sockets[Client] = Data;

    connect(Client,SIGNAL(readyRead()),this,SLOT(Income()));

}

void ApiScript::Finished()
{
    QLocalSocket *Client = (QLocalSocket *)(sender());
    if(Client)
    {
        if(Sockets.contains(Client))
        {
            QString key = Sockets[Client]->key;
            WorkingProcessesPids.remove(key);
            RemoveFromStartQueue(key);
            emit OnFinished(key,QString(),true);
        }
        Sockets.remove(Client);
        Client->deleteLater();
    }
}

void ApiScript::ParseMessage(const QString& Id, const QString& Message)
{
    QString Content;
    QString TagName;
    QMap<QString,QString> Params;

    QXmlStreamReader xmlReader(Message);
    while(!xmlReader.atEnd() && !xmlReader.hasError())
    {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if(token == QXmlStreamReader::StartElement)
        {
            TagName = xmlReader.name().toString();
            foreach(QXmlStreamAttribute attr,xmlReader.attributes())
            {
                Params[attr.name().toString()] = attr.value().toString();
            }
            xmlReader.readNext();
            Content = xmlReader.text().toString();
        }
    }
    if (xmlReader.hasError())
    {
        return;
    }

    if(TagName == "ScriptStart")
    {
        RemoveFromStartQueue(Id);
        emit OnStarted(Id);
    }


    emit OnMessage(Id,TagName,Content,Params);
}

void ApiScript::Income()
{
    QLocalSocket *Client = (QLocalSocket *)(sender());

    if(!Sockets.contains(Client))
        return;

    LocalServerData Data = Sockets[Client];

    Data->data.append(Client->readAll());
    QString str = QString::fromUtf8(Data->data);

    {
        bool cont = true;
        bool changed = false;
        while(cont)
        {
            cont = false;
            int s = str.indexOf("<Key>");

            if(s>=0)
            {
                int e = str.indexOf("</Key>",s);
                if(e>=0)
                {
                    changed = true;
                    QString mid = str.mid(s+5,e-s+6 - 5 - 6);
                    str.remove(0,e+6);
                    Data->key = mid;


                    break;
                }
            }

        }
        if(changed)
        {
            Data->data = str.toUtf8();
        }
    }

    {
        bool cont = true;
        bool changed = false;
        while(cont)
        {
            cont = false;
            int s = str.indexOf("<Messages>");

            if(s>=0)
            {
                int e = str.indexOf("</Messages>",s);
                if(e>=0)
                {
                    changed = true;
                    QString mid = str.mid(s + 10,e-s-10);
                    str.remove(0,e+11);
                    //qDebug()<<mid;
                    ParseMessage(Data->key, mid);
                    //emit OnMessage(Data->key, "<Messages>" + mid + "</Messages>");
                    cont = true;
                }
            }

        }
        if(changed)
        {
            Data->data = str.toUtf8();
        }
    }

}
