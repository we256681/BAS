#include "askifrunwithwebinterfacedialog.h"
#include "ui_askifrunwithwebinterfacedialog.h"
#include "every_cpp.h"

AskIfRunWithWebInterfaceDialog::AskIfRunWithWebInterfaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AskIfRunWithWebInterfaceDialog)
{
    ui->setupUi(this);
    UseWebInterface = false;
}

bool AskIfRunWithWebInterfaceDialog::GetUsesWebInterface()
{
    return UseWebInterface;
}

bool AskIfRunWithWebInterfaceDialog::GetRememberChoice()
{
    return ui->RememberCheckbox->isChecked();
}

void AskIfRunWithWebInterfaceDialog::SetScriptName(const QString& ScriptName)
{
    ui->MainLabel->setText(ScriptName + QString(" ") + tr("has web interface, do you want to use it or use default interface?"));
}

AskIfRunWithWebInterfaceDialog::~AskIfRunWithWebInterfaceDialog()
{
    delete ui;
}

void AskIfRunWithWebInterfaceDialog::on_RunWithWebInterfaceButton_clicked()
{
    UseWebInterface = true;
    this->close();
}

void AskIfRunWithWebInterfaceDialog::on_RunWithNormalInterfaceButton_clicked()
{
    UseWebInterface = false;
    this->close();
}
