#include "ihttpclient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "every_cpp.h"
namespace BrowserAutomationStudioFramework
{
    IHttpClient::IHttpClient(QObject *parent) :
        QObject(parent)
    {
    }

    PostOptions::PostOptions()
    {
        ContentTypeEncoding = "UTF-8";
        PrepareStrategy = "multipart";
        Method = "POST";
    }

    GetOptions::GetOptions()
    {
        Method = "GET";
    }


}
