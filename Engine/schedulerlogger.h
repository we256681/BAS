#ifndef SCHEDULERLOGGER_H
#define SCHEDULERLOGGER_H
#include "engine_global.h"
#include "ilogger.h"
#include "ischedulerconnector.h"


namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT SchedulerLogger : public ILogger
    {
        Q_OBJECT
        ISchedulerConnector *SchedulerConnector;
        int ResultIndex;
        QString FileName;
        public:
            explicit SchedulerLogger(QObject *parent = 0);
            void SetSchedulerConnector(ISchedulerConnector *SchedulerConnector);
            void SetResultIndex(int Index);

        private:
            void WriteInternal(const QString& line,LogLevel Level);

        public slots:
            virtual QString GetFileName();
            virtual void SetFileName(const QString& FileName);
            virtual void Write(const QString& line, LogLevel Level);
            virtual void Write(const QString& line);
            virtual void Clear();
            virtual void WriteHtml(const QString& line_html, const QString& line_raw, LogLevel Level);
            virtual void WriteHtml(const QString& line_html, const QString& line_raw);
            virtual void WriteFail(const QString& line, LogLevel Level);
            virtual void WriteFail(const QString& line);
            virtual void WriteSuccess(const QString& line, LogLevel Level);
            virtual void WriteSuccess(const QString& line);
            virtual void WriteInfo(const QString& line, LogLevel Level);
            virtual void WriteInfo(const QString& line);
            virtual void Filter(QList<LogLevel> Level, int ThreadNumber, const QString& Filter, bool IsActive);
            virtual void SetReplaceActionIdWithLink();
            virtual void SetReplaceActionIdWithColor();


    };
}


#endif // SCHEDULERLOGGER_H
