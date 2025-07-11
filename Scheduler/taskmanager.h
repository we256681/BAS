#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include "apiexternal.h"
#include "apiscript.h"
#include "taskpersist.h"
#include "stream.h"
#include "manualactions.h"
#include "webapplication.h"
#include "logger.h"

class TaskManager : public QObject
{
    Q_OBJECT
    ApiExternal *_ApiExternal = 0;
    ApiScript *_ApiScript = 0;
    TaskPersist *_TaskPersist = 0;
    Logger *_Logger = 0;
    Stream *_Stream = 0;
    ManualActions *_ManualActions = 0;
    WebApplication *_WebApplication = 0;
public:
    explicit TaskManager(QObject *parent = nullptr);
    void SetApiExternal(ApiExternal *_ApiExternal);
    void SetManualActions(ManualActions *_ManualActions);
    void SetApiScript(ApiScript *_ApiScript);
    void SetTaskPersist(TaskPersist *_TaskPersist);
    void SetStream(Stream *_Stream);
    void SetLogger(Logger *_Logger);
    void SetWebApplication(WebApplication *_WebApplication);
    void Start();

signals:
    void Iddle();
private slots:
    void TaskUpdate(QString Id, QString Text, bool IsHard);
    void TaskInsert(QString Text, bool IsArchive);
    void StartResourceEditLocal(QString Id,QString Text,QString Actual);
    void StartResourceEditRemote(QString Id,QString Name,QString Login,QString Pass,QString Actual);
    void TaskRemove(QString Id);
    void TaskReport(QString Id);

    void TaskStop(QString Id);
    void TaskCancel(QString Id);
    void TaskStart(QString Id);
    void TaskArchive(QString Id);
    void TaskRestore(QString Id);
    void TaskDelete(QString Id);
    void TaskToggleVisibility(QString Id);

    void TaskRemoveArchived();
    void TaskClearAll();
    void RunTask(Task &T);
    void Process();
    void UpdateTaskAfterFinished(Task &T);
    void OnMessage(QString Id,QString TagName, QString Content, QMap<QString,QString> Params);

    void OnStarted(QString);
    void OnFinished(QString,QString,bool);

    int ObjectToInterval(const QJsonObject& Object);
    bool CheckProbability(int Probability);
    int GetRanomBetween(int Start, int End);

    void ClearApplicationLog();
    void ClearSchedulerLog();
    void OnConnection(QWebSocket * Socket);
    void ManualAnswer(QString Id,QString,bool);
};

#endif // TASKMANAGER_H

