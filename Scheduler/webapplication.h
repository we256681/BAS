#ifndef WEBAPPLICATION_H
#define WEBAPPLICATION_H

#include <QObject>
#include "httplistener.h"
#include "httprequesthandler.h"

using namespace stefanfrings;

class WebApplication : public HttpRequestHandler
{
    Q_OBJECT
    HttpListener * Listener = 0;
    QSettings * Settings;

public:
    explicit WebApplication(QObject *parent = nullptr);
    void Start(int Port);
    void service(HttpRequest& request, HttpResponse& response);
signals:

public slots:
};

#endif // WEBAPPLICATION_H
