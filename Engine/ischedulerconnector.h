#ifndef ISCHEDULERCONNECTOR_H
#define ISCHEDULERCONNECTOR_H
#include "engine_global.h"

#include <QObject>
#include <QMap>

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT ISchedulerConnector : public QObject
    {
        Q_OBJECT
    public:
        explicit ISchedulerConnector(QObject *parent = 0);
        virtual void StartIfNeeded(const QStringList& Params) = 0;
        virtual void Start(const QString& Id) = 0;
        virtual bool GetIsActive() = 0;
        virtual void MessageSend(const QString& Message) = 0;
        virtual void MessageSendFormat(const QString& TagName, const QString& Content = QString(),
                                       const QString& AttributeName1 = QString(), const QString& AttributeData1 = QString(),
                                       const QString& AttributeName2 = QString(), const QString& AttributeData2 = QString(),
                                       const QString& AttributeName3 = QString(), const QString& AttributeData3 = QString()) = 0;


    signals:
        void OnClose();
        void OnVisibilityToggle();
        void OnMessage(QString TagName, QString Content, QMap<QString,QString> Params);
    public slots:

    };
}

#endif // ISCHEDULERCONNECTOR_H
