#ifndef GOOGLEDRIVEAPI_H
#define GOOGLEDRIVEAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>


#include "engine_global.h"

class O2Google;
class O2Requestor;
namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT GoogleDriveAPI : public QObject
    {
        Q_OBJECT
        O2Google* API;
        QNetworkAccessManager *Manager;
        O2Requestor *Requestor;
        enum TaskEnum
        {
            DoUploadFile
        }Task;
        int Stage;
        QString Data1;
        QString Data2;
        QString Data3;
    public:
        explicit GoogleDriveAPI(QObject *parent = 0);
        void Init(const QString& ClientId,const QString& ClientSecret);
        void UploadFile(const QString& FilePath,const QString& FileName);
    signals:
        void Result(bool IsSuccess, QString LinkOrErrorMessage);
        void SetStatus(QString LinkOrErrorMessage);
    private slots:

        void LinkedChanged();
        void LinkingFailed();
        void LinkingSucceeded();
        void OpenBrowser(QString);
        void CloseBrowser();
        void Finished(int id, QNetworkReply::NetworkError error, QByteArray data);
        void ReportError(const QByteArray& Data);

    };
}

#endif // GOOGLEDRIVEAPI_H
