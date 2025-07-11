#ifndef SCHEDULERSOLVER_H
#define SCHEDULERSOLVER_H

#include <QObject>
#include "engine_global.h"
#include "isolver.h"
#include "ischedulerconnector.h"


using namespace BrowserAutomationStudioFramework;

class ENGINESHARED_EXPORT SchdulerSolver : public ISolver
{
        Q_OBJECT
        ISchedulerConnector *SchedulerConnector;
    public:
        explicit SchdulerSolver(QObject *parent = 0);
        void SetSchdulerConnector(ISchedulerConnector *SchedulerConnector);
    public slots:
        virtual bool TimeLimited();
        virtual QString Solve(const QString& base64,const QStringList& props);
        virtual void ReportBad(const QString& id);
    private slots:
        void ReceivedMessage(QString TagName, QString Content, QMap<QString,QString> Params);
        QString GetRandomString();

};

#endif // WEBINTERFACESOLVER_H
