#ifndef MODULECREATOR_H
#define MODULECREATOR_H

#include <QObject>
#include <QVariantMap>
#include <QtScript>
#include "imodulecreator.h"
#include "iembeddedlanguagemanager.h"
#include "EmbeddedDefinitions.h"
#include "engine_global.h"


namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT ModuleCreator : public IModuleCreator
    {
        Q_OBJECT
        QVariantMap ModuleInfo;
        QString Script;

    public:
        struct ParameterData
        {
            QString Name;
            QString Type;
            QString Description;
            QString Defaults;
        };

        struct FunctionData
        {
            QString Name;
            QString Description;
            bool HasReturnValue = false;
            QString ReturnVariableName;
            QString ReturnValueDescription;
            QString Code;
            QList<ParameterData> Parameters;
        };


        explicit ModuleCreator(QObject *parent = 0);

        virtual void SetScript(const QString& Script);
        virtual void SetModuleInfo(const QString& ModuleInfoText);

        virtual QString GetScript();
        virtual QString GetModuleInfo();

        virtual void SetProperty(const QString& Key, const QString& Value);
        virtual QString GetProperty(const QString& Key);
        virtual QString CreateModule(bool& ModuleInstalled, bool IsScriptRunning, QList<EmbeddedLanguage>& LanguageList, QList<EmbeddedModule>& ModuleList, QList<EmbeddedCodeItem>& CodeItems);
        virtual QString InstallModule(const QString& ModulePath, int OverwriteMode);


        virtual QStringList GetFunctionList();

    private:
        QString GetRandomString();
        QString GetEmbeddedId();
        QString PrepareCode(const QString& Code, bool UseFunctionMap, QMap<QString,QString>& FunctionMap);
        QList<FunctionData> ParseCode();
        QString JavascriptEscape(const QString& Data);
        void WriteAction(const QString& InterfaceFilename,const QString& SelectFilename,const QString& CodeFilename,const QString& CodeId, FunctionData& Data, bool UseFunctionMap, QMap<QString,QString>& FunctionMap);
    };
}

#endif // MODULECREATOR_H
