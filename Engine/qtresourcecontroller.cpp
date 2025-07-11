#include "qtresourcecontroller.h"
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>

#include <QCheckBox>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QElapsedTimer>
#include "qxtcheckcombobox.h"
#include "dragsection.h"
#include "designchooserresourcewidget.h"
#include "designresourcewrapper.h"

#include "resourcemodelfixedstring.h"
#include "resourcemodelfixedinteger.h"
#include "resourcemodelrandominteger.h"
#include "resourcemodelrandomstring.h"
#include "resourcemodelfile.h"
#include "resourcemodeldirectory.h"
#include "resourcemodelurl.h"
#include "resourcemodelselect.h"
#include "resourcemodeldatabase.h"
#include "userresourcewidget.h"
#include "multilanguagestring.h"
#include "qAccordion/qaccordion.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    QtResourceController::QtResourceController(QObject *parent) :
        IResourceController(parent), TabWidget(0), Layout(0), UseAccordion(false), UseUIConstructor(false)
    {
        AllValidator = new GeneralValidator(this);
        WidgetUIConstructor = 0;
        ConstructResource = 0;
        IsDatabaseDirty = false;
    }

    void QtResourceController::SetConstructResource(IConstructResource *ConstructResource)
    {
        this->ConstructResource = ConstructResource;
    }

    void QtResourceController::SetIsDatabaseDirty(bool IsDatabaseDirty)
    {
        this->IsDatabaseDirty = IsDatabaseDirty;
    }


    void QtResourceController::SetUseAccordion()
    {
        UseAccordion = true;
    }

    void QtResourceController::SetUseUIConstructor()
    {
        UseUIConstructor = true;
    }

    void QtResourceController::SetLanguage(const QString& Language)
    {
        this->Language = Language;
    }

    void QtResourceController::SetLanguageModel(ILanguageModel* LanguageModel)
    {
        this->LanguageModel = LanguageModel;
    }

    QWidget * QtResourceController::GetWidget()
    {
        return Widget;

    }

    void QtResourceController::SetWidget(QWidget * Widget)
    {
        this->Widget = Widget;
    }

    void QtResourceController::SetResourceWidgetFactory(IResourceWidgetFactory *WidgetFactory)
    {
        this->WidgetFactory = WidgetFactory;
    }


    bool QtResourceController::GetIncludeSections()
    {
        return IncludeSections;
    }

    void QtResourceController::SetIncludeSections(bool IncludeSections)
    {
        this->IncludeSections = IncludeSections;
    }

    IResourceModel* QtResourceController::GetModelByType(const QString& Type, IResourceWidget*r)
    {
        if(Type == "FixedString")
        {
            ResourceModelFixedString *m = new ResourceModelFixedString();
            m->SetSectionName(r->GetSectionName());
            m->SetIsAdvanced(r->GetIsAdvanced());
            m->SetVisibilityConditionValue(r->GetVisibilityConditionValue());
            m->SetVisibilityConditionVariable(r->GetVisibilityConditionVariable());
            m->SetName(r->GetVariableName());
            m->SetDescription(r->GetDescription());
            m->SetValue(r->Get("User.LineEditText.FixedStringValue").toString());
            if(r->GetAvailableNames().isEmpty())
                m->SetAllowedTypes(r->GetTypeId());
            else
                m->SetAllowedTypes(r->GetAvailableNames());
            m->SetVisibleByUser(r->GetVisibleToUser());
            m->SetEnabledByUser(r->GetEnabledToUser());

            bool e = r->Get("Advanced.ContainsCheckBox.NotEmptyCheckBox").toBool();
            if(e)
                m->SetNotEmpty(r->Get("Advanced.CheckBoxChecked.NotEmptyCheckBox").toBool());
            else
            {
                m->SetNotEmpty(r->Get("User.PropertyNotEmpty.FixedStringValue").toBool());
            }


            return m;
        }else if(Type == "FixedInteger")
        {
            ResourceModelFixedInteger *m = new ResourceModelFixedInteger();
            m->SetName(r->GetVariableName());
            m->SetIsAdvanced(r->GetIsAdvanced());
            m->SetSectionName(r->GetSectionName());
            m->SetVisibilityConditionValue(r->GetVisibilityConditionValue());
            m->SetVisibilityConditionVariable(r->GetVisibilityConditionVariable());

            m->SetDescription(r->GetDescription());
            if(r->GetAvailableNames().isEmpty())
                m->SetAllowedTypes(r->GetTypeId());
            else
                m->SetAllowedTypes(r->GetAvailableNames());
            m->SetVisibleByUser(r->GetVisibleToUser());
            m->SetEnabledByUser(r->GetEnabledToUser());
            m->SetValue(r->Get("User.SpinBoxValue.FixedIntegerValue").toInt());

            bool e = r->Get("Advanced.ContainsSpinBox.EditMinimum").toBool();
            if(e)
                m->SetMinValue(r->Get("Advanced.SpinBoxValue.EditMinimum").toInt());

            e = r->Get("Advanced.ContainsSpinBox.EditMaximum").toBool();
            if(e)
                m->SetMaxValue(r->Get("Advanced.SpinBoxValue.EditMaximum").toInt());

            return m;
        }else if(Type == "RandomString")
        {
            ResourceModelRandomString *m = new ResourceModelRandomString();
            m->SetSectionName(r->GetSectionName());
            m->SetIsAdvanced(r->GetIsAdvanced());
            m->SetVisibilityConditionValue(r->GetVisibilityConditionValue());
            m->SetVisibilityConditionVariable(r->GetVisibilityConditionVariable());
            m->SetName(r->GetVariableName());
            m->SetDescription(r->GetDescription());
            m->SetValue(r->Get("User.LineEditText.RandomStringValue").toString());
            if(r->GetAvailableNames().isEmpty())
                m->SetAllowedTypes(r->GetTypeId());
            else
                m->SetAllowedTypes(r->GetAvailableNames());
            m->SetVisibleByUser(r->GetVisibleToUser());
            m->SetEnabledByUser(r->GetEnabledToUser());
            return m;
        }else if(Type == "RandomInteger")
        {
            ResourceModelRandomInteger *m = new ResourceModelRandomInteger();
            m->SetSectionName(r->GetSectionName());
            m->SetIsAdvanced(r->GetIsAdvanced());
            m->SetVisibilityConditionValue(r->GetVisibilityConditionValue());
            m->SetVisibilityConditionVariable(r->GetVisibilityConditionVariable());
            m->SetName(r->GetVariableName());
            m->SetDescription(r->GetDescription());
            if(r->GetAvailableNames().isEmpty())
                m->SetAllowedTypes(r->GetTypeId());
            else
                m->SetAllowedTypes(r->GetAvailableNames());
            m->SetVisibleByUser(r->GetVisibleToUser());
            m->SetEnabledByUser(r->GetEnabledToUser());
            m->SetMinUserValue(r->Get("User.SpinBoxValue.RIMinimumValue").toInt());
            m->SetMaxUserValue(r->Get("User.SpinBoxValue.RIMaximumValue").toInt());


            bool e = r->Get("Advanced.ContainsSpinBox.EditMinimum_2").toBool();
            if(e)
                m->SetMinDesignValue(r->Get("Advanced.SpinBoxValue.EditMinimum_2").toInt());

            e = r->Get("Advanced.ContainsSpinBox.EditMaximum_2").toBool();
            if(e)
                m->SetMaxDesignValue(r->Get("Advanced.SpinBoxValue.EditMaximum_2").toInt());

            return m;
        }else if(Type == "LinesFromFile")
        {
            ResourceModelFile *m = new ResourceModelFile();
            m->SetSectionName(r->GetSectionName());
            m->SetIsAdvanced(r->GetIsAdvanced());
            m->SetVisibilityConditionValue(r->GetVisibilityConditionValue());
            m->SetVisibilityConditionVariable(r->GetVisibilityConditionVariable());
            m->SetName(r->GetVariableName());
            m->SetDescription(r->GetDescription());
            m->SetFilename(r->Get("User.LineEditText.FileFilename").toString());
            m->SetRead(r->Get("User.CheckBoxChecked.FileRead").toBool());
            m->SetWrite(r->Get("User.CheckBoxChecked.FileWrite").toBool());
            m->SetMix(r->Get("User.CheckBoxChecked.FileMix").toBool());
            m->SetSuccessAttempts(r->Get("User.SpinBoxValue.FileMaxSuccess").toInt());
            m->SetFailAttempts(r->Get("User.SpinBoxValue.FileMaxFail").toInt());
            m->SetNumberSimultaneousUse(r->Get("User.SpinBoxValue.FileMaxSimultaneous").toInt());
            m->SetIntervalBetweenUsage(r->Get("User.SpinBoxValue.FileInterval").toInt());
            if(r->Get("User.CheckBoxChecked.FileReload").toBool())
                m->SetLoadInterval(r->Get("User.SpinBoxValue.FileReloadInterval").toInt());
            else
                m->SetLoadInterval(0);

            if(r->Get("User.CheckBoxChecked.FileRenew").toBool())
                m->SetRenewInterval(r->Get("User.SpinBoxValue.FileRenewInterval").toInt());
            else
                m->SetRenewInterval(-1);

            if(r->GetAvailableNames().isEmpty())
                m->SetAllowedTypes(r->GetTypeId());
            else
                m->SetAllowedTypes(r->GetAvailableNames());
            m->SetVisibleByUser(r->GetVisibleToUser());
            m->SetEnabledByUser(r->GetEnabledToUser());
            m->SetGreedy(r->Get("User.CheckBoxChecked.FileGreedy").toBool());

            return m;
        }else if(Type == "Database")
        {
            ResourceModelDatabase *m = new ResourceModelDatabase();
            m->SetSectionName(r->GetSectionName());
            m->SetIsAdvanced(r->GetIsAdvanced());
            m->SetVisibilityConditionValue(r->GetVisibilityConditionValue());
            m->SetVisibilityConditionVariable(r->GetVisibilityConditionVariable());
            m->SetName(r->GetVariableName());
            m->SetDescription(r->GetDescription());

            m->SetGroupList(r->Get("User.ComboBoxMulti.DatabaseGroups").toStringList());

            if(r->Get("Advanced.ContainsCombobox.NotEmptyCheckBox").toBool())
            {
                if(IsDatabaseDirty)
                {
                    m->SetTableId(r->Get("OriginalDatabaseTableId").toInt());
                    QList<int> ColumnIds;
                    for(QString& ColumnIdString: r->Get("OriginalDatabaseColumnsIds").toStringList())
                    {
                        ColumnIds.append(ColumnIdString.toInt());
                    }
                    m->SetColumnIds(ColumnIds);


                }else
                {
                    QString ComboboxString = r->Get("Advanced.ComboboxCurrentData.TableCombobox").toString();
                    m->SetTableId(ComboboxString.toInt());
                    QList<int> ColumnIds;
                    for(QString& ColumnIdString: r->Get("DatabaseColumnsIds").toStringList())
                    {
                        ColumnIds.append(ColumnIdString.toInt());
                    }
                    m->SetColumnIds(ColumnIds);
                }
            }else
            {
                m->SetTableId(r->GetTableId());
                QList<int> ColumnIds;
                for(QString& ColumnIdString: r->Get("User.PropertyUserDatabaseColumnsIds.All").toStringList())
                {
                    ColumnIds.append(ColumnIdString.toInt());
                }
                m->SetColumnIds(ColumnIds);
            }



            m->SetClean(r->Get("User.CheckBoxChecked.DatabaseClear").toBool());
            m->SetMix(r->Get("User.CheckBoxChecked.DatabaseMix").toBool());
            m->SetSuccessAttempts(r->Get("User.SpinBoxValue.DatabaseMaxSuccess").toInt());
            m->SetFailAttempts(r->Get("User.SpinBoxValue.DatabaseMaxFail").toInt());
            m->SetNumberSimultaneousUse(r->Get("User.SpinBoxValue.DatabaseMaxSimultaneous").toInt());
            m->SetIntervalBetweenUsage(r->Get("User.SpinBoxValue.DatabaseInterval").toInt());
            if(r->Get("User.CheckBoxChecked.DatabaseReload").toBool())
                m->SetLoadInterval(r->Get("User.SpinBoxValue.DatabaseReloadInterval").toInt());
            else
                m->SetLoadInterval(0);
            if(r->Get("User.CheckBoxChecked.DatabaseRenew").toBool())
                m->SetRenewInterval(r->Get("User.SpinBoxValue.DatabaseRenewInterval").toInt());
            else
                m->SetRenewInterval(-1);

            if(r->GetAvailableNames().isEmpty())
                m->SetAllowedTypes(r->GetTypeId());
            else
                m->SetAllowedTypes(r->GetAvailableNames());
            m->SetVisibleByUser(r->GetVisibleToUser());
            m->SetEnabledByUser(r->GetEnabledToUser());
            m->SetGreedy(r->Get("User.CheckBoxChecked.DatabaseGreedy").toBool());


            return m;
        }

        else if(Type == "FilesFromDirectory")
        {
            ResourceModelDirectory *m = new ResourceModelDirectory();
            m->SetSectionName(r->GetSectionName());
            m->SetIsAdvanced(r->GetIsAdvanced());
            m->SetVisibilityConditionValue(r->GetVisibilityConditionValue());
            m->SetVisibilityConditionVariable(r->GetVisibilityConditionVariable());
            m->SetName(r->GetVariableName());
            m->SetDescription(r->GetDescription());

            m->SetDirectory(r->Get("User.LineEditText.DirectoryDirectory").toString());
            m->SetWildcard(r->Get("User.LineEditText.DirectoryWildcard").toString());
            m->SetClean(r->Get("User.CheckBoxChecked.DirectoryClean").toBool());
            m->SetMix(r->Get("User.CheckBoxChecked.DirectoryMix").toBool());
            m->SetSuccessAttempts(r->Get("User.SpinBoxValue.DirectoryMaxSuccess").toInt());
            m->SetFailAttempts(r->Get("User.SpinBoxValue.DirectoryMaxFail").toInt());
            m->SetNumberSimultaneousUse(r->Get("User.SpinBoxValue.DirectoryMaxSimultaneous").toInt());
            m->SetIntervalBetweenUsage(r->Get("User.SpinBoxValue.DirectoryInterval").toInt());
            if(r->Get("User.CheckBoxChecked.DirectoryReload").toBool())
                m->SetLoadInterval(r->Get("User.SpinBoxValue.DirectoryReloadInterval").toInt());
            else
                m->SetLoadInterval(0);

            if(r->Get("User.CheckBoxChecked.DirectoryRenew").toBool())
                m->SetRenewInterval(r->Get("User.SpinBoxValue.DirectoryRenewInterval").toInt());
            else
                m->SetRenewInterval(-1);

            if(r->GetAvailableNames().isEmpty())
                m->SetAllowedTypes(r->GetTypeId());
            else
                m->SetAllowedTypes(r->GetAvailableNames());
            m->SetVisibleByUser(r->GetVisibleToUser());
            m->SetEnabledByUser(r->GetEnabledToUser());


            m->SetGreedy(r->Get("User.CheckBoxChecked.DirectoryGreedy").toBool());



            return m;
        }else if(Type == "LinesFromUrl")
        {
            ResourceModelUrl *m = new ResourceModelUrl();
            m->SetSectionName(r->GetSectionName());
            m->SetIsAdvanced(r->GetIsAdvanced());
            m->SetVisibilityConditionValue(r->GetVisibilityConditionValue());
            m->SetVisibilityConditionVariable(r->GetVisibilityConditionVariable());
            m->SetName(r->GetVariableName());
            m->SetDescription(r->GetDescription());

            m->SetUrl(r->Get("User.LineEditText.UrlUrl").toString());
            m->SetMix(r->Get("User.CheckBoxChecked.UrlMix").toBool());
            m->SetSuccessAttempts(r->Get("User.SpinBoxValue.UrlMaxSuccess").toInt());
            m->SetFailAttempts(r->Get("User.SpinBoxValue.UrlMaxFail").toInt());
            m->SetNumberSimultaneousUse(r->Get("User.SpinBoxValue.UrlMaxSimultaneous").toInt());
            m->SetIntervalBetweenUsage(r->Get("User.SpinBoxValue.UrlInterval").toInt());

            bool combo = r->Get("Advanced.ContainsCombobox.CaptureMethod").toBool();
            if(combo)
                m->SetCaptureMethod(r->Get("Advanced.ComboboxIndex.CaptureMethod").toInt());
            else
                m->SetCaptureMethod(r->Get("User.PropertyCaptureMethod.UrlUrl").toInt());


            bool edit = r->Get("Advanced.ContainsLineEdit.CaptureArgument").toBool();
            if(edit)
                m->SetCaptureArgument(r->Get("Advanced.LineEditText.CaptureArgument").toString());
            else
            {
                m->SetCaptureArgument(r->Get("User.PropertyCaptureArgument.UrlUrl").toString());
            }


            if(r->Get("User.CheckBoxChecked.UrlRenew").toBool())
                m->SetRenewInterval(r->Get("User.SpinBoxValue.UrlRenewInterval").toInt());
            else
                m->SetRenewInterval(-1);


            if(r->Get("User.CheckBoxChecked.UrlReload").toBool())
                m->SetLoadInterval(r->Get("User.SpinBoxValue.UrlReloadInterval").toInt());
            else
                m->SetLoadInterval(-r->Get("User.SpinBoxValue.UrlReloadInterval").toInt());

            if(r->GetAvailableNames().isEmpty())
                m->SetAllowedTypes(r->GetTypeId());
            else
                m->SetAllowedTypes(r->GetAvailableNames());
            m->SetVisibleByUser(r->GetVisibleToUser());
            m->SetEnabledByUser(r->GetEnabledToUser());

            m->SetGreedy(r->Get("User.CheckBoxChecked.UrlGreedy").toBool());

            return m;
        }else if(Type == "Select")
        {
            ResourceModelSelect *m = new ResourceModelSelect();
            m->SetSectionName(r->GetSectionName());
            m->SetIsAdvanced(r->GetIsAdvanced());
            m->SetVisibilityConditionValue(r->GetVisibilityConditionValue());
            m->SetVisibilityConditionVariable(r->GetVisibilityConditionVariable());
            m->SetName(r->GetVariableName());
            m->SetDescription(r->GetDescription());

            QString MultiselectType = r->Get("User.MultiselectType").toString();
            QStringList MultiselectValues = r->Get("User.MultiselectValues").toStringList();

            m->SetValues(MultiselectValues);

            QList<int> SelectedIndex;
            for(QString& SelectedString: r->Get("User.MultiselectIndexes").toStringList())
            {
                SelectedIndex.append(SelectedString.toInt());
            }
            m->SetSelected(SelectedIndex);

            if(MultiselectType == "Radio")
                m->SetSelectType(ResourceModelSelect::Radio);
            if(MultiselectType == "Check")
                m->SetSelectType(ResourceModelSelect::Check);
            if(MultiselectType == "Combo")
                m->SetSelectType(ResourceModelSelect::Combo);
            if(MultiselectType == "DragAndDrop")
                m->SetSelectType(ResourceModelSelect::DragAndDrop);


            if(r->GetAvailableNames().isEmpty())
                m->SetAllowedTypes(r->GetTypeId());
            else
                m->SetAllowedTypes(r->GetAvailableNames());
            m->SetVisibleByUser(r->GetVisibleToUser());
            m->SetEnabledByUser(r->GetEnabledToUser());
            return m;

        }
        return 0;

    }

    void QtResourceController::CreateUIConstructorWidgetIfNeeded()
    {
        if(!WidgetUIConstructor)
        {
            UIConstructor *a = new UIConstructor(Widget);
            QLabel *lab = new QLabel(Widget);
            lab->setOpenExternalLinks(true);
            lab->setText(

                        QString("<div>%1</div>")
                        .arg(tr("Looking for customizable user interface for your scripts?"))

                        + QString("<div>%1 <a style='color:white' href='https://bablosoft.com/bas/interface'>%2</a> (%3).</div>")
                        .arg(tr("Check"))
                        .arg(tr("interface constructor"))
                        .arg(tr("need premium"))

                        + QString("<div>%1 <a style='color:white' href='https://bablosoft.com/interface-builder/index.html'>%2</a> %3.</div>")
                        .arg(tr("See free"))
                        .arg(tr("demo"))
                        .arg(tr("to test how it works"))

                        + QString("<div>%1 <a style='color:white' href='https://bablosoft.com/test-interface/index.html'>%2</a>.</div>")
                        .arg(tr("And final result as"))
                        .arg(tr("generated interface"))

                         + QString("<div style='margin-top:20px;color:gray'>%1</div>")
                        .arg(tr("This is how user interface will look like:"))



                        );
            QWidget *b = new QWidget(Widget);
            QVBoxLayout *l = new QVBoxLayout(Widget);
            lab->setWordWrap(true);
            lab->setAlignment(Qt::AlignHCenter);
            lab->setMargin(5);
            l->setAlignment(Qt::AlignTop);
            l->addWidget(lab);
            l->addWidget(a);
            l->addWidget(b);
            Widget->setLayout(l);
            WidgetUIConstructor = a;
            Widget = b;
            Widget->setVisible(false);

        }else
        {
            //Clear old widgets
            UIConstructor *w = new UIConstructor(WidgetUIConstructor->parentWidget());
            ((QVBoxLayout *)WidgetUIConstructor->parentWidget()->layout())->insertWidget(1,w);
            WidgetUIConstructor->deleteLater();
            WidgetUIConstructor = w;
        }
        WidgetUIConstructor->SetLanguage(Language);

        connect(WidgetUIConstructor,SIGNAL(RemoveWidget(QWidget*)),this,SLOT(DeleteUnit(QWidget*)));
        connect(WidgetUIConstructor,SIGNAL(MoveUnitInsideTab(QWidget*,QWidget*,bool)),this,SLOT(MoveUnit(QWidget*,QWidget*,bool)));
        connect(WidgetUIConstructor,SIGNAL(UpdateClearState()),this,SLOT(UpdateClearState()));
        connect(WidgetUIConstructor,SIGNAL(ChangeTab(QWidget*,MultiLanguageString&)),this,SLOT(ChangeUnitTab(QWidget*,MultiLanguageString&)));
    }

    void QtResourceController::UpdateClearState()
    {
        if(WidgetUIConstructor && WidgetUIConstructor->IsClear())
            emit WidgetsEmpty();
    }

    void QtResourceController::DeleteUnit(QWidget* From)
    {
        From->deleteLater();
        UpdateClearState();
    }

    void QtResourceController::MoveUnit(QWidget* From, QWidget* To,bool After)
    {
        QVBoxLayout *layout =  dynamic_cast<QVBoxLayout *>(From->parentWidget()->layout());
        int index = -1;
        if(From == To)
        {
            index = layout->indexOf(To);
        }
        layout->removeWidget(From);

        if(!To)
        {
            index = 0;
        }else
        {
            if(index<0)
                index = layout->indexOf(To);

            if(After)
            {
                index++;
            }
            else
            {
                //index--;
            }
        }

        layout->insertWidget(index,From);

    }

    void QtResourceController::ChangeUnitTab(QWidget* From,MultiLanguageString& TabName)
    {
        foreach(QObject * i,From->children())
        {
            IResourceWidget* w = qobject_cast<IResourceWidget*>(i);

            if(w)
            {
                w->SetSectionName(TabName);
            }
        }
    }

    void QtResourceController::FromViewToModel(IResources * resources, bool Clear)
    {
        if(Clear)
            DeleteAllModel(resources);

        QList<QWidget *> list;
        int size = Widget->layout()->count();
        for(int i = 0;i<size;i++)
        {
            QLayoutItem * item = Widget->layout()->itemAt(i);
            if(!item)
                continue;
            QWidget *widget = item->widget();
            if(!widget)
                continue;
            list.append(widget->findChildren<QWidget *>());
            list.append(widget);
        }

        //QList<QWidget *> list = Widget->findChildren<QWidget *>();
        list.append(Widget);


        //list.append(Widget);
        //QList<QWidget *> listsearch;


        //bool AccordionFound = false;
        /*foreach(QWidget * widget,list)
        {
            QAccordion* accordion = qobject_cast<QAccordion*>(widget);
            if(accordion)
            {
                int size = accordion->numberOfContentPanes();
                for(int i = 0;i<size;i++)
                {
                    listsearch += accordion->getContentPane(i)->getContentFrame()->findChildren<QWidget *>();
                }
                AccordionFound = true;
                break;
            }
        }*/

        /*if(!AccordionFound)
        {
            listsearch = list;
        }*/

        foreach(QWidget * widget,list)
        {
            foreach(QObject * i,widget->children())
            {
                IResourceWidget* r = qobject_cast<IResourceWidget*>(i);

                if(r)
                {
                    IResourceModel *m = GetModelByType(r->GetTypeId(),r);
                    m->setParent(resources);
                    resources->GetModelList()->append(m);
                    QStringList types = r->GetAvailableNames().split(",");
                    QList<IResourceModel*> Defaults;
                    foreach(QString type, types)
                    {
                        if(type != r->GetTypeId())
                        {
                            IResourceModel *md = GetModelByType(type,r);
                            if(md)
                            {
                                md->setParent(resources);
                                Defaults.append(md);
                            }
                        }
                    }
                    m->SetDefaults(Defaults);

                }
            }
        }

    }

    void QtResourceController::GetTabWiget()
    {
        if(GetIncludeSections())
        {
            if(!TabWidget)
            {
                QLayout *LayoutSection;
                if(UseAccordion)
                {
                    LayoutSection = new FlowLayout(Widget);
                }else
                {
                    LayoutSection = new QVBoxLayout(Widget);
                }
                LayoutSection->setContentsMargins(0,0,0,0);
                LayoutSection->setSpacing(0);
                LayoutSection->setAlignment(Qt::AlignTop);
                Widget->setLayout(LayoutSection);
                TabWidget = new ExtendedTabWidget();
                Validator = new ExtendedTabWidgetValidator(TabWidget);


                LayoutSection->addWidget(TabWidget);
            }
        }else
        {
            if(!Layout)
            {
                if(UseAccordion)
                {
                    Layout = new FlowLayout(Widget);
                }else
                {
                    Layout = new QVBoxLayout(Widget);
                }
                Layout->setContentsMargins(0,0,0,0);
                Layout->setSpacing(0);
                Layout->setAlignment(Qt::AlignTop);
                Widget->setLayout(Layout);
            }
        }

    }

    QLayout * QtResourceController::GetLayoutForSection(const QString& SectionName)
    {
        GetTabWiget();
        if(GetIncludeSections())
        {
            QStringList list = TabWidget->pageTitleList();
            int index = list.indexOf(SectionName);
            if(index<0)
            {
                QWidget *widget = new QWidget(TabWidget);
                QLayout * layout;
                if(UseAccordion)
                {
                    layout = new FlowLayout(widget);
                }else
                {
                    layout = new QVBoxLayout(widget);
                }
                layout->setContentsMargins(0,0,0,0);
                layout->setSpacing(0);
                layout->setAlignment(Qt::AlignTop);


                widget->setLayout(layout);
                TabWidget->addTab(widget,SectionName);
                return layout;
            }else
            {
                QWidget *widget = TabWidget->widget(index);
                return dynamic_cast<QLayout *>(widget->layout());
            }

        }else
        {
            return Layout;
        }

    }

    void QtResourceController::DeleteAllView()
    {
        QList<QWidget *> list = Widget->findChildren<QWidget *>();
        foreach(QWidget * w,list)
        {
            if(w->parentWidget() == Widget)
            {
                w->deleteLater();
            }
        }
        emit WidgetsEmpty();
        if(UseUIConstructor)
        {
            CreateUIConstructorWidgetIfNeeded();
            //Widget->setVisible(false);
        }

    }

    void QtResourceController::DeleteAllModel(IResources * resources)
    {
        int len = resources->GetModelList()->size();
        for(int i = 0;i<len;i++)
        {
            IResourceModel* w = resources->GetModelList()->at(i);
            delete w;
        }

        resources->GetModelList()->clear();
    }

    void QtResourceController::AddWidgetToView()
    {
        IResourceWidget *w = WidgetFactory->CreateObject();
        w->SetLanguage(Language);
        w->SetLanguageModel(LanguageModel);

        w->SetTypeId("FixedStringValue");

        if(ConstructResource)
        {
            QStringList groups;
            QHash<QString,QString> VariablesAndValues;

            /*QList<QGroupBox*> Widgets = Widget->findChildren<QGroupBox*>("DesignResourceWidget");

            for(QGroupBox* group:Widgets)
            {
                foreach(QObject * i,group->children())
                {
                    IResourceWidget* w = qobject_cast<IResourceWidget*>(i);

                    if(w)
                    {
                        groups.append(w->GetSectionName().GetTranslation("en") + QString("|") + w->GetSectionName().GetTranslation("ru"));
                        if(w->GetTypeId() == "Select")
                        {
                            QPlainTextEdit * SelectValuesEdit = w->GetTemplateWidgetByType(false,w->GetTypeId())->findChild<QPlainTextEdit *>("SelectValuesEdit");
                            if(SelectValuesEdit)
                            {
                                VariablesAndValues.insert(w->GetVariableName(), SelectValuesEdit->toPlainText());
                            }
                        }
                    }
                }
            }*/

            QList<DesignResourceWrapper *> Widgets = Widget->findChildren<DesignResourceWrapper *>();

            foreach(DesignResourceWrapper * w,Widgets)
            {
                groups.append(w->GetSectionName().GetTranslation("en") + QString("|") + w->GetSectionName().GetTranslation("ru"));
                if(w->GetTypeId() == "Select")
                {
                    QStringList Values = w->Get("User.MultiselectValues").toStringList();
                    VariablesAndValues.insert(w->GetVariableName(), Values.join("\r\n"));
                }
            }



            groups.removeDuplicates();

            ConstructResource->SetGroups(groups);
            ConstructResource->SetSelectsVariablesAndValues(VariablesAndValues);
            if(!ConstructResource->ConstructResource(w))
            {
                w->deleteLater();
                return;
            }
        }
        if(w->GetVariableName().isEmpty())
        {
            w->SetVariableName("place_variable_name");
            MultiLanguageString str;
            str.SetTranslation("en","PLEASE FILL DESCRIPTION");
            str.SetTranslation("ru",QString::fromStdWString(std::wstring(L"\x0412\x0412\x0415\x0414\x0418\x0422\x0415\x0020\x041E\x041F\x0418\x0421\x0410\x041D\x0418\x0415")));
            w->SetDescription(str);
        }


        if(UseAccordion)
        {
            QLayout * Layout = GetLayoutForSection("");
            QAccordion *accordion = 0;

            /* Find accordion */
            int size = Layout->count();
            for(int i = 0;i<size;i++)
            {
                QLayoutItem *item = Layout->itemAt(i);
                QWidget *w = item->widget();
                if(w)
                {

                    accordion = qobject_cast<QAccordion *>(w);
                    if(accordion)
                    {
                        break;
                    }
                }
            }
            if(accordion == 0)
            {
                accordion = new QAccordion(Widget);
                Layout->addWidget(accordion);
            }

            if (accordion->insertContentPane(0,QString::number(qrand() % 100000) + QString(":") + w->GetVariableName()))
            {
                QFrame *contentFrame = accordion->getContentPane(0)->getContentFrame();
                accordion->getContentPane(0)->setMaximumHeight(3000);

                contentFrame->setLayout(new QVBoxLayout());
                contentFrame->layout()->addWidget(w->GetMainWidget());
                w->GetMainWidget()->setParent(contentFrame);
                accordion->getContentPane(0)->headerClicked(QPoint());

                connect(w,SIGNAL(VariableNameChanged(QString)),accordion->getContentPane(0),SLOT(setHeader(QString)));
                connect(w,SIGNAL(ResourceDestroyed()),accordion->getContentPane(0),SLOT(selfRemove()));
                connect(w,SIGNAL(ResourceDestroyed()),accordion->getContentPane(0),SLOT(deleteLater()));
                if(UseAccordion || UseUIConstructor)
                    connect(w,SIGNAL(ResourceDestroyed()),this,SLOT(ResourceDestroyed()));

                connect(w,SIGNAL(Up(int)),accordion->getContentPane(0),SLOT(selfUp()));
                connect(w,SIGNAL(Down(int)),accordion->getContentPane(0),SLOT(selfDown()));
            }


        }
        else
        {
            GetLayoutForSection("")->addWidget(w->GetMainWidget());
            //connect(w,SIGNAL(Up(int)),GetLayoutForSection(""),SLOT(moveItemUp(int)));
            //connect(w,SIGNAL(Down(int)),GetLayoutForSection(""),SLOT(moveItemDown(int)));
        }
        if(WidgetUIConstructor)
        {
            WidgetUIConstructor->AddUnitToCurrentTab(w->GetVariableName(),w->GetMainWidget());
        }
        emit WidgetsNotEmpty();
    }

    void QtResourceController::ResourceDestroyed()
    {
        if(!Widget)
        {
            emit WidgetsEmpty();
            return;
        }
        QAccordion* accordion = Widget->findChild<QAccordion *>();
        if(accordion)
        {
            int size = accordion->numberOfContentPanes();
            if(size>0)
            {
                emit WidgetsNotEmpty();
            }else
            {
                emit WidgetsEmpty();
            }
        }else
        {
            emit WidgetsEmpty();
        }
    }

    void QtResourceController::CheckIfAllSectionsAreEmpty(IResources * resources)
    {
        int size = resources->GetModelList()->size();
        for(int i = 0;i<size;i++)
        {
            IResourceModel * res = resources->GetModelList()->at(i);
            if(!res->GetSectionName().GetTranslation(Language).isEmpty())
            {
                return;
            }
        }
        SetIncludeSections(false);

    }

    void QtResourceController::ShowModel(const QString &Type, IResourceModel* res, IResourceWidget *w, QHash<QString, QPair<MultiSelect*, IResourceWidget *> >& Triggers)
    {
        if(Type == "FixedString")
        {
            ResourceModelFixedString * r = qobject_cast<ResourceModelFixedString *>(res);
            if(w->Get("Advanced.ContainsCheckBox.NotEmptyCheckBox").toBool())
            {
                w->Set("Advanced.CheckBoxChecked.NotEmptyCheckBox",QVariant(r->GetNotEmpty()));
            }else
            {
                w->Set("User.PropertyNotEmpty.FixedStringValue", QVariant((bool)r->GetNotEmpty()));
            }
            w->Set("User.LineEditText.FixedStringValue", QVariant("1"));
            w->Set("User.LineEditText.FixedStringValue", QVariant("2"));
            w->Set("User.LineEditText.FixedStringValue", QVariant(r->GetValue()));
        }else if(Type == "FixedInteger")
        {
            ResourceModelFixedInteger * r = qobject_cast<ResourceModelFixedInteger *>(res);

            w->Set("User.SpinBoxMinValue.FixedIntegerValue", QVariant(r->GetMinValue()));
            w->Set("User.SpinBoxMaxValue.FixedIntegerValue", QVariant(r->GetMaxValue()));
            w->Set("User.SpinBoxValue.FixedIntegerValue", QVariant(r->GetValue()));

            if(w->Get("Advanced.ContainsSpinBox.EditMinimum").toBool())
            {
                w->Set("Advanced.SpinBoxValue.EditMinimum", r->GetMinValue());
                w->Set("Advanced.SpinBoxValue.EditMaximum", r->GetMaxValue());
            }

        }else if(Type == "RandomString")
        {
            ResourceModelRandomString * r = qobject_cast<ResourceModelRandomString *>(res);
            w->Set("User.LineEditText.RandomStringValue", QVariant(r->GetValue()));
        }else if(Type == "RandomInteger")
        {
            ResourceModelRandomInteger * r = qobject_cast<ResourceModelRandomInteger *>(res);

            w->Set("User.SpinBoxMinValue.RIMaximumValue", QVariant(r->GetMinDesignValue()));
            w->Set("User.SpinBoxMaxValue.RIMaximumValue", QVariant(r->GetMaxDesignValue()));
            w->Set("User.SpinBoxMinValue.RIMinimumValue", QVariant(r->GetMinDesignValue()));
            w->Set("User.SpinBoxMaxValue.RIMinimumValue", QVariant(r->GetMaxDesignValue()));

            w->Set("User.SpinBoxValue.RIMinimumValue", QVariant(r->GetMinUserValue()));
            w->Set("User.SpinBoxValue.RIMaximumValue", QVariant(r->GetMaxUserValue()));

            if(w->Get("Advanced.ContainsSpinBox.EditMinimum_2").toBool())
            {
                w->Set("Advanced.SpinBoxValue.EditMinimum_2", QVariant(r->GetMinDesignValue()));
                w->Set("Advanced.SpinBoxValue.EditMaximum_2", QVariant(r->GetMaxDesignValue()));
            }

        }else if(Type == "LinesFromFile")
        {
            ResourceModelFile * r = qobject_cast<ResourceModelFile *>(res);
            w->Set("User.LineEditText.FileFilename", QVariant(r->GetFilename()));
            w->Set("User.CheckBoxChecked.FileWrite", QVariant(r->GetWrite()));
            w->Set("User.CheckBoxChecked.FileRead", QVariant(r->GetRead()));
            w->Set("User.CheckBoxChecked.FileMix", QVariant(r->GetMix()));
            w->Set("User.SpinBoxValue.FileMaxSuccess", QVariant(r->GetSuccessAttempts()));
            w->Set("User.SpinBoxValue.FileMaxFail", QVariant(r->GetFailAttempts()));
            w->Set("User.SpinBoxValue.FileMaxSimultaneous", QVariant(r->GetNumberSimultaneousUse()));
            w->Set("User.SpinBoxValue.FileInterval", QVariant(r->GetIntervalBetweenUsage()));
            w->Set("User.CheckBoxChecked.FileReload", QVariant(r->GetLoadInterval() > 0));
            w->Set("User.SpinBoxValue.FileReloadInterval", QVariant(r->GetLoadInterval()));
            w->Set("User.CheckBoxChecked.FileRenew", QVariant(r->GetRenewInterval() > 0));
            w->Set("User.SpinBoxValue.FileRenewInterval", QVariant(r->GetRenewInterval()));
            w->Set("User.CheckBoxChecked.FileGreedy", QVariant(r->GetGreedy()));

        }else if(Type == "Database")
        {
            ResourceModelDatabase * r = qobject_cast<ResourceModelDatabase *>(res);
            w->SetTableId(r->GetTableId());

            w->Set("User.ComboBoxMulti.DatabaseGroups", QVariant(r->GetGroupList()));
            if(w->Get("Advanced.ContainsCombobox.TableCombobox").toBool())
            {
                /*int index = -1;

                for(int i = 0;i < TableCombobox->count();i++)
                {
                    if(TableCombobox->itemData(i).toInt() == r->GetTableId())
                    {
                        index = i;
                        break;
                    }
                }
                if(index>=0)
                    TableCombobox->setCurrentIndex(index);*/

                w->Set("Advanced.ComboboxCurrentData.TableCombobox", QVariant(QString::number(r->GetTableId())));

                w->Set("OriginalDatabaseTableId", QVariant(r->GetTableId()));
                QList<int> DataOriginal = r->GetColumnIds();
                QStringList Data;
                for(int Selected:DataOriginal)
                {
                    Data.append(QString::number(Selected));
                }

                w->Set("DatabaseColumnsIds", QVariant(Data));
            }else
            {
                QList<int> DataOriginal = r->GetColumnIds();
                QStringList Data;
                for(int Selected:DataOriginal)
                {
                    Data.append(QString::number(Selected));
                }

                w->Set("User.PropertyUserDatabaseColumnsIds.All", QVariant(Data));
            }


            w->Set("User.CheckBoxChecked.DatabaseClear", QVariant(r->GetClean()));
            w->Set("User.CheckBoxChecked.DatabaseMix", QVariant(r->GetMix()));
            w->Set("User.SpinBoxValue.DatabaseMaxSuccess", QVariant(r->GetSuccessAttempts()));
            w->Set("User.SpinBoxValue.DatabaseMaxFail", QVariant(r->GetFailAttempts()));
            w->Set("User.SpinBoxValue.DatabaseMaxSimultaneous", QVariant(r->GetNumberSimultaneousUse()));
            w->Set("User.SpinBoxValue.DatabaseInterval", QVariant(r->GetIntervalBetweenUsage()));
            w->Set("User.CheckBoxChecked.DatabaseReload", QVariant(r->GetLoadInterval() > 0));
            w->Set("User.SpinBoxValue.DatabaseReloadInterval", QVariant(r->GetLoadInterval()));
            w->Set("User.CheckBoxChecked.DatabaseRenew", QVariant(r->GetRenewInterval() > 0));
            w->Set("User.SpinBoxValue.DatabaseRenewInterval", QVariant(r->GetRenewInterval()));
            w->Set("User.CheckBoxChecked.DatabaseGreedy", QVariant(r->GetGreedy()));

        }else if(Type == "FilesFromDirectory")
        {
            ResourceModelDirectory * r = qobject_cast<ResourceModelDirectory *>(res);
            w->Set("User.LineEditText.DirectoryDirectory", QVariant(r->GetDirectory()));
            w->Set("User.LineEditText.DirectoryWildcard", QVariant(r->GetWildcard()));
            w->Set("User.CheckBoxChecked.DirectoryClean", QVariant(r->GetClean()));
            w->Set("User.CheckBoxChecked.DirectoryMix", QVariant(r->GetMix()));
            w->Set("User.SpinBoxValue.DirectoryMaxSuccess", QVariant(r->GetSuccessAttempts()));
            w->Set("User.SpinBoxValue.DirectoryMaxFail", QVariant(r->GetFailAttempts()));
            w->Set("User.SpinBoxValue.DirectoryMaxSimultaneous", QVariant(r->GetNumberSimultaneousUse()));
            w->Set("User.SpinBoxValue.DirectoryInterval", QVariant(r->GetIntervalBetweenUsage()));
            w->Set("User.CheckBoxChecked.DirectoryReload", QVariant(r->GetLoadInterval() > 0));
            w->Set("User.SpinBoxValue.DirectoryReloadInterval", QVariant(r->GetLoadInterval()));
            w->Set("User.CheckBoxChecked.DirectoryRenew", QVariant(r->GetRenewInterval() > 0));
            w->Set("User.SpinBoxValue.DirectoryRenewInterval", QVariant(r->GetRenewInterval()));
            w->Set("User.CheckBoxChecked.DirectoryGreedy", QVariant(r->GetGreedy()));
        }else if(Type == "LinesFromUrl")
        {
            ResourceModelUrl * r = qobject_cast<ResourceModelUrl *>(res);

            w->Set("User.LineEditText.UrlUrl", QVariant(r->GetUrl()));
            w->Set("User.CheckBoxChecked.UrlMix", QVariant(r->GetMix()));
            w->Set("User.SpinBoxValue.UrlMaxSuccess", QVariant(r->GetSuccessAttempts()));
            w->Set("User.SpinBoxValue.UrlMaxFail", QVariant(r->GetFailAttempts()));
            w->Set("User.SpinBoxValue.UrlMaxSimultaneous", QVariant(r->GetNumberSimultaneousUse()));
            w->Set("User.SpinBoxValue.UrlInterval", QVariant(r->GetIntervalBetweenUsage()));
            w->Set("User.CheckBoxChecked.UrlReload", QVariant(r->GetLoadInterval() > 0));
            w->Set("User.CheckBoxChecked.UrlGreedy", QVariant(r->GetGreedy()));
            w->Set("User.CheckBoxChecked.UrlRenew", QVariant(r->GetRenewInterval() > 0));
            w->Set("User.SpinBoxValue.UrlRenewInterval", QVariant(r->GetRenewInterval()));

            int interval = r->GetLoadInterval();
            if(interval < 0)
                interval = - interval;

            w->Set("User.SpinBoxValue.UrlReloadInterval", QVariant(interval));

            if(w->Get("Advanced.ContainsLineEdit.CaptureArgument").toBool())
            {
                w->Set("Advanced.ComboboxIndex.CaptureMethod", QVariant((int)r->GetCaptureMethod()));
                w->Set("Advanced.LineEditText.CaptureArgument", QVariant(r->GetCaptureArgument()));
            }else
            {
                w->Set("User.PropertyCaptureMethod.UrlUrl", QVariant((int)r->GetCaptureMethod()));
                w->Set("User.PropertyCaptureArgument.UrlUrl", QVariant(r->GetCaptureArgument()));
            }


        }else if(Type == "Select")
        {
            ResourceModelSelect * r = qobject_cast<ResourceModelSelect *>(res);
            QStringList list = r->GetValues();
            QString SelectType;
            if(r->GetSelectType() == ResourceModelSelect::Radio)
                SelectType = "Radio";
            else if(r->GetSelectType() == ResourceModelSelect::Check)
                SelectType = "Check";
            else if(r->GetSelectType() == ResourceModelSelect::Combo)
                SelectType = "Combo";
            else if(r->GetSelectType() == ResourceModelSelect::DragAndDrop)
                SelectType = "DragAndDrop";

            if(w->Get("Advanced.ContainsCombobox.SelectTypeCombo").toBool())
            {
                w->Set("Advanced.ComboboxIndex.SelectTypeCombo",QVariant(r->GetSelectType()));
                w->Set("Advanced.PlainTextEditText.SelectValuesEdit",QVariant(r->GetValues().join("\r\n")));
                w->Set("User.MultiselectType", QVariant(SelectType));
                w->Set("User.MultiselectValues", QVariant(list));
                QStringList SelectedList;
                for(int Selected:r->GetSelected())
                {
                    SelectedList.append(QString::number(Selected));
                }
                w->Set("User.MultiselectIndexes", QVariant(SelectedList));
            }else
            {
                QWidget *child = w->GetTemplateWidgetByType(true,Type)->findChild<QWidget *>("SelectWidget");
                QLayout * layout = child->layout();
                MultiSelect * OldSelect = child->findChild<MultiSelect *>();
                if(OldSelect)
                    OldSelect->deleteLater();

                MultiSelect * Select = new MultiSelect(child);

                Select->Update(SelectType, list, r->GetSelected());
                layout->addWidget(Select);
                QPair<MultiSelect*, IResourceWidget *> pair;
                pair.first = Select;
                pair.second = w;
                Triggers.insert(res->GetName(),pair);
            }

        }
    }

    void QtResourceController::FromModelToView(IResources * resources, bool Clear)
    {
        if(Clear)
            DeleteAllView();
        int size = resources->GetModelList()->size();
        if(size > 0)
        {
            emit WidgetsNotEmpty();
        }else
        {
            emit WidgetsEmpty();
        }
        CheckIfAllSectionsAreEmpty(resources);
        GetTabWiget();

        QAccordion *accordion = 0;

        QHash<QString, QPair<MultiSelect*, IResourceWidget *> > Triggers;
        QHash<QString, QList<IResourceWidget *> > Listeners;

        for(int i = 0;i<size;i++)
        {
            IResourceModel * res = resources->GetModelList()->at(i);


            IResourceWidget *w = WidgetFactory->CreateObject();
            w->SetAvailableNames(res->GetAllowedTypes());


            w->SetLanguage(Language);
            w->SetLanguageModel(LanguageModel);

            w->GetMainWidget()->setParent(Widget);


            w->SetSectionName(res->GetSectionName());
            w->SetVisibilityConditionValue(res->GetVisibilityConditionValue());

            w->SetVisibilityConditionVariable(res->GetVisibilityConditionVariable());
            w->SetVariableName(res->GetName());
            w->SetDescription(res->GetDescription());
            //qDebug()<<1<<res->GetTypeId();
            w->SetTypeId(res->GetTypeId());
            //qDebug()<<2;
            w->SetIsAdvanced(res->GetIsAdvanced());

            if(!res->GetVisibilityConditionVariable().isEmpty())
                Listeners[res->GetVisibilityConditionVariable()].append(w);

            /*if(!w->GetTemplateWidgetByType(true,res->GetTypeId()))
                continue;*/

            QString Type = res->GetTypeId();
            ShowModel(Type,res,w,Triggers);

            QList<IResourceModel*> Defaults = res->GetDefaults();
            foreach(IResourceModel* Default,Defaults)
            {
                if(w->GetAvailableNames().contains(Default->GetTypeId()))
                    ShowModel(Default->GetTypeId(),Default,w,Triggers);
            }

            w->SetVisibleToUser(res->GetVisibleByUser());
            w->SetEnabledToUser(res->GetEnabledByUser());
            QString SectionName = res->GetSectionName().GetTranslation(Language);
            QLayout *FLayout = GetLayoutForSection(SectionName);
            if(UseAccordion)
            {
                if(!accordion)
                {
                    accordion = new QAccordion(Widget);
                    FLayout->addWidget(accordion);
                }
                int Pane = accordion->addContentPane(QString::number(qrand() % 100000) + ":" + res->GetName());
                if (Pane != -1)
                {
                    QFrame *contentFrame = accordion->getContentPane(Pane)->getContentFrame();
                    accordion->getContentPane(Pane)->setMaximumHeight(3000);

                    contentFrame->setLayout(new QVBoxLayout());
                    contentFrame->layout()->addWidget(w->GetMainWidget());
                    w->GetMainWidget()->setParent(contentFrame);

                    connect(w,SIGNAL(VariableNameChanged(QString)),accordion->getContentPane(Pane),SLOT(setHeader(QString)));
                    connect(w,SIGNAL(ResourceDestroyed()),accordion->getContentPane(Pane),SLOT(selfRemove()));
                    connect(w,SIGNAL(ResourceDestroyed()),accordion->getContentPane(Pane),SLOT(deleteLater()));
                    if(UseAccordion || UseUIConstructor)
                        connect(w,SIGNAL(ResourceDestroyed()),this,SLOT(ResourceDestroyed()));

                    connect(w,SIGNAL(Up(int)),accordion->getContentPane(Pane),SLOT(selfUp()));
                    connect(w,SIGNAL(Down(int)),accordion->getContentPane(Pane),SLOT(selfDown()));
                }

            }else
            {
                FLayout->addWidget(w->GetMainWidget());
                //connect(w,SIGNAL(Up(int)),FLayout,SLOT(moveItemUp(int)));
                //connect(w,SIGNAL(Down(int)),FLayout,SLOT(moveItemDown(int)));
            }

            if(UseUIConstructor)
            {
                WidgetUIConstructor->AddUnit(res->GetName(),w->GetSectionName(),w->GetMainWidget());
            }



            if(TabWidget)
            {
                Validator->InsertResourceWidget(TabWidget->pageTitleList().indexOf(SectionName),w);
                QObject::connect(w,SIGNAL(VisibilityChanged(bool)),TabWidget, SLOT(HideEmptyTabsAndShowVisibleTabs()));
            }
            AllValidator->InsertResourceWidget(w);
            emit NewResourceWidget(w);
        }

        QHashIterator<QString, QPair<MultiSelect*, IResourceWidget *> > i(Triggers);

        //Connect triggers
        while (i.hasNext())
        {
            i.next();
            if(Listeners.contains(i.key()))
            {
                int size = Listeners[i.key()].size();
                for(int j = 0;j<size;j++)
                {
                    IResourceWidget * w = Listeners[i.key()][j];
                    QObject::connect(i.value().first,SIGNAL(ValueChanged(QString)),w, SLOT(VisibilityTriggerChanged(QString)));
                    QObject::connect(i.value().second,SIGNAL(VisibilityChanged(bool)),w, SLOT(VisibilityParentChanged(bool)));
                }

            }
        }

        //Fire triggers
        i.toFront();
        while (i.hasNext())
        {
            i.next();
            if(Listeners.contains(i.key()))
            {
                int size = Listeners[i.key()].size();
                for(int j = 0;j<size;j++)
                {
                    IResourceWidget * w = Listeners[i.key()][j];
                    w->VisibilityTriggerChanged(i.value().first->GetSelectedValue());
                }

            }
        }

        //Connect tab visibility update listeners
        i.toFront();
        while (i.hasNext())
        {
            i.next();
            if(Listeners.contains(i.key()))
            {
                if(TabWidget)
                {
                    QObject::connect(i.value().first,SIGNAL(ValueChanged(QString)),TabWidget, SLOT(HideEmptyTabsAndShowVisibleTabs()));
                }
            }
        }

        if(TabWidget)
        {
            TabWidget->HideEmptyTabsAndShowVisibleTabs();
            Validator->SetTabWidget(TabWidget);
        }


        emit WidgetGenerationDone();

    }

    IValidator* QtResourceController::GetValidator()
    {
        return AllValidator;
    }

}



