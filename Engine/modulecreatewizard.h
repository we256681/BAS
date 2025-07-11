#ifndef MODULECREATEWIZARD_H
#define MODULECREATEWIZARD_H

#include <QWizard>

#include "modulemanager.h"
#include "modulecreator.h"

namespace Ui {
class ModuleCreateWizard;
}

using namespace BrowserAutomationStudioFramework;

class ModuleCreateWizard : public QWizard
{
    Q_OBJECT
    IModuleCreator *_ModuleCreator;
    IModuleManager *_ModuleManager;
    bool IsAdvancedVisible;
    int CurrentPageId;
    QByteArray LastIcon;
    bool IsDefaultIcon;
public:
    explicit ModuleCreateWizard(QWidget *parent = nullptr);
    ~ModuleCreateWizard();

    void SetModuleManager(IModuleManager *_ModuleManager);
    void SetModuleCreator(IModuleCreator *_ModuleCreator);
    void SaveData();
    void Init();

private slots:
    void on_ShowMore_clicked();
    void PageChanged(int PageId);
    void CheckSkipAvailable();

    void on_Name_textChanged(const QString &arg1);

    void on_ShortDescriptionEn_textChanged(const QString &arg1);

    void UpdateIcon(const QByteArray& Data);


    void SetDefaultIcon();

    void on_ApplicationIcon_clicked();

private:
    Ui::ModuleCreateWizard *ui;
};

#endif // MODULECREATEWIZARD_H
