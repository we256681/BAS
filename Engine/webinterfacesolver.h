#ifndef WEBINTERFACESOLVER_H
#define WEBINTERFACESOLVER_H

#include <QObject>
#include "engine_global.h"
#include "isolver.h"
#include "iwebinterface.h"


using namespace BrowserAutomationStudioFramework;

class ENGINESHARED_EXPORT WebInterfaceSolver : public ISolver
{
        Q_OBJECT
        IWebInterface *WebInterface;
        int IdCounter;
    public:
        explicit WebInterfaceSolver(QObject *parent = 0);
        void SetWebIntrface(IWebInterface *WebInterface);
    public slots:
        virtual bool TimeLimited();
        virtual QString Solve(const QString& base64,const QStringList& props);
        virtual void ReportBad(const QString& id);
    private slots:
        void ReceivedMessage(QString, QVariantMap, bool, int);

};

#endif // WEBINTERFACESOLVER_H
