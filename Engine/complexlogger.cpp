#include "complexlogger.h"
#include "every_cpp.h"


namespace BrowserAutomationStudioFramework
{
    ComplexLogger::ComplexLogger(QObject *parent) :
        ILogger(parent)
    {
    }

    void ComplexLogger::SetReplaceActionIdWithLink()
    {

    }
    void ComplexLogger::SetReplaceActionIdWithColor()
    {

    }

    void ComplexLogger::AddLogger(ILogger* Logger)
    {
        Loggers.append(Logger);
    }

    QString ComplexLogger::GetFileName()
    {
        foreach(ILogger *log,Loggers)
        {
            QString res = log->GetFileName();
            if(!res.isEmpty())
                return res;

        }
    }

    void ComplexLogger::Clear()
    {
        foreach(ILogger *log,Loggers)
            log->Clear();
    }

    void ComplexLogger::Write(const QString& line, LogLevel Level)
    {
        foreach(ILogger *log,Loggers)
            log->Write(line, Level);

        emit NewLine();
    }

    void ComplexLogger::Write(const QString& line)
    {
        Write(line, LogUser);
    }

    void ComplexLogger::Filter(QList<LogLevel> Level, int ThreadNumber, const QString& Filter, bool IsActive)
    {
        foreach(ILogger *log,Loggers)
            log->Filter(Level, ThreadNumber, Filter, IsActive);
    }

    void ComplexLogger::SetFileName(const QString& FileName)
    {
        foreach(ILogger *log,Loggers)
            log->SetFileName(FileName);
    }

    void ComplexLogger::WriteHtml(const QString& line_html, const QString& line_raw, LogLevel Level)
    {
        foreach(ILogger *log,Loggers)
            log->WriteHtml(line_html, line_raw, Level);

        emit NewLine();
    }

    void ComplexLogger::WriteHtml(const QString& line_html, const QString& line_raw)
    {
        WriteHtml(line_html, line_raw, LogUser);
    }

    void ComplexLogger::WriteFail(const QString& line, LogLevel Level)
    {
        foreach(ILogger *log,Loggers)
            log->WriteFail(line, Level);

        emit NewLine();
    }

    void ComplexLogger::WriteFail(const QString& line)
    {
        WriteFail(line, LogUser);
    }

    void ComplexLogger::WriteSuccess(const QString& line, LogLevel Level)
    {
        foreach(ILogger *log,Loggers)
            log->WriteSuccess(line, Level);

        emit NewLine();
    }

    void ComplexLogger::WriteSuccess(const QString& line)
    {
        WriteSuccess(line, LogUser);

    }

    void ComplexLogger::WriteInfo(const QString& line, LogLevel Level)
    {
        foreach(ILogger *log,Loggers)
            log->WriteInfo(line, Level);

        emit NewLine();
    }

    void ComplexLogger::WriteInfo(const QString& line)
    {
        WriteInfo(line, LogUser);

    }

}
