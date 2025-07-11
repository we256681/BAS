#ifndef SCHEDULEPREDICT_H
#define SCHEDULEPREDICT_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QJsonObject>
#include <QDateTime>

struct PredictData
{
    QStringList Data;
    bool IsSuccess;
    QString Message;
    bool HasMore = false;
};

class SchedulePredict : public QObject
{
    Q_OBJECT
public:
    explicit SchedulePredict(QObject *parent = nullptr);
    PredictData Predict(QJsonObject ScheduleObject);
private:
    QString PackDate(const QDateTime& Date);
    QDateTime UnpackDate(const QString& DateString, QDateTime Default = QDateTime());
    int ObjectToInterval(const QJsonObject& Object);
    bool CheckProbability(int Probability);
    int GetRanomBetween(int Start, int End);
};

#endif // SCHEDULEPREDICT_H
