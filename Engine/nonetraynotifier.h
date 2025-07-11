#ifndef NONETRAYNOTIFIER_H
#define NONETRAYNOTIFIER_H
#include "engine_global.h"
#include "isystemtraynotifier.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT NoneTrayNotifier : public ISystemTrayNotifier
    {
        Q_OBJECT
    public:
        explicit NoneTrayNotifier(QObject *parent = 0);
    public slots:
        virtual void Init();
        virtual void Start();
        virtual void Stop();
        virtual void ShowMessage(const QString& title,const QString& message);
        virtual void SetContextmenu(QMenu * menu);
        virtual void SetCustomIcon(QIcon Icon);
    };
}

#endif // NONETRAYNOTIFIER_H
