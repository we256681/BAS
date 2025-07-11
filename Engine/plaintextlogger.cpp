#include "plaintextlogger.h"
#include <QApplication>
#include <QFileInfo>
#include <QRegularExpression>
#include <QRegexp>
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{

    PlainTextLogger::PlainTextLogger(QObject *parent) : ILogger(parent), TextEdit(0)
    {
        NumberOfLines = 0;
        LogSyntax = 0;
        ReplaceActionIdWithLink = false;
        ReplaceActionIdWithColor = false;
        HighlightSeparatorStart = "~~BAS~HIGHLIGHT~START~~";
        HighlightSeparatorEnd = "~~BAS~HIGHLIGHT~END~~";
    }

    void PlainTextLogger::FilterLine(QString& Line, bool& IsFiltered, LogLevel& Level, int& Color, bool IsExtendedString, bool DoHighlight, const QString& RawLine)
    {
        if(IsExtendedString)
        {
            if(Line.startsWith("[.]"))
            {
                Level = LogUser;
                Line = Line.remove(0,3);
            }
            if(Line.startsWith("[+]"))
            {
                Level = LogSuccess;
                Line = Line.remove(0,3);
            }
            if(Line.startsWith("[-]"))
            {
                Level = LogFail;
                Line = Line.remove(0,3);
            }
            if(Line.startsWith("[~]"))
            {
                Level = LogInfo;
                Line = Line.remove(0,3);
            }

            if(Line.startsWith("[black]"))
            {
                Color = 0;
                Line = Line.remove(0,7);
            }

            if(Line.startsWith("[red]"))
            {
                Color = 1;
                Line = Line.remove(0,5);
            }

            if(Line.startsWith("[green]"))
            {
                Color = 2;
                Line = Line.remove(0,7);
            }

            if(Line.startsWith("[gray]"))
            {
                Color = 3;
                Line = Line.remove(0,6);
            }
        }

        if(!FilterIsActive)
        {
            IsFiltered = false;
            return;
        }


        if(!FilterString.isEmpty() && FilterString != "*")
        {
            QRegExp Regexp(FilterString);
            Regexp.setPatternSyntax(QRegExp::Wildcard);
            int pos = 0;
            bool WasMatch = false;
            while ((pos = Regexp.indexIn(Line, pos)) != -1)
            {
                WasMatch = true;

                if(DoHighlight)
                {
                    Line = Line.insert(pos + Regexp.matchedLength(),HighlightSeparatorEnd);
                    Line = Line.insert(pos,HighlightSeparatorStart);

                    pos += HighlightSeparatorStart.length();
                    pos += HighlightSeparatorEnd.length();
                    pos += Regexp.matchedLength();
                }else
                {
                    break;
                }

            }

            if(!WasMatch)
            {
                IsFiltered = true;
                return;
            }

        }

        if(!FilterLevels.contains(Level))
        {
            IsFiltered = true;
            return;
        }

        if(FilterThreadNumber > 0)
        {
            int ThreadNumer = 0;
            QRegularExpression re("^(\\[[^\\]]+\\])?\\s*(\\[[^\\]]+\\])[^\\d\\[\\]]+(\\d+)");
            QString LineRegexp;
            if(!RawLine.isEmpty())
            {
                LineRegexp = RawLine;
            }else
            {
                LineRegexp = Line;
            }

            QRegularExpressionMatch match = re.match(LineRegexp);
            if (match.hasMatch())
            {
               ThreadNumer  = match.capturedTexts().last().toInt();
            }
            if(ThreadNumer != FilterThreadNumber)
            {
                IsFiltered = true;
                return;
            }
        }


        IsFiltered = false;
    }

    void PlainTextLogger::SetLogSyntax(ILogSyntax * LogSyntax)
    {
        this->LogSyntax = LogSyntax;
    }

    void PlainTextLogger::ApplyFiltering()
    {
        int PortionSize = 1024;
        QByteArray Separator = QString("~~CUSTOM~LOG~~").toUtf8();

        QStringList Lines;
        QByteArray LastLineString;
        qint64 Position = 0;
        if(!TextEdit)
            return;

        QFile file(FileName);
        if(!file.open(QIODevice::ReadOnly))
            return;

        Position = file.size();

        while(true)
        {
            int Size = Position;
            Position -= PortionSize;
            bool NeedBreak = Position <= 0;

            if(Position >= 0)
            {
                Size = PortionSize;
            }else
            {
                Position = 0;
            }

            file.seek(Position);

            QByteArray Read = file.read(Size);
            QByteArray Data = Read + LastLineString;

            bool StartsWithNewLine = Data.startsWith(Separator);
            QStringList list;
            while(true)
            {
                int index = Data.lastIndexOf(Separator);
                if(index<0)
                    break;

                QByteArray NewLine = Data.mid(index + Separator.size(), Data.length() - index - Separator.size());
                Data = Data.remove(index + Separator.size(), Data.length() - index - Separator.size());

                list.insert(0, QString::fromUtf8(NewLine));
                Data = Data.remove(index, Separator.size());
            }

            if(!StartsWithNewLine)
            {
                LastLineString = Data;
            }else
            {
                LastLineString.clear();
            }

            int it = 0;
            LogLevel Level;
            int Color;
            bool IsFiltered;
            QString UpdatedLine;
            QString CopyLine;
            for(QString &line:list)
            {
                CopyLine = line.trimmed();
                UpdatedLine = CopyLine;
                Color = 0;
                Level = LogUser;
                FilterLine(UpdatedLine,IsFiltered,Level,Color,true,false, QString());
                if(!IsFiltered)
                {
                    Lines.insert(it,CopyLine);
                    it++;
                }
                if(Lines.size() >= 300)
                {
                    NeedBreak = true;
                    break;
                }

            }
            if(NeedBreak)
            {
                if(!LastLineString.isEmpty() && Lines.size() < 300)
                {
                    CopyLine = QString::fromUtf8(LastLineString).trimmed();

                    UpdatedLine = CopyLine;
                    Color = 0;
                    Level = LogUser;
                    FilterLine(UpdatedLine,IsFiltered,Level,Color,true,false, QString());
                    if(!IsFiltered)
                    {
                        Lines.insert(0,CopyLine);
                    }
                }
                break;
            }
        }

        Clear();

        for(QString& Line: Lines)
        {
            LogLevel Level = LogUser;
            int Color = 0;
            bool IsFiltered;
            QString UpdatedLine = Line;
            FilterLine(UpdatedLine,IsFiltered,Level,Color,true,false, QString());
            if(LogSyntax)
            {
                QString UpdatedLineFormatted = LogSyntax->Process(UpdatedLine);
                WriteHtml(UpdatedLineFormatted,UpdatedLine,Level);
            }else
            {
                if(Color == 0)
                {
                    Write(UpdatedLine,Level);
                }
                if(Color == 1)
                {
                    WriteFail(UpdatedLine,Level);
                }
                if(Color == 2)
                {
                    WriteSuccess(UpdatedLine,Level);
                }
                if(Color == 3)
                {
                    WriteInfo(UpdatedLine,Level);
                }
            }
        }
    }


    void PlainTextLogger::SetReplaceActionIdWithLink()
    {
        ReplaceActionIdWithLink = true;
    }
    void PlainTextLogger::SetReplaceActionIdWithColor()
    {
        ReplaceActionIdWithColor = true;
    }


    QString PlainTextLogger::GetFileName()
    {
        return FileName;
    }

    void PlainTextLogger::SetFileName(const QString& FileName)
    {
        this->FileName = QFileInfo(FileName).absoluteFilePath();
    }

    void PlainTextLogger::Filter(QList<LogLevel> Level, int ThreadNumber, const QString& FilterString, bool IsActive)
    {
        bool NeedApply;
        if(!this->FilterIsActive && !IsActive)
        {
            NeedApply = false;
        }else
        {
            NeedApply = !(this->FilterLevels == Level && this->FilterThreadNumber == ThreadNumber && this->FilterString == FilterString && this->FilterIsActive == IsActive);
        }
        this->FilterLevels = Level;
        this->FilterThreadNumber = ThreadNumber;
        this->FilterString = FilterString;
        this->FilterIsActive = IsActive;

        if(NeedApply)
            ApplyFiltering();
    }

    void PlainTextLogger::SetPlainTextElement(QTextBrowser * TextEdit)
    {
        this->TextEdit = TextEdit;
    }

    QTextBrowser * PlainTextLogger::GetSetPlainTextElement()
    {
        return TextEdit;
    }

    void PlainTextLogger::WriteInternal(const QString& line,const QString& suffix,const QString& prefix, bool escape, LogLevel Level, const QString& raw)
    {
        if(!TextEdit)
            return;

        QString UpdatedLine = line;

        if(FilterIsActive)
        {
            LogLevel ParsedLevel = Level;
            int Color = 0;
            bool IsFiltered;
            FilterLine(UpdatedLine,IsFiltered,ParsedLevel,Color,false,true,raw);
            if(IsFiltered)
                return;

        }

        if(NumberOfLines>=1000)
        {
            NumberOfLines = 100;
            TextEdit->setHtml("");
            foreach(QString str, Tail)
                TextEdit->append(str);
            Tail.clear();
        }else if(NumberOfLines>=900)
        {
            Tail.append(UpdatedLine);
        }

        NumberOfLines++;
        emit NewLine();
        QString res = UpdatedLine;

        if(escape)
        {
            res.replace("&","&amp;").replace("\"","&quot;");
            res.replace("<","&lt;").replace(">","&gt;");
        }

        if(FilterIsActive)
            res.replace(HighlightSeparatorStart,"<span style='background-color:yellow;color:black'>").replace(HighlightSeparatorEnd,"</span>");

        QRegExp ReplaceFiles("openfile:\\/\\/\\/([^\\s]+)");
        res = res.replace(ReplaceFiles,"<a href='file:///\\1' style='color:gray;'>\\1</a>");


        QRegExp ReplaceIds("^\\[(\\d+)\\]");

        if(ReplaceActionIdWithLink)
            res = res.replace(ReplaceIds,"<a href='action://action\\1' style='color:gray;'>[\\1]</a>");
        else if (ReplaceActionIdWithColor)
            res = res.replace(ReplaceIds,"<span style='color:gray;'>[\\1]</span>");

        TextEdit->append(suffix + res + prefix);
    }

    void PlainTextLogger::WriteHtml(const QString& line_html, const QString& line_raw, LogLevel Level)
    {
        WriteInternal(line_html,"","",false, Level,line_raw);
    }

    void PlainTextLogger::WriteHtml(const QString& line_html, const QString& line_raw)
    {
        WriteHtml(line_html, line_raw, LogUser);
    }

    void PlainTextLogger::Write(const QString& line, LogLevel Level)
    {
        if(Color.isEmpty())
        {
            Color = QApplication::palette().color(QPalette::Text).name();
        }
        WriteInternal(line,QString("<font color='%1'>").arg(Color),"</font>",true, Level,QString());
    }

    void PlainTextLogger::Write(const QString& line)
    {
        Write(line, LogUser);
    }

    void PlainTextLogger::Clear()
    {
        TextEdit->clear();
        NumberOfLines = 0;
        Tail.clear();
        Color.clear();
    }

    void PlainTextLogger::WriteFail(const QString& line, LogLevel Level)
    {
        WriteInternal(line,"<font color='red'>","</font>",true, Level,QString());
    }

    void PlainTextLogger::WriteFail(const QString& line)
    {
        WriteFail(line, LogUser);
    }

    void PlainTextLogger::WriteSuccess(const QString& line, LogLevel Level)
    {
        WriteInternal(line,"<font color='#1c1'>","</font>",true, Level,QString());
    }

    void PlainTextLogger::WriteSuccess(const QString& line)
    {
        WriteSuccess(line, LogUser);
    }

    void PlainTextLogger::WriteInfo(const QString& line, LogLevel Level)
    {
        WriteInternal(line,"<font color='gray'>","</font>",true, Level,QString());
    }

    void PlainTextLogger::WriteInfo(const QString& line)
    {
        WriteInfo(line, LogUser);
    }
}
