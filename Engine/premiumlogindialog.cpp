#include "premiumlogindialog.h"
#include "ui_premiumlogindialog.h"
#include "every_cpp.h"

PremiumLoginDialog::PremiumLoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PremiumLoginDialog)
{
    ui->setupUi(this);
    UseWebInterface = false;

}

PremiumLoginDialog::~PremiumLoginDialog()
{
    delete ui;
}

bool PremiumLoginDialog::GetUsesWebInterface()
{
    return UseWebInterface;
}


QString PremiumLoginDialog::GetPremiumLogin()
{
    return ui->Username->text();
}


QString PremiumLoginDialog::GetPremiumPassword()
{
    return ui->Password->text();
}

void PremiumLoginDialog::SetPremiumLogin(const QString& Login)
{
    ui->Username->setText(Login);
}
void PremiumLoginDialog::SetPremiumPassword(const QString& Password)
{
    ui->Password->setText(Password);
}


void PremiumLoginDialog::on_Ok_clicked()
{
    UseWebInterface = true;
    close();
}

void PremiumLoginDialog::on_Cancel_clicked()
{
    UseWebInterface = false;
    close();
}
