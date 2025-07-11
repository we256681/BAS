#ifndef ASKIFRUNWITHWEBINTERFACEDIALOG_H
#define ASKIFRUNWITHWEBINTERFACEDIALOG_H

#include <QDialog>

namespace Ui {
class AskIfRunWithWebInterfaceDialog;
}

class AskIfRunWithWebInterfaceDialog : public QDialog
{
    Q_OBJECT
    bool UseWebInterface;
public:
    explicit AskIfRunWithWebInterfaceDialog(QWidget *parent = 0);
    ~AskIfRunWithWebInterfaceDialog();
    void SetScriptName(const QString& ScriptName);
    bool GetUsesWebInterface();
    bool GetRememberChoice();


private slots:
    void on_RunWithWebInterfaceButton_clicked();

    void on_RunWithNormalInterfaceButton_clicked();

private:
    Ui::AskIfRunWithWebInterfaceDialog *ui;
};

#endif // ASKIFRUNWITHWEBINTERFACEDIALOG_H
