#include "webdriverprofileremover.h"
#include <QSettings>
#include <QFile>
#include <QTimer>
#include <QTextStream>

WebDriverProfileRemover::WebDriverProfileRemover(QObject *parent) : QObject(parent)
{

}

void WebDriverProfileRemover::Log(const QString& Text,const QString& Arg1,const QString& Arg2,const QString& Arg3)
{
    QFile OutFile("web_driver.txt");
    OutFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&OutFile);
    QString Res;
    bool c1 = Text.contains("%1");
    bool c2 = Text.contains("%2");
    bool c3 = Text.contains("%3");
    if(c1 && c2 && c3)
    {
        Res = Text.arg(Arg1).arg(Arg2).arg(Arg3);
    }else if(c1 && c2)
    {
        Res = Text.arg(Arg1).arg(Arg2);
    }else if(c1)
    {
        Res = Text.arg(Arg1);
    }else
    {
        Res = Text;
    }


    ts << "WebDriverProcessComunicatorFactory " << Res << endl;
    OutFile.close();
}

void WebDriverProfileRemover::Remove(IHttpClient * HttpClient,const QString& SessionId,const QString& ProfileId, int Port, const QString& MLAKey)
{
    this->HttpClient = HttpClient;
    this->SessionId = SessionId;
    this->MLAKey = MLAKey;
    this->HttpClient->setParent(this);
    this->HttpClient->Connect(this,SLOT(SessionRemovedResult()));
    this->Port = Port;
    this->ProfileId = ProfileId;

    QHash<QString,QString> p;
    p["data"] = "{\"url\":\"https://local.app.multiloginapp.com/api/close\"}";

    PostOptions Options;
    Options.Method = "POST";
    Options.PrepareStrategy = "custom";
    HttpClient->AddHeader("Content-Type","application/json");

    QString url = QString("http://127.0.0.1:%1/session/%2/url").arg(QString::number(Port)).arg(SessionId);

    //Log("-> [%1][%2] %3", "GET", url, p["data"]);

    this->HttpClient->Post(url, p, Options);
}

void WebDriverProfileRemover::SessionRemovedResult()
{
    //Log("<- [%1] %2", HttpClient->GetLastUrl(), HttpClient->GetContent());
    QTimer::singleShot(3000,this,SLOT(Final()));
}

void WebDriverProfileRemover::Final()
{
    emit Removed();
    deleteLater();
}

