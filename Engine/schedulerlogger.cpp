#include "schedulerlogger.h"
#include <QRegularExpression>
#include <QVariantMap>
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    SchedulerLogger::SchedulerLogger(QObject *parent) :
        ILogger(parent)
    {
        SchedulerConnector = 0;
        ResultIndex = -1;
    }

    void SchedulerLogger::SetSchedulerConnector(ISchedulerConnector *SchedulerConnector)
    {
        this->SchedulerConnector = SchedulerConnector;
    }

    void SchedulerLogger::SetResultIndex(int Index)
    {
        this->ResultIndex = Index;
    }

    void SchedulerLogger::WriteInternal(const QString& line,LogLevel Level)
    {
        if(!SchedulerConnector)
            return;

        if(ResultIndex >= 0)
        {
            SchedulerConnector->MessageSendFormat("Result",line,"Index",QString::number(ResultIndex));
            return;
        }

        QString Res = line;
        QRegularExpression ReplaceIds("^\\[(\\d+)\\]");
        QRegularExpressionMatch Match = ReplaceIds.match(Res);

        QString TypeString;
        if(Level == LogUser)
            TypeString = "user";
        if(Level == LogSuccess)
            TypeString = "success";
        if(Level == LogFail)
            TypeString = "fail";
        if(Level == LogInfo)
            TypeString = "info";

        QString ActionId;
        if(Match.hasMatch())
        {
            ActionId = Match.capturedTexts().at(1);
            Res.remove(Match.capturedStart(),Match.capturedLength());
        }

        SchedulerConnector->MessageSendFormat("Log", Res, "ActionId", ActionId, "Type", TypeString);
    }


    QString SchedulerLogger::GetFileName()
    {
        return FileName;
    }
    void SchedulerLogger::SetFileName(const QString& FileName)
    {
        this->FileName = FileName;
    }
    void SchedulerLogger::Write(const QString& line, LogLevel Level)
    {
        WriteInternal(line,Level);
    }
    void SchedulerLogger::Write(const QString& line)
    {
        Write(line, LogUser);
    }
    void SchedulerLogger::Clear()
    {

    }
    void SchedulerLogger::WriteHtml(const QString& line_html, const QString& line_raw, LogLevel Level)
    {
        WriteInternal(line_raw,Level);
    }
    void SchedulerLogger::WriteHtml(const QString& line_html, const QString& line_raw)
    {
        WriteHtml(line_html, line_raw, LogUser);
    }
    void SchedulerLogger::WriteFail(const QString& line, LogLevel Level)
    {
        WriteInternal(line,LogFail);
    }
    void SchedulerLogger::WriteFail(const QString& line)
    {
        WriteFail(line, LogUser);
    }
    void SchedulerLogger::WriteSuccess(const QString& line, LogLevel Level)
    {
        WriteInternal(line,LogSuccess);
    }
    void SchedulerLogger::WriteSuccess(const QString& line)
    {
        WriteSuccess(line, LogUser);
    }
    void SchedulerLogger::WriteInfo(const QString& line, LogLevel Level)
    {
        WriteInternal(line,LogInfo);
    }
    void SchedulerLogger::WriteInfo(const QString& line)
    {
        WriteInfo(line, LogUser);
    }
    void SchedulerLogger::Filter(QList<LogLevel> Level, int ThreadNumber, const QString& Filter, bool IsActive)
    {

    }
    void SchedulerLogger::SetReplaceActionIdWithLink()
    {

    }
    void SchedulerLogger::SetReplaceActionIdWithColor()
    {

    }


}
