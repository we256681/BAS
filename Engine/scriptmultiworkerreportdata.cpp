#include "scriptmultiworkerreportdata.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    ScriptMultiWorkerReportData::ScriptMultiWorkerReportData(QObject *parent) :
        IScriptMultiWorkerReportData(parent)
    {
        SuccessCount = 0;
        FailCount = 0;
        SuccessAndRescuedCount = 0;
        FailAndRescuedCount = 0;
        Running = false;
        RunAtLeastOnce = false;
    }

    void ScriptMultiWorkerReportData::Success(const QString& message)
    {
        if(!Running)
            return;
        SuccessCount++;
        QString Message = message;
        Message = Message.replace("openfile:///","");
        if(SuccessMessages.contains(Message))
        {
            SuccessMessages[Message]++;
        }else
        {
            SuccessMessages[Message] = 1;
        }
    }

    void ScriptMultiWorkerReportData::SuccessAndRescued(const QString& message)
    {
        if(!Running)
            return;
        SuccessAndRescuedCount++;
        QString Message = message;
        Message = Message.replace("openfile:///","");
        Message = Message.replace("bablosoft.com","server.com");
        if(SuccessAndRescuedMessages.contains(Message))
        {
            SuccessAndRescuedMessages[Message]++;
        }else
        {
            SuccessAndRescuedMessages[Message] = 1;
        }
    }

    void ScriptMultiWorkerReportData::Fail(const QString& message)
    {
        if(!Running)
            return;
        FailCount++;
        QString Message = message;
        Message = Message.replace("openfile:///","");
        if(FailMessages.contains(Message))
        {
            FailMessages[Message]++;
        }else
        {
            FailMessages[Message] = 1;
        }
    }

    void ScriptMultiWorkerReportData::FailAndRescued(const QString& message)
    {
        if(!Running)
            return;
        FailAndRescuedCount++;
        QString Message = message;
        Message = Message.replace("openfile:///","");
        Message = Message.replace("bablosoft.com","server.com");
        if(FailAndRescuedMessages.contains(Message))
        {
            FailAndRescuedMessages[Message]++;
        }else
        {
            FailAndRescuedMessages[Message] = 1;
        }
    }

    void ScriptMultiWorkerReportData::Final(const QString& message)
    {
        if(!Running)
            return;
        QString Message = message;
        Message = Message.replace("openfile:///","");
        Message = Message.replace("bablosoft.com","server.com");
        FinalMessage = Message;
    }
    void ScriptMultiWorkerReportData::Start()
    {
        RunAtLeastOnce = true;
        Running = true;
        SuccessCount = 0;
        FailCount = 0;
        SuccessAndRescuedCount = 0;
        FailAndRescuedCount = 0;
        SuccessMessages.clear();
        FailMessages.clear();
        SuccessAndRescuedMessages.clear();
        FailAndRescuedMessages.clear();
        CaptchaStats.clear();
        ScriptStart = QDateTime::currentDateTime();
    }

    void ScriptMultiWorkerReportData::Stop()
    {
        if(Running)
        {
            ScriptEnd = QDateTime::currentDateTime();
            Running = false;
        }
    }

    void ScriptMultiWorkerReportData::CaptchaUsed(const QString& solver)
    {
        if(!Running)
            return;
        if(CaptchaStats.contains(solver))
        {
            QPair<int,int> pair = CaptchaStats[solver];
            pair.first++;
            CaptchaStats[solver] = pair;
        }else
        {
            QPair<int,int> pair;
            pair.first = 1;
            pair.second = 0;
            CaptchaStats[solver] = pair;
        }
    }
    void ScriptMultiWorkerReportData::CaptchaFailed(const QString& solver)
    {
        if(!Running)
            return;
        if(CaptchaStats.contains(solver))
        {
            QPair<int,int> pair = CaptchaStats[solver];
            pair.second++;
            CaptchaStats[solver] = pair;
        }else
        {
            QPair<int,int> pair;
            pair.first = 0;
            pair.second = 1;
            CaptchaStats[solver] = pair;
        }
    }

    int ScriptMultiWorkerReportData::GetSuccessCount()
    {
        return SuccessCount;
    }
    QHash<QString,int> ScriptMultiWorkerReportData::GetSuccessMessages()
    {
        return SuccessMessages;
    }
    int ScriptMultiWorkerReportData::GetFailCount()
    {
        return FailCount;
    }
    QHash<QString,int> ScriptMultiWorkerReportData::GetFailMessages()
    {
        return FailMessages;
    }

    int ScriptMultiWorkerReportData::GetSuccessAndRescuedCount()
    {
        return SuccessAndRescuedCount;
    }
    QHash<QString,int> ScriptMultiWorkerReportData::GetSuccessAndRescuedMessages()
    {
        return SuccessAndRescuedMessages;
    }
    int ScriptMultiWorkerReportData::GetFailAndRescuedCount()
    {
        return FailAndRescuedCount;
    }
    QHash<QString,int> ScriptMultiWorkerReportData::GetFailAndRescuedMessages()
    {
        return FailAndRescuedMessages;
    }

    QString ScriptMultiWorkerReportData::GetFinal()
    {
        return FinalMessage;
    }
    int ScriptMultiWorkerReportData::GetSecondsSpent()
    {
        if(!RunAtLeastOnce)
        {
            return 0;
        }

        QDateTime end;
        if(Running)
            end = QDateTime::currentDateTime();
        else
            end = ScriptEnd;
        return ScriptStart.secsTo(end);
    }

    bool ScriptMultiWorkerReportData::IsRunAtLeastOnce()
    {
        return RunAtLeastOnce;
    }

    bool ScriptMultiWorkerReportData::IsRunning()
    {
        return Running;
    }

    QHash<QString,QPair<int,int> > ScriptMultiWorkerReportData::GetCaptchaStats()
    {
        return CaptchaStats;
    }
}
