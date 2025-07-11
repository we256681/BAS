#include "modulemanagerwindow.h"
#include "ui_modulemanagerwindow.h"
#include <QCheckBox>
#include <QLabel>
#include <QDir>
#include <QUrl>
#include <QDebug>
#include <QFileDialog>
#include <QMenu>
#include <QDesktopServices>
#include <QMessageBox>
#include "moduleinfodialog.h"
#include "modulecreatewizard.h"
#include "scrollarearepaint.h"
#include "every_cpp.h"

ModuleManagerWindow::ModuleManagerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModuleManagerWindow)
{
    ui->setupUi(this);
    new ScrollAreaRepaint(ui->ScrollArea);
    ui->MainWidget->layout()->setAlignment(Qt::AlignTop);
    ui->ClearFilter->setVisible(false);
    ProjectChanged = false;
    IsModuleListChanged = false;
}

bool CompareModules(IModuleManager::ModuleInfo &m1, IModuleManager::ModuleInfo &m2)
{
    return m1->Name.compare(m2->Name) < 0;
}

bool ModuleManagerWindow::IsProjectChnaged()
{
    return ProjectChanged;
}

bool ModuleManagerWindow::GetIsModuleListChanged()
{
    return IsModuleListChanged;
}


void ModuleManagerWindow::Reload()
{
    for(QWidget * w:ui->MainWidget->findChildren<QWidget *>())
    {
        if(w != ui->LabelCustomModules && w != ui->LabelNativeModules && w != ui->LabelEmptyCustom)
        {
            w->deleteLater();
        }
    }

    QList<IModuleManager::ModuleInfo> ModulesAll = ModuleManager->GetModuleInfo(false);
    QList<IModuleManager::ModuleInfo> ModulesNative;
    QList<IModuleManager::ModuleInfo> ModulesCustom;

    for(IModuleManager::ModuleInfo Info:ModulesAll)
    {
        if(Info->Folder == "custom")
        {
            ModulesCustom.append(Info);
        }

        if(Info->Folder == "modules")
        {
            ModulesNative.append(Info);
        }
    }

    ModulesAll.clear();

    qSort(ModulesNative.begin(), ModulesNative.end(), CompareModules);
    qSort(ModulesCustom.begin(), ModulesCustom.end(), CompareModules);

    ModulesAll.append(ModulesCustom);
    ModulesAll.append(ModulesNative);

    int CustomLength = ModulesCustom.length();

    ModulesNative.clear();
    ModulesCustom.clear();


    int index = 1;
    for(IModuleManager::ModuleInfo Info:ModulesAll)
    {
        if(CustomLength + 1 == index)
        {
            ui->LabelEmptyCustom->setVisible(CustomLength == 0);
            ui->NotFoundCustom->setVisible(false);

            ui->gridLayout->removeWidget(ui->LabelEmptyCustom);
            ui->gridLayout->addWidget(ui->LabelEmptyCustom,index,0,Qt::AlignLeft);

            index++;

            ui->gridLayout->removeWidget(ui->NotFoundCustom);
            ui->gridLayout->addWidget(ui->NotFoundCustom,index,0,Qt::AlignLeft);
            index++;

            ui->gridLayout->removeWidget(ui->LabelNativeModules);
            ui->gridLayout->addWidget(ui->LabelNativeModules,index,0,Qt::AlignLeft);
            index++;
        }
        QCheckBox *CheckBox = new QCheckBox(ui->MainWidget);
        CheckBox->setChecked(Info->IsEnabled);
        CheckBox->setProperty("name",Info->Name);
        connect(CheckBox,SIGNAL(stateChanged(int)),this,SLOT(CheckBoxClicked()));

        QWidget * Widget = new QWidget(this);
        Widget->setProperty("name",Info->Name);
        Widget->setProperty("custom",Info->Folder == "custom");
        Widget->setLayout(new QHBoxLayout());

        QLabel *Label = new QLabel(ui->MainWidget);
        QString Path = ":/engine/images/modules.png";
        if(!Info->IconPath.isEmpty())
        {
            Path = Info->IconPath;
        }
        Label->setText(QString("<html><head/><body><a href=\"bas://loadmodule\" style=\" text-decoration: none;\"> <table><tr><td><div>&nbsp;&nbsp;<span style=\" text-decoration: underline; color:white\">%1</span> <span style=\"color:white\"> - %3</span></div></td></tr><tr><td><div>&nbsp;&nbsp;<span style=\"color:gray;\">%2</span></div></td></tr></table></a></body></html>").arg(Info->Name).arg(Info->Description).arg(QString::number(Info->MajorVersion) + QString(".") + QString::number(Info->MinorVersion)).arg(Path));
        Label->setProperty("name",Info->Name);
        Label->setProperty("custom",Info->Folder == "custom");
        connect(Label,SIGNAL(linkActivated(QString)),this,SLOT(LabelClicked()));
        Label->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(Label, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(LabelClicked()));

        QPushButton * Button = new QPushButton(this);
        connect(Button, SIGNAL(clicked(bool)), this, SLOT(LabelClicked()));

        Button->setProperty("name",Info->Name);
        Button->setProperty("custom",Info->Folder == "custom");
        Button->setMinimumWidth(32);
        Button->setMaximumWidth(32);
        Button->setMinimumHeight(32);
        Button->setMaximumHeight(32);
        Button->setIconSize(QSize(32,32));
        Button->setStyleSheet(QString("QPushButton {border: 0px;}"));
        Button->setIcon(QIcon(Path));
        Button->setCursor(Qt::PointingHandCursor);
        Button->setFlat(true);

        Widget->layout()->addItem(new QSpacerItem(25, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));
        Widget->layout()->addWidget(Button);
        Widget->layout()->addWidget(Label);
        Widget->layout()->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
        Widget->layout()->setContentsMargins(0,5,0,5);
        Widget->layout()->setSpacing(0);

        ui->gridLayout->addWidget(Widget,index,0,Qt::AlignLeft);
        ui->gridLayout->addWidget(CheckBox,index,1,Qt::AlignRight);
        index++;

    }

    ui->NotFoundNative->setVisible(false);
    ui->gridLayout->removeWidget(ui->NotFoundNative);
    ui->gridLayout->addWidget(ui->NotFoundNative,index,0,Qt::AlignLeft);


}

void ModuleManagerWindow::CheckBoxClicked()
{
    QCheckBox * CheckBox = qobject_cast<QCheckBox *>(sender());
    bool IsChecked = CheckBox->isChecked();
    if(ModuleManager->GetIsRunning())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(tr("Can't enable or disable module while script is running."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        CheckBox->setChecked(!IsChecked);
        return;
    }
    IsModuleListChanged = true;
    QString ModuleName = sender()->property("name").toString();
    ModuleManager->SetModuleEnabled(ModuleName,IsChecked);
}

void ModuleManagerWindow::LabelClicked()
{
    CurrentMenuModuleName = sender()->property("name").toString();
    bool IsCustom = sender()->property("custom").toString() == "true";

    CurrentMenuModuleFolder = (IsCustom) ? "custom" : "modules";

    /*QDir ModuleDir = QDir::current();
    ModuleDir.cd(sender()->property("custom").toBool() ? "custom" : "modules");
    ModuleDir.cd(ModuleName);

    qDebug()<<ModuleDir.absolutePath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(ModuleDir.absolutePath()));
    */

    bool IsEnabled = ModuleManager->IsModuleEnabled(CurrentMenuModuleName);

    QMenu contextMenu(tr("Context menu"), this);

    QAction NameAction(CurrentMenuModuleName, this);
    NameAction.setEnabled(false);
    contextMenu.addAction(&NameAction);

    contextMenu.addSeparator();

    QAction EnableDisableAction(IsEnabled ? tr("Disable") : tr("Enable"), this);
    connect(&EnableDisableAction, SIGNAL(triggered()), this, SLOT(MenuEnableDisable()));
    contextMenu.addAction(&EnableDisableAction);



    QAction InfoAction(tr("Info"), this);
    connect(&InfoAction, SIGNAL(triggered()), this, SLOT(MenuInfo()));
    contextMenu.addAction(&InfoAction);

    QAction OpenContainingFolderAction(tr("Open containing folder"), this);
    connect(&OpenContainingFolderAction, SIGNAL(triggered()), this, SLOT(MenuOpenContainingFolder()));
    contextMenu.addAction(&OpenContainingFolderAction);

    QPoint pos = QCursor::pos();

    if(IsCustom)
    {
        QAction UninstallAction(tr("Uninstall"), this);
        connect(&UninstallAction, SIGNAL(triggered()), this, SLOT(MenuUninstall()));
        contextMenu.addAction(&UninstallAction);
        contextMenu.exec(pos);
    }else
    {
        contextMenu.exec(pos);
    }




}

void ModuleManagerWindow::MenuEnableDisable()
{
    if(ModuleManager->GetIsRunning())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(tr("Can't enable or disable module while script is running."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
    IsModuleListChanged = true;
    for(QCheckBox* CheckBox:ui->MainWidget->findChildren<QCheckBox *>())
    {
        QString ModuleName = CheckBox->property("name").toString();
        if(ModuleName == CurrentMenuModuleName)
        {
            CheckBox->click();
            break;
        }
    }
}

void ModuleManagerWindow::MenuUninstall()
{
    if(ModuleManager->GetIsRunning())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(tr("Can't uninstall module while script is running."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    IsModuleListChanged = true;

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(QString(tr("This will completely remove '%1' module.\nThis operation is not invertible, do you want to proceed?")).arg(CurrentMenuModuleName));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    if(msgBox.exec() == QMessageBox::Cancel)
    {
        return;
    }

    if(!QDir(QString("custom/") + CurrentMenuModuleName).removeRecursively())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(tr("Failed to uninstall module"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }else
        Reload();
}

void ModuleManagerWindow::MenuInfo()
{
    ModuleInfoDialog Info;
    if(Info.Init(CurrentMenuModuleName, CurrentMenuModuleFolder))
        Info.exec();
}

void ModuleManagerWindow::MenuOpenContainingFolder()
{
    QDir ModuleDir = QDir::current();
    ModuleDir.cd(CurrentMenuModuleFolder);
    ModuleDir.cd(CurrentMenuModuleName);

    qDebug()<<ModuleDir.absolutePath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(ModuleDir.absolutePath()));
}

void ModuleManagerWindow::SetModuleManager(IModuleManager *ModuleManager)
{
    this->ModuleManager = ModuleManager;
}

void ModuleManagerWindow::SetModuleCreator(IModuleCreator *ModuleCreator)
{
    this->ModuleCreator = ModuleCreator;
}

void ModuleManagerWindow::SetEmbeddedLanguageManager(IEmbeddedLanguageManager *EmbeddedLanguageManager)
{
    this->EmbeddedLanguageManager = EmbeddedLanguageManager;
}

ModuleManagerWindow::~ModuleManagerWindow()
{
    delete ui;
}

void ModuleManagerWindow::on_Close_clicked()
{
    close();
}

void ModuleManagerWindow::Filter()
{
    QString Text = ui->FilterText->text();
    ui->ClearFilter->setVisible(!Text.isEmpty());
    bool IsCustomPresent = false;
    bool IsNativePresent = false;
    bool IsCustomVisible = false;
    bool IsNativeVisible = false;
    for(QWidget * l:ui->MainWidget->findChildren<QWidget *>())
    {
        if(l != ui->LabelCustomModules && l != ui->LabelNativeModules && l != ui->LabelEmptyCustom)
        {
            QString Name = l->property("name").toString();

            bool IsVisible = Name.toLower().contains(Text.toLower());

            if(!Name.isEmpty())
                l->setVisible(IsVisible);

            if(l->property("custom").isValid())
            {
                if(l->property("custom").toBool())
                {
                    IsCustomPresent = true;
                    if(IsVisible)
                        IsCustomVisible = true;
                }else
                {
                    IsNativePresent = true;
                    if(IsVisible)
                        IsNativeVisible = true;
                }
            }

        }
    }
    ui->NotFoundNative->setVisible(IsNativePresent && !IsNativeVisible);
    ui->NotFoundCustom->setVisible(IsCustomPresent && !IsCustomVisible);
    ui->LabelEmptyCustom->setVisible(!IsCustomPresent);
}


void ModuleManagerWindow::on_FilterText_textChanged(const QString &arg1)
{
    Filter();
}

void ModuleManagerWindow::on_ClearFilter_clicked()
{
    ui->FilterText->setText(QString());
    //Filter();
}

void ModuleManagerWindow::on_CreateNewModule_clicked()
{
    ModuleCreateWizard Wizard;
    Wizard.SetModuleManager(ModuleManager);
    Wizard.SetModuleCreator(ModuleCreator);
    Wizard.Init();
    if(Wizard.exec())
    {
        ProjectChanged = true;
        Wizard.SaveData();
        bool ModuleInstalled = false;
        QList<EmbeddedLanguage> Languages;
        QList<EmbeddedModule> Modules;
        EmbeddedLanguageManager->ReadLanguageListLocal(Languages,Modules);
        QString Path = ModuleCreator->CreateModule(ModuleInstalled, ModuleManager->GetIsRunning(), Languages, Modules,EmbeddedLanguageManager->GetEmbeddedCode());
        if(ModuleInstalled)
        {
            Reload();
        }
        QDesktopServices::openUrl(QUrl::fromLocalFile(Path));

    }
}

void ModuleManagerWindow::on_InstallModule_clicked()
{
    if(ModuleManager->GetIsRunning())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(tr("Can't install module while script is running."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Module"), "", tr("Module Files (*.zip);;All Files (*.*)"));

    if(fileName.length()>0)
    {
        IsModuleListChanged = true;
        QString InstallResult = ModuleCreator->InstallModule(fileName, 2);

        if(!InstallResult.isEmpty())
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText(QString(tr("Failed to install module.\n\n%1             \n")).arg(InstallResult));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }else
        {
            Reload();
        }
    }
}
