#include "webinterfacesolver.h"
#include "every_cpp.h"

WebInterfaceSolver::WebInterfaceSolver(QObject *parent) :
    ISolver(parent)
{
    WebInterface = 0;
    IdCounter = 0;
}

void WebInterfaceSolver::SetWebIntrface(IWebInterface *WebInterface)
{
    this->WebInterface = WebInterface;
    connect(WebInterface,SIGNAL(ReceivedMessage(QString,QVariantMap,bool,int)),this,SLOT(ReceivedMessage(QString,QVariantMap,bool,int)));
}

void WebInterfaceSolver::ReceivedMessage(QString Type, QVariantMap Data, bool IsAsync, int Id)
{
    //this.Send("captcha_solved",{text: Text,id: parseInt(CaptchaId),success: IsSuccess})

    if(Type == "captcha_solved")
    {
        QString id = QString::number(Data["id"].toInt());
        QString text = Data["text"].toString();
        bool success = Data["success"].toBool();

        emit Done(text,id,success,"0");
    }

}

bool WebInterfaceSolver::TimeLimited()
{
    return false;
}
QString WebInterfaceSolver::Solve(const QString& base64,const QStringList& props)
{
    if(base64.startsWith("BAS_USER_ASK"))
    {
        QString data = base64;
        data = data.remove(0,12);
        WebInterface->Send("captcha",false,0,"id",IdCounter,"is_image",false,"data",data);
    }else
    {
        WebInterface->Send("captcha",false,0,"id",IdCounter,"is_image",true,"data",base64);
    }

    return QString::number(IdCounter++);
}
void WebInterfaceSolver::ReportBad(const QString& id)
{

}
