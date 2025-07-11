#include "every_cpp.h"
#include "schedulerconnector.h"
#include <QTimer>
#include <QXmlStreamWriter>
#include <QCoreApplication>

namespace BrowserAutomationStudioFramework
{
    SchedulerConnector::SchedulerConnector(QObject *parent) : ISchedulerConnector(parent)
    {
    }

    void SchedulerConnector::Reset()
    {
        this->ReportDataVisualizer = 0;
        this->Worker = 0;
    }

    void SchedulerConnector::SetWorker(IMultiWorker *Worker)
    {
        this->Worker = Worker;
        if(Worker)
        {
            disconnect(Worker,SIGNAL(ThreadStarted()),this,SLOT(ThreadStarted()));
            disconnect(Worker,SIGNAL(ThreadFail(QString)),this,SLOT(ThreadFail()));
            disconnect(Worker,SIGNAL(ThreadSuccess()),this,SLOT(ThreadSuccess()));
            disconnect(Worker,SIGNAL(StageStatistic(int,int)),this,SLOT(StageStatistic(int,int)));
            connect(Worker,SIGNAL(ThreadStarted()),this,SLOT(ThreadStarted()));
            connect(Worker,SIGNAL(ThreadFail(QString)),this,SLOT(ThreadFail()));
            connect(Worker,SIGNAL(ThreadSuccess()),this,SLOT(ThreadSuccess()));
            connect(Worker,SIGNAL(StageStatistic(int,int)),this,SLOT(StageStatistic(int,int)));
        }
    }

    void SchedulerConnector::SetBrowserFactory(IBrowserFactory *BrowserFactory)
    {
        disconnect(BrowserFactory,SIGNAL(BrowserStart(IProcessComunicator*,int)),this,SLOT(BrowserStart(IProcessComunicator*,int)));
        disconnect(BrowserFactory,SIGNAL(BrowserStop(IProcessComunicator*,int)),this,SLOT(BrowserStop(IProcessComunicator*,int)));
        connect(BrowserFactory,SIGNAL(BrowserStart(IProcessComunicator*,int)),this,SLOT(BrowserStart(IProcessComunicator*,int)));
        connect(BrowserFactory,SIGNAL(BrowserStop(IProcessComunicator*,int)),this,SLOT(BrowserStop(IProcessComunicator*,int)));
    }

    void SchedulerConnector::MessageReceived(QString TagName, QString Content, QMap<QString,QString> Params)
    {
        emit OnMessage(TagName, Content, Params);

        if(TagName == "GetReport")
        {
            if(ReportDataVisualizer)
            {
                QString Res = ReportDataVisualizer->PrepareText();
                MessageSendFormat("Report",Res);
            }
        }
        if(TagName == "Terminate")
        {

            emit OnClose();
        }
        if(TagName == "Kill")
        {
            qApp->exit(0);
        }
        if(TagName == "VisibilityToggle")
        {
            emit OnVisibilityToggle();
        }
        if(TagName == "ShowBrowser")
        {
            int BrowserId = Params["Id"].toInt();
            if(CommunicatorMap.contains(BrowserId))
            {
                IProcessComunicator* ProcessComunicator = CommunicatorMap[BrowserId];
                if(ProcessComunicator)
                {
                    ProcessComunicator->OnShowBrowser();
                    ProcessComunicator->Send(QString("<Visible>1</Visible>"));
                }
            }

        }
    }

    void SchedulerConnector::SetReportDataVisualizer(IReportDataVisualizer *ReportDataVisualizer)
    {
        this->ReportDataVisualizer = ReportDataVisualizer;
    }

    void SchedulerConnector::StageStatistic(int MaxSuccess, int MaxFails)
    {
        MessageSendFormat("StageStatistic", QString(), QString("MaxSuccess"), QString::number(MaxSuccess), QString("MaxFails"), QString::number(MaxFails));
    }

    void SchedulerConnector::ThreadStarted()
    {
        MessageSendFormat("ThreadStarted", QString());
    }
    void SchedulerConnector::ThreadFail()
    {
        MessageSendFormat("ThreadFail", QString());
    }

    void SchedulerConnector::BrowserStart(IProcessComunicator* Communicator,int Index)
    {
        MessageSendFormat("BrowserStart", QString());

        int BrowserId = qrand() % 1000000 + 10;
        Communicator->setProperty("SchedulerBrowserId", BrowserId);
        CommunicatorMap[BrowserId] = Communicator;
        disconnect(Communicator,SIGNAL(Received(QString)),this,SLOT(Received(QString)));
        connect(Communicator,SIGNAL(Received(QString)),this,SLOT(Received(QString)));

    }
    void SchedulerConnector::BrowserStop(IProcessComunicator* Communicator,int Index)
    {
        MessageSendFormat("BrowserStop", QString());
        int BrowserId = Communicator->property("SchedulerBrowserId").toInt();
        CommunicatorMap.remove(BrowserId);
    }

    void SchedulerConnector::Received(const QString& value)
    {
        QXmlStreamReader xmlReader(value);
        while(!xmlReader.atEnd() && !xmlReader.hasError())
        {
            QXmlStreamReader::TokenType token = xmlReader.readNext();

            if(xmlReader.name() == "ManualControlIndicatorStart" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                QString Message = xmlReader.text().toString();
                IProcessComunicator* Communicator = qobject_cast<IProcessComunicator*>(sender());
                int BrowserId = Communicator->property("SchedulerBrowserId").toInt();
                MessageSendFormat("Manual",Message,"Id",QString::number(BrowserId),"Type","Browser");
                return;
            }

            if(xmlReader.name() == "ManualControlIndicatorStop" && token == QXmlStreamReader::StartElement)
            {
                IProcessComunicator* Communicator = qobject_cast<IProcessComunicator*>(sender());
                int BrowserId = Communicator->property("SchedulerBrowserId").toInt();
                MessageSendFormat("Manual",QString(),"Id",QString::number(BrowserId),"Type","BrowserStop");
                return;
            }
        }
    }


    void SchedulerConnector::ThreadSuccess()
    {
        MessageSendFormat("ThreadSuccess", QString());
    }

    void SchedulerConnector::StartIfNeeded(const QStringList& Params)
    {
        if(Params.contains("--scheduler-key"))
        {

            int index = Params.indexOf("--scheduler-key");
            if(index + 1 < Params.length())
            {
                QString SchedulerKey = Params[index + 1];
                Start(SchedulerKey);
            }
            if(!IsActive)
                exit(0);
        }
    }

    void SchedulerConnector::ConnectDelayed()
    {
        Socket->connectToServer(QString("\\\\.\\pipe\\basscheduler"));
    }

    void SchedulerConnector::Connected()
    {
        IsActive = true;
        ConnectLoop.quit();
    }

    void SchedulerConnector::ConnectTimeout()
    {
        //IsActive = false;
        ConnectLoop.quit();
    }

    void SchedulerConnector::Start(const QString& Id)
    {
        Socket = new QLocalSocket(this);
        connect(Socket, SIGNAL(readyRead()), this, SLOT(Income()));
        connect(Socket, SIGNAL(connected()), this, SLOT(Connected()));
        connect(Socket,SIGNAL(disconnected()), this, SIGNAL(OnClose()));
        QTimer::singleShot(100,this,SLOT(ConnectDelayed()));
        QTimer::singleShot(3000,this,SLOT(ConnectTimeout()));
        ConnectLoop.exec();
        if(IsActive)
            MessageSend(QString("<Key>") + Id + QString("</Key>"));

    }

    bool SchedulerConnector::GetIsActive()
    {
        return IsActive;
    }

    void SchedulerConnector::MessageSend(const QString& Message)
    {
        if(!IsActive)
            return;

        if(Socket)
        {
            Socket->write(QString("<Messages>" + Message + "</Messages>").toUtf8());
            Socket->flush();
        }
    }

    void SchedulerConnector::MessageSendFormat(const QString& TagName, const QString& Content,
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

        MessageSend(WriteString);
    }

    void SchedulerConnector::Income()
    {
        QLocalSocket *Client = (QLocalSocket *)(sender());

        Data.append(Client->readAll());
        QString str = QString::fromUtf8(Data);

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

                        ParseMessage(mid);
                        cont = true;
                    }
                }

            }
            if(changed)
            {
                Data = str.toUtf8();
            }
        }

    }

    void SchedulerConnector::ParseMessage(const QString& Message)
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

        MessageReceived(TagName,Content,Params);
    }

}
