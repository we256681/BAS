#ifndef IPCRESOURCESSMOOTHUSAGE_H
#define IPCRESOURCESSMOOTHUSAGE_H
#include "engine_global.h"

#include <QObject>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT IPCResourcesSmoothUsage : public QObject
    {
        Q_OBJECT
    public:
        explicit IPCResourcesSmoothUsage(QObject *parent = 0);

    signals:
        void Log(const QString& Text);
        void CanStartBrowser(int BrowserId);
    public slots:
        virtual void NeedToStartBrowser(int BrowserId) = 0;
        virtual void BrowserStarted(int BrowserId) = 0;

    };
}

#endif // IPCRESOURCESSMOOTHUSAGE_H
