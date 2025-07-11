#include "posttoantigate.h"
#include <QMap>
#include <QPixmap>
#include <QBuffer>
#include <QMapIterator>
#include <QJsonObject>
#include <QJsonDocument>
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    PostToAntigate::PostToAntigate(QObject *parent) :
        QObject(parent)
    {
    }

    void PostToAntigate::Post(const QString& id,const QString& key,const QString& base64, const QMap<QString,QString>& Properties, const QString& soft, bool DisableImageConvert, bool IsRecaptchaV3, bool IsJsonInterface)
    {
        this->id = id;
        this->IsRecaptchaV3 = IsRecaptchaV3;
        this->IsJsonInterface = IsJsonInterface;

        if(!IsJsonInterface)
        {
            QHash<QString,ContentData> post;

            {
                ContentData DataMethod;
                DataMethod.DataString = "post";
                post["method"] = DataMethod;
            }

            {
                ContentData DataKey;
                DataKey.DataString = key;
                post["key"] = DataKey;
            }

            if(!base64.isEmpty())
            {
                ContentData DataFile;

                if(DisableImageConvert)
                {
                    DataFile.DataRaw = QByteArray::fromBase64(base64.toUtf8());
                }else
                {
                    QImage image = QImage::fromData(QByteArray::fromBase64(base64.toUtf8()));

                    QByteArray ba;
                    QBuffer buffer(&ba);
                    buffer.open(QIODevice::WriteOnly);

                    image.save(&buffer, "JPEG");
                    buffer.close();

                    DataFile.DataRaw = ba;
                }
                DataFile.FileName = "image.jpg";
                DataFile.ContentType = "application/octet-stream";
                post["file"] = DataFile;
            }

            if(!soft.isEmpty())
            {
                ContentData DataSoftId;
                DataSoftId.DataString = soft;

                post["soft_id"] = DataSoftId;
            }

            QMapIterator<QString, QString> i(Properties);
            while (i.hasNext())
            {
                i.next();
                if(i.value().length() > 0 && i.key().length() > 0)
                {
                    ContentData DataKey;
                    DataKey.DataString = i.value();
                    post[i.key()] = DataKey;
                }
            }
            HttpClient->Connect(this,SLOT(submitRequestFinished()));
            HttpClient->Post(Server + "in.php",post);
        }else
        {
            QJsonObject Obj;

            QString MinScore;

            Obj["clientKey"] = QJsonValue::fromVariant(key);

            if(!soft.isEmpty())
            {
                Obj["softId"] = QJsonValue::fromVariant(soft);
            }

            QVariantMap Map;

            QMapIterator<QString, QString> i(Properties);
            while (i.hasNext())
            {
                i.next();
                if(i.value().length() > 0 && i.key().length() > 0)
                {
                    if(i.key() == "minScore")
                    {
                        float Val = i.value().toDouble();

                        if(Val < 0.5)
                            Val = 0.3;
                        else if(Val < 0.8)
                            Val = 0.7;
                        else
                            Val = 0.9;

                        MinScore = QString::number(Val, 'f', 1);

                        Map[i.key()] = "_BAS_REPLACE_SCORE_";
                    }else
                    {
                        Map[i.key()] = i.value();
                    }
                }
            }

            Obj["task"] = QJsonValue::fromVariant(Map);


            QJsonDocument doc(Obj);
            QString Json = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));

            Json = Json.replace("\"_BAS_REPLACE_SCORE_\"",MinScore);


            QHash<QString,QString> p;
            p["data"] = Json;

            PostOptions Options;
            Options.Method = "POST";
            Options.PrepareStrategy = "custom";
            HttpClient->AddHeader("Content-Type","application/json");


            HttpClient->Connect(this,SLOT(submitRequestFinished()));
            HttpClient->Post(Server + "createTask",p,Options);
        }



    }

    void PostToAntigate::SetServer(const QString& Server)
    {
        this->Server = Server;
    }

    void PostToAntigate::submitRequestFinished()
    {
        if(HttpClient->WasError())
        {
            emit PostedToAntigate(HttpClient->GetErrorString(),id,false,IsRecaptchaV3,IsJsonInterface);
            return;
        }
        QString res = HttpClient->GetContent();

        if(IsJsonInterface)
        {
            QJsonParseError err;
            QJsonDocument Document = QJsonDocument::fromJson(res.toUtf8(), &err);
            if(err.error || !Document.isObject())
            {
                emit PostedToAntigate(tr("Error parsing responce"),id,false,IsRecaptchaV3,IsJsonInterface);
                return;
            }

            QJsonObject Object = Document.object();
            if(
                    Object.contains("errorDescription")
                    && Object.value("errorDescription").isString()
                    && !Object.value("errorDescription").toString().isEmpty()
               )
            {
                emit PostedToAntigate(Object.value("errorDescription").toString(),id,false,IsRecaptchaV3,IsJsonInterface);
            }

            if(!Object.contains("taskId") || !Object.value("taskId").isDouble())
            {
                emit PostedToAntigate(tr("Can't find task id"),id,false,IsRecaptchaV3,IsJsonInterface);
                return;
            }

            QString r = QString::number(Object.value("taskId").toInt());
            emit PostedToAntigate(r,id,true,IsRecaptchaV3,IsJsonInterface);

        }else
        {
            if(!res.startsWith("OK|"))
            {
                emit PostedToAntigate(res,id,false,IsRecaptchaV3,IsJsonInterface);
                return;
            }


            QString r = res.remove(0,3);
            emit PostedToAntigate(r,id,true,IsRecaptchaV3,IsJsonInterface);
        }
    }

    void PostToAntigate::SetHttpClient(IHttpClient * HttpClient)
    {
        this->HttpClient = HttpClient;
    }

    IHttpClient * PostToAntigate::GetHttpClient()
    {
        return HttpClient;
    }
}
