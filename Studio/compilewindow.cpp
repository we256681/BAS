#include "compilewindow.h"
#include "ui_compilewindow.h"
#include <QTimer>
#include <QPixmap>
#include <QBuffer>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include "every_cpp.h"

CompileWindow::CompileWindow(Compiler *compiler,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompileWindow)
{
    ui->setupUi(this);

    ui->NameValidation->setVisible(false);
    this->compiler = compiler;

    if(compiler->GetType() == Compiler::NoProtection)
    {
        ui->NoProtection->setChecked(true);
    }

    if(compiler->GetType() == Compiler::PrivateScriptEnterPassForUser)
    {
        ui->PrivateScriptEnterPassForUser->setChecked(true);
    }

    if(compiler->GetType() == Compiler::PrivateScriptUserEnterPass)
    {
        ui->PrivateScriptUserEnterPass->setChecked(true);
    }

    if(compiler->GetIsEnginesInAppData())
    {
        ui->InstallInAppData->setChecked(true);
    }

    if(compiler->GetCopySilent())
    {
        ui->CopySilent->setChecked(true);
    }

    if(compiler->GetHideBrowsers() && compiler->GetType() != Compiler::NoProtection)
    {
        ui->HideBrowsers->setChecked(true);
    }

    if(compiler->GetIntegrateScheduler() && compiler->GetType() != Compiler::NoProtection)
    {
        ui->IntegrateScheduler->setChecked(true);
    }

    if(compiler->GetSingleInstance() && compiler->GetType() != Compiler::NoProtection)
    {
        ui->SingleInstance->setChecked(true);
    }

    if(!compiler->GetName().isEmpty())
        ui->lineEdit->setText(compiler->GetName());

    ui->ProtectionStrengthSlider->setValue(compiler->GetProtectionStrength());


    UpdateIcon(QByteArray::fromBase64(this->compiler->GetIcon().toUtf8()));
    if(!compiler->GetVersion().isEmpty())
    {
        QStringList list = compiler->GetVersion().split(".");
        if(list.length() == 3)
        {
            ui->spinBox->setValue(list.at(0).toInt());
            ui->spinBox_2->setValue(list.at(1).toInt());
            ui->spinBox_3->setValue(list.at(2).toInt());
        }
    }

    connect(this,SIGNAL(accepted()),this,SLOT(Accepted()));
    connect(ui->label_10,SIGNAL(linkActivated(QString)),this,SLOT(SetDefaultIcon()));
    connect(ui->ProtectionStrengthSlider,SIGNAL(valueChanged(int)),this,SLOT(SliderUpdate()));
    IsDefaultIcon = !this->compiler->GetIsCustomIcon();
    GuiUpdate();
    ui->UesrInterface->setOpenExternalLinks(true);

    TimerValidate = new QTimer(this);
    TimerValidate->setSingleShot(false);
    TimerValidate->setInterval(2000);
    IsValidating = false;
    IsDirty = false;
    Validate();
    connect(TimerValidate, SIGNAL(timeout()), this,SLOT(ValidateTimer()));
    TimerValidate->start();

    ui->label_6->setPixmap(QIcon(":/engine/images/info.png").pixmap(10,10));
    ui->label_9->setPixmap(QIcon(":/engine/images/info.png").pixmap(10,10));
    ui->label_3->setPixmap(QIcon(":/engine/images/info.png").pixmap(10,10));
    ui->label_14->setPixmap(QIcon(":/engine/images/info.png").pixmap(10,10));
    ui->label_15->setPixmap(QIcon(":/engine/images/info.png").pixmap(10,10));
    ui->label_16->setPixmap(QIcon(":/engine/images/info.png").pixmap(10,10));
    ui->label_17->setPixmap(QIcon(":/engine/images/info.png").pixmap(10,10));
    ui->label_18->setPixmap(QIcon(":/engine/images/info.png").pixmap(10,10));


}

bool CompileWindow::GetIsOverrideRemote()
{
    return IsOverrideRemote;
}

void CompileWindow::SetDefaultIcon()
{
    QFile f(":/studio/images/LogoMedium.png");
    if(f.open(QFile::ReadOnly))
    {
        UpdateIcon(f.readAll());
        IsDefaultIcon = true;
    }
    f.close();
}

void CompileWindow::UpdateIcon(const QByteArray& Data)
{
    QPixmap pixmap;
    if(pixmap.loadFromData(Data))
    {
        if(pixmap.width() > pixmap.height())
        {
            pixmap = pixmap.copy(0,0,pixmap.height(),pixmap.height());
        }
        if(pixmap.height() > pixmap.width())
        {
            pixmap = pixmap.copy(0,0,pixmap.width(),pixmap.width());
        }
        if(pixmap.width() > 128)
        {
            pixmap = pixmap.scaledToWidth(128,Qt::SmoothTransformation);
        }else if(pixmap.width() > 64 && pixmap.width() < 128)
        {
            pixmap = pixmap.scaledToWidth(64,Qt::SmoothTransformation);
        }else if(pixmap.width() > 48 && pixmap.width() < 64)
        {
            pixmap = pixmap.scaledToWidth(48,Qt::SmoothTransformation);
        }else if(pixmap.width() > 32 && pixmap.width() < 48)
        {
            pixmap = pixmap.scaledToWidth(32,Qt::SmoothTransformation);
        }else if(pixmap.width() > 16 && pixmap.width() < 32)
        {
            pixmap = pixmap.scaledToWidth(16,Qt::SmoothTransformation);
        }else if(pixmap.width() < 16)
            return;
        ui->ApplicationIcon->setIcon(QIcon(pixmap));

        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);

        pixmap.save(&buffer, "PNG", 100);
        buffer.close();
        IsDefaultIcon = false;
        LastIcon = ba;
    }
}

void CompileWindow::SliderUpdate()
{
    int Value = ui->ProtectionStrengthSlider->value();
    if(Value == 0)
    {
        ui->ProtectionStrengthLabel->setText(tr("No protection"));
    }else if(Value < 4)
    {
        ui->ProtectionStrengthLabel->setText(tr("Low"));
    }else if(Value < 7)
    {
        ui->ProtectionStrengthLabel->setText(tr("Medium"));
    }else if(Value < 10)
    {
        ui->ProtectionStrengthLabel->setText(tr("High"));
    }else
    {
        ui->ProtectionStrengthLabel->setText(tr("Ultra"));
    }
}

void CompileWindow::GuiUpdate()
{
    SliderUpdate();


    bool Visible = ui->PrivateScriptEnterPassForUser->isChecked();
    /*ui->UserName->setVisible(Visible);
    ui->Password->setVisible(Visible);
    ui->label_7->setVisible(Visible);
    ui->label_8->setVisible(Visible);*/

    ui->UserCredentials->setVisible(Visible);

    ui->InstallInAppData->setVisible(true);
    ui->CopySilent->setVisible(true);

    QString UserInterfaceText;


    if(ui->NoProtection->isChecked())
    {
        ui->HideBrowsers->setEnabled(false);
        ui->HideBrowsers->setChecked(false);
        ui->HideBrowsers->setStyleSheet("*{color:gray}");

        ui->IntegrateScheduler->setEnabled(false);
        ui->IntegrateScheduler->setChecked(false);
        ui->IntegrateScheduler->setStyleSheet("*{color:gray}");

        ui->SingleInstance->setEnabled(false);
        ui->SingleInstance->setChecked(false);
        ui->SingleInstance->setStyleSheet("*{color:gray}");


        ui->ApplicationIcon->setEnabled(false);
        ui->label_10->setText(tr("<html><head/><body><div>Only for premium users and for private scripts. </div></body></html>"));
        ui->label_10->setStyleSheet("*{color:gray}");

        UserInterfaceText = QString("<div>%1</div>")
                        .arg(tr("Custom interface is available only to premium users and private scripts."));

        ui->NameContainer_4->setVisible(false);
        ui->widget_9->setVisible(false);



    }else
    {
        ui->HideBrowsers->setEnabled(true);
        ui->HideBrowsers->setStyleSheet("*{color:white}");
        ui->IntegrateScheduler->setEnabled(true);
        ui->IntegrateScheduler->setStyleSheet("*{color:white}");
        ui->SingleInstance->setEnabled(true);
        ui->SingleInstance->setStyleSheet("*{color:white}");
        ui->ApplicationIcon->setEnabled(true);
        ui->label_10->setStyleSheet("*{color:white}");

        ui->label_10->setVisible(true);
        ui->label_10->setText(tr("<html><head/><body><div>Click on icon to change it.</div><div>Only for premium users and for private scripts. </div><div><a href=\"restore://defaults\"><span style=\" text-decoration: underline; color:#ffffff;\">Restore default icon.</span></a></div></body></html>"));

        UserInterfaceText = QString("<div>%1 <a style='color:white' href='https://bablosoft.com/bas/interface'>%2</a> %3.</div>")
                        .arg(tr("Visit"))
                        .arg(tr("this page"))
                        .arg(tr("in order to modify interface of your script"));

        ui->NameContainer_4->setVisible(true);
        ui->widget_9->setVisible(true);

    }




    UserInterfaceText +=

            QString("<div style='color:gray;margin-top:10px'>%1 <a style='color:gray' href='https://bablosoft.com/interface-builder/index.html'>%2</a> %3. %4 <a style='color:gray' href='https://bablosoft.com/test-interface/index.html'>%5</a>.</div>")
            .arg(tr("See free"))
            .arg(tr("demo"))
            .arg(tr("to test how it works"))
            .arg(tr("Final result may look like"))
            .arg(tr("this"));

    ui->UesrInterface->setText(UserInterfaceText);

    Validate();
    QTimer::singleShot(0,this,SLOT(Resize()));

}

void CompileWindow::ValidateTimer()
{
    if(IsDirty)
    {
        IsDirty = false;
        Validate();
    }
}

void CompileWindow::Validate()
{
    IsOverrideRemote = false;

    QString ScriptName = ui->lineEdit->text();
    QString Validation = tr("Validating name ...");
    bool IsValid = true;
    if(ui->NoProtection->isChecked())
    {
        if(ScriptName == "Script Name" || ScriptName == "ScriptName" || ScriptName.isEmpty())
        {
            Validation = tr("You are using default script name. It is recommended to change it to something meaningful.");
        }else
        {
            Validation.clear();
        }
    }else
    {
        QRegularExpression re("^[a-zA-Z][a-zA-Z0-9]*$");
        QRegularExpressionMatch match = re.match(ScriptName);
        if (match.hasMatch())
        {
            if(IsValidating)
            {
                IsDirty = true;
            }else
            {
                HttpClient *_HttpClient = new HttpClient(this);
                _HttpClient->Connect(this,SLOT(ValidateResult()));
                _HttpClient->Get(QString("https://bablosoft.com/scriptmeta/") + ScriptName);
                IsValidating = true;
            }
        }else
        {
            Validation = tr("Script name only allows letters, first char should be letter.");
            IsValid = false;
        }
    }

    if(Validation.isEmpty())
    {
        ui->NameValidation->hide();
    }else
    {
        ui->NameValidation->show();
        ui->NameValidation->setText(Validation);
    }

    if(IsValid)
    {
        ui->NameValidation->setStyleSheet("*{color:gray}");
        ui->Ok->setStyleSheet("");
        ui->Ok->setEnabled(true);
    }else
    {
        ui->NameValidation->setStyleSheet("*{color:red}");
        ui->Ok->setStyleSheet("*{color:gray}");
        ui->Ok->setEnabled(false);
    }

}

void CompileWindow::ValidateResult()
{
    IsValidating = false;
    HttpClient *_HttpClient = qobject_cast<HttpClient *>(sender());
    if(!_HttpClient)
        return;

    _HttpClient->deleteLater();

    if(IsDirty)
        return;

    QString ScriptName = ui->lineEdit->text();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromBase64(_HttpClient->GetBase64().toUtf8()), &err);
    if (err.error != QJsonParseError::NoError)
        return;

    if (!doc.isObject())
        return;

    QJsonObject Object = doc.object();

    if(Object["name"].toString() == ScriptName)
    {
        if(Object["found"].toInt() == 1)
        {
            ui->NameValidation->show();
            ui->NameValidation->setText(tr("Script already exists, if you are a script owner, you can use it, otherwise you must change it."));
            IsOverrideRemote = true;
        }else
        {
            ui->NameValidation->hide();
        }
    }

}


void CompileWindow::Resize()
{
    //QSize Size = sizeHint();
    //resize(width(),Size.height() + 10);

    resize(800,625);
}

void CompileWindow::Accepted()
{
    compiler->SetUsername(ui->UserName->text());
    compiler->SetPassword(ui->Password->text());
    compiler->SetCopySilent(ui->CopySilent->isChecked());
    if(ui->NoProtection->isChecked())
    {
        compiler->SetType(Compiler::NoProtection);
    }else if(ui->PrivateScriptEnterPassForUser->isChecked())
    {
        compiler->SetType(Compiler::PrivateScriptEnterPassForUser);
    }else if(ui->PrivateScriptUserEnterPass->isChecked())
    {
        compiler->SetType(Compiler::PrivateScriptUserEnterPass);
    }
    compiler->SetName(ui->lineEdit->text());
    compiler->SetProtectionStrength(ui->ProtectionStrengthSlider->value());
    compiler->SetIcon(QString::fromUtf8(LastIcon.toBase64()));
    compiler->SetIsCustomIcon(!IsDefaultIcon);
    compiler->SetHideBrowsers(ui->HideBrowsers->isChecked());
    compiler->SetIntegrateScheduler(ui->IntegrateScheduler->isChecked());
    compiler->SetSingleInstance(ui->SingleInstance->isChecked());
    compiler->SetIsEnginesInAppData(ui->InstallInAppData->isChecked());
    compiler->SetVersion(QString::number(ui->spinBox->value()) + "." + QString::number(ui->spinBox_2->value()) + "."+ QString::number(ui->spinBox_3->value()));
    compiler->SetOs("Windows");
}

CompileWindow::~CompileWindow()
{
    delete ui;
}

void CompileWindow::on_NoProtection_clicked()
{
    GuiUpdate();
}

void CompileWindow::on_PrivateScriptUserEnterPass_clicked()
{
    GuiUpdate();
}

void CompileWindow::on_PrivateScriptEnterPassForUser_clicked()
{
    GuiUpdate();
}

void CompileWindow::on_ApplicationIcon_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), "", tr("All Files (*.*)"));

    if(fileName.length()>0)
    {
        QFile f(fileName);
        if(f.open(QFile::ReadOnly))
        {
            UpdateIcon(f.readAll());
        }
        f.close();
    }
}

void CompileWindow::on_lineEdit_textChanged(const QString &arg1)
{
    Validate();
}

void CompileWindow::on_Ok_clicked()
{
    accept();
}

void CompileWindow::on_Cancel_clicked()
{
    reject();
}
