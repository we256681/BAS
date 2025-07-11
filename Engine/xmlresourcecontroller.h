#ifndef XMLRESOURCECONTROLLER_H
#define XMLRESOURCECONTROLLER_H
#include "engine_global.h"

#include "iresourcecontroller.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "imodulemanager.h"
#include "multilanguagestring.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT XmlResourceController : public IResourceController
    {
        Q_OBJECT
        QString Script;
        QString ModuleInfo;
        QString DatabaseId;
        QString Schema;
        bool ConnectionIsRemote;
        QString ConnectionServer;
        QString ConnectionPort;
        QString ConnectionLogin;
        QString ConnectionPassword;
        QString FileName;
        QString FileContent;
        bool Result;
        QString ErrorString;
        QString ScriptName;
        int ProtectionStrength = 4;
        QStringList UnusedModules;
        QString ScriptIcon;
        bool IsCutomIcon;
        QString ScriptVersion;
        bool HideBrowsers;
        bool IntegrateScheduler;
        bool SingleInstance;

        bool CopySilent;
        bool IsEnginesInAppData;
        QString CompileType;

        QString EngineVersion;
        QString ChromeCommandLine;
        QString SettingsWorker;
        QString ModulesMetaJson;
        QString EmbeddedData;

        QString InterfaceState;

        MultiLanguageString OutputTitle1;
        MultiLanguageString OutputTitle2;
        MultiLanguageString OutputTitle3;
        MultiLanguageString OutputTitle4;
        MultiLanguageString OutputTitle5;
        MultiLanguageString OutputTitle6;
        MultiLanguageString OutputTitle7;
        MultiLanguageString OutputTitle8;
        MultiLanguageString OutputTitle9;
        bool OutputVisible1;
        bool OutputVisible2;
        bool OutputVisible3;
        bool OutputVisible4;
        bool OutputVisible5;
        bool OutputVisible6;
        bool OutputVisible7;
        bool OutputVisible8;
        bool OutputVisible9;
        QString AvailableLanguages;
        QList<IModuleManager::ModulePreserve> Modules;
        void WriteModel(QXmlStreamWriter &xmlWriter,IResourceModel *m);
        IResourceModel* ReadModel(QHash<QString,QString>& attr);
    public:
        explicit XmlResourceController(QObject *parent = 0);
        void SetEmbeddedData(const QString & EmbeddedData);
        void SetScript(const QString & Script);
        void SetModuleInfo(const QString & ModuleInfo);
        void SetDatabaseId(const QString & DatabaseId);
        void SetSchema(const QString & Schema);
        void SetConnectionIsRemote(bool ConnectionIsRemote);
        void SetConnectionServer(const QString & ConnectionServer);
        void SetConnectionPort(const QString & ConnectionPort);
        void SetConnectionLogin(const QString & ConnectionLogin);
        void SetConnectionPassword(const QString & ConnectionPassword);

        void SetAvailableLanguages(const QString & AvailableLanguages);
        void SetScriptName(const QString & Name);
        void SetProtectionStrength(int ProtectionStrength);
        void SetUnusedModules(const QStringList & UnusedModules);
        void SetScriptIcon(const QString & Icon);
        void SetIsCustomIcon(bool IsCustomIcon);
        void SetHideBrowsers(bool HideBrowsers);
        void SetIntegrateScheduler(bool IntegrateScheduler);
        void SetSingleInstance(bool SingleInstance);
        bool GetSingleInstance();

        void SetScriptVersion(const QString & Version);
        void SetFileName(const QString & SetFileName);
        void SetFileContent(const QString & FileContent);

        void SetCopySilent(bool CopySilent);
        void SetIsEnginesInAppData(bool IsEnginesInAppData);
        void SetCompileType(const QString& CompileType);

        void SetEngineVersion(const QString & EngineVersion);
        void SetChromeCommandLine(const QString & ChromeCommandLine);
        void SetSettingsWorker(const QString & SettingsWorker);
        void SetModulesMetaJson(const QString & ModulesMetaJson);

        void SetInterfaceState(const QString & InterfaceState);

        void SetOutputTitle1(const MultiLanguageString& OutputTitle1);
        void SetOutputTitle2(const MultiLanguageString& OutputTitle2);
        void SetOutputTitle3(const MultiLanguageString& OutputTitle3);
        void SetOutputTitle4(const MultiLanguageString& OutputTitle4);
        void SetOutputTitle5(const MultiLanguageString& OutputTitle5);
        void SetOutputTitle6(const MultiLanguageString& OutputTitle6);
        void SetOutputTitle7(const MultiLanguageString& OutputTitle7);
        void SetOutputTitle8(const MultiLanguageString& OutputTitle8);
        void SetOutputTitle9(const MultiLanguageString& OutputTitle9);
        void SetOutputVisible1(bool OutputVisible1);
        void SetOutputVisible2(bool OutputVisible2);
        void SetOutputVisible3(bool OutputVisible3);
        void SetOutputVisible4(bool OutputVisible4);
        void SetOutputVisible5(bool OutputVisible5);
        void SetOutputVisible6(bool OutputVisible6);
        void SetOutputVisible7(bool OutputVisible7);
        void SetOutputVisible8(bool OutputVisible8);
        void SetOutputVisible9(bool OutputVisible9);

        void SetModulesPreserve(const QList<IModuleManager::ModulePreserve>& Modules);
        QList<IModuleManager::ModulePreserve> GetModulesPreserve();

        QString GetEmbeddedData();
        QString GetScript();
        QString GetModuleInfo();
        QString GetDatabaseId();
        QString GetSchema();
        bool GetConnectionIsRemote();
        QString GetConnectionServer();
        QString GetConnectionPort();
        QString GetConnectionLogin();
        QString GetConnectionPassword();


        QString GetScriptName();
        int GetProtectionStrength();
        QStringList GetUnusedModules();
        QString GetScriptIcon();
        bool GetIsCustomIcon();
        bool GetHideBrowsers();
        bool GetIntegrateScheduler();
        QString GetScriptVersion();
        QString GetFileName();
        QString GetFileContent();
        QString GetErrorString();
        QString GetAvailableLanguages();

        QString GetEngineVersion();
        QString GetChromeCommandLine();
        QString GetSettingsWorker();
        QString GetModulesMetaJson();

        QString GetInterfaceState();

        bool GetCopySilent();
        bool GetIsEnginesInAppData();
        QString GetCompileType();


        MultiLanguageString GetOutputTitle1();
        MultiLanguageString GetOutputTitle2();
        MultiLanguageString GetOutputTitle3();
        MultiLanguageString GetOutputTitle4();
        MultiLanguageString GetOutputTitle5();
        MultiLanguageString GetOutputTitle6();
        MultiLanguageString GetOutputTitle7();
        MultiLanguageString GetOutputTitle8();
        MultiLanguageString GetOutputTitle9();

        bool GetOutputVisible1();
        bool GetOutputVisible2();
        bool GetOutputVisible3();
        bool GetOutputVisible4();
        bool GetOutputVisible5();
        bool GetOutputVisible6();
        bool GetOutputVisible7();
        bool GetOutputVisible8();
        bool GetOutputVisible9();
        bool GetResult();
    signals:

    public slots:
        virtual void FromViewToModel(IResources * resources, bool Clear = true);
        virtual void FromModelToView(IResources * resources, bool Clear = true);
    };
}
#endif // XMLRESOURCECONTROLLER_H
