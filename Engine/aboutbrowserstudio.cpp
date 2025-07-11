#include "aboutbrowserstudio.h"
#include "ui_aboutbrowserstudio.h"
#include <QIcon>
#include "every_cpp.h"


AboutBrowserStudio::AboutBrowserStudio(const QString& Version,const QStringList& BrowserVersionList,const QString& ServerName,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutBrowserStudio)
{
    ui->setupUi(this);
    ui->LabelWebkit->setText(QString("<b>") + BrowserVersionList.join("\n") + QString("</b>"));
    ui->LabelVersion->setText(QString("<b>") + Version + QString("</b>"));
    ui->LabelLicenseType->setText(QString("<b>") + tr("Free") + QString("</b>"));

    ui->label->setPixmap(QIcon(":/engine/images/Logo.png").pixmap(100,100));

    ui->LabelDescription->setText(QString(tr("<html><head/><body><p><span style=\" font-size:14px;\">Browser Automation Studio - a comprehensive solution for creating complex network software. </span>Visit <a href=\"%1\"><span style=\" text-decoration: underline; color:#ffffff;\">%1</span></a> to see more details</p></body></html>")).arg(ServerName));

    ui->LogoLayout->setAlignment(ui->label,Qt::AlignTop);

    //Use minimal size and don't allow to resize
    this->resize(327,0);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

}

void AboutBrowserStudio::SetIsPremium()
{
    ui->LabelLicenseType->setText(QString("<b>") + tr("Premium") + QString("</b>"));

}

void AboutBrowserStudio::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

AboutBrowserStudio::~AboutBrowserStudio()
{
    delete ui;
}
