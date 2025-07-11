#include "postpreparestrategycustomencode.h"
#include <QRegularExpression>
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    PostPrepareStrategyCustomEncode::PostPrepareStrategyCustomEncode()
    {

    }

    void PostPrepareStrategyCustomEncode::ParseBoundary(const QString& Data)
    {
        QRegularExpression re("^\\s*\\-\\-([^\\s]+)");
        re.setPatternOptions(QRegularExpression::MultilineOption);
        QRegularExpressionMatch match = re.match(Data);
        if(match.hasMatch())
        {
            Boundary = match.capturedTexts()[1];
        }
    }

    QByteArray PostPrepareStrategyCustomEncode::ConvertBase64(const QString& Data)
    {
        QByteArray Res;
        QRegularExpression re("_browserautomationstudio_base64\\:([0-9a-zA-Z\\/\\+\\=]+)");
        int Offset = 0;
        re.setPatternOptions(QRegularExpression::MultilineOption);
        while(true)
        {
            QRegularExpressionMatch match = re.match(Data,Offset);
            if(match.hasMatch())
            {
                Res.append(Data.mid(Offset,match.capturedStart() - Offset).toUtf8());
                Res.append(QByteArray::fromBase64(match.capturedTexts()[1].toUtf8()));
                Offset = match.capturedEnd();
            }
            else
            {
                if(!Res.isEmpty())
                {
                    Res.append(Data.mid(Offset).toUtf8());
                }
                break;
            }
        }
        return Res;
    }

    QByteArray PostPrepareStrategyCustomEncode::GenerateData(const QHash<QString,ContentData> & params)
    {
        QByteArray data;
        if(params.contains("data"))
        {
            ContentData d = params["data"];
            if(!d.DataString.isEmpty())
            {

                if(ContentType == "multipart/form-data")
                {
                    ParseBoundary(d.DataString);
                }

                QByteArray Data = ConvertBase64(d.DataString);
                if(Data.isEmpty())
                {
                    Data = d.DataString.toUtf8();
                }

                data.append(Data);
            }
            else if(!d.DataRaw.isEmpty())
            {
                if(ContentType == "multipart/form-data")
                {
                    ParseBoundary(QString::fromUtf8(d.DataRaw));
                }
                data.append(d.DataRaw);
            }
        }
        return data;
    }


    void PostPrepareStrategyCustomEncode::SetContentType(const QString& ContentType)
    {
        QString NewValue = ContentType;
        NewValue = NewValue.replace("custom/","");
        this->ContentType = NewValue;
    }
    QByteArray PostPrepareStrategyCustomEncode::GetContentType()
    {
        if(ContentType == "multipart/form-data")
        {
            return (ContentType + QString("; boundary=") + Boundary).toUtf8();
        }else
            return ContentType.toUtf8();
    }

}
