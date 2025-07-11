#ifndef IMODULECREATOR_H
#define IMODULECREATOR_H

#include <QObject>
#include "iembeddedlanguagemanager.h"
#include "engine_global.h"


namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT IModuleCreator : public QObject
    {
        Q_OBJECT
    public:
        explicit IModuleCreator(QObject *parent = 0);

        virtual void SetScript(const QString& Script) = 0;
        virtual void SetModuleInfo(const QString& ModuleInfoText) = 0;

        virtual QString GetScript() = 0;
        virtual QString GetModuleInfo() = 0;

        virtual void SetProperty(const QString& Key, const QString& Value) = 0;
        virtual QString GetProperty(const QString& Key) = 0;
        virtual QString CreateModule(bool& ModuleInstalled, bool IsScriptRunning, QList<EmbeddedLanguage>& LanguageList, QList<EmbeddedModule>& ModuleList, QList<EmbeddedCodeItem>& CodeItems) = 0;

        //0 - Silent overwrite true
        //1 - Silent overwrite false
        //2 - Dialog
        virtual QString InstallModule(const QString& ModulePath, int OverwriteMode) = 0;

        virtual QStringList GetFunctionList() = 0;
    signals:

    public slots:
    };
}

#endif // IMODULECREATOR_H
