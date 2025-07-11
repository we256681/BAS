#ifndef WEBINTERFACE_H
#define WEBINTERFACE_H

#include "engine_global.h"
#include "iwebinterface.h"
#include "ihttpclient.h"

#include <QObject>
#include <QFile>
#include <QIcon>
#include <QtWebSockets/QtWebSockets>


namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT WebInterface : public IWebInterface
    {

        Q_OBJECT
        QString FileName;
        QString ScriptName;
        int ScriptId;
        QString Server;
        bool HasInterface;
        IHttpClient* HttpClient;
        QFile *LoginInterfaceFile;
        int WebSocketPort;
        bool IsNoConnection;
        QWebSocketServer *WebSocketServer;
        QWebSocket * Socket;
        QString Buffer;
        QString LastMessageType;
        QVariantMap LastMessageData;
        QIcon Icon;
        QImage IconImage;
        bool IsCustomIcon;
        bool TestingMode;
        bool WebInterfaceIsRememberChoice;
        bool WebInterfaceRememberChoiceUseCustom;
        QString PremiumUsername;
        QString PremiumPassword;
        QString Html;
        QString HtmlMainInterface;
        int StartType;
        bool IsDebug;
        bool IsSilent;
        struct BrowserItem
        {
            QString Id;
            bool Visible = false;
            bool ManualControl = false;
        };
        struct Task
        {
            QString Type;
            QVariantMap Data;
            bool IsAsync;
            int Id;
        };
        QMap<QString,BrowserItem> BrowserItems;
        int WindowX;
        int WindowY;
        int WindowWidth;
        int WindowHeight;

        bool IsRemoteControl;
        bool FailedToGetLoginInterface;
        QString RemoteControlScript;
        QString RemoteControlLogin;
        QString RemoteControlPassword;

        QString GetWorkerFolderName();

    public:
        explicit WebInterface(QObject *parent = 0);
        ~WebInterface();

        void SetFileName(const QString& FileName);

        void SetTesingMode();
        void SetLastChoice(bool WebInterfaceIsRememberChoice,bool WebInterfaceRememberChoiceUseCustom,const QString& PremiumUsername, const QString& PremiumPassword);

        bool GetWebInterfaceIsRememberChoice();
        bool GetWebInterfaceRememberChoiceUseCustom();
        QString GetPremiumUsername();
        QString GetPremiumPassword();
        QString GetHtmlInterface();
        void SetScriptName(const QString& ScriptName);

        void SetHttpClient(IHttpClient* HttpClient);
        IHttpClient* GetHttpClient();

        void OnNewMessage(const QString& MessageType, const QJsonObject& Params);

        virtual bool HasWebInterface();
        virtual bool Init(bool IsSilent);
        virtual bool GetFailedToGetLoginInterface();
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
                          const QVariant& Param4Value = QVariant());
        virtual void SendRaw(const QString& MessageType,
                          bool IsAsync,
                          int Id,
                          const QVariant& Params);

        virtual QString GetLastMessageType();
        virtual QVariantMap GetLastMessageData();
        virtual QVariantMap WaitForMessage(const QString& MessageType);
        virtual bool IsConnected();
        virtual QIcon GetIcon();
        virtual bool GetIsCustomIcon();
        void Close();

        virtual bool InitAsRemoteControl(int Port);
        virtual void SetRemoteControlData(const QString& Script, const QString& Login, const QString& Password);
        virtual bool GetIsRemoteControl();
        virtual QString GetRemoteControlScript();
        virtual QString GetRemoteControlLogin();
        virtual QString GetRemoteControlPassword();

    private:
        QString GetRandomString();

    signals:
        void Exit();
    private slots:
        void LoginInterfaceLoadFinished();
        void NewConnection();
        void Disconnected();
        void Received(QString Message);
        void StartServer();
        void LoginWithDialog();
        void LoginWithDialogResult();
        void AutoLogin();
        void AutoLoginResult();

    };
}


#endif // WEBINTERFACE_H
