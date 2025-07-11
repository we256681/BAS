#include "moduledll.h"
#include <QVariantMap>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QTimeZone>
#include <QRegularExpression>

extern "C" {

    void* StartDll()
    {
        return 0;
    }

    void EndDll(void * DllData)
    {

    }

    void* StartThread()
    {
        return 0;
    }

    void EndThread(void * DllData)
    {

    }

    void SystemTimezone(char *InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool *NeedToStop, bool* WasError)
    {
        const QDateTime dateTime1 = QDateTime::currentDateTime();
        const QDateTime dateTime2 = QDateTime(dateTime1.date(), dateTime1.time(), Qt::UTC);

        QByteArray ResArray = (QString::number(dateTime1.secsTo(dateTime2) / 60)).toUtf8();
        char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
        memcpy(ResMemory, ResArray.data(), ResArray.size());
    }

    void DateToString(char *InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool *NeedToStop, bool* WasError)
    {
        QJsonDocument InputObject;
        QJsonParseError err;
        InputObject = QJsonDocument::fromJson(QByteArray(InputJson),&err);
        if(err.error)
            return;

        if(!InputObject.object().contains("time"))
            return;

        if(!InputObject.object().contains("offset"))
            return;

        if(!InputObject.object().contains("format"))
            return;

        QString Format = InputObject.object()["format"].toString();
        QDateTime Data = QDateTime::fromString(InputObject.object()["time"].toString(), Qt::ISODate);
        int Offset = InputObject.object()["offset"].toInt();

        Data = Data.addSecs(Offset * 60);
        Data.setOffsetFromUtc(Offset * 60);

        QByteArray ResArray;
        if(Format == "N")
        {
            ResArray = (QString::number(Data.date().dayOfWeek())).toUtf8();
        }else if(Format == "Z")
        {
            ResArray = (QString::number(Data.date().dayOfYear())).toUtf8();
        }else
            ResArray = (Data.toString(Format)).toUtf8();
        char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
        memcpy(ResMemory, ResArray.data(), ResArray.size());
    }

    void StringToDate(char *InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool *NeedToStop, bool* WasError)
    {
        QJsonDocument InputObject;
        QJsonParseError err;
        InputObject = QJsonDocument::fromJson(QByteArray(InputJson),&err);
        if(err.error)
            return;

        if(!InputObject.object().contains("value"))
            return;

        if(!InputObject.object().contains("format"))
            return;

        QString Format = InputObject.object()["format"].toString();
        QString Value;
        qint64 DateInt;
        int TimezoneOffset = 0;

        if(InputObject.object()["value"].isString())
        {
            Value = InputObject.object()["value"].toString();
        }else if(InputObject.object()["value"].isDouble())
        {
            Value = QString::number(InputObject.object()["value"].toVariant().toLongLong());
        }

        bool IsAllDigits = !Value.isEmpty();
        for(int i = 0;i<Value.length();i++)
        {
            QChar c = Value.at(i);
            if(!c.isDigit())
            {
                IsAllDigits = false;
                break;
            }
        }

        if(IsAllDigits)
        {
            DateInt = Value.toLongLong();
        }else
        {
            if(Format == "auto")
            {
                QString ValueLower = Value.toLower();

                int Year = -1;
                int MonthNumber = -1;
                int DayNumber = -1;
                int AmOrPm = -1;
                QStringList Digits;
                int MinutesOffset = 0;
                int Hours = 0;
                int Minutes = 0;
                int Seconds = 0;
                int Milliseconds = 0;


                /* parse month */
                if(ValueLower.contains("ja"))
                    MonthNumber = 1;
                if(ValueLower.contains("fe"))
                    MonthNumber = 2;
                if(ValueLower.contains("mar"))
                    MonthNumber = 3;
                if(ValueLower.contains("ap"))
                    MonthNumber = 4;
                if(ValueLower.contains("may"))
                    MonthNumber = 5;
                if(ValueLower.contains("jun"))
                    MonthNumber = 6;
                if(ValueLower.contains("jul"))
                    MonthNumber = 7;
                if(ValueLower.contains("au"))
                    MonthNumber = 8;
                if(ValueLower.contains("se"))
                    MonthNumber = 9;
                if(ValueLower.contains("oc"))
                    MonthNumber = 10;
                if(ValueLower.contains("no"))
                    MonthNumber = 11;
                if(ValueLower.contains("de"))
                    MonthNumber = 12;


                /* am/pm */
                if(ValueLower.contains("am"))
                    AmOrPm = 1;
                if(ValueLower.contains("pm"))
                    AmOrPm = 2;

                /* extract timezone */
                QRegularExpression RegexpTimezon("([\\+\\-])(\\d\\d)\\:?(\\d\\d)");
                QRegularExpressionMatchIterator i = RegexpTimezon.globalMatch(ValueLower);
                while(i.hasNext())
                {
                    QRegularExpressionMatch MatchTimezone = i.next();
                    MinutesOffset = MatchTimezone.captured(3).toInt() + 60 * MatchTimezone.captured(2).toInt();
                    if(MinutesOffset < 840)
                    {
                        if(MatchTimezone.captured(1) == "-")
                        {
                            MinutesOffset *= -1;
                        }
                        ValueLower = ValueLower.replace(MatchTimezone.capturedStart(),MatchTimezone.capturedLength(),QString());
                        break;
                    }

                }

                /* extract time */
                QRegularExpression RegexpTime("(\\d?\\d)\\:(\\d?\\d)(\\:(\\d?\\d))?(\\.(\\d+))?");
                QRegularExpressionMatch MatchTime = RegexpTime.match(ValueLower);
                if (MatchTime.hasMatch())
                {
                    Hours = MatchTime.captured(1).toInt();
                    Minutes = MatchTime.captured(2).toInt();
                    if(MatchTime.capturedLength() > 4)
                        Seconds = MatchTime.captured(4).toInt();
                    if(MatchTime.capturedLength() > 6)
                        Milliseconds = MatchTime.captured(6).toInt();

                    ValueLower = ValueLower.replace(MatchTime.capturedStart(),MatchTime.capturedLength(),QString());
                }

                /* correct hours to am or pm */
                if(AmOrPm == 1)
                {
                    if(Hours == 12)
                        Hours = 0;
                }
                if(AmOrPm == 2)
                {
                    if(Hours != 12)
                        Hours += 12;
                }


                /* split rest of the string to digits */
                QRegularExpression RegexpDigits("\\d+");
                QRegularExpressionMatchIterator i2 = RegexpDigits.globalMatch(ValueLower);
                while(i2.hasNext())
                {
                    QRegularExpressionMatch match = i2.next();
                    Digits.append(match.captured(0));
                }
                Digits.removeAll(QString());


                /* try to find year and remove it */
                int FoundIndex = 0;
                bool Found = false;
                for(QString DigitString: Digits)
                {
                    int Digit = DigitString.toInt();

                    if(Digit > 32)
                    {
                        if(Digit > 1900)
                            Year = Digit;
                        else
                            Year = 1900 + Digit;
                        Found = true;
                        break;
                    }

                    FoundIndex++;
                }

                if(Found)
                    Digits.removeAt(FoundIndex);

                /* try to find day and remove it */
                FoundIndex = 0;
                Found = false;
                for(QString DigitString: Digits)
                {
                    int Digit = DigitString.toInt();

                    if(Digit > 12)
                    {
                        DayNumber = Digit;
                        Found = true;
                        break;
                    }

                    FoundIndex++;
                }

                if(Found)
                    Digits.removeAt(FoundIndex);


                for(QString DigitString: Digits)
                {
                    int Digit = DigitString.toInt();

                    if(DayNumber < 1)
                    {
                        DayNumber = Digit;
                    }else if(MonthNumber < 1)
                    {
                        MonthNumber = Digit;
                    }else if(MonthNumber < 1)
                    {
                        Year = Digit;
                    }else
                        break;
                }

                /* if date not found use standart */
                if(Year < 0)
                    Year = 2000;

                if(MonthNumber < 0)
                    MonthNumber = 1;

                if(DayNumber < 0)
                    DayNumber = 1;

                /* finally create date */


                QDate date(Year,MonthNumber,DayNumber);
                QTime time(Hours,Minutes,Seconds,Milliseconds);
                QDateTime DateTime(date,time);

                DateTime.setOffsetFromUtc(MinutesOffset * 60);
                DateInt = DateTime.toMSecsSinceEpoch();

                TimezoneOffset = MinutesOffset;

            }else
            {
                QDateTime DateTime = QDateTime::fromString(Value,Format);
                DateTime.setOffsetFromUtc(0);
                DateInt = DateTime.toMSecsSinceEpoch();
                TimezoneOffset = DateTime.offsetFromUtc() / 60;
            }
        }

        QByteArray ResArray = (QString::number(DateInt) + QString(",") + QString::number(TimezoneOffset)).toUtf8();
        char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
        memcpy(ResMemory, ResArray.data(), ResArray.size());

    }


}
