#include "chooserresourcewidget.h"
#include "ui_chooserresourcewidget.h"
#include <QFileDialog>
#include <QToolBar>
#include "every_cpp.h"

#include "toprightpositioner.h"

ChooserResourceWidget::ChooserResourceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChooserResourceWidget), StringBuilder(0), LastTableId(-1)
{
    ui->setupUi(this);

    AvailableTypes.append("page");
    AvailableTypes.append("page_2");
    AvailableTypes.append("page_4");
    AvailableTypes.append("page_5");
    AvailableTypes.append("page_6");
    AvailableTypes.append("page_3");
    AvailableTypes.append("page_7");
    AvailableTypes.append("page_8");
    AvailableTypes.append("page_9");
}

void ChooserResourceWidget::Render()
{

    SetupUiCustom();
    //RenderAdvancedText();

    DatabaseIndexesChangedInAction = false;

    if(AvailableTypes.contains("page_9"))
    {
        MultiWidget = new QxtCheckComboBox();
        MultiWidget->setObjectName("DatabaseGroups");
        MultiWidget->setParent(page_9);
        MultiWidget->setDefaultText(tr("---"));
        MultiWidget->setMaximumWidth(252);
        MultiWidget->setMinimumWidth(252);
        ComboLayout->addWidget(MultiWidget);
    }

    if(AvailableTypes.contains("page"))
        Widgets.append(page);

    if(AvailableTypes.contains("page_2"))
        Widgets.append(page_2);

    if(AvailableTypes.contains("page_4"))
        Widgets.append(page_4);

    if(AvailableTypes.contains("page_5"))
        Widgets.append(page_5);

    if(AvailableTypes.contains("page_6"))
        Widgets.append(page_6);

    if(AvailableTypes.contains("page_3"))
        Widgets.append(page_3);

    if(AvailableTypes.contains("page_7"))
        Widgets.append(page_7);

    if(AvailableTypes.contains("page_8"))
        Widgets.append(page_8);

    if(AvailableTypes.contains("page_9"))
        Widgets.append(page_9);

    if(AvailableTypes.contains("page_8"))
        DirectoryReload_toggled(DirectoryReload->isChecked());

    if(AvailableTypes.contains("page_3"))
        FileReload_toggled(FileReload->isChecked());

    if(AvailableTypes.contains("page_7"))
        UrlReload_toggled(UrlReload->isChecked());

    if(AvailableTypes.contains("page_8"))
        DirectoryRenew_toggled(DirectoryRenew->isChecked());

    if(AvailableTypes.contains("page_3"))
        FileRenew_toggled(FileRenew->isChecked());

    if(AvailableTypes.contains("page_7"))
        UrlRenew_toggled(UrlRenew->isChecked());

    if(AvailableTypes.contains("page_9"))
    {
        DatabaseReload_toggled(DatabaseReload->isChecked());
        DatabaseRenew_toggled(DatabaseRenew->isChecked());
    }
    ShowValidation = false;

    if(AvailableTypes.contains("page"))
        connect(FixedStringValue,SIGNAL(textChanged(QString)),this,SLOT(Validate()));

    if(AvailableTypes.contains("page_3"))
        connect(FileFilename,SIGNAL(textChanged(QString)),this,SLOT(Validate()));

    if(AvailableTypes.contains("page_7"))
        connect(UrlUrl,SIGNAL(textChanged(QString)),this,SLOT(Validate()));

    if(AvailableTypes.contains("page_8"))
        connect(DirectoryDirectory,SIGNAL(textChanged(QString)),this,SLOT(Validate()));

    if(AvailableTypes.contains("page_3"))
        connect(FileRead,SIGNAL(toggled(bool)),this,SLOT(Validate()));

    if(AvailableTypes.contains("page_3"))
        connect(FileWrite,SIGNAL(toggled(bool)),this,SLOT(Validate()));

    if(AvailableTypes.contains("page_9"))
        connect(MultiWidget,SIGNAL(checkedIndexesChanged(QList<int>,QList<int>)),this,SLOT(DatabaseIndexesChanged(QList<int>,QList<int>)));

    if(AvailableTypes.contains("page_3"))
        connect(ButtonFileSelect,SIGNAL(clicked(bool)),this,SLOT(ButtonFileSelect_clicked()));

    if(AvailableTypes.contains("page_8"))
        connect(ButtonDirectorySelect,SIGNAL(clicked(bool)),this,SLOT(ButtonDirectorySelect_clicked()));

    if(AvailableTypes.contains("page_4"))
        connect(RandomStringEdit,SIGNAL(clicked(bool)),this,SLOT(RandomStringEdit_clicked()));

    if(AvailableTypes.contains("page_8"))
        connect(DirectoryReload,SIGNAL(toggled(bool)),this,SLOT(DirectoryReload_toggled(bool)));

    if(AvailableTypes.contains("page_3"))
        connect(FileReload,SIGNAL(toggled(bool)),this,SLOT(FileReload_toggled(bool)));

    if(AvailableTypes.contains("page_7"))
        connect(UrlReload,SIGNAL(toggled(bool)),this,SLOT(UrlReload_toggled(bool)));

    if(AvailableTypes.contains("page_3"))
        connect(FileRenew,SIGNAL(toggled(bool)),this,SLOT(FileRenew_toggled(bool)));

    if(AvailableTypes.contains("page_7"))
        connect(UrlRenew,SIGNAL(toggled(bool)),this,SLOT(UrlRenew_toggled(bool)));

    if(AvailableTypes.contains("page_8"))
        connect(DirectoryRenew,SIGNAL(toggled(bool)),this,SLOT(DirectoryRenew_toggled(bool)));

    if(AvailableTypes.contains("page_9"))
    {
        connect(DatabaseReload,SIGNAL(toggled(bool)),this,SLOT(DatabaseReload_toggled(bool)));
        connect(DatabaseRenew,SIGNAL(toggled(bool)),this,SLOT(DatabaseRenew_toggled(bool)));
    }

    int MarginTop = 1;
    int MarginRight = 1;

    TopRightPositioner *Pos;

    int ValidationPixmapHeight = 22;
    int ValidationPixmapWidth = 17;

    /*ValidationPixmapWidth = LabelValidation1->pixmap()->width();
    ValidationPixmapHeight = FixedStringValue->sizeHint().height()-2;*/

    if(AvailableTypes.contains("page"))
        LayoutValidation1->removeWidget(LabelValidation1);

    if(AvailableTypes.contains("page_3"))
        horizontalLayout_6->removeWidget(LabelValidation2);

    if(AvailableTypes.contains("page_7"))
        horizontalLayout_9->removeWidget(LabelValidation3);

    if(AvailableTypes.contains("page_8"))
        horizontalLayout_10->removeWidget(LabelValidation4);

    if(AvailableTypes.contains("page"))
    {
        LabelValidation1->setPixmap(LabelValidation1->pixmap()->scaled(ValidationPixmapWidth,ValidationPixmapHeight));
        LabelValidation1->setMaximumHeight(ValidationPixmapHeight);
        LabelValidation1->setMinimumHeight(ValidationPixmapHeight);
        LabelValidation1->setMaximumWidth(ValidationPixmapWidth);
        LabelValidation1->setMinimumWidth(ValidationPixmapWidth);
    }

    if(AvailableTypes.contains("page_3"))
    {
        LabelValidation2->setPixmap(LabelValidation2->pixmap()->scaled(ValidationPixmapWidth,ValidationPixmapHeight));
        LabelValidation2->setMaximumHeight(ValidationPixmapHeight);
        LabelValidation2->setMinimumHeight(ValidationPixmapHeight);
        LabelValidation2->setMaximumWidth(ValidationPixmapWidth);
        LabelValidation2->setMinimumWidth(ValidationPixmapWidth);
    }

    if(AvailableTypes.contains("page_7"))
    {
        LabelValidation3->setPixmap(LabelValidation3->pixmap()->scaled(ValidationPixmapWidth,ValidationPixmapHeight));
        LabelValidation3->setMaximumHeight(ValidationPixmapHeight);
        LabelValidation3->setMinimumHeight(ValidationPixmapHeight);
        LabelValidation3->setMaximumWidth(ValidationPixmapWidth);
        LabelValidation3->setMinimumWidth(ValidationPixmapWidth);
    }

    if(AvailableTypes.contains("page_8"))
    {
        LabelValidation4->setPixmap(LabelValidation4->pixmap()->scaled(ValidationPixmapWidth,ValidationPixmapHeight));
        LabelValidation4->setMaximumHeight(ValidationPixmapHeight);
        LabelValidation4->setMinimumHeight(ValidationPixmapHeight);
        LabelValidation4->setMaximumWidth(ValidationPixmapWidth);
        LabelValidation4->setMinimumWidth(ValidationPixmapWidth);
    }

    if(AvailableTypes.contains("page_9"))
    {
        LabelValidation5->setPixmap(LabelValidation5->pixmap()->scaled(ValidationPixmapWidth,ValidationPixmapHeight));
        LabelValidation5->setMaximumHeight(ValidationPixmapHeight);
        LabelValidation5->setMinimumHeight(ValidationPixmapHeight);
        LabelValidation5->setMaximumWidth(ValidationPixmapWidth);
        LabelValidation5->setMinimumWidth(ValidationPixmapWidth);
    }


    if(AvailableTypes.contains("page"))
    {
        Pos = new TopRightPositioner(this);
        LabelValidation1->setVisible(false);
        Pos->SetChild(LabelValidation1);
        Pos->SetParent(FixedStringValue);
        Pos->SetMarginRight(MarginRight);
        Pos->SetMarginTop(MarginTop);
        Pos->Start();
    }


    if(AvailableTypes.contains("page_3"))
    {
        Pos = new TopRightPositioner(this);
        LabelValidation2->setVisible(false);
        Pos->SetChild(LabelValidation2);
        Pos->SetParent(FileFilename);
        Pos->SetMarginRight(MarginRight);
        Pos->SetMarginTop(MarginTop);
        Pos->Start();
    }

    if(AvailableTypes.contains("page_8"))
    {
        Pos = new TopRightPositioner(this);
        LabelValidation4->setVisible(false);
        Pos->SetChild(LabelValidation4);
        Pos->SetParent(DirectoryDirectory);
        Pos->SetMarginRight(MarginRight);
        Pos->SetMarginTop(MarginTop);
        Pos->Start();
    }

    if(AvailableTypes.contains("page_7"))
    {
        Pos = new TopRightPositioner(this);
        LabelValidation3->setVisible(false);
        Pos->SetChild(LabelValidation3);
        Pos->SetParent(UrlUrl);
        Pos->SetMarginRight(MarginRight);
        Pos->SetMarginTop(MarginTop);
        Pos->Start();
    }

    LastVisibleAdvanced = true;

}

void ChooserResourceWidget::RenderAdvanced()
{
    if(AvailableTypes.contains("page"))
    {
        if(!label_4)
        {
            label_4 = new QLabel(page);
            label_4->setTextFormat(Qt::PlainText);
            label_4->setObjectName(QStringLiteral("label_4"));
            formLayout_2->setWidget(0, QFormLayout::LabelRole, label_4);
        }
        label_4->setText(QApplication::translate("ChooserResourceWidget", "Value", 0));
    }

    if(AvailableTypes.contains("page_2"))
    {
        if(!label_5)
        {
            label_5 = new QLabel(page_2);
            label_5->setTextFormat(Qt::PlainText);
            label_5->setObjectName(QStringLiteral("label_5"));
            formLayout_4->setWidget(0, QFormLayout::LabelRole, label_5);
        }
        label_5->setText(QApplication::translate("ChooserResourceWidget", "Value", 0));
    }

    if(AvailableTypes.contains("page_4"))
    {
        if(!label_8)
        {
            label_8 = new QLabel(page_4);
            label_8->setTextFormat(Qt::PlainText);
            label_8->setObjectName(QStringLiteral("label_8"));
            formLayout_6->setWidget(0, QFormLayout::LabelRole, label_8);
        }
        label_8->setText(QApplication::translate("ChooserResourceWidget", "Value", 0));
    }

    if(AvailableTypes.contains("page_3"))
    {
        if(!label_10)
        {
            label_10 = new QLabel(page_3);
            label_10->setTextFormat(Qt::PlainText);
            label_10->setObjectName(QStringLiteral("label_10"));
            label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_10->setWordWrap(true);
            formLayout_8->setWidget(0, QFormLayout::LabelRole, label_10);
        }
        label_10->setText(QApplication::translate("ChooserResourceWidget", "Filenames or directory", 0));
        if(!label)
        {
            label = new QLabel(page_3);
            label->setTextFormat(Qt::PlainText);
            label->setObjectName(QStringLiteral("label"));
            label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label->setWordWrap(true);
            formLayout_8->setWidget(2, QFormLayout::LabelRole, label);
        }
        label->setToolTip(QApplication::translate("ChooserResourceWidget", "If lines in file will be erased after max success usage or max fail usage.", 0));
        label->setText(QApplication::translate("ChooserResourceWidget", "Write file", 0));
        if(!label_3)
        {
            label_3 = new QLabel(page_3);
            label_3->setTextFormat(Qt::PlainText);
            label_3->setObjectName(QStringLiteral("label_3"));
            label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_3->setWordWrap(false);
            formLayout_8->setWidget(3, QFormLayout::LabelRole, label_3);
        }
        label_3->setToolTip(QApplication::translate("ChooserResourceWidget", "If lines will be mixed after load. This options selected will result data in random order.", 0));
        label_3->setText(QApplication::translate("ChooserResourceWidget", "Mix lines", 0));
        if(!label_111)
        {
            label_111 = new QLabel(page_3);
            label_111->setTextFormat(Qt::PlainText);
            label_111->setObjectName(QStringLiteral("label_111"));
            label_111->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_111->setWordWrap(true);
            formLayout_8->setWidget(4, QFormLayout::LabelRole, label_111);
        }
        label_111->setToolTip(QApplication::translate("ChooserResourceWidget", "Max success usage, after which line will be removed. Success definition depends on your script purpose. For example, if you register accounts and define proxy, this integer will designate maximum registration from one proxy.", 0));
        label_111->setText(QApplication::translate("ChooserResourceWidget", "Max success usage", 0));
        if(!label_17)
        {
            label_17 = new QLabel(page_3);
            label_17->setTextFormat(Qt::PlainText);
            label_17->setObjectName(QStringLiteral("label_17"));
            label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_17->setWordWrap(true);
            formLayout_8->setWidget(5, QFormLayout::LabelRole, label_17);
        }
        label_17->setToolTip(QApplication::translate("ChooserResourceWidget", "Max fail usage, after which line will be removed. Fail is often page load timeout or wait timeout. If you set this property low - resource will be banned quickly, otherwise engine will try a lot of time before give up with each file line.", 0));
        label_17->setText(QApplication::translate("ChooserResourceWidget", "Max fail usage", 0));
        if(!label_18)
        {
            label_18 = new QLabel(page_3);
            label_18->setTextFormat(Qt::PlainText);
            label_18->setObjectName(QStringLiteral("label_18"));
            label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_18->setWordWrap(true);
            formLayout_8->setWidget(6, QFormLayout::LabelRole, label_18);
        }
        label_18->setToolTip(QApplication::translate("ChooserResourceWidget", "How many times one line could be used by different threads. For example is you set it to 5 and the resource is proxy - one proxy could be used with up to 5 threads simultaneously. If you set it too high - proxy could be banned by server.", 0));
        label_18->setText(QApplication::translate("ChooserResourceWidget", "Max number of simultaneous use", 0));
        if(!label_19)
        {
            label_19 = new QLabel(page_3);
            label_19->setTextFormat(Qt::PlainText);
            label_19->setObjectName(QStringLiteral("label_19"));
            label_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_19->setWordWrap(true);
            formLayout_8->setWidget(7, QFormLayout::LabelRole, label_19);
        }
        label_19->setToolTip(QApplication::translate("ChooserResourceWidget", "Interval between usage. How much time needed to wait to use line repeatedly. For example, if you register accounts and define proxy this integer will designate maximum interval between usage of each proxy.", 0));
        label_19->setText(QApplication::translate("ChooserResourceWidget", "Interval between usage(millisecond)", 0));
        if(!label_26)
        {
            label_26 = new QLabel(page_3);
            label_26->setTextFormat(Qt::PlainText);
            label_26->setObjectName(QStringLiteral("label_26"));
            label_26->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_26->setWordWrap(true);
            formLayout_8->setWidget(9, QFormLayout::LabelRole, label_26);
        }
        label_26->setToolTip(QApplication::translate("ChooserResourceWidget", "If file needs to be reloaded periodically in certain periods of time. For example, you can point external application to grab names and write result to file. Engine will reload this file and data with new names. This is expensive option, use it only if you sure, that file content will change.", 0));
        label_26->setText(QApplication::translate("ChooserResourceWidget", "Reload periodically", 0));
        /*if(!label_27)
        {
            label_27 = new QLabel(page_3);
            label_27->setTextFormat(Qt::PlainText);
            label_27->setObjectName(QStringLiteral("label_27"));
            label_27->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_27->setWordWrap(true);
            formLayout_8->setWidget(11, QFormLayout::LabelRole, label_27);
        }
        label_27->setToolTip(QApplication::translate("ChooserResourceWidget", "How often engine will reload file.", 0));
        label_27->setText(QApplication::translate("ChooserResourceWidget", "Reload interval (millisecond)", 0));*/
        if(!label_37)
        {
            label_37 = new QLabel(page_3);
            label_37->setTextFormat(Qt::PlainText);
            label_37->setObjectName(QStringLiteral("label_37"));
            label_37->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_37->setWordWrap(true);
            int position;
            formLayout_8->getWidgetPosition(FileRenew,&position,0);
            formLayout_8->setWidget(position, QFormLayout::LabelRole, label_37);
        }
        label_37->setToolTip(QApplication::translate("ChooserResourceWidget", "If data needs to be renewed in certain periods of time. In this case all information about usage of lines will be cleared periodically and data could be used again.", 0));
        label_37->setText(QApplication::translate("ChooserResourceWidget", "Renew periodically", 0));
        /*if(!label_38)
        {
            label_38 = new QLabel(page_3);
            label_38->setTextFormat(Qt::PlainText);
            label_38->setObjectName(QStringLiteral("label_38"));
            label_38->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_38->setWordWrap(true);
            formLayout_8->setWidget(14, QFormLayout::LabelRole, label_38);
        }
        label_38->setToolTip(QApplication::translate("ChooserResourceWidget", "How often engine will renew data.", 0));
        label_38->setText(QApplication::translate("ChooserResourceWidget", "Renew interval", 0));*/
        if(!label_7)
        {
            label_7 = new QLabel(page_3);
            label_7->setTextFormat(Qt::PlainText);
            label_7->setObjectName(QStringLiteral("label_7"));
            formLayout_8->setWidget(8, QFormLayout::LabelRole, label_7);
        }
        label_7->setToolTip(QApplication::translate("ChooserResourceWidget", "If this option is set, engine will try to use same lines again and again. Otherwise, new line will be used on every resource request.", 0));
        label_7->setText(QApplication::translate("ChooserResourceWidget", "Greedy algorithm", 0));
        if(!label_50)
        {
            label_50 = new QLabel(page_3);
            label_50->setTextFormat(Qt::PlainText);
            label_50->setObjectName(QStringLiteral("label_50"));
            formLayout_8->setWidget(1, QFormLayout::LabelRole, label_50);
        }
        label_50->setToolTip(QApplication::translate("ChooserResourceWidget", "If engine will read from file.", 0));
        label_50->setText(QApplication::translate("ChooserResourceWidget", "Read file", 0));
    }

    if(AvailableTypes.contains("page_7"))
    {
        if(!label_11)
        {
            label_11 = new QLabel(page_7);
            label_11->setTextFormat(Qt::PlainText);
            label_11->setObjectName(QStringLiteral("label_11"));
            label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_11->setWordWrap(true);
            formLayout_9->setWidget(0, QFormLayout::LabelRole, label_11);
        }
        label_11->setText(QApplication::translate("ChooserResourceWidget", "Urls separated by | symbol", 0));
        if(!label_13)
        {
            label_13 = new QLabel(page_7);
            label_13->setTextFormat(Qt::PlainText);
            label_13->setObjectName(QStringLiteral("label_13"));
            label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_13->setWordWrap(true);
            formLayout_9->setWidget(1, QFormLayout::LabelRole, label_13);
        }
        label_13->setToolTip(QApplication::translate("ChooserResourceWidget", "If lines will be mixed after load. This options selected will result data in random order.", 0));
        label_13->setText(QApplication::translate("ChooserResourceWidget", "Mix lines", 0));
        if(!label_112)
        {
            label_112 = new QLabel(page_7);
            label_112->setTextFormat(Qt::PlainText);
            label_112->setObjectName(QStringLiteral("label_112"));
            label_112->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_112->setWordWrap(true);
            formLayout_9->setWidget(2, QFormLayout::LabelRole, label_112);
        }
        label_112->setToolTip(QApplication::translate("ChooserResourceWidget", "Max success usage, after which line will be removed. Success definition depends on your script purpose. For example, if you register accounts and define proxy, this integer will designate maximum registration from one proxy.", 0));
        label_112->setText(QApplication::translate("ChooserResourceWidget", "Max success usage", 0));
        if(!label_20)
        {
            label_20 = new QLabel(page_7);
            label_20->setTextFormat(Qt::PlainText);
            label_20->setObjectName(QStringLiteral("label_20"));
            label_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_20->setWordWrap(true);
            formLayout_9->setWidget(3, QFormLayout::LabelRole, label_20);
        }
        label_20->setToolTip(QApplication::translate("ChooserResourceWidget", "Max fail usage, after which line will be removed. Fail is often page load timeout or wait timeout. If you set this property low - resource will be banned quickly, otherwise engine will try a lot of time before give up with each line.", 0));
        label_20->setText(QApplication::translate("ChooserResourceWidget", "Max fail usage", 0));
        if(!label_21)
        {
            label_21 = new QLabel(page_7);
            label_21->setTextFormat(Qt::PlainText);
            label_21->setObjectName(QStringLiteral("label_21"));
            label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_21->setWordWrap(true);
            formLayout_9->setWidget(4, QFormLayout::LabelRole, label_21);
        }
        label_21->setToolTip(QApplication::translate("ChooserResourceWidget", "How many times one line could be used by different threads. For example is you set it to 5 and the resource is proxy - one proxy could be used with up to 5 threads simultaneously. If you set it too high - proxy could be banned by server.", 0));
        label_21->setText(QApplication::translate("ChooserResourceWidget", "Max number of simultaneous use", 0));
        if(!label_22)
        {
            label_22 = new QLabel(page_7);
            label_22->setTextFormat(Qt::PlainText);
            label_22->setObjectName(QStringLiteral("label_22"));
            label_22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_22->setWordWrap(true);
            formLayout_9->setWidget(5, QFormLayout::LabelRole, label_22);
        }
        label_22->setToolTip(QApplication::translate("ChooserResourceWidget", "Interval between usage. How much time needed to wait to use line repeatedly. For example, if you register accounts and define proxy this integer will designate maximum interval between usage of each proxy.", 0));
        label_22->setText(QApplication::translate("ChooserResourceWidget", "Interval between usage(millisecond)", 0));
        if(!label_29)
        {
            label_29 = new QLabel(page_7);
            label_29->setTextFormat(Qt::PlainText);
            label_29->setObjectName(QStringLiteral("label_29"));
            label_29->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_29->setWordWrap(true);
            formLayout_9->setWidget(7, QFormLayout::LabelRole, label_29);
        }
        label_29->setToolTip(QApplication::translate("ChooserResourceWidget", "If url needs to be reloaded periodically in certain periods of time. For example, you can load proxies from url and this option enable, engine will reload fresh proxies. This is expensive option, use it only if you sure, that url content will change.", 0));
        label_29->setText(QApplication::translate("ChooserResourceWidget", "Reload periodically", 0));
        /*if(!label_28)
        {
            label_28 = new QLabel(page_7);
            label_28->setTextFormat(Qt::PlainText);
            label_28->setObjectName(QStringLiteral("label_28"));
            label_28->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_28->setWordWrap(true);
            formLayout_9->setWidget(9, QFormLayout::LabelRole, label_28);
        }
        label_28->setToolTip(QApplication::translate("ChooserResourceWidget", "How often engine will reload url.", 0));
        label_28->setText(QApplication::translate("ChooserResourceWidget", "Reload interval (millisecond)", 0));*/
        if(!label_35)
        {
            label_35 = new QLabel(page_7);
            label_35->setTextFormat(Qt::PlainText);
            label_35->setObjectName(QStringLiteral("label_35"));
            label_35->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_35->setWordWrap(true);
            int position;
            formLayout_9->getWidgetPosition(UrlRenew,&position,0);
            formLayout_9->setWidget(position, QFormLayout::LabelRole, label_35);
        }
        label_35->setToolTip(QApplication::translate("ChooserResourceWidget", "If data needs to be renewed in certain periods of time. In this case all information about usage of lines will be cleared periodically and data could be used again.", 0));
        label_35->setText(QApplication::translate("ChooserResourceWidget", "Renew periodically", 0));
        /*if(!label_36)
        {
            label_36 = new QLabel(page_7);
            label_36->setTextFormat(Qt::PlainText);
            label_36->setObjectName(QStringLiteral("label_36"));
            label_36->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_36->setWordWrap(true);
            formLayout_9->setWidget(12, QFormLayout::LabelRole, label_36);
        }
        label_36->setToolTip(QApplication::translate("ChooserResourceWidget", "How often engine will renew data.", 0));
        label_36->setText(QApplication::translate("ChooserResourceWidget", "Renew interval", 0));*/
        if(!label_31)
        {
            label_31 = new QLabel(page_7);
            label_31->setTextFormat(Qt::PlainText);
            label_31->setObjectName(QStringLiteral("label_31"));
            formLayout_9->setWidget(6, QFormLayout::LabelRole, label_31);
        }
        label_31->setToolTip(QApplication::translate("ChooserResourceWidget", "If this option is set, engine will try to use same lines again and again. Otherwise, new line will be used on every resource request.", 0));
        label_31->setText(QApplication::translate("ChooserResourceWidget", "Greedy algorithm", 0));
    }

    if(AvailableTypes.contains("page_8"))
    {
        if(!label_12)
        {
            label_12 = new QLabel(page_8);
            label_12->setTextFormat(Qt::PlainText);
            label_12->setObjectName(QStringLiteral("label_12"));
            label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_12->setWordWrap(true);
            formLayout_10->setWidget(0, QFormLayout::LabelRole, label_12);
        }
        label_12->setText(QApplication::translate("ChooserResourceWidget", "Directory", 0));
        if(!label_14)
        {
            label_14 = new QLabel(page_8);
            label_14->setTextFormat(Qt::PlainText);
            label_14->setObjectName(QStringLiteral("label_14"));
            label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_14->setWordWrap(true);
            formLayout_10->setWidget(2, QFormLayout::LabelRole, label_14);
        }
        label_14->setToolTip(QApplication::translate("ChooserResourceWidget", "If files will be deleted after max success usage or max fail usage.", 0));
        label_14->setText(QApplication::translate("ChooserResourceWidget", "Delete files after use", 0));
        if(!label_15)
        {
            label_15 = new QLabel(page_8);
            label_15->setTextFormat(Qt::PlainText);
            label_15->setObjectName(QStringLiteral("label_15"));
            label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_15->setWordWrap(true);
            formLayout_10->setWidget(3, QFormLayout::LabelRole, label_15);
        }
        label_15->setToolTip(QApplication::translate("ChooserResourceWidget", "If files will be mixed after load. This options selected will result data in random order.", 0));
        label_15->setText(QApplication::translate("ChooserResourceWidget", "Mix files", 0));
        if(!label_113)
        {
            label_113 = new QLabel(page_8);
            label_113->setTextFormat(Qt::PlainText);
            label_113->setObjectName(QStringLiteral("label_113"));
            label_113->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_113->setWordWrap(true);
            formLayout_10->setWidget(4, QFormLayout::LabelRole, label_113);
        }
        label_113->setToolTip(QApplication::translate("ChooserResourceWidget", "Max success usage, after which file will be removed. Success definition depends on your script purpose. For example, if you register accounts and define picture, this integer will designate maximum times one picture could be in different accounts.", 0));
        label_113->setText(QApplication::translate("ChooserResourceWidget", "Max success usage", 0));
        if(!label_23)
        {
            label_23 = new QLabel(page_8);
            label_23->setTextFormat(Qt::PlainText);
            label_23->setObjectName(QStringLiteral("label_23"));
            label_23->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_23->setWordWrap(true);
            formLayout_10->setWidget(5, QFormLayout::LabelRole, label_23);
        }
        label_23->setToolTip(QApplication::translate("ChooserResourceWidget", "Max fail usage, after which line will be removed. Fail is often page load timeout or wait timeout. If you set this property low - resource will be banned quickly, otherwise engine will try a lot of time before give up with each file.", 0));
        label_23->setText(QApplication::translate("ChooserResourceWidget", "Max fail usage", 0));
        if(!label_24)
        {
            label_24 = new QLabel(page_8);
            label_24->setTextFormat(Qt::PlainText);
            label_24->setObjectName(QStringLiteral("label_24"));
            label_24->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_24->setWordWrap(true);
            formLayout_10->setWidget(6, QFormLayout::LabelRole, label_24);
        }
        label_24->setToolTip(QApplication::translate("ChooserResourceWidget", "How many times one file could be used by different threads.", 0));
        label_24->setText(QApplication::translate("ChooserResourceWidget", "Max number of simultaneous use", 0));
        if(!label_25)
        {
            label_25 = new QLabel(page_8);
            label_25->setTextFormat(Qt::PlainText);
            label_25->setObjectName(QStringLiteral("label_25"));
            label_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_25->setWordWrap(true);
            formLayout_10->setWidget(7, QFormLayout::LabelRole, label_25);
        }
        label_25->setToolTip(QApplication::translate("ChooserResourceWidget", "Interval between usage. How much time needed to wait to use file repeatedly.", 0));
        label_25->setText(QApplication::translate("ChooserResourceWidget", "Interval between usage(millisecond)", 0));
        if(!label_16)
        {
            label_16 = new QLabel(page_8);
            label_16->setTextFormat(Qt::PlainText);
            label_16->setObjectName(QStringLiteral("label_16"));
            formLayout_10->setWidget(1, QFormLayout::LabelRole, label_16);
        }
        label_16->setText(QApplication::translate("ChooserResourceWidget", "Wildcard", 0));
        if(!label_6)
        {
            label_6 = new QLabel(page_8);
            label_6->setTextFormat(Qt::PlainText);
            label_6->setObjectName(QStringLiteral("label_6"));
            label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_6->setWordWrap(true);
            formLayout_10->setWidget(9, QFormLayout::LabelRole, label_6);
        }
        label_6->setToolTip(QApplication::translate("ChooserResourceWidget", "If folder content needs to be reloaded periodically in certain periods of time. This is expensive option, use it only if you sure, that folder content will change.", 0));
        label_6->setText(QApplication::translate("ChooserResourceWidget", "Reload periodically", 0));
        /*if(!label_30)
        {
            label_30 = new QLabel(page_8);
            label_30->setTextFormat(Qt::PlainText);
            label_30->setObjectName(QStringLiteral("label_30"));
            label_30->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_30->setWordWrap(true);
            formLayout_10->setWidget(10, QFormLayout::LabelRole, label_30);
        }
        label_30->setToolTip(QApplication::translate("ChooserResourceWidget", "How often engine will reload directory content.", 0));
        label_30->setText(QApplication::translate("ChooserResourceWidget", "Reload interval (millisecond)", 0));*/
        if(!label_33)
        {
            label_33 = new QLabel(page_8);
            label_33->setTextFormat(Qt::PlainText);
            label_33->setObjectName(QStringLiteral("label_33"));
            label_33->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_33->setWordWrap(true);
            int position;
            formLayout_10->getWidgetPosition(DirectoryRenew,&position,0);
            formLayout_10->setWidget(position, QFormLayout::LabelRole, label_33);
        }
        label_33->setToolTip(QApplication::translate("ChooserResourceWidget", "If data needs to be renewed in certain periods of time. In this case all information about usage of files will be cleared periodically and files could be used again.", 0));
        label_33->setText(QApplication::translate("ChooserResourceWidget", "Renew periodically", 0));
        /*if(!label_34)
        {
            label_34 = new QLabel(page_8);
            label_34->setTextFormat(Qt::PlainText);
            label_34->setObjectName(QStringLiteral("label_34"));
            label_34->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_34->setWordWrap(true);
            formLayout_10->setWidget(12, QFormLayout::LabelRole, label_34);
        }
        label_34->setToolTip(QApplication::translate("ChooserResourceWidget", "How often engine will renew data.", 0));
        label_34->setText(QApplication::translate("ChooserResourceWidget", "Renew interval", 0));*/
        if(!label_32)
        {
            label_32 = new QLabel(page_8);
            label_32->setTextFormat(Qt::PlainText);
            label_32->setObjectName(QStringLiteral("label_32"));
            label_32->setContextMenuPolicy(Qt::PreventContextMenu);
            formLayout_10->setWidget(8, QFormLayout::LabelRole, label_32);
        }
        label_32->setToolTip(QApplication::translate("ChooserResourceWidget", "If this option is set, engine will try to use same lines again and again. Otherwise, new line will be used on every resource request.", 0));
        label_32->setText(QApplication::translate("ChooserResourceWidget", "Greedy algorithm", 0));
    }

    if(AvailableTypes.contains("page_9"))
    {
        if(!label_39)
        {
            label_39 = new QLabel(widget_2);
            label_39->setTextFormat(Qt::PlainText);
            label_39->setObjectName(QStringLiteral("label_39"));
            label_39->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_39->setWordWrap(true);
            formLayout->setWidget(1, QFormLayout::LabelRole, label_39);
        }
        label_39->setToolTip(QApplication::translate("ChooserResourceWidget", "If rows will be deleted after max success usage or max fail usage.", 0));
        label_39->setText(QApplication::translate("ChooserResourceWidget", "Change table", 0));
        if(!label_40)
        {
            label_40 = new QLabel(widget_2);
            label_40->setTextFormat(Qt::PlainText);
            label_40->setObjectName(QStringLiteral("label_40"));
            label_40->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_40->setWordWrap(true);
            formLayout->setWidget(2, QFormLayout::LabelRole, label_40);
        }
        label_40->setToolTip(QApplication::translate("ChooserResourceWidget", "If rows will be mixed after load. This options selected will result data in random order.", 0));
        label_40->setText(QApplication::translate("ChooserResourceWidget", "Mix rows", 0));
        if(!label_41)
        {
            label_41 = new QLabel(widget_2);
            label_41->setTextFormat(Qt::PlainText);
            label_41->setObjectName(QStringLiteral("label_41"));
            label_41->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_41->setWordWrap(true);
            formLayout->setWidget(3, QFormLayout::LabelRole, label_41);
        }
        label_41->setToolTip(QApplication::translate("ChooserResourceWidget", "Max success usage, after which row will be removed. Success definition depends on your script purpose.", 0));
        label_41->setText(QApplication::translate("ChooserResourceWidget", "Max success usage", 0));
        if(!label_42)
        {
            label_42 = new QLabel(widget_2);
            label_42->setTextFormat(Qt::PlainText);
            label_42->setObjectName(QStringLiteral("label_42"));
            label_42->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_42->setWordWrap(true);
            formLayout->setWidget(4, QFormLayout::LabelRole, label_42);
        }
        label_42->setToolTip(QApplication::translate("ChooserResourceWidget", "Max row usage, after which line will be removed. Fail is often page load timeout or wait timeout. If you set this property low - resource will be banned quickly, otherwise engine will try a lot of time before give up with each row.", 0));
        label_42->setText(QApplication::translate("ChooserResourceWidget", "Max fail usage", 0));
        if(!label_43)
        {
            label_43 = new QLabel(widget_2);
            label_43->setTextFormat(Qt::PlainText);
            label_43->setObjectName(QStringLiteral("label_43"));
            label_43->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_43->setWordWrap(true);
            formLayout->setWidget(5, QFormLayout::LabelRole, label_43);
        }
        label_43->setToolTip(QApplication::translate("ChooserResourceWidget", "How many times one row could be used by different threads.", 0));
        label_43->setText(QApplication::translate("ChooserResourceWidget", "Max number of simultaneous use", 0));
        if(!label_44)
        {
            label_44 = new QLabel(widget_2);
            label_44->setTextFormat(Qt::PlainText);
            label_44->setObjectName(QStringLiteral("label_44"));
            label_44->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_44->setWordWrap(true);
            formLayout->setWidget(6, QFormLayout::LabelRole, label_44);
        }
        label_44->setToolTip(QApplication::translate("ChooserResourceWidget", "Interval between usage. How much time needed to wait to use row repeatedly.", 0));
        label_44->setText(QApplication::translate("ChooserResourceWidget", "Interval between usage(millisecond)", 0));
        if(!label_45)
        {
            label_45 = new QLabel(widget_2);
            label_45->setTextFormat(Qt::PlainText);
            label_45->setObjectName(QStringLiteral("label_45"));
            label_45->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_45->setWordWrap(true);
            formLayout->setWidget(7, QFormLayout::LabelRole, label_45);
        }
        label_45->setToolTip(QApplication::translate("ChooserResourceWidget", "If this option is set, engine will try to use same lines again and again. Otherwise, new line will be used on every resource request.", 0));
        label_45->setText(QApplication::translate("ChooserResourceWidget", "Greedy algorithm", 0));
        if(!label_46)
        {
            label_46 = new QLabel(widget_2);
            label_46->setTextFormat(Qt::PlainText);
            label_46->setObjectName(QStringLiteral("label_46"));
            label_46->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_46->setWordWrap(true);
            formLayout->setWidget(8, QFormLayout::LabelRole, label_46);
        }
        label_46->setToolTip(QApplication::translate("ChooserResourceWidget", "If rows need to be reloaded periodically in certain periods of time. This is expensive option, use it only if you sure, that table content will change.", 0));
        label_46->setText(QApplication::translate("ChooserResourceWidget", "Reload periodically", 0));
        /*if(!label_47)
        {
            label_47 = new QLabel(widget_2);
            label_47->setTextFormat(Qt::PlainText);
            label_47->setObjectName(QStringLiteral("label_47"));
            label_47->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_47->setWordWrap(true);
            formLayout->setWidget(9, QFormLayout::LabelRole, label_47);
        }
        label_47->setToolTip(QApplication::translate("ChooserResourceWidget", "How often engine will reload table content.", 0));
        label_47->setText(QApplication::translate("ChooserResourceWidget", "Reload interval (millisecond)", 0));*/
        if(!label_48)
        {
            label_48 = new QLabel(widget_2);
            label_48->setTextFormat(Qt::PlainText);
            label_48->setObjectName(QStringLiteral("label_48"));
            label_48->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_48->setWordWrap(true);
            int position;
            formLayout->getWidgetPosition(DatabaseRenew,&position,0);
            formLayout->setWidget(position, QFormLayout::LabelRole, label_48);
        }
        label_48->setToolTip(QApplication::translate("ChooserResourceWidget", "If data needs to be renewed in certain periods of time. In this case all information about usage of files will be cleared periodically and rows could be used again.", 0));
        label_48->setText(QApplication::translate("ChooserResourceWidget", "Renew periodically", 0));
        /*if(!label_49)
        {
            label_49 = new QLabel(widget_2);
            label_49->setTextFormat(Qt::PlainText);
            label_49->setObjectName(QStringLiteral("label_49"));
            label_49->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
            label_49->setWordWrap(true);
            formLayout->setWidget(11, QFormLayout::LabelRole, label_49);
        }
        label_49->setToolTip(QApplication::translate("ChooserResourceWidget", "How often engine will renew data.", 0));
        label_49->setText(QApplication::translate("ChooserResourceWidget", "Renew interval", 0));*/
    }
}

void ChooserResourceWidget::SetupUiCustom()
{
    if(AvailableTypes.contains("page"))
    {
        page = new QWidget(this);
        page->setObjectName(QStringLiteral("page"));
        horizontalLayout = new QHBoxLayout(page);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout_2->setFormAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        formLayout_2->setHorizontalSpacing(6);
        formLayout_2->setVerticalSpacing(6);
        LayoutValidation1 = new QHBoxLayout();
        LayoutValidation1->setSpacing(5);
        LayoutValidation1->setObjectName(QStringLiteral("LayoutValidation1"));
        LayoutValidation1->setContentsMargins(-1, 0, -1, -1);
        FixedStringValue = new QLineEdit(page);
        FixedStringValue->setObjectName(QStringLiteral("FixedStringValue"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(FixedStringValue->sizePolicy().hasHeightForWidth());
        FixedStringValue->setSizePolicy(sizePolicy1);
        FixedStringValue->setMinimumSize(QSize(175, 0));
        FixedStringValue->setMaximumSize(QSize(175, 16777215));
        FixedStringValue->setText(QStringLiteral(""));
        LayoutValidation1->addWidget(FixedStringValue);
        LabelValidation1 = new QLabel(page);
        LabelValidation1->setObjectName(QStringLiteral("LabelValidation1"));
        LabelValidation1->setPixmap(QPixmap(QString::fromUtf8(":/engine/images/invalid.png")));
        LayoutValidation1->addWidget(LabelValidation1);
        formLayout_2->setLayout(0, QFormLayout::FieldRole, LayoutValidation1);
        horizontalLayout->addLayout(formLayout_2);
    }

    if(AvailableTypes.contains("page_2"))
    {
        page_2 = new QWidget(this);
        page_2->setObjectName(QStringLiteral("page_2"));
        verticalLayout_2 = new QVBoxLayout(page_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        formLayout_4 = new QFormLayout();
        formLayout_4->setObjectName(QStringLiteral("formLayout_4"));
        formLayout_4->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout_4->setFormAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        formLayout_4->setHorizontalSpacing(6);
        formLayout_4->setVerticalSpacing(6);

        FixedIntegerValue = new QSpinBox(page_2);
        FixedIntegerValue->setObjectName(QStringLiteral("FixedIntegerValue"));
        FixedIntegerValue->setMinimumSize(QSize(175, 0));
        FixedIntegerValue->setMaximumSize(QSize(175, 16777215));
        FixedIntegerValue->setMinimum(-999999999);
        FixedIntegerValue->setMaximum(999999999);
        formLayout_4->setWidget(0, QFormLayout::FieldRole, FixedIntegerValue);
        verticalLayout_2->addLayout(formLayout_4);
    }

    if(AvailableTypes.contains("page_4"))
    {
        page_4 = new QWidget(this);
        page_4->setObjectName(QStringLiteral("page_4"));
        horizontalLayout_7 = new QHBoxLayout(page_4);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        formLayout_6 = new QFormLayout();
        formLayout_6->setObjectName(QStringLiteral("formLayout_6"));
        formLayout_6->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout_6->setFormAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        formLayout_6->setHorizontalSpacing(6);
        formLayout_6->setVerticalSpacing(6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(3);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        RandomStringValue = new QLineEdit(page_4);
        RandomStringValue->setObjectName(QStringLiteral("RandomStringValue"));
        RandomStringValue->setMinimumSize(QSize(150, 0));
        RandomStringValue->setMaximumSize(QSize(150, 16777215));
        RandomStringValue->setText(QStringLiteral(""));
        horizontalLayout_5->addWidget(RandomStringValue);
        RandomStringEdit = new QPushButton(page_4);
        RandomStringEdit->setObjectName(QStringLiteral("RandomStringEdit"));
        RandomStringEdit->setMinimumSize(QSize(20, 0));
        RandomStringEdit->setMaximumSize(QSize(20, 16777215));
        QIcon icon;
        icon.addFile(QStringLiteral(":/engine/images/edit.png"), QSize(), QIcon::Normal, QIcon::Off);
        RandomStringEdit->setIcon(icon);
        RandomStringEdit->setIconSize(QSize(14, 14));
        horizontalLayout_5->addWidget(RandomStringEdit);
        formLayout_6->setLayout(0, QFormLayout::FieldRole, horizontalLayout_5);
        horizontalLayout_7->addLayout(formLayout_6);
    }

    if(AvailableTypes.contains("page_5"))
    {
        page_5 = new QWidget(this);
        page_5->setObjectName(QStringLiteral("page_5"));
        horizontalLayout_8 = new QHBoxLayout(page_5);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        formLayout_7 = new QFormLayout();
        formLayout_7->setObjectName(QStringLiteral("formLayout_7"));
        formLayout_7->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout_7->setFormAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        formLayout_7->setHorizontalSpacing(6);
        formLayout_7->setVerticalSpacing(6);
        label_9 = new QLabel(page_5);
        label_9->setTextFormat(Qt::PlainText);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setText(QApplication::translate("ChooserResourceWidget", "Minimum Value", 0));
        formLayout_7->setWidget(0, QFormLayout::LabelRole, label_9);
        RIMinimumValue = new QSpinBox(page_5);
        RIMinimumValue->setObjectName(QStringLiteral("RIMinimumValue"));
        RIMinimumValue->setMinimumSize(QSize(150, 25));
        RIMinimumValue->setMaximumSize(QSize(150, 16777215));
        RIMinimumValue->setMinimum(-999999999);
        RIMinimumValue->setMaximum(999999999);
        RIMinimumValue->setValue(1);
        formLayout_7->setWidget(0, QFormLayout::FieldRole, RIMinimumValue);
        label_2 = new QLabel(page_5);
        label_2->setTextFormat(Qt::PlainText);
        label_2->setText(QApplication::translate("ChooserResourceWidget", "Maximum Value", 0));
        label_2->setObjectName(QStringLiteral("label_2"));
        formLayout_7->setWidget(1, QFormLayout::LabelRole, label_2);
        RIMaximumValue = new QSpinBox(page_5);
        RIMaximumValue->setObjectName(QStringLiteral("RIMaximumValue"));
        RIMaximumValue->setMinimumSize(QSize(150, 25));
        RIMaximumValue->setMaximumSize(QSize(150, 16777215));
        RIMaximumValue->setMinimum(-999999999);
        RIMaximumValue->setMaximum(999999999);
        RIMaximumValue->setValue(10);
        formLayout_7->setWidget(1, QFormLayout::FieldRole, RIMaximumValue);
        horizontalLayout_8->addLayout(formLayout_7);
    }

    if(AvailableTypes.contains("page_6"))
    {
        page_6 = new QWidget(this);
        page_6->setObjectName(QStringLiteral("page_6"));
        verticalLayout_3 = new QVBoxLayout(page_6);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        SelectWidget = new QWidget(page_6);
        SelectWidget->setObjectName(QStringLiteral("SelectWidget"));
        horizontalLayout_3 = new QHBoxLayout(SelectWidget);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        horizontalLayout_3->addLayout(verticalLayout_9);
        verticalLayout_8->addWidget(SelectWidget);
        verticalLayout_3->addLayout(verticalLayout_8);
    }

    if(AvailableTypes.contains("page_3"))
    {
        page_3 = new QWidget(this);
        page_3->setObjectName(QStringLiteral("page_3"));
        verticalLayout = new QVBoxLayout(page_3);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        formLayout_8 = new QFormLayout();
        formLayout_8->setObjectName(QStringLiteral("formLayout_8"));
        formLayout_8->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout_8->setLabelAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        formLayout_8->setFormAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        formLayout_8->setHorizontalSpacing(6);
        formLayout_8->setVerticalSpacing(12);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(3);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        FileFilename = new QLineEdit(page_3);
        FileFilename->setObjectName(QStringLiteral("FileFilename"));
        FileFilename->setMinimumSize(QSize(100, 0));
        FileFilename->setMaximumSize(QSize(100, 16777215));
        FileFilename->setText(QStringLiteral(""));
        horizontalLayout_6->addWidget(FileFilename);
        LabelValidation2 = new QLabel(page_3);
        LabelValidation2->setObjectName(QStringLiteral("LabelValidation2"));
        LabelValidation2->setPixmap(QPixmap(QString::fromUtf8(":/engine/images/invalid.png")));
        horizontalLayout_6->addWidget(LabelValidation2);
        ButtonFileSelect = new QPushButton(page_3);
        ButtonFileSelect->setObjectName(QStringLiteral("ButtonFileSelect"));
        ButtonFileSelect->setMinimumSize(QSize(20, 0));
        ButtonFileSelect->setMaximumSize(QSize(20, 16777215));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/engine/images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonFileSelect->setIcon(icon1);
        ButtonFileSelect->setIconSize(QSize(14, 14));
        horizontalLayout_6->addWidget(ButtonFileSelect);
        formLayout_8->setLayout(0, QFormLayout::FieldRole, horizontalLayout_6);

        FileWrite = new QCheckBox(page_3);
        FileWrite->setObjectName(QStringLiteral("FileWrite"));
        formLayout_8->setWidget(2, QFormLayout::FieldRole, FileWrite);

        FileMix = new QCheckBox(page_3);
        FileMix->setObjectName(QStringLiteral("FileMix"));
        formLayout_8->setWidget(3, QFormLayout::FieldRole, FileMix);




        FileMaxSuccess = new QSpinBox(page_3);
        FileMaxSuccess->setObjectName(QStringLiteral("FileMaxSuccess"));
        FileMaxSuccess->setMinimumSize(QSize(100, 0));
        FileMaxSuccess->setMaximumSize(QSize(100, 16777215));
        FileMaxSuccess->setMinimum(1);
        FileMaxSuccess->setMaximum(999999999);
        formLayout_8->setWidget(4, QFormLayout::FieldRole, FileMaxSuccess);
        FileMaxFail = new QSpinBox(page_3);
        FileMaxFail->setObjectName(QStringLiteral("FileMaxFail"));
        FileMaxFail->setMinimumSize(QSize(100, 0));
        FileMaxFail->setMaximumSize(QSize(100, 16777215));
        FileMaxFail->setMinimum(1);
        FileMaxFail->setMaximum(999999999);
        formLayout_8->setWidget(5, QFormLayout::FieldRole, FileMaxFail);
        FileMaxSimultaneous = new QSpinBox(page_3);
        FileMaxSimultaneous->setObjectName(QStringLiteral("FileMaxSimultaneous"));
        FileMaxSimultaneous->setMinimumSize(QSize(100, 0));
        FileMaxSimultaneous->setMaximumSize(QSize(100, 16777215));
        FileMaxSimultaneous->setMinimum(1);
        FileMaxSimultaneous->setMaximum(999999999);
        formLayout_8->setWidget(6, QFormLayout::FieldRole, FileMaxSimultaneous);
        FileInterval = new QSpinBox(page_3);
        FileInterval->setObjectName(QStringLiteral("FileInterval"));
        FileInterval->setMinimumSize(QSize(100, 0));
        FileInterval->setMaximumSize(QSize(100, 16777215));
        FileInterval->setMinimum(0);
        FileInterval->setMaximum(999999999);
        FileInterval->setValue(5000);
        formLayout_8->setWidget(7, QFormLayout::FieldRole, FileInterval);

        FileReload = new QCheckBox(page_3);
        FileReload->setObjectName(QStringLiteral("FileReload"));
        FileReload->setChecked(false);
        formLayout_8->setWidget(9, QFormLayout::FieldRole, FileReload);

        FileReloadInterval = new QSpinBox(page_3);
        FileReloadInterval->setObjectName(QStringLiteral("FileReloadInterval"));
        FileReloadInterval->setMinimumSize(QSize(100, 0));
        FileReloadInterval->setMaximumSize(QSize(100, 16777215));
        FileReloadInterval->setMinimum(1000);
        FileReloadInterval->setMaximum(999999999);
        FileReloadInterval->setSingleStep(1000);
        FileReloadInterval->setValue(60000);
        formLayout_8->setWidget(11, QFormLayout::FieldRole, FileReloadInterval);


        FileRenew = new QCheckBox(page_3);
        FileRenew->setObjectName(QStringLiteral("FileRenew"));
        formLayout_8->setWidget(13, QFormLayout::FieldRole, FileRenew);
        FileRenewInterval = new QSpinBox(page_3);
        FileRenewInterval->setObjectName(QStringLiteral("FileRenewInterval"));
        FileRenewInterval->setMinimumSize(QSize(100, 0));
        FileRenewInterval->setMaximumSize(QSize(100, 16777215));
        FileRenewInterval->setMinimum(1000);
        FileRenewInterval->setMaximum(999999999);
        FileRenewInterval->setValue(600000);
        formLayout_8->setWidget(14, QFormLayout::FieldRole, FileRenewInterval);

        FileGreedy = new QCheckBox(page_3);
        FileGreedy->setObjectName(QStringLiteral("FileGreedy"));
        formLayout_8->setWidget(8, QFormLayout::FieldRole, FileGreedy);

        FileRead = new QCheckBox(page_3);
        FileRead->setObjectName(QStringLiteral("FileRead"));
        FileRead->setChecked(true);
        formLayout_8->setWidget(1, QFormLayout::FieldRole, FileRead);
        verticalLayout->addLayout(formLayout_8);
    }

    if(AvailableTypes.contains("page_7"))
    {
        page_7 = new QWidget(this);
        page_7->setObjectName(QStringLiteral("page_7"));
        verticalLayout_6 = new QVBoxLayout(page_7);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        formLayout_9 = new QFormLayout();
        formLayout_9->setObjectName(QStringLiteral("formLayout_9"));
        formLayout_9->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout_9->setLabelAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        formLayout_9->setFormAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        formLayout_9->setHorizontalSpacing(6);
        formLayout_9->setVerticalSpacing(12);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(3);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        UrlUrl = new QLineEdit(page_7);
        UrlUrl->setObjectName(QStringLiteral("UrlUrl"));
        UrlUrl->setMinimumSize(QSize(120, 0));
        UrlUrl->setMaximumSize(QSize(120, 16777215));
        UrlUrl->setText(QStringLiteral(""));
        horizontalLayout_9->addWidget(UrlUrl);
        LabelValidation3 = new QLabel(page_7);
        LabelValidation3->setObjectName(QStringLiteral("LabelValidation3"));
        LabelValidation3->setPixmap(QPixmap(QString::fromUtf8(":/engine/images/invalid.png")));
        horizontalLayout_9->addWidget(LabelValidation3);
        formLayout_9->setLayout(0, QFormLayout::FieldRole, horizontalLayout_9);

        UrlMix = new QCheckBox(page_7);
        UrlMix->setObjectName(QStringLiteral("UrlMix"));
        formLayout_9->setWidget(1, QFormLayout::FieldRole, UrlMix);

        UrlMaxSuccess = new QSpinBox(page_7);
        UrlMaxSuccess->setObjectName(QStringLiteral("UrlMaxSuccess"));
        UrlMaxSuccess->setMinimumSize(QSize(100, 0));
        UrlMaxSuccess->setMaximumSize(QSize(100, 16777215));
        UrlMaxSuccess->setMinimum(1);
        UrlMaxSuccess->setMaximum(999999999);
        formLayout_9->setWidget(2, QFormLayout::FieldRole, UrlMaxSuccess);

        UrlMaxFail = new QSpinBox(page_7);
        UrlMaxFail->setObjectName(QStringLiteral("UrlMaxFail"));
        UrlMaxFail->setMinimumSize(QSize(100, 0));
        UrlMaxFail->setMaximumSize(QSize(100, 16777215));
        UrlMaxFail->setMinimum(1);
        UrlMaxFail->setMaximum(999999999);
        formLayout_9->setWidget(3, QFormLayout::FieldRole, UrlMaxFail);


        UrlMaxSimultaneous = new QSpinBox(page_7);
        UrlMaxSimultaneous->setObjectName(QStringLiteral("UrlMaxSimultaneous"));
        UrlMaxSimultaneous->setMinimumSize(QSize(100, 0));
        UrlMaxSimultaneous->setMaximumSize(QSize(100, 16777215));
        UrlMaxSimultaneous->setMinimum(1);
        UrlMaxSimultaneous->setMaximum(999999999);
        formLayout_9->setWidget(4, QFormLayout::FieldRole, UrlMaxSimultaneous);

        UrlInterval = new QSpinBox(page_7);
        UrlInterval->setObjectName(QStringLiteral("UrlInterval"));
        UrlInterval->setMinimumSize(QSize(100, 0));
        UrlInterval->setMaximumSize(QSize(100, 16777215));
        UrlInterval->setMinimum(0);
        UrlInterval->setMaximum(999999999);
        UrlInterval->setValue(5000);
        formLayout_9->setWidget(5, QFormLayout::FieldRole, UrlInterval);

        UrlGreedy = new QCheckBox(page_7);
        UrlGreedy->setObjectName(QStringLiteral("UrlGreedy"));
        formLayout_9->setWidget(6, QFormLayout::FieldRole, UrlGreedy);

        UrlReload = new QCheckBox(page_7);
        UrlReload->setObjectName(QStringLiteral("UrlReload"));
        UrlReload->setEnabled(true);
        UrlReload->setChecked(false);
        formLayout_9->setWidget(7, QFormLayout::FieldRole, UrlReload);

        UrlReloadInterval = new QSpinBox(page_7);
        UrlReloadInterval->setObjectName(QStringLiteral("UrlReloadInterval"));
        UrlReloadInterval->setMinimumSize(QSize(100, 0));
        UrlReloadInterval->setMaximumSize(QSize(100, 16777215));
        UrlReloadInterval->setMinimum(500);
        UrlReloadInterval->setMaximum(999999999);
        UrlReloadInterval->setSingleStep(1000);
        UrlReloadInterval->setValue(10000);
        formLayout_9->setWidget(8, QFormLayout::FieldRole, UrlReloadInterval);


        UrlRenew = new QCheckBox(page_7);
        UrlRenew->setObjectName(QStringLiteral("UrlRenew"));
        formLayout_9->setWidget(9, QFormLayout::FieldRole, UrlRenew);

        UrlRenewInterval = new QSpinBox(page_7);
        UrlRenewInterval->setObjectName(QStringLiteral("UrlRenewInterval"));
        UrlRenewInterval->setMinimumSize(QSize(100, 0));
        UrlRenewInterval->setMaximumSize(QSize(100, 16777215));
        UrlRenewInterval->setMinimum(1000);
        UrlRenewInterval->setMaximum(999999999);
        UrlRenewInterval->setValue(600000);
        formLayout_9->setWidget(10, QFormLayout::FieldRole, UrlRenewInterval);


        verticalLayout_6->addLayout(formLayout_9);
    }

    if(AvailableTypes.contains("page_8"))
    {
        page_8 = new QWidget(this);
        page_8->setObjectName(QStringLiteral("page_8"));
        verticalLayout_7 = new QVBoxLayout(page_8);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        formLayout_10 = new QFormLayout();
        formLayout_10->setObjectName(QStringLiteral("formLayout_10"));
        formLayout_10->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout_10->setLabelAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        formLayout_10->setFormAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        formLayout_10->setHorizontalSpacing(6);
        formLayout_10->setVerticalSpacing(12);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(3);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        DirectoryDirectory = new QLineEdit(page_8);
        DirectoryDirectory->setObjectName(QStringLiteral("DirectoryDirectory"));
        DirectoryDirectory->setMinimumSize(QSize(100, 0));
        DirectoryDirectory->setMaximumSize(QSize(100, 16777215));
        DirectoryDirectory->setText(QStringLiteral(""));
        horizontalLayout_10->addWidget(DirectoryDirectory);
        LabelValidation4 = new QLabel(page_8);
        LabelValidation4->setObjectName(QStringLiteral("LabelValidation4"));
        LabelValidation4->setPixmap(QPixmap(QString::fromUtf8(":/engine/images/invalid.png")));
        horizontalLayout_10->addWidget(LabelValidation4);
        ButtonDirectorySelect = new QPushButton(page_8);
        ButtonDirectorySelect->setObjectName(QStringLiteral("ButtonDirectorySelect"));
        ButtonDirectorySelect->setMinimumSize(QSize(20, 0));
        ButtonDirectorySelect->setMaximumSize(QSize(20, 16777215));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/engine/images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        ButtonDirectorySelect->setIcon(icon1);
        ButtonDirectorySelect->setIconSize(QSize(14, 14));
        horizontalLayout_10->addWidget(ButtonDirectorySelect);
        formLayout_10->setLayout(0, QFormLayout::FieldRole, horizontalLayout_10);

        DirectoryClean = new QCheckBox(page_8);
        DirectoryClean->setObjectName(QStringLiteral("DirectoryClean"));
        formLayout_10->setWidget(2, QFormLayout::FieldRole, DirectoryClean);

        DirectoryMix = new QCheckBox(page_8);
        DirectoryMix->setObjectName(QStringLiteral("DirectoryMix"));
        formLayout_10->setWidget(3, QFormLayout::FieldRole, DirectoryMix);




        DirectoryMaxSuccess = new QSpinBox(page_8);
        DirectoryMaxSuccess->setObjectName(QStringLiteral("DirectoryMaxSuccess"));
        DirectoryMaxSuccess->setMinimumSize(QSize(100, 0));
        DirectoryMaxSuccess->setMaximumSize(QSize(100, 16777215));
        DirectoryMaxSuccess->setMinimum(1);
        DirectoryMaxSuccess->setMaximum(999999999);
        formLayout_10->setWidget(4, QFormLayout::FieldRole, DirectoryMaxSuccess);
        DirectoryMaxFail = new QSpinBox(page_8);
        DirectoryMaxFail->setObjectName(QStringLiteral("DirectoryMaxFail"));
        DirectoryMaxFail->setMinimumSize(QSize(100, 0));
        DirectoryMaxFail->setMaximumSize(QSize(100, 16777215));
        DirectoryMaxFail->setMinimum(1);
        DirectoryMaxFail->setMaximum(999999999);
        formLayout_10->setWidget(5, QFormLayout::FieldRole, DirectoryMaxFail);
        DirectoryMaxSimultaneous = new QSpinBox(page_8);
        DirectoryMaxSimultaneous->setObjectName(QStringLiteral("DirectoryMaxSimultaneous"));
        DirectoryMaxSimultaneous->setMinimumSize(QSize(100, 0));
        DirectoryMaxSimultaneous->setMaximumSize(QSize(100, 16777215));
        DirectoryMaxSimultaneous->setMinimum(1);
        DirectoryMaxSimultaneous->setMaximum(999999999);
        formLayout_10->setWidget(6, QFormLayout::FieldRole, DirectoryMaxSimultaneous);
        DirectoryInterval = new QSpinBox(page_8);
        DirectoryInterval->setObjectName(QStringLiteral("DirectoryInterval"));
        DirectoryInterval->setMinimumSize(QSize(100, 0));
        DirectoryInterval->setMaximumSize(QSize(100, 16777215));
        DirectoryInterval->setMinimum(0);
        DirectoryInterval->setMaximum(999999999);
        DirectoryInterval->setValue(5000);
        formLayout_10->setWidget(7, QFormLayout::FieldRole, DirectoryInterval);

        DirectoryWildcard = new QLineEdit(page_8);
        DirectoryWildcard->setObjectName(QStringLiteral("DirectoryWildcard"));
        DirectoryWildcard->setMinimumSize(QSize(100, 0));
        DirectoryWildcard->setMaximumSize(QSize(100, 16777215));
        DirectoryWildcard->setText(QStringLiteral("*.*"));
        formLayout_10->setWidget(1, QFormLayout::FieldRole, DirectoryWildcard);


        DirectoryReload = new QCheckBox(page_8);
        DirectoryReload->setObjectName(QStringLiteral("DirectoryReload"));
        DirectoryReload->setChecked(false);
        formLayout_10->setWidget(9, QFormLayout::FieldRole, DirectoryReload);
        DirectoryReloadInterval = new QSpinBox(page_8);
        DirectoryReloadInterval->setObjectName(QStringLiteral("DirectoryReloadInterval"));
        DirectoryReloadInterval->setMinimumSize(QSize(100, 0));
        DirectoryReloadInterval->setMaximumSize(QSize(100, 16777215));
        DirectoryReloadInterval->setMinimum(10000);
        DirectoryReloadInterval->setMaximum(999999999);
        DirectoryReloadInterval->setSingleStep(1000);
        DirectoryReloadInterval->setValue(60000);
        formLayout_10->setWidget(10, QFormLayout::FieldRole, DirectoryReloadInterval);

        DirectoryRenew = new QCheckBox(page_8);
        DirectoryRenew->setObjectName(QStringLiteral("DirectoryRenew"));
        formLayout_10->setWidget(11, QFormLayout::FieldRole, DirectoryRenew);

        DirectoryRenewInterval = new QSpinBox(page_8);
        DirectoryRenewInterval->setObjectName(QStringLiteral("DirectoryRenewInterval"));
        DirectoryRenewInterval->setMinimumSize(QSize(100, 0));
        DirectoryRenewInterval->setMaximumSize(QSize(100, 16777215));
        DirectoryRenewInterval->setMinimum(1000);
        DirectoryRenewInterval->setMaximum(999999999);
        DirectoryRenewInterval->setValue(600000);
        formLayout_10->setWidget(12, QFormLayout::FieldRole, DirectoryRenewInterval);

        DirectoryGreedy = new QCheckBox(page_8);
        DirectoryGreedy->setObjectName(QStringLiteral("DirectoryGreedy"));
        formLayout_10->setWidget(8, QFormLayout::FieldRole, DirectoryGreedy);
        verticalLayout_7->addLayout(formLayout_10);
    }

    if(AvailableTypes.contains("page_9"))
    {
        page_9 = new QWidget(this);
        page_9->setObjectName(QStringLiteral("page_9"));
        sizePolicy2.setHeightForWidth(page_9->sizePolicy().hasHeightForWidth());
        page_9->setSizePolicy(sizePolicy2);
        verticalLayout_4 = new QVBoxLayout(page_9);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(page_9);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy2);
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        horizontalLayout_2->addItem(horizontalSpacer);
        ComboLayout = new QHBoxLayout();
        ComboLayout->setSpacing(0);
        ComboLayout->setObjectName(QStringLiteral("ComboLayout"));
        horizontalLayout_2->addLayout(ComboLayout);
        LabelValidation5 = new QLabel(widget);
        LabelValidation5->setObjectName(QStringLiteral("LabelValidation5"));
        sizePolicy2.setHeightForWidth(LabelValidation5->sizePolicy().hasHeightForWidth());
        LabelValidation5->setSizePolicy(sizePolicy2);
        LabelValidation5->setPixmap(QPixmap(QString::fromUtf8(":/engine/images/invalid.png")));
        horizontalLayout_2->addWidget(LabelValidation5);
        verticalLayout_4->addWidget(widget);
        widget_2 = new QWidget(page_9);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        verticalLayout_10 = new QVBoxLayout(widget_2);
        verticalLayout_10->setSpacing(0);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        verticalLayout_10->setContentsMargins(0, 0, 0, 0);
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
        formLayout->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        formLayout->setFormAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        formLayout->setVerticalSpacing(12);
        formLayout->setContentsMargins(30, -1, -1, 30);

        DatabaseClear = new QCheckBox(widget_2);
        DatabaseClear->setObjectName(QStringLiteral("DatabaseClear"));
        formLayout->setWidget(1, QFormLayout::FieldRole, DatabaseClear);

        DatabaseMix = new QCheckBox(widget_2);
        DatabaseMix->setObjectName(QStringLiteral("DatabaseMix"));
        formLayout->setWidget(2, QFormLayout::FieldRole, DatabaseMix);

        DatabaseMaxSuccess = new QSpinBox(widget_2);
        DatabaseMaxSuccess->setObjectName(QStringLiteral("DatabaseMaxSuccess"));
        DatabaseMaxSuccess->setMaximumSize(QSize(100, 16777215));
        DatabaseMaxSuccess->setMinimum(1);
        DatabaseMaxSuccess->setMaximum(999999999);
        formLayout->setWidget(3, QFormLayout::FieldRole, DatabaseMaxSuccess);

        DatabaseMaxFail = new QSpinBox(widget_2);
        DatabaseMaxFail->setObjectName(QStringLiteral("DatabaseMaxFail"));
        DatabaseMaxFail->setMaximumSize(QSize(100, 16777215));
        DatabaseMaxFail->setMinimum(1);
        DatabaseMaxFail->setMaximum(999999999);
        formLayout->setWidget(4, QFormLayout::FieldRole, DatabaseMaxFail);

        DatabaseMaxSimultaneous = new QSpinBox(widget_2);
        DatabaseMaxSimultaneous->setObjectName(QStringLiteral("DatabaseMaxSimultaneous"));
        DatabaseMaxSimultaneous->setMaximumSize(QSize(100, 16777215));
        DatabaseMaxSimultaneous->setMinimum(1);
        DatabaseMaxSimultaneous->setMaximum(999999999);
        formLayout->setWidget(5, QFormLayout::FieldRole, DatabaseMaxSimultaneous);

        DatabaseInterval = new QSpinBox(widget_2);
        DatabaseInterval->setObjectName(QStringLiteral("DatabaseInterval"));
        DatabaseInterval->setMaximumSize(QSize(100, 16777215));
        DatabaseInterval->setMinimum(0);
        DatabaseInterval->setMaximum(999999999);
        formLayout->setWidget(6, QFormLayout::FieldRole, DatabaseInterval);

        DatabaseGreedy = new QCheckBox(widget_2);
        DatabaseGreedy->setObjectName(QStringLiteral("DatabaseGreedy"));
        formLayout->setWidget(7, QFormLayout::FieldRole, DatabaseGreedy);

        DatabaseReload = new QCheckBox(widget_2);
        DatabaseReload->setObjectName(QStringLiteral("DatabaseReload"));
        formLayout->setWidget(8, QFormLayout::FieldRole, DatabaseReload);

        DatabaseReloadInterval = new QSpinBox(widget_2);
        DatabaseReloadInterval->setObjectName(QStringLiteral("DatabaseReloadInterval"));
        DatabaseReloadInterval->setMaximumSize(QSize(100, 16777215));
        DatabaseReloadInterval->setMinimum(10000);
        DatabaseReloadInterval->setMaximum(99999999);
        DatabaseReloadInterval->setValue(60000);
        formLayout->setWidget(9, QFormLayout::FieldRole, DatabaseReloadInterval);

        DatabaseRenew = new QCheckBox(widget_2);
        DatabaseRenew->setObjectName(QStringLiteral("DatabaseRenew"));
        formLayout->setWidget(10, QFormLayout::FieldRole, DatabaseRenew);

        DatabaseRenewInterval = new QSpinBox(widget_2);
        DatabaseRenewInterval->setObjectName(QStringLiteral("DatabaseRenewInterval"));
        DatabaseRenewInterval->setMaximumSize(QSize(100, 16777215));
        DatabaseRenewInterval->setMinimum(10000);
        DatabaseRenewInterval->setMaximum(999999999);
        DatabaseRenewInterval->setValue(60000);
        formLayout->setWidget(11, QFormLayout::FieldRole, DatabaseRenewInterval);
        verticalLayout_10->addLayout(formLayout);
        verticalLayout_4->addWidget(widget_2);
    }

}

void ChooserResourceWidget::SetDatabaseState(IDatabaseState * DatabaseState)
{
    this->DatabaseState = DatabaseState;
    connect(DatabaseState,SIGNAL(GroupsChanged()),this,SLOT(GroupsChanged()));
}

void ChooserResourceWidget::GroupsChanged()
{
    SetTableId(LastTableId);
}

void ChooserResourceWidget::SetTableId(int TableId)
{
    this->LastTableId = TableId;
    if(TableId == -1)
        return;
    QStringList Checked = MultiWidget->checkedData();
    MultiWidget->clear();

    foreach(DatabaseGroup Group, DatabaseState->GetGroups(TableId))
    {
        MultiWidget->addItem(Group.Name, Group.Id);
    }

    MultiWidget->setCheckedData(Checked);

}

int ChooserResourceWidget::GetTableId()
{
    return this->LastTableId;
}

void ChooserResourceWidget::DatabaseIndexesChanged(QList<int> Previous,QList<int> Now)
{
    if(DatabaseIndexesChangedInAction)
        return;


    DatabaseIndexesChangedInAction = true;
    if(!Previous.contains(0) && Now.contains(0))
        MultiWidget->setCheckedIndexes(QList<int>()<<0);
    else if(Previous.length() == 1 && Previous[0] == 0 && Now.length() == 2 && Now.contains(0))
    {
        QList<int> Next = Now;
        Next.removeAll(0);

        MultiWidget->setCheckedIndexes(Next);
    }

    Validate();
    DatabaseIndexesChangedInAction = false;
}


void ChooserResourceWidget::SetVisibleAdvanced(bool Visible)
{

    LastVisibleAdvanced = Visible;
    if(CurrentTabName == "page")
    {
        if(!Visible)
        {
            FixedStringValue->setMaximumWidth(100000);
            FixedStringValue->setMinimumWidth(0);
        }else
        {
            FixedStringValue->setMaximumWidth(175);
            FixedStringValue->setMinimumWidth(175);
        }
        if(label_4)
            label_4->setVisible(Visible);
    }

    if(CurrentTabName == "page_2")
    {
        if(!Visible)
        {
            FixedIntegerValue->setMaximumWidth(100000);
            FixedIntegerValue->setMinimumWidth(0);
        }else
        {
            FixedIntegerValue->setMaximumWidth(175);
            FixedIntegerValue->setMinimumWidth(175);
        }
        if(label_5)
            label_5->setVisible(Visible);
    }

    if(CurrentTabName == "page_4")
    {
        if(!Visible)
        {
            RandomStringValue->setMaximumWidth(100000);
            RandomStringValue->setMinimumWidth(0);
        }else
        {
            RandomStringValue->setMaximumWidth(150);
            RandomStringValue->setMinimumWidth(150);
        }
        if(label_8)
            label_8->setVisible(Visible);
    }

    if(CurrentTabName == "page_5")
    {
        if(!Visible)
        {
            RIMaximumValue->setMaximumWidth(100000);
            RIMaximumValue->setMinimumWidth(0);
            RIMinimumValue->setMaximumWidth(100000);
            RIMinimumValue->setMinimumWidth(0);
        }else
        {
            RIMaximumValue->setMaximumWidth(150);
            RIMaximumValue->setMinimumWidth(150);
            RIMinimumValue->setMaximumWidth(150);
            RIMinimumValue->setMinimumWidth(150);
        }

    }

    if(CurrentTabName == "page_3")
    {
        if(!Visible)
        {
            if(label)
                formLayout_8->removeWidget(label);
            formLayout_8->removeWidget(FileWrite);
            if(label_50)
                formLayout_8->removeWidget(label_50);
            formLayout_8->removeWidget(FileRead);
            if(label_3)
                formLayout_8->removeWidget(label_3);
            formLayout_8->removeWidget(FileMix);
            if(label_26)
                formLayout_8->removeWidget(label_26);
            formLayout_8->removeWidget(FileReload);
            if(label_27)
                formLayout_8->removeWidget(label_27);
            formLayout_8->removeWidget(FileReloadInterval);

            formLayout_8->removeWidget(FileMaxFail);
            formLayout_8->removeWidget(FileMaxSuccess);
            formLayout_8->removeWidget(FileMaxSimultaneous);
            formLayout_8->removeWidget(FileInterval);
            formLayout_8->removeWidget(FileGreedy);
            formLayout_8->removeWidget(FileRenew);
            formLayout_8->removeWidget(FileRenewInterval);

            if(label_18)
                formLayout_8->removeWidget(label_18);
            if(label_111)
                formLayout_8->removeWidget(label_111);
            if(label_17)
                formLayout_8->removeWidget(label_17);
            if(label_19)
                formLayout_8->removeWidget(label_19);
            if(label_7)
                formLayout_8->removeWidget(label_7);
            if(label_37)
                formLayout_8->removeWidget(label_37);
            if(label_38)
                formLayout_8->removeWidget(label_38);
            FileFilename->setMaximumWidth(100000);
            FileFilename->setMinimumWidth(0);


        }else
        {
            formLayout_8->insertRow(1,label_50,FileRead);
            formLayout_8->insertRow(2,label,FileWrite);
            formLayout_8->insertRow(3,label_3,FileMix);
            formLayout_8->insertRow(4,label_111,FileMaxSuccess);
            formLayout_8->insertRow(5,label_17,FileMaxFail);
            formLayout_8->insertRow(6,label_18,FileMaxSimultaneous);
            formLayout_8->insertRow(7,label_19,FileInterval);
            formLayout_8->insertRow(8,label_7,FileGreedy);
            formLayout_8->insertRow(9,label_26,FileReload);
            formLayout_8->insertRow(10,label_27,FileReloadInterval);
            formLayout_8->insertRow(11,label_37,FileRenew);
            formLayout_8->insertRow(12,label_38,FileRenewInterval);

            FileFilename->setMaximumWidth(100);
            FileFilename->setMinimumWidth(100);



        }
        if(label_10)
            label_10->setVisible(Visible);
        if(label)
            label->setVisible(Visible);
        if(label_3)
            label_3->setVisible(Visible);
        FileMix->setVisible(Visible);
        FileReload->setVisible(Visible);
        FileReloadInterval->setVisible(Visible);
        FileWrite->setVisible(Visible);
        FileRead->setVisible(Visible);
        if(label_50)
            label_50->setVisible(Visible);
        if(label_18)
            label_18->setVisible(Visible);
        if(label_111)
            label_111->setVisible(Visible);
        if(label_17)
            label_17->setVisible(Visible);
        if(label_19)
            label_19->setVisible(Visible);
        if(label_7)
            label_7->setVisible(Visible);
        if(label_37)
            label_37->setVisible(Visible);
        if(label_26)
            label_26->setVisible(Visible);
        if(label_27)
            label_27->setVisible(Visible);
        if(label_38)
            label_38->setVisible(Visible);
        FileMaxSimultaneous->setVisible(Visible);
        FileMaxSuccess->setVisible(Visible);
        FileMaxFail->setVisible(Visible);
        FileInterval->setVisible(Visible);
        FileGreedy->setVisible(Visible);
        FileRenew->setVisible(Visible);
        FileRenewInterval->setVisible(Visible);

        if(Visible)
        {
            FileRenew_toggled(FileRenew->isChecked());
            FileReload_toggled(FileReload->isChecked());
        }
    }


    if(CurrentTabName == "page_9")
    {
        if(!Visible)
        {
            if(label_39)
                formLayout->removeWidget(label_39);
            formLayout->removeWidget(DatabaseClear);
            if(label_40)
                formLayout->removeWidget(label_40);
            formLayout->removeWidget(DatabaseMix);
            if(label_46)
                formLayout->removeWidget(label_46);
            formLayout->removeWidget(DatabaseReload);
            if(label_47)
                formLayout->removeWidget(label_47);
            formLayout->removeWidget(DatabaseReloadInterval);


            formLayout->removeWidget(DatabaseMaxFail);
            formLayout->removeWidget(DatabaseMaxSuccess);
            formLayout->removeWidget(DatabaseMaxSimultaneous);
            formLayout->removeWidget(DatabaseInterval);
            formLayout->removeWidget(DatabaseGreedy);
            formLayout->removeWidget(DatabaseRenew);
            formLayout->removeWidget(DatabaseRenewInterval);

            if(label_41)
                formLayout->removeWidget(label_41);
            if(label_42)
                formLayout->removeWidget(label_42);
            if(label_43)
                formLayout->removeWidget(label_43);
            if(label_44)
                formLayout->removeWidget(label_44);
            if(label_45)
                formLayout->removeWidget(label_45);
            if(label_48)
                formLayout->removeWidget(label_48);
            if(label_49)
                formLayout->removeWidget(label_49);

            horizontalSpacer->changeSize(0,0, QSizePolicy::Fixed, QSizePolicy::Fixed);

            MultiWidget->setMaximumWidth(252);
            MultiWidget->setMinimumWidth(252);

        }else
        {
            formLayout->insertRow(1,label_39,DatabaseClear);
            formLayout->insertRow(2,label_40,DatabaseMix);
            formLayout->insertRow(3,label_41,DatabaseMaxSuccess);
            formLayout->insertRow(4,label_42,DatabaseMaxFail);
            formLayout->insertRow(5,label_43,DatabaseMaxSimultaneous);
            formLayout->insertRow(6,label_44,DatabaseInterval);
            formLayout->insertRow(7,label_45,DatabaseGreedy);
            formLayout->insertRow(8,label_46,DatabaseReload);
            formLayout->insertRow(9,label_47,DatabaseReloadInterval);
            formLayout->insertRow(10,label_48,DatabaseRenew);
            formLayout->insertRow(11,label_49,DatabaseRenewInterval);

            horizontalSpacer->changeSize(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);

            MultiWidget->setMaximumWidth(252);
            MultiWidget->setMinimumWidth(252);


        }
        widget_2->setVisible(Visible);

        if(label_39)
            label_39->setVisible(Visible);
        if(label_40)
            label_40->setVisible(Visible);
        if(label_46)
            label_46->setVisible(Visible);
        if(label_47)
            label_47->setVisible(Visible);
        if(label_41)
            label_41->setVisible(Visible);
        if(label_42)
            label_42->setVisible(Visible);
        if(label_43)
            label_43->setVisible(Visible);
        if(label_44)
            label_44->setVisible(Visible);
        if(label_45)
            label_45->setVisible(Visible);
        if(label_48)
            label_48->setVisible(Visible);
        if(label_49)
            label_49->setVisible(Visible);
        DatabaseMaxSimultaneous->setVisible(Visible);
        DatabaseMaxSuccess->setVisible(Visible);
        DatabaseMaxFail->setVisible(Visible);
        DatabaseInterval->setVisible(Visible);
        DatabaseGreedy->setVisible(Visible);
        DatabaseRenew->setVisible(Visible);
        DatabaseRenewInterval->setVisible(Visible);
        DatabaseClear->setVisible(Visible);
        DatabaseMix->setVisible(Visible);
        DatabaseReload->setVisible(Visible);
        DatabaseReloadInterval->setVisible(Visible);

        if(Visible)
        {
            DatabaseRenew_toggled(DatabaseRenew->isChecked());
            DatabaseReload_toggled(DatabaseReload->isChecked());
        }
    }

    if(CurrentTabName == "page_7")
    {
        if(!Visible)
        {
            if(label_13)
                formLayout_9->removeWidget(label_13);
            formLayout_9->removeWidget(UrlMix);
            if(label_29)
                formLayout_9->removeWidget(label_29);
            formLayout_9->removeWidget(UrlReload);
            if(label_28)
                formLayout_9->removeWidget(label_28);
            formLayout_9->removeWidget(UrlReloadInterval);


            formLayout_9->removeWidget(UrlMaxSuccess);
            formLayout_9->removeWidget(UrlMaxFail);
            formLayout_9->removeWidget(UrlMaxSimultaneous);
            formLayout_9->removeWidget(UrlInterval);
            formLayout_9->removeWidget(UrlGreedy);
            formLayout_9->removeWidget(UrlRenew);
            formLayout_9->removeWidget(UrlRenewInterval);


            if(label_112)
                formLayout_9->removeWidget(label_112);
            if(label_20)
                formLayout_9->removeWidget(label_20);
            if(label_21)
                formLayout_9->removeWidget(label_21);
            if(label_22)
                formLayout_9->removeWidget(label_22);
            if(label_31)
                formLayout_9->removeWidget(label_31);
            if(label_35)
                formLayout_9->removeWidget(label_35);
            if(label_36)
                formLayout_9->removeWidget(label_36);
            UrlUrl->setMaximumWidth(100000);
            UrlUrl->setMinimumWidth(0);
        }else
        {
            formLayout_9->insertRow(1,label_13,UrlMix);
            formLayout_9->insertRow(2,label_112,UrlMaxSuccess);
            formLayout_9->insertRow(3,label_20,UrlMaxFail);
            formLayout_9->insertRow(4,label_21,UrlMaxSimultaneous);
            formLayout_9->insertRow(5,label_22,UrlInterval);
            formLayout_9->insertRow(6,label_31,UrlGreedy);
            formLayout_9->insertRow(7,label_29,UrlReload);
            formLayout_9->insertRow(8,label_28,UrlReloadInterval);
            formLayout_9->insertRow(9,label_35,UrlRenew);
            formLayout_9->insertRow(10,label_36,UrlRenewInterval);

            UrlUrl->setMaximumWidth(120);
            UrlUrl->setMinimumWidth(120);

        }
        if(label_11)
            label_11->setVisible(Visible);
        if(label_13)
            label_13->setVisible(Visible);
        UrlMix->setVisible(Visible);
        if(label_28)
            label_28->setVisible(Visible);
        if(label_29)
            label_29->setVisible(Visible);
        UrlReloadInterval->setVisible(Visible);
        UrlReload->setVisible(Visible);
        if(label_112)
            label_112->setVisible(Visible);
        if(label_20)
            label_20->setVisible(Visible);
        if(label_21)
            label_21->setVisible(Visible);
        if(label_22)
            label_22->setVisible(Visible);
        UrlMaxFail->setVisible(Visible);
        UrlMaxSuccess->setVisible(Visible);
        UrlMaxSimultaneous->setVisible(Visible);
        UrlInterval->setVisible(Visible);
        UrlGreedy->setVisible(Visible);
        if(label_31)
            label_31->setVisible(Visible);
        UrlRenew->setVisible(Visible);
        UrlRenewInterval->setVisible(Visible);
        if(label_35)
            label_35->setVisible(Visible);
        if(label_36)
            label_36->setVisible(Visible);

        if(Visible)
        {
            UrlReload_toggled(UrlReload->isChecked());
            UrlRenew_toggled(UrlRenew->isChecked());
        }
    }

    if(CurrentTabName == "page_8")
    {
        if(!Visible)
        {
            if(label_16)
                formLayout_10->removeWidget(label_16);
            formLayout_10->removeWidget(DirectoryWildcard);
            if(label_14)
                formLayout_10->removeWidget(label_14);
            formLayout_10->removeWidget(DirectoryClean);
            if(label_15)
                formLayout_10->removeWidget(label_15);
            formLayout_10->removeWidget(DirectoryMix);
            if(label_6)
                formLayout_10->removeWidget(label_6);
            formLayout_10->removeWidget(DirectoryReload);
            if(label_30)
                formLayout_10->removeWidget(label_30);
            formLayout_10->removeWidget(DirectoryReloadInterval);


            formLayout_10->removeWidget(DirectoryMaxFail);
            formLayout_10->removeWidget(DirectoryMaxSimultaneous);
            formLayout_10->removeWidget(DirectoryMaxSuccess);
            formLayout_10->removeWidget(DirectoryInterval);
            formLayout_10->removeWidget(DirectoryGreedy);
            formLayout_10->removeWidget(DirectoryRenew);
            formLayout_10->removeWidget(DirectoryRenewInterval);

            if(label_113)
                formLayout_10->removeWidget(label_113);
            if(label_23)
                formLayout_10->removeWidget(label_23);
            if(label_24)
                formLayout_10->removeWidget(label_24);
            if(label_25)
                formLayout_10->removeWidget(label_25);
            if(label_32)
                formLayout_10->removeWidget(label_32);
            if(label_33)
                formLayout_10->removeWidget(label_33);
            if(label_34)
                formLayout_10->removeWidget(label_34);
            DirectoryDirectory->setMaximumWidth(100000);
            DirectoryDirectory->setMinimumWidth(0);
        }else
        {
            formLayout_10->insertRow(1,label_16,DirectoryWildcard);
            formLayout_10->insertRow(2,label_14,DirectoryClean);
            formLayout_10->insertRow(3,label_15,DirectoryMix);
            formLayout_10->insertRow(4,label_113,DirectoryMaxSuccess);
            formLayout_10->insertRow(5,label_23,DirectoryMaxFail);
            formLayout_10->insertRow(6,label_24,DirectoryMaxSimultaneous);
            formLayout_10->insertRow(7,label_25,DirectoryInterval);
            formLayout_10->insertRow(8,label_32,DirectoryGreedy);
            formLayout_10->insertRow(9,label_6,DirectoryReload);
            formLayout_10->insertRow(10,label_30,DirectoryReloadInterval);
            formLayout_10->insertRow(11,label_33,DirectoryRenew);
            formLayout_10->insertRow(12,label_34,DirectoryRenewInterval);
            DirectoryDirectory->setMaximumWidth(100);
            DirectoryDirectory->setMinimumWidth(100);
        }

        if(label_12)
            label_12->setVisible(Visible);
        if(label_16)
            label_16->setVisible(Visible);
        DirectoryWildcard->setVisible(Visible);
        if(label_14)
            label_14->setVisible(Visible);
        DirectoryClean->setVisible(Visible);
        if(label_15)
            label_15->setVisible(Visible);
        DirectoryMix->setVisible(Visible);
        if(label_6)
            label_6->setVisible(Visible);
        DirectoryReload->setVisible(Visible);
        if(label_30)
            label_30->setVisible(Visible);
        DirectoryReloadInterval->setVisible(Visible);

        if(label_113)
            label_113->setVisible(Visible);
        if(label_23)
            label_23->setVisible(Visible);
        if(label_24)
            label_24->setVisible(Visible);
        if(label_25)
            label_25->setVisible(Visible);
        DirectoryMaxFail->setVisible(Visible);
        DirectoryMaxSimultaneous->setVisible(Visible);
        DirectoryMaxSuccess->setVisible(Visible);
        DirectoryInterval->setVisible(Visible);
        if(label_32)
            label_32->setVisible(Visible);
        DirectoryGreedy->setVisible(Visible);
        DirectoryRenew->setVisible(Visible);
        DirectoryRenewInterval->setVisible(Visible);
        if(label_33)
            label_33->setVisible(Visible);
        if(label_34)
            label_34->setVisible(Visible);

        if(Visible)
        {
            DirectoryRenew_toggled(DirectoryRenew->isChecked());
            DirectoryReload_toggled(DirectoryReload->isChecked());
        }
    }


}

void ChooserResourceWidget::Validate()
{
    bool PreviousValidationRes = LastValidateResult;
    LastValidateResult = true;


    QString name = CurrentTabName;


        if(name == "page_9")
        {
            if(MultiWidget->currentIndexes().isEmpty() && ShowValidation)
            {
                LabelValidation5->setVisible(true);
                MultiWidget->setToolTip(tr("No groups selected"));

                LastValidateResult = false;
            }else
            {
                LabelValidation5->setVisible(false);

                MultiWidget->setToolTip("");
            }

        }else if(name == "page")
        {
            bool NeedValidate = false;
            QVariant p = FixedStringValue->property("NotEmpty");
            if(!p.isNull())
            {
                NeedValidate = p.toBool();
            }
            if(FixedStringValue->text().isEmpty() && ShowValidation && NeedValidate)
            {
                LabelValidation1->setVisible(true);
                FixedStringValue->setToolTip(tr("Field is empty"));

                LastValidateResult = false;
            }else
            {
                LabelValidation1->setVisible(false);

                FixedStringValue->setToolTip("");
            }

        }else if(name == "page_3")
        {

            if(ShowValidation)
            {
                bool IsValid = false;
                QString filename = FileFilename->text();
                if(filename.isEmpty())
                {
                    IsValid = !FileWrite->isChecked() && !FileRead->isChecked();
                }else
                {

                    foreach(QString f,filename.split("|"))
                    {
                        QFileInfo info(f);
                        IsValid = !FileRead->isChecked() || info.exists();
                        if(!IsValid)
                        {
                            break;
                        }

                    }

                }

                if(!IsValid)
                {
                    LabelValidation2->setVisible(true);
                    FileFilename->setToolTip(tr("File not exists"));

                    LastValidateResult = false;
                }else
                {
                    LabelValidation2->setVisible(false);
                    FileFilename->setToolTip("");
                }
            }

        }else if(name == "page_7")
        {

            if(ShowValidation)
            {
                bool IsValid = false;
                QString urlstring = UrlUrl->text();
                if(urlstring.isEmpty())
                {
                    IsValid = false;
                }else
                {

                    foreach(QString s,urlstring.split("|"))
                    {
                        QUrl url(s);
                        IsValid = url.isValid() && (url.scheme() == "http" || url.scheme() == "https") && !url.host().isEmpty();
                        if(!IsValid)
                        {
                           break;
                        }
                    }
                }

                if(!IsValid)
                {
                    LabelValidation3->setVisible(true);
                    UrlUrl->setToolTip(tr("Url not valid"));

                    LastValidateResult = false;
                }else
                {
                    LabelValidation3->setVisible(false);
                    UrlUrl->setToolTip("");
                }
            }

        }else if(name == "page_8")
        {

            if(ShowValidation)
            {
                bool IsValid = false;
                QString dir = DirectoryDirectory->text();
                if(dir.isEmpty())
                {
                    IsValid = false;
                }else
                {
                    QFileInfo info(dir);
                    IsValid = info.exists() && info.isDir();
                }

                if(!IsValid)
                {
                    LabelValidation4->setVisible(true);
                    DirectoryDirectory->setToolTip(tr("Directory not exists"));

                    LastValidateResult = false;
                }else
                {
                    LabelValidation4->setVisible(false);
                    DirectoryDirectory->setToolTip("");
                }
            }

        }



    if(PreviousValidationRes != LastValidateResult)
        emit ValidationStateChanged();

}

void ChooserResourceWidget::SetShowValidation(bool ShowValidation)
{
    this->ShowValidation = ShowValidation;
    Validate();
}

bool ChooserResourceWidget::IsValid()
{
    Validate();
    return LastValidateResult;
}

IStringBuilder * ChooserResourceWidget::GetStringBuilder()
{
    return StringBuilder;
}

void ChooserResourceWidget::SetStringBuilder(IStringBuilder * val)
{

    StringBuilder = val;

}

void ChooserResourceWidget::SetRandomStringEdit(const QString& val)
{
    RandomStringValue->setText(val);
}

void ChooserResourceWidget::setCurrentIndex(int index)
{

    int size = Widgets.length();
    for(int i = 0;i<size;i++)
    {
        Widgets.at(i)->hide();
        ui->mainLayout->removeWidget(Widgets.at(i));
    }

    QWidget* Widget = Widgets.at(index);
    ui->mainLayout->addWidget(Widget);
    Widget->show();

    CurrentTabName = Widget->objectName();


    //Widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    //Widget->adjustSize();
    //stackedWidget->adjustSize();
    SetVisibleAdvanced(LastVisibleAdvanced);
    Validate();
}

ChooserResourceWidget::~ChooserResourceWidget()
{
    delete ui;
}

QWidget* ChooserResourceWidget::GetTemplateWidgetByIndex(int index)
{
    return Widgets.at(index);
}

void ChooserResourceWidget::RemoveWidgetAtIndex(int index)
{
    AvailableTypes.removeAt(index);
    /*QWidget *widget = GetTemplateWidgetByIndex(index);
    Widgets.removeAt(index);
    delete widget;*/
}

void ChooserResourceWidget::ButtonFileSelect_clicked()
{
    if(FileRead->isChecked())
    {
        QString fileName = QFileDialog::getOpenFileNames(this, tr("Open File"), "", tr("All Files (*.*)")).join("|");
        if(fileName.length()>0)
        {
            FileFilename->setText(fileName);
        }
    }else
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("All Files (*.*)"));
        if(fileName.length()>0)
        {
            FileFilename->setText(fileName);
        }
    }

}

void ChooserResourceWidget::ButtonDirectorySelect_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"",0);
    if(directory.length()>0)
    {
        DirectoryDirectory->setText(directory);
    }
}


void ChooserResourceWidget::RandomStringEdit_clicked()
{
    disconnect(StringBuilder,SIGNAL(Changed(QString)),this,SLOT(SetRandomStringEdit(QString)));
    connect(StringBuilder,SIGNAL(Changed(QString)),this,SLOT(SetRandomStringEdit(QString)));

    StringBuilder->Show(RandomStringValue->text());
    disconnect(StringBuilder,SIGNAL(Changed(QString)),this,SLOT(SetRandomStringEdit(QString)));

}

void ChooserResourceWidget::DirectoryReload_toggled(bool checked)
{
    DirectoryReloadInterval->setVisible(checked);
    if(label_30)
        label_30->setVisible(checked);
    if(!checked)
    {
        formLayout_10->removeWidget(DirectoryReloadInterval);
        if(label_30)
            formLayout_10->removeWidget(label_30);
    }else
    {
        int position;
        formLayout_10->getWidgetPosition(DirectoryReload,&position,0);

        formLayout_10->insertRow(position + 1,label_30,DirectoryReloadInterval);
    }
}


void ChooserResourceWidget::FileReload_toggled(bool checked)
{
    FileReloadInterval->setVisible(checked);
    if(label_27)
        label_27->setVisible(checked);
    if(!checked)
    {
        formLayout_8->removeWidget(FileReloadInterval);
        if(label_27)
            formLayout_8->removeWidget(label_27);
    }else
    {
        int position;
        formLayout_8->getWidgetPosition(FileReload,&position,0);

        formLayout_8->insertRow(position + 1,label_27,FileReloadInterval);
    }
}


void ChooserResourceWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        //ui->retranslateUi(this);
        //RetranslateUi();
        break;
    default:
        break;
    }
}





void ChooserResourceWidget::UrlReload_toggled(bool checked)
{
    UrlReloadInterval->setVisible(checked);
    if(label_28)
        label_28->setVisible(checked);
    if(!checked)
    {
        formLayout_9->removeWidget(UrlReloadInterval);
        if(label_28)
            formLayout_9->removeWidget(label_28);
    }else
    {
        int position;
        formLayout_9->getWidgetPosition(UrlReload,&position,0);
        formLayout_9->insertRow(position + 1,label_28,UrlReloadInterval);
    }
}


void ChooserResourceWidget::FileRenew_toggled(bool checked)
{
    FileRenewInterval->setVisible(checked);
    if(label_38)
        label_38->setVisible(checked);
    if(!checked)
    {
        formLayout_8->removeWidget(FileRenewInterval);
        if(label_38)
            formLayout_8->removeWidget(label_38);
    }else
    {
        formLayout_8->addRow(label_38,FileRenewInterval);
    }
}

void ChooserResourceWidget::UrlRenew_toggled(bool checked)
{
    UrlRenewInterval->setVisible(checked);
    if(label_36)
        label_36->setVisible(checked);
    if(!checked)
    {
        formLayout_9->removeWidget(UrlRenewInterval);
        if(label_36)
            formLayout_9->removeWidget(label_36);
    }else
    {
        formLayout_9->addRow(label_36,UrlRenewInterval);
    }
}

void ChooserResourceWidget::DirectoryRenew_toggled(bool checked)
{
    DirectoryRenewInterval->setVisible(checked);
    if(label_34)
        label_34->setVisible(checked);
    if(!checked)
    {
        formLayout_10->removeWidget(DirectoryRenewInterval);
        if(label_34)
            formLayout_10->removeWidget(label_34);
    }else
    {
        formLayout_10->addRow(label_34,DirectoryRenewInterval);
    }
}

void ChooserResourceWidget::DatabaseReload_toggled(bool checked)
{
    DatabaseReloadInterval->setVisible(checked);
    if(label_47)
        label_47->setVisible(checked);
    if(!checked)
    {
        formLayout->removeWidget(DatabaseReloadInterval);
        if(label_47)
            formLayout->removeWidget(label_47);
    }else
    {
        int position;
        formLayout->getWidgetPosition(DatabaseReload,&position,0);

        formLayout->insertRow(position + 1,label_47,DatabaseReloadInterval);
    }
}

void ChooserResourceWidget::DatabaseRenew_toggled(bool checked)
{
    DatabaseRenewInterval->setVisible(checked);
    if(label_49)
        label_49->setVisible(checked);
    if(!checked)
    {
        formLayout->removeWidget(DatabaseRenewInterval);
        if(label_49)
            formLayout->removeWidget(label_49);
    }else
    {
        formLayout->addRow(label_49,DatabaseRenewInterval);
    }
}
