#include "googledriveapi.h"
#include <QTimer>
#include <QFile>
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QClipboard>
#include <QApplication>
#include "o2google.h"
#include "o2requestor.h"
#include "o0settingsstore.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    GoogleDriveAPI::GoogleDriveAPI(QObject *parent) : QObject(parent)
    {

    }

    void GoogleDriveAPI::Init(const QString& ClientId,const QString& ClientSecret)
    {
        API = new O2Google(this);
        connect(API, SIGNAL(linkedChanged()), this, SLOT(LinkedChanged()));
        connect(API, SIGNAL(linkingFailed()), this, SLOT(LinkingFailed()));
        connect(API, SIGNAL(linkingSucceeded()), this, SLOT(LinkingSucceeded()));
        connect(API, SIGNAL(openBrowser(QString)), this, SLOT(OpenBrowser(QString)));
        connect(API, SIGNAL(closeBrowser()), this, SLOT(CloseBrowser()));

        API->setStore(new O0SettingsStore("2453507941", this));
        API->setClientId(ClientId);
        API->setClientSecret(ClientSecret);
        API->setScope("https://www.googleapis.com/auth/drive.file");

        Manager = new QNetworkAccessManager(this);
        Requestor = new O2Requestor(Manager, API, this);

        connect(Requestor,SIGNAL(finished(int,QNetworkReply::NetworkError,QByteArray)),this,SLOT(Finished(int,QNetworkReply::NetworkError,QByteArray)));
        Stage = 0;
    }
    void GoogleDriveAPI::UploadFile(const QString& FilePath,const QString& FileName)
    {
        Task = DoUploadFile;
        this->Data1 = FilePath;
        this->Data2 = FileName;

        API->link();
    }

    void GoogleDriveAPI::LinkedChanged()
    {

    }

    void GoogleDriveAPI::LinkingFailed()
    {
        emit Result(false,tr("Authorization error"));
    }

    void GoogleDriveAPI::LinkingSucceeded()
    {
        if(Task == DoUploadFile)
        {
            Stage = 0;
            QNetworkRequest Request(QUrl("https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart"));
            QString boundary("7d44e178b0439");
            Request.setHeader(QNetworkRequest::ContentTypeHeader, QString("multipart/related; boundary=") + boundary);


            QString DataString = QString("--") + boundary + QString("\r\n");

            DataString += QString("Content-Type: application/json; charset=UTF-8\r\n");
            DataString += QString("\r\n");
            QJsonObject o;
            o["name"] = Data2;
            QJsonDocument doc;
            doc.setObject(o);
            DataString += doc.toJson(QJsonDocument::Compact) + QString("\r\n");
            DataString += QString("\r\n");
            DataString += QString("--") + boundary + QString("\r\n");
            DataString += QString("Content-Type: application/xml\r\n");
            DataString += QString("\r\n");
            QFile File(Data1);
            File.open(QFile::ReadOnly | QFile::Text);
            QTextStream in(&File);
            in.setCodec("UTF-8");
            DataString += in.readAll();
            File.close();
            DataString += QString("\r\n");
            DataString += QString("--") + boundary + QString("--\r\n");

            Requestor->post(Request,DataString.toUtf8());
        }

    }

    void GoogleDriveAPI::ReportError(const QByteArray& Data)
    {
        QJsonDocument JsonDocument = QJsonDocument::fromJson(Data);
        QJsonObject JsonObject = JsonDocument.object();
        if(JsonObject.contains("error") && JsonObject["error"].toObject().contains("message"))
        {
            emit Result(false, QString(tr("API error : ")) + JsonObject["error"].toObject()["message"].toString());
        }
    }


    void GoogleDriveAPI::Finished(int id, QNetworkReply::NetworkError error, QByteArray data)
    {
        if(error != QNetworkReply::NoError)
        {
            ReportError(data);
            return;
        }


        if(Task == DoUploadFile)
        {
            if(Stage == 0)
            {
                Stage = 1;
                QJsonDocument JsonDocument = QJsonDocument::fromJson(data);
                QJsonObject JsonObject = JsonDocument.object();
                if(!JsonObject.contains("id"))
                {
                    emit Result(false, tr("Failed to obtain file id"));
                    return;
                }
                QString FileId = JsonObject["id"].toString();
                QNetworkRequest Request(QUrl(QString("https://www.googleapis.com/drive/v3/files/") + FileId + QString("/permissions")));
                Request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));

                Data3 = FileId;
                Requestor->post(Request,QString("{'role': 'reader','type': 'anyone'}").toUtf8());
                return;
            }

            if(Stage == 1)
            {
                Stage = 2;
                QString Url = QString("https://drive.google.com/uc?id=") + Data3 + QString("&export=download");
                emit Result(true, Url);
            }
        }
    }


    void GoogleDriveAPI::OpenBrowser(QString Url)
    {
        QDesktopServices::openUrl(QUrl(Url));
        emit SetStatus(tr("Browser is opened, you need to authorize BAS to upload files on your Google Drive."));
    }

    void GoogleDriveAPI::CloseBrowser()
    {
        emit SetStatus(tr("Project upload continues ..."));
    }
}
