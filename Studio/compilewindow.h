#ifndef COMPILEWINDOW_H
#define COMPILEWINDOW_H

#include <QDialog>
#include <QTimer>
#include "compiler.h"
#include "httpclient.h"
namespace Ui {
class CompileWindow;
}

using namespace BrowserAutomationStudioFramework;

class CompileWindow : public QDialog
{
    Q_OBJECT
    Compiler *compiler;
    QByteArray LastIcon;
    bool IsDefaultIcon;
    QTimer *TimerValidate;
    bool IsDirty;
    bool IsValidating;
    bool IsOverrideRemote;

public:
    explicit CompileWindow(Compiler *compiler,QWidget *parent = 0);
    void GuiUpdate();
    bool GetIsOverrideRemote();
    ~CompileWindow();

private:
    Ui::CompileWindow *ui;
private slots:
    void Accepted();
    void Resize();
    void on_NoProtection_clicked();
    void on_PrivateScriptUserEnterPass_clicked();
    void on_PrivateScriptEnterPassForUser_clicked();
    void on_ApplicationIcon_clicked();
    void UpdateIcon(const QByteArray& Data);
    void SetDefaultIcon();
    void Validate();
    void ValidateResult();
    void ValidateTimer();
    void SliderUpdate();


    void on_lineEdit_textChanged(const QString &arg1);
    void on_Ok_clicked();
    void on_Cancel_clicked();
};

#endif // COMPILEWINDOW_H
