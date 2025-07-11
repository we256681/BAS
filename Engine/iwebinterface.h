#ifndef IWEBINTERFACE_H
#define IWEBINTERFACE_H

#include "engine_global.h"

#include <QObject>
#include <QVariant>
#include "engine_global.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT IWebInterface : public QObject
    {
        Q_OBJECT
    public:
        explicit IWebInterface(QObject *parent = nullptr);
        virtual bool HasWebInterface() = 0;
        virtual bool Init(bool IsSilent) = 0;
        virtual bool GetFailedToGetLoginInterface() = 0;
        virtual void Send(const QString& MessageType,
                          bool IsAsync = false,
                          int Id = 0,
                          const QString& Param1Name = QString(),
                          const QVariant& Param1Value = QVariant(),
                          const QString& Param2Name = QString(),
                          const QVariant& Param2Value = QVariant(),
                          const QString& Param3Name = QString(),
                          const QVariant& Param3Value = QVariant(),
                          const QString& Param4Name = QString(),
                          const QVariant& Param4Value = QVariant()
                ) = 0;
        virtual void SendRaw(const QString& MessageType,
                          bool IsAsync,
                          int Id,
                          const QVariant& Params) = 0;

        virtual QString GetLastMessageType() = 0;
        virtual QVariantMap GetLastMessageData() = 0;
        virtual QVariantMap WaitForMessage(const QString& MessageType) = 0;
        virtual bool IsConnected() = 0;
        virtual QIcon GetIcon() = 0;
        virtual bool GetIsCustomIcon() = 0;



        virtual bool InitAsRemoteControl(int Port) = 0;
        virtual bool GetIsRemoteControl() = 0;
        virtual void SetRemoteControlData(const QString& Script, const QString& Login, const QString& Password) = 0;
        virtual QString GetRemoteControlScript() = 0;
        virtual QString GetRemoteControlLogin() = 0;
        virtual QString GetRemoteControlPassword() = 0;
    signals:
        void Initialized();
        void ReceivedMessage(QString, QVariantMap, bool, int);

    public slots:
    };
}

#endif // IWEBINTERFACE_H
