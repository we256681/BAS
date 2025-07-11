#ifndef MODULEINFODIALOG_H
#define MODULEINFODIALOG_H

#include <QDialog>

namespace Ui {
class ModuleInfoDialog;
}

class ModuleInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModuleInfoDialog(QWidget *parent = nullptr);
    ~ModuleInfoDialog();
    bool Init(const QString& ModuleName, const QString& Folder);


private slots:
    void on_pushButton_clicked();

private:
    Ui::ModuleInfoDialog *ui;
};

#endif // MODULEINFODIALOG_H
