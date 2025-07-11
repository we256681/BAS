#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H
#include <QObject>
#include <QHash>
#include <QFile>
#include <QSharedPointer>
#include <QTimer>
#include "imodulemanager.h"
#include "engine_global.h"


namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT ModuleManager : public IModuleManager
    {
        Q_OBJECT
        QList<ModuleInfo> ModuleInfoCache;
        struct DllDataClass
        {
            QString DllName;
            int ThreadNumber;
            void *data;
        };
        QTimer* IddleTimer = 0;
        using DllData = std::shared_ptr<DllDataClass>;
        QList<DllData> DllDataList;
        QList<DllData> ThreadDataList;
        bool IsRunning = false;
        void RemoveAllThreadData(int ThreadNumber);
        DllData FindDllData(const QList<DllData> & List, const QString& DllName, int ThreadNumber = -1);
        ModuleFunction FindFunction(const QString& DllName, const QString& FunctionName);
        void* GetDllData(const QString& DllName);
        void* GetThreadData(const QString& DllName, unsigned int ThreadId);
        void GetModuleInfoFromFolder(const QString& Folder, QList<ModuleInfo>& List,bool InitializeLibraries);
        void GetModulePreserveFromFolder(const QString& Folder, QList<IModuleManager::ModulePreserve>& List);


        QList<ModulePreserve> ModulePreserveCache;
        QSharedPointer<QFile> LockExternal;

    public:
        explicit ModuleManager(QObject *parent = 0);
        virtual void TransferModulesFromOldVersion();
        virtual QList<EmbeddedLanguage> GetAllEmbeddedLanguages();
        virtual QList<EmbeddedModule> GetAllEmbeddedModules();
        virtual QList<EmbeddedCodeItem> GetAllEmbeddedCodeItems();
        virtual QList<EmbeddedLanguage> GetAllEmbeddedLanguages(QStringList& ActiveModules);
        virtual QList<EmbeddedModule> GetAllEmbeddedModules(QStringList& ActiveModules);
        virtual QList<EmbeddedCodeItem> GetAllEmbeddedCodeItems(QStringList& ActiveModules);

        virtual QList<ModuleInfo> GetModuleInfo(bool InitializeLibraries);
        virtual bool IsModuleEnabled(const QString& ModuleName);
        virtual void SetModuleEnabled(const QString& ModuleName, bool IsEnabled);
        virtual int GetModuleSystemVersion();
        virtual QList<QString> GetModuleEngineCode(const QStringList& Exclude);
        virtual QList<QString> GetModuleEngineCode(const QList<ModulePreserve>& Filter, const QStringList& Exclude);
        virtual void CacheBrowserCode();
        virtual void StartAllDlls();
        virtual void StopAllDlls();
        virtual void StartThread(unsigned int ThreadId);
        virtual void StopThread(unsigned int ThreadId);
        virtual bool GetIsRunning();
        virtual FunctionRunData* PrepareExecuteFunction(const QString& DllName, const QString& FunctionName, const QString& InputParam, unsigned int ThreadId);

        virtual QList<ModulePreserve> GetAllModules();
        virtual QList<ModulePreserve> GetModulesUsedInProject(const QString& Project, bool IncludeStandartModulesWithEmbeddedLanguages = false);
        virtual QStringList GetStandartModulesNotUsedInProject(const QString& Project);
        virtual bool PackModules(QList<ModulePreserve>& Modules);
        virtual void UnpackModules(QList<ModulePreserve>& Modules);
        virtual void ClearPreserveCache();

    signals:

    private slots:
        void IddleThreads();

    };
}

#endif // MODULEMANAGER_H

