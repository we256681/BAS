#include "designresourcewrapper.h"
#include <QVBoxLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QPlainTextEdit>
#include "qxtcheckcombobox.h"
#include "multiselect.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    DesignResourceWrapper::DesignResourceWrapper(QObject *parent) :
        IResourceWidget(parent)
    {
        ParentWidget = new QWidget();
        this->setParent(ParentWidget);

        //Defaults
        Data["User.LineEditText.FileFilename"] = QVariant(QString());
        Data["User.CheckBoxChecked.FileWrite"] = QVariant(false);
        Data["User.CheckBoxChecked.FileRead"] = QVariant(true);
        Data["User.CheckBoxChecked.FileMix"] = QVariant(false);
        Data["User.SpinBoxValue.FileMaxSuccess"] = QVariant(1);
        Data["User.SpinBoxValue.FileMaxFail"] = QVariant(1);
        Data["User.SpinBoxValue.FileMaxSimultaneous"] = QVariant(1);
        Data["User.SpinBoxValue.FileInterval"] = QVariant(5000);
        Data["User.CheckBoxChecked.FileReload"] = QVariant(false);
        Data["User.SpinBoxValue.FileReloadInterval"] = QVariant(0);
        Data["User.CheckBoxChecked.FileRenew"] = QVariant(false);
        Data["User.SpinBoxValue.FileRenewInterval"] = QVariant(0);
        Data["User.CheckBoxChecked.FileGreedy"] = QVariant(false);


        Data["User.LineEditText.DirectoryDirectory"] = QVariant(QString());
        Data["User.LineEditText.DirectoryWildcard"] = QVariant(QString("*.*"));
        Data["User.CheckBoxChecked.DirectoryClean"] = QVariant(false);
        Data["User.CheckBoxChecked.DirectoryMix"] = QVariant(false);
        Data["User.SpinBoxValue.DirectoryMaxSuccess"] = QVariant(1);
        Data["User.SpinBoxValue.DirectoryMaxFail"] = QVariant(1);
        Data["User.SpinBoxValue.DirectoryMaxSimultaneous"] = QVariant(1);
        Data["User.SpinBoxValue.DirectoryInterval"] = QVariant(5000);
        Data["User.CheckBoxChecked.DirectoryReload"] = QVariant(false);
        Data["User.SpinBoxValue.DirectoryReloadInterval"] = QVariant(0);
        Data["User.CheckBoxChecked.DirectoryRenew"] = QVariant(false);
        Data["User.SpinBoxValue.DirectoryRenewInterval"] = QVariant(0);
        Data["User.CheckBoxChecked.DirectoryGreedy"] = QVariant(false);


        Data["User.LineEditText.UrlUrl"] = QVariant(QString());
        Data["User.CheckBoxChecked.UrlMix"] = QVariant(false);
        Data["User.SpinBoxValue.UrlMaxSuccess"] = QVariant(1);
        Data["User.SpinBoxValue.UrlMaxFail"] = QVariant(1);
        Data["User.SpinBoxValue.UrlMaxSimultaneous"] = QVariant(1);
        Data["User.SpinBoxValue.UrlInterval"] = QVariant(5000);
        Data["User.CheckBoxChecked.UrlReload"] = QVariant(false);
        Data["User.CheckBoxChecked.UrlGreedy"] = QVariant(false);
        Data["User.CheckBoxChecked.UrlRenew"] = QVariant(false);
        Data["User.SpinBoxValue.UrlRenewInterval"] = QVariant(0);
        Data["User.SpinBoxValue.UrlReloadInterval"] = QVariant(0);
        Data["Advanced.ComboboxIndex.CaptureMethod"] = QVariant(0);
        Data["Advanced.LineEditText.CaptureArgument"] = QVariant(QString("[\\r\\n]"));

        Data["OriginalDatabaseTableId"] = QVariant(0);
        Data["Advanced.ComboboxCurrentData.TableCombobox"] = QVariant(QString("0"));
        Data["OriginalDatabaseColumnsIds"] = QVariant(QStringList());
        Data["User.ComboBoxMulti.DatabaseGroups"] = QVariant(QStringList());
        Data["User.CheckBoxChecked.DatabaseClear"] = QVariant(false);
        Data["User.CheckBoxChecked.DatabaseMix"] = QVariant(false);
        Data["User.SpinBoxValue.DatabaseMaxSuccess"] = QVariant(1);
        Data["User.SpinBoxValue.DatabaseMaxFail"] = QVariant(1);
        Data["User.SpinBoxValue.DatabaseMaxSimultaneous"] = QVariant(1);
        Data["User.SpinBoxValue.DatabaseInterval"] = QVariant(5000);
        Data["User.CheckBoxChecked.DatabaseReload"] = QVariant(false);
        Data["User.SpinBoxValue.DatabaseReloadInterval"] = QVariant(0);
        Data["User.CheckBoxChecked.DatabaseRenew"] = QVariant(false);
        Data["User.SpinBoxValue.DatabaseRenewInterval"] = QVariant(0);
        Data["User.CheckBoxChecked.DatabaseGreedy"] = QVariant(false);

        Data["User.MultiselectType"] = QVariant(QString("Combo"));
        QStringList MultiselectValues;
        MultiselectValues.append("Option 1");
        MultiselectValues.append("Option 2");
        MultiselectValues.append("Option 3");
        Data["User.MultiselectValues"] = QVariant(MultiselectValues);
        Data["User.MultiselectIndexes"] = QVariant(QString("0"));
        Data["Advanced.ComboboxIndex.SelectTypeCombo"] = QVariant(0);
        Data["Advanced.PlainTextEditText.SelectValuesEdit"] = QVariant(QString("Option 1\nOption 2\nOption 3"));
    }

    void DesignResourceWrapper::Render()
    {
        if(Widget)
            return;
        QVBoxLayout *layout = new QVBoxLayout(ParentWidget);
        layout->setMargin(0);
        ParentWidget->setLayout(layout);
        Widget = new DesignResourceWidget(ParentWidget, this);
        connect(Widget,SIGNAL(Up(int)),this,SIGNAL(Up(int)));
        connect(Widget,SIGNAL(Down(int)),this,SIGNAL(Down(int)));
        connect(Widget,SIGNAL(VariableNameChanged(QString)),this,SIGNAL(VariableNameChanged(QString)));
        connect(Widget,SIGNAL(WillBeDeleted()),this,SIGNAL(ResourceDestroyed()));
        layout->addWidget(Widget);

        SetAvailableNames(AvailableNames);
        SetLanguageModel(Model);
        SetStringBuilder(StringBuilder);
        SetVariableName(VariableName);
        Widget->SetDatabaseState(DatabaseState);
        SetDescription(Description);
        SetTypeId(TypeId);
        SetVisibleToUser(VisibleToUser);
        SetEnabledToUser(EnabledToUser);
        SetSectionName(SectionName);
        SetVisibilityConditionValue(VisibilityConditionValue);
        SetVisibilityConditionVariable(VisibilityConditionVariable);
        SetIsAdvanced(IsAdvanced);




        for(QString& Key: Data.keys())
        {
            QStringList SplitKey = Key.split(".");
            QVariant Value = Data[Key];
            if(SplitKey.length() == 3)
            {
                QString KeyType = SplitKey[0];
                QString ActionType = SplitKey[1];
                QString ElementId = SplitKey[2];

                if(ActionType.startsWith("Contains"))
                {
                    //Do nothing
                }else if(ActionType.startsWith("Property"))
                {
                    //Do nothing, only works with user resource widget
                }else if(ActionType == "LineEditText")
                {
                    Widget->findChild<QLineEdit *>(ElementId)->setText(Value.toString());
                }else if(ActionType == "CheckBoxChecked")
                {
                    Widget->findChild<QCheckBox *>(ElementId)->setChecked(Value.toBool());
                }else if(ActionType == "SpinBoxValue")
                {
                    Widget->findChild<QSpinBox *>(ElementId)->setValue(Value.toInt());
                }else if(ActionType == "SpinBoxMinValue")
                {
                    Widget->findChild<QSpinBox *>(ElementId)->setMinimum(Value.toInt());
                }else if(ActionType == "SpinBoxMaxValue")
                {
                    Widget->findChild<QSpinBox *>(ElementId)->setMaximum(Value.toInt());
                }else if(ActionType == "ComboBoxMulti")
                {
                    Widget->findChild<QxtCheckComboBox *>(ElementId)->setCheckedData(Value.toStringList());
                }else if(ActionType == "ComboboxIndex")
                {
                    Widget->findChild<QComboBox *>(ElementId)->setCurrentIndex(Value.toInt());
                }else if(ActionType == "ComboboxCurrentData")
                {
                    QComboBox* TableCombobox = Widget->findChild<QComboBox *>(ElementId);
                    int index = -1;

                    for(int i = 0;i < TableCombobox->count();i++)
                    {
                        if(TableCombobox->itemData(i).toString() == Value.toString())
                        {
                            index = i;
                            break;
                        }
                    }
                    if(index>=0)
                        TableCombobox->setCurrentIndex(index);
                }else if(ActionType == "PlainTextEditText")
                {
                    Widget->findChild<QPlainTextEdit *>(ElementId)->setPlainText(Value.toString());
                }

            }
        }
        MultiSelect * _Multiselect = Widget->findChild<MultiSelect *>();
        QString MultiSelectType = Data["User.MultiselectType"].toString();
        QStringList MultiSelectValues = Data["User.MultiselectValues"].toStringList();

        QList<int> MultiSelectIndexes;
        QStringList Indexes = Data["User.MultiselectIndexes"].toStringList();
        for(QString& SelectedString: Indexes)
        {
            MultiSelectIndexes.append(SelectedString.toInt());
        }
        _Multiselect->Update(MultiSelectType,MultiSelectValues,MultiSelectIndexes);

        {
            QStringList Data = this->Data["DatabaseColumnsIds"].toStringList();
            QList<int> DataRaw;

            for(QString& Item: Data)
            {
                DataRaw.append(Item.toInt());
            }
            Widget->SetDatabaseColumnsIds(DataRaw);
        }


        Widget->RenderAdvanced();
    }

    void DesignResourceWrapper::Set(const QString& Name, const QVariant& Value)
    {
        Data.insert(Name, Value);
    }

    QVariant DesignResourceWrapper::Get(const QString& Name)
    {
        if(Widget)
        {
            if(Name == "User.MultiselectType")
            {
                MultiSelect * _Multiselect = Widget->findChild<MultiSelect *>();
                return QVariant(_Multiselect->GetType());
            }
            if(Name == "User.MultiselectValues")
            {
                MultiSelect * _Multiselect = Widget->findChild<MultiSelect *>();
                return QVariant(_Multiselect->GetValues());
            }
            if(Name == "User.MultiselectIndexes")
            {
                MultiSelect * _Multiselect = Widget->findChild<MultiSelect *>();
                QStringList SelectedList;
                for(int Selected:_Multiselect->GetSelectedIndex())
                {
                    SelectedList.append(QString::number(Selected));
                }
                return QVariant(SelectedList);
            }
            if(Name == "DatabaseColumnsIds")
            {
                QList<int> DataRaw = Widget->GetDatabaseColumnsIds();
                QStringList Data;
                for(int Item: DataRaw)
                {
                    Data.append(QString::number(Item));
                }
                return QVariant(Data);
            }

            QStringList SplitKey = Name.split(".");
            if(SplitKey.length() == 3)
            {
                QString KeyType = SplitKey[0];
                QString ActionType = SplitKey[1];
                QString ElementId = SplitKey[2];

                if(ActionType.startsWith("Contains"))
                {
                    return QVariant(true);
                }else if(ActionType.startsWith("Property"))
                {
                    return QVariant();
                }else if(ActionType == "LineEditText")
                {
                    return QVariant(Widget->findChild<QLineEdit *>(ElementId)->text());
                }else if(ActionType == "CheckBoxChecked")
                {
                    return QVariant(Widget->findChild<QCheckBox *>(ElementId)->isChecked());
                }else if(ActionType == "SpinBoxValue")
                {
                    return QVariant(Widget->findChild<QSpinBox *>(ElementId)->value());
                }else if(ActionType == "SpinBoxMinValue")
                {
                    return QVariant(Widget->findChild<QSpinBox *>(ElementId)->minimum());
                }else if(ActionType == "SpinBoxMaxValue")
                {
                    return QVariant(Widget->findChild<QSpinBox *>(ElementId)->maximum());
                }else if(ActionType == "ComboBoxMulti")
                {
                    return QVariant(Widget->findChild<QxtCheckComboBox *>(ElementId)->checkedData());
                }else if(ActionType == "ComboboxIndex")
                {
                    return QVariant(Widget->findChild<QComboBox *>(ElementId)->currentIndex());
                }else if(ActionType == "ComboboxCurrentData")
                {
                    return QVariant(Widget->findChild<QComboBox *>(ElementId)->currentData().toString());
                }else if(ActionType == "PlainTextEditText")
                {
                    return QVariant(Widget->findChild<QPlainTextEdit *>(ElementId)->toPlainText());
                }

            }


        }

        if(Name.contains(".Contains"))
        {
            return QVariant(true);
        }
        return Data[Name];
    }

    void DesignResourceWrapper::SetTableId(int TableId)
    {

    }

    int DesignResourceWrapper::GetTableId()
    {
        return -1;
    }

    void DesignResourceWrapper::SetLanguageModel(ILanguageModel *Model)
    {
        if(Widget)
        {
            Widget->SetLanguageModel(Model);
        }
        this->Model = Model;
    }

    ILanguageModel * DesignResourceWrapper::GetLanguageModel()
    {
        if(Widget)
            return Widget->GetLanguageModel();
        return Model;
    }

    void DesignResourceWrapper::SetLanguage(const QString& Language)
    {

    }

    IStringBuilder * DesignResourceWrapper::GetStringBuilder()
    {
        if(Widget)
            return Widget->GetStringBuilder();
        return StringBuilder;
    }

    void DesignResourceWrapper::SetStringBuilder(IStringBuilder * val)
    {
        if(Widget)
        {
            Widget->SetStringBuilder(val);
        }
        this->StringBuilder = val;
    }

    void DesignResourceWrapper::SetDatabaseState(IDatabaseState *DatabaseState)
    {
        if(Widget)
        {
            Widget->SetDatabaseState(DatabaseState);
        }
        this->DatabaseState = DatabaseState;
        foreach(DatabaseTable Table, DatabaseState->GetDatabaseTables())
        {
            Data["OriginalDatabaseTableId"] = QVariant(Table.Id);
            Data["Advanced.ComboboxCurrentData.TableCombobox"] = QVariant(QString(QString::number(Table.Id)));
            break;

        }
    }

    DesignResourceWrapper::~DesignResourceWrapper()
    {

    }


    void DesignResourceWrapper::SetVariableName(const QString& text)
    {
        if(Widget)
        {
            Widget->SetVariableName(text);
        }
        VariableName = text;
    }

    QString DesignResourceWrapper::GetVariableName()
    {
        if(Widget)
            return Widget->GetVariableName();
        return VariableName;
    }

    MultiLanguageString DesignResourceWrapper::GetDescription()
    {
        if(Widget)
        {
            return Widget->GetDescription();
        }
        return Description;
    }

    void DesignResourceWrapper::SetDescription(const MultiLanguageString& text)
    {
        if(Widget)
        {
            Widget->SetDescription(text);
        }
        this->Description = text;
    }

    void DesignResourceWrapper::SetTypeId(const QString& type)
    {
        if(Widget)
        {
            Widget->SetTypeId(type);
        }
        this->TypeId = type;
    }

    QWidget* DesignResourceWrapper::GetTemplateCurrentWidget(bool IsUser)
    {
        return Widget->GetTemplateCurrentWidget(IsUser);
    }

    QWidget* DesignResourceWrapper::GetTemplateWidgetByType(bool IsUser,const QString& type)
    {
        return Widget->GetTemplateWidgetByType(IsUser,type);
    }

    QString DesignResourceWrapper::GetTypeId()
    {
        if(Widget)
            return Widget->GetTypeId();
        return TypeId;
    }

    QWidget* DesignResourceWrapper::GetMainWidget()
    {
        return ParentWidget;
    }

    void DesignResourceWrapper::SetVisibleToUser(bool visible)
    {
        if(Widget)
            Widget->SetVisibleToUser(visible);
        VisibleToUser = visible;
    }

    void DesignResourceWrapper::SetEnabledToUser(bool enabled)
    {
        if(Widget)
            Widget->SetEnabledToUser(enabled);
        EnabledToUser = enabled;
    }

    void DesignResourceWrapper::SetAvailableNames(const QString& text)
    {
        if(Widget)
            Widget->SetAvailableNames(text);
        AvailableNames = text;
    }

    bool DesignResourceWrapper::GetVisibleToUser()
    {
        if(Widget)
            return Widget->GetVisibleToUser();
        return VisibleToUser;
    }

    bool DesignResourceWrapper::GetEnabledToUser()
    {
        if(Widget)
            return Widget->GetEnabledToUser();
        return EnabledToUser;
    }

    QString DesignResourceWrapper::GetAvailableNames()
    {
        if(Widget)
            return Widget->GetAvailableNames();
        return AvailableNames;
    }

    MultiLanguageString DesignResourceWrapper::GetSectionName()
    {
        if(Widget)
            return Widget->GetSectionName();
        return SectionName;
    }

    void DesignResourceWrapper::SetSectionName(const MultiLanguageString& SectionName)
    {
        if(Widget)
            Widget->SetSectionName(SectionName);
        this->SectionName = SectionName;
    }

    QString DesignResourceWrapper::GetVisibilityConditionVariable()
    {
        if(Widget)
            return Widget->GetVisibilityConditionVariable();
        return VisibilityConditionVariable;
    }

    void DesignResourceWrapper::SetVisibilityConditionVariable(const QString& VisibilityConditionVariable)
    {
        if(Widget)
            Widget->SetVisibilityConditionVariable(VisibilityConditionVariable);
        this->VisibilityConditionVariable = VisibilityConditionVariable;
    }

    QString DesignResourceWrapper::GetVisibilityConditionValue()
    {
        if(Widget)
            return Widget->GetVisibilityConditionValue();
        return VisibilityConditionValue;
    }

    void DesignResourceWrapper::SetVisibilityConditionValue(const QString& VisibilityConditionValue)
    {
        if(Widget)
            Widget->SetVisibilityConditionValue(VisibilityConditionValue);
        this->VisibilityConditionValue = VisibilityConditionValue;
    }

    void DesignResourceWrapper::VisibilityTriggerChanged(const QString& Value)
    {

    }

    void DesignResourceWrapper::VisibilityParentChanged(bool Visible)
    {


    }
    bool DesignResourceWrapper::IsValid()
    {
        return true;
    }

    bool DesignResourceWrapper::GetIsAdvanced()
    {
        if(Widget)
            return Widget->GetIsAdvanced();
        return IsAdvanced;
    }
    void DesignResourceWrapper::SetIsAdvanced(bool IsAdvanced)
    {
        if(Widget)
            Widget->SetIsAdvanced(IsAdvanced);
        this->IsAdvanced = IsAdvanced;
    }

    bool DesignResourceWrapper::GetIsAdvancedVisible()
    {
        return true;
    }

    void DesignResourceWrapper::VisibilityAdvancedChanged(bool Visible)
    {

    }

    bool DesignResourceWrapper::GetRequiresValidation()
    {
        return false;
    }

    void DesignResourceWrapper::SetVisibleAdvanced(bool Visible)
    {

    }

}
