#include "recentswidget.h"
#include "ui_recentswidget.h"
#include <QSettings>
#include <QByteArray>
#include <QLabel>
#include <QFileInfo>
#include <QWidget>
#include <QDebug>
#include <QDesktopWidget>
#include <QPushButton>

RecentsWidget::RecentsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecentsWidget)
{
    ui->setupUi(this);
}

void RecentsWidget::Reload(const QString& CurrentFile)
{
    QLayoutItem* item;
    while ( ( item = ui->Container->layout()->takeAt( 0 ) ) != NULL )
    {
        delete item->widget();
        delete item;
    }

    {
        QLabel * Label = new QLabel(this);

        Label->setText(QString("<div style='margin-left:10px;'>") + tr("Current project") + QString(":</div>"));
        ui->Container->layout()->addWidget(Label);
    }

    {
        QWidget * Widget = new QWidget(this);
        Widget->setLayout(new QHBoxLayout());

        QLabel * Label = new QLabel(this);

        QString FileName = QFileInfo(CurrentFile).fileName();
        QString FilePath = CurrentFile;
        int max_lent = 100;
        if(FileName.length()>max_lent)
            FileName = FileName.left(max_lent - 3) + "...";
        if(FilePath.length()>max_lent)
            FilePath = FilePath.left(max_lent - 3) + "...";

        Label->setText(QString("<html><head/><body><table style=\"margin-bottom:10px;margin-top:10px\"><tr><td><div>&nbsp;&nbsp;<span style=\" text-decoration: underline; color:white\">%1</span></div></td></tr><tr><td><div>&nbsp;&nbsp;<span style=\"color:gray;\">%2</span></div></td></tr></table></body></html>").arg(FileName).arg(FilePath));
        Label->setToolTip(CurrentFile);

        QPushButton * Button = new QPushButton(this);
        Button->setMinimumWidth(32);
        Button->setMaximumWidth(32);
        Button->setMinimumHeight(32);
        Button->setMaximumHeight(32);
        Button->setIconSize(QSize(32,32));
        Button->setStyleSheet(QString("QPushButton {border: 0px;}"));
        Button->setIcon(QIcon(":/images/current.png"));
        Button->setFlat(true);
        Button->setToolTip(CurrentFile);

        Widget->layout()->addItem(new QSpacerItem(25, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));
        Widget->layout()->addWidget(Button);
        Widget->layout()->addWidget(Label);
        Widget->layout()->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
        Widget->layout()->setMargin(0);
        Widget->layout()->setSpacing(0);

        ui->Container->layout()->addWidget(Widget);

        /*QLabel * Label = new QLabel(this);

        Label->setProperty("file",QVariant(CurrentFile));

        QString FileName = QFileInfo(CurrentFile).fileName();
        QString FilePath = CurrentFile;
        int max_lent = 100;
        if(FileName.length()>max_lent)
            FileName = FileName.left(max_lent - 3) + "...";
        if(FilePath.length()>max_lent)
            FilePath = FilePath.left(max_lent - 3) + "...";
        Label->setText(QString("<html><head/><body><span style=\" text-decoration: none; \"> <table style=\"margin-left:25px; margin-bottom:10px; margin-top:10px\"><tr><td rowspan=\"2\"><img src=\"://images/current.png\"/></td><td><div>&nbsp;&nbsp;<span style=\" text-decoration: underline; color:white\">%1</span></div></td></tr><tr><td><div>&nbsp;&nbsp;<span style=\"color:gray;\">%2</span></div></td></tr></table></span></body></html>").arg(FileName).arg(FilePath));
        Label->setToolTip(CurrentFile);
        Label->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->Container->layout()->addWidget(Label);*/
    }
    QSettings Settings("settings.ini",QSettings::IniFormat);
    QString RecentProjects = Settings.value("RecentProjects",QString()).toString();
    QStringList List = RecentProjects.split(",");
    List.removeDuplicates();
    int it = 0;
    for(QString RecentEncoded:List)
    {
        QString Recent = QString::fromUtf8(QByteArray::fromBase64(RecentEncoded.toUtf8()));
        if(Recent == CurrentFile)
        {
            continue;
        }
        if(QFileInfo(Recent).exists())
        {
            it++;
            break;
        }
    }

    if(it > 0)
    {
        QLabel * Label = new QLabel(this);

        Label->setText(QString("<div style='margin-left:10px;'>") + tr("Recent projects") + QString(":</div>"));
        ui->Container->layout()->addWidget(Label);
    }

    it = 0;
    int max_it = 5;

    QRect rec = QApplication::desktop()->screenGeometry();

    int screen_height = rec.height();

    if(screen_height < 905 - 60 * 3)
    {
        max_it = 1;
    }else if(screen_height < 905 - 60 * 2)
    {
        max_it = 2;
    }else if(screen_height < 905 - 60 * 1)
    {
        max_it = 3;
    }else if(screen_height < 905)
    {
        max_it = 4;
    }

    for(QString RecentEncoded:List)
    {
        if(it>=max_it)
            break;
        QString Recent = QString::fromUtf8(QByteArray::fromBase64(RecentEncoded.toUtf8()));
        if(Recent == CurrentFile)
        {
            continue;
        }
        if(QFileInfo(Recent).exists())
        {
            QWidget * Widget = new QWidget(this);
            Widget->setLayout(new QHBoxLayout());

            QLabel * Label = new QLabel(this);
            Label->setProperty("file",QVariant(Recent));
            connect(Label,SIGNAL(linkActivated(QString)),this,SLOT(ButtonClicked()));
            QString FileName = QFileInfo(Recent).fileName();
            QString FilePath = Recent;
            int max_lent = 100;
            if(FileName.length()>max_lent)
                FileName = FileName.left(max_lent - 3) + "...";
            if(FilePath.length()>max_lent)
                FilePath = FilePath.left(max_lent - 3) + "...";
            Label->setText(QString("<html><head/><body><a href=\"bas://loadproject\" style=\"text-decoration: none;\"> <table style=\"margin-bottom:10px;margin-top:10px\"><tr><td><div>&nbsp;&nbsp;<span style=\" text-decoration: underline; color:white\">%1</span></div></td></tr><tr><td><div>&nbsp;&nbsp;<span style=\"color:gray;\">%2</span></div></td></tr></table></a></body></html>").arg(FileName).arg(FilePath));
            Label->setToolTip(tr("Open") + QString(" ") + Recent);

            QPushButton * Button = new QPushButton(this);
            Button->setProperty("file",QVariant(Recent));
            connect(Button,SIGNAL(clicked(bool)),this,SLOT(ButtonClicked()));
            Button->setMinimumWidth(32);
            Button->setMaximumWidth(32);
            Button->setMinimumHeight(32);
            Button->setMaximumHeight(32);
            Button->setIconSize(QSize(32,32));
            Button->setStyleSheet(QString("QPushButton {border: 0px;}"));
            Button->setIcon(QIcon(":/studio/images/open.png"));
            Button->setCursor(Qt::PointingHandCursor);
            Button->setFlat(true);
            Button->setToolTip(tr("Open") + QString(" ") + Recent);

            Widget->layout()->addItem(new QSpacerItem(25, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));
            Widget->layout()->addWidget(Button);
            Widget->layout()->addWidget(Label);
            Widget->layout()->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
            Widget->layout()->setMargin(0);
            Widget->layout()->setSpacing(0);

            ui->Container->layout()->addWidget(Widget);
            it++;
        }

    }

}

void RecentsWidget::ButtonClicked()
{
    QString File = sender()->property("file").toString();
    emit FileSelected(File);
}

void RecentsWidget::OpenFile(const QString& File)
{
    if(File.isEmpty())
        return;
    QSettings Settings("settings.ini",QSettings::IniFormat);

    QString RecentProjects = Settings.value("RecentProjects",QString()).toString();
    QStringList List = RecentProjects.split(",");
    QString FileEncrypted = QString::fromUtf8(File.toUtf8().toBase64());
    List.removeAll(FileEncrypted);
    List.push_front(FileEncrypted);
    List = List.mid(0,20);
    Settings.setValue("RecentProjects",List.join(","));
}

RecentsWidget::~RecentsWidget()
{
    delete ui;
}
