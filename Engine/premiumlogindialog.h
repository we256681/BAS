#ifndef PREMIUMLOGINDIALOG_H
#define PREMIUMLOGINDIALOG_H

#include <QDialog>

namespace Ui {
class PremiumLoginDialog;
}

class PremiumLoginDialog : public QDialog
{
    Q_OBJECT
    bool UseWebInterface;

public:
    explicit PremiumLoginDialog(QWidget *parent = 0);
    ~PremiumLoginDialog();
    bool GetUsesWebInterface();
    QString GetPremiumLogin();
    QString GetPremiumPassword();
    void SetPremiumLogin(const QString& Login);
    void SetPremiumPassword(const QString& Password);

private slots:
    void on_Ok_clicked();

    void on_Cancel_clicked();

private:
    Ui::PremiumLoginDialog *ui;
};

#endif // PREMIUMLOGINDIALOG_H
