#include "databaseadminfilteritem.h"
#include "ui_databaseadminfilteritem.h"

DatabaseAdminFilterItem::DatabaseAdminFilterItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseAdminFilterItem)
{
    ui->setupUi(this);
    ui->pushButton->setIcon(QIcon(":/engine/images/crud_delete.png"));
    ui->pushButton->setMinimumHeight(18);
    ui->pushButton->setMinimumWidth(18);
    ui->pushButton->setMaximumHeight(18);
    ui->pushButton->setMaximumWidth(18);
}

void DatabaseAdminFilterItem::SetText(const QString& Text)
{
    ui->LabelText->setText(Text);
}

DatabaseAdminFilterItem::~DatabaseAdminFilterItem()
{
    delete ui;
}

void DatabaseAdminFilterItem::on_pushButton_clicked()
{
    emit NeedDelete();
}
