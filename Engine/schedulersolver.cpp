#include "schedulersolver.h"
#include "every_cpp.h"
#include <QMap>

SchdulerSolver::SchdulerSolver(QObject *parent) :
    ISolver(parent)
{
    SchedulerConnector = 0;
}

QString SchdulerSolver::GetRandomString()
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

void SchdulerSolver::SetSchdulerConnector(ISchedulerConnector *SchedulerConnector)
{
    this->SchedulerConnector = SchedulerConnector;
    connect(SchedulerConnector,SIGNAL(OnMessage(QString,QString,QMap<QString,QString>)),this,SLOT(ReceivedMessage(QString,QString,QMap<QString,QString>)));
}

void SchdulerSolver::ReceivedMessage(QString TagName, QString Content, QMap<QString,QString> Params)
{

    if(TagName == "Manual")
    {

        QString Type = Params.value("Type");

        if(Type == "Text" || Type == "Image")
        {
            QString Id = Params["Id"];

            QString Text = Content;
            bool Success = Params["Success"] == "true";

            emit Done(Text,Id,Success,"0");
        }
    }

}

bool SchdulerSolver::TimeLimited()
{
    return false;
}
QString SchdulerSolver::Solve(const QString& base64,const QStringList& props)
{
    QString Id = GetRandomString();
    if(base64.startsWith("BAS_USER_ASK"))
    {
        QString data = base64;
        data = data.remove(0,12);
        SchedulerConnector->MessageSendFormat("Manual",data,"Id",Id,"Type","Text");
    }else
    {
        SchedulerConnector->MessageSendFormat("Manual",base64,"Id",Id,"Type","Image");
    }

    return Id;
}
void SchdulerSolver::ReportBad(const QString& id)
{

}
