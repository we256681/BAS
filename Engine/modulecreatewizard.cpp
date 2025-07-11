#include "modulecreatewizard.h"
#include <QAbstractButton>
#include <QCheckBox>
#include <QBuffer>
#include <QFileDialog>
#include "ui_modulecreatewizard.h"

ModuleCreateWizard::ModuleCreateWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::ModuleCreateWizard)
{
    ui->setupUi(this);

    connect(this,SIGNAL(currentIdChanged(int)),this,SLOT(PageChanged(int)));

    connect(this->button(QWizard::CustomButton1),SIGNAL(clicked(bool)),this,SLOT(accept()));

    ui->ValidationFunctions->setVisible(false);
    ui->ValidationNameWrongCharacters->setVisible(false);

    this->button(QWizard::CustomButton1)->setVisible(false);
    this->setButtonText(QWizard::CustomButton1,tr("Skip"));

    ui->ValidationName->setVisible(false);
    ui->ValidationDescription->setVisible(false);

    connect(ui->label_15,SIGNAL(linkActivated(QString)),this,SLOT(SetDefaultIcon()));


    //QSizePolicy SizePolicyAdvanced = ui->AdvancedContainer->sizePolicy();
    //SizePolicyAdvanced.setRetainSizeWhenHidden(true);
    //ui->AdvancedContainer->setSizePolicy(SizePolicyAdvanced);

    ui->AdvancedContainer->setVisible(false);
    IsAdvancedVisible = false;
    CurrentPageId = 0;
    IsDefaultIcon = false;
}


void ModuleCreateWizard::CheckSkipAvailable()
{
    this->button(QWizard::CustomButton1)->setVisible(false);
    if(ui->Name->text().isEmpty())
    {
        return;
    }

    QRegularExpression re("^[0-9a-zA-Z_]*$");
    QRegularExpressionMatch match = re.match(ui->Name->text());

    if(!match.hasMatch())
    {
        return;
    }

    if(ui->ShortDescriptionEn->text().isEmpty())
    {
        return;
    }

    bool IsValid = false;
    for(QCheckBox * Check:ui->FunctionScrollContent->findChildren<QCheckBox *>())
    {
        if(Check->property("name").isValid())
        {
            if(Check->isChecked())
            {
                IsValid = true;
                break;
            }
        }
    }
    if(IsValid)
    {
        this->button(QWizard::CustomButton1)->setVisible(true);
    }
}

void ModuleCreateWizard::SetModuleManager(IModuleManager *_ModuleManager)
{
    this->_ModuleManager = _ModuleManager;
}

void ModuleCreateWizard::SetModuleCreator(IModuleCreator *_ModuleCreator)
{
    this->_ModuleCreator = _ModuleCreator;
}

void ModuleCreateWizard::SaveData()
{
    QStringList ExcludedFunctions;
    for(QCheckBox * Check:ui->FunctionScrollContent->findChildren<QCheckBox *>())
    {
        if(Check->property("name").isValid())
        {
            QString Name = Check->property("name").toString();
            if(!Check->isChecked())
            {
                ExcludedFunctions.append(Name);
            }
        }
    }

    _ModuleCreator->SetProperty("ExcludedFunctions", ExcludedFunctions.join(","));

    _ModuleCreator->SetProperty("Name", ui->Name->text());
    _ModuleCreator->SetProperty("ShortDescriptionEn", ui->ShortDescriptionEn->text());
    _ModuleCreator->SetProperty("ShortDescriptionRu", ui->ShortDescriptionRu->text());

    _ModuleCreator->SetProperty("VersionMajor", QString::number(ui->VersionMajor->value()));
    _ModuleCreator->SetProperty("VersionMinor", QString::number(ui->VersionMinor->value()));


    _ModuleCreator->SetProperty("DetailedDescriptionEn", ui->DetailedDescriptionEn->toPlainText());
    _ModuleCreator->SetProperty("DetailedDescriptionRu", ui->DetailedDescriptionRu->toPlainText());
    _ModuleCreator->SetProperty("DeveloperName", ui->DeveloperName->text());
    _ModuleCreator->SetProperty("DeveloperMail", ui->DeveloperMail->text());
    _ModuleCreator->SetProperty("DeveloperSite", ui->DeveloperSite->text());

    _ModuleCreator->SetProperty("RandomizeFunctionName", ui->RandomizeFunctionName->isChecked() ? "true" : "false");
    _ModuleCreator->SetProperty("InstallModule", ui->InstallModule->isChecked() ? "true" : "false");

    _ModuleCreator->SetProperty("IsDefaultIcon", IsDefaultIcon ? "true" : "false");
    _ModuleCreator->SetProperty("IconData", QString::fromUtf8(LastIcon.toBase64()));


}


void ModuleCreateWizard::Init()
{
    CheckSkipAvailable();


    QStringList FunctionNames = _ModuleCreator->GetFunctionList();
    QStringList ExcludedFunctions = _ModuleCreator->GetProperty("ExcludedFunctions").split(",");

    for(QString& Name:FunctionNames)
    {
        QCheckBox *Сheck = new QCheckBox(ui->FunctionScrollContent);
        Сheck->setText(Name);
        Сheck->setProperty("name",Name);
        Сheck->setChecked(!ExcludedFunctions.contains(Name));
        connect(Сheck,SIGNAL(toggled(bool)),this,SLOT(CheckSkipAvailable()));
        ui->FunctionScrollContent->layout()->addWidget(Сheck);
    }

    ui->NoFunctionsLabel->setVisible(FunctionNames.isEmpty());
    ui->FunctionListScroll->setVisible(!FunctionNames.isEmpty());

    ui->Name->setText(_ModuleCreator->GetProperty("Name"));
    ui->ShortDescriptionEn->setText(_ModuleCreator->GetProperty("ShortDescriptionEn"));
    ui->ShortDescriptionRu->setText(_ModuleCreator->GetProperty("ShortDescriptionRu"));

    QString VersionMajorString = _ModuleCreator->GetProperty("VersionMajor");
    if(!VersionMajorString.isEmpty())
        ui->VersionMajor->setValue(VersionMajorString.toInt());

    QString VersionMinorString = _ModuleCreator->GetProperty("VersionMinor");
    if(!VersionMinorString.isEmpty())
        ui->VersionMinor->setValue(VersionMinorString.toInt());

    ui->DetailedDescriptionEn->setPlainText(_ModuleCreator->GetProperty("DetailedDescriptionEn"));
    ui->DetailedDescriptionRu->setPlainText(_ModuleCreator->GetProperty("DetailedDescriptionRu"));
    ui->DeveloperName->setText(_ModuleCreator->GetProperty("DeveloperName"));
    ui->DeveloperMail->setText(_ModuleCreator->GetProperty("DeveloperMail"));
    ui->DeveloperSite->setText(_ModuleCreator->GetProperty("DeveloperSite"));

    QString RandomizeFunctionName = _ModuleCreator->GetProperty("RandomizeFunctionName");
    ui->RandomizeFunctionName->setChecked(RandomizeFunctionName == "true" || RandomizeFunctionName.isEmpty());

    QString InstallModuleString = _ModuleCreator->GetProperty("InstallModule");
    ui->InstallModule->setChecked(InstallModuleString == "true" || InstallModuleString.isEmpty());

    IsDefaultIcon = true;
    if(_ModuleCreator->GetProperty("IsDefaultIcon") == "false")
    {
        IsDefaultIcon = false;
    }

    if(!IsDefaultIcon)
    {
        QByteArray Data = QByteArray::fromBase64(_ModuleCreator->GetProperty("IconData").toUtf8());
        UpdateIcon(Data);
    }else
    {
        SetDefaultIcon();
    }

}

void ModuleCreateWizard::UpdateIcon(const QByteArray& Data)
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
        if(pixmap.width() != 32)
        {
            pixmap = pixmap.scaledToWidth(32,Qt::SmoothTransformation);
        }

        ui->ApplicationIcon->setIcon(QIcon(pixmap));

        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);

        pixmap.save(&buffer, "PNG", 100);
        buffer.close();

        IsDefaultIcon = false;
        LastIcon = ba;
        return;
    }
    SetDefaultIcon();
}

void ModuleCreateWizard::SetDefaultIcon()
{
    IsDefaultIcon = true;
    LastIcon.clear();
    QByteArray Data;
    QFile f(":/engine/images/modules.png");
    if(f.open(QFile::ReadOnly))
    {
        Data = f.readAll();
    }
    f.close();

    QPixmap pixmap;
    if(pixmap.loadFromData(Data))
    {
        ui->ApplicationIcon->setIcon(QIcon(pixmap));
    }
}

void ModuleCreateWizard::PageChanged(int PageId)
{
    bool IsValid = true;
    if(CurrentPageId == 1 && PageId == 2)
    {
        ui->ValidationDescription->setVisible(false);
        ui->ValidationName->setVisible(false);
        ui->ValidationNameWrongCharacters->setVisible(false);
        QString Name = ui->Name->text();
        if(Name.isEmpty())
        {
            IsValid = false;
            ui->ValidationName->setVisible(true);
        }else
        {
            QRegularExpression re("^[0-9a-zA-Z_]*$");
            QRegularExpressionMatch match = re.match(Name);

            if(!match.hasMatch())
            {
                IsValid = false;
                ui->ValidationNameWrongCharacters->setVisible(true);
            }
        }


        if(ui->ShortDescriptionEn->text().isEmpty())
        {
            IsValid = false;
            ui->ValidationDescription->setVisible(true);
        }
    }

    if(CurrentPageId == 0 && PageId == 1)
    {
        ui->ValidationFunctions->setVisible(false);
        IsValid = false;
        for(QCheckBox * Check:ui->FunctionScrollContent->findChildren<QCheckBox *>())
        {
            if(Check->property("name").isValid())
            {
                if(Check->isChecked())
                {
                    IsValid = true;
                    break;
                }
            }
        }

        if(!IsValid)
        {
            ui->ValidationFunctions->setVisible(true);
        }

    }

    CurrentPageId = PageId;

    if(!IsValid)
        back();
}

ModuleCreateWizard::~ModuleCreateWizard()
{
    delete ui;
}

void ModuleCreateWizard::on_ShowMore_clicked()
{
    IsAdvancedVisible = !IsAdvancedVisible;
    ui->AdvancedContainer->setVisible(IsAdvancedVisible);
    if(IsAdvancedVisible)
    {
        ui->ShowMore->setText(tr("Show less"));
        ui->ShowMore->setIcon(QIcon(":/engine/images/up.png"));
    }else
    {
        ui->ShowMore->setText(tr("Show more"));
        ui->ShowMore->setIcon(QIcon(":/engine/images/down.png"));
    }


}

void ModuleCreateWizard::on_Name_textChanged(const QString &arg1)
{
    CheckSkipAvailable();
}

void ModuleCreateWizard::on_ShortDescriptionEn_textChanged(const QString &arg1)
{
    CheckSkipAvailable();
}




void ModuleCreateWizard::on_ApplicationIcon_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Icon"), "", tr("All Files (*.*)"));

    if(fileName.length()>0)
    {
        QByteArray Data;
        QFile f(fileName);
        if(f.open(QFile::ReadOnly))
        {
            Data = f.readAll();
        }
        f.close();

        UpdateIcon(Data);
    }
}
