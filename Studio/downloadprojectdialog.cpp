#include "downloadprojectdialog.h"
#include "ui_downloadprojectdialog.h"
#include <QRegularExpression>
#include <QUrl>
#include <QDir>
#include <QClipboard>
#include "parsecontentdisposition.h"

DownloadProjectDialog::DownloadProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadProjectDialog)
{
    ui->setupUi(this);
    ui->Validation->hide();
    ui->progressBar->hide();
    Success = false;

    QClipboard *Clipboard = QApplication::clipboard();
    QString TextInClipboard = Clipboard->text();
    TextInClipboard = IsValid(TextInClipboard);
    ui->lineEdit->setText(TextInClipboard);
    ui->lineEdit->setFocus();
    ui->lineEdit->selectAll();
}

QString DownloadProjectDialog::GetFilename()
{

   return QDateTime::currentDateTime().toString("dd.MM.yyyy.hh:mm");
}

QString DownloadProjectDialog::IsValid(const QString& Url)
{
    QUrl U(Url);
    if(!U.isValid())
        return QString();

    QString UString = U.url(QUrl::FullyEncoded);
    if(U.scheme().isEmpty())
    {
        UString = QString("http://") + UString;
    }
    UString = UString.replace("www.dropbox.com","dl.dropbox.com");
    QRegularExpression re(R"(^(https?:\/\/)?(www\.)?[-a-zA-Z0-9@:%._\+~#=]{2,256}\.[a-z]{2,4}\b([-a-zA-Z0-9@:%_\+.~#?&//=]*)$)");
    QRegularExpressionMatch match = re.match(UString);
    if(match.hasMatch())
    {
        return UString;
    }else
    {
        return QString();
    }
}


bool DownloadProjectDialog::GetIsSuccess()
{
    return Success;
}

QString DownloadProjectDialog::GetProject()
{
    return Project;
}

DownloadProjectDialog::~DownloadProjectDialog()
{
    delete ui;
}

void DownloadProjectDialog::on_Cancel_clicked()
{
    this->close();
}

void DownloadProjectDialog::DownloadProgress(qint64 BytesReceived,qint64 BytesTotal)
{
    if(BytesTotal)
        ui->progressBar->setValue(BytesReceived * 100 / BytesTotal);
}

void DownloadProjectDialog::on_Ok_clicked()
{
    QString Url = ui->lineEdit->text();
    ui->Validation->setVisible(false);
    Url = IsValid(Url);
    if(Url.isEmpty())
    {
        ui->Validation->setText(tr("Url is invalid"));
        ui->Validation->setVisible(true);
        return;
    }
    ui->Ok->setEnabled(false);
    ui->Ok->setStyleSheet("*{color:gray}");
    ui->progressBar->setValue(0);
    ui->progressBar->show();

    IHttpClient *_HttpClient = new HttpClient();
    _HttpClient->setParent(this);
    _HttpClient->Connect(this,SLOT(UrlLoaded()));
    connect(_HttpClient,SIGNAL(DownloadProgress(qint64,qint64)), this, SLOT(DownloadProgress(qint64,qint64)));
    _HttpClient->Get(Url);
}

void DownloadProjectDialog::UrlLoaded()
{
    IHttpClient *_HttpClient = qobject_cast<IHttpClient *>(sender());
    if(!_HttpClient)
        return;

    _HttpClient->Disconnect();

    QString Location = _HttpClient->GetHeader("Location");

    {
        //Relative location
        while(Location.startsWith("."))
            Location.remove(0,1);

        if(Location.startsWith("//"))
        {
            QUrl url = QUrl(_HttpClient->GetLastUrl());

            QUrl urllocation = QUrl(Location);
            urllocation.setScheme(url.scheme());
            Location = urllocation.toString();

        }else if(Location.startsWith("/"))
        {
            QUrl url = QUrl(_HttpClient->GetLastUrl());
            QUrl urllocation = QUrl(Location);
            url.setPath(urllocation.path());
            url.setQuery(urllocation.query());
            url.setFragment(urllocation.fragment());

            Location = url.toString();
        }
    }
    if(!Location.isEmpty())
    {
        _HttpClient->Connect(this,SLOT(UrlLoaded()));
        _HttpClient->Get(Location);
        return;
    }


    _HttpClient->deleteLater();
    ui->progressBar->hide();

    if(_HttpClient->WasError())
    {
        ui->Validation->setText(tr("Error during downloading project : ") + _HttpClient->GetErrorString());
        ui->Ok->setEnabled(true);
        ui->Ok->setStyleSheet("");
        ui->lineEdit->setFocus();
        ui->lineEdit->selectAll();
        ui->Validation->setVisible(true);
        return;
    }

    QString ProjectString = _HttpClient->GetContent();
    if(!ProjectString.contains("</BrowserAutomationStudioProject>") || !ProjectString.contains("<BrowserAutomationStudioProject>"))
    {
        ui->Validation->setText(tr("Downloaded file doesn't seems like a BAS project. Remember, that you need to put direct link to the project."));
        ui->Ok->setEnabled(true);
        ui->Ok->setStyleSheet("");
        ui->lineEdit->setFocus();
        ui->lineEdit->selectAll();
        ui->Validation->setVisible(true);
        return;
    }

    QString Filename;

    QString ContentDisposition = _HttpClient->GetHeader("Content-Disposition");

    if(!ContentDisposition.isEmpty())
    {
        Filename = ContentDispositionToFileName(ContentDisposition);
    }

    if(Filename.isEmpty())
    {
        Filename = GetFilename() + QString(".xml");
    }

    QDir WriteDir("fromurl");
    WriteDir.mkpath(".");
    QString AbsolutePath = WriteDir.absoluteFilePath(Filename);

    if(QFile(AbsolutePath).exists())
    {
        AbsolutePath = WriteDir.absoluteFilePath(QFileInfo(AbsolutePath).baseName() + QString(".") + GetFilename() + QString(".xml"));
    }

    QFile ProjectFile(AbsolutePath);
    ProjectFile.open(QIODevice::WriteOnly);
    ProjectFile.write(QByteArray::fromBase64(_HttpClient->GetBase64().toUtf8()));
    ProjectFile.close();
    Success = true;
    Project = AbsolutePath;
    accept();
}

