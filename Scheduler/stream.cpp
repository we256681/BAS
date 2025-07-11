#include "stream.h"
#include <QJsonObject>
#include <QJsonDocument>

Stream::Stream(QObject *parent) : QObject(parent)
{

}


void Stream::Start(int Port)
{
    Server = new QWebSocketServer("Stream", QWebSocketServer::NonSecureMode, this);
    Server->listen(QHostAddress::LocalHost, Port);
    connect(Server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void Stream::SendRaw(const QString& Data)
{
    for(QWebSocket *Socket: Connections)
        Socket->sendTextMessage(Data);
}

void Stream::SendSocketRaw(QWebSocket *Socket,const QString& Data)
{
    Socket->sendTextMessage(Data);
}

void Stream::Send(const QString& Type, QJsonObject Params)
{
    QJsonObject Obj;
    Obj.insert("type", Type);
    Obj.insert("params", Params);
    QJsonDocument doc(Obj);
    SendRaw(QString::fromUtf8(doc.toJson()));
}

void Stream::newConnection()
{
    QWebSocket * Socket = Server->nextPendingConnection();
    Connections.append(Socket);
    connect(Socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    emit OnConnection(Socket);
}

void Stream::socketDisconnected()
{
    QWebSocket * Socket = qobject_cast<QWebSocket *>(sender());
    if(!Socket)
        return;
    Connections.removeAll(Socket);
    Socket->deleteLater();
}
