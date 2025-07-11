#ifndef COMPILERESULTS_H
#define COMPILERESULTS_H

#include <QDialog>
#include "compiler.h"
#include "httpclientfactory.h"


namespace Ui {
class CompileResults;
}

using namespace BrowserAutomationStudioFramework;


class CompileResults : public QDialog
{
    Q_OBJECT
    Compiler *_Compiler;
    QString CurrentProject;
    HttpClientFactory *_HttpClientFactory;
    enum
    {
        Initial,
        Finished,
        Error
    }Stage;
    QString PremiumUsername;
    QString PremiumPassword;
public:
    explicit CompileResults(QWidget *parent = nullptr);
    void Init(Compiler* _Compiler, const QString& CurrentProject, bool IsOverrideRemote, const QString& PremiumUsername, const QString& PremiumPassword);
    ~CompileResults();
    QString GetPremiumUsername();
    QString GetPremiumPassword();

private:
    Ui::CompileResults *ui;

signals:
    void CreateArchive();
    void SaveCurrentProject();
private slots:
    void on_Cancel_clicked();
    void on_Ok_clicked();
    void Disable();
    void Enable();
    void Finish();
    void FailToConnectToServer();
    void Submit();
    void HttpResponce();
    void OpenCompiledProject();
};

#endif // COMPILERESULTS_H
