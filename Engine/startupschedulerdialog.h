#ifndef STARTUPSCHEDULERDIALOG_H
#define STARTUPSCHEDULERDIALOG_H
#include "engine_global.h"

#include <QDialog>

namespace Ui {
class StartupSchedulerDialog;
}

class ENGINESHARED_EXPORT StartupSchedulerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartupSchedulerDialog(QWidget *parent = nullptr);
    ~StartupSchedulerDialog();
    bool GetNeedToStartScheduler();
    void SetNeedToStartScheduler(bool NeedToStartScheduler);

private slots:
    void on_pushButton_clicked();

private:
    Ui::StartupSchedulerDialog *ui;
};

#endif // STARTUPSCHEDULERDIALOG_H
