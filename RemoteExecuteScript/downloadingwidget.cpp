#include "downloadingwidget.h"
#include "ui_downloadingwidget.h"
#include "windows.h"

DownloadingWidget::DownloadingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadingWidget)
{
    ui->setupUi(this);
    ui->LabelError->setVisible(false);

}

void DownloadingWidget::Show()
{
    show();

    SwitchToThisWindow((HWND)this->winId(),true);
    HWND hCurWnd = ::GetForegroundWindow();
    DWORD dwMyID = ::GetCurrentThreadId();
    DWORD dwCurID = ::GetWindowThreadProcessId(hCurWnd, NULL);
    ::AttachThreadInput(dwCurID, dwMyID, TRUE);
    ::SetWindowPos((HWND)this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    ::SetWindowPos((HWND)this->winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    ::SetForegroundWindow((HWND)this->winId());
    ::AttachThreadInput(dwCurID, dwMyID, FALSE);
    ::SetFocus((HWND)this->winId());
    ::SetActiveWindow((HWND)this->winId());

}

DownloadingWidget::~DownloadingWidget()
{
    delete ui;
}


void DownloadingWidget::SetProgress(int Progress)
{
    ui->progressBar->setValue(Progress);
}

void DownloadingWidget::SetError(const QString& Error)
{
    ui->LabelError->setVisible(true);
    ui->progressBar->setVisible(false);
    ui->LabelError->setText(Error);
    ui->label->setVisible(false);
    ui->SkipButton->setText("Close");
    Show();

}

void DownloadingWidget::on_SkipButton_clicked()
{
    emit Cancel();
}
