#include "webapplication.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

WebApplication::WebApplication(QObject *parent) : HttpRequestHandler(parent)
{
    Settings = new QSettings("settings2.ini", QSettings::IniFormat, this);
}

void WebApplication::Start(int Port)
{
    Settings->setValue("port", Port);
    new HttpListener(Settings,this,this);
}


void WebApplication::service(HttpRequest& request, HttpResponse& response)
{
    QString Path = QString::fromUtf8(request.getPath());
    QString FilePath = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/gui/html/" + Path);
    if(!QFileInfo(FilePath).exists())
    {
        response.setStatus(404);
        response.write("",true);
        return;
    }


    QFile File(FilePath);
    if(!File.open(QIODevice::ReadOnly))
    {
        response.setStatus(404);
        response.write("",true);
        return;
    }
    QByteArray Data = File.readAll();
    File.close();

    response.setStatus(200);
    response.write(Data,true);
}
