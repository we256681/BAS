#include "filelogger.h"
#include <QTextStream>
#include <QDir>
#include <QFileInfo>
#include "every_cpp.h"
namespace BrowserAutomationStudioFramework
{

    FileLogger::FileLogger(QObject *parent) : ILogger(parent)
    {
        IsAddSpecialLineEnding = false;
    }

    void FileLogger::SetReplaceActionIdWithLink()
    {

    }
    void FileLogger::SetReplaceActionIdWithColor()
    {

    }

    QString FileLogger::GetFileName()
    {
        QFileInfo info(FileName);

        return info.absoluteFilePath();
    }

    void FileLogger::Clear()
    {
        File->remove();
        File->close();
        File->open(QIODevice::WriteOnly | QIODevice::Append);
    }

    void FileLogger::Filter(QList<LogLevel> Level, int ThreadNumber, const QString& Filter, bool IsActive)
    {

    }

    void FileLogger::SetFileName(const QString& FileName)
    {
        QFileInfo info(FileName);
        this->FileName = FileName;
        FileIterator = 0;
        info.dir().mkpath(".");
        File = new QFile(FileName);
        FlushNumber = 0;
        NewFileNumber = 0;
        File->open(QIODevice::WriteOnly | QIODevice::Append);
    }

    FileLogger::~FileLogger()
    {
        File->close();
        delete File;
    }

    void FileLogger::WriteHtml(const QString& line_html, const QString& line_raw, LogLevel Level)
    {
        WriteInternal(line_raw, Level, Black);
    }

    void FileLogger::WriteHtml(const QString& line_html, const QString& line_raw)
    {
        WriteHtml(line_html, line_raw, LogUser);
    }

    void FileLogger::Write(const QString& line, LogLevel Level)
    {
        WriteInternal(line,Level, Black);
    }

    void FileLogger::Write(const QString& line)
    {
        Write(line,LogUser);
    }

    void FileLogger::AddSpecialLineEnding()
    {
        IsAddSpecialLineEnding = true;
    }


    void FileLogger::WriteInternal(const QString& line, LogLevel Level, ColorId Color)
    {

        //Write
        QTextStream out(File);
        out.setCodec("UTF-8");
        if(IsAddSpecialLineEnding)
        {
            out<<"~~CUSTOM~LOG~~";
            switch(Level)
            {
                case LogUser: out<<"[.]"; break;
                case LogSuccess: out<<"[+]"; break;
                case LogFail: out<<"[-]"; break;
                case LogInfo: out<<"[~]"; break;
            }

            switch(Color)
            {
                case Black: out<<"[black]"; break;
                case Red: out<<"[red]"; break;
                case Green: out<<"[green]"; break;
                case Gray: out<<"[gray]"; break;
            }
        }
        out<<line<<"\r\n";


        //Increase counters
        FlushNumber++;
        NewFileNumber++;

        //Flush
        if(FlushNumber >= 100)
        {
            FlushNumber = 0;
            File->flush();
        }
        //CreateNewFile
        //if(NewFileNumber >= 1000000)
        if(false)
        {
            FileIterator++;
            QFileInfo info(FileName);
            QString suffix = info.suffix();
            QString OldFileName = FileName;
            QString NewFileName = FileName.replace(suffix, QString::number(FileIterator) + "." + suffix);
            FileName = OldFileName;
            NewFileNumber = 0;
            File->close();
            delete File;
            File = new QFile(NewFileName);
            File->open(QIODevice::WriteOnly | QIODevice::Append);

        }
        emit NewLine();

    }

    void FileLogger::WriteFail(const QString& line, LogLevel Level)
    {
        WriteInternal(line, Level, Red);
    }

    void FileLogger::WriteFail(const QString& line)
    {
        WriteFail(line, LogUser);
    }

    void FileLogger::WriteSuccess(const QString& line, LogLevel Level)
    {
        WriteInternal(line, Level, Green);
    }

    void FileLogger::WriteSuccess(const QString& line)
    {
        WriteSuccess(line, LogUser);
    }

    void FileLogger::WriteInfo(const QString& line, LogLevel Level)
    {
        WriteInternal(line, Level, Gray);
    }

    void FileLogger::WriteInfo(const QString& line)
    {
        WriteInfo(line, LogUser);
    }

}
