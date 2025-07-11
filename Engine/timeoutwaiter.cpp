#include "timeoutwaiter.h"
#include <QTimer>
#include "qtimerproxy.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    TimeoutWaiter::TimeoutWaiter(QObject *parent) :
        IWaiter(parent)
    {
        IsReportAboutStartedOrEnded = false;
        Timer = 0;
        IsAborted = false;
        RestoreConnectors();
        GeneralWait = 60000;
        SolverWait = 200000;
        GeneralWaitNext = -1;
        SolverWaitNext = -1;
        SkipWaitHandlerMode = false;
    }

    void TimeoutWaiter::SetReportAboutStartedOrEnded()
    {
        IsReportAboutStartedOrEnded = true;
    }

    bool TimeoutWaiter::GetReportAboutStartedOrEnded()
    {
        return IsReportAboutStartedOrEnded;
    }


    int TimeoutWaiter::GetGeneralWait()
    {
        if(GeneralWaitNext > 0)
        {
            int res = GeneralWaitNext;
            GeneralWaitNext = -1;
            return res;
        }
        return GeneralWait;
    }

    int TimeoutWaiter::PeekGeneralWait()
    {
        if(GeneralWaitNext > 0)
        {
            return GeneralWaitNext;
        }
        return GeneralWait;
    }

    int TimeoutWaiter::GetSolverWait()
    {
        if(SolverWaitNext > 0)
        {
            int res = SolverWaitNext;
            SolverWaitNext = -1;
            return res;
        }
        return SolverWait;
    }

    void TimeoutWaiter::SetSkipWaitHandlerMode()
    {
        SkipWaitHandlerMode = true;
    }

    void TimeoutWaiter::SetGeneralWaitTimeout(int timeout)
    {
        if(timeout<1000)
            GeneralWait = 1000;
        else
            GeneralWait = timeout;
    }

    void TimeoutWaiter::SetSolverWaitTimeout(int timeout)
    {
        if(timeout<1000)
            SolverWait = 1000;
        else
            SolverWait = timeout;
    }

    void TimeoutWaiter::SetGeneralWaitTimeoutNext(int timeout)
    {
        if(timeout<=0)
        {
            GeneralWaitNext = -1;
            return;
        }
        if(timeout<1000)
            GeneralWaitNext = 1000;
        else
            GeneralWaitNext = timeout;
    }

    int TimeoutWaiter::GetGeneralWaitTimeoutNext()
    {
        return GeneralWaitNext;
    }

    int TimeoutWaiter::GetSolverWaitTimeoutNext()
    {
        return SolverWaitNext;
    }

    void TimeoutWaiter::SetSolverWaitTimeoutNext(int timeout)
    {
        if(timeout<=0)
        {
            SolverWaitNext = -1;
            return;
        }
        if(timeout<1000)
            SolverWaitNext = 1000;
        else
            SolverWaitNext = timeout;
    }

    ITimerProxy *TimeoutWaiter::GetTimer()
    {
        if(!Timer)
        {
            Timer = new QTimerProxy(this);
        }
        return Timer;
    }

    /*QTimer *TimeoutWaiter::GetTimer()
    {
        if(!Timer)
        {
            qDebug()<<"Created Timer";
            Timer = new QTimer(this);
            Timer->setSingleShot(true);
        }
        return Timer;
    }*/


    void TimeoutWaiter::SetTimer(ITimerProxy *Timer)
    {
        this->Timer = Timer;
    }


    void TimeoutWaiter::WaitForSignal(const QObject *object_wait, const char * slot_signal ,const QObject *object_success,const char * slot_success,const QObject *object_fail,const char * slot_fail, int ExactTimeout)
    {
        if(IsAborted)
        {
            return;
        }
        if(IsReportAboutStartedOrEnded)
        {
            emit Started();
        }
        ConnectorSuccess = new WaiterConnector(this);
        ConnectorFail = new WaiterConnector(this);


        connect(ConnectorSuccess,SIGNAL(signal()),GetTimer(),SLOT(stop()));
        disconnect(object_wait,slot_signal,this,SLOT(clean()));
        connect(object_wait,slot_signal,this,SLOT(clean()),Qt::QueuedConnection);
        connect(object_wait,slot_signal,ConnectorSuccess,SLOT(slot()));
        connect(ConnectorSuccess,SIGNAL(signal()),this,SLOT(RestoreConnectors()));
        connect(ConnectorSuccess,SIGNAL(signal()),ConnectorFail,SLOT(end()));
        connect(ConnectorSuccess,SIGNAL(signal()),object_success,slot_success);


        connect(ConnectorFail,SIGNAL(signal()),GetTimer(),SLOT(stop()));
        connect(ConnectorFail,SIGNAL(signal()),this,SLOT(RestoreConnectors()));
        connect(ConnectorFail,SIGNAL(signal()),ConnectorSuccess,SLOT(end()));
        connect(ConnectorFail,SIGNAL(signal()),object_fail,slot_fail);

        int timeout = ExactTimeout;
        if(timeout < 0)
            timeout = GetGeneralWait();

        GetTimer()->singleShot(timeout,ConnectorFail,SLOT(slot()));
    }

    void TimeoutWaiter::WaitInfinity(const QObject *object_wait, const char * slot_signal ,const QObject *object_success,const char * slot_success)
    {
        if(IsAborted)
        {
            return;
        }
        if(IsReportAboutStartedOrEnded)
        {
            emit Started();
        }
        ConnectorSuccess = new WaiterConnector(this);
        disconnect(object_wait,slot_signal,this,SLOT(clean()));
        connect(object_wait,slot_signal,this,SLOT(clean()),Qt::QueuedConnection);
        connect(object_wait,slot_signal,ConnectorSuccess,SLOT(slot()));
        connect(ConnectorSuccess,SIGNAL(signal()),this,SLOT(RestoreConnectors()));
        connect(ConnectorSuccess,SIGNAL(signal()),object_success,slot_success);
    }


    void TimeoutWaiter::WaitForStageFinished(const QObject *object_stage,const char * signal_finished,const QObject *object_listener,const char * slot_finished)
    {
        if(IsAborted)
        {
            return;
        }
        if(IsReportAboutStartedOrEnded)
        {
            emit Started();
        }
        ConnectorSuccess = new WaiterConnector(this);
        disconnect(object_stage,signal_finished,this,SLOT(clean()));
        connect(object_stage,signal_finished,this,SLOT(clean()),Qt::QueuedConnection);

        connect(object_stage,signal_finished,ConnectorSuccess,SLOT(slot()));
        connect(ConnectorSuccess,SIGNAL(signal()),this,SLOT(RestoreConnectors()));
        connect(ConnectorSuccess,SIGNAL(signal()),object_listener,slot_finished);
    }

    void TimeoutWaiter::WaitForSolver(ISolver * solver, const QString & id,const QObject *object_success,const char * slot_success,const QObject *object_fail,const char * slot_fail)
    {
        if(IsAborted)
        {
            return;
        }
        if(IsReportAboutStartedOrEnded)
        {
            emit Started();
        }
        Text = id;
        Solver = solver;
        ConnectorSuccess = new WaiterConnector(this);
        if(solver->TimeLimited())
            ConnectorFail = new WaiterConnector(this);



        connect(ConnectorSuccess,SIGNAL(signal()),GetTimer(),SLOT(stop()));
        connect(ConnectorSuccess,SIGNAL(signal()),this,SLOT(RestoreConnectors()));
        if(solver->TimeLimited())
            connect(ConnectorSuccess,SIGNAL(signal()),ConnectorFail,SLOT(end()));
        connect(ConnectorSuccess,SIGNAL(signal()),object_success,slot_success);

        if(solver->TimeLimited())
        {
            connect(ConnectorFail,SIGNAL(signal()),this,SLOT(DisconnectFromSolver()));
            connect(ConnectorFail,SIGNAL(signal()),GetTimer(),SLOT(stop()));
            connect(ConnectorFail,SIGNAL(signal()),this,SLOT(RestoreConnectors()));
            connect(ConnectorFail,SIGNAL(signal()),ConnectorSuccess,SLOT(end()));
            connect(ConnectorFail,SIGNAL(signal()),object_fail,slot_fail);
            GetTimer()->singleShot(GetSolverWait(),ConnectorFail,SLOT(slot()));
        }

        connect(solver,SIGNAL(Done(QString,QString,bool,QString)),this,SLOT(ObtainSolverResult(QString,QString,bool,QString)));

    }

    void TimeoutWaiter::DisconnectFromSolver()
    {
        disconnect(Solver,SIGNAL(Done(QString,QString,bool,QString)),this,SLOT(ObtainSolverResult(QString,QString,bool,QString)));
    }

    QString TimeoutWaiter::GetLastSolverResult()
    {
        return SolverResult;
    }

    QString TimeoutWaiter::GetLastSolverId()
    {
        return SolverId;
    }

    void TimeoutWaiter::ObtainSolverResult(const QString& val, const QString& id, bool res, const QString& solver_id)
    {
        if(IsAborted)
        {
            DisconnectFromSolver();
            return;
        }

        if(Text == id)
        {
            DisconnectFromSolver();
            SolverId = solver_id;
            if(!res)
            {
                SolverResult = "CAPTCHA_FAIL : " + val;
                ConnectorSuccess->slot();
            }else
            {
                SolverResult = val;
                ConnectorSuccess->slot();
            }
        }
    }



    void TimeoutWaiter::WaitForHandler(IEngineResources *res, const QString name, const QSet<QString>& refuse,const QObject *object_success,const char * slot_success,const QObject *object_fail,const char * slot_fail)
    {

        if(IsAborted)
        {
            return;
        }

        if(IsReportAboutStartedOrEnded)
        {
            emit Started();
        }

        ResourceHandler = 0;
        WaitingForResourceIteration = 0;
        EngineResources = res;
        Text = name;
        Refuse = refuse;
        ResourceRefused = false;
        ConnectorSuccess = new WaiterConnector(this);
        ConnectorFail = new WaiterConnector(this);

        connect(ConnectorSuccess,SIGNAL(signal()),GetTimer(),SLOT(stop()));
        connect(ConnectorSuccess,SIGNAL(signal()),this,SLOT(RestoreConnectors()));
        connect(ConnectorSuccess,SIGNAL(signal()),ConnectorFail,SLOT(end()));
        connect(ConnectorSuccess,SIGNAL(signal()),object_success,slot_success);

        connect(ConnectorFail,SIGNAL(signal()),GetTimer(),SLOT(stop()));
        connect(ConnectorFail,SIGNAL(signal()),this,SLOT(RestoreConnectors()));
        connect(ConnectorFail,SIGNAL(signal()),ConnectorSuccess,SLOT(end()));
        connect(ConnectorFail,SIGNAL(signal()),object_fail,slot_fail);


        WaitForHandlerIteration();
    }
    void TimeoutWaiter::WaitForHandlerIteration()
    {
        if(IsAborted)
        {
            return;
        }

        ResourceHandler = EngineResources->GetHandler(Text, Refuse);
        if(!ResourceHandler)
        {
            if(ConnectorFail)
                ConnectorFail->slot();
            return;
        }
        switch(ResourceHandler->GetHandlerStatus())
        {
            case IResourceHandler::Ready:
                if(ConnectorSuccess)
                    ConnectorSuccess->slot();
            return;
            case IResourceHandler::Refused:
                ResourceRefused = true;
                ResourceHandler->deleteLater();
                if(ConnectorFail)
                    ConnectorFail->slot();
            return;
            case IResourceHandler::Wait:
                ResourceHandler->deleteLater();
                if(SkipWaitHandlerMode)
                {
                    if(ConnectorFail)
                        ConnectorFail->slot();
                    return;
                } break;
            case IResourceHandler::NotAvailable:
                ResourceHandler->deleteLater();
                if(ConnectorFail)
                    ConnectorFail->slot();
            return;
        }

        if(WaitingForResourceIteration == 1)
        {
            emit NeedToWaitForResource(Text);
        }
        WaitingForResourceIteration++;
        GetTimer()->singleShot(5000,this,SLOT(WaitForHandlerIteration()));
    }

    bool TimeoutWaiter::IsResourceRefused()
    {
        return ResourceRefused;
    }


    IResourceHandler *TimeoutWaiter::GetLastHandler()
    {
        return ResourceHandler;
    }

    void TimeoutWaiter::WaitForElement(IWebElement* Element, const QString& text,const QObject *object_success,const char * slot_success,const QObject *object_fail,const char * slot_fail)
    {
        if(IsAborted)
        {
            return;
        }

        if(IsReportAboutStartedOrEnded)
        {
            emit Started();
        }
        Iterator = 0;
        WebElement = Element;
        Text = text;
        ConnectorSuccess = new WaiterConnector(this);
        ConnectorFail = new WaiterConnector(this);

        connect(ConnectorSuccess,SIGNAL(signal()),GetTimer(),SLOT(stop()));
        connect(ConnectorSuccess,SIGNAL(signal()),this,SLOT(RestoreConnectors()));
        connect(ConnectorSuccess,SIGNAL(signal()),ConnectorFail,SLOT(end()));
        connect(ConnectorSuccess,SIGNAL(signal()),object_success,slot_success);

        connect(ConnectorFail,SIGNAL(signal()),GetTimer(),SLOT(stop()));
        connect(ConnectorFail,SIGNAL(signal()),this,SLOT(RestoreConnectors()));
        connect(ConnectorFail,SIGNAL(signal()),ConnectorSuccess,SLOT(end()));
        connect(ConnectorFail,SIGNAL(signal()),object_fail,slot_fail);

        WaitForElementIteration();
    }

    void TimeoutWaiter::WaitForElementIteration()
    {
        if(IsAborted)
        {
            return;
        }
        WebElement->RunScriptAsync("jQuery(this).val()", this, SLOT(WaitForElementRespond(QString)));

    }

    void TimeoutWaiter::WaitForElementRespond(const QString& res)
    {
        if(IsAborted)
        {
            return;
        }

        if(res.endsWith(Text))
        {
            ConnectorSuccess->slot();
            return;
        }

        if(Iterator++ > 60)
        {
            ConnectorFail->slot();
            return;
        }

        GetTimer()->singleShot(1000,this,SLOT(WaitForElementIteration()));
    }

    void TimeoutWaiter::Sleep(int milliseconds,const QObject *object_success,const char * slot_success)
    {
        if(IsAborted)
        {
            return;
        }

        if(IsReportAboutStartedOrEnded)
        {
            emit Started();
        }

        ConnectorSuccess = new WaiterConnector(this);


        connect(ConnectorSuccess,SIGNAL(signal()),GetTimer(),SLOT(stop()));
        connect(ConnectorSuccess,SIGNAL(signal()),this,SLOT(RestoreConnectors()));
        connect(ConnectorSuccess,SIGNAL(signal()),object_success,slot_success);

        GetTimer()->singleShot(milliseconds,ConnectorSuccess,SLOT(slot()));
    }

    void TimeoutWaiter::Abort()
    {
        IsAborted = true;
        if(Timer)
        {
            Timer->stop();
            Timer->deleteLater();
            Timer = 0;
        }
        if(ConnectorSuccess)
        {
            ConnectorSuccess->end();
            ConnectorSuccess = 0;
        }
        if(ConnectorFail)
        {
            ConnectorFail->end();
            ConnectorFail = 0;
        }

    }

    void TimeoutWaiter::Stop()
    {
        if(ConnectorSuccess)
        {
            ConnectorSuccess->end();
            ConnectorSuccess = 0;
        }
        if(ConnectorFail)
        {
            ConnectorFail->end();
            ConnectorFail = 0;
        }

    }
    void TimeoutWaiter::RestoreConnectors()
    {
        ConnectorSuccess = 0;
        ConnectorFail = 0;
        if(IsReportAboutStartedOrEnded)
        {
            emit Ended();
        }
    }

    bool TimeoutWaiter::IsActive()
    {
        if(IsAborted)
            return false;

        return ConnectorSuccess;
    }

    void TimeoutWaiter::clean()
    {
        connect(sender(),SIGNAL(destroyed(QObject*)),this,SLOT(dummy()));
        disconnect(sender(),SIGNAL(destroyed(QObject*)),this,SLOT(dummy()));
    }

    void TimeoutWaiter::dummy()
    {

    }

}
