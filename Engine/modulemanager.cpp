#include "modulemanager.h"
#include <QDir>
#include <QTextStream>
#include <QDirIterator>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QCoreApplication>
#include <QLibrary>
#include <QSet>
#include <QBuffer>
#include "quazip.h"
#include "JlCompress.h"

#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    ModuleManager::ModuleManager(QObject *parent) :
        IModuleManager(parent)
    {
        QDir(QString("external/") + QString::number(qApp->applicationPid())).mkpath(".");
        LockExternal = QSharedPointer<QFile>::create(QString("external/") + QString::number(qApp->applicationPid()) + QString("/lock"));
        LockExternal->remove();
        LockExternal->open(QIODevice::WriteOnly);
    }

    static bool recurseCopyAddDir(QDir d, QDir t, const QStringList& e)
    {
        QStringList qsl = d.entryList(QStringList()<<"*",QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);

        foreach (QString file, qsl)
        {
            QFileInfo finfo(d.absoluteFilePath(file));

            if (finfo.isSymLink())
                return true;

            if (finfo.isDir())
            {
                if(!t.mkdir(file))
                {
                    return false;
                }

                QDir sd(finfo.filePath());
                if(!recurseCopyAddDir(sd, QDir(t.absoluteFilePath(file)),e))
                {
                    return false;
                }
            } else
            {
                bool Exclude = false;
                for(const QString& f:e)
                {
                    if(finfo.absoluteFilePath().contains(f))
                    {
                        Exclude = true;
                        break;
                    }
                }
                if(!Exclude)
                {
                    if(!QFile::copy(finfo.absoluteFilePath(),t.absoluteFilePath(file)))
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    void ModuleManager::TransferModulesFromOldVersion()
    {
        if(QFileInfo("modules_transfered.txt").exists())
        {
            return;
        }
        {
            QFile file("modules_transfered.txt");
            file.open(QIODevice::WriteOnly);
            file.close();
        }
        QDirIterator it("..");
        QDir Current = QDir::current();
        QStringList CurrentPathList = QDir::toNativeSeparators(Current.path()).split(QDir::separator(), QString::SkipEmptyParts);
        QString CurrentVersion;
        if(!CurrentPathList.isEmpty())
        {
            CurrentVersion = CurrentPathList.last();
        }
        if(CurrentVersion.isEmpty())
            return;

        int PreviousVersionX = 0;
        int PreviousVersionY = 0;
        int PreviousVersionZ = 0;

        QString PreviousVersion;

        while (it.hasNext())
        {
            it.next();
            QString Candidate = it.fileName();
            if(it.fileInfo().isDir() && Candidate != "." && Candidate != ".." && Candidate != CurrentVersion)
            {
                QStringList CandidateSplit = Candidate.split(".");

                if(CandidateSplit.size() == 3)
                {
                    int CandidateVersionX = 0;
                    int CandidateVersionY = 0;
                    int CandidateVersionZ = 0;

                    CandidateVersionX = CandidateSplit.at(0).toInt();
                    CandidateVersionY = CandidateSplit.at(1).toInt();
                    CandidateVersionZ = CandidateSplit.at(2).toInt();

                    //if Candidate > PreviousVersion
                    if(
                            CandidateVersionX > PreviousVersionX
                            || (CandidateVersionX == PreviousVersionX && CandidateVersionY > PreviousVersionY)
                            || (CandidateVersionX == PreviousVersionX && CandidateVersionY == PreviousVersionY && CandidateVersionZ > PreviousVersionZ)
                       )
                    {
                        PreviousVersionX = 0;
                        PreviousVersionY = 0;
                        PreviousVersionZ = 0;

                        PreviousVersion = Candidate;

                    }
                }
            }
        }

        if(PreviousVersion.isEmpty())
            return;

        Current.mkpath("custom");
        Current.cd("custom");
        QDir Target = QDir::current();
        Target.cd("..");
        Target.cd(PreviousVersion);
        if(Target.cd("custom"))
            recurseCopyAddDir(Target,Current,QStringList());
    }


    ModuleManager::DllData ModuleManager::FindDllData(const QList<DllData> & List, const QString& DllName, int ThreadNumber)
    {
        for(DllData dat:List)
        {
            if(dat->DllName == DllName && dat->ThreadNumber == ThreadNumber)
                return dat;
        }
        return 0;
    }

    void ModuleManager::StartAllDlls()
    {
        if(!IddleTimer)
        {
            IddleTimer = new QTimer(this);
            IddleTimer->setSingleShot(false);
            IddleTimer->setInterval(3000);
            connect(IddleTimer,SIGNAL(timeout()),this,SLOT(IddleThreads()));
            IddleTimer->start();
        }
        DllDataList.clear();
        ThreadDataList.clear();
        ModuleInfoCache = GetModuleInfo(true);
        for(ModuleInfo Info:ModuleInfoCache)
        {
            if(!Info->IsEnabled)
                continue;
            for(ModuleDll dll:Info->Dlls)
            {
                //qDebug()<<dll->Name;
                //qDebug()<<dll->FileName;
                //qDebug()<<dll->StartDllFunction;
                DllData data = std::make_shared<DllDataClass>();
                data->DllName = dll->Name;
                data->ThreadNumber = -1;
                if(dll->StartDllFunction)
                    data->data = dll->StartDllFunction();
                else
                    data->data = 0;
                //qDebug()<<data->data;
                DllDataList.append(data);
            }
        }
        IsRunning = true;
    }

    void ModuleManager::StopAllDlls()
    {
        if(IddleTimer)
        {
            IddleTimer->stop();
            IddleTimer->deleteLater();
            IddleTimer = 0;
        }
        for(ModuleInfo Info:ModuleInfoCache)
        {
            if(!Info->IsEnabled)
                continue;
            for(ModuleDll dll:Info->Dlls)
            {
                //qDebug()<<dll->Name;
                //qDebug()<<dll->FileName;
                //qDebug()<<dll->EndDllFunction;
                if(dll->EndDllFunction)
                {
                    dll->EndDllFunction(GetDllData(dll->Name));
                }
            }
        }
        DllDataList.clear();
        ModuleInfoCache.clear();
        IsRunning = false;
    }

    void ModuleManager::RemoveAllThreadData(int ThreadNumber)
    {
        QMutableListIterator<DllData> i(ThreadDataList);
        while (i.hasNext())
        {
            DllData data = i.next();
            if(data->ThreadNumber == ThreadNumber)
                i.remove();
        }
    }

    void ModuleManager::StartThread(unsigned int ThreadId)
    {
        RemoveAllThreadData(ThreadId);
        for(ModuleInfo Info:ModuleInfoCache)
        {
            if(!Info->IsEnabled)
                continue;
            for(ModuleDll dll:Info->Dlls)
            {
                //qDebug()<<dll->Name;
                //qDebug()<<dll->FileName;
                //qDebug()<<dll->EndDllFunction;
                DllData data = std::make_shared<DllDataClass>();
                data->DllName = dll->Name;
                data->ThreadNumber = ThreadId;
                if(dll->StartThreadFunction)
                    data->data = dll->StartThreadFunction();
                else
                    data->data = 0;
                ThreadDataList.append(data);
            }
        }
    }

    void ModuleManager::IddleThreads()
    {
        for(ModuleInfo Info:ModuleInfoCache)
        {
            if(!Info->IsEnabled)
                continue;
            for(ModuleDll dll:Info->Dlls)
            {
                if(dll->IddleThreadFunction)
                {
                    QListIterator<DllData> i(ThreadDataList);
                    while (i.hasNext())
                    {
                        DllData data = i.next();
                        if(data->DllName == dll->Name)
                        {
                            dll->IddleThreadFunction(data->data);
                        }
                    }
                }
            }
        }

    }

    bool ModuleManager::GetIsRunning()
    {
        return IsRunning;
    }

    void ModuleManager::StopThread(unsigned int ThreadId)
    {
        for(ModuleInfo Info:ModuleInfoCache)
        {
            if(!Info->IsEnabled)
                continue;
            for(ModuleDll dll:Info->Dlls)
            {
                //qDebug()<<dll->Name;
                //qDebug()<<dll->FileName;
                //qDebug()<<dll->EndDllFunction;
                if(dll->EndThreadFunction)
                    dll->EndThreadFunction(GetThreadData(dll->Name,ThreadId));
            }
        }
        RemoveAllThreadData(ThreadId);
    }

    void* ModuleManager::GetDllData(const QString& DllName)
    {
        DllData data = FindDllData(DllDataList,DllName);
        if(data)
            return data->data;
        return 0;
    }

    void* ModuleManager::GetThreadData(const QString& DllName, unsigned int ThreadId)
    {
        DllData data = FindDllData(ThreadDataList,DllName,ThreadId);
        if(data)
            return data->data;
        return 0;
    }

    IModuleManager::ModuleFunction ModuleManager::FindFunction(const QString& DllName, const QString& FunctionName)
    {
        for(ModuleInfo module:ModuleInfoCache)
        {
            if(!module->IsEnabled)
                continue;
            for(ModuleDll dll :module->Dlls)
            {
                if(dll->Name == DllName)
                {
                    for(ModuleFunction func :dll->FunctionList)
                    {
                        if(func->Alias == FunctionName)
                        {
                            return func;
                        }
                    }
                    return 0;
                }
            }
        }
        return 0;
    }

    FunctionRunData * ModuleManager::PrepareExecuteFunction(const QString& DllName, const QString& FunctionName, const QString& InputParam, unsigned int ThreadId)
    {
        FunctionRunData *res = new FunctionRunData();
        res->FunctionPointer = 0;
        res->DllData = 0;
        res->ThreadData = 0;
        res->ThreadId = ThreadId;
        res->IsError = true;
        res->IsAync = false;
        res->WaitInfinite = false;
        res->NeedToStop = false;
        res->FunctionName = FunctionName;
        res->DllName = DllName;
        ModuleFunction func = FindFunction(DllName, FunctionName);

        if(!func)
        {
            res->ErrorString = std::string("Failed to find function ") +  DllName.toStdString() + std::string(".") + FunctionName.toStdString();
            return res;
        }


        res->FunctionPointer = func->WorkFunction;
        res->IsAync = func->IsAsync;
        res->WaitInfinite = func->IsWaitInfinite;

        res->DllData = FindDllData(DllDataList,DllName)->data;
        res->ThreadData = FindDllData(ThreadDataList,DllName,ThreadId)->data;

        QByteArray InputData = InputParam.toUtf8();
        res->InputJson.assign(InputData.begin(),InputData.end());
        res->InputJson.push_back(0);

        res->IsError = false;

        return res;
    }


    QList<QString> ModuleManager::GetModuleEngineCode(const QStringList& Exclude)
    {
        QList<QString> res;
        for(IModuleManager::ModuleInfo Info: GetModuleInfo(false))
        {
            if(Info->IsEnabled && !Exclude.contains(Info->Name))
            {
                res.append(Info->EngineCode);
            }
        }
        return res;
    }

    void ModuleManager::CacheBrowserCode()
    {
        QString res;
        for(IModuleManager::ModuleInfo Info: GetModuleInfo(false))
        {
            if(Info->IsEnabled && Info->Name != "FormDataFake")
            {
                for(QString& Code: Info->BrowserCode)
                {
                    res.append(Code);
                    res.append("\n");
                }
            }
        }

        QFile BrowserCode("browser_code.txt");
        if(BrowserCode.open(QIODevice::WriteOnly))
        {
            BrowserCode.write(res.toUtf8().toBase64());
        }
        BrowserCode.close();

    }

    QList<QString> ModuleManager::GetModuleEngineCode(const QList<ModulePreserve>& Filter, const QStringList& Exclude)
    {
        QSet<QString> UsedModules;
        for(ModulePreserve Module:Filter)
        {
            UsedModules.insert(Module->Name);
        }
        QList<QString> res;
        for(IModuleManager::ModuleInfo Info: GetModuleInfo(false))
        {
            bool IsCustom = Info->Folder == "custom" || Info->Folder.contains("external");
            if(Info->IsEnabled && !Exclude.contains(Info->Name) && (!IsCustom || UsedModules.contains(Info->Name)))
            {
                res.append(Info->EngineCode);
            }
        }
        return res;
    }

    void ModuleManager::GetModulePreserveFromFolder(const QString& Folder, QList<IModuleManager::ModulePreserve>& List)
    {
        QDir dir(Folder);
        dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        QStringList dirList = dir.entryList();

        QJsonDocument DocumentMeta;
        {
            QJsonParseError err;

            QFile meta("modules/meta.json");
            if(meta.open(QIODevice::ReadOnly))
            {
                QByteArray json = meta.readAll();
                DocumentMeta = QJsonDocument::fromJson(json, &err);
                meta.close();
            }
        }

        foreach(QString dir, dirList)
        {
            QJsonParseError err;
            QFile manifest(Folder + QString("/") + dir + QString("/manifest.json"));
            if(manifest.open(QIODevice::ReadOnly))
            {
                QByteArray json = manifest.readAll();
                QJsonDocument doc = QJsonDocument::fromJson(json, &err);
                manifest.close();

                if(err.error == QJsonParseError::NoError)
                {
                    if(doc.object().contains("name")
                    && doc.object().contains("description")
                    && doc.object().contains("developer_name")
                    && doc.object().contains("developer_email")
                    && doc.object().contains("developer_site")
                    && doc.object().contains("api_version")
                    && doc.object().contains("engine")
                    && doc.object().contains("browser")
                    && doc.object().contains("actions")
                    && doc.object().contains("major_version")
                    && doc.object().contains("minor_version")
                            )
                    {
                        QString Name = doc.object()["name"].toString();
                        if(DocumentMeta.object().contains(Name) && !DocumentMeta.object()[Name].toBool())
                        {
                            continue;
                        }

                        IModuleManager::ModulePreserve Preserve = std::make_shared<IModuleManager::ModulePreserveClass>();

                        Preserve->Name = Name;
                        Preserve->Folder = Folder;

                        bool IsAutogenerated = doc.object().contains("is_autogenerated") && doc.object().value("is_autogenerated").toBool();
                        bool HasEngine = doc.object().contains("engine") && !doc.object().value("engine").toArray().isEmpty();
                        bool HasBrowser = doc.object().contains("browser") && !doc.object().value("browser").toArray().isEmpty();
                        Preserve->IsUnconditionallyIncluded = (HasEngine && !IsAutogenerated) || HasBrowser;
                        Preserve->IsAutogenerated = IsAutogenerated;

                        //Overwrite modules
                        QMutableListIterator<ModulePreserve> i(List);
                        while (i.hasNext())
                        {
                            if (i.next()->Name == Name)
                                i.remove();
                        }
                        List.append(Preserve);

                    }
                }
            }
        }
    }


    void ModuleManager::GetModuleInfoFromFolder(const QString& Folder, QList<IModuleManager::ModuleInfo>& List,bool InitializeLibraries)
    {
        QDir dir(Folder);
        dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        QStringList dirList = dir.entryList();

        QJsonDocument DocumentMeta;
        {
            QJsonParseError err;

            QFile meta("modules/meta.json");
            if(meta.open(QIODevice::ReadOnly))
            {
                QByteArray json = meta.readAll();
                DocumentMeta = QJsonDocument::fromJson(json, &err);
                meta.close();
            }
        }

        foreach(QString dir, dirList)
        {
            QJsonParseError err;
            QFile manifest(Folder + QString("/") + dir + QString("/manifest.json"));
            if(manifest.open(QIODevice::ReadOnly))
            {
                QByteArray json = manifest.readAll();
                QJsonDocument doc = QJsonDocument::fromJson(json, &err);
                manifest.close();

                if(err.error == QJsonParseError::NoError)
                {
                    if(doc.object().contains("name")
                    && doc.object().contains("description")
                    && doc.object().contains("developer_name")
                    && doc.object().contains("developer_email")
                    && doc.object().contains("developer_site")
                    && doc.object().contains("api_version")
                    && doc.object().contains("engine")
                    && doc.object().contains("browser")
                    && doc.object().contains("actions")
                    && doc.object().contains("major_version")
                    && doc.object().contains("minor_version")
                            )
                    {
                        if(doc.object()["api_version"].toInt() > GetModuleSystemVersion())
                        {
                            continue;
                        }
                        IModuleManager::ModuleInfo Info = std::make_shared<IModuleManager::ModuleInfoClass>();


                        if(doc.object().contains("languages"))
                        {
                            QJsonArray LanguagesArray = doc.object()["languages"].toArray();
                            for(QJsonValue LanguageValue: LanguagesArray)
                            {
                                EmbeddedLanguage Language;

                                QJsonObject LanguageObject = LanguageValue.toObject();
                                Language.Name = LanguageObject.value("Name").toString();
                                Language.Version = LanguageObject.value("Version").toString();

                                Info->EmbeddedLanguages.append(Language);
                            }


                        }


                        if(doc.object().contains("modules"))
                        {
                            QJsonArray ModulesArray = doc.object()["modules"].toArray();
                            for(QJsonValue ModuleValue: ModulesArray)
                            {
                                EmbeddedModule Module;

                                QJsonObject ModuleObject = ModuleValue.toObject();
                                Module.LanguageName = ModuleObject.value("LanguageName").toString();
                                Module.LanguageVersion = ModuleObject.value("LanguageVersion").toString();
                                Module.ModuleName = ModuleObject.value("ModuleName").toString();
                                Module.ModuleVersion = ModuleObject.value("ModuleVersion").toString();
                                Info->EmbeddedModules.append(Module);

                            }

                        }

                        if(doc.object().contains("embeddeddata"))
                        {
                            QJsonArray CodesArray = doc.object()["embeddeddata"].toArray();
                            for(QJsonValue CodeValue: CodesArray)
                            {
                                EmbeddedCodeItem Code;

                                QJsonObject CodeObject = CodeValue.toObject();

                                Code.Data = CodeObject.value("Data").toString();
                                Code.DataName = CodeObject.value("DataName").toString();
                                Code.DataType = CodeObject.value("DataType").toInt();
                                Code.LanguageName = CodeObject.value("LanguageName").toString();
                                Code.LanguageVersion = CodeObject.value("LanguageVersion").toString();
                                Info->EmbeddedCodeItems.append(Code);

                            }

                        }



                        Info->Name = doc.object()["name"].toString();
                        if(doc.object().contains("icon"))
                        {
                            Info->IconPath = doc.object()["icon"].toString();
                            if(!Info->IconPath.isEmpty())
                            {
                                Info->IconPath = Folder + QString("/") + dir + QString("/") + Info->IconPath;
                            }
                        }

                        Info->Description = doc.object()["description"].toString();
                        Info->DeveloperName = doc.object()["developer_name"].toString();
                        Info->DeveloperEmail = doc.object()["developer_email"].toString();
                        Info->DeveloperSite = doc.object()["developer_site"].toString();
                        Info->ApiVersion = doc.object()["api_version"].toInt();
                        Info->MajorVersion = doc.object()["major_version"].toInt();
                        Info->MinorVersion = doc.object()["minor_version"].toInt();

                        Info->IsEnabled = true;
                        Info->Folder = Folder;

                        if(DocumentMeta.object().contains(Info->Name))
                        {
                            Info->IsEnabled = DocumentMeta.object()[Info->Name].toBool();
                        }

                        if(Info->IsEnabled)
                        {
                            if(doc.object().contains("browser"))
                            {
                                foreach(QJsonValue val, doc.object()["browser"].toArray())
                                {
                                    QFile browser_module(Folder + QString("/") + dir + QString("/") + val.toString());
                                    if(browser_module.open(QIODevice::ReadOnly))
                                    {
                                        Info->BrowserCode.append(QString("\n") + QString::fromUtf8(browser_module.readAll()) + QString("\n"));
                                        browser_module.close();
                                    }
                                }
                            }
                            foreach(QJsonValue val, doc.object()["engine"].toArray())
                            {
                                QFile engine_module(Folder + QString("/") + dir + QString("/") + val.toString());
                                if(engine_module.open(QIODevice::ReadOnly))
                                {
                                    Info->EngineCode.append(QString("{\n") + QString::fromUtf8(engine_module.readAll()) + QString("\n}"));
                                    engine_module.close();
                                }
                            }
                            if(doc.object().contains("dll") && InitializeLibraries)
                            {
                                foreach(QJsonValue val, doc.object()["dll"].toArray())
                                {
                                    if(!val.isObject())
                                        continue;
                                    QJsonObject obj = val.toObject();
                                    if(obj.contains("name") && obj.contains("filename64") && obj.contains("filename32") && obj.contains("exportlist"))
                                    {
                                        ModuleDll dll = std::make_shared<ModuleDllClass>();
                                        dll->Name = obj["name"].toString();

                                        #ifdef IS_X64
                                            dll->FileName = obj["filename64"].toString();
                                        #else
                                            dll->FileName = obj["filename32"].toString();
                                        #endif

                                        dll->Library = new QLibrary(Folder + QString("/") + dir + QString("/") + dll->FileName);

                                        dll->Library->load();


                                        if(obj.contains("startdllfunction"))
                                        {
                                            dll->StartDllFunction = (ModuleOnStartFunction)dll->Library->resolve(obj["startdllfunction"].toString().toUtf8().data());
                                        }else
                                        {
                                            dll->StartDllFunction = 0;
                                        }
                                        if(obj.contains("enddllfunction"))
                                        {
                                            dll->EndDllFunction = (ModuleOnEndFunction)dll->Library->resolve(obj["enddllfunction"].toString().toUtf8().data());
                                        }else
                                        {
                                            dll->EndDllFunction = 0;
                                        }

                                        if(obj.contains("startthreadfunction"))
                                        {
                                            dll->StartThreadFunction = (ModuleOnStartFunction)dll->Library->resolve(obj["startthreadfunction"].toString().toUtf8().data());
                                        }else
                                        {
                                            dll->StartThreadFunction = 0;
                                        }

                                        if(obj.contains("iddlefunction"))
                                        {
                                            dll->IddleThreadFunction = (ModuleOnIddleFunction)dll->Library->resolve(obj["iddlefunction"].toString().toUtf8().data());
                                        }else
                                        {
                                            dll->IddleThreadFunction = 0;
                                        }

                                        if(obj.contains("endthreadfunction"))
                                        {
                                            dll->EndThreadFunction = (ModuleOnEndFunction)dll->Library->resolve(obj["endthreadfunction"].toString().toUtf8().data());
                                        }else
                                        {
                                            dll->EndThreadFunction = 0;
                                        }
                                        if(obj.contains("exportlist") && obj["exportlist"].isArray())
                                        {
                                            foreach(QJsonValue val, obj["exportlist"].toArray())
                                            {
                                                if(!val.isObject())
                                                    continue;
                                                QJsonObject obj = val.toObject();
                                                if(obj.contains("name") && obj.contains("workfunction"))
                                                {
                                                    ModuleFunction func = std::make_shared<ModuleFunctionClass>();
                                                    func->Alias = obj["name"].toString();
                                                    if(obj.contains("isasync"))
                                                    {
                                                        func->IsAsync = obj["isasync"].toBool();
                                                    }else
                                                    {
                                                        func->IsAsync = false;
                                                    }

                                                    if(obj.contains("waitinfinite"))
                                                    {
                                                        func->IsWaitInfinite = obj["waitinfinite"].toBool();
                                                    }else
                                                    {
                                                        func->IsWaitInfinite = false;
                                                    }

                                                    func->WorkFunction = (ModuleWorkFunction)dll->Library->resolve(Folder + QString("/") + dir + QString("/") + dll->FileName, obj["workfunction"].toString().toUtf8().data());
                                                    dll->FunctionList.append(func);
                                                }
                                            }
                                        }




                                        Info->Dlls.append(dll);
                                    }
                                }


                            }
                        }
                        //Overwrite modules
                        QMutableListIterator<ModuleInfo> i(List);
                        while (i.hasNext())
                        {
                            if (i.next()->Name == Info->Name)
                                i.remove();
                        }
                        List.append(Info);
                    }
                }
            }
        }
    }

    QList<EmbeddedLanguage> ModuleManager::GetAllEmbeddedLanguages()
    {
        QList<EmbeddedLanguage> Languages;

        QList<IModuleManager::ModuleInfo> Modules = GetModuleInfo(false);
        for(IModuleManager::ModuleInfo Module:Modules)
        {
            if(Module->IsEnabled)
            {
                for(EmbeddedLanguage Language:Module->EmbeddedLanguages)
                {
                    Languages.append(Language);
                }
            }
        }
        return Languages;
    }
    QList<EmbeddedModule> ModuleManager::GetAllEmbeddedModules()
    {
        QList<EmbeddedModule> Result;

        QList<IModuleManager::ModuleInfo> Modules = GetModuleInfo(false);
        for(IModuleManager::ModuleInfo Module:Modules)
        {
            if(Module->IsEnabled)
            {
                for(EmbeddedModule _EmbeddedModule:Module->EmbeddedModules)
                {
                    Result.append(_EmbeddedModule);
                }
            }
        }
        return Result;
    }

    QList<EmbeddedCodeItem> ModuleManager::GetAllEmbeddedCodeItems()
    {
        QList<EmbeddedCodeItem> Result;

        QList<IModuleManager::ModuleInfo> Modules = GetModuleInfo(false);
        for(IModuleManager::ModuleInfo Module:Modules)
        {
            if(Module->IsEnabled)
            {
                for(EmbeddedCodeItem _EmbeddedCodeItem:Module->EmbeddedCodeItems)
                {
                    Result.append(_EmbeddedCodeItem);
                }
            }
        }
        return Result;
    }

    QList<EmbeddedLanguage> ModuleManager::GetAllEmbeddedLanguages(QStringList& ActiveModules)
    {
        QList<EmbeddedLanguage> Languages;

        QList<IModuleManager::ModuleInfo> Modules = GetModuleInfo(false);
        for(IModuleManager::ModuleInfo Module:Modules)
        {
            if(Module->IsEnabled && ActiveModules.contains(Module->Name))
            {
                for(EmbeddedLanguage Language:Module->EmbeddedLanguages)
                {
                    Languages.append(Language);
                }
            }
        }
        return Languages;
    }
    QList<EmbeddedModule> ModuleManager::GetAllEmbeddedModules(QStringList& ActiveModules)
    {
        QList<EmbeddedModule> Result;

        QList<IModuleManager::ModuleInfo> Modules = GetModuleInfo(false);
        for(IModuleManager::ModuleInfo Module:Modules)
        {
            if(Module->IsEnabled && ActiveModules.contains(Module->Name))
            {
                for(EmbeddedModule _EmbeddedModule:Module->EmbeddedModules)
                {
                    Result.append(_EmbeddedModule);
                }
            }
        }
        return Result;
    }
    QList<EmbeddedCodeItem> ModuleManager::GetAllEmbeddedCodeItems(QStringList& ActiveModules)
    {
        QList<EmbeddedCodeItem> Result;

        QList<IModuleManager::ModuleInfo> Modules = GetModuleInfo(false);
        for(IModuleManager::ModuleInfo Module:Modules)
        {
            if(Module->IsEnabled && ActiveModules.contains(Module->Name))
            {
                for(EmbeddedCodeItem _EmbeddedCodeItem:Module->EmbeddedCodeItems)
                {
                    Result.append(_EmbeddedCodeItem);
                }
            }
        }
        return Result;
    }

    QList<IModuleManager::ModuleInfo> ModuleManager::GetModuleInfo(bool InitializeLibraries)
    {
        QList<IModuleManager::ModuleInfo> Res;
        GetModuleInfoFromFolder("modules",Res,InitializeLibraries);
        GetModuleInfoFromFolder(QString("external/") + QString::number(qApp->applicationPid()),Res,InitializeLibraries);
        GetModuleInfoFromFolder("custom",Res,InitializeLibraries);
        return Res;
    }

    bool ModuleManager::IsModuleEnabled(const QString& ModuleName)
    {
        QJsonParseError err;
        QFile meta("modules/meta.json");
        if(meta.open(QIODevice::ReadOnly))
        {
            QByteArray json = meta.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(json, &err);
            meta.close();

            if(err.error == QJsonParseError::NoError)
            {
                if(doc.object().contains(ModuleName) && doc.object()[ModuleName].isBool())
                {
                    return doc.object()[ModuleName].toBool();
                }
            }
        }
        return true;
    }

    void ModuleManager::SetModuleEnabled(const QString& ModuleName, bool IsEnabled)
    {
        QByteArray json;
        {
            QFile meta("modules/meta.json");
            if(meta.open(QIODevice::ReadOnly))
            {
                json = meta.readAll();
                meta.close();
            }
        }

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(json, &err);

        if(err.error == QJsonParseError::NoError)
        {
            {
                QFile meta("modules/meta.json");
                meta.remove();
            }
            QFile meta("modules/meta.json");
            if(meta.open(QIODevice::WriteOnly))
            {
                QJsonObject object = doc.object();
                object[ModuleName] = IsEnabled;
                doc.setObject(object);
                meta.write(doc.toJson());
                return;
            }
        }

        //Error happend

        {
            QFile meta("modules/meta.json");
            meta.remove();
        }

        {
            QFile meta("modules/meta.json");

            if(meta.open(QIODevice::WriteOnly))
            {
                QVariantMap map;
                map.insert(ModuleName, IsEnabled);
                QJsonObject object = QJsonObject::fromVariantMap(map);

                QJsonDocument document;
                document.setObject(object);

                meta.write(document.toJson());
                meta.close();
            }
        }

    }

    int ModuleManager::GetModuleSystemVersion()
    {
        return 1;
    }

    QList<IModuleManager::ModulePreserve> ModuleManager::GetAllModules()
    {
        QList<IModuleManager::ModulePreserve> Result;
        GetModulePreserveFromFolder(QString("external/") + QString::number(qApp->applicationPid()),Result);
        GetModulePreserveFromFolder("custom",Result);
        GetModulePreserveFromFolder("modules",Result);
        return Result;
    }

    QList<IModuleManager::ModulePreserve> ModuleManager::GetModulesUsedInProject(const QString& Project, bool IncludeStandartModulesWithEmbeddedLanguages)
    {
        QList<IModuleManager::ModulePreserve> Result;
        GetModulePreserveFromFolder(QString("external/") + QString::number(qApp->applicationPid()),Result);
        GetModulePreserveFromFolder("custom",Result);

        if(IncludeStandartModulesWithEmbeddedLanguages)
        {
            QList<IModuleManager::ModulePreserve> StandartPreserve;
            QList<IModuleManager::ModuleInfo> StandartInfo;

            GetModuleInfoFromFolder("modules",StandartInfo,false);
            GetModulePreserveFromFolder("modules",StandartPreserve);

            for(const IModuleManager::ModulePreserve& _ModulePreserve: StandartPreserve)
            {
                for(const IModuleManager::ModuleInfo& _ModuleInfo: StandartInfo)
                {
                    if(_ModuleInfo->Name == _ModulePreserve->Name)
                    {
                        if(!_ModuleInfo->EmbeddedLanguages.empty())
                        {
                            Result.append(_ModulePreserve);
                        }
                        break;
                    }
                }
            }

        }
        QMap<QString,QString> ActionToModule;
        for(IModuleManager::ModulePreserve Module:Result)
        {
            QJsonParseError err;
            QFile manifest(Module->Folder + QString("/") + Module->Name + QString("/manifest.json"));
            if(manifest.open(QIODevice::ReadOnly))
            {
                QByteArray json = manifest.readAll();
                QJsonDocument doc = QJsonDocument::fromJson(json, &err);
                manifest.close();

                if(err.error == QJsonParseError::NoError)
                {
                    if(doc.object().contains("actions"))
                    {
                        foreach(QJsonValue val, doc.object()["actions"].toArray())
                        {
                            if(!val.isObject())
                                continue;
                            QJsonObject obj = val.toObject();
                            if(obj.contains("name"))
                            {
                                ActionToModule[obj["name"].toString()] = Module->Name;
                            }
                        }

                    }
                }
            }
        }

        QSet<QString> UsedActions;

        int IndexDatStart = 0;
        while(true)
        {
            IndexDatStart = Project.indexOf("/*Dat:",IndexDatStart);
            if(IndexDatStart < 0)
                break;

            IndexDatStart += 6;

            int IndexDatEnd = Project.indexOf("*/",IndexDatStart);

            QString Base64Data = Project.mid(IndexDatStart,IndexDatEnd - IndexDatStart);

            QJsonParseError err;

            QByteArray JsonData = QByteArray::fromBase64(Base64Data.toUtf8());

            QJsonDocument doc = QJsonDocument::fromJson(JsonData, &err);

            if(err.error == QJsonParseError::NoError)
            {
                if(doc.object().contains("s"))
                {
                    QString ActionName = doc.object().value("s").toString();
                    if(!ActionName.isEmpty())
                        UsedActions.insert(ActionName);
                }
            }


            IndexDatStart = IndexDatEnd + 2;
        }

        QSet<QString> UsedModules;

        for(QString Action:UsedActions)
        {
            if(ActionToModule.contains(Action))
                UsedModules.insert(ActionToModule.value(Action));
        }


        QMutableListIterator<IModuleManager::ModulePreserve> i(Result);
        while (i.hasNext())
        {
            IModuleManager::ModulePreserve Data = i.next();
            if(!Data->IsUnconditionallyIncluded && !UsedModules.contains(Data->Name))
                i.remove();
        }

        return Result;
    }

    QStringList ModuleManager::GetStandartModulesNotUsedInProject(const QString& Project)
    {
        QList<IModuleManager::ModulePreserve> Result;
        GetModulePreserveFromFolder("modules",Result);
        QMap<QString,QString> ActionToModule;
        QSet<QString> UnusedModules;
        for(IModuleManager::ModulePreserve Module:Result)
        {
            if(Module->IsAutogenerated)
            {
                UnusedModules.insert(Module->Name);
                QJsonParseError err;
                QFile manifest(Module->Folder + QString("/") + Module->Name + QString("/manifest.json"));
                if(manifest.open(QIODevice::ReadOnly))
                {
                    QByteArray json = manifest.readAll();
                    QJsonDocument doc = QJsonDocument::fromJson(json, &err);
                    manifest.close();

                    if(err.error == QJsonParseError::NoError)
                    {
                        if(doc.object().contains("actions"))
                        {
                            foreach(QJsonValue val, doc.object()["actions"].toArray())
                            {
                                if(!val.isObject())
                                    continue;
                                QJsonObject obj = val.toObject();
                                if(obj.contains("name"))
                                {
                                    ActionToModule[obj["name"].toString()] = Module->Name;
                                }
                            }

                        }

                        if(doc.object().contains("dependent_actions"))
                        {
                            foreach(QJsonValue val, doc.object()["dependent_actions"].toArray())
                            {
                                if(!val.isString())
                                    continue;
                                QString Name = val.toString();
                                ActionToModule[Name] = Module->Name;
                            }

                        }
                    }
                }
            }
        }

        QSet<QString> UsedActions;

        int IndexDatStart = 0;
        while(true)
        {
            IndexDatStart = Project.indexOf("/*Dat:",IndexDatStart);
            if(IndexDatStart < 0)
                break;

            IndexDatStart += 6;

            int IndexDatEnd = Project.indexOf("*/",IndexDatStart);

            QString Base64Data = Project.mid(IndexDatStart,IndexDatEnd - IndexDatStart);

            QJsonParseError err;

            QByteArray JsonData = QByteArray::fromBase64(Base64Data.toUtf8());

            QJsonDocument doc = QJsonDocument::fromJson(JsonData, &err);

            if(err.error == QJsonParseError::NoError)
            {
                if(doc.object().contains("s"))
                {
                    QString ActionName = doc.object().value("s").toString();
                    if(!ActionName.isEmpty())
                        UsedActions.insert(ActionName);
                }
            }


            IndexDatStart = IndexDatEnd + 2;
        }


        for(QString Action:UsedActions)
        {
            if(ActionToModule.contains(Action))
                UnusedModules.remove(ActionToModule.value(Action));
        }


        return UnusedModules.toList();
    }

    void ModuleManager::ClearPreserveCache()
    {
        ModulePreserveCache.clear();
    }

    bool ModuleManager::PackModules(QList<ModulePreserve>& Modules)
    {
        QSet<QString> CacheName;
        QSet<QString> CurrentNames;
        for(ModulePreserve Module:Modules)
        {
            CurrentNames.insert(Module->Name);
        }
        for(ModulePreserve Module:ModulePreserveCache)
        {
            CacheName.insert(Module->Name);
        }
        if(CacheName == CurrentNames)
        {
            Modules = ModulePreserveCache;
            return false;
        }
        for(IModuleManager::ModulePreserve& Module:Modules)
        {
            QByteArray ZipData;
            QBuffer ZipBuffer(&ZipData);
            ZipBuffer.open(QIODevice::WriteOnly);

            QuaZip Zip(&ZipBuffer);
            Zip.open(QuaZip::mdCreate);
            QString ModuleLocation = Module->Folder + QString("/") + Module->Name;

            if(JlCompress::compressSubDir(&Zip,ModuleLocation,ModuleLocation,true))
            {
                Zip.close();

                if(!Zip.getZipError())
                {
                    Module->Data = ZipData;
                }
            }else
            {
                Zip.close();
            }
        }
        ModulePreserveCache = Modules;
        return true;
    }

    void ModuleManager::UnpackModules(QList<ModulePreserve>& Modules)
    {

        {
            QString ExternalPath = QString("external");
            QDirIterator directories(ExternalPath, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
            while(directories.hasNext())
            {
                directories.next();
                QString dir = directories.filePath();
                QString LockFilePath = QDir::cleanPath(dir + QDir::separator() + "lock");
                QFile LockFile(LockFilePath);
                LockFile.remove();
                if(!LockFile.exists())
                {
                    QDir(dir).removeRecursively();
                }
            }
        }



        QDir ExternalDir(QString("external/") + QString::number(qApp->applicationPid()));
        ExternalDir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        QStringList ExternalModuleList = ExternalDir.entryList();
        for(QString& Module: ExternalModuleList)
        {
            QDir ModuleDir(ExternalDir.absoluteFilePath(Module));
            ModuleDir.removeRecursively();
        }
        ExternalDir.mkpath(".");

        for(IModuleManager::ModulePreserve& Module:Modules)
        {
            QDir ModuleDir(ExternalDir.absoluteFilePath(Module->Name));

            QBuffer DataBuffer(&(Module->Data));
            DataBuffer.open(QIODevice::ReadOnly);

            QuaZip zip(&DataBuffer);
            zip.open(QuaZip::mdUnzip);

            QStringList extracted;
            if (!zip.goToFirstFile()) {
                zip.close();
                continue;
            }
            do {
                QString name = zip.getCurrentFileName();
                QString absFilePath = ModuleDir.absoluteFilePath(name);
                if (!JlCompress::extractFile(&zip, "", absFilePath)) {
                    JlCompress::removeFile(extracted);
                    zip.close();
                    break;
                }
                extracted.append(absFilePath);
            } while (zip.goToNextFile());

            zip.close();
            if(zip.getZipError()!=0) {
                JlCompress::removeFile(extracted);
            }
        }
    }

}
