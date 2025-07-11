#ifndef EMBEDDEDLANGUAGEINSTALLDIALOG_H
#define EMBEDDEDLANGUAGEINSTALLDIALOG_H

#include <QDialog>

namespace Ui {
class EmbeddedLanguageInstallDialog;
}

class EmbeddedLanguageInstallDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmbeddedLanguageInstallDialog(QWidget *parent = 0);
    ~EmbeddedLanguageInstallDialog();
    void SetIsError();
    void SetLabel(const QString& Label);
    void SetTitle(const QString& Title);
    void closeEvent(QCloseEvent *event);

private:
    Ui::EmbeddedLanguageInstallDialog *ui;
    void keyPressEvent(QKeyEvent *e);

signals:
    void RunWithoutEmbeddedLanguages();
    void Close();
private slots:
    void on_ShowLog_clicked();
};

#endif // EMBEDDEDLANGUAGEINSTALLDIALOG_H
