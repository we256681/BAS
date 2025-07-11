#include "resourcewizard.h"
#include "ui_resourcewizard.h"
#include <QAbstractButton>
#include <QDebug>
#include <QMessageBox>
#include <QCompleter>
#include "classcomboboxfillpictures.h"
#include "multiselect.h"

ResourceWizard::ResourceWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::ResourceWizard)
{
    ui->setupUi(this);
    this->setButtonText(QWizard::CustomButton1,tr("Skip"));
    this->button(QWizard::CustomButton1)->setVisible(false);

    connect(button(QWizard::CustomButton1),SIGNAL(clicked()),this,SLOT(accept()));
    connect(this,SIGNAL(currentIdChanged(int)),this,SLOT(PageChanged(int)));
    ClassComboBoxFillPictures Pictures;
    ui->FixedString->setIcon(Pictures.GetIconFor(0));
    ui->FixedInteger->setIcon(Pictures.GetIconFor(1));
    ui->RandomString->setIcon(Pictures.GetIconFor(2));
    ui->RandomInteger->setIcon(Pictures.GetIconFor(3));
    ui->Select->setIcon(Pictures.GetIconFor(4));
    ui->LinesFromFile->setIcon(Pictures.GetIconFor(5));
    ui->LinesFromUrl->setIcon(Pictures.GetIconFor(6));
    ui->FilesFromDirectory->setIcon(Pictures.GetIconFor(7));
    ui->Database->setIcon(Pictures.GetIconFor(8));

    HideAllTypesLabels();
    ui->FixedStringLabel->setVisible(true);
    Type = "FixedString";
    CurrentPageId = -1;
    ui->StartStringBuilder->setIcon(QIcon(":/engine/images/edit.png"));
    ui->SelectDefaultValueContainer->layout()->addWidget(new MultiSelect(ui->SelectDefaultValueContainer));
    ui->SelectDefaultValueContainer->layout()->setAlignment(Qt::AlignTop);
    UpdateSelect();
    IsAdditionalIf = false;
    IsAdditionalTab = false;

    disconnect( button( QWizard::CancelButton ), SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( button( QWizard::CancelButton ), SIGNAL( clicked() ), this, SLOT( askReject() ) );

    setButtonText(QWizard::CancelButton, tr("Cancel"));
    setButtonText(QWizard::NextButton, tr(" Next >"));
    setButtonText(QWizard::BackButton, tr("< Back "));

    ui->TabEn->installEventFilter(this);
    ui->TabRu->installEventFilter(this);
    ui->VisibilityName->installEventFilter(this);
    ui->VisibilityValue->installEventFilter(this);


    TooltipUserFocus = false;
    ShowTooltip = false;
    TooltipUserFocus2 = false;
    ShowTooltip2 = false;
    TooltipUserFocus3 = false;
    ShowTooltip3 = false;
    TooltipUserFocus4 = false;
    ShowTooltip4 = false;

    ui->ToTab->setVisible(false);
    ui->label_26->setVisible(false);
    ui->label_28->setVisible(false);
}

void ResourceWizard::askReject()
{
    if(CurrentPageId>=1)
    {
        QMessageBox msgBox;
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Are you sure, all changes will be lost?"));
        if(msgBox.exec() != QMessageBox::Ok)
        {
            return;
        }
    }
    reject();
}

void ResourceWizard::SetStringBuilder(IStringBuilder * StringBuilder)
{
    this->StringBuilder = StringBuilder;
}
void ResourceWizard::SetGroups(const QStringList& Groups)
{
    this->Groups = Groups;
}
void ResourceWizard::SetSelectsVariablesAndValues(const QHash<QString,QString>& VariablesAndValues)
{
    this->VariablesAndValues = VariablesAndValues;
}

void ResourceWizard::HideAllTypesLabels()
{
    ui->FixedStringLabel->setVisible(false);
    ui->FixedIntegerLabel->setVisible(false);
    ui->RandomStringLabel->setVisible(false);
    ui->RandomIntegerLabel->setVisible(false);
    ui->SelectLabel->setVisible(false);
    ui->LinesFromFileLabel->setVisible(false);
    ui->LinesFromUrlLabel->setVisible(false);
    ui->FilesFromDirectoryLabel->setVisible(false);
    ui->DatabaseLabel->setVisible(false);
}

void ResourceWizard::PageChanged(int PageId)
{
    //qDebug()<<"start"<<"PageId"<<PageId<<"CurrentPageId"<<CurrentPageId;
    if(CurrentPageId == 0)
    {
        MultiLanguageString str;
        str.SetTranslation("en",ui->DescriptionEn->text());
        str.SetTranslation("ru",ui->DescripnionRu->text());

        ResourceWidget->SetVariableName(ui->Name->text());
        ResourceWidget->SetDescription(str);
    }else if(CurrentPageId == 1)
    {
        ResourceWidget->SetTypeId(Type);
        this->button(QWizard::CustomButton1)->setVisible(true);


    }else if(CurrentPageId == 2)
    {
        if(Type == "FixedString")
        {
            if(ResourceWidget->Get("Advanced.ContainsCheckBox.NotEmptyCheckBox").toBool())
            {
                ResourceWidget->Set("Advanced.CheckBoxChecked.NotEmptyCheckBox",QVariant(ui->StringNotEmpty->isChecked()));
            }
            ResourceWidget->Set("User.LineEditText.FixedStringValue", QVariant(ui->StringDefaultValue->text()));
        }
    }else if(CurrentPageId == 10)
    {
        if(IsAdditionalTab)
        {
            IsAdditionalTab = false;
            MultiLanguageString str;
            str.SetTranslation("en",ui->TabEn->text());
            str.SetTranslation("ru",ui->TabRu->text());
            ResourceWidget->SetSectionName(str);
        }
    }else if(CurrentPageId == 9)
    {
        if(IsAdditionalIf)
        {
            IsAdditionalIf = false;
            ResourceWidget->SetVisibilityConditionValue(ui->VisibilityValue->text());
            ResourceWidget->SetVisibilityConditionVariable(ui->VisibilityName->text());

        }
    }else if(CurrentPageId == 8)
    {
        if(Type == "Select")
        {
            QStringList list = ui->SelectLines->toPlainText().split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

            MultiSelect *multi = ui->SelectDefaultValueContainer->findChild<MultiSelect *>();

            if(ResourceWidget->Get("Advanced.ContainsCombobox.SelectTypeCombo").toBool())
            {
                ResourceWidget->Set("Advanced.ComboboxIndex.SelectTypeCombo",QVariant(ui->SelectType->currentIndex()));
                ResourceWidget->Set("Advanced.PlainTextEditText.SelectValuesEdit",QVariant(ui->SelectLines->toPlainText()));
            }
            ResourceWidget->Set("User.MultiselectType", QVariant(ui->SelectType->currentText()));
            ResourceWidget->Set("User.MultiselectValues", QVariant(list));
            QStringList SelectedList;
            for(int Selected:multi->GetSelectedIndex())
            {
                SelectedList.append(QString::number(Selected));
            }
            ResourceWidget->Set("User.MultiselectIndexes", QVariant(SelectedList));


        }
    }else if(CurrentPageId == 7)
    {
        if(Type == "RandomString")
        {
            ResourceWidget->Set("User.LineEditText.RandomStringValue",QVariant(ui->RandomStringValue->text()));
        }
    }else if(CurrentPageId == 4)
    {
        if(Type == "FixedInteger")
        {
            if(ResourceWidget->Get("Advanced.ContainsSpinBox.EditMinimum").toBool())
            {
                ResourceWidget->Set("Advanced.SpinBoxValue.EditMinimum",QVariant(ui->MinValueNumber->value()));
            }
            if(ResourceWidget->Get("Advanced.ContainsSpinBox.EditMaximum").toBool())
            {
                ResourceWidget->Set("Advanced.SpinBoxValue.EditMaximum",QVariant(ui->MaxValueNumber->value()));
            }
            ResourceWidget->Set("User.SpinBoxValue.FixedIntegerValue",QVariant(ui->Number->value()));
        }

    }else if(CurrentPageId == 3)
    {

        if(Type == "RandomInteger")
        {
            ResourceWidget->Set("User.SpinBoxValue.RIMinimumValue", QVariant(ui->NumberMin->value()));
            ResourceWidget->Set("User.SpinBoxValue.RIMaximumValue", QVariant(ui->NumberMax->value()));

            if(ResourceWidget->Get("Advanced.ContainsSpinBox.EditMinimum_2").toBool())
            {
                ResourceWidget->Set("Advanced.SpinBoxValue.EditMinimum_2", QVariant(ui->MinValueRange->value()));
                ResourceWidget->Set("Advanced.SpinBoxValue.EditMaximum_2", QVariant(ui->MaxValueRange->value()));
            }
        }
    }else if(CurrentPageId == 5)
    {
        if(Type == "LinesFromFile")
        {
            ResourceWidget->Set("User.CheckBoxChecked.FileWrite", QVariant(!ui->OnlyRead->isChecked()));
            ResourceWidget->Set("User.CheckBoxChecked.FileRead", QVariant(!ui->OnlyWrite->isChecked()));
            ResourceWidget->Set("User.CheckBoxChecked.FileMix", QVariant(ui->MixLines->isChecked()));
        }
        if(Type == "FilesFromDirectory")
        {
            ResourceWidget->Set("User.CheckBoxChecked.DirectoryClean", QVariant(!ui->OnlyRead->isChecked()));
            ResourceWidget->Set("User.CheckBoxChecked.DirectoryMix", QVariant(ui->MixLines->isChecked()));
        }

        if(Type == "Database")
        {
            ResourceWidget->Set("User.CheckBoxChecked.DatabaseClear", QVariant(!ui->OnlyRead->isChecked()));
            ResourceWidget->Set("User.CheckBoxChecked.DatabaseMix", QVariant(ui->MixLines->isChecked()));
        }
    }else if(CurrentPageId == 6)
    {
        int Success = 0;
        int Fails = 0;
        if(ui->EachLineOneTime->isChecked())
        {
            Success = 1;
            Fails = 1;
        }else if(ui->EachLineSeveralTimes->isChecked())
        {
            Success = 10;
            Fails = 10;
        }else if(ui->EachLineAnyNumberOfTimes->isChecked())
        {
            Success = 100000;
            Fails = 100000;
        }

        if(Type == "LinesFromFile")
        {
            ResourceWidget->Set("User.SpinBoxValue.FileMaxSuccess", QVariant(Success));
            ResourceWidget->Set("User.SpinBoxValue.FileMaxFail", QVariant(Fails));
        }

        if(Type == "FilesFromDirectory")
        {
            ResourceWidget->Set("User.SpinBoxValue.DirectoryMaxSuccess", QVariant(Success));
            ResourceWidget->Set("User.SpinBoxValue.DirectoryMaxFail", QVariant(Fails));
        }

        if(Type == "Database")
        {
            ResourceWidget->Set("User.SpinBoxValue.DatabaseMaxSuccess", QVariant(Success));
            ResourceWidget->Set("User.SpinBoxValue.DatabaseMaxFail", QVariant(Fails));
        }

        if(Type == "LinesFromUrl")
        {
            ResourceWidget->Set("User.SpinBoxValue.UrlMaxSuccess", QVariant(Success));
            ResourceWidget->Set("User.SpinBoxValue.UrlMaxFail", QVariant(Fails));
        }
    }

    if(PageId > CurrentPageId)
    {
        if(PageId == 2 && Type != "FixedString")
        {
            next();
            return;
        }
        if(PageId == 3 && Type != "RandomInteger")
        {
            next();
            return;
        }
        if(PageId == 4 && Type != "FixedInteger")
        {
            next();
            return;
        }
        if(PageId == 5 && Type != "LinesFromFile" && Type != "FilesFromDirectory" && Type != "Database")
        {
            next();
            return;
        }
        if(PageId == 6 && Type != "LinesFromFile" && Type != "FilesFromDirectory" && Type != "Database" && Type != "LinesFromUrl")
        {
            next();
            return;
        }
        if(PageId == 7 && Type != "RandomString")
        {
            next();
            return;
        }
        if(PageId == 8 && Type != "Select")
        {
            next();
            return;
        }
        if(PageId == 9 && !IsAdditionalIf)
        {
            next();
            return;
        }
        if(PageId == 10 && !IsAdditionalTab)
        {
            next();
            return;
        }
    }else
    {
        if(PageId == 2 && Type != "FixedString")
        {
            back();
            return;
        }
        if(PageId == 3 && Type != "RandomInteger")
        {
            back();
            return;
        }
        if(PageId == 4 && Type != "FixedInteger")
        {
            back();
            return;
        }
        if(PageId == 5 && Type != "LinesFromFile" && Type != "FilesFromDirectory" && Type != "Database")
        {
            back();
            return;
        }
        if(PageId == 6 && Type != "LinesFromFile" && Type != "FilesFromDirectory" && Type != "Database" && Type != "LinesFromUrl")
        {
            back();
            return;
        }
        if(PageId == 7 && Type != "RandomString")
        {
            back();
            return;
        }
        if(PageId == 8 && Type != "Select")
        {
            back();
            return;
        }
        if(PageId == 9 && !IsAdditionalIf)
        {
            back();
            return;
        }
        if(PageId == 10 && !IsAdditionalTab)
        {
            back();
            return;
        }
    }

    CurrentPageId = PageId;
}

void ResourceWizard::SetResourceWidget(IResourceWidget *ResourceWidget)
{
    this->ResourceWidget = ResourceWidget;
}

ResourceWizard::~ResourceWizard()
{
    delete ui;
}


void ResourceWizard::on_Name_textEdited(const QString &arg1)
{
    QString Name = NamePrev;
    NamePrev = arg1;

    if(ui->DescripnionRu->text() == Name && ui->DescriptionEn->text() == Name)
    {
        ui->DescripnionRu->setText(arg1);
        ui->DescriptionEn->setText(arg1);
    }
}

void ResourceWizard::on_FixedString_toggled(bool checked)
{
    if(checked)
    {
        HideAllTypesLabels();
        ui->FixedStringLabel->setVisible(true);
        Type = "FixedString";
    }
}

void ResourceWizard::on_FixedInteger_toggled(bool checked)
{
    if(checked)
    {
        HideAllTypesLabels();
        ui->FixedIntegerLabel->setVisible(true);
        Type = "FixedInteger";
    }
}

void ResourceWizard::on_RandomString_toggled(bool checked)
{
    if(checked)
    {
        HideAllTypesLabels();
        ui->RandomStringLabel->setVisible(true);
        Type = "RandomString";
    }
}

void ResourceWizard::on_RandomInteger_toggled(bool checked)
{
    if(checked)
    {
        HideAllTypesLabels();
        ui->RandomIntegerLabel->setVisible(true);
        Type = "RandomInteger";
    }
}

void ResourceWizard::on_Select_toggled(bool checked)
{
    if(checked)
    {
        HideAllTypesLabels();
        ui->SelectLabel->setVisible(true);
        Type = "Select";
    }
}

void ResourceWizard::on_LinesFromFile_toggled(bool checked)
{
    if(checked)
    {
        HideAllTypesLabels();
        ui->LinesFromFileLabel->setVisible(true);
        Type = "LinesFromFile";
    }
}

void ResourceWizard::on_LinesFromUrl_toggled(bool checked)
{
    if(checked)
    {
        HideAllTypesLabels();
        ui->LinesFromUrlLabel->setVisible(true);
        Type = "LinesFromUrl";
    }
}

void ResourceWizard::on_FilesFromDirectory_toggled(bool checked)
{
    if(checked)
    {
        HideAllTypesLabels();
        ui->FilesFromDirectoryLabel->setVisible(true);
        Type = "FilesFromDirectory";
    }
}

void ResourceWizard::on_Database_toggled(bool checked)
{
    if(checked)
    {
        HideAllTypesLabels();
        ui->DatabaseLabel->setVisible(true);
        Type = "Database";
    }
}

void ResourceWizard::SetRandomStringEdit(const QString& text)
{
    ui->RandomStringValue->setText(text);
}

void ResourceWizard::on_StartStringBuilder_clicked()
{
    disconnect(StringBuilder,SIGNAL(Changed(QString)),this,SLOT(SetRandomStringEdit(QString)));
    connect(StringBuilder,SIGNAL(Changed(QString)),this,SLOT(SetRandomStringEdit(QString)));

    StringBuilder->Show(ui->RandomStringValue->text());
    disconnect(StringBuilder,SIGNAL(Changed(QString)),this,SLOT(SetRandomStringEdit(QString)));
}

void ResourceWizard::UpdateSelect()
{
    QPlainTextEdit * SelectValuesEdit = ui->SelectLines;
    QComboBox * SelectTypeCombo = ui->SelectType;
    QStringList list = SelectValuesEdit->toPlainText().split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
    QWidget* SelectWidget = ui->SelectDefaultValueContainer;
    MultiSelect *multi = SelectWidget->findChild<MultiSelect *>();
    if(multi)
        multi->Update(SelectTypeCombo->currentText(),list, QList<int>());
}

void ResourceWizard::on_SelectType_currentIndexChanged(int index)
{
    UpdateSelect();
}

void ResourceWizard::on_SelectLines_textChanged()
{
    UpdateSelect();
}

void ResourceWizard::on_Finish_clicked()
{
    accept();
}

void ResourceWizard::on_ToTab_clicked()
{
    IsAdditionalTab = true;
    back();
}

void ResourceWizard::SetSectionName(const QString& SectionName)
{
    QStringList list = SectionName.split("|");
    ui->TabEn->setText(list.first());
    ui->TabRu->setText(list.last());
    if(ui->TabEn->completer())
    {
        ui->TabEn->completer()->disconnect();
        ui->TabEn->completer()->deleteLater();
    }
    if(ui->TabRu->completer())
    {
        ui->TabRu->completer()->disconnect();
        ui->TabRu->completer()->deleteLater();
    }
}

bool ResourceWizard::eventFilter(QObject *target, QEvent *event)
{

    if (target == ui->TabEn)
    {
        if (event->type() == QEvent::FocusIn)
        {
            if(TooltipUserFocus)
            {
                TooltipUserFocus = false;
                ShowTooltip = true;
            }else
            {
                TooltipUserFocus = true;
            }


        }else if (event->type() == QEvent::MouseButtonRelease)
        {
            if(ShowTooltip)
            {
                ShowTooltip = false;

                if(!Groups.empty())
                {
                    QCompleter *completer = new QCompleter(Groups, ui->TabEn);
                    connect(completer,SIGNAL(activated(QString)),this,SLOT(SetSectionName(QString)));
                    completer->setCaseSensitivity(Qt::CaseInsensitive);
                    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

                    ui->TabEn->setCompleter(completer);
                    ui->TabEn->completer()->complete();
                }
            }
        }
    }

    if (target == ui->TabRu)
    {
        if (event->type() == QEvent::FocusIn)
        {
            if(TooltipUserFocus2)
            {
                TooltipUserFocus2 = false;
                ShowTooltip2 = true;
            }else
            {
                TooltipUserFocus2 = true;
            }


        }else if (event->type() == QEvent::MouseButtonRelease)
        {
            if(ShowTooltip2)
            {
                ShowTooltip2 = false;

                if(!Groups.empty())
                {
                    QCompleter *completer = new QCompleter(Groups, ui->TabRu);
                    connect(completer,SIGNAL(activated(QString)),this,SLOT(SetSectionName(QString)));
                    completer->setCaseSensitivity(Qt::CaseInsensitive);
                    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

                    ui->TabRu->setCompleter(completer);
                    ui->TabRu->completer()->complete();
                }
            }
        }
    }

    if (target == ui->VisibilityName)
    {
        if (event->type() == QEvent::FocusIn)
        {
            if(TooltipUserFocus3)
            {
                TooltipUserFocus3 = false;
                ShowTooltip3 = true;
            }else
            {
                TooltipUserFocus3 = true;
            }


        }else if (event->type() == QEvent::MouseButtonRelease)
        {
            if(ShowTooltip3)
            {
                ShowTooltip3 = false;

                if(!VariablesAndValues.empty())
                {
                    QCompleter *completer = new QCompleter(VariablesAndValues.keys(), ui->VisibilityName);
                    completer->setCaseSensitivity(Qt::CaseInsensitive);
                    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

                    ui->VisibilityName->setCompleter(completer);
                    ui->VisibilityName->completer()->complete();
                }
            }
        }
    }

    if (target == ui->VisibilityValue)
    {
        if (event->type() == QEvent::FocusIn)
        {
            if(TooltipUserFocus4)
            {
                TooltipUserFocus4 = false;
                ShowTooltip4 = true;
            }else
            {
                TooltipUserFocus4 = true;
            }


        }else if (event->type() == QEvent::MouseButtonRelease)
        {
            if(ShowTooltip4)
            {
                ShowTooltip4 = false;

                if(VariablesAndValues.contains(ui->VisibilityName->text()))
                {
                    QCompleter *completer = new QCompleter(VariablesAndValues[ui->VisibilityName->text()].split(QRegExp("[\r\n]"),QString::SkipEmptyParts), ui->VisibilityValue);
                    completer->setCaseSensitivity(Qt::CaseInsensitive);
                    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

                    ui->VisibilityValue->setCompleter(completer);
                    ui->VisibilityValue->completer()->complete();
                }
            }
        }
    }



    return QWidget::eventFilter(target, event);
}

void ResourceWizard::on_VisibleIf_clicked()
{
    IsAdditionalIf = true;
    back();
}
