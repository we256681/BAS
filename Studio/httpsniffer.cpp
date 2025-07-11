#include "httpsniffer.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDesktopServices>
#include "ui_httpsniffer.h"

HttpSniffer::HttpSniffer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HttpSniffer)
{
    ui->setupUi(this);
    setWindowTitle(tr("HTTP Sniffer"));
    QStringList Columns;
    Columns.append(tr("Method"));
    Columns.append(tr("Code"));
    Columns.append(tr("Send Bytes"));
    Columns.append(tr("Received Bytes"));
    Columns.append(tr("Url"));

    int index = 0;
    for(QString& ColumnName:Columns)
    {
        ui->Requests->setHorizontalHeaderItem(index, new QTableWidgetItem(ColumnName));
        index++;
    }

    ui->splitter->setSizes(QList<int>()<<600<<400);

    connect(ui->Requests,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(ItemChanged(int,int)));

}

void HttpSniffer::ItemChanged(int row,int column)
{
    ui->ReceivedText->clear();
    ui->SentText->clear();
    if(row >= 0 && row <= Data.length() - 1)
    {
        SnifferReport& Report = Data[row];
        ui->ReceivedText->setPlainText(Report.ReceivedHeaders + QString("\n\n") + QString::fromUtf8(Report.ReceivedData));
        ui->SentText->setPlainText(Report.SentHeaders + QString("\n\n") + QString::fromUtf8(Report.SentData));

        if(!Report.ContentType.contains("text/html")
                && !Report.ContentType.contains("text/javascript")
                && !Report.ContentType.contains("application/x-javascript")
                && !Report.ContentType.contains("application/javascript")
                && !Report.ContentType.contains("application/json")
                && !Report.ContentType.contains("image/")
                )
        {
            ui->Preview->setVisible(false);
        }else
        {
            ui->Preview->setVisible(true);
        }
    }
}

void HttpSniffer::ItemUpdate()
{
    ItemChanged(ui->Requests->currentRow(),0);
}


void HttpSniffer::NewSnifferReport(SnifferReport Report)
{
    if(!ui->SaveLogs->isChecked())
    {
        return;
    }
    int index = 0;
    bool Found = false;
    SnifferReport FinalReport;
    for(SnifferReport& D: Data)
    {
        if(D.RequestId == Report.RequestId)
        {
            Found = true;
            break;
        }
        index ++;
    }
    if(!Found)
    {
        Data.append(Report);
        Report.Clone(FinalReport);
        index = Data.length() - 1;
        ui->Requests->insertRow(index);
        if(Data.length() == 1)
        {
            ui->Requests->selectRow(0);
        }

    }else
    {
        SnifferReport& D = Data[index];
        if(Report.Code)
            D.Code = Report.Code;

        D.IsFinished = Report.IsFinished;

        if(!Report.Method.isEmpty())
            D.Method = Report.Method;

        if(Report.ReceivedBytes)
            D.ReceivedBytes += Report.ReceivedBytes;

        D.ReceivedData += Report.ReceivedData;
        D.ReceivedHeaders += Report.ReceivedHeaders;

        D.RequestId = Report.RequestId;

        if(Report.SentBytes)
            D.SentBytes = Report.SentBytes;

        if(!Report.ContentType.isEmpty())
            D.ContentType = Report.ContentType;

        D.SentHeaders += Report.SentHeaders;
        D.SentData += Report.SentData;

        if(!Report.Url.isEmpty())
            D.Url = Report.Url;

        D.Clone(FinalReport);
    }

    QTableWidgetItem *item;

    item = new QTableWidgetItem(FinalReport.Method);
    item->setTextAlignment(Qt::AlignCenter);
    ui->Requests->setItem(index, 0, item);

    item = new QTableWidgetItem(QString::number(FinalReport.Code));
    item->setTextAlignment(Qt::AlignCenter);
    ui->Requests->setItem(index, 1, item);

    item = new QTableWidgetItem(QString::number(FinalReport.SentBytes));
    item->setTextAlignment(Qt::AlignCenter);
    ui->Requests->setItem(index, 2, item);

    item = new QTableWidgetItem(QString::number(FinalReport.ReceivedBytes));
    item->setTextAlignment(Qt::AlignCenter);
    ui->Requests->setItem(index, 3, item);

    item = new QTableWidgetItem(FinalReport.Url);
    item->setTextAlignment(Qt::AlignLeft);
    ui->Requests->setItem(index, 4, new QTableWidgetItem(FinalReport.Url));
    this->show();
    this->raise();
    ItemUpdate();
}

void HttpSniffer::Show()
{
    this->show();
    this->raise();
    ui->SaveLogs->setChecked(true);
    emit StateChanged(true);
}


HttpSniffer::~HttpSniffer()
{
    delete ui;
}

void HttpSniffer::on_SaveLogs_clicked()
{
    emit StateChanged(ui->SaveLogs->isChecked());
}

void HttpSniffer::on_Close_clicked()
{
    close();
}

void HttpSniffer::on_ClearLogs_clicked()
{
    ui->Requests->setRowCount(0);
    Data.clear();
}

void HttpSniffer::on_Preview_clicked()
{
    int row = ui->Requests->currentRow();
    if(row >= 0 && row <= Data.length() - 1)
    {
        SnifferReport& Report = Data[row];
        QString Extension;
        if(Report.ContentType.contains("text/html"))
            Extension = ".html";
        if(Report.ContentType.contains("text/javascript") || Report.ContentType.contains("application/x-javascript") || Report.ContentType.contains("application/json") || Report.ContentType.contains("application/javascript"))
            Extension = ".js";

        if(Report.ContentType.contains("image/gif"))
            Extension = ".gif";

        if(Report.ContentType.contains("image/jpeg") || Report.ContentType.contains("image/jpg"))
            Extension = ".jpeg";

        if(Report.ContentType.contains("image/png"))
            Extension = ".png";

        if(!Extension.isEmpty())
        {
            QString File = QString("preview") + Extension;

            QFile file(File);
            file.remove();
            if(file.open(QIODevice::WriteOnly))
            {
                file.write(Report.ReceivedData);
                file.close();
            }
            QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(File).absoluteFilePath()));
        }
    }
}
