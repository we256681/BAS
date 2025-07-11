#ifndef WEBDRIVERPROFILEREMOVER_H
#define WEBDRIVERPROFILEREMOVER_H

#include <QObject>
#include "ihttpclient.h"

using namespace BrowserAutomationStudioFramework;

class WebDriverProfileRemover : public QObject
{
    Q_OBJECT
    IHttpClient * HttpClient;
    QString SessionId;
    QString MLAKey;
    int Port;
    QString ProfileId;
public:
    explicit WebDriverProfileRemover(QObject *parent = nullptr);
    void Remove(IHttpClient * HttpClient,const QString& SessionId,const QString& ProfileId, int Port, const QString& MLAKey);
    void Log(const QString& Text,const QString& Arg1,const QString& Arg2 = QString(),const QString& Arg3 = QString());


signals:
    void Removed();
private slots:
    void SessionRemovedResult();
    void Final();
};

#endif // WEBDRIVERPROFILEREMOVER_H
