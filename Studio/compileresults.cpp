#include "compileresults.h"
#include <QDesktopServices>
#include <QUrl>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QFile>
#include <QJsonObject>
#include <QSettings>
#include <QXmlStreamReader>
#include <QDomDocument>
#include "preprocessor.h"
#include "noneencryptor.h"

#include "ui_compileresults.h"


CompileResults::CompileResults(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompileResults)
{
    ui->setupUi(this);
    Stage = Initial;
    _HttpClientFactory = new HttpClientFactory(this);
}

void CompileResults::Init(Compiler* _Compiler, const QString& CurrentProject, bool IsOverrideRemote, const QString& PremiumUsername, const QString& PremiumPassword)
{
    this->CurrentProject = CurrentProject;
    this->_Compiler = _Compiler;

    this->PremiumUsername = PremiumUsername;
    this->PremiumPassword = PremiumPassword;

    ui->Login->setText(PremiumUsername);
    ui->Password->setText(PremiumPassword);

    ui->AuthWidget->setVisible(false);
    ui->AuthBorder->setVisible(false);

    QSettings Settings("settings.ini",QSettings::IniFormat);

    ui->RemoteProjectCheckbox->setChecked(Settings.value("CompileRemoteProject",true).toBool());
    ui->LocalProjectCheckbox->setChecked(Settings.value("CompileLocalProject",true).toBool());
    ui->CreateApplicationCheckbox->setChecked(Settings.value("CompileApplication",true).toBool());




    if((ui->Login->text().isEmpty() || ui->Password->text().isEmpty()) && _Compiler->GetType() != Compiler::NoProtection)
    {
        ui->AuthWidget->setVisible(true);
        ui->AuthBorder->setVisible(true);
    }

    ui->AuthValidation->setVisible(false);

    ui->LocalProjectLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    //ui->CreateApplicationLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    connect(ui->CreateApplicationLabel,SIGNAL(linkActivated(QString)),this,SLOT(OpenCompiledProject()));

    ui->LocalProjectLabel->setText(QString(tr("Save setting to current project located in %1 file. It is recommended to use this option.")).arg(CurrentProject));
    if(IsOverrideRemote)
    {
        ui->RemoteProjectLabel->setText(QString(tr("Update project '%1' on server, it requires authentication with premium account, and you must be the script owner.")).arg(_Compiler->GetName()));
    }else
    {
        ui->RemoteProjectLabel->setText(QString(tr("Create new project '%1' on server, it requires authentication with premium account.")).arg(_Compiler->GetName()));
    }

    if(_Compiler->GetType() == Compiler::NoProtection)
    {
        ui->RemoteProjectWidget->setVisible(false);
        ui->RemoteProjectBorder->setVisible(false);
        ui->CreateApplicationLabel->setText(QString(tr("Create standalone application which you can give to other person.")));
    }else
    {
        ui->CreateApplicationLabel->setText(QString(tr("Create standalone application which you can give to other person. If you are just updated your script or even changed BAS version it is not necessary to create application again and send to all users, it is enough just to upload project on server.")));
    }


}

QString CompileResults::GetPremiumUsername()
{
    return PremiumUsername;
}
QString CompileResults::GetPremiumPassword()
{
    return PremiumPassword;
}

void CompileResults::OpenCompiledProject()
{
    QDesktopServices::openUrl(QUrl("file:///" + _Compiler->GetCompiledFolder()));
}

void CompileResults::Disable()
{
    ui->Ok->setEnabled(false);
    ui->Ok->setStyleSheet("*{color:gray}");
}
void CompileResults::Enable()
{
    ui->Ok->setEnabled(true);
    ui->Ok->setStyleSheet("");
}
void CompileResults::Finish()
{
    Enable();
    ui->Ok->setText(tr("Finish"));
    ui->AuthWidget->setVisible(false);
    ui->AuthBorder->setVisible(false);

    Stage = Finished;
}

void CompileResults::FailToConnectToServer()
{
    Enable();
    ui->Ok->setText(tr("Start"));
    Stage = Error;
    ui->AuthWidget->setVisible(true);
    ui->AuthBorder->setVisible(true);
    ui->RemoteProjectLabel->setText(tr("Failed to connect to server."));
    ui->RemoteProjectLabel->setStyleSheet("*{color:red}");
}

void CompileResults::Submit()
{



    if(ui->Login->text().isEmpty() || ui->Password->text().isEmpty())
    {
        ui->AuthValidation->setText(tr("Username or password is empty"));
        ui->AuthWidget->show();
        ui->AuthBorder->show();
        ui->AuthValidation->show();
        Stage = Error;
        Enable();
        return;
    }

    PremiumUsername = ui->Login->text();
    PremiumPassword = ui->Password->text();

    ui->AuthValidation->hide();
    ui->AuthWidget->hide();
    ui->AuthBorder->hide();


    ui->RemoteProjectLabel->setText(tr("Uploading project ..."));
    ui->RemoteProjectLabel->setStyleSheet("*{color:gray}");


    QString UrlString =
            QString("https://bablosoft.com/createorupdatescript") +
            QString("?email=") + QUrl::toPercentEncoding(ui->Login->text()) +
            QString("&name=") + QUrl::toPercentEncoding(_Compiler->GetName()) +
            QString("&pass=") + QUrl::toPercentEncoding(ui->Password->text()) +
            QString("&mode=") + QUrl::toPercentEncoding("4");

    IHttpClient * Client = _HttpClientFactory->GetHttpClient(false);
    Client->setParent(this);
    Client->Connect(this,SLOT(HttpResponce()));

    QHash<QString,ContentData> p;

    QFile File(CurrentProject);
    File.open(QFile::ReadOnly);
    QString DataRaw = File.readAll();
    File.close();

    Preprocessor _Preprocessor;
    _Preprocessor.SetIsRecord(false);
    NoneEncryptor _NoneEncryptor;
    _Preprocessor.SetEncryptor(&_NoneEncryptor);

    int ParanoicLevel = _Compiler->GetProtectionStrength();
    if(ParanoicLevel > 0)
    {
        ParanoicLevel = 13 - ParanoicLevel;
    }

    {
        QDomDocument Document;

        if(Document.setContent(DataRaw, false))
        {
            QDomElement ProjectElement = Document.documentElement();

            //Remove InterfaceState tag
            QDomElement InterfaceStateElement = ProjectElement.firstChildElement("InterfaceState");
            QDomNode InterfaceStateTextElement = InterfaceStateElement.firstChild();
            QDomNode NewInterfaceStateTextElement = Document.createTextNode(QString());
            InterfaceStateElement.replaceChild(NewInterfaceStateTextElement, InterfaceStateTextElement);

            QDomElement ScriptElement = ProjectElement.firstChildElement("Script");
            QDomNode ScriptTextElement = ScriptElement.firstChild();
            QString Script = ScriptTextElement.toText().data();
            Script = _Preprocessor.Preprocess(Script, ParanoicLevel, true);
            Script = _Preprocessor.Encrypt(Script);

            QDomNode NewScriptTextElement = Document.createCDATASection(Script);
            ScriptElement.replaceChild(NewScriptTextElement, ScriptTextElement);
            DataRaw = Document.toString();

        }
    }

    p["data"].DataRaw = DataRaw.toUtf8();

    PostOptions Options;
    Options.Method = "POST";
    Options.PrepareStrategy = "custom";

    Client->Post(UrlString,p,Options);
}

void CompileResults::HttpResponce()
{
    IHttpClient *_HttpClient = qobject_cast<IHttpClient *>(sender());
    if(!_HttpClient)
    {
        FailToConnectToServer();
        return;
    }

    _HttpClient->deleteLater();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromBase64(_HttpClient->GetBase64().toUtf8()), &err);
    if (err.error != QJsonParseError::NoError)
    {
        FailToConnectToServer();
        return;
    }

    if (!doc.isObject())
    {
        FailToConnectToServer();
        return;
    }

    QJsonObject Object = doc.object();

    if(Object["success"].toBool())
    {
        Finish();
        ui->RemoteProjectLabel->setText(QString(tr("<div>Project '%1' was uploaded. Here are some links that may help you tweak it:</div>"
                                                   "<div><a style='color:white' href='https://bablosoft.com/bas/scripts'>List of your scripts</a></div>"
                                                   "<div><a style='color:white' href='https://bablosoft.com/bas/users'>List of your users</a></div>"
                                                   "<div><a style='color:white' href='https://bablosoft.com/bas/interface'>Customize script interface</a></div>"


                                                   )).arg(_Compiler->GetName()));
        ui->RemoteProjectLabel->setStyleSheet("*{color:#0c0}");
    }else
    {
        Enable();
        ui->Ok->setText(tr("Start"));
        Stage = Error;
        ui->AuthWidget->setVisible(true);
        ui->AuthBorder->setVisible(true);
        ui->RemoteProjectLabel->setText(QString(tr("Failed to upload with error: '%1'")).arg(Object["message"].toString()));
        ui->RemoteProjectLabel->setStyleSheet("*{color:red}");
    }
}

CompileResults::~CompileResults()
{
    delete ui;
}

void CompileResults::on_Cancel_clicked()
{
    reject();
}

void CompileResults::on_Ok_clicked()
{
    if(Stage == Initial)
    {
        QSettings Settings("settings.ini",QSettings::IniFormat);

        Settings.setValue("CompileRemoteProject",ui->RemoteProjectCheckbox->isChecked());
        Settings.setValue("CompileLocalProject",ui->LocalProjectCheckbox->isChecked());
        Settings.setValue("CompileApplication",ui->CreateApplicationCheckbox->isChecked());

        Disable();
        if(ui->LocalProjectCheckbox->isChecked())
        {
            emit SaveCurrentProject();
            ui->LocalProjectLabel->setText(QString(tr("Local project located in %1 file was updated.")).arg(CurrentProject));
            ui->LocalProjectLabel->setStyleSheet("*{color:#0c0}");
        }
        ui->LocalProjectCheckbox->setEnabled(false);
        ui->LocalProjectCheckbox->setStyleSheet("*{color:gray}");

        if(ui->CreateApplicationCheckbox->isChecked())
        {
            emit CreateArchive();

            ui->CreateApplicationLabel->setText(QString(tr("<html><head/><body>Standalone application created, it is located in <a style='color:white' href='http://opencompiled.com'>%1</a> folder.</body></html>")).arg(_Compiler->GetCompiledFolder()));
            ui->CreateApplicationLabel->setStyleSheet("*{color:#0c0}");
        }

        ui->CreateApplicationCheckbox->setEnabled(false);
        ui->CreateApplicationCheckbox->setStyleSheet("*{color:gray}");

        ui->RemoteProjectCheckbox->setEnabled(false);
        ui->RemoteProjectCheckbox->setStyleSheet("*{color:gray}");

        if(_Compiler->GetType() != Compiler::NoProtection && ui->RemoteProjectCheckbox->isChecked())
        {
            Submit();
            return;
        }else
        {
            Finish();
            return;
        }
    }
    if(Stage == Error)
    {
        Disable();

        Submit();
        return;
    }
    if(Stage == Finished)
    {
        accept();
        return;
    }
}
