#include "webdriveprocesscomunicator.h"
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QImage>
#include <QBuffer>
#include <QStringList>
#include <QApplication>
#include <QRegularExpression>
#include <QDir>
#include <QSettings>
#include "webdriverprofileremover.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    WebDriveProcessComunicator::WebDriveProcessComunicator(QObject *parent) :
        IProcessComunicator(parent)
    {
        IsRecord = false;
        Logger = 0;
        Id = qrand() % 1000000 + 1;
        HttpClientFactory = 0;
        _HasProcess = false;
        _IsFail = false;
        _IsDie = false;
        IsClosed = false;

        QSettings Settings("settings_multilogin.ini",QSettings::IniFormat);
        ApiLocation = Settings.value("ApiLocation","https://api.multiloginapp.com/").toString();

        Timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
        SearchFunctions = "%BAS_SCRIPT%";
        Viewer = 0;
        Log("Created");
    }

    /* Helpers */
    void WebDriveProcessComunicator::Log(const QString& Text,const QString& Arg1,const QString& Arg2,const QString& Arg3)
    {
        QString Res;

        bool c1 = Text.contains("%1");
        bool c2 = Text.contains("%2");
        bool c3 = Text.contains("%3");
        if(c1 && c2 && c3)
        {
            Res = Text.arg(Arg1).arg(Arg2).arg(Arg3);
        }else if(c1 && c2)
        {
            Res = Text.arg(Arg1).arg(Arg2);
        }else if(c1)
        {
            Res = Text.arg(Arg1);
        }else
        {
            Res = Text;
        }

        if(!MLAKey.isEmpty())
            Res = Res.replace(MLAKey,MLAKey.mid(0,5) + QString("..."));

        if(Logger)
        {
            bool IsMultilogin = QRegExp("^\\s*(\\<\\-|\\-\\>)").indexIn(Res) == 0;
            bool IsBAS = QRegExp("^\\s*(Send|Received)\\s*").indexIn(Res) == 0;
            LogLevel Level;
            if(IsMultilogin)
                Level = LogUser;
            else if(IsBAS)
                Level = LogSuccess;
            else
                Level = LogInfo;


            QString ResHtml = Res;
            ResHtml = QString("<span style='color:gray'>ProcessCommunicator [") + QString::number(Id) + QString("] </span> ") + ResHtml.replace("<","&lt;").
                    replace(">","&gt;").
                    replace(QRegExp("^\\s*(\\-\\&gt\\;|\\&lt\\;\\-)\\s*\\[([^\\]]*)\\]\\s*\\[([^\\]]*)\\]"),"\\1 [\\2] [<span style='text-decoration: underline;'>\\3</span>] ").
                    replace(QRegExp("^\\s*(\\-\\&gt\\;|\\&lt\\;\\-)\\s*\\[([^\\]]*)\\]"),"\\1 [<span style='text-decoration: underline;'>\\2</span>] ").
                    replace(QRegExp("^\\s*Send\\s*"),"<span style='color:#f975e3;'>bas sent</span> ").
                    replace(QRegExp("^\\s*Received\\s*"),"<span style='color:#f9f474'>bas received</span> ").
                    replace(QRegExp("^\\s*\\&lt\\;\\-"),"<span style='color:#41f48f;'>mla received</span>").
                    replace(QRegExp("^\\s*\\-\\&gt\\;"),"<span style='color:#4292f4;'>mla sent</span>");

            QString ResText = Res;
            ResText = QString("ProcessCommunicator [") + QString::number(Id) + QString("] ")  + ResText.
                  replace(QRegExp("^\\s*(\\-\\>|<\\-)\\s*\\[([^\\]]*)\\]\\s*\\[([^\\]]*)\\]"),"\\1 [\\2] [\\3] ").
                  replace(QRegExp("^\\s*Send\\s*"),"bas sent ").
                  replace(QRegExp("^\\s*Received\\s*"),"bas received ").
                  replace(QRegExp("^\\s*\\<\\-"),"mla received").
                  replace(QRegExp("^\\s*\\-\\>"),"mla sent");

            Logger->WriteHtml(ResHtml,ResText,Level);
        }
    }

    void WebDriveProcessComunicator::SetLogger(ILogger * Logger)
    {
        this->Logger = Logger;
    }

    void WebDriveProcessComunicator::SetIsRecord()
    {
        this->IsRecord = true;
    }

    void WebDriveProcessComunicator::SetRecordId(const QString& RecordId)
    {
        this->RecordId = RecordId;
    }

    IHttpClient * WebDriveProcessComunicator::LogHttpClientAndDelete(QObject *Client)
    {
        Client->deleteLater();
        IHttpClient * HttpClient = qobject_cast<IHttpClient*>(Client);
        HttpClient->Disconnect();

        Log("<- [%1] %2", HttpClient->GetLastUrl(), HttpClient->GetContent());
        return HttpClient;
    }

    QString WebDriveProcessComunicator::LogHttpClientDeleteAndGetValue(QObject *Client)
    {
        Client->deleteLater();
        IHttpClient * HttpClient = qobject_cast<IHttpClient*>(Client);
        HttpClient->Disconnect();

        Log("<- [%1] %2", HttpClient->GetLastUrl(), HttpClient->GetContent());

        QJsonDocument JsonResponse = QJsonDocument::fromJson(HttpClient->GetPageData());
        QJsonObject JsonObj = JsonResponse.object();
        QString value = JsonObj["value"].toString();

        return value;
    }

    void WebDriveProcessComunicator::Api(const QString& Method, const QString& Data, const QString& Url, const QString& Responce)
    {
        Api(Method,Data,Url,Responce,this,SLOT(SendNextResponceAndDeleteSender()));
    }

    void WebDriveProcessComunicator::Api(const QString& Method, const QString& Data, const QString& Url, const QString& Responce, QObject *object, const char * slot)
    {
        if(SessionId.isEmpty())
            return;

        if(!_HasProcess)
            return;

        if(Responce != "NOCHANGE")
            NextResponce = Responce;


        IHttpClient * HttpClient = HttpClientFactory->GetHttpClient(false);
        HttpClient->AddHeader("Expect","");
        HttpClient->setParent(this);
        HttpClient->Connect(object,slot);
        QHash<QString,QString> p;
        p["data"] = Data;

        PostOptions Options;
        Options.Method = Method;
        Options.PrepareStrategy = "custom";
        HttpClient->AddHeader("Content-Type","application/json");

        QString url;
        url = QString("http://127.0.0.1:%1/session/%2%3").arg(QString::number(Port)).arg(SessionId).arg(Url);

        Log("-> [%1][%2] %3", Method, url, p["data"]);

        HttpClient->Post(url, p, Options);


    }

    void WebDriveProcessComunicator::ScrollToCoordinates(int X, int Y, QObject *object, const char * slot)
    {
        QStringList args;
        args.append(QString::number(X));
        args.append(QString::number(Y));
        QString Json = GenerateJson("script",
        "(function(arg){%BAS_SCRIPT%BrowserAutomationStudio_ScrollToCoordinates(parseInt(arg[0]),parseInt(arg[1]),arg[2]);})(arguments)","args",args);
        Api("POST",Json,"/execute_async","NOCHANGE",object,slot);
    }

    void WebDriveProcessComunicator::FindElementAndExecuteScript(const QString& Selector, const QString& Script, QObject *object, const char * slot, bool DoScroll)
    {
        ExpandFramesX = 0;
        ExpandFramesY = 0;
        FindElementAndExecuteScriptInternal(Selector, Script, object, slot, true, DoScroll);
    }

    void WebDriveProcessComunicator::FindElementAndExecuteScriptInternal(const QString& Selector, const QString& Script, QObject *object, const char * slot, bool IsFirst , bool DoScroll)
    {
        IsScrollFrame = DoScroll;
        if(IsFirst && IsInsideFrame)
        {
            MoveToTopFrame(Selector, Script, object, slot);
            return;
        }

        if(ParseJsonArray(Selector).contains("frame_element"))
        {
            ExpandFrames(Selector, Script, object, slot);
            return;
        }
        QStringList args;
        args.append(Selector);

        QString ScrollXScript;
        QString ScrollYScript;

        if(IsInsideFrame)
        {
            ScrollXScript = QString::number(ScrollX);
            ScrollYScript = QString::number(ScrollY);
        }else
        {
            ScrollXScript = "document.body.scrollLeft";
            ScrollYScript = "document.body.scrollTop";
        }


        QString CurrentScript = QString("(function (callback,selector)"
                                 "{") + SearchFunctions +QString(";let self = BrowserAutomationStudio_FindElement(selector);") +
                                     ((DoScroll)? "BrowserAutomationStudio_ScrollToElement(self, function(coordinates){" : "") +
                                         QString(";let positionx=") + QString::number(ExpandFramesX) +
                                         QString(";let positiony=") + QString::number(ExpandFramesY) +
                                         QString(";let scrollx=") + ScrollXScript +
                                         QString(";let scrolly=") + ScrollYScript + QString(";") +

                                         Script +
                                     ((DoScroll)? "})" : "") +
                                 QString("})(arguments[1],arguments[0])");

        QString Json = GenerateJson("script",CurrentScript ,"args",args);

        Api("POST",Json,"/execute_async","NOCHANGE",object,slot);
    }

    void WebDriveProcessComunicator::FindElementScrollAndExecuteScriptAdjustCoordinates(const QString& Selector, const QString& Script, QObject *object, const char * slot)
    {
        if(!ParseJsonArray(Selector).contains("frame_element"))
        {
            FindElementAndExecuteScript(Selector, Script, object, slot, true);
            return;
        }
        AdjustSelector = Selector;
        ObjectToExecuteAfterAjust = object;
        SlotToExecuteAfterAjust = QString(slot).toUtf8();
        ScriptToExecuteAfterAdjust = Script;
        FindElementAndExecuteScript(Selector, QString("callback({value:'',coordinates:coordinates});"), this, SLOT(AfterAdjust()),true);
    }

    void WebDriveProcessComunicator::AfterAdjust()
    {
        LogHttpClientAndDelete(sender());
        QString Json = GenerateJson("id", QVariant());

        IsInsideFrame = false;
        ExpandFramesX = 0;
        ExpandFramesY = 0;

        Api("POST",Json,"/frame","NOCHANGE",this,SLOT(AfterAdjustAndResetFrame()));

    }

    void WebDriveProcessComunicator::AfterAdjustAndResetFrame()
    {
        LogHttpClientAndDelete(sender());

        QString Json = GenerateJson("script","return document.body.scrollLeft + '|' + document.body.scrollTop","args",QStringList());
        Api("POST",Json,"/execute","NOCHANGE",this,SLOT(AfterGetMainWindowScroll()));
    }

    void WebDriveProcessComunicator::AfterGetMainWindowScroll()
    {
        QString value = LogHttpClientDeleteAndGetValue(sender());


        QStringList List = value.split("|");
        ScrollX = List[0].toInt();
        ScrollY = List[1].toInt();


        FindElementAndExecuteScript(AdjustSelector, ScriptToExecuteAfterAdjust, ObjectToExecuteAfterAjust, SlotToExecuteAfterAjust.constData(),true);

    }

    void WebDriveProcessComunicator::ExpandFrames(const QString& Selector, const QString& Script, QObject *object, const char * slot)
    {
        QStringList Path = ParseJsonArray(Selector);

        int index = Path.indexOf("frame_element");

        QString SelectorBeforeFrame;
        QString SelectorAfterFrame;
        SelectorBeforeFrame = GenerateJsonArray(Path.mid(0,index));
        SelectorAfterFrame = GenerateJsonArray(Path.mid(index + 2));
        SetFrameChangeArguments(SelectorAfterFrame, Script, object, slot);

        QString Json = GenerateJson("find_frame_function", QString("function(selector){") + SearchFunctions + QString(";return BrowserAutomationStudio_FindElement(selector);}"),
                                    "find_frame_arguments", QStringList()<<SelectorBeforeFrame,
                                    "frame_properties_function", QString("function(selector){") + SearchFunctions + QString(";let rect = BrowserAutomationStudio_GetInternalBoundingRect(BrowserAutomationStudio_FindElement(selector)); return rect.left + '|' + rect.top}"),
                                    "frame_properties_arguments", QStringList()<<SelectorBeforeFrame);
        IsInsideFrame = true;

        Api("POST",Json,"/frame","NOCHANGE",this,SLOT(ChangeFrameResult()));


    }
    void WebDriveProcessComunicator::MoveToTopFrame(const QString& Selector, const QString& Script, QObject *object, const char * slot)
    {
        SetFrameChangeArguments(Selector, Script, object, slot);
        QString Json = GenerateJson("id", QVariant());

        IsInsideFrame = false;
        ExpandFramesX = 0;
        ExpandFramesY = 0;

        Api("POST",Json,"/frame","NOCHANGE",this,SLOT(ChangeFrameResult()));

    }
    void WebDriveProcessComunicator::SetFrameChangeArguments(const QString& Selector, const QString& Script, QObject *object, const char * slot)
    {
        ExpandFramesSelector = Selector;
        ExpandFramesScript = Script;
        ExpandFramesObject = object;
        ExpandFramesSlot = QString(slot).toUtf8();
    }
    void WebDriveProcessComunicator::ChangeFrameResult()
    {
        IHttpClient * HttpClient = LogHttpClientAndDelete(sender());

        QJsonDocument JsonResponse = QJsonDocument::fromJson(HttpClient->GetPageData());
        QJsonObject JsonObj = JsonResponse.object();

        if(JsonObj["status"].toInt())
        {
            FailedToSearchForFrame();
            return;
        }


        QStringList split = JsonObj["value"].toString().split("|");

        ExpandFramesX += split.first().toInt();
        ExpandFramesY += split.last().toInt();
        FindElementAndExecuteScriptInternal(ExpandFramesSelector, ExpandFramesScript, ExpandFramesObject, ExpandFramesSlot.constData(), false, IsScrollFrame);
    }

    void WebDriveProcessComunicator::FailedToSearchForFrame()
    {
        if(ElementCommand == "script")
        {
            QString NextResponceCopy = NextResponce;
            NextResponceCopy.replace("%VALUE%","");
            SendReceived(NextResponceCopy);
            return;
        }
        if(ElementCommand == "exist")
        {
            QString NextResponceCopy = NextResponce;
            NextResponceCopy.replace("%VALUE%","0");
            SendReceived(NextResponceCopy);
            return;
        }
        QTimer::singleShot(1000,this,SLOT(ExecuteElementAction()));
    }

    void WebDriveProcessComunicator::SendNextResponce()
    {
        SendReceived(NextResponce);
    }

    void WebDriveProcessComunicator::SearchProfilesResult()
    {
        IHttpClient * HttpClient = LogHttpClientAndDelete(sender());
        if(NextResponce.contains("%VALUE%"))
        {
            QJsonDocument JsonResponse = QJsonDocument::fromJson(HttpClient->GetPageData());
            QJsonObject JsonObj = JsonResponse.object();

            QVariantMap Map;
            QVariantList List;


            if(JsonObj.contains("data") && JsonObj["data"].isArray())
            {
                for(QJsonValueRef Val: JsonObj["data"].toArray())
                {
                    if(Val.isObject() && Val.toObject().contains("sid") && Val.toObject().contains("name") && Val.toObject()["sid"].isString() && Val.toObject()["name"].isString())
                    {
                        QString id = Val.toObject()["sid"].toString();
                        QString name = Val.toObject()["name"].toString();
                        QRegExp MaskRegexp;
                        MaskRegexp.setPatternSyntax(QRegExp::Wildcard);
                        MaskRegexp.setPattern(OnlineProfileSearchPattern);
                        if(MaskRegexp.exactMatch(name))
                        {
                            List.append(id + QString(":") + name);
                        }
                    }
                }
            }

            Map["data"] = List;

            QJsonObject object = QJsonObject::fromVariantMap(Map);
            QJsonDocument document;
            document.setObject(object);

            QString Params = QString::fromUtf8(document.toJson());

            QString string;
            QXmlStreamWriter writer(&string);
            writer.writeStartElement("temp");
            writer.writeCharacters(Params);
            writer.writeEndElement();

            NextResponce = NextResponce.replace("%VALUE%", string.replace("<temp>","").replace("</temp>",""));
        }
        SendReceived(NextResponce);
    }

    void WebDriveProcessComunicator::SendNextResponceAndDeleteSender()
    {
        IHttpClient * HttpClient = LogHttpClientAndDelete(sender());

        if(NextResponce.isEmpty())
            return;

        if(NextResponce.contains("%VALUE%"))
        {
            QJsonDocument JsonResponse = QJsonDocument::fromJson(HttpClient->GetPageData());
            QJsonObject JsonObj = JsonResponse.object();

            QString value;

            if(JsonObj["value"].isDouble())
            {
                value = QString::number(JsonObj["value"].toDouble());
            }else if(JsonObj["value"].isString())
                value = JsonObj["value"].toString();
            else if(JsonObj["value"].isObject() && JsonObj["value"].toObject().contains("coordinates"))
            {
                QStringList list = JsonObj["value"].toObject()["coordinates"].toString().split(",");
                ScrollX = list[0].toInt();
                ScrollY = list[1].toInt();

                if(JsonObj["value"].toObject()["value"].isDouble())
                {
                    value = QString::number(JsonObj["value"].toObject()["value"].toDouble());
                }else if(JsonObj["value"].toObject()["value"].isString())
                    value = JsonObj["value"].toObject()["value"].toString();
            }

            if(value == QString("BAS_NOT_EXISTS"))
            {
                QTimer::singleShot(1000,this,SLOT(ExecuteElementAction()));
                return;
            }

            QString string;
            QXmlStreamWriter writer(&string);
            writer.writeStartElement("temp");
            writer.writeCharacters(value);
            writer.writeEndElement();

            NextResponce = NextResponce.replace("%VALUE%", string.replace("<temp>","").replace("</temp>",""));
        }

        SendReceived(NextResponce);
    }

    QString WebDriveProcessComunicator::EscapeStringJavascript(const QString &Text)
    {
        QString res = QString::fromUtf8(QJsonDocument::fromVariant(QVariant(QStringList()<<Text)).toJson(QJsonDocument::Compact));
        res = res.remove(0,1);
        res = res.remove(res.length() - 1,1);
        return res;
    }

    QStringList WebDriveProcessComunicator::ParseJsonArray(const QString &JsonArrayAsString)
    {
        QStringList res;
        QJsonDocument JsonDocument = QJsonDocument::fromJson(JsonArrayAsString.toUtf8());
        QJsonArray JsonArray = JsonDocument.array();
        for(QJsonValueRef ValueString:JsonArray)
            res.append(ValueString.toString());
        return res;
    }

    QString WebDriveProcessComunicator::GenerateJsonArray(const QStringList &JsonArray)
    {
        QJsonArray Res = QJsonArray::fromStringList(JsonArray);
        QJsonDocument doc(Res);
        return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    }

    QString WebDriveProcessComunicator::GenerateJson(const QString& Name1, const QVariant& Value1,const QString& Name2, const QVariant& Value2,const QString& Name3, const QVariant& Value3,const QString& Name4, const QVariant& Value4)
    {
        QJsonObject Obj;

        Obj[Name1] = QJsonValue::fromVariant(Value1);
        if(!Name2.isEmpty())
        {
            Obj[Name2] = QJsonValue::fromVariant(Value2);
        }

        if(!Name3.isEmpty())
        {
            Obj[Name3] = QJsonValue::fromVariant(Value3);
        }

        if(!Name4.isEmpty())
        {
            Obj[Name4] = QJsonValue::fromVariant(Value4);
        }

        QJsonDocument doc(Obj);
        QString Json = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
        return Json;
    }

    void WebDriveProcessComunicator::SendReceived(const QString& Text)
    {
        if(IsInsideFrame)
        {
            SendReceivedResponse = Text;
            IsInsideFrame = false;
            QString Json = GenerateJson("id", QVariant());
            Api("POST",Json,"/frame","NOCHANGE",this,SLOT(AfterRestoreFrame()));
            return;
        }
        QString TextCopy = Text;
        Log("Received %1", TextCopy);
        emit Received(TextCopy);
    }

    void WebDriveProcessComunicator::OnShowBrowser()
    {
        emit ShowBrowser();
    }

    void WebDriveProcessComunicator::OnHideBrowser()
    {
        emit HideBrowser();
    }

    void WebDriveProcessComunicator::AfterRestoreFrame()
    {
        SendReceived(SendReceivedResponse);
        SendReceivedResponse.clear();
    }


    /* Interface implementation */

    void WebDriveProcessComunicator::SetPort(int Port)
    {
        this->Port = Port;
    }


    void WebDriveProcessComunicator::SetHttpClientFactory(IHttpClientFactory *HttpClientFactory)
    {
        this->HttpClientFactory = HttpClientFactory;
        connect(HttpClientFactory,SIGNAL(destroyed(QObject*)),this,SLOT(SetIsClosed()));
    }

    void WebDriveProcessComunicator::LockProfile(int CommunicatorId, const QString& ProfilePath)
    {
        if(Id == CommunicatorId)
        {
            ProfileLock = QSharedPointer<QFile>::create(QDir::cleanPath(ProfilePath + QDir::separator() + "lock.txt"));
            ProfileLock->remove();
            ProfileLock->open(QIODevice::WriteOnly);

        }
    }

    void WebDriveProcessComunicator::SessionCreated(int CommunicatorId, const QString& SessionId, int Port, bool IsDie, const QString& DieString, bool IsFail, const QString& FailString)
    {
        if(Id == CommunicatorId)
        {
            this->_IsDie = false;
            this->_IsFail = false;

            QString LogLocation = tr(" See openfile:///multilogin_log.txt for more details.");
            if(IsDie)
            {
                this->_IsDie = true;
                this->_DieString = DieString + LogLocation;
                emit ProcessStarted();
            }else if(IsFail)
            {
                this->_IsFail = true;
                this->_FailString = FailString + LogLocation;
                emit ProcessStarted();
            }else
            {
                if(BrowserEngine == "MLAMimic")
                {
                    QStringList Split = SessionId.split("|");
                    this->SessionId = Split.first();
                    this->MLAKey = Split.last();
                    if(Split.length() >= 2)
                    {
                        this->ProfileId = Split[1];
                    }
                }else
                {
                    this->SessionId = SessionId;
                }
                Log("Session created %1",SessionId);

                JustStarted = true;
                _HasProcess = true;
                this->Port = Port;

                if(BrowserEngine == "MLAMimic")
                    Api("POST",GenerateJson("use_editor",IsRecord,"editor_id",RecordId),"/start_using_extensions","NOCHANGE",this,SLOT(ResponceStartUsingExtensions()));
                else
                    ResizeGuess();
            }
        }
    }

    void WebDriveProcessComunicator::ResponceStartUsingExtensions()
    {
        ChromePID = LogHttpClientDeleteAndGetValue(sender());
        ResizeGuess();
    }

    void WebDriveProcessComunicator::ResizeGuess()
    {
        QString Json = GenerateJson("width",1024,"height",600);
        QString url;
        if(BrowserEngine == "WebDriverChrome" || BrowserEngine == "MLAMimic")
        {
            url = "/window/current/size";
        }else if(BrowserEngine == "WebDriverFirefox")
        {
            url = "/window/rect";
        }
        Api("POST",Json,url,"",this,SLOT(ResponceResizeGuess()));
    }


    int WebDriveProcessComunicator::GetId()
    {
        return Id;
    }

    bool WebDriveProcessComunicator::GetIsOnlineProfile()
    {
        return ProfileId.startsWith("mla_id:") || ProfileId.startsWith("mla_name:");
    }


    QString WebDriveProcessComunicator::GetProfileId()
    {
       if(ProfileId.startsWith("mla_name:"))
           return QString();
       QString res = ProfileId;
       return res;
    }

    void WebDriveProcessComunicator::SetBrowserEngine(const QString& BrowserEngine)
    {
        this->BrowserEngine = BrowserEngine;
    }

    WebDriveProcessComunicator::~WebDriveProcessComunicator()
    {
        Logger = 0;
        if(Viewer)
        {
            Viewer->deleteLater();
            Viewer = 0;
        }
        Abort();
    }


    void WebDriveProcessComunicator::SetLocation(const QString& Location)
    {
    }

    void WebDriveProcessComunicator::SetRecord(bool IsRecord)
    {

    }

    void WebDriveProcessComunicator::CreateProcess(const QStringList& arguments, bool IsTemporaryProfile)
    {
        //Log(QString("CreateProcess ") + arguments.join(","));
        for(const QString& arg:arguments)
        {
            if(arg.startsWith("Profile="))
            {
                QString ProfilePath = arg;
                ProfilePath = ProfilePath.mid(8);
                ProfileId = ProfilePath;
            }
        }
        emit NewSession(arguments, BrowserEngine, GetId());
    }

    void WebDriveProcessComunicator::ConnectToProcess(const QString& key_in, const QString& key_out)
    {

    }

    void WebDriveProcessComunicator::SetIsClosed()
    {
        IsClosed = true;
    }

    void WebDriveProcessComunicator::Abort()
    {
        if(IsClosed)
            return;
        IsClosed = true;

        if(BrowserEngine.startsWith("MLA"))
        {
            WebDriverProfileRemover * Remover = new WebDriverProfileRemover();

            IHttpClient * Client = HttpClientFactory->GetHttpClient(false);
            Client->AddHeader("Expect","");

            Remover->Remove(Client,SessionId,ProfileId,Port,MLAKey);
        }else
        {
            Api("DELETE","","","");
        }
    }

    void WebDriveProcessComunicator::AbortByUser()
    {
        Abort();
    }

    bool WebDriveProcessComunicator::IsError()
    {
        return false;
    }

    QString WebDriveProcessComunicator::ErrorString()
    {
        return QString();
    }

    bool WebDriveProcessComunicator::HasProcess()
    {
        return _HasProcess;
    }

    bool WebDriveProcessComunicator::IsFail()
    {
        return _IsFail;
    }

    QString WebDriveProcessComunicator::FailString()
    {
        return _FailString;
    }

    bool WebDriveProcessComunicator::IsDie()
    {
        return _IsDie;
    }

    QString WebDriveProcessComunicator::DieString()
    {
        return _DieString;
    }


    qint64 WebDriveProcessComunicator::GetPID()
    {
        return 0;
    }

    void WebDriveProcessComunicator::Suspend()
    {
    }

    void WebDriveProcessComunicator::SetGeneralTimeout(int Timeout)
    {
        this->GeneralTimeout = Timeout;
    }


    void WebDriveProcessComunicator::Send(const QString& value)
    {
        if(SessionId.isEmpty())
            return;

        if(!_HasProcess)
            return;

        Log("Send %1",value);
        QXmlStreamReader xmlReader;
        xmlReader.addData(value);

        while(!xmlReader.atEnd() && !xmlReader.hasError())
        {
            QXmlStreamReader::TokenType token = xmlReader.readNext();
            if(xmlReader.name() == "Reset" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();

                if(JustStarted)
                {
                    Log("Just started, so skip reset");
                    emit Received("<Reset/>");
                }
                else
                {
                    Log("Aboring");
                    JustStarted = true;
                    Abort();
                }

            }
            if(xmlReader.name() == "Load" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();

                NextUrl = xmlReader.text().toString();
                QRegularExpression re("[a-zA-Z]\\:");
                QRegularExpressionMatch match = re.match(NextUrl);
                if(!match.hasMatch())
                {
                    NextUrl.insert(0,QString("http://"));
                }


                if(BrowserTimeout != GeneralTimeout)
                {
                    QString Json = GenerateJson("type","page load","ms",GeneralTimeout);
                    Api("POST",Json,"/timeouts","",this,SLOT(AfterTimeoutChanged()));
                }else
                {
                    QTimer::singleShot(100, this, SLOT(LoadFinal()));
                }


            }

            if(xmlReader.name() == "RemoveLocalProfile" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();

                NextResponce = "<RemoveLocalProfile/>";

                QString FingerprintIdPath = QDir::cleanPath(xmlReader.text().toString() + QDir::separator() + "fingerprint_id.txt");
                QFile FingerprintFile(FingerprintIdPath);
                FingerprintFile.open(QIODevice::ReadOnly);
                QString FingerprintId = QString::fromUtf8(FingerprintFile.readAll());
                FingerprintFile.close();

                if(FingerprintId.isEmpty())
                {
                    SendNextResponce();
                }else
                {
                    IHttpClient * HttpClient = HttpClientFactory->GetHttpClient(false);
                    HttpClient->AddHeader("Expect","");
                    HttpClient->setParent(this);
                    HttpClient->Connect(this,SLOT(SendNextResponceAndDeleteSender()));
                    QVariantList Fingerprints;
                    QVariantMap ParamsMap;
                    Fingerprints.append(FingerprintId);

                    ParamsMap["fingerprintIds"] = Fingerprints;

                    QJsonObject object = QJsonObject::fromVariantMap(ParamsMap);
                    QJsonDocument document;
                    document.setObject(object);

                    QString Params = QString::fromUtf8(document.toJson());


                    QHash<QString,QString> p;
                    p["data"] = Params;
                    PostOptions Options;
                    Options.PrepareStrategy = "custom";
                    HttpClient->AddHeader("Content-Type","application/json");

                    QString url = QString("http://localhost:%1/api/v1/localprofile/remove").arg(QString::number(Port));
                    Log("-> [%1] %2", url, p["data"]);
                    HttpClient->Post(url, p, Options);
                }


            }

            if(xmlReader.name() == "RemoveOnlineProfile" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();

                NextResponce = "<RemoveOnlineProfile/>";
                NextProfileIdDelete = xmlReader.text().toString();


                if(NextProfileIdDelete == GetProfileId().mid(7))
                {
                    //Remove current profile
                    WebDriverProfileRemover * Remover = new WebDriverProfileRemover();
                    connect(Remover, SIGNAL(Removed()), this, SLOT(RemoveOnlineProfileId()));
                    Remover->Remove(HttpClientFactory->GetHttpClient(false),SessionId,ProfileId,Port,MLAKey);
                }else
                {
                    //Remove other profile
                    RemoveOnlineProfileId();
                }
            }

            if(xmlReader.name() == "FindAllOnlineProfiles" && token == QXmlStreamReader::StartElement)
            {

                for(QXmlStreamAttribute attr: xmlReader.attributes())
                {
                    if(attr.name() == "SearchPattern")
                    {
                        OnlineProfileSearchPattern = QString("*") + attr.value().toString() + QString("*");
                    }
                }

                NextResponce = QString("<FindAllOnlineProfiles>%VALUE%</FindAllOnlineProfiles>");

                //Search all profiles
                IHttpClient * HttpClient = HttpClientFactory->GetHttpClient(false);
                HttpClient->AddHeader("Expect","");
                HttpClient->setParent(this);
                HttpClient->Connect(this,SLOT(SearchProfilesResult()));

                QString url = ApiLocation + QString("v1/profile/list?token=") + MLAKey;
                Log("-> [%1]", url);
                HttpClient->Get(url);
            }

            if(xmlReader.name() == "SetStartupScript" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();

                QString Json = GenerateJson("script",xmlReader.text().toString());
                Api("POST",Json,"/startup_script","<SetStartupScript></SetStartupScript>");

            }
            if(xmlReader.name() == "DisableBrowser" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();

                NextResponce = "<DisableBrowser/>";

                WebDriverProfileRemover * Remover = new WebDriverProfileRemover();
                if(ProfileLock.data())
                {
                    ProfileLock->close();
                    ProfileLock->deleteLater();
                    ProfileLock.reset(0);
                }
                connect(Remover, SIGNAL(Removed()), this, SLOT(SendNextResponce()));
                Remover->Remove(HttpClientFactory->GetHttpClient(false),SessionId,ProfileId,Port,MLAKey);


            }
            if(xmlReader.name() == "GetUrl" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                Api("GET","","/url","<GetUrl>%VALUE%</GetUrl>");
            }

            if(xmlReader.name() == "Render" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();

                QString RenderParams = xmlReader.text().toString();

                QStringList Split = RenderParams.split(",");
                int X = Split[0].toInt() + Split[2].toInt()/2;
                int Y = Split[1].toInt() + Split[3].toInt()/2;
                NextResponce = RenderParams;
                ScrollToCoordinates(X,Y,this,SLOT(ResponceScreenshotScroll()));
            }

            if(xmlReader.name() == "AddHeader" && token == QXmlStreamReader::StartElement)
            {
                QString key, value;

                for(QXmlStreamAttribute attr: xmlReader.attributes())
                {
                    if(attr.name() == "name")
                    {
                        key = attr.value().toString();
                    }
                    if(attr.name() == "value")
                    {
                        value = attr.value().toString();
                    }
                }


                QString Json = GenerateJson("key",key,"value",value);
                Api("POST",Json,"/header","<AddHeader></AddHeader>");
            }

            if(xmlReader.name() == "AddCacheMaskAllow" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                QString Mask = xmlReader.text().toString();
                QString Json = GenerateJson("mask",Mask);
                Api("POST",Json,"/cache_mask_allow","<AddCacheMaskAllow></AddCacheMaskAllow>");
            }

            if(xmlReader.name() == "AddCacheMaskDeny" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                QString Mask = xmlReader.text().toString();
                QString Json = GenerateJson("mask",Mask);
                Api("POST",Json,"/cache_mask_deny","<AddCacheMaskDeny></AddCacheMaskDeny>");
            }

            if(xmlReader.name() == "ClearMasks" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                Api("DELETE","","/cache_masks","<ClearMasks></ClearMasks>");
            }

            if(xmlReader.name() == "ClearData" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                Api("DELETE","","/network_data","<ClearData></ClearData>");
            }

            if(xmlReader.name() == "AddRequestMaskAllow" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                QString Mask = xmlReader.text().toString();
                QString Json = GenerateJson("mask",Mask);
                Api("POST",Json,"/request_mask_allow","<AddRequestMaskAllow></AddRequestMaskAllow>");
            }

            if(xmlReader.name() == "AddRequestMaskDeny" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                QString Mask = xmlReader.text().toString();
                QString Json = GenerateJson("mask",Mask);
                Api("POST",Json,"/request_mask_deny","<AddRequestMaskDeny></AddRequestMaskDeny>");
            }

            if(xmlReader.name() == "IsUrlLoadedByMask" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                QString Mask = xmlReader.text().toString();
                QString Json = GenerateJson("mask",Mask);
                Api("POST",Json,"/is_url_loaded","<IsUrlLoadedByMask>%VALUE%</IsUrlLoadedByMask>");
            }

            if(xmlReader.name() == "FindStatusByMask" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                QString Mask = xmlReader.text().toString();
                QString Json = GenerateJson("mask",Mask);
                Api("POST",Json,"/url_load_status","<FindStatusByMask>%VALUE%</FindStatusByMask>");
            }

            if(xmlReader.name() == "FindCacheByMaskString" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                QString Mask = xmlReader.text().toString();
                QString Json = GenerateJson("mask",Mask);
                Api("POST",Json,"/response_body_string","<FindCacheByMaskString>%VALUE%</FindCacheByMaskString>");
            }

            if(xmlReader.name() == "FindCacheByMaskBase64" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                QString Mask = xmlReader.text().toString();
                QString Json = GenerateJson("mask",Mask);
                Api("POST",Json,"/response_body_base64","<FindCacheByMaskBase64>%VALUE%</FindCacheByMaskBase64>");
            }

            if(xmlReader.name() == "SaveCookies" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                Api("GET","","/all_cookies","<SaveCookies>%VALUE%</SaveCookies>");
            }

            if(xmlReader.name() == "RestoreCookies" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                QString Cookies = xmlReader.text().toString();
                QString Json = GenerateJson("cookies",Cookies);
                Api("POST",Json,"/all_cookies","<RestoreCookies></RestoreCookies>");
            }


            if(xmlReader.name() == "GetLoadStats" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();

                Api("GET","","/load_stats","<GetLoadStats>%VALUE%</GetLoadStats>");
            }

            if(xmlReader.name() == "Resize" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                QStringList SizeSplit = xmlReader.text().toString().split(",");

                QString Json = GenerateJson("width",SizeSplit[0].toInt() + WidthDelta,"height",SizeSplit[1].toInt() + HeightDelta);

                QString url;
                if(BrowserEngine == "WebDriverChrome" || BrowserEngine == "MLAMimic")
                {
                    url = "/window/current/size";
                }else if(BrowserEngine == "WebDriverFirefox")
                {
                    url = "/window/rect";
                }
                Api("POST",Json,url,"<Resize/>");
            }

            if(xmlReader.name() == "Scroll" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                QStringList ScrollSplit = xmlReader.text().toString().split(",");
                NextResponce = "<Scroll/>";
                ScrollToCoordinates(ScrollSplit.first().toInt(),ScrollSplit.last().toInt(),this,SLOT(SendNextResponceAndDeleteSender()));
            }

            if(xmlReader.name() == "MouseMove" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                QStringList MouseSplit = xmlReader.text().toString().split(",");
                int X = MouseSplit[0].toInt();
                int Y = MouseSplit[1].toInt();
                LastMouseMoveX = X;
                LastMouseMoveY = Y;
                ScrollToCoordinates(X,Y,this,SLOT(MouseMoveAfterScroll()));
            }

            if((xmlReader.name() == "MouseClick" || xmlReader.name() == "MouseClickUp" || xmlReader.name() == "MouseClickDown") && token == QXmlStreamReader::StartElement)
            {
                if(xmlReader.name() == "MouseClickUp")
                {
                    MouseUrl = "/buttonup";
                    NextResponce = "<MouseClickUp/>";
                }else if(xmlReader.name() == "MouseClickDown")
                {
                    MouseUrl = "/buttondown";
                    NextResponce = "<MouseClickDown/>";
                }else
                {
                    MouseUrl = "/click";
                    NextResponce = "<MouseClick/>";
                }
                xmlReader.readNext();
                QStringList MouseSplit = xmlReader.text().toString().split(",");
                int X = MouseSplit[0].toInt();
                int Y = MouseSplit[1].toInt();
                if(LastMouseMoveX == X && LastMouseMoveY == Y)
                {
                    QString Json = GenerateJson("button",0);
                    Api("POST",Json,MouseUrl,"NOCHANGE");
                }else
                {
                    ScrollToCoordinates(X,Y,this,SLOT(MouseMoveAfterScrollBeforeClick()));
                }
            }

            if(xmlReader.name() == "Visible" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                if(xmlReader.text() == "1")
                {
                    if(Viewer == 0)
                    {
                        Viewer = new WebDriverBrowserViewer();
                        connect(Viewer,SIGNAL(NeedScreenshot()),this,SLOT(NeedScreenshot()));
                        connect(Viewer,SIGNAL(Closed()),this,SLOT(ResetViewer()));
                        connect(this,SIGNAL(ViewerData(QImage,int,int)),Viewer,SLOT(ViewerData(QImage,int,int)));
                        Viewer->show();
                    }else
                    {
                        Viewer->show();
                    }
                }else
                {
                    if(Viewer)
                    {
                        Viewer->deleteLater();
                        Viewer = 0;
                    }
                }

            }

            if(xmlReader.name() == "ResetNoCookies" && token == QXmlStreamReader::StartElement)
            {
               xmlReader.readNext();
               SendReceived("<Reset></Reset>");
            }

            if(xmlReader.name() == "Element" && token == QXmlStreamReader::StartElement)
            {
                QJsonArray Path;
                xmlReader.readNext();
                ElementCommand = xmlReader.name().toString();
                for(QXmlStreamAttribute attr: xmlReader.attributes())
                {
                    if(attr.name() == "ID")
                    {
                        ElementId = attr.value().toString();
                    }

                }

                while(true)
                {
                    QXmlStreamReader::TokenType token = xmlReader.readNext();

                    if(xmlReader.name() == "Element" && token == QXmlStreamReader::EndElement)
                    {
                        break;
                    }


                    if(xmlReader.name() == "A" && token == QXmlStreamReader::StartElement)
                    {
                        for(QXmlStreamAttribute attr: xmlReader.attributes())
                        {
                            if(attr.name() == "value")
                            {
                                ElementArgumentA = attr.value().toString();
                            }
                        }
                    }
                    if(xmlReader.name() == "B" && token == QXmlStreamReader::StartElement)
                    {
                        for(QXmlStreamAttribute attr: xmlReader.attributes())
                        {
                            if(attr.name() == "value")
                            {
                                ElementArgumentB = attr.value().toString();
                            }
                        }
                    }
                    if(xmlReader.name() == "Path" && token == QXmlStreamReader::StartElement)
                    {
                        while(true)
                        {
                            QXmlStreamReader::TokenType token = xmlReader.readNext();
                            if(xmlReader.name() == "Path" && token == QXmlStreamReader::EndElement)
                            {
                                break;
                            }

                            if(token == QXmlStreamReader::StartElement)
                            {
                                Path.append(xmlReader.name().toString());
                                xmlReader.readNext();
                                Path.append(xmlReader.text().toString());
                            }

                        }
                    }
                }

                QJsonDocument doc(Path);
                ElementSelector = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));

                ExecuteElementAction();

            }

        }
    }

    void WebDriveProcessComunicator::ExecuteElementAction()
    {
        NextResponce=QString("<Element ID=\"%1\"><%2>%VALUE%</%2></Element>").arg(ElementId).arg(ElementCommand);

        if(ElementCommand == "script")
        {
            QString script =
                    QString("if(!self){callback('');return;};") +
                    QString("var obj=null;try{obj = ") + ElementArgumentA + QString(";}catch(e){}") +
                    QString("var res='';if(typeof(obj)!='undefined'&&obj !== null){res=obj.toString()}callback(res);");


            FindElementAndExecuteScript(ElementSelector,script,this,SLOT(SendNextResponceAndDeleteSender()),false);
        }

        if(ElementCommand == "attr")
        {
            QString script = QString("if(!self){callback('BAS_NOT_EXISTS');return;};var res='';var attr=") + EscapeStringJavascript(ElementArgumentA) + QString(";if(self){if(self.hasAttribute(attr))res=self.getAttribute(attr);}callback(res);");
            FindElementAndExecuteScript(ElementSelector,script,this,SLOT(SendNextResponceAndDeleteSender()),false);
        }

        if(ElementCommand == "set_attr")
        {
            QString script = QString("if(!self){callback('BAS_NOT_EXISTS');return;};var attr=") + EscapeStringJavascript(ElementArgumentA) + QString(";var val=") + EscapeStringJavascript(ElementArgumentB) + QString(";if(self){if(val.length === 0)self.removeAttribute(attr);else self.setAttribute(attr,val);}callback('');");
            FindElementAndExecuteScript(ElementSelector,script,this,SLOT(SendNextResponceAndDeleteSender()),false);
        }

        if(ElementCommand == "length")
        {
            QString script = QString("if(!self){callback('BAS_NOT_EXISTS');return;};var res = '';if(self){res = self.length;}callback(res);");
            FindElementAndExecuteScript(ElementSelector,script,this,SLOT(SendNextResponceAndDeleteSender()),false);
        }



        if(ElementCommand == "xml")
        {
            QString script = QString("if(!self){callback('BAS_NOT_EXISTS');return;};var res = '';if(self){res = self.outerHTML}callback(res);");
            FindElementAndExecuteScript(ElementSelector,script,this,SLOT(SendNextResponceAndDeleteSender()),false);
        }

        if(ElementCommand == "focus")
        {
            QString script = QString("callback({value:'',coordinates:coordinates});");
            FindElementAndExecuteScript(ElementSelector,script,this,SLOT(SendNextResponceAndDeleteSender()),true);
        }

        if(ElementCommand == "text")
        {
            QString script = QString("if(!self){callback('BAS_NOT_EXISTS');return;};var res = '';try{if(self){if(self.tagName.toLowerCase()=='input'||self.tagName.toLowerCase()=='textarea')res=self.value;else res=self.textContent}}catch(e){}callback(res);");
            FindElementAndExecuteScript(ElementSelector,script,this,SLOT(SendNextResponceAndDeleteSender()), false);
        }

        if(ElementCommand == "style")
        {
            QString arg = EscapeStringJavascript(ElementArgumentA);
            QString script = QString("if(!self){callback('BAS_NOT_EXISTS');return;};var res='';if(self)res=window.getComputedStyle(self)[") + arg + QString("];callback(res);");
            FindElementAndExecuteScript(ElementSelector,script,this,SLOT(SendNextResponceAndDeleteSender()),false);
        }

        if(ElementCommand == "exist")
        {
            QString script = QString("var res;if(self)res='1';else res='0';callback(res);");
            FindElementAndExecuteScript(ElementSelector,script,this,SLOT(SendNextResponceAndDeleteSender()),false);
        }

        if(ElementCommand == "render_base64")
        {
            QString script = "if(!self){callback('BAS_NOT_EXISTS');return;}callback(coordinates);";
            FindElementAndExecuteScript(ElementSelector,script,this,SLOT(RenderBase64AfterScroll()),true);
        }

        if(ElementCommand == "random_point")
        {
            QString get_point =
                    QString("var x=0;for(var i=0;i<10;i++){x+=Math.random()*((rect.right-2-rect.left+1)/10);};x=Math.floor(x)+rect.left+1;if(x>rect.right-1)x=rect.right-1;if(x<rect.left+1)x=rect.left+1;"
                            "var y=0;for(var i=0;i<10;i++){y+=Math.random()*((rect.bottom-2-rect.top+1)/10);};y=Math.floor(y)+rect.top+1;if(y>rect.bottom-1)y=rect.bottom-1;if(y<rect.top+1)y=rect.top+1;");
            QString script = QString("{"
                                 "if(!self){callback('BAS_NOT_EXISTS');return;}"
                                 "var items=self.getClientRects();if(items.length == 0){callback('BAS_NOT_EXISTS');return;};"
                                 "var rect=items[Math.floor(Math.random()*items.length)];")
                                 + get_point +
                                 QString("x+=scrollx + positionx;"
                                 "y+=scrolly + positiony;"
                                 "var res=parseInt(x)+','+parseInt(y);"
                                 "callback({value: res,coordinates: coordinates});}"
                            );

            script = QString("(function(){") + script + QString("})()");
            FindElementScrollAndExecuteScriptAdjustCoordinates(ElementSelector,script,this,SLOT(SendNextResponceAndDeleteSender()));
        }

        if(ElementCommand == "clarify")
        {
            QString get_point =
                    QString("var x=0;for(var i=0;i<10;i++){x+=Math.random()*((rect.right-2-rect.left+1)/10);};x=Math.floor(x)+rect.left+1;if(x>rect.right-1)x=rect.right-1;if(x<rect.left+1)x=rect.left+1;"
                            "var y=0;for(var i=0;i<10;i++){y+=Math.random()*((rect.bottom-2-rect.top+1)/10);};y=Math.floor(y)+rect.top+1;if(y>rect.bottom-1)y=rect.bottom-1;if(y<rect.top+1)y=rect.top+1;");

            QString script = QString("{"
                                 "if(!self){callback('BAS_NOT_EXISTS');return;}"
                                 "var items=self.getClientRects();if(items.length == 0){callback('BAS_NOT_EXISTS');return;};"
                                 "var len = items.length;"
                                 "for(var i = 0;i<len;i++)"
                                 "{"
                                    "var item = items[i];"
                                    "var x = ") + ElementArgumentA + QString(" - parseInt(scrollx) - parseInt(positionx);"
                                    ";var y = ") + ElementArgumentB + QString(" - parseInt(scrolly) - parseInt(positiony);"
                                    ";if(y >= item.top && y <= item.bottom && x >= item.left && x <= item.right)"
                                    "{"
                                       "callback('');"
                                       "return;"
                                    "}"
                                 "}"
                                 "var rect=items[Math.floor(Math.random()*items.length)];")
                                 + get_point +
                                 QString("x+= scrollx + positionx;"
                                 "y += scrolly + positiony;"
                                 "var res=parseInt(x)+','+parseInt(y);"
                                 "callback({value: res,coordinates: coordinates});}");
            script = QString("(function(){") + script + QString("})()");
            FindElementScrollAndExecuteScriptAdjustCoordinates(ElementSelector,script,this,SLOT(SendNextResponceAndDeleteSender()));

        }

        if(ElementCommand == "type")
        {
            if(ElementArgumentA == "<MOUSERIGHT>")
            {
                QString Json = GenerateJson("button",2);
                Api("POST",Json,"/click","NOCHANGE");
            }else if(ElementArgumentA == "<MOUSEDOUBLE>")
            {
                Api("POST","","/doubleclick","NOCHANGE");
            }else
                Type(ElementArgumentA,100,this,SLOT(SendNextResponceAndDeleteSender()));
        }

    }

    void WebDriveProcessComunicator::ResetViewer()
    {
        SendReceived("<Minimized>1</Minimized>");
        if(Viewer)
        {
            Viewer->deleteLater();
            Viewer = 0;
        }
    }


    /* Custom Logic */

    void WebDriveProcessComunicator::ResponceScreenshotScroll()
    {
        QString value = LogHttpClientDeleteAndGetValue(sender());
        NextResponce = value + QString(",") + NextResponce;
        Api("GET","","/screenshot","NOCHANGE",this,SLOT(ResponceScreenshot()));
    }

    void WebDriveProcessComunicator::ResponceScreenshot()
    {
        QString value = LogHttpClientDeleteAndGetValue(sender());

        QImage image = QImage::fromData(QByteArray::fromBase64(value.toUtf8()));
        QStringList Split = NextResponce.split(",");
        int w2 = Split[2 + 4].toInt() / 2;
        int h2 = Split[3 + 4].toInt() / 2;
        image = image.copy(Split[2].toInt() - Split[0].toInt() - w2, Split[3].toInt() - Split[1].toInt() - h2, Split[2 + 4].toInt(), Split[3 + 4].toInt());

        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);

        image.save(&buffer, "PNG");
        buffer.close();
        value = QString::fromUtf8(ba.toBase64());

        QString resp = QString("<Render>") + value + QString("</Render>");
        SendReceived(resp);
    }

    void WebDriveProcessComunicator::ResponceResizeGuess()
    {
        LogHttpClientAndDelete(sender());
        QString Json = GenerateJson("script","return window.innerWidth.toString() + ',' + window.innerHeight.toString()","args",QStringList());
        Api("POST",Json,"/execute","",this,SLOT(ResponceResizeExact()));

    }
    void WebDriveProcessComunicator::ResponceResizeExact()
    {
        QString value = LogHttpClientDeleteAndGetValue(sender());

        QStringList List = value.split(",");
        WidthDelta = 1024 - List[0].toInt();
        HeightDelta = 600 - List[1].toInt();

        QString Json = GenerateJson("width",1024 + WidthDelta,"height",600 + HeightDelta);
        QString url;
        if(BrowserEngine == "WebDriverChrome" || BrowserEngine == "MLAMimic")
        {
            url = "/window/current/size";
        }else if(BrowserEngine == "WebDriverFirefox")
        {
            url = "/window/rect";
        }
        Api("POST",Json,url,"",this,SLOT(SetInitialTimeoutPageLoad()));
    }

    void WebDriveProcessComunicator::SetInitialTimeoutPageLoad()
    {
        LogHttpClientAndDelete(sender());
        QString Json = GenerateJson("type","page load","ms",60000.0);
        Api("POST",Json,"/timeouts","",this,SLOT(SetInitialTimeoutScript()));
    }

    void WebDriveProcessComunicator::SetInitialTimeoutScript()
    {
        LogHttpClientAndDelete(sender());
        QString Json = GenerateJson("ms",10000.0);
        Api("POST",Json,"/timeouts/async_script","",this,SLOT(ProcessCreatedFinal()));
    }


    void WebDriveProcessComunicator::ProcessCreatedFinal()
    {
        LogHttpClientAndDelete(sender());
        emit ProcessStarted();
    }

    void WebDriveProcessComunicator::AfterTimeoutChanged()
    {
        LogHttpClientAndDelete(sender());
        BrowserTimeout = GeneralTimeout;
        LoadFinal();
    }

    void WebDriveProcessComunicator::LoadFinal()
    {
        QString Json = GenerateJson("url",NextUrl);
        NextUrl.clear();
        SendReceived("<LoadedInstant/>");
        Api("POST",Json,"/url","<Load>0</Load>");
    }

    void WebDriveProcessComunicator::RemoveOnlineProfileId()
    {
        IHttpClient * HttpClient = HttpClientFactory->GetHttpClient(false);
        HttpClient->AddHeader("Expect","");
        HttpClient->setParent(this);
        HttpClient->Connect(this,SLOT(SendNextResponceAndDeleteSender()));

        QString url = ApiLocation + QString("v1/profile/remove?profileId=") + NextProfileIdDelete + QString("&token=") + MLAKey;
        Log("-> [%1]", url);
        HttpClient->Get(url);
    }

    void WebDriveProcessComunicator::MouseMoveAfterScroll()
    {
        QString val = LogHttpClientDeleteAndGetValue(sender());

        QStringList list = val.split(",");
        if(list.length() == 4)
        {
            ScrollX = list[0].toInt();
            ScrollY = list[1].toInt();
            PositionTopLeftX = list[2].toInt();
            PositionTopLeftY = list[3].toInt();
        }

        QString Json = GenerateJson("element",QVariant(),"xoffset",PositionTopLeftX - ScrollX - MouseX,"yoffset",PositionTopLeftY - ScrollY - MouseY);
        MouseX = PositionTopLeftX - ScrollX;
        MouseY = PositionTopLeftY - ScrollY;
        Api("Post",Json,"/moveto","<MouseMove/>");
    }

    void WebDriveProcessComunicator::MouseMoveAfterScrollBeforeClick()
    {
        QString val = LogHttpClientDeleteAndGetValue(sender());

        QStringList list = val.split(",");
        if(list.length() == 4)
        {
            ScrollX = list[0].toInt();
            ScrollY = list[1].toInt();
            PositionTopLeftX = list[2].toInt();
            PositionTopLeftY = list[3].toInt();
        }
        QString Json = GenerateJson("element",QVariant(),"xoffset",PositionTopLeftX - ScrollX - MouseX,"yoffset",PositionTopLeftY - ScrollY - MouseY);
        MouseX = PositionTopLeftX - ScrollX;
        MouseY = PositionTopLeftY - ScrollY;
        Api("Post",Json,"/moveto","NOCHANGE", this, SLOT(MouseClickAfterScroll()));
    }

    void WebDriveProcessComunicator::MouseClickAfterScroll()
    {
        QString val = LogHttpClientDeleteAndGetValue(sender());

        QStringList list = val.split(",");
        if(list.length() == 4)
        {
            ScrollX = list[0].toInt();
            ScrollY = list[1].toInt();
            PositionTopLeftX = list[2].toInt();
            PositionTopLeftY = list[3].toInt();
        }
        QString Json = GenerateJson("button",0);
        Api("POST",Json,MouseUrl,"NOCHANGE");
    }

    void WebDriveProcessComunicator::NeedScreenshot()
    {
        Api("GET","","/screenshot","NOCHANGE",this,SLOT(ResponceScreenshotForWidget()));
    }

    void WebDriveProcessComunicator::ResponceScreenshotForWidget()
    {
        QString value = LogHttpClientDeleteAndGetValue(sender());
        QImage image = QImage::fromData(QByteArray::fromBase64(value.toUtf8()));
        emit ViewerData(image,MouseX,MouseY);
    }

    void WebDriveProcessComunicator::Type(const QString& Text,int Interval, QObject *object, const char * slot)
    {
        QString TextCopy = Text;
        TextCopy = TextCopy.replace("<RETURN>",QChar(0xE007));
        TextCopy = TextCopy.replace("<CANCEL>",QChar(0xE001));
        TextCopy = TextCopy.replace("<BACK>",QChar(0xE003));
        TextCopy = TextCopy.replace("<TAB>",QChar(0xE004));
        TextCopy = TextCopy.replace("<CLEAR>",QChar(0xE005));
        TextCopy = TextCopy.replace("<CAPITAL>","");
        TextCopy = TextCopy.replace("<ESCAPE>",QChar(0xE00C));
        TextCopy = TextCopy.replace("<PRIOR>","");
        TextCopy = TextCopy.replace("<NEXT>","");
        TextCopy = TextCopy.replace("<END>",QChar(0xE010));
        TextCopy = TextCopy.replace("<HOME>",QChar(0xE011));
        TextCopy = TextCopy.replace("<LEFT>",QChar(0xE012));
        TextCopy = TextCopy.replace("<UP>",QChar(0xE013));
        TextCopy = TextCopy.replace("<RIGHT>",QChar(0xE014));
        TextCopy = TextCopy.replace("<DOWN>",QChar(0xE015));

        TextCopy = TextCopy.replace("<SELECT>","");
        TextCopy = TextCopy.replace("<PRINT>","");
        TextCopy = TextCopy.replace("<EXECUTE>","");
        TextCopy = TextCopy.replace("<SNAPSHOT>","");
        TextCopy = TextCopy.replace("<INSERT>",QChar(0xE016));
        TextCopy = TextCopy.replace("<DELETE>",QChar(0xE017));

        TextCopy = TextCopy.replace("<MOUSESCROLLUP>",QChar(0xE032));
        TextCopy = TextCopy.replace("<MOUSESCROLLDOWN>",QChar(0xE031));

        /* Ctrl */
        TextCopy = TextCopy.replace("<CONTROL>",QChar(0xE009));
        /* Alt */
        TextCopy = TextCopy.replace("<MENU>",QChar(0xE00A));
        /* Shift */
        TextCopy = TextCopy.replace("<SHIFT>",QChar(0xE008));

        QStringList list = TextCopy.split("");
        list.removeAll("");
        int index = 0;
        bool IsInsideSpecial = false;
        while(true)
        {
            if(index >= list.length())
                break;
            bool IsSpecial = list[index] == QChar(0xE009) || list[index] == QChar(0xE00A) || list[index] == QChar(0xE008);
            if(!IsSpecial && IsInsideSpecial)
            {
                index++;
                list.insert(index,QChar(0xE000));
            }
            IsInsideSpecial = IsSpecial;
            index++;
        }

        QString Json = GenerateJson("value",list);
        Api("POST",Json,"/keys","NOCHANGE",object,slot);
    }

    void WebDriveProcessComunicator::RenderBase64AfterScroll()
    {
        QString value = LogHttpClientDeleteAndGetValue(sender());
        if(value == "BAS_NOT_EXISTS")
        {
            ExecuteElementAction();
            return;
        }
        QStringList list = value.split(",");
        ScrollX = list[0].toInt();
        ScrollY = list[1].toInt();
        PositionTopLeftX = list[4].toInt();
        PositionTopLeftY = list[5].toInt();
        PositionRightBottomX = list[6].toInt();
        PositionRightBottomY = list[7].toInt();
        Api("GET","","/screenshot","NOCHANGE",this,SLOT(RenderBase64Final()));

    }

    void WebDriveProcessComunicator::RenderBase64Final()
    {
        QString value = LogHttpClientDeleteAndGetValue(sender());

        QImage image = QImage::fromData(QByteArray::fromBase64(value.toUtf8()));
        image = image.copy(PositionTopLeftX + ExpandFramesX, PositionTopLeftY + ExpandFramesY, PositionRightBottomX - PositionTopLeftX, PositionRightBottomY - PositionTopLeftY);

        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);

        image.save(&buffer, "PNG");
        buffer.close();
        value = QString::fromUtf8(ba.toBase64());

        SendReceived(NextResponce.replace("%VALUE%",value));
    }

}
