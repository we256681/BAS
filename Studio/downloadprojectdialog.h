#ifndef DOWNLOADPROJECTDIALOG_H
#define DOWNLOADPROJECTDIALOG_H

#include <QDialog>
#include "httpclient.h"


namespace Ui {
class DownloadProjectDialog;
}

using namespace BrowserAutomationStudioFramework;

class DownloadProjectDialog : public QDialog
{
    Q_OBJECT
    bool Success;
    QString Project;
public:
    explicit DownloadProjectDialog(QWidget *parent = nullptr);
    ~DownloadProjectDialog();
    bool GetIsSuccess();
    QString GetProject();

private slots:
    void on_Cancel_clicked();
    void UrlLoaded();
    void DownloadProgress(qint64 BytesReceived,qint64 BytesTotal);
    void on_Ok_clicked();

private:
    QString IsValid(const QString& Url);
    QString GetFilename();

    Ui::DownloadProjectDialog *ui;
};

#endif // DOWNLOADPROJECTDIALOG_H
