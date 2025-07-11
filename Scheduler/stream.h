#ifndef STREAM_H
#define STREAM_H

#include <QObject>
#include <QList>
#include <QJsonObject>
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"

class Stream : public QObject
{
    Q_OBJECT
    QWebSocketServer * Server = 0;
    QList<QWebSocket*> Connections;
public:
    explicit Stream(QObject *parent = nullptr);
    void SendRaw(const QString& Data);
    void Send(const QString& Type, QJsonObject Params);
    void SendSocketRaw(QWebSocket *Socket,const QString& Data);
signals:
    void OnConnection(QWebSocket *);
public slots:
    void Start(int Port);

private slots:
    void newConnection();
    void socketDisconnected();
};

#endif // STREAM_H
