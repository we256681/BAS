#include "webdriverlogfilter.h"
#include "ui_webdriverlogfilter.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    WebDriverLogFilter::WebDriverLogFilter(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::WebDriverLogFilter)
    {
        ui->setupUi(this);
        setWindowTitle(tr("Multilogin Log filter"));
        ui->ThreadIndex->setVisible(false);
        ui->label->setVisible(false);
        ui->label_5->setVisible(false);
        ui->Fail->setVisible(false);
        AvailableLevels = QList<LogLevel>() << LogFail << LogSuccess << LogUser << LogInfo;
        SetThreadsNumber(0);
        PrevThreadNumber = 0;
        IsActive = false;
    }

    void WebDriverLogFilter::GenerateFilterChangedEvents()
    {
        QList<LogLevel> Levels;
        if(AvailableLevels.contains(LogFail) && ui->Fail->isChecked())
            Levels.append(LogFail);
        if(AvailableLevels.contains(LogSuccess) && ui->Success->isChecked())
            Levels.append(LogSuccess);
        if(AvailableLevels.contains(LogInfo) && ui->Info->isChecked())
            Levels.append(LogInfo);
        if(AvailableLevels.contains(LogUser) && ui->User->isChecked())
            Levels.append(LogUser);

        emit FilterChanged(Levels, ui->ThreadIndex->value(), ui->FilterEdit->text(), IsActive);

    }

    void WebDriverLogFilter::SetThreadsNumber(int Number)
    {
        if(Number < PrevThreadNumber && Number)
            Number = PrevThreadNumber;

        ui->ThreadIndex->setValue(0);
        ui->ThreadIndex->setMaximum(Number);

        PrevThreadNumber = Number;
    }

    void WebDriverLogFilter::Clear()
    {
        ClearInternal();
        IsActive = false;
        PrevThreadNumber = 0;
        GenerateFilterChangedEvents();
    }

    void WebDriverLogFilter::ClearInternal()
    {
        ui->ThreadIndex->setValue(0);

        ui->FilterEdit->clear();

        ui->Success->setVisible(AvailableLevels.contains(LogSuccess));
        ui->User->setVisible(AvailableLevels.contains(LogUser));
        ui->Info->setVisible(AvailableLevels.contains(LogInfo));

        ui->Success->setChecked(AvailableLevels.contains(LogSuccess));
        ui->User->setChecked(AvailableLevels.contains(LogUser));
        ui->Info->setChecked(AvailableLevels.contains(LogInfo));
    }

    void WebDriverLogFilter::SetAvailableLevels(QList<LogLevel> AvailableLevels)
    {
        this->AvailableLevels = AvailableLevels;
    }


    WebDriverLogFilter::~WebDriverLogFilter()
    {
        delete ui;
    }

    void WebDriverLogFilter::on_ClearLog_clicked()
    {
        ClearInternal();
    }

    void WebDriverLogFilter::on_EnableFilter_clicked()
    {
        IsActive = true;
        GenerateFilterChangedEvents();
        accept();
    }

    void WebDriverLogFilter::on_DisableFilter_clicked()
    {
        IsActive = false;
        GenerateFilterChangedEvents();
        reject();
    }

    void WebDriverLogFilter::on_ClearFilter_clicked()
    {
        ui->FilterEdit->clear();
    }


}


