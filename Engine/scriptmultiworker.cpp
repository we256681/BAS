#include "scriptmultiworker.h"
#include <QTime>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>
#include "csvhelperwrapper.h"
#include "browserextensionmanager.h"
#include <limits>
#include "every_cpp.h"


namespace BrowserAutomationStudioFramework
{
    ScriptMultiWorker::ScriptMultiWorker(QObject *parent) :
        IMultiWorker(parent), engine(0), Waiter(0), ResourceHandlers(0), IsAborted(false), StageTimeoutTimer(0), DoTrace(false), Helper(0), IsRecord(false), DieInstant(false), SuccessNumber(0), FailNumber(0), CurrentThreadNumber(-1), ProfilerData(0), EmbeddedLanguageManager(0), IsStudio(false)
    {
        _PcapDNSListenServer = new PcapDNSListenServer(this);
        _BrowserExtensionManager = new BrowserExtensionManager(this);
    }


    void ScriptMultiWorker::SetProjectPath(const QString& ProjectPath)
    {
        this->ProjectPath = ProjectPath;
    }
    QString ScriptMultiWorker::GetProjectPath()
    {
        return ProjectPath;
    }

    void ScriptMultiWorker::SetModuleManager(IModuleManager *ModuleManager)
    {
        this->ModuleManager = ModuleManager;
    }

    IModuleManager* ScriptMultiWorker::GetModuleManager()
    {
        return ModuleManager;
    }

    void ScriptMultiWorker::SetDatabaseConnector(IDatabaseConnector *DatabaseConnector)
    {
        this->DatabaseConnector = DatabaseConnector;
    }

    IDatabaseConnector * ScriptMultiWorker::GetDatabaseConnector()
    {
        return DatabaseConnector;
    }

    void ScriptMultiWorker::SetCsvHelper(ICsvHelper *CsvHelper)
    {
        this->CsvHelper = CsvHelper;
    }

    ICsvHelper * ScriptMultiWorker::GetCsvHelper()
    {
        return CsvHelper;
    }

    void ScriptMultiWorker::SetBrowserVersionSelector(IBrowserVersionSelector *BrowserVersionSelector)
    {
        this->_BrowserVersionSelector = BrowserVersionSelector;
    }

    IBrowserVersionSelector * ScriptMultiWorker::GetBrowserVersionSelector()
    {
        return _BrowserVersionSelector;
    }

    void ScriptMultiWorker::SetProfilerData(IProfilerData *ProfilerData)
    {
        this->ProfilerData = ProfilerData;
    }

    IProfilerData * ScriptMultiWorker::GetProfilerData()
    {
        return ProfilerData;
    }

    void ScriptMultiWorker::SetEmbeddedLanguageManager(IEmbeddedLanguageManager *EmbeddedLanguageManager)
    {
        this->EmbeddedLanguageManager = EmbeddedLanguageManager;
    }

    IEmbeddedLanguageManager * ScriptMultiWorker::GetEmbeddedLanguageManager()
    {
        return EmbeddedLanguageManager;
    }

    void ScriptMultiWorker::SetStringBuilder(IStringBuilder *StringBuilder)
    {
        this->StringBuilder = StringBuilder;
    }

    IStringBuilder * ScriptMultiWorker::GetStringBuilder()
    {
        return StringBuilder;
    }

    void ScriptMultiWorker::SetHelperFactory(IHelperFactory* HelperFactory)
    {
        this->HelperFactory = HelperFactory;
    }
    IHelperFactory* ScriptMultiWorker::GetHelperFactory()
    {
        return HelperFactory;
    }

    void ScriptMultiWorker::SetHtmlParserFactory(IHtmlParserFactory* HtmlParserFactory)
    {
        this->HtmlParserFactory = HtmlParserFactory;
    }

    IHtmlParserFactory* ScriptMultiWorker::GetHtmlParserFactory()
    {
        return HtmlParserFactory;
    }

    void ScriptMultiWorker::SetProperties(IProperties* Properties)
    {
        this->Properties = Properties;
    }

    IProperties* ScriptMultiWorker::GetProperties()
    {
        return Properties;
    }

    void ScriptMultiWorker::SetWorkerSettings(IWorkerSettings* WorkerSettings)
    {
        this->WorkerSettings = WorkerSettings;
    }

    IWorkerSettings* ScriptMultiWorker::GetWorkerSettings()
    {
        return WorkerSettings;
    }

    void ScriptMultiWorker::SetReportData(IScriptMultiWorkerReportData *ReportData)
    {
        this->ReportData = ReportData;
    }

    IScriptMultiWorkerReportData * ScriptMultiWorker::GetReportData()
    {
        return ReportData;
    }


    void ScriptMultiWorker::SetHttpClientFactory(IHttpClientFactory* HttpClientFactory)
    {
        this->HttpClientFactory = HttpClientFactory;
        this->_BrowserExtensionManager->SetHttpClientFactory(HttpClientFactory);
    }

    IHttpClientFactory* ScriptMultiWorker::GetHttpClientFactory()
    {
        return HttpClientFactory;
    }

    void ScriptMultiWorker::SetPop3ClientFactory(IPop3ClientFactory* Pop3ClientFactory)
    {
        this->Pop3ClientFactory = Pop3ClientFactory;
    }

    IPop3ClientFactory* ScriptMultiWorker::GetPop3ClientFactory()
    {
        return Pop3ClientFactory;
    }

    void ScriptMultiWorker::SetImapClientFactory(IImapClientFactory* ImapClientFactory)
    {
        this->ImapClientFactory = ImapClientFactory;
    }

    IImapClientFactory* ScriptMultiWorker::GetImapClientFactory()
    {
        return ImapClientFactory;
    }


    void ScriptMultiWorker::SetPreprocessor(IPreprocessor *Preprocessor)
    {
        this->Preprocessor = Preprocessor;
    }

    IPreprocessor * ScriptMultiWorker::GetPreprocessor()
    {
        return Preprocessor;

    }


    void ScriptMultiWorker::SetWorkerFactory(IWorkerFactory *WorkerFactory)
    {
        this->WorkerFactory = WorkerFactory;
    }

    IWorkerFactory * ScriptMultiWorker::GetWorkerFactory()
    {
        return WorkerFactory;
    }

    void ScriptMultiWorker::SetBrowserFactory(IBrowserFactory *BrowserFactory)
    {
        this->BrowserFactory = BrowserFactory;
    }

    IBrowserFactory * ScriptMultiWorker::GetBrowserFactory()
    {
        return BrowserFactory;
    }

    void ScriptMultiWorker::SetLogger(ILogger *Logger)
    {
        this->Logger = Logger;
    }

    ILogger * ScriptMultiWorker::GetLogger()
    {
        return Logger;
    }

    void ScriptMultiWorker::SetResults1(ILogger *Results1)
    {
        this->Results1 = Results1;
    }
    ILogger * ScriptMultiWorker::GetResults1()
    {
        return Results1;
    }

    void ScriptMultiWorker::SetResults2(ILogger *Results2)
    {
        this->Results2 = Results2;
    }
    ILogger * ScriptMultiWorker::GetResults2()
    {
        return Results2;
    }

    void ScriptMultiWorker::SetResults3(ILogger *Results3)
    {
        this->Results3 = Results3;
    }
    ILogger * ScriptMultiWorker::GetResults3()
    {
        return Results3;
    }

    void ScriptMultiWorker::SetResults4(ILogger *Results4)
    {
        this->Results4 = Results4;
    }
    ILogger * ScriptMultiWorker::GetResults4()
    {
        return Results4;
    }

    void ScriptMultiWorker::SetResults5(ILogger *Results5)
    {
        this->Results5 = Results5;
    }
    ILogger * ScriptMultiWorker::GetResults5()
    {
        return Results5;
    }

    void ScriptMultiWorker::SetResults6(ILogger *Results6)
    {
        this->Results6 = Results6;
    }
    ILogger * ScriptMultiWorker::GetResults6()
    {
        return Results6;
    }

    void ScriptMultiWorker::SetResults7(ILogger *Results7)
    {
        this->Results7 = Results7;
    }
    ILogger * ScriptMultiWorker::GetResults7()
    {
        return Results7;
    }

    void ScriptMultiWorker::SetResults8(ILogger *Results8)
    {
        this->Results8 = Results8;
    }
    ILogger * ScriptMultiWorker::GetResults8()
    {
        return Results8;
    }

    void ScriptMultiWorker::SetResults9(ILogger *Results9)
    {
        this->Results9 = Results9;
    }
    ILogger * ScriptMultiWorker::GetResults9()
    {
        return Results9;
    }

    void ScriptMultiWorker::SetScript(const QString& Script)
    {
        this->Script = Script;
    }

    QString ScriptMultiWorker::GetScript()
    {
        return Script;
    }

    void ScriptMultiWorker::SetScriptSuspender(IScriptSuspender* ScriptSuspender)
    {
        this->ScriptSuspender = ScriptSuspender;

    }

    IScriptSuspender* ScriptMultiWorker::GetScriptSuspender()
    {
        return ScriptSuspender;
    }

    void ScriptMultiWorker::SetAbortFunction(const QString& AbortFunction)
    {
        this->AbortFunction = AbortFunction;
    }


    void ScriptMultiWorker::SetWaiterFactory(IWaiterFactory* WaiterFactory)
    {
        this->WaiterFactory = WaiterFactory;
        if(Waiter)
            Waiter->deleteLater();
        Waiter = WaiterFactory->CreateWaiter();
        Waiter->setParent(this);
    }
    IWaiterFactory* ScriptMultiWorker::GetWaiterFactory()
    {
        return WaiterFactory;
    }

    void ScriptMultiWorker::SetScriptResources(IScriptResources* ScriptResources)
    {
        this->ScriptResources = ScriptResources;
    }
    IScriptResources* ScriptMultiWorker::GetScriptResources()
    {
        return ScriptResources;
    }

    void ScriptMultiWorker::SetResourceHandlersFactory(IResourceHandlersFactory* ResourceHandlersFactory)
    {
        this->ResourceHandlersFactory = ResourceHandlersFactory;
        if(ResourceHandlers)
            ResourceHandlers->deleteLater();
        ResourceHandlers = ResourceHandlersFactory->CreateResourceHandlers();
        ResourceHandlers->setParent(this);
    }
    IResourceHandlersFactory* ScriptMultiWorker::GetResourceHandlersFactory()
    {
        return ResourceHandlersFactory;
    }

    void ScriptMultiWorker::SetSolverFactory(ISolverFactory* FactorySolver)
    {
        this->FactorySolver = FactorySolver;
    }
    ISolverFactory* ScriptMultiWorker::GetSolverFactory()
    {
        return FactorySolver;
    }

    void ScriptMultiWorker::SetEngineResources(IEngineResources* EngineRes)
    {
        this->EngineRes = EngineRes;
    }
    IEngineResources* ScriptMultiWorker::GetEngineResources()
    {
        return EngineRes;
    }

    void ScriptMultiWorker::SetDoTrace(bool DoTrace)
    {
        this->DoTrace = DoTrace;
    }

    bool ScriptMultiWorker::GetDoTrace()
    {
        return DoTrace;
    }

    void ScriptMultiWorker::SetAdditionEngineScripts(const QList<QString>& AdditionalScripts)
    {
        this->AdditionalScripts = AdditionalScripts;
    }

    QList<QString> ScriptMultiWorker::GetAdditionEngineScripts()
    {
        return AdditionalScripts;
    }

    void ScriptMultiWorker::SetIsRecord(bool IsRecord)
    {
        this->IsRecord = IsRecord;
    }

    void ScriptMultiWorker::SetEmptyProject()
    {
        EmptyProject = true;
    }


    void ScriptMultiWorker::SetIsStudio(bool IsStudio)
    {
        this->IsStudio = IsStudio;
    }


    bool ScriptMultiWorker::GetIsRecord()
    {
        return IsRecord;
    }

    void ScriptMultiWorker::ProgressValueSlot(int value)
    {
        emit ProgressValue(value);
    }

    void ScriptMultiWorker::ProgressMaximumSlot(int max)
    {
        emit ProgressMaximum(max);
    }

    void ScriptMultiWorker::AddModule(QObject *Module, const QString& Name, bool AddToMultiWorker, bool AddToWorker)
    {
        if(AddToMultiWorker)
            ModulesMultiWorker.insert(Name,Module);

        if(AddToWorker)
            ModulesScriptWorker.insert(Name,Module);
    }

    void ScriptMultiWorker::NoResources(const QString& Message)
    {
        Logger->WriteFail(Message, LogFail);
    }

    void ScriptMultiWorker::WaitForAllModuleFunctionsToFinish()
    {
        //Set NeedToStop variable, this is a signal for all dlls, that funciton must finish
        emit StopAllFunctions();

        QTime EndTime = QTime::currentTime().addSecs(5);

        while(true)
        {
            //Check how many functions are working
            int NumberOfWorkingFunctions = 0;
            emit GetNumberOfRunningFunctions(&NumberOfWorkingFunctions);

            //Timeout
            if(QTime::currentTime() >= EndTime)
                break;

            if(!NumberOfWorkingFunctions)
                break;
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }

    void ScriptMultiWorker::Run()
    {
        connect(BrowserFactory->GetPCResourcesSmoothUsage(),SIGNAL(Log(QString)),this,SLOT(NoResources(QString)));
        _PcapDNSListenServer->Start();
        PcapPort = _PcapDNSListenServer->GetPort();
        if(!Helper)
        {
            Helper = HelperFactory->GetHelper();
            Helper->setParent(this);
        }

        engine = new QScriptEngine(this);
        NeedToSetAsyncResult = false;

        QScriptValue ScriptMultiWorkerValue = engine->newQObject(this);
        engine->globalObject().setProperty("MultiScriptWorker", ScriptMultiWorkerValue);

        QScriptValue CsvHelperValue = engine->newQObject(new CsvHelperWrapper(engine,CsvHelper,this));
        engine->globalObject().setProperty("CsvHelper", CsvHelperValue);

        QScriptValue Results1Value = engine->newQObject(Results1);
        engine->globalObject().setProperty("Results1", Results1Value);

        QScriptValue Results2Value = engine->newQObject(Results2);
        engine->globalObject().setProperty("Results2", Results2Value);

        QScriptValue Results3Value = engine->newQObject(Results3);
        engine->globalObject().setProperty("Results3", Results3Value);

        QScriptValue Results4Value = engine->newQObject(Results4);
        engine->globalObject().setProperty("Results4", Results4Value);

        QScriptValue Results5Value = engine->newQObject(Results5);
        engine->globalObject().setProperty("Results5", Results5Value);

        QScriptValue Results6Value = engine->newQObject(Results6);
        engine->globalObject().setProperty("Results6", Results6Value);

        QScriptValue Results7Value = engine->newQObject(Results7);
        engine->globalObject().setProperty("Results7", Results7Value);

        QScriptValue Results8Value = engine->newQObject(Results8);
        engine->globalObject().setProperty("Results8", Results8Value);

        QScriptValue Results9Value = engine->newQObject(Results9);
        engine->globalObject().setProperty("Results9", Results9Value);

        QScriptValue PropertiesValue = engine->newQObject(Properties);
        engine->globalObject().setProperty("Properties", PropertiesValue);


        QScriptValue EngineResValue = engine->newQObject(EngineRes);
        engine->globalObject().setProperty("EngineRes", EngineResValue);

        QScriptValue ResourceHandlersValue = engine->newQObject(ResourceHandlers);
        engine->globalObject().setProperty("ResourceHandlers", ResourceHandlersValue);

        QScriptValue LoggerValue = engine->newQObject(Logger);
        engine->globalObject().setProperty("Logger", LoggerValue);

        QScriptValue WaiterValue = engine->newQObject(Waiter);
        engine->globalObject().setProperty("Waiter", WaiterValue);

        QScriptValue HelperValue = engine->newQObject(Helper);
        engine->globalObject().setProperty("Helper", HelperValue);

        QHash<QString,QObject*>::iterator i = ModulesMultiWorker.begin();
        while (i != ModulesMultiWorker.end())
        {
            QScriptValue Value = engine->newQObject(i.value());
            engine->globalObject().setProperty(i.key(), Value);
            i++;
        }

        connect(ScriptSuspender, SIGNAL(Suspended(IWorker*)), this, SLOT(WorkerSuspended(IWorker*)));
        connect(ScriptSuspender, SIGNAL(Resumed(IWorker*)), this, SLOT(WorkerResumed()));

        foreach(QString script, ScriptResources->GetMultiEngineScripts())
            engine->evaluate(script);



        //Prepare additional scripts one time
        int ScriptLength = AdditionalScripts.size();
        for(int i = 0;i<ScriptLength;i++)
        {
            AdditionalScripts[i] = Preprocessor->Preprocess(AdditionalScripts[i],0,false);
        }
        //Script = Preprocessor->Preprocess(Script,0);

        ReportData->Start();

        RunSubScript();


    }
    void ScriptMultiWorker::RunSubScript()
    {
        if(engine->isEvaluating())
        {
            OnFinishScript += ";" + Script;
            return;
        }
        while(true)
        {
            if(NeedToSetAsyncResult)
            {
                engine->evaluate("_set_result(MultiScriptWorker.GetAsyncResult())");
                NeedToSetAsyncResult = false;
            }
            //qDebug()<<"engine->evaluate(Script)"<<Script;
            engine->evaluate(Script);
            if(engine->hasUncaughtException())
            {
                ResourceHandlers->Fail();
                ReportData->Final(tr("Script finished with error :") + engine->uncaughtException().toString());
                ReportData->Stop();
                QString Exception = engine->uncaughtException().toString();

                QStringList lines = Script.split(QRegExp("[\r\n]"));
                QString ActionId;
                int i = engine->uncaughtExceptionLineNumber();
                if(i >= lines.length())
                   i = lines.length() - 1;
                if(i<0)
                    i = 0;
                if(!lines.empty())
                {
                    for(;i>=0;i--)
                    {
                       QString line = lines[i];
                       QRegExp r("_sa\\((\\d+)\\)");
                       int f = line.indexOf(r);

                       if(f>=0)
                       {
                           ActionId = r.capturedTexts().at(1);
                           break;
                       }
                    }
                }
                Exception += tr(" during execution of action ") + ActionId;

                Logger->WriteFail(tr("[%1] Script finished with error : ").arg(CurrentTimeString()) + Exception, LogFail );
                emit Finished();
                break;
            }else
            {
                if(!IsAborted && !OnFinishScript.isEmpty())
                {
                    Script = OnFinishScript;
                    OnFinishScript.clear();
                }else if(!IsAborted && Waiter && !Waiter->IsActive())
                {
                    Script = "_next()";
                    continue;

                }else
                {
                    break;
                }
            }
        }
    }

    void ScriptMultiWorker::Decrypt(const QString& Data)
    {
        Script = Preprocessor->Decrypt(Data);
        RunSubScript();
    }


    void ScriptMultiWorker::AbortSubscript()
    {
        engine->abortEvaluation();
    }

    void ScriptMultiWorker::SuccessInternal()
    {

        ReportData->Stop();
        Abort();
    }

    void ScriptMultiWorker::FailInternal(const QString&  message)
    {
        Logger->WriteFail(message, LogFail);
        ReportData->Fail(tr("Ended with message: ") + message);
        ReportData->Final(message);
        ReportData->Stop();
        Abort();
    }

    void ScriptMultiWorker::Abort()
    {
        NoNeedToCreateWorkersMore = true;
        if(engine)
        {
            engine->abortEvaluation();
        }
        if(Waiter)
            Waiter->Abort();

        ReportData->Final(tr("Aborted By User"));
        ReportData->Stop();

        AbortWorkers(true);


        ScriptFinished();

    }

    void ScriptMultiWorker::AbortNotInstant()
    {
        NoNeedToCreateWorkersMore = true;
        ReportData->Final(tr("Aborted By User"));
    }

    void ScriptMultiWorker::InterruptAction()
    {
        emit InterruptActionSignal();
    }

    void ScriptMultiWorker::ScriptFinished()
    {
        if(IsAborted)
            return;
        engine->evaluate(AbortFunction);
        AbortFunction.clear();
        IsAborted = true;
        ResourceHandlers->Success();
        Logger->Write(tr("[%1] Script finished correctly").arg(CurrentTimeString()), LogSuccess);
        emit Finished();
    }


    void ScriptMultiWorker::AbortWorkers(bool NotSignalResourceHandlers)
    {
        foreach(IWorker *w,Workers)
        {
            if(w)
            {
                if(NotSignalResourceHandlers)
                {
                    w->Abort(IsRecord);
                }else
                {
                    w->Abort(true);
                }
            }

        }

        foreach(IWorker *w,Substages.GetAllWorkers())
        {
            if(w)
            {
                if(NotSignalResourceHandlers)
                {
                    w->Abort(IsRecord);
                }else
                {
                    w->Abort(true);
                }
            }

        }


        foreach(IBrowser *b,Browsers)
        {
            b->SetWorker(0);
        }

        foreach(IBrowser *b,Substages.GetAllBrowsers())
        {
            b->SetWorker(0);
        }
    }

    void ScriptMultiWorker::RunSubstage(const QString& FunctionName, qint64 ThreadsNumber, qint64 MaximumSuccess, qint64 MaximumFailure, int StageId)
    {
        if(IsRecord)
        {
            ThreadsNumber = 1;
            MaximumSuccess = 1;
            MaximumFailure = 1;
        }

        int total = Substages.MaxIndex();
        {
            QList<IBrowser*> BrowserLists = BrowserFactory->Create(ThreadsNumber);
            Substages.AddSubstage(StageId,MaximumSuccess,MaximumFailure,ThreadsNumber,total + ThreadsNumber,BrowserLists);
        }
        QList<IWorker *> * WorkersList = Substages.GetWorkers(StageId);
        QList<IBrowser *> * BrowserLists = Substages.GetBrowsers(StageId);

        for(int i = 0;i<ThreadsNumber;i++)
        {
            IWorker *NewWorker = CreateWorker(i,Workers.length() + total + i + 1,FunctionName,StageId,WorkersList,BrowserLists);
            if(NewWorker)
                NewWorker->Run();
        }
        GenerateThreadNumberChanged();
    }

    void ScriptMultiWorker::GenerateThreadNumberChanged()
    {
        emit ThreadNumberChanged(CurrentThreadNumber + Substages.TotalThreads());
    }

    void ScriptMultiWorker::RunStage(qint64 ThreadsNumber, qint64 MaximumSuccess, qint64 MaximumFailure, qint64 MaxRunTime, const QString& WorkerFunction, const QString& callback)
    {
        if(EmptyProject)
        {
            ThreadsNumber = 1;
            MaximumSuccess = 1;
            MaximumFailure = 1;
            MaxRunTime = 0;
        }
        emit StageStatistic(MaximumSuccess, MaximumFailure);
        bool HasLog = false;
        if(IsStudio && !IsRecord)
        {
            QString msg1 = tr("Script started with %1 threads and will finish after %2 success execution or %3 fail execution or when resource will finish.").arg(QString::number(ThreadsNumber)).arg(QString::number(MaximumSuccess)).arg(QString::number(MaximumFailure));
            QString msg2 = tr("You can change these params in record mode.");
            Logger->WriteInfo(msg1, LogInfo);
            Logger->WriteInfo(msg2, LogInfo);
            HasLog = true;
        }
        if(IsStudio && ModuleManager->IsModuleEnabled("FunCaptcha"))
        {
            QString msg1 = tr("FunCaptcha module is enabled, it can reduce browser authenticity.");
            QString msg2 = tr("So it is recommended to <a href='disablefuncaptcha://disablefuncaptcha' style='color:red'>turn it off</a>, unless you are working with funcaptcha.");
            Logger->WriteHtml(QString("<font color='red'>") + msg1 + QString("</font>"), msg1, LogFail);
            Logger->WriteHtml(QString("<font color='red'>") + msg2 + QString("</font>"), msg2, LogFail);
            HasLog = true;
        }
        if(IsStudio && ModuleManager->IsModuleEnabled("ReCaptcha"))
        {
            QString msg1 = tr("ReCaptcha module is enabled, it can reduce browser authenticity.");
            QString msg2 = tr("So it is recommended to <a href='disablerecaptcha://disablerecaptcha' style='color:red'>turn it off</a>, unless you are working with recaptcha.");
            Logger->WriteHtml(QString("<font color='red'>") + msg1 + QString("</font>"), msg1, LogFail);
            Logger->WriteHtml(QString("<font color='red'>") + msg2 + QString("</font>"), msg2, LogFail);
            HasLog = true;
        }
        if(IsStudio && ModuleManager->IsModuleEnabled("HCaptcha"))
        {
            QString msg1 = tr("HCaptcha module is enabled, it can reduce browser authenticity.");
            QString msg2 = tr("So it is recommended to <a href='disablehcaptcha://disablehcaptcha' style='color:red'>turn it off</a>, unless you are working with hcaptcha.");
            Logger->WriteHtml(QString("<font color='red'>") + msg1 + QString("</font>"), msg1, LogFail);
            Logger->WriteHtml(QString("<font color='red'>") + msg2 + QString("</font>"), msg2, LogFail);
            HasLog = true;
        }

        if(HasLog)
        {
            Logger->WriteInfo("", LogInfo);
        }

        Substages.Clear();

        if(IsRecord)
        {
            CurrentThreadNumber = 1;
            ThreadsNumber = 1;
            MaximumSuccess = 1;
            MaximumFailure = 1;
            MaxRunTime = 0;
        }

        if(MaximumSuccess < 0)
            MaximumSuccess = std::numeric_limits<qint64>::max();
        if(MaximumFailure < 0)
            MaximumFailure = std::numeric_limits<qint64>::max();
        ReportData->Final(tr("Success"));
        Script = callback;
        SuccessLeft = MaximumSuccess;
        FailLeft = MaximumFailure;
        WorkerScript = WorkerFunction;
        NoNeedToCreateWorkersMore = false;
        BrowserFactory->ClearAll();
        Browsers = BrowserFactory->Create(ThreadsNumber);
        WorkerRunning = ThreadsNumber;
        CurrentThreadNumber = ThreadsNumber;
        Waiter->WaitForStageFinished(this,SIGNAL(StageFinished()),this,SLOT(RunSubScript()));
        QList<IWorker *>NewWorkers;
        for(int i = 0;i<ThreadsNumber;i++)
        {
            NewWorkers.append(CreateWorker(i,i+1,"", 0, &Workers, &Browsers));
        }
        for(IWorker * NewWorker:NewWorkers)
        {
            if(NewWorker)
                NewWorker->Run();
        }
        if(MaxRunTime>10)
        {
            StageTimeoutTimer = new QTimer(this);
            StageTimeoutTimer->setSingleShot(true);

            connect(StageTimeoutTimer,SIGNAL(timeout()), this, SLOT(StageTimeout()));
            StageTimeoutTimer->start(MaxRunTime);
        }
        GenerateThreadNumberChanged();
    }

    void ScriptMultiWorker::StageTimeout()
    {
        ReportData->Final(tr("Stage Timeout"));
        StageTimeoutTimer->deleteLater();
        StageTimeoutTimer = 0;
        NoNeedToCreateWorkersMore = true;
        AbortWorkers(false);
    }

    void ScriptMultiWorker::WorkerSuspended(IWorker * Worker)
    {
        WorkerFinishedWithArgument(Worker);
    }

    void ScriptMultiWorker::WorkerResumed()
    {
        int index = -1;int i = 0;
        foreach(IWorker *w,Workers)
        {
            if(!w)
                index = i;
            i++;
        }
        if(index >= 0)
        {
            TakeWorkerFromSuspended(index);
        }
    }

    void ScriptMultiWorker::WorkerFinishedWithArgument(IWorker * w)
    {

        if(w->GetBrowser())
        {
            w->GetBrowser()->CloseBrowser();
        }


        ModuleManager->StopThread(w->GetThreadNumber());
        if(ProfilerData)
        {
            ProfilerData->RunAction(w->GetThreadNumber(), 0);
        }


        if(w->SubstageGetParentId() > 0)
        {
            QList<IWorker *> * WorkersList = Substages.GetWorkers(w->SubstageGetParentId());
            if(!WorkersList)
                return;

            QList<IBrowser *> * BrowsersList = Substages.GetBrowsers(w->SubstageGetParentId());
            if(!BrowsersList)
                return;


            int index = WorkersList->indexOf(w);
            if(index < 0)
                return;

            WorkersList->replace(index,0);

            switch(w->GetResultStatus())
            {
                case IWorker::SuccessStatus:
                {
                    if(!Substages.IsDeadInstant(w->SubstageGetParentId()))
                    {
                        emit ThreadSuccess();
                        ReportData->Success(w->GetResultMessageRaw());
                        LogLevel Level = LogSuccess;
                        if(w->GetResultMessageRaw() != "Ok")
                        {
                            Level = LogUser;
                        }
                        Logger->WriteSuccess(w->GetResultMessage(), Level);
                    }
                    Substages.Success(w->SubstageGetParentId());

                }break;
                case IWorker::FailStatus:
                {
                    if(!Substages.IsDeadInstant(w->SubstageGetParentId()))
                    {
                        emit ThreadFail(w->GetResultMessageRaw());
                        ReportData->Fail(w->GetResultMessageRaw());
                        Logger->WriteFail(w->GetResultMessage(), LogFail);
                    }
                    Substages.Failure(w->SubstageGetParentId());
                }break;
                case IWorker::DieStatus:
                {
                    if(!Substages.IsDeadInstant(w->SubstageGetParentId()))
                    {
                        emit ThreadFail(w->GetResultMessageRaw());
                        if(w->GetResultMessageRaw() == "Ok")
                        {
                            ReportData->Success(w->GetResultMessageRaw());
                            Logger->WriteSuccess(w->GetResultMessage(), LogSuccess);
                        }else if(!w->GetResultMessageRaw().contains("-BAS-SILENT-"))
                        {
                            ReportData->Fail(w->GetResultMessageRaw());
                            Logger->Write(w->GetResultMessage(), LogFail);
                        }
                     }
                    Substages.Die(w->SubstageGetParentId(), w->IsDieInstant());
                }break;
            }


            w->GetWaiter()->Abort();
            w->disconnect();
            w->deleteLater();

            if(NoNeedToCreateWorkersMore)
            {
                Substages.EndThread(w->SubstageGetParentId());
            }else if(Substages.CreateThread(w->SubstageGetParentId()))
            {
                IWorker *NewWorker = CreateWorker(index,w->GetThreadNumber(), w->SubstageGetStartingFunction(), w->SubstageGetParentId(), WorkersList, BrowsersList);
                if(NewWorker)
                    NewWorker->Run();
            }

            if(Substages.NeedToFinishStage(w->SubstageGetParentId()))
            {
                QList<IBrowser *> * Browsers = Substages.GetBrowsers(w->SubstageGetParentId());
                if(Browsers)
                    BrowserFactory->Clear(Browsers);
                Substages.RemoveStage(w->SubstageGetParentId());
                emit SubstageFinished(w->SubstageGetParentId());
            }

            if(WorkerRunning + Substages.TotalStages() + Substages.TotalQueued() == 0 && NoNeedToCreateWorkersMore)
            {
                //Logger->Write("Stage Finished");
                if(StageTimeoutTimer)
                {
                    StageTimeoutTimer->deleteLater();
                    StageTimeoutTimer = 0;
                }

                emit StageFinished();
                return;
            }

            //Start queued custom functions if any
            CustomThreadAutostartQueue();

            return;
        }

            int SuspendedCount = ScriptSuspender->Count();
            bool DontCreateNewWaitForSuspended = false;
            bool IsSuspended = false;
            bool LastDieInstant = DieInstant;
            if(w->IsDieInstant())
                DieInstant = true;

        bool DontCreateMore = w->IsDontCreateMore();

        bool IsAllDontCreateMore = true;

        if(Workers.size() < CurrentThreadNumber)
        {
            IsAllDontCreateMore = false;
        }else
        {
            foreach(IWorker *w,Workers)
            {
                if(w && !w->IsDontCreateMore())
                {
                    IsAllDontCreateMore = false;
                    break;
                }
            }
        }


            switch(w->GetResultStatus())
            {
                case IWorker::SuccessStatus:
                {
                    if(w->GetResultMessageRaw() != "none")
                    {
                        LogLevel Level = LogSuccess;
                        if(w->GetResultMessageRaw() != "Ok")
                        {
                            Level = LogUser;
                        }
                        Logger->WriteSuccess(w->GetResultMessage(), Level);

                        if(!LastDieInstant)
                        {
                            emit ThreadSuccess();
                            ReportData->Success(w->GetResultMessageRaw());
                            SuccessNumber ++;
                            emit Success();
                        }

                        SuccessLeft--;
                        if(SuccessLeft<=0)
                        {
                           NoNeedToCreateWorkersMore = true;
                        }else if(SuccessLeft - SuspendedCount <= 0)
                        {
                            DontCreateNewWaitForSuspended = true;
                        }
                    }

                }break;
                case IWorker::FailStatus:
                {
                    Logger->WriteFail(w->GetResultMessage(), LogFail);
                    if(!LastDieInstant)
                    {
                        emit ThreadFail(w->GetResultMessageRaw());
                        ReportData->Fail(w->GetResultMessageRawWithId());
                        FailNumber ++;
                        emit Failed();
                    }
                    FailLeft--;
                    if(FailLeft<=0)
                    {
                        ReportData->Final(tr("Too Much Fails"));
                        NoNeedToCreateWorkersMore = true;
                    }
                }break;
                case IWorker::DieStatus:
                {
                    Logger->Write(w->GetResultMessage(), LogFail);
                    if(!LastDieInstant)
                    {
                        emit ThreadFail(w->GetResultMessageRaw());
                        ReportData->Fail(tr("Ended with message: ") + w->GetResultMessageRaw());
                        ReportData->Final(tr("Ended with message: ") + w->GetResultMessageRaw());
                    }
                    NoNeedToCreateWorkersMore = true;

                }break;
                case IWorker::SuspendStatus:
                {
                    Logger->Write(w->GetResultMessage(), LogInfo);
                    IsSuspended = true;

                    if(SuccessLeft - SuspendedCount <= 0)
                    {
                        DontCreateNewWaitForSuspended = true;
                    }
                }break;

            }
        int index = Workers.indexOf(w);
            if(!IsSuspended)
            {
                w->GetWaiter()->Abort();
                w->disconnect();
                w->deleteLater();
            }

            Workers.replace(index,0);
            if(DontCreateNewWaitForSuspended)
            {

                WorkerResumed();
                return;
            }



            WorkerRunning --;

        bool CleanupNotWorkingCustomThreads = false;
        if((WorkerRunning + Substages.TotalStages() + Substages.TotalQueued() == 0 && NoNeedToCreateWorkersMore) || IsAllDontCreateMore)
        {
            NoNeedToCreateWorkersMore = true;
            //Logger->Write("Stage Finished");
            if(StageTimeoutTimer)
            {
                StageTimeoutTimer->deleteLater();
                StageTimeoutTimer = 0;
            }

            emit StageFinished();
            return;
        }else if((WorkerRunning == 0 && NoNeedToCreateWorkersMore) || IsAllDontCreateMore)
        {
            //All main threads finished - close iddling custom threads
            CleanupNotWorkingCustomThreads = true;

        }
        if(NoNeedToCreateWorkersMore)
        {
            if(DieInstant)
                AbortWorkers(false);
            else
            {
                foreach(IWorker *w,Workers)
                {
                    if(w)
                       w->GetWaiter()->SetSkipWaitHandlerMode();
                }
            }
        }else
        {
            if(!DontCreateMore)
            {
                WorkerRunning ++;
                IWorker* NewWorker = CreateWorker(index,index+1, "", 0, &Workers, &Browsers);
                if(NewWorker)
                    NewWorker->Run();
            }
        }

        if(CleanupNotWorkingCustomThreads)
        {
            //All main threads finished - close iddling custom threads
            for(int ThreadId: Substages.CustomThreadGetAllNotRunning())
            {
                CustomThreadStop(ThreadId);
            }
        }
    }

    void ScriptMultiWorker::WorkerFinished()
    {
        IWorker * w = qobject_cast<IWorker*>(sender());
        WorkerFinishedWithArgument(w);
    }

    bool ScriptMultiWorker::TakeWorkerFromSuspended(int index)
    {
        QPair<IWorker*,IScriptSuspender::WorkerStatus> SuspenderStatus = ScriptSuspender->GetWorker();
        if(SuspenderStatus.second == IScriptSuspender::Ready)
        {
            IWorker *worker = SuspenderStatus.first;
            worker->setParent(this);
            Workers.replace(index,worker);
            worker->SetBrowser(Browsers.at(index));
            worker->RunSubScript();
            return true;
        }
        return false;

    }

    void ScriptMultiWorker::StartThreadWebInterface(int ThreadId)
    {
        //This message is received from web interface
        if(IsRecord)
        {
            return;
        }else
        {
            RunSubstage("_BAS_WEBINTERFACE_WAIT",1,1,1,ThreadId);
        }
    }

    void ScriptMultiWorker::StopThreadWebInterface(int ThreadId)
    {
        //This message is received from web interface
        if(IsRecord)
        {
            return;
        }else
        {
            QList<BrowserAutomationStudioFramework::IWorker *> * List = Substages.GetWorkers(ThreadId);
            if(!List || List->size() != 1)
                return;

            if(List->at(0)->IsTaskRunning())
            {
                QJsonObject Obj;
                Obj["Message"] = "Task stopped manually";
                Obj["Result"] = QJsonValue::Null;
                Obj["Success"] = false;
                QJsonDocument doc(Obj);
                emit RunTaskResult(List->at(0)->RunningTaskId(), QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
            }
            List->at(0)->Die("-BAS-SILENT-",true);

        }
    }

    void ScriptMultiWorker::RunTaskInThreadWebInterface(int ThreadId, int ResultId, QString FunctionName, QString Parameters)
    {
        //This message is received from web interface
        if(IsRecord)
        {
            //Send data to ChromeWorker
            WebInterfaceTaskId = ResultId;
            emit RunTaskInRecordMode(ResultId,FunctionName,Parameters);
        }else
        {
            emit TaskDataReceived(ThreadId,FunctionName,Parameters,ResultId);
        }
    }

    void ScriptMultiWorker::RunTaskResultScenario(int ResultId,QString ResultData)
    {
        if(WebInterfaceTaskId == -2)
        {
            //Send data to web interface
            CustomThreadResult(WebInterfaceThreadId,false,false,"null","Stopped by moving execution point");
        }else
        {
            //Send data to web interface
            emit RunTaskResult(ResultId, ResultData);
        }

        //This message is received from Chrome worker
        WebInterfaceTaskId = -1;
        WebInterfaceThreadId = -1;


    }

    void ScriptMultiWorker::RunTaskResultReturnActionRecord(QString ResultData)
    {
        //This message is received from js

        if(WebInterfaceTaskId == -2)
        {
            //Record mode, return result
            QJsonDocument InputObject;
            QJsonParseError err;
            InputObject = QJsonDocument::fromJson(ResultData.toUtf8(),&err);

            QString Message = InputObject.object()["Message"].toString();
            bool Success = InputObject.object()["Success"].toBool();
            QVariant ResultVariant = InputObject.object()["Result"].toVariant();
            QString ResultString;
            if(ResultVariant.type() == QVariant::Map || ResultVariant.type() == QVariant::List)
            {
                ResultString = QString::fromUtf8(QJsonDocument::fromVariant(ResultVariant).toJson());
            }else
            {
                QStringList List = QStringList(ResultVariant.toString());

                ResultString = QString::fromUtf8(QJsonDocument::fromVariant(List).toJson(QJsonDocument::Compact));
                ResultString.remove(0,1);
                ResultString.remove(ResultString.size() - 1,1);
            }
            CustomThreadResult(WebInterfaceThreadId,Success,false,ResultString,Message);
            return;
        }

        //Receive task result from return action

        //Notify scenario editor about current task is finished
        if(WebInterfaceTaskId != -1)
            emit RunTaskInRecordMode(-1,QString(),QString());

        //Send data to web interface
        emit RunTaskResult(WebInterfaceTaskId, ResultData);

        //Clear current task
        WebInterfaceTaskId = -1;
        WebInterfaceThreadId = -1;
    }


    void ScriptMultiWorker::CustomThreadStart(int ThreadId)
    {
        //Don't create thread if alreadu exists
        if(!Substages.HasStage(ThreadId))
        {
            //Use same code as web interface.
            StartThreadWebInterface(ThreadId);
        }

        Substages.SetCustomThreadInfo(ThreadId, false, false, "null", "Not started");
    }

    void ScriptMultiWorker::CustomThreadRunFunction(int ThreadId, QString FunctionName, QString Parameters, bool StopThreadAfterTaskFinish, bool StartPostponedOnIdle)
    {
        if(Substages.CustomThreadGetIsRunning(ThreadId))
        {
            //Custom thread exists right now and it is running, queue
            Substages.CustomThreadQueueFunction(ThreadId,FunctionName,Parameters,StopThreadAfterTaskFinish,StartPostponedOnIdle,0,false);
            return;
        }

        //Always -2 for custom threads
        int ResultId = -2;

        WebInterfaceThreadId = ThreadId;

        //Set if need to stop thread after task finishes
        Substages.CustomThreadSetStopThreadAfterTaskFinish(ThreadId,StopThreadAfterTaskFinish);
        Substages.CustomThreadSetStartPostponedOnIdle(ThreadId,StartPostponedOnIdle);
        Substages.SetCustomThreadInfo(ThreadId, false, true, "null", "");


        if(IsRecord)
        {
            //Set task id
            WebInterfaceTaskId = ResultId;
        }else
        {
            //Start task
            emit TaskDataReceived(ThreadId,FunctionName,Parameters,ResultId);
        }
    }

    void ScriptMultiWorker::CustomThreadStop(int ThreadId)
    {
        //Stop thread in record mode

        Substages.SetCustomThreadInfo(ThreadId, false, false, "null","Task manually stopped");
        Substages.CustomThreadRemoveQueue(ThreadId);

        if(!IsRecord)
        {
            QList<BrowserAutomationStudioFramework::IWorker *> * List = Substages.GetWorkers(ThreadId);
            if(!List || List->size() != 1)
                return;

            List->at(0)->Die("Ok",true);
        }
    }

    void ScriptMultiWorker::CustomThreadStopList(QString ThreadList)
    {
        //Stop list of threads
        QList<int> Threads;
        for(QString &ThreadString: ThreadList.split(","))
        {
            int Thread = ThreadString.toInt();
            if(Substages.CustomThreadIsQueue(Thread))
            {
                //Stop queue threads first
                CustomThreadStop(Thread);
            }else
            {
                Threads.append(Thread);
            }
        }

        for(int Thread: Threads)
        {
            CustomThreadStop(Thread);
        }
    }


    void ScriptMultiWorker::CustomThreadQueueFunction(int ThreadId, QString FunctionName, QString Parameters, bool StopThreadAfterTaskFinish, bool StartPostponedOnIdle, int MaxThreads)
    {
        //The maximum number of running threads has been reached. Queue function.
        if(!IsRecord)
        {
            Substages.CustomThreadQueueFunction(ThreadId, FunctionName, Parameters, StopThreadAfterTaskFinish, StartPostponedOnIdle, MaxThreads, true);
        }
    }

    void ScriptMultiWorker::CustomThreadAutostartQueue()
    {
        //Check for tasks available to run
        int ThreadId = 0;
        QString FunctionName;
        QString Parameters;
        bool StopThreadAfterTaskFinish;
        bool StartPostponedOnIdle;
        int MaxThreads;

        while(Substages.CustomThreadsNextQueuedFunction(ThreadId, FunctionName, Parameters, StopThreadAfterTaskFinish, StartPostponedOnIdle, MaxThreads, true, 0))
        {
            CustomThreadStart(ThreadId);
            CustomThreadRunFunction(ThreadId, FunctionName, Parameters, StopThreadAfterTaskFinish, StartPostponedOnIdle);
        }
    }


    void ScriptMultiWorker::CustomThreadResult(int ThreadId, bool IsSuccess, bool IsRunning, QString ResultJson, QString ErrorMesage)
    {
        QString ResultJsonPrepared = ResultJson;
        if(ResultJsonPrepared.isEmpty())
            ResultJsonPrepared = "null";

        //Set info about results
        Substages.SetCustomThreadInfo(ThreadId,IsSuccess, IsRunning, ResultJsonPrepared, ErrorMesage);


        if(IsRecord)
        {
            if(WebInterfaceTaskId != -1)
                emit RunTaskInRecordMode(-1,QString(),QString());

            //Clear current task
            WebInterfaceTaskId = -1;
            WebInterfaceThreadId = -1;
        }else
        {
            int ThreadIdOutput = 0;
            QString FunctionNameOutput;
            QString ParametersOutput;
            bool StopThreadAfterTaskFinishOutput;
            bool StartPostponedOnIdle;
            int MaxThreadsOutput;

            if(Substages.CustomThreadsNextQueuedFunction(ThreadIdOutput, FunctionNameOutput, ParametersOutput, StopThreadAfterTaskFinishOutput, StartPostponedOnIdle, MaxThreadsOutput, false, ThreadId))
            {
                CustomThreadRunFunction(ThreadId, FunctionNameOutput, ParametersOutput, StopThreadAfterTaskFinishOutput, StartPostponedOnIdle);
            }else if(Substages.CustomThreadGetStopThreadAfterTaskFinish(ThreadId) || WorkerRunning == 0)
            {
                //Kill thread if StopThreadAfterTaskFinish is set, or if no main threads runnig
                //Stop thread, only single task per thread allowed
                QList<BrowserAutomationStudioFramework::IWorker *> * List = Substages.GetWorkers(ThreadId);
                if(List && List->size() == 1)
                {
                    List->at(0)->Die("Ok",true);
                }
            }

        }
    }


    IWorker * ScriptMultiWorker::CreateWorker(int index, int ThreadIndex, const QString& StartingFunction, int StageId, QList<IWorker *>* WorkersList, QList<IBrowser *>* BrowsersList)
    {
        if(TakeWorkerFromSuspended(index))
            return 0;


        IWorker *worker = WorkerFactory->CreateWorker();

        if(EmptyProject)
        {
            EmptyProject = false;
            worker->SetEmptyProject();
        }

        worker->SubstageSetStartingFunction(StartingFunction);
        worker->SubstageSetParentId(StageId);
        worker->SetModuleManager(ModuleManager);
        worker->SetProfilerData(ProfilerData);
        worker->SetStringBuilder(StringBuilder);
        worker->SetEmbeddedLanguageManager(EmbeddedLanguageManager);
        worker->SetBrowserExtensionManager(_BrowserExtensionManager);
        worker->SetAdditionEngineScripts(&AdditionalScripts);
        worker->SetIsRecord(IsRecord);
        worker->SetSubstageManager(&Substages);
        connect(worker,SIGNAL(WebInterfaceEvalSignal(QString)),this,SIGNAL(WebInterfaceEvalSignal(QString)));
        connect(this,SIGNAL(InterruptActionSignal()),worker,SLOT(InterruptAction()));
        connect(worker,SIGNAL(SuccessedButRescued(QString)),ReportData,SLOT(SuccessAndRescued(QString)));
        connect(worker,SIGNAL(FailedButRescued(QString)),ReportData,SLOT(FailAndRescued(QString)));
        connect(worker,SIGNAL(ProgressMaximum(int)),this,SLOT(ProgressMaximumSlot(int)));
        connect(worker,SIGNAL(ProgressValue(int)),this,SLOT(ProgressValueSlot(int)));
        connect(worker,SIGNAL(SubstageBeginSignal(QString,qint64,qint64,qint64,int)),this,SLOT(RunSubstage(QString,qint64,qint64,qint64,int)));
        connect(worker,SIGNAL(SendTaskResultRecordSignal(QString)),this,SLOT(RunTaskResultReturnActionRecord(QString)));
        connect(worker,SIGNAL(SendTaskResultRunSignal(int,QString)),this,SIGNAL(RunTaskResult(int,QString)));
        connect(worker,SIGNAL(PrepareFunctionSignal(QString)),this,SIGNAL(PrepareFunctionSignal(QString)));

        connect(worker,SIGNAL(CustomThreadStart(int)),this,SLOT(CustomThreadStart(int)));
        connect(worker,SIGNAL(CustomThreadStop(int)),this,SLOT(CustomThreadStop(int)));
        connect(worker,SIGNAL(CustomThreadResult(int,bool,bool,QString,QString)),this,SLOT(CustomThreadResult(int,bool,bool,QString,QString)));
        connect(worker,SIGNAL(CustomThreadRunFunction(int,QString,QString,bool,bool)),this,SLOT(CustomThreadRunFunction(int,QString,QString,bool,bool)));
        connect(worker,SIGNAL(CustomThreadQueueFunction(int,QString,QString,bool,bool,int)),this,SLOT(CustomThreadQueueFunction(int,QString,QString,bool,bool,int)));
        connect(worker,SIGNAL(CustomThreadStopList(QString)),this,SLOT(CustomThreadStopList(QString)));

        connect(this,SIGNAL(SubstageFinished(int)),worker,SLOT(SubstageFinished(int)));
        connect(this,SIGNAL(TaskDataReceived(int,QString,QString,int)),worker,SLOT(TaskDataReceived(int,QString,QString,int)));
        connect(this,SIGNAL(PrepareFunctionResult(QString,QString)),worker,SLOT(PrepareFunctionResult(QString,QString)));
        connect(this,SIGNAL(StopAllFunctions()),worker,SLOT(StopAllFunctions()));
        connect(this,SIGNAL(GetNumberOfRunningFunctions(int*)),worker,SLOT(GetNumberOfRunningFunctions(int*)),Qt::DirectConnection);

        QHash<QString,QObject*>::iterator i = ModulesScriptWorker.begin();
        while (i != ModulesScriptWorker.end())
        {
            worker->AddModule(i.value(),i.key());
            i++;
        }

        worker->SetDatabaseConnector(DatabaseConnector);
        worker->SetPreprocessor(Preprocessor);
        worker->SetThreadNumber(ThreadIndex);
        worker->SetSuccessNumber(&SuccessNumber);
        worker->SetFailNumber(&FailNumber);
        worker->SetProjectPath(GetProjectPath());

        worker->SetHttpClientFactory(HttpClientFactory);
        worker->SetPop3ClientFactory(Pop3ClientFactory);
        worker->SetImapClientFactory(ImapClientFactory);
        worker->SetHtmlParserFactory(HtmlParserFactory);
        worker->SetHelperFactory(HelperFactory);
        worker->SetProperties(Properties);
        worker->SetCsvHelper(CsvHelper);

        worker->setParent(this);
        worker->SetDoTrace(GetDoTrace());

        IWorkerSettings *NewWorkerSettings = WorkerSettings->Clone();
        NewWorkerSettings->SetBrowserVersionSelector(_BrowserVersionSelector);
        NewWorkerSettings->SetPcapPort(PcapPort);
        IBrowser * BrowserAtIndex = BrowsersList->at(index);
        NewWorkerSettings->setParent(BrowserAtIndex);
        BrowserAtIndex->SetWorkerSettings(NewWorkerSettings);

        worker->SetBrowser(BrowserAtIndex);

        worker->SetLogger(Logger);

        worker->SetResults1(Results1);
        worker->SetResults2(Results2);
        worker->SetResults3(Results3);
        worker->SetResults4(Results4);
        worker->SetResults5(Results5);
        worker->SetResults6(Results6);
        worker->SetResults7(Results7);
        worker->SetResults8(Results8);
        worker->SetResults9(Results9);

        worker->SetScript(WorkerScript);

        worker->SetScriptResources(ScriptResources);

        worker->SetScriptSuspender(ScriptSuspender);

        IResourceHandlers* ResourceHandlers = ResourceHandlersFactory->CreateResourceHandlers();
        ResourceHandlers->setParent(worker);
        worker->SetResourceHandlers(ResourceHandlers);

        IWaiter* WaiterLocal = WaiterFactory->CreateWaiter();
        WaiterLocal->setParent(worker);
        if(!IsRecord && ProfilerData && ProfilerData->GetIsProfilerEnabled())
            WaiterLocal->SetReportAboutStartedOrEnded();

        worker->SetWaiter(WaiterLocal);

        worker->SetSolverFactory(FactorySolver);

        worker->SetEngineResources(EngineRes);

        if(index < WorkersList->size())
        {
            WorkersList->replace(index,worker);
        }else
        {
            WorkersList->append(worker);
        }

        connect(worker,SIGNAL(Finished()),this,SLOT(WorkerFinished()));
        ModuleManager->StartThread(worker->GetThreadNumber());

        emit ThreadStarted();

        return worker;
    }

    void ScriptMultiWorker::SetAsyncResult(const QScriptValue & AsyncResult)
    {
        NeedToSetAsyncResult = true;
        this->AsyncResult = AsyncResult;
    }

    QScriptValue ScriptMultiWorker::GetAsyncResult()
    {
        return AsyncResult;
    }

    void ScriptMultiWorker::ReloadResource(const QString& name)
    {
        EngineRes->Reload(name);
    }

    void ScriptMultiWorker::RemoveResource(const QString& name)
    {
        EngineRes->Remove(name);
    }

    void ScriptMultiWorker::AddResource(const QString& name, bool successes, bool fails, int simultaneous, int interval, bool greedy, bool dont_give_up)
    {
        EngineRes->Create(name, successes, fails, simultaneous, interval, greedy, dont_give_up);
    }

    void ScriptMultiWorker::GetHandler(const QString& name, bool die_on_fail, const QString& callback)
    {
        DieOnFailHandler = die_on_fail;
        Script = callback;
        QSet<QString> temp;
        Waiter->WaitForHandler(EngineRes,name,temp,this,SLOT(HandlerWaitFinishedSuccess()),this,SLOT(HandlerWaitFinishedFailed()));
    }

    QScriptValue ScriptMultiWorker::GetAllData(const QString& name)
    {
        QList<QScriptValue> data = EngineRes->GetAllData(name);

        QScriptValue res = engine->newArray(data.length());
        int index = 0;
        foreach(QScriptValue val, data)
        {
            res.setProperty(index,val);

            index ++;
        }

        return res;

    }

    QScriptValue ScriptMultiWorker::GetRandomSubarrayData(const QString& name, int size)
    {
        QList<QScriptValue> data = EngineRes->GetRandomSubarrayData(name, size);

        QScriptValue res = engine->newArray(data.length());
        int index = 0;
        foreach(QScriptValue val, data)
        {
            res.setProperty(index,val);

            index ++;
        }

        return res;
    }

    QScriptValue ScriptMultiWorker::GetAtIndex(const QString& name, int index)
    {
        return EngineRes->GetAtIndex(name,index);
    }

    void ScriptMultiWorker::SetAtIndex(const QString& name, int index, const QString& val)
    {
        EngineRes->SetAtIndex(name,index,val);
    }

    QScriptValue ScriptMultiWorker::GetTotalLength(const QString& name)
    {
        return QScriptValue(EngineRes->GetTotalLength(name));
    }

    void ScriptMultiWorker::HandlerWaitFinishedSuccess()
    {
        IResourceHandler *ihandler = Waiter->GetLastHandler();
        if(!ihandler)
        {
            SetAsyncResult(engine->undefinedValue());
        }else
        {
            ResourceHandlers->AddHandler(ihandler);
            SetAsyncResult(engine->newQObject(ihandler));
        }


        RunSubScript();
    }

    void ScriptMultiWorker::Sleep(int msec, const QString& callback)
    {
        Script = callback;
        Waiter->Sleep(msec,this,SLOT(RunSubScript()));
    }

    void ScriptMultiWorker::HandlerWaitFinishedFailed()
    {
        if(DieOnFailHandler)
        {
            ResourceHandlers->Fail();
            ReportData->Final(tr("All data have been processed"));
            Logger->Write(tr("All data have been processed"), LogFail);
            emit Finished();
        }else
        {
            engine->evaluate("_set_result(null)");
            RunSubScript();
        }
    }

    QString ScriptMultiWorker::CurrentTimeString()
    {
        return QTime::currentTime().toString("hh:mm:ss");;
    }

    QString ScriptMultiWorker::DatabaseAddGroup(const QString& GroupName,const QString& GroupDescription, int TableId)
    {
        DatabaseGroup Group;
        Group.IsNull = false;
        Group.Name = GroupName;
        Group.Description = GroupDescription;
        return DatabaseConnector->InsertGroup(Group,TableId);
    }



}
