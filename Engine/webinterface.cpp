#include "webinterface.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QJsonDocument>
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>
#include <QJsonObject>
#include <QDir>
#include <QApplication>
#include <QRegularExpression>
#include <QMessageBox>
#include "askifrunwithwebinterfacedialog.h"
#include "premiumlogindialog.h"
#include "browserversionselector.h"

#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    WebInterface::WebInterface(QObject *parent) :
        IWebInterface(parent)
    {
        HasInterface = false;
        LoginInterfaceFile = 0;
        WebSocketServer = 0;
        IsNoConnection = true;
        Socket = 0;
        IsCustomIcon = false;
        TestingMode = false;
        WebInterfaceIsRememberChoice = false;
        WebInterfaceRememberChoiceUseCustom = false;
        IsDebug = false;
        WindowX = 10000;
        WindowY = 10000;
        WindowWidth = 10000;
        WindowHeight = 10000;
        IsRemoteControl = false;
    }

    WebInterface::~WebInterface()
    {
        Close();
    }

    QString WebInterface::GetLastMessageType()
    {
        return LastMessageType;
    }
    QVariantMap WebInterface::GetLastMessageData()
    {
        return LastMessageData;
    }
    QVariantMap WebInterface::WaitForMessage(const QString& MessageType)
    {
        while(true)
        {
            QEventLoop loop;
            connect(this, SIGNAL(ReceivedMessage(QString,QVariantMap,bool,int)), &loop, SLOT(quit()));
            connect(this, SIGNAL(Exit()), &loop, SLOT(quit()));
            loop.exec();
            if(IsNoConnection)
                break;
            if(GetLastMessageType() == MessageType)
                break;
        }
        return GetLastMessageData();
    }

    bool WebInterface::IsConnected()
    {
        return !IsNoConnection;
    }

    QIcon WebInterface::GetIcon()
    {
        return Icon;
    }

    bool WebInterface::GetIsCustomIcon()
    {
        return IsCustomIcon;
    }


    QString WebInterface::GetRandomString()
    {
       const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
       const int randomStringLength = 12;

       QString randomString;
       for(int i=0; i<randomStringLength; ++i)
       {
           int index = qrand() % possibleCharacters.length();
           QChar nextChar = possibleCharacters.at(index);
           randomString.append(nextChar);
       }
       return randomString;
    }

    void WebInterface::Close()
    {
        Html.clear();
        HtmlMainInterface.clear();
        IsDebug = false;
        WindowX = 10000;
        WindowY = 10000;
        WindowWidth = 10000;
        WindowHeight = 10000;
        LastMessageType.clear();
        LastMessageData.clear();
        IsNoConnection = true;
        Buffer.clear();
        if(LoginInterfaceFile)
        {
            LoginInterfaceFile->close();
            LoginInterfaceFile->remove();
            LoginInterfaceFile->deleteLater();
            LoginInterfaceFile = 0;
        }
        if(WebSocketServer)
        {
            WebSocketServer->close();
            WebSocketServer->deleteLater();
            WebSocketServer = 0;
        }
        if(Socket)
        {
            Socket->close();
            Socket->deleteLater();
            Socket = 0;
        }
    }


    bool WebInterface::HasWebInterface()
    {
        return HasInterface;
    }
    void WebInterface::SetFileName(const QString& FileName)
    {
        this->FileName = FileName;
    }

    void WebInterface::SetTesingMode()
    {
        TestingMode = true;
    }
    void WebInterface::SetLastChoice(bool WebInterfaceIsRememberChoice,bool WebInterfaceRememberChoiceUseCustom,const QString& PremiumUsername, const QString& PremiumPassword)
    {
        this->WebInterfaceIsRememberChoice = WebInterfaceIsRememberChoice;
        this->WebInterfaceRememberChoiceUseCustom = WebInterfaceRememberChoiceUseCustom;
        this->PremiumUsername = PremiumUsername;
        this->PremiumPassword = PremiumPassword;
    }

    QString WebInterface::GetPremiumUsername()
    {
        return PremiumUsername;
    }
    QString WebInterface::GetPremiumPassword()
    {
        return PremiumPassword;
    }

    QString WebInterface::GetHtmlInterface()
    {
        return HtmlMainInterface.replace(QRegularExpression("\\?v\\=\\d+"),QString("?v=") + GetRandomString());
    }

    bool WebInterface::GetWebInterfaceIsRememberChoice()
    {
        return WebInterfaceIsRememberChoice;
    }

    bool WebInterface::GetWebInterfaceRememberChoiceUseCustom()
    {
        return WebInterfaceRememberChoiceUseCustom;
    }

    void WebInterface::SetScriptName(const QString& ScriptName)
    {
        this->ScriptName = ScriptName;
        this->Server = "https://bablosoft.com";

    }

    void WebInterface::SetHttpClient(IHttpClient* HttpClient)
    {
        this->HttpClient = HttpClient;
    }


    IHttpClient* WebInterface::GetHttpClient()
    {
        return HttpClient;
    }

    bool WebInterface::InitAsRemoteControl(int Port)
    {
        Close();
        IsRemoteControl = true;
        WebSocketPort = Port;

        WebSocketServer = new QWebSocketServer("InterfaceServer", QWebSocketServer::NonSecureMode, this);

        if(!WebSocketServer->listen(QHostAddress::LocalHost, WebSocketPort))
        {
            Close();
            return false;
        }

        connect(WebSocketServer, SIGNAL(newConnection()), this, SLOT(NewConnection()));

        return true;
    }

    bool WebInterface::GetFailedToGetLoginInterface()
    {
        return FailedToGetLoginInterface;
    }


    bool WebInterface::Init(bool IsSilent)
    {
        this->IsSilent = IsSilent;
        Close();
        HasInterface = false;
        FailedToGetLoginInterface = false;

        if(WebInterfaceIsRememberChoice)
        {
            if(!WebInterfaceRememberChoiceUseCustom)
            {
                return false;
            }
        }

        bool FoundScript = false;

        if(TestingMode)
        {
            if(!ScriptName.isEmpty() && ScriptName != "Script Name" && ScriptName != "ScriptName")
            {
                FoundScript = true;
            }
        }else
        {
            QFile file(FileName);
            QXmlStreamReader xmlReader;
            if (!file.open(QIODevice::ReadOnly| QIODevice::Text))
            {
                return false;
            }

            xmlReader.setDevice(&file);
            ScriptName.clear();

            while(!xmlReader.atEnd() && !xmlReader.hasError())
            {
                QXmlStreamReader::TokenType token = xmlReader.readNext();
                if(xmlReader.name() == "Remote" && token == QXmlStreamReader::StartElement)
                {
                    foreach(QXmlStreamAttribute attr, xmlReader.attributes())
                    {
                        if(attr.name() == "ScriptName")
                        {
                            ScriptName = attr.value().toString();
                            FoundScript = true;
                        }else if(attr.name() == "Server")
                        {
                            Server = attr.value().toString();
                        }
                    }
                }
            }
            file.close();
        }

        if(!FoundScript)
        {
            return false;
        }

        HttpClient->Connect(this,SLOT(LoginInterfaceLoadFinished()));
        HttpClient->Get(Server.replace("http://","https://") + "/apps/" + ScriptName + "/logininterface");

        return true;
    }

    void WebInterface::LoginInterfaceLoadFinished()
    {
        if(HttpClient->WasError())
        {
            if(TestingMode)
            {
                WebInterfaceIsRememberChoice = true;
                WebInterfaceRememberChoiceUseCustom = false;
            }
            FailedToGetLoginInterface = true;
            emit Initialized();
            return;
        }

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(HttpClient->GetContent().toUtf8(), &err);
        if(err.error != QJsonParseError::NoError)
        {
            if(TestingMode)
            {
                WebInterfaceIsRememberChoice = true;
                WebInterfaceRememberChoiceUseCustom = false;
            }
            FailedToGetLoginInterface = true;
            emit Initialized();
            return;
        }


        QVariantMap map = doc.object().toVariantMap();

        if(!map["success"].toBool())
        {
            emit Initialized();
            return;
        }

        if(map["is_debug"].toBool())
        {
            IsDebug = true;
        }

        if(map.contains("x"))
        {
            WindowX = map["x"].toInt();
        }

        if(map.contains("y"))
        {
            WindowY = map["y"].toInt();
        }

        if(map.contains("width"))
        {
            WindowWidth = map["width"].toInt();
        }

        if(map.contains("height"))
        {
            WindowHeight = map["height"].toInt();
        }

        //Set application icon if needed
        {
            IsCustomIcon = false;
            QString ScriptIcon;
            if(map.contains("icon"))
            {
                ScriptIcon = map["icon"].toString();
            }
            if(!ScriptIcon.isEmpty())
            {
                IconImage.loadFromData(QByteArray::fromBase64(ScriptIcon.toUtf8()));
                if(IconImage.width() > 10)
                {
                     IsCustomIcon = true;
                     Icon = QIcon(QPixmap::fromImage(IconImage));
                }
            }
            if(!IsCustomIcon)
            {
                Icon = QIcon(":/fastexecutescript/images/Logo.png");
                IconImage = QImage(":/fastexecutescript/images/Logo.png");
            }
        }

        if(!map["uses_html_interface"].toBool())
        {
            emit Initialized();
            return;
        }

        ScriptId = map["script_id"].toInt();
        WebSocketPort = qrand() % 10000 + 10000;
        StartType = map["interface_start_type"].toInt();


        if(TestingMode)
        {
            Html = QString("<html>"
                           "<head>"
                               "<meta charset=\"UTF-8\">"
                               "<meta name=\"http-equiv\" content=\"Content-type: text/html; charset=UTF-8\"/>"
                               "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                               "<script src=\"https://bablosoft.com/buildinterface/jquery/jquery.min.js\" type=\"text/javascript\" charset=\"utf-8\"></script>"
                               "<script src=\"https://bablosoft.com/buildinterface/bas-api/bas-api.js?v=") + GetRandomString() + QString("\" type=\"text/javascript\" charset=\"utf-8\"></script>"
                               "<script type=\"text/javascript\">"
                                    "$(document).ready(function(){Api.Init(%1)})"
                               "</script>"
                           "</head>"
                           "<body style=\"display:none\">"
                           "</body>"
                            "</html>").arg(QString::number(WebSocketPort));

            if(WebInterfaceIsRememberChoice)
            {
                if(!WebInterfaceRememberChoiceUseCustom)
                {
                    emit Initialized();
                    return;
                }
            }else
            {
                AskIfRunWithWebInterfaceDialog Dialog;
                Dialog.SetScriptName(ScriptName);
                Dialog.exec();
                WebInterfaceIsRememberChoice = Dialog.GetRememberChoice();
                if(WebInterfaceIsRememberChoice)
                    WebInterfaceRememberChoiceUseCustom = Dialog.GetUsesWebInterface();
                if(!Dialog.GetUsesWebInterface())
                {
                    emit Initialized();
                    return;
                }
            }
            if(!PremiumUsername.isEmpty() && !PremiumPassword.isEmpty())
            {
                AutoLogin();
            }else
            {
                LoginWithDialog();
            }
        }else
        {
            Html = map["login_interface_html"].toString().replace("Api.Init(0)",QString("Api.Init(%1)").arg(QString::number(WebSocketPort)));
            StartServer();
        }



    }

    void WebInterface::AutoLogin()
    {
        HttpClient->Connect(this,SLOT(AutoLoginResult()));
        QHash<QString,QString> p;
        p["email"] = PremiumUsername;
        p["pass"] = PremiumPassword;

        HttpClient->Post(Server.replace("http://","https://") + "/bas/preparemianinterface/" + QString::number(ScriptId),p);

    }

    void WebInterface::AutoLoginResult()
    {
        if(HttpClient->WasError())
        {
            emit Initialized();
            return;
        }

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(HttpClient->GetContent().toUtf8(), &err);
        if(err.error != QJsonParseError::NoError)
        {
            emit Initialized();
            return;
        }


        QVariantMap map = doc.object().toVariantMap();

        if(!map["success"].toBool())
        {
            LoginWithDialog();
        }else
        {
            HtmlMainInterface = map["data"].toString();
            StartServer();
        }
    }

    void WebInterface::LoginWithDialog()
    {
        PremiumLoginDialog Dialog;
        Dialog.SetPremiumLogin(PremiumUsername);
        Dialog.SetPremiumPassword(PremiumPassword);
        Dialog.exec();
        PremiumUsername = Dialog.GetPremiumLogin();
        PremiumPassword = Dialog.GetPremiumPassword();
        if(!Dialog.GetUsesWebInterface())
        {
            emit Initialized();
            return;
        }else
        {
            HttpClient->Connect(this,SLOT(LoginWithDialogResult()));
            QHash<QString,QString> p;
            p["email"] = PremiumUsername;
            p["pass"] = PremiumPassword;

            HttpClient->Post(Server.replace("http://","https://") + "/bas/preparemianinterface/" + QString::number(ScriptId),p);
        }
    }

    void WebInterface::LoginWithDialogResult()
    {
        if(HttpClient->WasError())
        {
            QMessageBox::information(0, "Failed to obtain interface", "Http error");

            emit Initialized();
            return;
        }

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(HttpClient->GetContent().toUtf8(), &err);
        if(err.error != QJsonParseError::NoError)
        {
            QMessageBox::information(0, "Failed to obtain interface", "Failed to parse json");

            emit Initialized();
            return;
        }

        QVariantMap map = doc.object().toVariantMap();

        if(!map["success"].toBool())
        {
            QMessageBox::information(0, "Failed to obtain interface", map["message"].toString());
            emit Initialized();
            return;
        }else
        {
            HtmlMainInterface = map["data"].toString();
            StartServer();
        }
    }

    QString WebInterface::GetWorkerFolderName()
    {
        QString WorkerFolderName = QString("Worker");
        BrowserVersionSelector _BrowserVersionSelector;
        IBrowserVersionSelector::BrowserItem _BrowserItem = _BrowserVersionSelector.GetDefaultBrowserItem();
        if(!_BrowserItem.IsNull)
        {
            WorkerFolderName = _BrowserItem.WorkerFolderName;
        }
        return WorkerFolderName;
    }


    void WebInterface::StartServer()
    {
        QString WorkerFolderName = GetWorkerFolderName();

        QString LoginHtml = Html;

        {
            QDir dir("webinterface");
            dir.mkpath(".");
            dir.setNameFilters(QStringList() << "*.*");
            dir.setFilter(QDir::Files);
            foreach(QString dirFile, dir.entryList())
            {
                dir.remove(dirFile);
            }
        }

        QString BaseFileName = QString("webinterface/") + GetRandomString() + ".html";
        LoginInterfaceFile = new QFile(QFileInfo(BaseFileName).absoluteFilePath());
        QFileInfo(QString("webinterface/") + GetRandomString() + ".html").absoluteFilePath();

        if(!LoginInterfaceFile->open(QIODevice::WriteOnly))
        {
            emit Initialized();
            return;
        }
        LoginInterfaceFile->write(LoginHtml.toUtf8());
        LoginInterfaceFile->flush();


        WebSocketServer = new QWebSocketServer("InterfaceServer", QWebSocketServer::NonSecureMode, this);

        if(!WebSocketServer->listen(QHostAddress::LocalHost, WebSocketPort))
        {
            Close();
            emit Initialized();
            return;

        }

        connect(WebSocketServer, SIGNAL(newConnection()), this, SLOT(NewConnection()));
        if(StartType == 1)
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(LoginInterfaceFile->fileName()).absoluteFilePath()));
        }else if(StartType == 0)
        {
            if(IconImage.width()>=32)
            {
                IconImage.scaled(32,32,Qt::IgnoreAspectRatio,Qt::SmoothTransformation).save(WorkerFolderName + QString("/iconbig.ico"),"ICO",100);
                IconImage.scaled(16,16,Qt::IgnoreAspectRatio,Qt::SmoothTransformation).save(WorkerFolderName + QString("/iconsmall.ico"),"ICO",100);

            }else
            {
                IconImage.scaled(16,16,Qt::IgnoreAspectRatio,Qt::SmoothTransformation).save(WorkerFolderName + QString("/iconbig.ico"),"ICO",100);
                IconImage.scaled(16,16,Qt::IgnoreAspectRatio,Qt::SmoothTransformation).save(WorkerFolderName + QString("/iconsmall.ico"),"ICO",100);

            }

            QStringList ParamList = QStringList()<<QString("--title=%1").arg(ScriptName)
                         <<QString("--url=%1").arg(QFileInfo(LoginInterfaceFile->fileName()).absoluteFilePath())
                         <<QString("--iconbig=%1").arg("iconbig.ico")
                         <<QString("--iconsmall=%1").arg("iconsmall.ico");

            if(WindowWidth < 10000 && WindowHeight < 10000)
            {
                ParamList.append(QString("--width=") + QString::number(WindowWidth));
                ParamList.append(QString("--height=") + QString::number(WindowHeight));
            }

            if(WindowX < 10000 && WindowY < 10000)
            {
                ParamList.append(QString("--x=") + QString::number(WindowX));
                ParamList.append(QString("--y=") + QString::number(WindowY));
            }



            if(IsDebug && !IsSilent)
            {
                ParamList.append("--debug");
            }

            if(IsSilent)
            {
                ParamList.append("--silent");
            }

            QProcess::startDetached(
                        WorkerFolderName + QString("/UserInterface.exe"),ParamList,WorkerFolderName
                 );
        }
    }

    void WebInterface::NewConnection()
    {
        if(!IsNoConnection)
            return;

        if(!WebSocketServer)
            return;

        IsNoConnection = false;
        Socket = WebSocketServer->nextPendingConnection();
        connect(Socket,SIGNAL(disconnected()),this,SLOT(Disconnected()));
        connect(Socket,SIGNAL(textMessageReceived(QString)),this,SLOT(Received(QString)));

        HasInterface = true;
        emit Initialized();
    }

    void WebInterface::Received(QString Message)
    {
        Buffer += Message;
        QStringList Split = Buffer.split("---Message--End---");
        QList<Task> Tasks;
        for(int i = 0;i<Split.length() - 1;i++)
        {
            QString DataString = Split[i];
            QJsonParseError err;
            QJsonDocument Document = QJsonDocument::fromJson(DataString.toUtf8(), &err);
            QJsonObject Object = Document.object();
            LastMessageType = Object["type"].toString();
            LastMessageData = Object["data"].toObject().toVariantMap();
            bool IsAsync = Object.contains("async") && Object["async"].toBool();
            int Id = Object["id"].toInt();
            Task t;
            t.Type = LastMessageType;
            t.Data = LastMessageData;
            t.Id = Id;
            t.IsAsync = IsAsync;
            Tasks.append(t);

        }
        Buffer = Split.last();
        for(Task & t:Tasks)
        {
            emit ReceivedMessage(t.Type, t.Data, t.IsAsync, t.Id);
        }

    }

    void WebInterface::Disconnected()
    {
        if(IsNoConnection)
            return;

        IsNoConnection = true;
        emit Exit();
        if(!TestingMode)
        {
            qApp->closeAllWindows();
            qApp->quit();
        }
    }

    void WebInterface::SendRaw(const QString& MessageType, bool IsAsync, int Id, const QVariant& Params)
    {
        if(IsNoConnection)
            return;

        if(!Socket)
            return;

        QJsonObject Obj;
        Obj["type"] = MessageType;
        Obj["data"] = QJsonValue::fromVariant(Params);
        Obj["id"] = Id;
        Obj["async"] = IsAsync;

        QJsonDocument doc(Obj);
        QString Json = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));

        Json += "---Message--End---";
        Socket->sendTextMessage(Json);
    }

    void WebInterface::OnNewMessage(const QString& MessageType, const QJsonObject& Params)
    {
        if(MessageType == "browser_add" && Params.contains("browser_id"))
        {
            QString BrowserId = Params.value("browser_id").toString();
            BrowserItem Item;
            Item.Id = BrowserId;
            BrowserItems[BrowserId] = Item;
            return;
        }

        if(MessageType == "browser_remove" && Params.contains("browser_id"))
        {
            QString BrowserId = Params.value("browser_id").toString();
            if(!BrowserItems.contains(BrowserId))
                return;
            BrowserItem _Item = BrowserItems[BrowserId];
            BrowserItems.remove(BrowserId);
            if(_Item.ManualControl)
            {
                Send("manual_control_end",false,0,"browser_id",BrowserId);
            }
            if(_Item.Visible)
            {
                Send("hide_browser",false,0,"browser_id",BrowserId);
            }
            return;
        }

        if(MessageType == "show_browser" && Params.contains("browser_id"))
        {
            QString BrowserId = Params.value("browser_id").toString();
            if(!BrowserItems.contains(BrowserId))
                return;
            BrowserItem &_Item = BrowserItems[BrowserId];
            _Item.Visible = true;
            return;
        }

        if(MessageType == "hide_browser" && Params.contains("browser_id"))
        {
            QString BrowserId = Params.value("browser_id").toString();
            if(!BrowserItems.contains(BrowserId))
                return;
            BrowserItem &_Item = BrowserItems[BrowserId];
            _Item.Visible = false;
            return;
        }

        if(MessageType == "manual_control_start" && Params.contains("browser_id"))
        {
            QString BrowserId = Params.value("browser_id").toString();
            if(!BrowserItems.contains(BrowserId))
                return;
            BrowserItem &_Item = BrowserItems[BrowserId];
            _Item.ManualControl = true;
            return;
        }

        if(MessageType == "manual_control_end" && Params.contains("browser_id"))
        {
            QString BrowserId = Params.value("browser_id").toString();
            if(!BrowserItems.contains(BrowserId))
                return;
            BrowserItem &_Item = BrowserItems[BrowserId];
            _Item.ManualControl = false;
            return;
        }

    }

    void WebInterface::Send(const QString& MessageType,bool IsAsync,int Id,const QString& Param1Name, const QVariant& Param1Value,const QString& Param2Name, const QVariant& Param2Value,const QString& Param3Name, const QVariant& Param3Value,const QString& Param4Name, const QVariant& Param4Value)
    {
        if(IsNoConnection)
            return;

        if(!Socket)
            return;

        QJsonObject Params;

        if(!Param1Name.isEmpty())
        {
            Params[Param1Name] = QJsonValue::fromVariant(Param1Value);
        }

        if(!Param2Name.isEmpty())
        {
            Params[Param2Name] = QJsonValue::fromVariant(Param2Value);
        }

        if(!Param3Name.isEmpty())
        {
            Params[Param3Name] = QJsonValue::fromVariant(Param3Value);
        }

        if(!Param4Name.isEmpty())
        {
            Params[Param4Name] = QJsonValue::fromVariant(Param4Value);
        }

        OnNewMessage(MessageType, Params);

        QJsonObject Obj;
        Obj["type"] = MessageType;
        Obj["data"] = Params;
        Obj["id"] = Id;
        Obj["async"] = IsAsync;

        QJsonDocument doc(Obj);
        QString Json = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));

        Json += "---Message--End---";
        Socket->sendTextMessage(Json);
    }

    bool WebInterface::GetIsRemoteControl()
    {
        return IsRemoteControl;
    }

    void WebInterface::SetRemoteControlData(const QString& Script, const QString& Login, const QString& Password)
    {
        this->RemoteControlScript = Script;
        this->RemoteControlLogin = Login;
        this->RemoteControlPassword = Password;
    }

    QString WebInterface::GetRemoteControlScript()
    {
        return RemoteControlScript;
    }

    QString WebInterface::GetRemoteControlLogin()
    {
        return RemoteControlLogin;
    }

    QString WebInterface::GetRemoteControlPassword()
    {
        return RemoteControlPassword;
    }


}
