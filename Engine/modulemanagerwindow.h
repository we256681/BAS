#ifndef MODULEMANAGERWINDOW_H
#define MODULEMANAGERWINDOW_H

#include <QDialog>
#include "imodulemanager.h"
#include "imodulecreator.h"
#include "iembeddedlanguagemanager.h"
#include "engine_global.h"


namespace Ui {
class ModuleManagerWindow;
}

using namespace BrowserAutomationStudioFramework;

class ENGINESHARED_EXPORT ModuleManagerWindow : public QDialog
{
    Q_OBJECT
    IModuleManager * ModuleManager;
    IModuleCreator * ModuleCreator;
    IEmbeddedLanguageManager * EmbeddedLanguageManager;
    bool ProjectChanged;
    QString CurrentMenuModuleName;
    QString CurrentMenuModuleFolder;
    bool IsModuleListChanged;
public:
    explicit ModuleManagerWindow(QWidget *parent = 0);
    void SetModuleManager(IModuleManager *ModuleManager);
    void SetModuleCreator(IModuleCreator *ModuleCreator);
    void SetEmbeddedLanguageManager(IEmbeddedLanguageManager *EmbeddedLanguageManager);
    void Reload();


    ~ModuleManagerWindow();
    bool IsProjectChnaged();
    bool GetIsModuleListChanged();

private slots:
    void Filter();
    void on_Close_clicked();

    void LabelClicked();
    void CheckBoxClicked();


    void on_FilterText_textChanged(const QString &arg1);

    void on_ClearFilter_clicked();

    void on_CreateNewModule_clicked();

    void on_InstallModule_clicked();

    void MenuEnableDisable();

    void MenuUninstall();

    void MenuInfo();

    void MenuOpenContainingFolder();


private:
    Ui::ModuleManagerWindow *ui;
};

#endif // MODULEMANAGERWINDOW_H
