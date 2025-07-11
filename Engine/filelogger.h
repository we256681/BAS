#ifndef FILELOGGER_H
#define FILELOGGER_H
#include "engine_global.h"

#include "ilogger.h"
#include <QFile>

namespace BrowserAutomationStudioFramework
{

    class ENGINESHARED_EXPORT FileLogger : public ILogger
    {
        Q_OBJECT
        QFile *File;
        QString FileName;
        bool IsAddSpecialLineEnding;
        int FileIterator;
        int FlushNumber;
        int NewFileNumber;
        enum ColorId
        {
            Red,
            Green,
            Black,
            Gray
        };
        void WriteInternal(const QString& line, LogLevel Level, ColorId Color);
    public:
        explicit FileLogger(QObject *parent = 0);

        void AddSpecialLineEnding();

        ~FileLogger();
    signals:

    public slots:
        virtual QString GetFileName();
        virtual void SetFileName(const QString& FileName);
        virtual void Filter(QList<LogLevel> Level, int ThreadNumber, const QString& Filter, bool IsActive);

        virtual void Clear();
        virtual void Write(const QString& line, LogLevel Level);
        virtual void Write(const QString& line);
        virtual void WriteHtml(const QString& line_html, const QString& line_raw, LogLevel Level);
        virtual void WriteHtml(const QString& line_html, const QString& line_raw);
        virtual void WriteFail(const QString& line, LogLevel Level);
        virtual void WriteFail(const QString& line);
        virtual void WriteSuccess(const QString& line, LogLevel Level);
        virtual void WriteSuccess(const QString& line);
        virtual void WriteInfo(const QString& line, LogLevel Level);
        virtual void WriteInfo(const QString& line);
        virtual void SetReplaceActionIdWithLink();
        virtual void SetReplaceActionIdWithColor();

    };
}

#endif // FILELOGGER_H
