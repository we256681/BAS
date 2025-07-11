#ifndef CHOOSERRESOURCEWIDGET_H
#define CHOOSERRESOURCEWIDGET_H
#include "engine_global.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>

#include "istringbuilder.h"
#include "qxtcheckcombobox.h"
#include "idatabasestate.h"

namespace Ui {
class ChooserResourceWidget;
}

using namespace BrowserAutomationStudioFramework;

class ENGINESHARED_EXPORT ChooserResourceWidget : public QWidget
{
    Q_OBJECT
    IStringBuilder *StringBuilder;
    bool ShowValidation;
    bool LastValidateResult;
    QList<QWidget *> Widgets;
    QxtCheckComboBox * MultiWidget;
    bool DatabaseIndexesChangedInAction;
    IDatabaseState * DatabaseState;
    int LastTableId;
    bool LastVisibleAdvanced;

    QString CurrentTabName;
    QStringList AvailableTypes;

    /*Custom UI*/

    QWidget *page = 0;
    QHBoxLayout *horizontalLayout = 0;
    QFormLayout *formLayout_2 = 0;
    QHBoxLayout *LayoutValidation1 = 0;
    QLineEdit *FixedStringValue = 0;
    QLabel *LabelValidation1 = 0;
    QLabel *label_4 = 0;
    QWidget *page_2 = 0;
    QVBoxLayout *verticalLayout_2 = 0;
    QFormLayout *formLayout_4 = 0;
    QLabel *label_5 = 0;
    QSpinBox *FixedIntegerValue = 0;
    QWidget *page_4 = 0;
    QHBoxLayout *horizontalLayout_7 = 0;
    QFormLayout *formLayout_6 = 0;
    QLabel *label_8 = 0;
    QHBoxLayout *horizontalLayout_5 = 0;
    QLineEdit *RandomStringValue = 0;
    QPushButton *RandomStringEdit = 0;
    QWidget *page_5 = 0;
    QHBoxLayout *horizontalLayout_8 = 0;
    QFormLayout *formLayout_7 = 0;
    QLabel *label_9 = 0;
    QSpinBox *RIMinimumValue = 0;
    QLabel *label_2 = 0;
    QSpinBox *RIMaximumValue = 0;
    QWidget *page_6 = 0;
    QVBoxLayout *verticalLayout_3 = 0;
    QVBoxLayout *verticalLayout_8 = 0;
    QWidget *SelectWidget = 0;
    QHBoxLayout *horizontalLayout_3 = 0;
    QVBoxLayout *verticalLayout_9 = 0;
    QWidget *page_3 = 0;
    QVBoxLayout *verticalLayout = 0;
    QFormLayout *formLayout_8 = 0;
    QLabel *label_10 = 0;
    QHBoxLayout *horizontalLayout_6 = 0;
    QLineEdit *FileFilename = 0;
    QLabel *LabelValidation2 = 0;
    QPushButton *ButtonFileSelect = 0;
    QLabel *label = 0;
    QCheckBox *FileWrite = 0;
    QLabel *label_3 = 0;
    QCheckBox *FileMix = 0;
    QLabel *label_111 = 0;
    QLabel *label_17 = 0;
    QLabel *label_18 = 0;
    QLabel *label_19 = 0;
    QSpinBox *FileMaxSuccess = 0;
    QSpinBox *FileMaxFail = 0;
    QSpinBox *FileMaxSimultaneous = 0;
    QSpinBox *FileInterval = 0;
    QLabel *label_26 = 0;
    QCheckBox *FileReload = 0;
    QLabel *label_27 = 0;
    QSpinBox *FileReloadInterval = 0;
    QLabel *label_37 = 0;
    QLabel *label_38 = 0;
    QCheckBox *FileRenew = 0;
    QSpinBox *FileRenewInterval = 0;
    QLabel *label_7 = 0;
    QCheckBox *FileGreedy = 0;
    QLabel *label_50 = 0;
    QCheckBox *FileRead = 0;
    QWidget *page_7 = 0;
    QVBoxLayout *verticalLayout_6 = 0;
    QFormLayout *formLayout_9 = 0;
    QLabel *label_11 = 0;
    QHBoxLayout *horizontalLayout_9 = 0;
    QLineEdit *UrlUrl = 0;
    QLabel *LabelValidation3 = 0;
    QLabel *label_13 = 0;
    QCheckBox *UrlMix = 0;
    QLabel *label_112 = 0;
    QSpinBox *UrlMaxSuccess = 0;
    QLabel *label_20 = 0;
    QSpinBox *UrlMaxFail = 0;
    QLabel *label_21 = 0;
    QSpinBox *UrlMaxSimultaneous = 0;
    QLabel *label_22 = 0;
    QSpinBox *UrlInterval = 0;
    QLabel *label_29 = 0;
    QCheckBox *UrlReload = 0;
    QLabel *label_28 = 0;
    QSpinBox *UrlReloadInterval = 0;
    QLabel *label_35 = 0;
    QLabel *label_36 = 0;
    QCheckBox *UrlRenew = 0;
    QSpinBox *UrlRenewInterval = 0;
    QLabel *label_31 = 0;
    QCheckBox *UrlGreedy = 0;
    QWidget *page_8 = 0;
    QVBoxLayout *verticalLayout_7 = 0;
    QFormLayout *formLayout_10 = 0;
    QLabel *label_12 = 0;
    QHBoxLayout *horizontalLayout_10 = 0;
    QLineEdit *DirectoryDirectory = 0;
    QLabel *LabelValidation4 = 0;
    QPushButton *ButtonDirectorySelect = 0;
    QLabel *label_14 = 0;
    QCheckBox *DirectoryClean = 0;
    QLabel *label_15 = 0;
    QCheckBox *DirectoryMix = 0;
    QLabel *label_113 = 0;
    QLabel *label_23 = 0;
    QLabel *label_24 = 0;
    QLabel *label_25 = 0;
    QSpinBox *DirectoryMaxSuccess = 0;
    QSpinBox *DirectoryMaxFail = 0;
    QSpinBox *DirectoryMaxSimultaneous = 0;
    QSpinBox *DirectoryInterval = 0;
    QLabel *label_16 = 0;
    QLineEdit *DirectoryWildcard = 0;
    QLabel *label_6 = 0;
    QLabel *label_30 = 0;
    QCheckBox *DirectoryReload = 0;
    QSpinBox *DirectoryReloadInterval = 0;
    QLabel *label_33 = 0;
    QCheckBox *DirectoryRenew = 0;
    QLabel *label_34 = 0;
    QSpinBox *DirectoryRenewInterval = 0;
    QLabel *label_32 = 0;
    QCheckBox *DirectoryGreedy = 0;
    QWidget *page_9 = 0;
    QVBoxLayout *verticalLayout_4 = 0;
    QWidget *widget = 0;
    QHBoxLayout *horizontalLayout_2 = 0;
    QSpacerItem *horizontalSpacer = 0;
    QHBoxLayout *ComboLayout = 0;
    QLabel *LabelValidation5 = 0;
    QWidget *widget_2 = 0;
    QVBoxLayout *verticalLayout_10 = 0;
    QFormLayout *formLayout = 0;
    QLabel *label_39 = 0;
    QCheckBox *DatabaseClear = 0;
    QLabel *label_40 = 0;
    QCheckBox *DatabaseMix = 0;
    QLabel *label_41 = 0;
    QSpinBox *DatabaseMaxSuccess = 0;
    QLabel *label_42 = 0;
    QSpinBox *DatabaseMaxFail = 0;
    QLabel *label_43 = 0;
    QSpinBox *DatabaseMaxSimultaneous = 0;
    QLabel *label_44 = 0;
    QSpinBox *DatabaseInterval = 0;
    QLabel *label_45 = 0;
    QCheckBox *DatabaseGreedy = 0;
    QLabel *label_46 = 0;
    QCheckBox *DatabaseReload = 0;
    QLabel *label_47 = 0;
    QSpinBox *DatabaseReloadInterval = 0;
    QLabel *label_48 = 0;
    QCheckBox *DatabaseRenew = 0;
    QLabel *label_49 = 0;
    QSpinBox *DatabaseRenewInterval = 0;
    QSizePolicy sizePolicy2;

signals:
    void ValidationStateChanged();
public:
    IStringBuilder * GetStringBuilder();
    void SetStringBuilder(IStringBuilder * val);
    void SetDatabaseState(IDatabaseState * DatabaseState);

    explicit ChooserResourceWidget(QWidget *parent = 0);
    ~ChooserResourceWidget();
    QWidget* GetTemplateWidgetByIndex(int index);
    void RemoveWidgetAtIndex(int index);
    void SetShowValidation(bool ShowValidation);
    bool IsValid();
    int GetTableId();
    void Render();
    void RenderAdvanced();

public slots:
    void setCurrentIndex(int index);
    void SetVisibleAdvanced(bool Visible);
    void SetTableId(int TableId);

private:

    Ui::ChooserResourceWidget *ui;
    void changeEvent(QEvent *e);
    void SetupUiCustom();

private slots:
    void Validate();

    void ButtonFileSelect_clicked();
    void ButtonDirectorySelect_clicked();

    void RandomStringEdit_clicked();
    void SetRandomStringEdit(const QString& val);
    void DirectoryReload_toggled(bool checked);
    void FileReload_toggled(bool checked);
    void UrlReload_toggled(bool checked);
    void FileRenew_toggled(bool checked);
    void UrlRenew_toggled(bool checked);
    void DirectoryRenew_toggled(bool checked);
    void DatabaseReload_toggled(bool checked);
    void DatabaseRenew_toggled(bool checked);
    void DatabaseIndexesChanged(QList<int> Previous,QList<int> Now);
    void GroupsChanged();

};

#endif // CHOOSERRESOURCEWIDGET_H
