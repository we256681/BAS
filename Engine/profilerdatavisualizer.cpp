#include "profilerdatavisualizer.h"
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{

    ProfilerDataVisualizer::ProfilerDataVisualizer(QObject *parent) :
        IProfilerDataVisualizer(parent)
    {
        VisualizerDialog = 0;
    }

    void ProfilerDataVisualizer::SetProfilerData(IProfilerData* ProfilerData)
    {
        this->ProfilerData = ProfilerData;
    }

    void ProfilerDataVisualizer::DisableProfiler()
    {
        ProfilerData->SetIsProfilerEnabled(false);
        if(VisualizerDialog)
            VisualizerDialog->close();
    }



    void ProfilerDataVisualizer::ShowReport()
    {
        if(!ProfilerData->GetIsProfilerEnabled())
        {
            QMessageBox msgBox;
            msgBox.setText(QObject::tr("Do you want to enable the measurement of the actions execution time?"));
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            if(QMessageBox::Cancel == msgBox.exec())
            {
                return;
            }
            ProfilerData->SetIsProfilerEnabled(true);
        }

        if(VisualizerDialog)
        {
            VisualizerDialog->SetHtml(ProfilerData->GenerateReport());
            VisualizerDialog->raise();
        }else
        {
            VisualizerDialog = new ReportDataVisualizerDialog();
            QPushButton *Disable = new QPushButton(VisualizerDialog);
            connect(Disable,SIGNAL(clicked(bool)),this,SLOT(DisableProfiler()));
            Disable->setText(tr("Disable Measurement"));
            Disable->setIcon(QIcon(":/engine/images/stop.png"));
            Disable->setIconSize(QSize(18,18));
            VisualizerDialog->AddButton(Disable);
            VisualizerDialog->setWindowIcon(QIcon(":/engine/images/racing.png"));
            VisualizerDialog->setWindowTitle(tr("Actions Execution Time Report"));

            connect(VisualizerDialog,SIGNAL(Reload()),this,SLOT(ShowReport()));
            VisualizerDialog->SetHtml(ProfilerData->GenerateReport());
            VisualizerDialog->exec();
            delete VisualizerDialog;
            VisualizerDialog = 0;
        }
    }
}
