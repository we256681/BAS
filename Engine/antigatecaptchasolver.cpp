#include "antigatecaptchasolver.h"
#include <QHash>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    AntigateCaptchaSolver::AntigateCaptchaSolver(QObject *parent) :
        ISolver(parent),Iterator(0),StartedMonitor(false), timeout(8000), delay(-1), MultipleIds(true), StartImmediate(false)
    {
        Server = "http://antigate.com/";
    }

    void AntigateCaptchaSolver::SetStartImmediate(bool StartImmediate)
    {
        this->StartImmediate = StartImmediate;
    }

    void AntigateCaptchaSolver::SetTimeout(int timeout)
    {
        this->timeout = timeout;
    }

    void AntigateCaptchaSolver::SetDelay(int delay)
    {
        this->delay = delay;
    }

    void AntigateCaptchaSolver::SetSoftId(const QString& SoftId)
    {
        this->SoftId = SoftId;
    }

    void AntigateCaptchaSolver::SetMultipleIds(bool MultipleIds)
    {
        this->MultipleIds = MultipleIds;
    }

    void AntigateCaptchaSolver::StartMonitor()
    {
        if(!StartedMonitor)
        {
            HttpClient = HttpClientFactory->GetHttpClient();
            HttpClient->setParent(this);
            timer.setSingleShot(true);
            if(StartImmediate)
                timer.start(10);
            else
                timer.start(delay == -1 ? timeout : delay);
            connect(&timer, SIGNAL(timeout()), this, SLOT(StartIteration()));
            StartedMonitor = true;
        }

    }

    void AntigateCaptchaSolver::SetServer(const QString& Server)
    {
        this->Server = Server;
    }

    void AntigateCaptchaSolver::StartSingleIteration()
    {
        QTimer * Timer = qobject_cast<QTimer*>(sender());

        if(!Timer)
            return;

        SingleIdWorker *Worker = 0;
        for(SingleIdWorker* w: Workers)
        {
            if(Timer == w->timer)
                Worker = w;
        }
        if(!Worker)
            return;

        Worker->client->Connect(this,SLOT(DoneSignleIteration()));

        if(Worker->is_json_interface)
        {
            QJsonObject Obj;

            Obj["clientKey"] = QJsonValue::fromVariant(key);
            Obj["taskId"] = QJsonValue::fromVariant(Worker->antigate_id.toInt());

            QJsonDocument doc(Obj);
            QString Json = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));


            QHash<QString,QString> p;
            p["data"] = Json;

            PostOptions Options;
            Options.Method = "POST";
            Options.PrepareStrategy = "custom";
            Worker->client->AddHeader("Content-Type","application/json");
            Worker->client->Post(Server + "getTaskResult",p,Options);
        }else
        {
            QString url = QString(Server + "res.php?key=%1&action=get&id=%2").arg(key).arg(Worker->antigate_id);
            Worker->client->Get(url);
        }



        //qDebug()<<"Sending"<<url<<"Length"<<Workers.size();
    }

    void AntigateCaptchaSolver::StartIteration()
    {
        QString ids = "";
        QList<QString> keys = IdToAntigateIdList.keys();
        int size = IdToAntigateIdList.size();
        for(int i = 0;i<size;i++)
        {
           if(i)
               ids.append(",");
           ids.append(keys.at(i));
        }
        if(size>0)
        {
            HttpClient->Connect(this,SLOT(DoneIteration()));
            HttpClient->Get(QString(Server + "res.php?key=%1&action=get&ids=%2").arg(key).arg(ids));
        }else
        {
            timer.start(timeout);
        }
    }


    void AntigateCaptchaSolver::DoneSignleIteration()
    {
        IHttpClient * Client = qobject_cast<IHttpClient*>(sender());

        if(!Client)
            return;

        SingleIdWorker *Worker = 0;
        for(SingleIdWorker* w: Workers)
        {
            if(Client == w->client)
                Worker = w;
        }
        if(!Worker)
            return;

        if(Worker->client->WasError())
        {
            Worker->timer->start(timeout);
            return;
        }

        QString res = Worker->client->GetContent();

        if(Worker->is_json_interface)
        {
            QJsonParseError err;
            QJsonDocument Document = QJsonDocument::fromJson(res.toUtf8(), &err);
            if(err.error || !Document.isObject())
            {
                Worker->timer->start(timeout);
                return;
            }

            QJsonObject Object = Document.object();

            //Has error, close
            if(
                    Object.contains("errorDescription")
                    && Object.value("errorDescription").isString()
                    && !Object.value("errorDescription").toString().isEmpty()
               )
            {
                if(Worker->is_recaptcha_v3)
                {
                    emit DoneRecatpchaV3(Object.value("errorDescription").toString(),Worker->id,false,Worker->antigate_id);
                }else
                {
                    emit Done(Object.value("errorDescription").toString(),Worker->id,false,Worker->antigate_id);
                }
            }

            //Processing, check later
            if(
                    Object.contains("status")
                    && Object.value("status").isString()
                    && Object.value("status").toString() == "processing"
              )
            {
                Worker->timer->start(timeout);
                return;
            }


            if(Object.contains("solution") && Object.value("solution").isObject())
            {
                QJsonObject SolutionObject = Object.value("solution").toObject(); 

                if(SolutionObject.contains("gRecaptchaResponse") && SolutionObject.value("gRecaptchaResponse").isString())
                {
                    QString Solution = Object.value("solution").toObject().value("gRecaptchaResponse").toString();

                    if(Worker->is_recaptcha_v3)
                    {
                        emit DoneRecatpchaV3(Solution,Worker->id,true,Worker->antigate_id);
                    }else
                    {
                        emit Done(Solution,Worker->id,true,Worker->antigate_id);
                    }
                }else
                {
                    QString Solution = QJsonDocument(SolutionObject).toJson(QJsonDocument::Compact);
                    emit Done(Solution, Worker->id, true, Worker->antigate_id);
                }
            }else
            {
                //No response and no error, retry
                Worker->timer->start(timeout);
                return;
            }



        }else
        {
            if(res!="CAPCHA_NOT_READY")
            {
                bool Success = false;
                if(res.startsWith("OK|"))
                {
                    res = res.remove(0,3);
                    Success = true;
                }
                if(Worker->is_recaptcha_v3)
                {
                    emit DoneRecatpchaV3(res,Worker->id,Success,Worker->antigate_id);
                }else
                {
                    emit Done(res,Worker->id,Success,Worker->antigate_id);
                }
                Workers.removeAll(Worker);
                Worker->deleteLater();

                //qDebug()<<"Finished"<<res<<Worker->id<<Success<<Worker->antigate_id<<"Length"<<Workers.size();
            }else
            {
                Worker->timer->start(timeout);
            }
        }
    }

    void AntigateCaptchaSolver::DoneIteration()
    {

        if(HttpClient->WasError())
        {
            timer.start(timeout);
            return;
        }

        QString res = HttpClient->GetContent();

        QStringList list = res.split("|");
        if(list.size()!=IdToAntigateIdList.size())
        {
            timer.start(timeout);
            return;
        }
        QList<QString> v = IdToAntigateIdList.values();
        QList<QString> k = IdToAntigateIdList.keys();
        QList<QString> to_delete;
        for(int i = 0;i<list.size();i++)
        {
            if(list[i]!="CAPCHA_NOT_READY")
            {
                to_delete.append(k[i]);
                if(RecaptchaV3Ids.contains(k[i]))
                {
                    emit DoneRecatpchaV3(list[i],v[i],true,k[i]);
                }else
                {
                    emit Done(list[i],v[i],true,k[i]);
                }
            }
        }
        foreach(QString d, to_delete)
        {
            IdToAntigateIdList.remove(d);
            RecaptchaV3Ids.removeAll(d);
        }
        timer.start(timeout);
    }


    QString AntigateCaptchaSolver::Solve(const QString& base64,const QStringList& props)
    {
        QMap<QString,QString> Properties;
        bool isname = true;
        QString name = "";
        bool DisableImageConvert = false;
        bool IsRecaptchaV3 = false;
        bool IsJsonInterface = false;
        QString IdOverride;
        foreach(QString str, props)
        {
            if(isname)
            {
                name = str;
            }else
            {
                if(name == "key" || name == "service_key")
                {
                    key = str;
                }else if(name == "bas_disable_image_convert")
                {
                    DisableImageConvert = str.toInt();
                }else if(name == "serverurl" || name == "service_url")
                {
                    Server = !str.endsWith("/") ? str.append("/") : str;
                }else if(name == "timeout")
                {
                    SetTimeout(str.toInt());
                }else if(name == "delay")
                {
                    SetDelay(str.toInt());
                }else if(name == "recatpchav3")
                {
                    IsRecaptchaV3 = true;
                }else if(name == "id_override")
                {
                    IdOverride = str;
                }else if(name == "is_json_interface")
                {
                    IsJsonInterface = str == "1";
                }
                else if(!str.isEmpty())
                {
                    Properties.insert(name,str);
                }

            }
            isname = !isname;
        }

        emit Used();
        QString i;
        if(IdOverride.isEmpty())
            i = QString::number(Iterator++);
        else
            i = IdOverride;
        PostToAntigate * post = new PostToAntigate(this);
        post->SetServer(Server);
        IHttpClient * client = HttpClientFactory->GetHttpClient();
        client->setParent(post);
        post->SetHttpClient(client);
        connect(post,SIGNAL(PostedToAntigate(QString,QString,bool,bool,bool)),this,SLOT(PostedToAntigate(QString,QString,bool,bool,bool)));
        post->Post(i,key,base64,Properties,SoftId,DisableImageConvert,IsRecaptchaV3,IsJsonInterface);

        return i;
    }

    void AntigateCaptchaSolver::PostedToAntigate(const QString& antigate_id, const QString& id, bool res, bool is_recaptcha_v3, bool is_json_interface)
    {
        sender()->deleteLater();
        if(res)
        {
            if(MultipleIds && !is_json_interface)
            {
                IdToAntigateIdList[antigate_id] = id;
                if(is_recaptcha_v3)
                    RecaptchaV3Ids.append(id);
                StartMonitor();
            }else
            {
                SingleIdWorker * Worker = new SingleIdWorker();
                Worker->setParent(this);
                Worker->id = id;
                Worker->is_recaptcha_v3 = is_recaptcha_v3;
                Worker->is_json_interface = is_json_interface;
                Worker->antigate_id = antigate_id;
                Worker->timer = new QTimer(Worker);

                Worker->client = HttpClientFactory->GetHttpClient();
                Worker->client->setParent(Worker);
                Worker->timer->setSingleShot(true);
                Worker->timer->start(delay == -1 ? timeout : delay);
                connect(Worker->timer, SIGNAL(timeout()), this, SLOT(StartSingleIteration()));
                Workers.append(Worker);

            }
        }
        else
        {
            if(is_recaptcha_v3)
            {
                emit DoneRecatpchaV3(antigate_id,id,false,0);
            }else
            {
                emit Done(antigate_id,id,false,0);
            }
        }

    }

    void AntigateCaptchaSolver::ReportBad(const QString& id)
    {
        emit Failed();
        IHttpClient * client = HttpClientFactory->GetHttpClient();
        client->setParent(this);
        client->Connect(this,SLOT(DeleteSender()));
        QString url = Server + "res.php?action=reportbad&id=" + id + "&key=" + key;
        client->Get(url);
    }

    void AntigateCaptchaSolver::DeleteSender()
    {
        //IHttpClient *Client = qobject_cast<IHttpClient *>(sender());
        sender()->deleteLater();
    }

    void AntigateCaptchaSolver::SetHttpClientFactory(IHttpClientFactory * HttpClientFactory)
    {
        this->HttpClientFactory = HttpClientFactory;
    }

    IHttpClientFactory * AntigateCaptchaSolver::GetHttpClientFactory()
    {
        return HttpClientFactory;
    }

    bool AntigateCaptchaSolver::TimeLimited()
    {
        return true;
    }
}
