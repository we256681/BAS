#include "browserlistitem.h"
#include <QDebug>
#include <QSizePolicy>
#include "ui_browserlistitem.h"

BrowserListItem::BrowserListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BrowserListItem)
{
    ui->setupUi(this);
    IsManual = false;
    SetVisibilityInternal(false);
    setCursor(Qt::PointingHandCursor);

    ui->ManualControlIndicator->setPixmap(QIcon(":/engine/images/manual_control.png").pixmap(16,16));
    QSizePolicy p = ui->ManualControlIndicator->sizePolicy();
    p.setRetainSizeWhenHidden(true);
    ui->ManualControlIndicator->setSizePolicy(p);

    StopManualControl();
    Number = 0;
}

void BrowserListItem::mousePressEvent( QMouseEvent * event )
{
    SetVisibility(!IsVisible);
}

void BrowserListItem::SetNumber(int Number)
{
    this->Number = Number;
    ui->BrowserNumber->setText(QString(tr("Browser #%1")).arg(QString::number(Number + 1)));
}

int BrowserListItem::GetNumber()
{
    return Number;
}

void BrowserListItem::SetVisibilityInternal(bool IsVisible)
{
    this->IsVisible = IsVisible;
    QString VisibleText;
    if(IsVisible)
    {
        VisibleText = tr("<small><u>Hide browser</u></small>");
        ui->VisibilityIndicator->setPixmap(QIcon(":/engine/images/chrome_enable.png").pixmap(16,16));
        ui->LabelShowHide->setProperty("active", true);
        ui->BrowserNumber->setProperty("active", true);
    }else
    {
        VisibleText = tr("<small><u>Show browser</u></small>");
        ui->VisibilityIndicator->setPixmap(QIcon(":/engine/images/chrome_disable.png").pixmap(16,16));

        ui->LabelShowHide->setProperty("active", false);
        ui->BrowserNumber->setProperty("active", false);
    }

    ui->LabelShowHide->setText(VisibleText);
    ui->BrowserNumber->style()->unpolish(ui->BrowserNumber);
    ui->BrowserNumber->style()->polish(ui->BrowserNumber);
    ui->BrowserNumber->update();
    ui->LabelShowHide->style()->unpolish(ui->LabelShowHide);
    ui->LabelShowHide->style()->polish(ui->LabelShowHide);
    ui->LabelShowHide->update();
}


void BrowserListItem::SetVisibility(bool IsVisible)
{
    SetVisibilityInternal(IsVisible);
    emit Changed(IsVisible);
}

void BrowserListItem::StartManualControl(const QString& Message)
{
    IsManual = true;
    QString FullMessage = Message;
    if(!FullMessage.isEmpty())
        FullMessage += tr(". ");
    FullMessage += tr("Browser is under user contol. Click to open.");
    this->setToolTip(FullMessage);
    ui->ManualControlIndicator->show();
    this->setStyleSheet("#BrowserListItemBorder{background-color: #313131;border-radius:2px;border-color: #a5453d;border-width:1px;border-style:solid;}#BorderOut{border-radius:2px;border-color: #682b27;border-width:1px;border-style:solid;}*[active=false]{color:#ccc}*[active=true]{color:white}");
}

void BrowserListItem::StopManualControl()
{
    IsManual = false;
    this->setToolTip(QString());
    ui->ManualControlIndicator->hide();
    this->setStyleSheet("#BrowserListItemBorder{background-color: #313131;border-radius:2px;border-color: #494949;border-width:1px;border-style:solid;}#BorderOut{border-radius:2px;border-color: #2a2a2a;border-width:1px;border-style:solid;}*[active=false]{color:#ccc}*[active=true]{color:white}");
}

bool BrowserListItem::GetIsManual()
{
    return IsManual;
}

bool BrowserListItem::GetIsVisible()
{
    return IsVisible;
}

BrowserListItem::~BrowserListItem()
{
    delete ui;
}

