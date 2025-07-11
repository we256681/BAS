#ifndef UPLOADPROJECT_H
#define UPLOADPROJECT_H

#include <QDialog>
#include "googledriveapi.h"

namespace Ui {
class UploadProject;
}

using namespace BrowserAutomationStudioFramework;

class UploadProject : public QDialog
{
    Q_OBJECT
    QString CurrentProject;
    GoogleDriveAPI *API;
public:
    explicit UploadProject(QWidget *parent = nullptr);
    void SetCurrentProject(const QString& CurrentProject);
    void Start();
    ~UploadProject();

private slots:
    void on_Cancel_clicked();
    void on_Ok_clicked();
    void Result(bool IsSuccess, QString LinkOrErrorMessage);
    void SetStatus(QString LinkOrErrorMessage);

signals:
    void Show();

private:
    Ui::UploadProject *ui;
};

#endif // UPLOADPROJECT_H
