#include "parsecontentdisposition.h"
#include <streambuf>
#include <QRegularExpression>
#include <QUrl>
#include <mimetic/mimetic.h>

namespace BrowserAutomationStudioFramework
{
    QString ContentDispositionToFileName(const QString& Value)
    {
        try
        {
            mimetic::ContentDisposition Header(Value.toStdString());
            QString res = QString::fromStdString(Header.param("filename*"));
            if(!res.isEmpty())
            {
                QRegularExpression re(R"(^[^']*'[^']*'(.*)$)");
                QRegularExpressionMatch match = re.match(res);
                if (match.hasMatch())
                {
                    QString Text = match.captured(1);
                    res = QUrl::fromPercentEncoding(Text.toUtf8());
                    return res;
                }
            }
            return QString::fromStdString(Header.param("filename"));
        }catch(...)
        {
            return QString();
        }

        return QString();
    }
}
