#include "webinterfacelogger.h"
#include <QRegularExpression>
#include <QVariantMap>
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    WebInterfaceLogger::WebInterfaceLogger(QObject *parent) :
        ILogger(parent)
    {
        WebInterface = 0;
        ResultIndex = -1;
    }

    void WebInterfaceLogger::SetWebInterface(IWebInterface *WebInterface)
    {
        this->WebInterface = WebInterface;
    }

    void WebInterfaceLogger::SetResultIndex(int Index)
    {
        this->ResultIndex = Index;
    }

    void WebInterfaceLogger::WriteInternal(const QString& line,LogLevel Level)
    {
        if(!WebInterface)
            return;

        if(ResultIndex >= 0)
        {
            WebInterface->Send("result", false, 0, "number", ResultIndex + 1);
            return;
        }

        QString Res = line;
        QRegularExpression ReplaceIds("^\\[(\\d+)\\]");
        QRegularExpressionMatch Match = ReplaceIds.match(Res);
        QVariantMap SendData;

        QString TypeString;
        if(Level == LogUser)
            TypeString = "user";
        if(Level == LogSuccess)
            TypeString = "success";
        if(Level == LogFail)
            TypeString = "fail";
        if(Level == LogInfo)
            TypeString = "info";

        SendData["type"] = TypeString;
        if(Match.hasMatch())
        {
            SendData["action_id"] = Match.capturedTexts().at(1);
            Res.remove(Match.capturedStart(),Match.capturedLength());
        }else
        {
            SendData["action_id"] = QVariant();
        }

        SendData["text"] = Res;
        WebInterface->SendRaw("log", false, 0, SendData);
    }


    QString WebInterfaceLogger::GetFileName()
    {
        return FileName;
    }
    void WebInterfaceLogger::SetFileName(const QString& FileName)
    {
        this->FileName = FileName;
    }
    void WebInterfaceLogger::Write(const QString& line, LogLevel Level)
    {
        WriteInternal(line,Level);
    }
    void WebInterfaceLogger::Write(const QString& line)
    {
        Write(line, LogUser);
    }
    void WebInterfaceLogger::Clear()
    {

    }
    void WebInterfaceLogger::WriteHtml(const QString& line_html, const QString& line_raw, LogLevel Level)
    {
        WriteInternal(line_raw,Level);
    }
    void WebInterfaceLogger::WriteHtml(const QString& line_html, const QString& line_raw)
    {
        WriteHtml(line_html, line_raw, LogUser);
    }
    void WebInterfaceLogger::WriteFail(const QString& line, LogLevel Level)
    {
        WriteInternal(line,LogFail);
    }
    void WebInterfaceLogger::WriteFail(const QString& line)
    {
        WriteFail(line, LogUser);
    }
    void WebInterfaceLogger::WriteSuccess(const QString& line, LogLevel Level)
    {
        WriteInternal(line,LogSuccess);
    }
    void WebInterfaceLogger::WriteSuccess(const QString& line)
    {
        WriteSuccess(line, LogUser);
    }
    void WebInterfaceLogger::WriteInfo(const QString& line, LogLevel Level)
    {
        WriteInternal(line,LogInfo);
    }
    void WebInterfaceLogger::WriteInfo(const QString& line)
    {
        WriteInfo(line, LogUser);
    }
    void WebInterfaceLogger::Filter(QList<LogLevel> Level, int ThreadNumber, const QString& Filter, bool IsActive)
    {

    }
    void WebInterfaceLogger::SetReplaceActionIdWithLink()
    {

    }
    void WebInterfaceLogger::SetReplaceActionIdWithColor()
    {

    }


}
