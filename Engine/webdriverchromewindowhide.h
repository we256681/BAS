#ifndef WEBDRIVERCHROMEWINDOWHIDE_H
#define WEBDRIVERCHROMEWINDOWHIDE_H

#include <QObject>
#include "engine_global.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT WebDriverChromeWindowHide : public QObject
    {
        Q_OBJECT
        QList<QString> PIDS;
    private slots:
        void HideAndStart();

    public:
        explicit WebDriverChromeWindowHide(QObject *parent = 0);
        void Start();

    signals:

    public slots:
        void Hide();
        void AddPID(const QString& PID);
        void RemovePID(const QString& PID);

    };
}

#endif // WEBDRIVERCHROMEWINDOWHIDE_H
