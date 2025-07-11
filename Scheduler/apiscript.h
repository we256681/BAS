#ifndef APISCRIPT_H
#define APISCRIPT_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QProcess>
#include "logger.h"
#include <memory>


class ApiScript : public QObject
{
    Q_OBJECT
    struct LocalServerDataClass
    {
        QString key;
        QByteArray data;
    };
    QList<QProcess *> StartingProcesses;
    using LocalServerData = std::shared_ptr<LocalServerDataClass>;

    QLocalServer *Server = 0;
    Logger *_Logger;
    QHash<QLocalSocket *,LocalServerData> Sockets;
    QMap<QString,QStringList> TaskStartQueue;
    QMap<QString,QString> WorkingProcessesPids;
    QString TaskStartingId;
    QDateTime TaskStartingDate;

public:
    explicit ApiScript(QObject *parent = nullptr);
    void Start();
    void SetLogger(Logger * _Logger);

signals:
    //Id
    void OnStarted(QString);
    //Id,Log,IsSuccess
    void OnFinished(QString,QString,bool);
    //Id,Message
    void OnMessage(QString,QString,QString,QMap<QString,QString>);
public slots:
    void SendMessageRaw(QString Id,QString Message);
    void SendMessageFormat(QString Id,const QString& TagName, const QString& Content = QString(),
                           const QString& AttributeName1 = QString(), const QString& AttributeData1 = QString(),
                           const QString& AttributeName2 = QString(), const QString& AttributeData2 = QString(),
                           const QString& AttributeName3 = QString(), const QString& AttributeData3 = QString());
    void RunProcess(QString Id,QStringList CommandLine);
    void TerminateProcess(QString Id);
    void Timer();
private slots:
    void ParseMessage(const QString& Id, const QString& Message);
    void NewConnection();
    void Finished();
    void Income();
    QLocalSocket * FindLocalServerByKey(const QString& Key);
    void ProcessFinished();
    void RemoveFromStartQueue(const QString& Id);
    void ReadProcessOutput();


};

#endif // APISCRIPT_H
