#include "userresourcewrapper.h"
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include "qxtcheckcombobox.h"
#include "multiselect.h"
#include "every_cpp.h"


namespace BrowserAutomationStudioFramework
{
    UserResourceWrapper::UserResourceWrapper(QObject *parent) :
        IResourceWidget(parent)
    {
        Widget = new UserResourceWidget();
        this->setParent(Widget);
        DependendVisibilityParentVisible = true;
        IsVisibleAccordingToTrigger = true;
        IsAdvancedVisible = true;

        connect(Widget,SIGNAL(ValidationStateChanged()),this,SIGNAL(ValidationStateChanged()));
    }

    void UserResourceWrapper::SetDatabaseState(IDatabaseState *DatabaseState)
    {
        Widget->SetDatabaseState(DatabaseState);
    }

    void UserResourceWrapper::SetTableId(int TableId)
    {
        Widget->SetTableId(TableId);
    }

    int UserResourceWrapper::GetTableId()
    {
        return Widget->GetTableId();
    }


    bool UserResourceWrapper::GetIsAdvancedVisible()
    {
        return IsAdvancedVisible;
    }

    void UserResourceWrapper::SetIndex(int index)
    {

    }

    void UserResourceWrapper::SetLanguage(const QString& Language)
    {
        Widget->SetLanguage(Language);
    }

    void UserResourceWrapper::SetLanguageModel(ILanguageModel *Model)
    {

    }
    ILanguageModel * UserResourceWrapper::GetLanguageModel()
    {
        return 0;
    }


    void UserResourceWrapper::SetStringBuilder(IStringBuilder * val)
    {
        Widget->SetStringBuilder(val);
    }

    IStringBuilder * UserResourceWrapper::GetStringBuilder()
    {
        return Widget->GetStringBuilder();
    }

    UserResourceWrapper::~UserResourceWrapper()
    {

    }


    void UserResourceWrapper::SetVariableName(const QString& text)
    {
        Widget->SetVariableName(text);
    }

    QString UserResourceWrapper::GetVariableName()
    {
        return Widget->GetVariableName();
    }

    MultiLanguageString UserResourceWrapper::GetDescription()
    {
        return Widget->GetDescription();
    }

    void UserResourceWrapper::SetDescription(const MultiLanguageString& text)
    {
        Widget->SetDescription(text);
    }

    void UserResourceWrapper::SetTypeId(const QString& type)
    {
        Widget->SetTypeId(type);
    }

    QWidget* UserResourceWrapper::GetTemplateCurrentWidget(bool IsUser)
    {
        return Widget->GetTemplateCurrentWidget(IsUser);
    }

    QWidget* UserResourceWrapper::GetTemplateWidgetByType(bool IsUser,const QString& type)
    {
        return Widget->GetTemplateWidgetByType(IsUser,type);
    }

    QString UserResourceWrapper::GetTypeId()
    {
        return Widget->GetTypeId();
    }

    QWidget* UserResourceWrapper::GetMainWidget()
    {
        return Widget;
    }

    void UserResourceWrapper::Render()
    {

    }

    void UserResourceWrapper::Set(const QString& Name, const QVariant& Value)
    {
        if(Name == "User.PropertyNotEmpty.FixedStringValue")
        {
            Widget->findChild<QLineEdit *>("FixedStringValue")->setProperty("NotEmpty",Value);
        }

        if(Name == "User.PropertyCaptureMethod.UrlUrl")
        {
            Widget->findChild<QLineEdit *>("UrlUrl")->setProperty("CaptureMethod",Value);
        }

        if(Name == "User.PropertyCaptureArgument.UrlUrl")
        {
            Widget->findChild<QLineEdit *>("UrlUrl")->setProperty("CaptureArgument",Value);
        }

        if(Name == "User.PropertyUserDatabaseColumnsIds.All")
        {
            QList<int> DataRaw;
            QStringList Data = Value.toStringList();
            for(QString& Item: Data)
            {
                DataRaw.append(Item.toInt());
            }
            Widget->SetColumnIds(DataRaw);
        }

        QStringList SplitKey = Name.split(".");
        if(SplitKey.length() == 3)
        {
            QString KeyType = SplitKey[0];
            QString ActionType = SplitKey[1];
            QString ElementId = SplitKey[2];

            if(ActionType.startsWith("Contains"))
            {
                //Do nothing
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
            }else if(ActionType == "PlainTextEditText")
            {
                Widget->findChild<QPlainTextEdit *>(ElementId)->setPlainText(Value.toString());
            }

        }
    }

    QVariant UserResourceWrapper::Get(const QString& Name)
    {
        if(Name == "User.PropertyNotEmpty.FixedStringValue")
        {
            return QVariant(Widget->findChild<QLineEdit *>("FixedStringValue")->property("NotEmpty").toBool());
        }

        if(Name == "User.PropertyCaptureMethod.UrlUrl")
        {
            return QVariant(Widget->findChild<QLineEdit *>("UrlUrl")->property("CaptureMethod").toInt());
        }

        if(Name == "User.PropertyCaptureArgument.UrlUrl")
        {
            return QVariant(Widget->findChild<QLineEdit *>("UrlUrl")->property("CaptureArgument").toString());
        }

        if(Name == "User.PropertyUserDatabaseColumnsIds.All")
        {
            QList<int> DataRaw = Widget->GetColumnIds();
            QStringList Data;
            for(int Item: DataRaw)
            {
                Data.append(QString::number(Item));
            }
            return QVariant(Data);
        }

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

        QStringList SplitKey = Name.split(".");
        if(SplitKey.length() == 3)
        {
            QString KeyType = SplitKey[0];
            QString ActionType = SplitKey[1];
            QString ElementId = SplitKey[2];

            if(ActionType.startsWith("Contains"))
            {
                return QVariant(false);
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
            }else if(ActionType == "PlainTextEditText")
            {
                return QVariant(Widget->findChild<QPlainTextEdit *>(ElementId)->toPlainText());
            }

        }
        return QVariant();
    }

    void UserResourceWrapper::SetVisibleToUser(bool visible)
    {
        IsVisible = visible;
        Widget->SetVisibleToUser(visible);
    }

    void UserResourceWrapper::SetEnabledToUser(bool enabled)
    {
        Widget->SetEnabledToUser(enabled);
    }

    void UserResourceWrapper::SetAvailableNames(const QString& text)
    {
        Widget->SetAvailableNames(text);
    }

    bool UserResourceWrapper::GetVisibleToUser()
    {
        return Widget->GetVisibleToUser();
    }

    bool UserResourceWrapper::GetEnabledToUser()
    {
        return Widget->GetEnabledToUser();
    }

    QString UserResourceWrapper::GetAvailableNames()
    {
        return Widget->GetAvailableNames();
    }

    MultiLanguageString UserResourceWrapper::GetSectionName()
    {
        return Widget->GetSectionName();
    }

    void UserResourceWrapper::SetSectionName(const MultiLanguageString& SectionName)
    {
        Widget->SetSectionName(SectionName);
    }

    QString UserResourceWrapper::GetVisibilityConditionVariable()
    {
        return Widget->GetVisibilityConditionVariable();
    }

    void UserResourceWrapper::SetVisibilityConditionVariable(const QString& VisibilityConditionVariable)
    {
        Widget->SetVisibilityConditionVariable(VisibilityConditionVariable);
    }

    QString UserResourceWrapper::GetVisibilityConditionValue()
    {
        return Widget->GetVisibilityConditionValue();
    }

    void UserResourceWrapper::SetVisibilityConditionValue(const QString& VisibilityConditionValue)
    {
        Widget->SetVisibilityConditionValue(VisibilityConditionValue);
    }

    void UserResourceWrapper::VisibilityTriggerChanged(const QString& Value)
    {
        QStringList list = GetVisibilityConditionValue().split(QRegExp("[\\|\\;\\,]"));
        bool res = false;
        foreach(QString item, list)
        {
            if(Value.contains(item))
            {
                res = true;
                break;
            }
        }
        IsVisibleAccordingToTrigger = res;
        emit RequiresValidationStateChanged(IsVisible && IsVisibleAccordingToTrigger && DependendVisibilityParentVisible);

        if(res)
        {
            Widget->SetVisibleToUser(IsVisible && DependendVisibilityParentVisible && IsAdvancedVisible);
            emit VisibilityChanged(IsVisible && DependendVisibilityParentVisible && IsAdvancedVisible);
        }else
        {
            Widget->SetVisibleToUser(false);
            emit VisibilityChanged(false);
        }


    }

    void UserResourceWrapper::VisibilityParentChanged(bool Visible)
    {
        DependendVisibilityParentVisible = Visible;
        emit RequiresValidationStateChanged(IsVisible && IsVisibleAccordingToTrigger && DependendVisibilityParentVisible);
        if(Visible)
        {
            Widget->SetVisibleToUser(IsVisible && IsVisibleAccordingToTrigger && IsAdvancedVisible);
            emit VisibilityChanged(IsVisible && IsVisibleAccordingToTrigger && IsAdvancedVisible);
        }else
        {
            Widget->SetVisibleToUser(false);
            emit VisibilityChanged(false);
        }


    }

    void UserResourceWrapper::VisibilityAdvancedChanged(bool Visible)
    {
        if(GetIsAdvanced())
        {
            IsAdvancedVisible = Visible;
            if(Visible)
            {
                Widget->SetVisibleToUser(IsVisible && IsVisibleAccordingToTrigger && DependendVisibilityParentVisible);
                emit VisibilityChanged(IsVisible && IsVisibleAccordingToTrigger && DependendVisibilityParentVisible);
            }else
            {
                Widget->SetVisibleToUser(false);
                emit VisibilityChanged(false);
            }
        }
        Widget->SetVisibleAdvanced(Visible);



    }

    bool UserResourceWrapper::IsValid()
    {
        return Widget->IsValid();
    }

    bool UserResourceWrapper::GetIsAdvanced()
    {
        return Widget->GetIsAdvanced();
    }

    void UserResourceWrapper::SetIsAdvanced(bool IsAdvanced)
    {
        Widget->SetIsAdvanced(IsAdvanced);
    }

    bool UserResourceWrapper::GetRequiresValidation()
    {
        return IsVisible && IsVisibleAccordingToTrigger && DependendVisibilityParentVisible && Widget->GetEnabledToUser();
    }

    void UserResourceWrapper::SetVisibleAdvanced(bool Visible)
    {
        Widget->SetVisibleToUser(Visible);
    }

}
