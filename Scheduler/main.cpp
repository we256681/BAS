#include <QCoreApplication>
#include <QDir>
#include "taskmanager.h"
#include "apiexternal.h"
#include "webapplication.h"
#include "apiscript.h"
#include "taskpersist.h"
#include "schedulepredict.h"
#include "logger.h"
#include "manualactions.h"
#include "runmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    RunManager * _RunManager = new RunManager();
    if(!_RunManager->OnStart(qApp->arguments()))
        return 0;

    QTime dieTime = QTime::currentTime().addMSecs( 5000 );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }

    qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
    WebApplication * _WebApplication = new WebApplication();
    ApiExternal * _ApiExternal = new ApiExternal();
    ApiScript * _ApiScript = new ApiScript();
    TaskManager * _TaskManager = new TaskManager();
    TaskPersist * _TaskPersist = new TaskPersist();
    Logger * _Logger = new Logger();
    Stream * _Stream = new Stream();
    SchedulePredict *_SchedulePredict = new SchedulePredict();
    ManualActions *_ManualActions = new ManualActions();

    _Logger->SetStream(_Stream);
    _TaskPersist->SetStream(_Stream);
    _TaskPersist->SetManualActions(_ManualActions);

    _ApiExternal->SetRunManager(_RunManager);
    _ApiExternal->SetSchedulePredict(_SchedulePredict);
    _TaskManager->SetManualActions(_ManualActions);
    _TaskManager->SetApiExternal(_ApiExternal);
    _TaskManager->SetApiScript(_ApiScript);
    _TaskManager->SetTaskPersist(_TaskPersist);
    _TaskManager->SetLogger(_Logger);
    _TaskManager->SetStream(_Stream);
    _TaskManager->SetWebApplication(_WebApplication);
    _ManualActions->SetApiScript(_ApiScript);
    _ManualActions->SetStream(_Stream);
    _ApiScript->SetLogger(_Logger);

    _TaskManager->Start();
    return a.exec();
}

