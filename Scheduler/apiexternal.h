#ifndef APIEXTERNAL_H
#define APIEXTERNAL_H

#include <QObject>
#include <QSettings>
#include "httplistener.h"
#include "httprequesthandler.h"
#include "taskpersist.h"
#include "logger.h"
#include "schedulepredict.h"
#include "manualactions.h"
#include "runmanager.h"

using namespace stefanfrings;

class ApiExternal : public HttpRequestHandler
{
    Q_OBJECT
    HttpListener * Listener = 0;
    QSettings * Settings;
    /*TaskPersist * _TaskPersist;
    Logger * _Logger;
    ManualActions * _ManualActions;*/
    SchedulePredict * _SchedulePredict;
    RunManager * _RunManager;
public:
    explicit ApiExternal(QObject *parent = nullptr);
    void Start(int Port);
    /*void SetTaskPersist(TaskPersist *_TaskPersist);
    void SetManualActions(ManualActions *_ManualActions);
    void SetLogger(Logger *_Logger);*/
    void SetSchedulePredict(SchedulePredict *_SchedulePredict);
    void SetRunManager(RunManager * _RunManager);
    void service(HttpRequest& request, HttpResponse& response);
signals:
    void TaskInsert(QString,bool);
    void TaskUpdate(QString,QString,bool);
    void TaskReport(QString);

    void TaskStop(QString);
    void TaskCancel(QString);
    void TaskStart(QString);
    void TaskArchive(QString);
    void TaskRestore(QString);
    void TaskDelete(QString);
    void TaskToggleVisibility(QString);
    void StartResourceEditLocal(QString,QString,QString);
    void StartResourceEditRemote(QString,QString,QString,QString,QString);

    void TaskClearAll();
    void TaskRemoveArchived();
    void ManualAnswer(QString, QString, bool);
    void ClearApplicationLog();
    void ClearSchedulerLog();
    void GetSummary();
public slots:
};

#endif // APIEXTERNAL_H
