#ifndef PROFILEBACKGROUNDREMOVER_H
#define PROFILEBACKGROUNDREMOVER_H

#include <QObject>
#include <QTimer>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include "engine_global.h"


class ENGINESHARED_EXPORT ProfileBackgroundRemover : public QObject
{
    Q_OBJECT
    QFuture<void> Future;
    bool IsRunning = false;
    bool EndedAtLeastOnce = false;
    QDateTime LastEndTime;
    QTimer *_Timer;

public:
    explicit ProfileBackgroundRemover(QObject *parent = nullptr);
    void Run(bool immediate = false);
private slots:
    void Timer();
signals:

};

#endif // PROFILEBACKGROUNDREMOVER_H
