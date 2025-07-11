#ifndef CHECKSCRIPT_H
#define CHECKSCRIPT_H
#include "engine_global.h"

#include <QObject>
#include "icheckscript.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT CheckScript : public ICheckScript
    {
        Q_OBJECT
    public:
        explicit CheckScript(QObject *parent = 0);
        virtual bool Check(const QString& Script);

    signals:

    public slots:

    };
}
#endif // CHECKSCRIPT_H
