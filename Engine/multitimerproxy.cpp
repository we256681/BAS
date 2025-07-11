#include "multitimerproxy.h"
#include <QDebug>
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    MultiTimerProxy::MultiTimerProxy(QObject *parent) :
        ITimerProxy(parent)
    {
        FireNext = false;
    }

    void MultiTimerProxy::timeoutSlot(const QDateTime& Now)
    {
        if(FireNext)
        {
            if(Now>NextTimeout)
            {
                FireNext = false;
                NewConnect = false;
                emit timeoutSignal();
                if(!NewConnect)
                    disconnect(this,SIGNAL(timeoutSignal()),0,0);
            }
        }
    }
    void MultiTimerProxy::stop()
    {
        FireNext = false;
        disconnect(this,SIGNAL(timeoutSignal()),0,0);
    }

    MultiTimerProxy::~MultiTimerProxy()
    {
        FireNext = false;
        disconnect(this,SIGNAL(timeoutSignal()),0,0);
    }

    void MultiTimerProxy::singleShot(int interval,const QObject* object, const char * slot_signal)
    {
        NewConnect = true;
        disconnect(this,SIGNAL(timeoutSignal()),0,0);
        connect(this,SIGNAL(timeoutSignal()),object,slot_signal,Qt::QueuedConnection);

        NextTimeout = QDateTime::currentDateTime();
        NextTimeout = NextTimeout.addMSecs(interval);
        FireNext = true;
    }
}
