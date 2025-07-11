#ifndef ILOGGER_H
#define ILOGGER_H
#include "engine_global.h"

#include <QObject>

namespace BrowserAutomationStudioFramework
{
    enum LogLevel
    {
        LogAny,
        LogUser,
        LogSuccess,
        LogFail,
        LogInfo
    };
    class ENGINESHARED_EXPORT ILogger : public QObject
    {
    Q_OBJECT
    public:

        explicit ILogger(QObject *parent = 0);

    signals:
        void NewLine();
    public slots:
        virtual QString GetFileName() = 0;
        virtual void SetFileName(const QString& FileName) = 0;
        virtual void Write(const QString& line, LogLevel Level) = 0;
        virtual void Write(const QString& line) = 0;
        virtual void Clear() = 0;
        virtual void WriteHtml(const QString& line_html, const QString& line_raw, LogLevel Level) = 0;
        virtual void WriteHtml(const QString& line_html, const QString& line_raw) = 0;
        virtual void WriteFail(const QString& line, LogLevel Level) = 0;
        virtual void WriteFail(const QString& line) = 0;
        virtual void WriteSuccess(const QString& line, LogLevel Level) = 0;
        virtual void WriteSuccess(const QString& line) = 0;
        virtual void WriteInfo(const QString& line, LogLevel Level) = 0;
        virtual void WriteInfo(const QString& line) = 0;
        virtual void Filter(QList<LogLevel> Level, int ThreadNumber, const QString& Filter, bool IsActive) = 0;
        virtual void SetReplaceActionIdWithLink() = 0;
        virtual void SetReplaceActionIdWithColor() = 0;

    };
}

#endif // ILOGGER_H
