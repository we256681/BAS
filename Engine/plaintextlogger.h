#ifndef PLAINTEXTLOGGER_H
#define PLAINTEXTLOGGER_H
#include "engine_global.h"

#include "ilogger.h"
#include "ilogsyntax.h"
#include <QTextBrowser>

namespace BrowserAutomationStudioFramework
{

    class ENGINESHARED_EXPORT PlainTextLogger : public ILogger
    {
    Q_OBJECT
        QTextBrowser * TextEdit;
        ILogSyntax *LogSyntax;
        int NumberOfLines;
        QStringList Tail;
        QString Color;
        bool ReplaceActionIdWithLink;
        bool ReplaceActionIdWithColor;
        QString FileName;

        QList<LogLevel> FilterLevels;
        int FilterThreadNumber = 0;
        QString FilterString;
        bool FilterIsActive = false;

        QString HighlightSeparatorStart;
        QString HighlightSeparatorEnd;

        void WriteInternal(const QString& line,const QString& suffix,const QString& prefix, bool escape, LogLevel Level, const QString& raw);
        void ApplyFiltering();
        void FilterLine(QString& Line, bool& IsFiltered, LogLevel& Level, int& Color, bool IsExtendedString, bool DoHighlight, const QString& RawLine);
    public:
        explicit PlainTextLogger(QObject *parent = 0);

        void SetPlainTextElement(QTextBrowser * TextEdit);
        void SetLogSyntax(ILogSyntax * LogSyntax);
        QTextBrowser * GetSetPlainTextElement();

    signals:

    public slots:

        virtual QString GetFileName();
        virtual void SetFileName(const QString& FileName);
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

        virtual void Filter(QList<LogLevel> Level, int ThreadNumber, const QString& FilterString, bool IsActive);
        virtual void SetReplaceActionIdWithLink();
        virtual void SetReplaceActionIdWithColor();
    };
}

#endif // PLAINTEXTLOGGER_H
