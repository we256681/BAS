#ifndef PROCESSDELETER_H
#define PROCESSDELETER_H

#include <QObject>
#include <QProcess>
#include <QTimer>

class ProcessDeleter : public QObject
{
    Q_OBJECT
    QProcess * Process;
    bool IsTemporaryProfile;
public:
    ProcessDeleter(bool IsTemporaryProfile);
    void Start(QProcess * Process);
public slots:
    void Timer();
};

#endif // PROCESSDELETER_H
