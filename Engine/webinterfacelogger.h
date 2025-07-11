#ifndef WEBINTERFACELOGGER_H
#define WEBINTERFACELOGGER_H
#include "engine_global.h"
#include "ilogger.h"
#include "iwebinterface.h"


namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT WebInterfaceLogger : public ILogger
    {
        Q_OBJECT
        QString FileName;
        IWebInterface *WebInterface;
        int ResultIndex;
        public:
            explicit WebInterfaceLogger(QObject *parent = 0);
            void SetWebInterface(IWebInterface *WebInterface);
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

#endif // WEBINTERFACELOGGER_H
