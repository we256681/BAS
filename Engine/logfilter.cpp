#include "logfilter.h"
#include "ui_logfilter.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    LogFilter::LogFilter(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::LogFilter)
    {
        ui->setupUi(this);
        setWindowTitle(tr("Log filter"));
        AvailableLevels = QList<LogLevel>() << LogFail << LogSuccess << LogUser << LogInfo;
        SetThreadsNumber(0);
        PrevThreadNumber = 0;
        IsActive = false;
    }

    void LogFilter::GenerateFilterChangedEvents()
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

    void LogFilter::SetThreadsNumber(int Number)
    {
        if(Number < PrevThreadNumber && Number)
            Number = PrevThreadNumber;

        ui->ThreadIndex->setValue(0);
        ui->ThreadIndex->setMaximum(Number);

        PrevThreadNumber = Number;
    }

    void LogFilter::Clear()
    {
        ClearInternal();
        IsActive = false;
        PrevThreadNumber = 0;
        GenerateFilterChangedEvents();
    }

    void LogFilter::ClearInternal()
    {
        ui->ThreadIndex->setValue(0);

        ui->FilterEdit->clear();

        ui->Fail->setVisible(AvailableLevels.contains(LogFail));
        ui->Success->setVisible(AvailableLevels.contains(LogSuccess));
        ui->User->setVisible(AvailableLevels.contains(LogUser));
        ui->Info->setVisible(AvailableLevels.contains(LogInfo));

        ui->Fail->setChecked(AvailableLevels.contains(LogFail));
        ui->Success->setChecked(AvailableLevels.contains(LogSuccess));
        ui->User->setChecked(AvailableLevels.contains(LogUser));
        ui->Info->setChecked(AvailableLevels.contains(LogInfo));
    }

    void LogFilter::SetAvailableLevels(QList<LogLevel> AvailableLevels)
    {
        this->AvailableLevels = AvailableLevels;
    }


    LogFilter::~LogFilter()
    {
        delete ui;
    }

    void LogFilter::on_ClearLog_clicked()
    {
        ClearInternal();
    }

    void LogFilter::on_EnableFilter_clicked()
    {
        IsActive = true;
        GenerateFilterChangedEvents();
        accept();
    }

    void LogFilter::on_DisableFilter_clicked()
    {
        IsActive = false;
        GenerateFilterChangedEvents();
        reject();
    }

    void LogFilter::on_ClearFilter_clicked()
    {
        ui->FilterEdit->clear();
    }


}


