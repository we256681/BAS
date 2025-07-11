#include "subprocesslistwidget.h"
#include "ui_subprocesslistwidget.h"
#include <QLabel>
#include <QXmlStreamReader>
#include "every_cpp.h"


namespace BrowserAutomationStudioFramework
{
    SubprocessListWidget::SubprocessListWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::SubprocessListWidget)
    {
        ui->setupUi(this);

        AutoStart = false;
        BrowsersHidden = false;

        Layout = new FlowLayout(ui->MainWidget,0,0,0);
        ui->MainWidget->setLayout(Layout);
    }

    void SubprocessListWidget::UpdateItemPosition(BrowserListItem *Item)
    {
        int Index = Layout->indexOf(Item);
        int NewIndex = -1;
        if(Index < 0)
            return;

        int Number = Item->GetNumber();
        bool Manual = Item->GetIsManual();

        int I = 0;
        bool PrevManual = true;
        int PrevNumber = -1;
        while(true)
        {
            QLayoutItem * CurrentItem = Layout->itemAt(I);
            if(!CurrentItem)
                break;

            BrowserListItem * CurrentWidget = qobject_cast<BrowserListItem *>(CurrentItem->widget());
            if(CurrentWidget == Item)
            {
                I++;
                continue;
            }
            if(!CurrentWidget)
            {
                I++;
                continue;
            }

            int CurrentNumber = CurrentWidget->GetNumber();
            bool CurrentManual = CurrentWidget->GetIsManual();

            if(Manual)
            {
                if(PrevManual && !CurrentManual)
                {
                    NewIndex = I;
                    break;
                }
            }else
            {
                if(
                        (!PrevManual && !CurrentManual && Number > PrevNumber && Number <= CurrentNumber)
                        ||
                        (PrevManual && !CurrentManual && Number <= CurrentNumber)
                  )
                {
                    NewIndex = I;
                    break;
                }
            }

            PrevManual = CurrentManual;
            PrevNumber = CurrentNumber;
            I++;
        }



        if(NewIndex >= 0)
        {
            if(NewIndex >= Index)
            {
                NewIndex--;
            }

            if(Index != NewIndex)
            {

                QLayoutItem * i = Layout->takeAt(Index);
                Layout->insertAt(i,NewIndex);
            }

        }else
        {
            QLayoutItem * i = Layout->takeAt(Index);
            Layout->addItem(i);
        }

    }

    void SubprocessListWidget::SetAutoStart(bool AutoStart)
    {
        this->AutoStart = AutoStart;
    }

    void SubprocessListWidget::SetBrowsersHidden(bool BrowsersHidden)
    {
        this->BrowsersHidden = BrowsersHidden;
        if(BrowsersHidden)
        {
            this->hide();
        }
    }

    void SubprocessListWidget::Clear()
    {
        QLayoutItem* item;
        while ( ( item = Layout->takeAt( 0 ) ) != 0 )
        {
            delete item->widget();
            delete item;
        }
        Data.clear();
        Indexes.clear();
        this->disconnect();

    }

    void SubprocessListWidget::CommunicatorDestroyed()
    {
        IProcessComunicator* Communicator = qobject_cast<IProcessComunicator*>(sender());
        if(!Communicator)
            return;
        QList<IProcessComunicator*> values = Data.values();

        int index = values.indexOf(Communicator);
        if(index >= 0)
        {
            BrowserListItem * Checkbox = Data.key(values.at(index));
            Checkbox->StopManualControl();
            if(!AutoStart)
                Checkbox->SetVisibility(false);
            Checkbox->hide();
            Data[Checkbox] = 0;
            UpdateItemPosition(Checkbox);
        }
    }

    void SubprocessListWidget::Add(IProcessComunicator* Communicator, int index)
    {
        BrowserListItem *Box = 0;
        connect(Communicator,SIGNAL(Received(QString)),this,SLOT(Received(QString)));
        connect(Communicator,SIGNAL(OnSuspend()),this,SLOT(CommunicatorDestroyed()));
        if(Indexes.contains(index))
        {
            Box = Indexes[index];
            Box->show();
            Data[Box] = Communicator;
            if(Box->GetIsVisible())
            {
                 Communicator->OnShowBrowser();
                 Communicator->Send("<Visible>1</Visible>");
            }
            Box->StopManualControl();
        }else
        {
            Box = new BrowserListItem(ui->MainWidget);
            if(BrowsersHidden)
            {
                Box->hide();
            }
            Data.insert(Box,Communicator);
            Indexes.insert(index,Box);
            Box->SetNumber(index);
            Layout->addWidget(Box);
            connect(Box,SIGNAL(Changed(bool)),this,SLOT(ChangedCheckState(bool)));
        }
        UpdateItemPosition(Box);
        if(AutoStart)
            Box->SetVisibility(true);
    }

    void SubprocessListWidget::Received(const QString & Message)
    {

        QXmlStreamReader xmlReader(Message);

        while(!xmlReader.atEnd() && !xmlReader.hasError())
        {
            QXmlStreamReader::TokenType token = xmlReader.readNext();

            if(xmlReader.name() == "Minimized" && token == QXmlStreamReader::StartElement)
            {
                IProcessComunicator *Comunicator = qobject_cast<IProcessComunicator *>(sender());
                BrowserListItem *box = Data.key(Comunicator);
                if(box)
                {
                    box->SetVisibility(false);
                }
            }

            if(xmlReader.name() == "ManualControlIndicatorStart" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                IProcessComunicator *Comunicator = qobject_cast<IProcessComunicator *>(sender());
                BrowserListItem *box = Data.key(Comunicator);
                if(box)
                {
                    box->StartManualControl(xmlReader.text().toString());
                    UpdateItemPosition(box);
                }
            }
            if(xmlReader.name() == "ManualControlIndicatorStop" && token == QXmlStreamReader::StartElement)
            {
                IProcessComunicator *Comunicator = qobject_cast<IProcessComunicator *>(sender());
                BrowserListItem *box = Data.key(Comunicator);
                if(box)
                {
                    box->StopManualControl();
                    UpdateItemPosition(box);
                }
            }
        }

        if (xmlReader.hasError())
        {
            return;
        }
    }

    void SubprocessListWidget::ChangedCheckState(bool state)
    {
        IProcessComunicator * Communicator = Data[qobject_cast<BrowserListItem *>(sender())];
        if(Communicator)
        {
            if(state)
            {
                Communicator->OnShowBrowser();
            }else
            {
                Communicator->OnHideBrowser();
            }
            Communicator->Send(QString("<Visible>%1</Visible>").arg(QString::number(state)));
        }
    }

    SubprocessListWidget::~SubprocessListWidget()
    {
        delete ui;
    }

    void SubprocessListWidget::changeEvent(QEvent *e)
    {
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
        }
    }

}
