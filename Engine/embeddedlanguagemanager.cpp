#include "embeddedlanguagemanager.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QSettings>
#include <QTimer>
#include "nodeconnector.h"
#include "every_cpp.h"


namespace BrowserAutomationStudioFramework
{
    EmbeddedLanguageManager::EmbeddedLanguageManager(QObject *parent) : IEmbeddedLanguageManager(parent)
    {
        _WasError = false;
        WriteLogs = false;
        IsSilent = false;
        IsActive = false;
        IsChecking = false;
        HideInterface = false;
        IsRecord = false;
        RunWithoutEmbeddedLanguages = false;
        _EmbeddedLanguageInstallDialog = 0;
    }

    QList<EmbeddedLanguage> EmbeddedLanguageManager::GetMainLanguages()
    {
        return AllLanguages;
    }

    QList<EmbeddedModule> EmbeddedLanguageManager::GetMainModules()
    {
        return AllModules;
    }

    void EmbeddedLanguageManager::SetIsRecord(bool IsRecord)
    {
        this->IsRecord = IsRecord;
        emit IsRecordChanged(IsRecord);
    }

    void EmbeddedLanguageManager::CloseDialog()
    {
        if(_EmbeddedLanguageInstallDialog)
        {
            _EmbeddedLanguageInstallDialog->hide();
            _EmbeddedLanguageInstallDialog->deleteLater();
            _EmbeddedLanguageInstallDialog = 0;
        }
        emit InitializationHide();
    }

    void EmbeddedLanguageManager::Restart()
    {
        Stop();
        Start();
    }

    void EmbeddedLanguageManager::Error(const QString& String)
    {
        _WasError = true;
        _ErrorString = String;
        if(IsSilent)
        {
            emit Started();
        }else
        {
            if(IsChecking)
            {
                IsCheckingFail = true;
                CheckFailError = String;
                ReadData(DataBeforeChecking);
                DataBeforeChecking.clear();
                QTimer::singleShot(1,this,SLOT(Restart()));
            }else
            {
                ActivateWindow();
                if(_EmbeddedLanguageInstallDialog)
                    _EmbeddedLanguageInstallDialog->SetIsError();
                emit InitializationFail(String);
            }
        }

    }



    EmbeddedLanguageManager::~EmbeddedLanguageManager()
    {
        CloseDialog();
    }

    void EmbeddedLanguageManager::SetDiffPatcher(IDiffPatcher *DiffPatcher)
    {
        this->DiffPatcher = DiffPatcher;
    }

    void EmbeddedLanguageManager::ReadLanguageList(bool WriteLogs, bool IsSilent, bool HideInterface, QList<EmbeddedLanguage>& Languages, QList<EmbeddedModule>& Modules)
    {
        this->HideInterface = HideInterface;
        this->IsSilent = IsSilent;
        this->WriteLogs = WriteLogs;
        ReadLanguageListLocal(AllLanguages,AllModules);

        for(EmbeddedLanguage& _Language:Languages)
        {
            MergeLanguage(_Language);
        }
        for(EmbeddedModule& _Module:Modules)
        {
            MergeModule(_Module);
        }

    }

    void EmbeddedLanguageManager::MergeLanguage(EmbeddedLanguage& _Language)
    {
        bool Found = false;
        for(EmbeddedLanguage& LangCurrent:AllLanguages)
        {
            if(LangCurrent.Name == _Language.Name && LangCurrent.Version == _Language.Version)
            {
                Found = true;
                break;
            }
        }
        if(!Found)
            AllLanguages.append(_Language);
    }

    void EmbeddedLanguageManager::MergeModule(EmbeddedModule& _Module)
    {
        bool Found = false;
        QString LastVersion = "*";
        for(EmbeddedModule& ModulesCurrent:AllModules)
        {
            if(ModulesCurrent.ModuleName == _Module.ModuleName && ModulesCurrent.LanguageVersion == _Module.LanguageVersion && ModulesCurrent.LanguageName == _Module.LanguageName)
            {
                LastVersion = ModulesCurrent.ModuleVersion;
                Found = true;
                if(LastVersion == "*")
                {
                   LastVersion = _Module.ModuleVersion;
                }else if(_Module.ModuleVersion != "*")
                {
                    QStringList CurrentList = _Module.ModuleVersion.split(".");
                    QStringList LastList = LastVersion.split(".");

                    int CurrentA = 0;
                    int CurrentB = 0;
                    int CurrentC = 0;
                    int LastA = 0;
                    int LastB = 0;
                    int LastC = 0;
                    if(CurrentList.size() > 0)
                    {
                        CurrentA = CurrentList.at(0).toInt();
                    }
                    if(CurrentList.size() > 1)
                    {
                        CurrentB = CurrentList.at(1).toInt();
                    }
                    if(CurrentList.size() > 2)
                    {
                        CurrentC = CurrentList.at(2).toInt();
                    }
                    if(LastList.size() > 0)
                    {
                        LastA = LastList.at(0).toInt();
                    }
                    if(LastList.size() > 1)
                    {
                        LastB = LastList.at(1).toInt();
                    }
                    if(LastList.size() > 2)
                    {
                        LastC = LastList.at(2).toInt();
                    }

                    //If current version is greater than latest
                    if(CurrentA > LastA || (CurrentA == LastA && CurrentB > LastB) || (CurrentA == LastA && CurrentB == LastB && CurrentC > LastC))
                    {
                        LastVersion = _Module.ModuleVersion;
                    }

                }
                ModulesCurrent.ModuleVersion = LastVersion;

                break;
            }
        }
        if(!Found)
            AllModules.append(_Module);
    }

    void EmbeddedLanguageManager::ReadLanguageListLocal(QList<EmbeddedLanguage>& Languages, QList<EmbeddedModule>& Modules)
    {
        Languages.clear();
        QSettings SettingsWorker("settings_worker.ini",QSettings::IniFormat);
        QString LanguageStrings = SettingsWorker.value("Languages","").toString();
        for(QString& LanguageString:LanguageStrings.split(";"))
        {
            QStringList LanguageSplit = LanguageString.split(",");
            if(LanguageSplit.length() == 2 && LanguageSplit[0] == QString("Node") && (LanguageSplit[1] == QString("8.6.0") || LanguageSplit[1] == QString("12.18.3") || LanguageSplit[1] == QString("18.10.0")))
            {
                EmbeddedLanguage l;
                l.Name = LanguageSplit[0];
                l.Version = LanguageSplit[1];
                Languages.append(l);
            }
        }
        Modules.clear();
        QString ModuleStrings = SettingsWorker.value("Modules","").toString();
        for(QString& ModuleString:ModuleStrings.split(";"))
        {
            QStringList ModuleSplit = ModuleString.split(",");
            if(ModuleSplit.length() == 4)
            {
                EmbeddedModule m;
                m.LanguageName = ModuleSplit[0];
                m.LanguageVersion = ModuleSplit[1];
                m.ModuleName = ModuleSplit[2];
                m.ModuleVersion = ModuleSplit[3];
                Modules.append(m);
            }
        }
    }

    QList<EmbeddedCodeItem> EmbeddedLanguageManager::GetEmbeddedCode()
    {
        return AllData;
    }

    void EmbeddedLanguageManager::SetModuleCode(QList<EmbeddedCodeItem> AdditionalData)
    {
        this->AdditionalData = AdditionalData;
    }


    void EmbeddedLanguageManager::ReadData(const QString& EmbeddedData)
    {
        AllData.clear();

        QJsonDocument JsonDocument = QJsonDocument::fromJson(EmbeddedData.toUtf8());
        QJsonArray JsonArray = JsonDocument.array();
        for(QJsonValue Value: JsonArray)
        {
            QJsonObject JsonObject = Value.toObject();
            EmbeddedCodeItem Item;
            Item.Data = JsonObject["data"].toString();
            Item.DataName = JsonObject["data_name"].toString();
            Item.DataType = JsonObject["data_type"].toInt();
            Item.LanguageName = JsonObject["language_name"].toString();
            Item.LanguageVersion = JsonObject["language_version"].toString();
            AllData.append(Item);
        }
    }

    QString EmbeddedLanguageManager::WriteData()
    {
        if(AllData.empty())
        {
            return "[]";
        }
        QJsonArray a;
        for(EmbeddedCodeItem& Item:AllData)
        {
            QJsonObject o;
            o["data"] = Item.Data;
            o["data_name"] = Item.DataName;
            o["data_type"] = Item.DataType;
            o["language_name"] = Item.LanguageName;
            o["language_version"] = Item.LanguageVersion;
            a.append(o);
        }

        QJsonDocument doc;
        doc.setArray(a);
        QString DataRaw = doc.toJson(QJsonDocument::Compact);

        return DataRaw;
    }

    QString EmbeddedLanguageManager::SerializeData()
    {
        QString DataRaw = WriteData();
        return DataRaw;
    }

    void EmbeddedLanguageManager::DeserializeData(const QString& Data)
    {
        DataBeforeChecking = WriteData();


        QString JsonString = Data;
        int Index = JsonString.indexOf("|");
        if(Index >= 0)
        {
            CheckId = JsonString.mid(0,Index);
            JsonString = JsonString.mid(Index + 1);
        }

        ReadData(JsonString);
        if(RunWithoutEmbeddedLanguages)
        {
            emit CheckResult(CheckId, true, "");
            CheckId.clear();
        }else
        {
            IsChecking = true;
            IsCheckingFail = false;
            Stop();
            Start();
        }
    }

    bool EmbeddedLanguageManager::WasError()
    {
        return _WasError;
    }

    bool EmbeddedLanguageManager::IsRunWithoutLanguages()
    {
        return RunWithoutEmbeddedLanguages;
    }


    QString EmbeddedLanguageManager::ErrorString()
    {
        return _ErrorString;
    }

    void EmbeddedLanguageManager::Start()
    {
        RunWithoutEmbeddedLanguages = false;
        StartInternal();
    }

    void EmbeddedLanguageManager::StartInternal()
    {

        _WasError = false;
        _ErrorString.clear();
        DialogTitle.clear();
        DialogLabel.clear();
        if(IsSilent)
        {
            //QTimer::singleShot(20000,this,SLOT(Timeout()));
        }else
        {
            QTimer::singleShot(1000,this,SLOT(ActivateWindow()));
        }
        Stop();
        if(RunWithoutEmbeddedLanguages)
        {
            LanguageStartIterator = 999999;
        }else
        {
            LanguageStartIterator = 0;
        }

        CurrentRetryNumber = 0;

        StartNextLanguage();
    }

    void EmbeddedLanguageManager::Timeout()
    {
        _WasError = true;
        _ErrorString = tr("Timeout");
        CloseDialog();
        emit Started();
    }

    void EmbeddedLanguageManager::Cancel()
    {
        _WasError = true;
        _ErrorString = tr("Canceled by user");

        CloseDialog();
        emit Started();
    }

    void EmbeddedLanguageManager::ContinueEmbeddedLanguages()
    {
        RunWithoutEmbeddedLanguages = true;
        Stop();
        StartInternal();
    }

    void EmbeddedLanguageManager::ActivateWindow()
    {
        if(!_EmbeddedLanguageInstallDialog && !IsActive && !IsChecking && !HideInterface)
        {
            _EmbeddedLanguageInstallDialog = new EmbeddedLanguageInstallDialog();
            connect(_EmbeddedLanguageInstallDialog,SIGNAL(RunWithoutEmbeddedLanguages()),this,SLOT(ContinueEmbeddedLanguages()));
            connect(_EmbeddedLanguageInstallDialog,SIGNAL(Close()),this,SLOT(Cancel()));
            _EmbeddedLanguageInstallDialog->SetLabel(DialogLabel);
            DialogLabel.clear();
            _EmbeddedLanguageInstallDialog->SetTitle(DialogTitle);
            _EmbeddedLanguageInstallDialog->setModal(true);
            _EmbeddedLanguageInstallDialog->show();
        }
        emit InitializationShow();

    }

    void EmbeddedLanguageManager::StartNextLanguage()
    {
        if(LanguageStartIterator >= AllLanguages.length())
        {
            CloseDialog();
            IsActive = true;
            if(IsChecking)
            {
                if(!CheckId.isEmpty())
                {
                    if(IsCheckingFail)
                    {
                        emit CheckResult(CheckId,false,CheckFailError);
                    }else
                        emit CheckResult(CheckId,true,"");
                }
                IsChecking = false;
                DataBeforeChecking.clear();
                CheckId.clear();
            }else
            {
                emit Started();
            }


            return;
        }
        EmbeddedLanguage& Lang = AllLanguages[LanguageStartIterator];

        QString RetryString;
        if(CurrentRetryNumber > 0)
        {
            RetryString = QString(tr(" (retry %1)")).arg(QString::number(CurrentRetryNumber + 1));
        }
        DialogTitle = QString(tr("Starting %1 %2%3     ")).arg(Lang.Name).arg(Lang.Version).arg(RetryString);
        if(_EmbeddedLanguageInstallDialog)
            _EmbeddedLanguageInstallDialog->SetTitle(DialogTitle);
        emit InitializationTitle(DialogTitle);

        /* Validation */
        if(Lang.Name != "Node" || (Lang.Version != "8.6.0" && Lang.Version != "12.18.3" && Lang.Version != "18.10.0"))
        {
            Error(QString(tr("Unknown language or version %1 %2")).arg(Lang.Name).arg(Lang.Version));
            return;
        }

        NodeConnector *Connector = 0;

        if(Lang.Name == "Node")
        {
            /* Setup node connector */
            Connector = new NodeConnector(this);
            if(Lang.Version != "8.6.0")
                Connector->SetHasPipeVersion();

            if(LanguageStartIterator >= AllLanguages.length() - 1)
                Connector->SetAutoCleanWhenFinishedSuccess();
            if(CurrentRetryNumber >= MaxRetryNumber)
                Connector->SetAutoCleanWhenFinishedFail();
            Connector->SetLanguageVersion(Lang.Version);
            Connector->SetIsRecord(IsRecord);

            /* Add all modules */
            for(EmbeddedModule&m: AllModules)
            {
                if(m.LanguageName == Lang.Name && m.LanguageVersion == Lang.Version)
                    Connector->AddModule(m.ModuleName,m.ModuleVersion);
            }

            /* Add functions and files */
            QList<EmbeddedCodeItem> Data;
            Data.append(AllData);
            Data.append(AdditionalData);
            for(EmbeddedCodeItem&i: Data)
            {
                if(i.LanguageName == Lang.Name && i.LanguageVersion == Lang.Version)
                {
                    if(i.DataType == 0)
                    {
                        Connector->AddFunction(i.DataName,i.Data);
                    }else if(i.DataType == 1)
                    {
                        Connector->AddFile(i.DataName,i.Data);
                    }

                }
            }

        }

        connect(Connector,SIGNAL(Started(bool,QString,bool)),this,SLOT(NextLanguageStarted(bool,QString,bool)));
        connect(Connector,SIGNAL(Log(QString)),this,SLOT(ConnectorStartingLog(QString)));
        connect(Connector,SIGNAL(LogFile(QString)),this,SIGNAL(InitializationLog(QString)));
        connect(Connector,SIGNAL(ReceivedResultData(quint64,QString,bool,QString)),this,SIGNAL(ReceivedResultData(quint64,QString,bool,QString)));
        connect(Connector,SIGNAL(ReceivedApiData(quint64,QString,QString)),this,SIGNAL(ReceivedApiData(quint64,QString,QString)));
        connect(Connector,SIGNAL(LogConsole(QString,quint64)),this,SIGNAL(ConsoleLog(QString,quint64)));
        connect(this,SIGNAL(IsRecordChanged(bool)),Connector,SLOT(SetIsRecord(bool)));
        Connectors.append(Connector);
        Connector->Start();

    }

    void EmbeddedLanguageManager::ConnectorStartingLog(QString Text)
    {
        DialogLabel = Text;

        if(_EmbeddedLanguageInstallDialog)
        {
            _EmbeddedLanguageInstallDialog->SetLabel(DialogLabel);
            DialogLabel.clear();
        }
        emit InitializationLog(Text);
    }

    void EmbeddedLanguageManager::NextLanguageStarted(bool IsError, QString ErrorMessage, bool AllowRetry)
    {
        disconnect(Connectors.last(),SIGNAL(Started(bool,QString,bool)),this,SLOT(NextLanguageStarted(bool,QString,bool)));
        disconnect(Connectors.last(),SIGNAL(Log(QString)),this,SLOT(ConnectorStartingLog(QString)));

        if(!Connectors.isEmpty() && AllowRetry && IsError && (CurrentRetryNumber < MaxRetryNumber))
        {
            CurrentRetryNumber++;
            Connectors.last()->deleteLater();
            Connectors.removeLast();
            StartNextLanguage();
            return;
        }

        CurrentRetryNumber = 0;

        _WasError = IsError;
        _ErrorString = ErrorMessage;
        if(IsError)
        {
            Error(_ErrorString);
            return;
        }

        LanguageStartIterator++;
        StartNextLanguage();

    }

    void EmbeddedLanguageManager::Stop()
    {
        IsActive = false;
        CloseDialog();
        for(ILanguageConnector* Connector: Connectors)
        {
            Connector->Stop();
            Connector->deleteLater();
        }
        Connectors.clear();
    }

    quint64 EmbeddedLanguageManager::SendFunction(const QString& Variables, const QString& Function, const QString& LanguageName, const QString& LanguageVersion, QString& StartErrorString)
    {
        if(RunWithoutEmbeddedLanguages)
        {
            StartErrorString = tr("Running script without embedded languages");
            return 0;
        }

        ILanguageConnector* C = 0;

        for(ILanguageConnector* Connector: Connectors)
        {
            if(Connector->GetLanguageName() == LanguageName && Connector->GetLanguageVersion() == LanguageVersion)
            {
                C = Connector;
            }
        }

        if(!C)
        {
            StartErrorString = tr("Failed to find language connector");
            return 0;
        }

        quint64 res = C->SendFunction(Variables, Function);
        if(!res)
        {
            StartErrorString = tr("Error during send function to connector");
            return 0;
        }

        return res;
    }

    void EmbeddedLanguageManager::SendApiResponce(const QString& Variables, quint64 Id, const QString& LanguageName, const QString& LanguageVersion)
    {
        ILanguageConnector* C = 0;

        for(ILanguageConnector* Connector: Connectors)
        {
            if(Connector->GetLanguageName() == LanguageName && Connector->GetLanguageVersion() == LanguageVersion)
            {
                C = Connector;
            }
        }

        if(!C)
            return;

        C->SendApiResponce(Variables, Id);

    }

    void EmbeddedLanguageManager::KillTask(quint64 Id, const QString& LanguageName, const QString& LanguageVersion)
    {
        ILanguageConnector* C = 0;

        for(ILanguageConnector* Connector: Connectors)
        {
            if(Connector->GetLanguageName() == LanguageName && Connector->GetLanguageVersion() == LanguageVersion)
            {
                C = Connector;
            }
        }

        if(!C)
            return;

        C->KillTask(Id);
    }

}
