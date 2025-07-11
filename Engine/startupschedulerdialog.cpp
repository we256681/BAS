#include "startupschedulerdialog.h"
#include "ui_startupschedulerdialog.h"

#include "every_cpp.h"

StartupSchedulerDialog::StartupSchedulerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartupSchedulerDialog)
{
    ui->setupUi(this);
    this->layout()->removeWidget(ui->Background);
    ui->Background->setParent(this);
    ui->Background->resize(150,150);
    ui->Background->move(230,-10);
    this->resize(352,272);

}

StartupSchedulerDialog::~StartupSchedulerDialog()
{
    delete ui;
}

bool StartupSchedulerDialog::GetNeedToStartScheduler()
{
    return ui->RunScheduler->isChecked();
}

void StartupSchedulerDialog::SetNeedToStartScheduler(bool NeedToStartScheduler)
{
    if(NeedToStartScheduler)
    {
        ui->RunScheduler->setChecked(true);
        ui->RunInstant->setChecked(false);
    }else
    {
        ui->RunScheduler->setChecked(false);
        ui->RunInstant->setChecked(true);
    }
}

void StartupSchedulerDialog::on_pushButton_clicked()
{
    this->accept();
}
