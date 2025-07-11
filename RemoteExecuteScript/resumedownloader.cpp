#include "resumedownloader.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QCryptographicHash>
#include <QStringList>


ResumeDownloader::ResumeDownloader(QObject *parent) : QObject(parent)
{
    CheckSpeedTimer = new QTimer(this);
    CheckSpeedTimer->setSingleShot(false);
    CheckSpeedTimer->setInterval(CheckSpeedInterval);
    connect(CheckSpeedTimer,SIGNAL(timeout()),this,SLOT(CheckSpeed()));
}

void ResumeDownloader::CheckSpeed()
{
    emit Log(QString("Downloaded ") + QString::number(CheckSpeedCurrentValue - CheckSpeedLastValue) + QString(" bytes"));
    if(CheckSpeedCurrentValue - CheckSpeedLastValue < CheckSpeedMinimumSize)
    {
        Client->Stop();
        FinishDownloadNextChunk();
        return;
    }
    CheckSpeedLastValue = CheckSpeedCurrentValue;
}

void ResumeDownloader::Init(IHttpClient *Client)
{
    this->Client = Client;
}

void ResumeDownloader::DownloadProgressSlot(qint64 BytesReceived, qint64 BytesTotal)
{
    CheckSpeedCurrentValue = BytesReceived;
    emit DownloadProgress(BytesReceived + CurrentDownloadPosition,TotalSize);
}

bool ResumeDownloader::WasError()
{
    return _WasError;
}

QString ResumeDownloader::GetErrorString()
{
    return _LastError;
}

QList<QByteArray>* ResumeDownloader::GetPageData()
{
    return &ResultChunks;
}

void ResumeDownloader::Get(const QString &Url)
{
    this->Url = Url;
    this->MetaUrl = Url + QString(".meta.json");

    emit Log(QString("Downloading meta ") + MetaUrl);

    //Download meta
    Client->Connect(this,SLOT(DownloadMetaResult()));
    Client->SetTimeout(MaximumDownloadTimeForSimpleRequest);
    Client->Get(MetaUrl);
}


void ResumeDownloader::DownloadMetaResult()
{
    IsMeta = false;
    if(!Client->WasError())
    {
        QJsonParseError err;
        QJsonDocument JsonResponse = QJsonDocument::fromJson(Client->GetContent().toUtf8(),&err);
        if(err.error == QJsonParseError::NoError)
        {
            QJsonObject JsonObj = JsonResponse.object();
            if(JsonObj.contains("TotalSize") && JsonObj.contains("Chunks") && JsonObj.contains("Checksum") && JsonObj.contains("Url"))
            {
                TotalSize = JsonObj["TotalSize"].toString().toLongLong();
                Checksum = JsonObj["Checksum"].toString();
                AlternativeUrl = JsonObj["Url"].toString();
                if(!AlternativeUrl.isEmpty())
                {
                    Url = AlternativeUrl;
                    emit Log(QString("Using alternative url - ") + Url);
                }
                QVariantList array = JsonObj["Chunks"].toArray().toVariantList();
                foreach(QVariant v,array)
                {
                    Chunks.append(v.toString());
                }
                IsMeta = true;
                emit Log(QString("Meta was correctly downloaded and parsed"));
            }else
            {
                emit Log(QString("Meta json has wrong format"));
            }

        }else
        {
            emit Log(QString("Failed to parse meta json with error - ") + err.errorString());
        }
    }else
    {
        emit Log(QString("Failed to download meta with error - ") + Client->GetErrorString());
    }

    if(!IsMeta)
    {
        emit Log(QString("Trying to get file size"));
        Client->AddHeader("Range", QString("bytes=0-0"));
        Client->Connect(this,SLOT(TotalSizeResult()));
        Client->SetTimeout(MaximumDownloadTimeForSimpleRequest);
        Client->Get(Url);
    }else
    {
        CurrentChunkIndex = 0;
        StartDownloadNextChunk();
    }
}

void ResumeDownloader::TotalSizeResult()
{
    if(Client->WasError())
    {
        _LastError = QString("Failed to get file size with error - ") + Client->GetErrorString();
        emit Log(_LastError);
        _WasError = true;
        emit Finished();
        return;
    }
    QString ContentRange = Client->GetHeader("Content-Range");
    if(ContentRange.isEmpty())
    {
        _LastError = QString("Failed to get file size, range header is empty");
        emit Log(_LastError);
        _WasError = true;
        emit Finished();
        return;
    }
    QStringList ContentRangeSplit = ContentRange.split("/");
    if(ContentRangeSplit.length() != 2)
    {
        _LastError = QString("Failed to get file size, range header wrong format");
        emit Log(_LastError);
        _WasError = true;
        emit Finished();
        return;
    }

    TotalSize = ContentRangeSplit.at(1).toLongLong();
    if(TotalSize <= 0)
    {
        _LastError = QString("File size is less than 0");
        emit Log(_LastError);
        _WasError = true;
        emit Finished();
        return;
    }

    emit Log(QString("Obtained file size - ") + QString::number(TotalSize));

    CurrentChunkIndex = 0;
    StartDownloadNextChunk();
}

void ResumeDownloader::StartDownloadNextChunk()
{

    //Stop condition
    bool AllDataDownloaded = false;
    if(CurrentDownloadPosition >= TotalSize)
    {
        AllDataDownloaded = true;
    }

    if(AllDataDownloaded)
    {
        bool IsSuccess = true;
        QString Error;

        if(IsMeta)
        {
            QList<QByteArray>* AllData = GetPageData();
            int Length = AllData->size();

            QCryptographicHash Hash(QCryptographicHash::Sha1);

            for(int i = 0;i<Length;i++)
            {
                Hash.addData(AllData->at(i));
            }


            QString CurrentSha1 = QString(Hash.result().toHex());
            QString CorrectSha1 = Checksum;

            if(CurrentSha1 != CorrectSha1)
            {
                Error = QString("Failed to verify checksum, want ") + CorrectSha1 + QString(", got ") + CurrentSha1;
                IsSuccess = false;

            }
        }

        if(IsSuccess)
        {
            _WasError = false;
            _LastError.clear();
            emit Log(QString("All data has been downloaded"));
        }else
        {
            _WasError = true;
            _LastError = Error;
            emit Log(Error);
        }

        emit Finished();
        return;
    }

    CurrentRetryCount = 0;

    DownloadNextChunk();
}


void ResumeDownloader::DownloadNextChunk()
{
    emit Log(QString("Start download chunk - ") + QString::number(CurrentDownloadPosition));

    //Download chunk
    Client->Connect(this,SLOT(FinishDownloadNextChunk()));
    Client->AddHeader("Range", QString("bytes=") + QString::number(CurrentDownloadPosition) + QString("-") + QString::number(CurrentDownloadPosition + ChunkSize - 1));
    Client->SetTimeout(MaximumDownloadTimeForOneChunk);
    CheckSpeedLastValue = 0;
    CheckSpeedCurrentValue = 0;
    CheckSpeedTimer->start();
    Client->Get(Url);
    disconnect(this->Client,SIGNAL(DownloadProgress(qint64,qint64)),this,SLOT(DownloadProgressSlot(qint64,qint64)));
    connect(this->Client,SIGNAL(DownloadProgress(qint64,qint64)),this,SLOT(DownloadProgressSlot(qint64,qint64)));

}

void ResumeDownloader::FinishDownloadNextChunk()
{
    CheckSpeedTimer->stop();
    bool IsSuccess = false;
    QString Error;
    if(Client->WasError())
    {
        Error = Client->GetErrorString();
        IsSuccess = false;
    }else
    {
        QByteArray Data = Client->GetPageData();
        if(IsMeta)
        {
            QString CurrentSha1 = QString(QCryptographicHash::hash(Data,QCryptographicHash::Sha1).toHex());
            QString CorrectSha1 = Chunks[CurrentChunkIndex];

            if(CurrentSha1 != CorrectSha1)
            {
                Error = QString("Failed to verify checksum, want ") + CorrectSha1 + QString(", got ") + CurrentSha1;
                IsSuccess = false;
            }else
            {
                IsSuccess = true;
            }
        }else
        {
            IsSuccess = true;
        }
    }

    if(!IsSuccess)
    {
        CurrentRetryCount ++;
        if(CurrentRetryCount > RetryCount)
        {
            emit Log(QString("Too much fails, stop"));
            _WasError = true;
            _LastError = Error;
            emit Finished();
            return;
        }
        emit Log(QString("Finish download next chunk with error - ") + Error);
        emit Log(QString("Sleep for ") + QString::number(RetryInterval) + QString(" msec"));
        QTime EndSleepTime = QTime::currentTime().addMSecs(RetryInterval);
        while(QTime::currentTime() < EndSleepTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 30);

        DownloadNextChunk();
    }else
    {
        emit Log(QString("Chunk downlad success"));

        ResultChunks.append(Client->GetPageData());
        CurrentDownloadPosition += ChunkSize;
        CurrentChunkIndex ++;
        StartDownloadNextChunk();
    }
}
