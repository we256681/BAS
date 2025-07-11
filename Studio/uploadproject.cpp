#include "uploadproject.h"
#include <QFileInfo>
#include <QClipboard>
#include "ui_uploadproject.h"

UploadProject::UploadProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UploadProject)
{
    ui->setupUi(this);
    ui->Error->hide();
    ui->Ok->hide();
    API = new GoogleDriveAPI(this);
}

void UploadProject::SetCurrentProject(const QString& CurrentProject)
{
    this->CurrentProject = CurrentProject;
}

void UploadProject::Start()
{
    API->Init("498969785969-osm727ke4ukhfss6k83ed3cdd77ammvl.apps.googleusercontent.com","cE8DzYdEIeNwgYbUGfUleVNN");
    connect(API,SIGNAL(Result(bool,QString)),this,SLOT(Result(bool,QString)));
    connect(API,SIGNAL(SetStatus(QString)),this,SLOT(SetStatus(QString)));
    API->UploadFile(CurrentProject,QFileInfo(CurrentProject).fileName());
}

void UploadProject::Result(bool IsSuccess, QString LinkOrErrorMessage)
{
    if(IsSuccess)
    {
        ui->Status->setText(tr("Current project has been uploaded, link was copied to clipboard."));
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(LinkOrErrorMessage);
        emit Show();
    }else
    {
        ui->Status->setVisible(false);
        ui->Error->setText(LinkOrErrorMessage);
        ui->Error->setVisible(true);
    }
    ui->Ok->setVisible(true);
}


void UploadProject::SetStatus(QString LinkOrErrorMessage)
{
    ui->Status->setText(LinkOrErrorMessage);
}


UploadProject::~UploadProject()
{
    delete ui;
}

void UploadProject::on_Cancel_clicked()
{
    close();
}

void UploadProject::on_Ok_clicked()
{
    close();
}
