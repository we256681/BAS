#include "pcapdnslistenserver.h"
#include <QTcpSocket>
#include <QByteArray>
#include <QDebug>

PcapDNSListenServer::PcapDNSListenServer(QObject *parent) : QObject(parent)
{
    Server = new QTcpServer(this);
    connect(Server,SIGNAL(newConnection()),this,SLOT(NewConnection()));
}

void PcapDNSListenServer::NewConnection()
{
    QTcpSocket *Connection = Server->nextPendingConnection();
    if(!Connection)
        return;

    connect(Connection,SIGNAL(readyRead()),this,SLOT(ReadyRead()));
    connect(Connection,SIGNAL(disconnected()),Connection,SLOT(deleteLater()));

}

int PcapDNSListenServer::GetPort()
{
    return Port;
}

void PcapDNSListenServer::ReadyRead()
{
    QTcpSocket *Connection = (QTcpSocket *)(sender());
    QByteArray Data = Connection->readAll();
    QByteArray Res;
    if(Data.isEmpty())
        return;
    if(Data.at(0) == 1)
    {
        //QString Domain = QString::fromUtf8(Data.mid(5));
        //qDebug()<<Domain;

        Res.resize(9);
        Res[0] = 0x01;
        Res[1] = 0x04;
        Res[2] = 0x00;
        Res[3] = 0x00;
        Res[4] = 0x00;
        Res[5] = 0x0a;
        Res[6] = Id & 0xFF;
        Res[7] = (Id >> 8) & 0xFF;
        Res[8] = (Id >> 16) & 0xFF;
        Id++;
    }
    if(Data.at(0) == 7)
    {
        Res.resize(5);
        Res[0] = 0x07;
        Res[1] = 0x00;
        Res[2] = 0x00;
        Res[3] = 0x00;
        Res[4] = 0x00;
    }

    if(!Res.isEmpty())
    {
        Connection->write(Res);
        Connection->flush();
    }

}

void PcapDNSListenServer::Start()
{
    if(Started)
        return;

    Started = Server->listen(QHostAddress::LocalHost);

    Port = Server->serverPort();
}
