#include "moduleinfodialog.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "ui_moduleinfodialog.h"

ModuleInfoDialog::ModuleInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModuleInfoDialog)
{
    ui->setupUi(this);
    ui->Name->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->Description->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->Version->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->DeveloperName->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->DeveloperEmail->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->DeveloperSite->setTextInteractionFlags(Qt::TextSelectableByMouse);
}

ModuleInfoDialog::~ModuleInfoDialog()
{
    delete ui;
}

bool ModuleInfoDialog::Init(const QString& ModuleName, const QString& Folder)
{
    ui->Name->setText(ModuleName);
    bool IsSuccess = false;

    QJsonParseError err;
    QFile manifest(Folder + QString("/") + ModuleName + QString("/manifest.json"));
    if(manifest.open(QIODevice::ReadOnly))
    {
        QByteArray json = manifest.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(json, &err);
        manifest.close();

        if(err.error == QJsonParseError::NoError)
        {
            if(doc.object().contains("name")
            && doc.object().contains("description")
            && doc.object().contains("developer_name")
            && doc.object().contains("developer_email")
            && doc.object().contains("developer_site")
            && doc.object().contains("major_version")
            && doc.object().contains("minor_version")
                    )
            {
                QString IconPath;
                if(doc.object().contains("icon"))
                {
                    IconPath = doc.object()["icon"].toString();
                    if(!IconPath.isEmpty())
                    {
                        IconPath = Folder + QString("/") + ModuleName + QString("/") + IconPath;
                    }
                }

                if(IconPath.isEmpty())
                {
                    IconPath = ":/engine/images/modules.png";
                }

                ui->Icon->setPixmap(QIcon(IconPath).pixmap(32,32));

                ui->Description->setText(doc.object()["description"].toString());
                ui->DeveloperName->setText(doc.object()["developer_name"].toString());
                ui->DeveloperEmail->setText(doc.object()["developer_email"].toString());
                ui->DeveloperSite->setText(doc.object()["developer_site"].toString());

                QString Version;
                Version.append(QString::number(doc.object()["major_version"].toInt()));
                Version.append(".");
                Version.append(QString::number(doc.object()["minor_version"].toInt()));

                ui->Version->setText(Version);

                IsSuccess = true;


            }
        }
    }
    return IsSuccess;

}


void ModuleInfoDialog::on_pushButton_clicked()
{
    accept();
}
