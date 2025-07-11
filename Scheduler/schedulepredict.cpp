#include "schedulepredict.h"
#include <QDateTime>
#include <QJsonArray>
#include <algorithm>

SchedulePredict::SchedulePredict(QObject *parent) : QObject(parent)
{

}

QString SchedulePredict::PackDate(const QDateTime& Date)
{
    return QString::number(Date.toMSecsSinceEpoch());
}

QDateTime SchedulePredict::UnpackDate(const QString& DateString, QDateTime Default)
{
    if(DateString.isEmpty())
        return Default;
    return QDateTime::fromMSecsSinceEpoch(DateString.toLongLong());
}

int SchedulePredict::ObjectToInterval(const QJsonObject& Object)
{
    int res = 0;
    if(Object.contains("minute"))
    {
        res += 60 * Object.value("minute").toInt();
    }

    if(Object.contains("hour"))
    {
        res += 60 * 60 * Object.value("hour").toInt();
    }

    if(Object.contains("day"))
    {
        res += 24 * 60 * 60 * (Object.value("day").toInt() - 1);
    }

    if(Object.contains("week"))
    {
        res += 24 * 60 * 60 * (Object.value("week").toInt() - 1);
    }
    return res;
}

bool SchedulePredict::CheckProbability(int Probability)
{
    return qrand() % 100 < Probability;
}

int SchedulePredict::GetRanomBetween(int Start, int End)
{
    if(Start == End)
        return Start;
    int Big;
    int Small;
    if(Start > End)
    {
        Big = Start;
        Small = End;
    }else
    {
        Small = Start;
        Big = End;
    }
    return qrand() % (Big - Small + 1) + Small ;

}

PredictData SchedulePredict::Predict(QJsonObject ScheduleObject)
{
    PredictData Data;
    QDateTime Now = QDateTime::currentDateTime();

    if(!ScheduleObject.contains("type"))
    {
        Data.IsSuccess = false;
        Data.Message = "Type is not selected";
        return Data;
    }

    QString Type = ScheduleObject.value("type").toString();
    if(Type == "now")
    {
        Data.IsSuccess = true;
        Data.Data.append(PackDate(Now));
        return Data;
    }

    if(Type == "at")
    {
        if(!ScheduleObject.contains("date_start"))
        {
            Data.IsSuccess = false;
            Data.Message = "Date start is not selected";
            return Data;
        }

        Data.IsSuccess = true;
        Data.Data.append(ScheduleObject.value("date_start").toString());
        return Data;
    }

    if(Type == "interval")
    {
        //Read params
        if(!ScheduleObject.contains("interval"))
        {
            Data.IsSuccess = false;
            Data.Message = "Interval is not selected";
            return Data;
        }

        int IntervalMinutes = ScheduleObject.value("interval").toInt();

        int MaximumStartNumber = 10000;
        if(ScheduleObject.contains("max_start_number"))
        {
            MaximumStartNumber = ScheduleObject.value("max_start_number").toInt();
        }

        QDateTime StartDate;
        if(ScheduleObject.contains("date_start"))
        {
            StartDate = UnpackDate(ScheduleObject.value("date_start").toString());
        }
        if(!StartDate.isValid())
            StartDate = Now;

        if(StartDate < Now)
            StartDate = Now;


        QDateTime EndDate;
        if(ScheduleObject.contains("date_end"))
        {
            EndDate = UnpackDate(ScheduleObject.value("date_end").toString());
        }
        if(!EndDate.isValid())
            EndDate = QDateTime(QDate(2200, 1, 1), QTime(0, 0, 0));

        QDateTime CurrentDate = StartDate;

        while(true)
        {
            //Has more
            if(Data.Data.size() >= 10)
            {
                Data.IsSuccess = true;
                Data.HasMore = true;
                return Data;
            }

            //Finished because of limit
            if(CurrentDate >= EndDate || Data.Data.size() >= MaximumStartNumber)
            {
                if(Data.Data.isEmpty())
                {
                    Data.IsSuccess = false;
                    Data.Message = "List is empty";
                    return Data;
                }else
                {
                    Data.IsSuccess = true;
                    return Data;
                }
            }

            Data.Data.append(PackDate(CurrentDate));
            CurrentDate = CurrentDate.addSecs(IntervalMinutes);
        }


    }

    if(Type == "hour" || Type == "day" || Type == "week" || Type == "month")
    {
        bool IsHour = Type == "hour";
        bool IsDay = Type == "day";
        bool IsWeek = Type == "week";
        bool IsMonth = Type == "month";

        QString SectionsObject = QString("sections_") + Type;

        //Read params
        if(!ScheduleObject.contains(SectionsObject))
        {
            Data.IsSuccess = false;
            Data.Message = "Intervals is not selected";
            return Data;
        }

        QJsonArray JsonSections = ScheduleObject.value(SectionsObject).toArray();

        int MaximumStartNumber = 10000;
        if(ScheduleObject.contains("max_start_number"))
        {
            MaximumStartNumber = ScheduleObject.value("max_start_number").toInt();
        }

        QDateTime StartDate;
        if(ScheduleObject.contains("date_start"))
        {
            StartDate = UnpackDate(ScheduleObject.value("date_start").toString());
        }
        if(!StartDate.isValid())
            StartDate = Now;

        if(StartDate < Now)
            StartDate = Now;


        QDateTime EndDate;
        if(ScheduleObject.contains("date_end"))
        {
            EndDate = UnpackDate(ScheduleObject.value("date_end").toString());
        }
        if(!EndDate.isValid())
            EndDate = QDateTime(QDate(2200, 1, 1), QTime(0, 0, 0));

        //Setup intervals
        QDateTime CurrentDate = StartDate;
        int SecondsInterval = 0;
        int MonthInterval = 0;

        if(IsHour)
        {
            CurrentDate = QDateTime(CurrentDate.date(),QTime(CurrentDate.time().hour(),0));
            SecondsInterval = 60 * 60; //1 hour
        }

        if(IsDay)
        {
            CurrentDate = QDateTime(CurrentDate.date(),QTime(0,0));
            SecondsInterval = 24 * 60 * 60; //1 day
        }

        if(IsWeek)
        {
            int Offset = CurrentDate.date().dayOfWeek() - 1;
            CurrentDate = QDateTime(CurrentDate.date().addDays(-Offset),QTime(0,0));
            SecondsInterval = 7 * 24 * 60 * 60; //1 week
        }

        if(IsMonth)
        {
            CurrentDate = QDateTime(QDate(CurrentDate.date().year(),CurrentDate.date().month(),1),QTime(0,0));
            MonthInterval = 1; //1 month
        }

        QList<QDateTime> ResList;
        int Index = 0;

        //Predict launches
        while(
              ResList.size() < 20 &&
              ResList.size() < MaximumStartNumber &&
              CurrentDate < EndDate &&
              Index < 10000
              )
        {
            QDateTime Start = CurrentDate;
            QDateTime End = CurrentDate;

            End = End.addSecs(SecondsInterval);
            End = End.addMonths(MonthInterval);

            for(QJsonValueRef SectionValue:JsonSections)
            {
                QJsonObject SectionObject = SectionValue.toObject();
                int StartOffset = ObjectToInterval(SectionObject.value("start").toObject());
                int EndOffset = ObjectToInterval(SectionObject.value("end").toObject());
                int Probability = SectionObject.value("probability").toInt();
                if(CheckProbability(Probability))
                {
                    int Value = GetRanomBetween(StartOffset, EndOffset);
                    QDateTime InsertDate = CurrentDate.addSecs(Value);
                    if(InsertDate >= Now && InsertDate >= StartDate && InsertDate <= EndDate && InsertDate >= Start && InsertDate <= End)
                    {
                       ResList.append(InsertDate);
                    }
                }

            }

            CurrentDate = End;

            Index ++;
        }

        std::sort(ResList.begin(), ResList.end());

        for(QDateTime & Item: ResList)
        {
            //Has more
            if(Data.Data.size() >= 10)
            {
                Data.IsSuccess = true;
                Data.HasMore = true;
                return Data;
            }

            //Finished because of limit
            if(Item >= EndDate || Data.Data.size() >= MaximumStartNumber)
            {
                if(Data.Data.isEmpty())
                {
                    Data.IsSuccess = false;
                    Data.Message = "List is empty";
                    return Data;
                }else
                {
                    Data.IsSuccess = true;
                    return Data;
                }
            }

            Data.Data.append(PackDate(Item));
        }

        if(Data.Data.isEmpty())
        {
            Data.IsSuccess = false;
            Data.Message = "List is empty";
            return Data;
        }else
        {
            Data.IsSuccess = true;
            return Data;
        }
    }


    if(Type == "list")
    {
        //Read params
        if(!ScheduleObject.contains("date_start_list"))
        {
            Data.IsSuccess = false;
            Data.Message = "Date list is empty";
            return Data;
        }

        QJsonArray ArrayDates = ScheduleObject.value("date_start_list").toArray();
        QList<QDateTime> DateList;

        for(QJsonValueRef DateValue:ArrayDates)
        {
            QString StringDate = DateValue.toString();
            QDateTime Date = UnpackDate(StringDate);
            DateList.append(Date);
        }

        std::sort(DateList.begin(), DateList.end());

        for(QDateTime & Item: DateList)
        {
            //Has more
            if(Data.Data.size() >= 10)
            {
                Data.IsSuccess = true;
                Data.HasMore = true;
                return Data;
            }

            Data.Data.append(PackDate(Item));
        }

        if(Data.Data.isEmpty())
        {
            Data.IsSuccess = false;
            Data.Message = "List is empty";
            return Data;
        }else
        {
            Data.IsSuccess = true;
            return Data;
        }


    }

    if(Type == "event")
    {
        Data.IsSuccess = false;
        Data.Message = "Next run time depends on other task";
        return Data;
    }

    Data.IsSuccess = false;
    Data.Message = "Unknown type";
    return Data;
}
