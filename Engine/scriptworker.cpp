#include "scriptworker.h"
#include "debugterminate.h"
#include "csvhelperwrapper.h"
#include <limits>
#include <algorithm>
#include <QHostAddress>
#define NOMINMAX
#include <windows.h>
#include "every_cpp.h"
#include <QClipboard>
#include <QApplication>

namespace BrowserAutomationStudioFramework
{


    ScriptWorker::ScriptWorker(QObject *parent) :
        IWorker(parent), Browser(0), Logger(0), Results1(0),Results2(0), Results3(0),Results4(0),Results5(0),Results6(0),Results7(0),Results8(0),Results9(0), Waiter(0),engine(0),ThreadNumber(0),IsAborted(false),ProcessComunicator(0),ProcessComunicatorVirtual(0), HttpClient1(0), HttpClient2(0), Pop3Client(0), ImapClient(0), DoTrace(false), MaxFail(999999), MaxSuccess(100), IsFailExceedRunning(false), IsSuccessExceedRunning(false), FunctionData(0), CurrentWebElement(0), HttpClientIndex(1),DieInstant(false), DontCreateMore(false), SuccessNumber(0), FailNumber(0), HttpClientNextTimeout(-1), SolverNotFailNextTime(false), SubstageId(0), SubstageParentId(0), CurrentAction(-1), ProfilerData(0), EmbeddedLanguageManager(0), EmbeddedActionId(0), GarbrageCollectorWasRun(false), _DNSLookup(0), EmbeddedExecutingApiCode(false), TaskRunning(false), WaitForNextTaskRunning(false), EmbeddedIsFunctionCall(false), _SubstageManager(0), BrowserExtensionManager(0)
    {
        AvoidEndlessLoopForUnhandledExceptions.Init(1000,10);
    }


    void ScriptWorker::SetSuccessNumber(qint64* SuccessNumber)
    {
        this->SuccessNumber = SuccessNumber;
    }
    qint64 ScriptWorker::GetSuccessNumber()
    {
        if(!SuccessNumber)
            return -1;
        return *SuccessNumber;
    }

    void ScriptWorker::SetFailNumber(qint64* FailNumber)
    {
        this->FailNumber = FailNumber;
    }
    qint64 ScriptWorker::GetFailNumber()
    {
        if(!FailNumber)
            return -1;
        return *FailNumber;
    }

    void ScriptWorker::SetProjectPath(const QString& Path)
    {
        this->ProjectPath = Path;
    }
    QString ScriptWorker::GetProjectPath()
    {
        return ProjectPath;
    }


    bool ScriptWorker::IsDieInstant()
    {
        return DieInstant;
    }

    bool ScriptWorker::IsDontCreateMore()
    {
        return DontCreateMore;
    }

    IHttpClient* ScriptWorker::GetActualHttpClient()
    {
        if(HttpClientIndex == 1)
            return HttpClient1;
        else
            return HttpClient2;
    }

    void ScriptWorker::SwitchHttpClient(int index)
    {
        HttpClientIndex = index;
    }

    void ScriptWorker::XpathHttpClient()
    {
        HtmlParser->Parse(GetActualHttpClient()->GetContent());
    }

    ScriptWorker::~ScriptWorker()
    {
        EmbeddedTaskKill();

        for(FunctionRunData* func:FunctionDataList)
            func->Stop();
        FunctionDataList.clear();
    }

    void ScriptWorker::SetProfilerData(IProfilerData *ProfilerData)
    {
        this->ProfilerData = ProfilerData;
    }

    IProfilerData * ScriptWorker::GetProfilerData()
    {
        return ProfilerData;
    }

    void ScriptWorker::SetModuleManager(IModuleManager *ModuleManager)
    {
        this->ModuleManager = ModuleManager;
    }

    IModuleManager* ScriptWorker::GetModuleManager()
    {
        return ModuleManager;
    }

    void ScriptWorker::SetDatabaseConnector(IDatabaseConnector *DatabaseConnector)
    {
        this->DatabaseConnector = DatabaseConnector;
    }
    IDatabaseConnector * ScriptWorker::GetDatabaseConnector()
    {
        return DatabaseConnector;
    }


    void ScriptWorker::SetCsvHelper(ICsvHelper *CsvHelper)
    {
        this->CsvHelper = CsvHelper;
    }

    ICsvHelper * ScriptWorker::GetCsvHelper()
    {
        return CsvHelper;
    }

    void ScriptWorker::SetPreprocessor(IPreprocessor* Preprocessor)
    {
        this->Preprocessor = Preprocessor;
    }

    void ScriptWorker::SetProperties(IProperties* Properties)
    {
        this->Properties = Properties;
    }

    IProperties* ScriptWorker::GetProperties()
    {
        return Properties;
    }

    void ScriptWorker::SetSubstageManager(SubstageManager* _SubstageManager)
    {
        this->_SubstageManager = _SubstageManager;
    }

    SubstageManager* ScriptWorker::GetSubstageManager()
    {
        return _SubstageManager;
    }

    IPreprocessor* ScriptWorker::GetPreprocessor()
    {
        return Preprocessor;
    }

    void ScriptWorker::SetHelperFactory(IHelperFactory* HelperFactory)
    {
        this->HelperFactory = HelperFactory;
    }

    IHelperFactory* ScriptWorker::GetHelperFactory()
    {
        return HelperFactory;
    }

    void ScriptWorker::SetHtmlParserFactory(IHtmlParserFactory* HtmlParserFactory)
    {
        this->HtmlParserFactory = HtmlParserFactory;
    }

    IHtmlParserFactory* ScriptWorker::GetHtmlParserFactory()
    {
        return HtmlParserFactory;
    }

    void ScriptWorker::SetHttpClientFactory(IHttpClientFactory* HttpClientFactory)
    {
        this->HttpClientFactory = HttpClientFactory;
    }

    IHttpClientFactory* ScriptWorker::GetHttpClientFactory()
    {
        return HttpClientFactory;
    }

    void ScriptWorker::SetProcessComunicator(IProcessComunicator *ProcessComunicator)
    {
        this->ProcessComunicator = ProcessComunicator;
        if(this->ProcessComunicator)
            this->ProcessComunicator->setProperty("TaskId",SubstageParentId);
    }

    void ScriptWorker::SetProcessComunicatorVirtual(IProcessComunicator *ProcessComunicatorVirtual)
    {
        this->ProcessComunicatorVirtual = ProcessComunicatorVirtual;
        if(this->ProcessComunicatorVirtual)
            this->ProcessComunicatorVirtual->setProperty("TaskId",SubstageParentId);
    }

    void ScriptWorker::SetStringBuilder(IStringBuilder *StringBuilder)
    {
        this->StringBuilder = StringBuilder;
    }

    void ScriptWorker::SetEmbeddedLanguageManager(IEmbeddedLanguageManager *EmbeddedLanguageManager)
    {
        this->EmbeddedLanguageManager = EmbeddedLanguageManager;
        connect(EmbeddedLanguageManager, SIGNAL(ReceivedApiData(quint64,QString,QString)),this,SLOT(ReceivedApiData(quint64,QString,QString)));
        connect(EmbeddedLanguageManager,SIGNAL(ReceivedResultData(quint64,QString,bool,QString)),this,SLOT(ReceivedResultData(quint64,QString,bool,QString)));
        connect(EmbeddedLanguageManager,SIGNAL(ConsoleLog(QString,quint64)),this,SLOT(EmbeddedLog(QString,quint64)));

    }

    IEmbeddedLanguageManager * ScriptWorker::GetEmbeddedLanguageManager()
    {
        return EmbeddedLanguageManager;
    }

    void ScriptWorker::SetBrowserExtensionManager(IBrowserExtensionManager *BrowserExtensionManager)
    {
        this->BrowserExtensionManager = BrowserExtensionManager;
        connect(BrowserExtensionManager, SIGNAL(RequireRequestDone(QString,QString,bool)),this,SLOT(RequireRequestDone(QString,QString,bool)));

    }

    IBrowserExtensionManager * ScriptWorker::GetBrowserExtensionManager()
    {
        return BrowserExtensionManager;
    }

    IStringBuilder * ScriptWorker::GetStringBuilder()
    {
        return StringBuilder;
    }

    IProcessComunicator * ScriptWorker::GetProcessComunicator()
    {
        return ProcessComunicator;
    }

    IProcessComunicator * ScriptWorker::GetProcessComunicatorVirtual()
    {
        return ProcessComunicatorVirtual;
    }

    IProcessComunicator * ScriptWorker::GetProcessComunicatorActual()
    {
        if(ProcessComunicatorVirtual)
            return ProcessComunicatorVirtual;
        return ProcessComunicator;
    }


    void ScriptWorker::SetAsyncResult(const QScriptValue & AsyncResult)
    {
        NeedToSetAsyncResult = true;
        this->AsyncResult = AsyncResult;
    }

    QScriptValue ScriptWorker::GetAsyncResult()
    {
        return AsyncResult;
    }

    void ScriptWorker::SetAsyncFailMessage(const QString & AsyncFailMessage)
    {
        this->AsyncFailMessage = AsyncFailMessage;
    }

    QString ScriptWorker::GetAndClearAsyncFailMessage()
    {
        QString Res = AsyncFailMessage;
        AsyncFailMessage.clear();
        return Res;
    }

    void ScriptWorker::SetAsyncDieMessage(const QString & AsyncDieMessage)
    {
        this->AsyncDieMessage = AsyncDieMessage;
    }

    QString ScriptWorker::GetAndClearAsyncDieMessage()
    {
        QString Res = AsyncDieMessage;
        AsyncDieMessage.clear();
        return Res;
    }


    void ScriptWorker::SetThreadNumber(qint64 ThreadNumber)
    {
        this->ThreadNumber = ThreadNumber;
    }
    qint64 ScriptWorker::GetThreadNumber()
    {
        return ThreadNumber;
    }

    void ScriptWorker::SetResourceHandlers(IResourceHandlers* ResourceHandlers)
    {
        this->ResourceHandlers = ResourceHandlers;
    }

    IResourceHandlers* ScriptWorker::GetResourceHandlers()
    {
        return ResourceHandlers;
    }

    void ScriptWorker::SetBrowser(IBrowser *Browser)
    {
        this->Browser = Browser;
    }

    IBrowser * ScriptWorker::GetBrowser()
    {
        return Browser;
    }

    void ScriptWorker::SetWaiter(IWaiter *Waiter)
    {
        this->Waiter = Waiter;
        if(Waiter->GetReportAboutStartedOrEnded())
        {
            connect(Waiter,SIGNAL(Started()),this,SLOT(OnWaiterStarted()));
            connect(Waiter,SIGNAL(Ended()),this,SLOT(OnWaiterEnded()));
        }
        connect(Waiter,SIGNAL(NeedToWaitForResource(QString)),this,SLOT(NeedToWaitForResource(QString)));

    }



    IWaiter * ScriptWorker::GetWaiter()
    {
        return Waiter;
    }


    void ScriptWorker::SetScriptResources(IScriptResources* ScriptResources)
    {
        this->ScriptResources = ScriptResources;
    }

    IScriptResources* ScriptWorker::GetScriptResources()
    {
        return ScriptResources;
    }

    void ScriptWorker::SetLogger(ILogger *Logger)
    {
        this->Logger = Logger;
    }

    ILogger * ScriptWorker::GetLogger()
    {
        return Logger;
    }

    void ScriptWorker::SetResults1(ILogger *Results1)
    {
        this->Results1 = Results1;
    }
    ILogger * ScriptWorker::GetResults1()
    {
        return Results1;
    }

    void ScriptWorker::SetResults2(ILogger *Results2)
    {
        this->Results2 = Results2;
    }
    ILogger * ScriptWorker::GetResults2()
    {
        return Results2;
    }

    void ScriptWorker::SetResults3(ILogger *Results3)
    {
        this->Results3 = Results3;
    }
    ILogger * ScriptWorker::GetResults3()
    {
        return Results3;
    }

    void ScriptWorker::SetResults4(ILogger *Results4)
    {
        this->Results4 = Results4;
    }
    ILogger * ScriptWorker::GetResults4()
    {
        return Results4;
    }

    void ScriptWorker::SetResults5(ILogger *Results5)
    {
        this->Results5 = Results5;
    }
    ILogger * ScriptWorker::GetResults5()
    {
        return Results5;
    }

    void ScriptWorker::SetResults6(ILogger *Results6)
    {
        this->Results6 = Results6;
    }
    ILogger * ScriptWorker::GetResults6()
    {
        return Results6;
    }

    void ScriptWorker::SetResults7(ILogger *Results7)
    {
        this->Results7 = Results7;
    }
    ILogger * ScriptWorker::GetResults7()
    {
        return Results7;
    }


    void ScriptWorker::SetResults8(ILogger *Results8)
    {
        this->Results8 = Results8;
    }
    ILogger * ScriptWorker::GetResults8()
    {
        return Results8;
    }

    void ScriptWorker::SetResults9(ILogger *Results9)
    {
        this->Results9 = Results9;
    }
    ILogger * ScriptWorker::GetResults9()
    {
        return Results9;
    }

    void ScriptWorker::SetScript(const QString& Script)
    {
        if(DeleteScriptLater)
        {
            if(!this->Script.isEmpty())
            {
                QChar* chars = const_cast<QChar*>(this->Script.constData());
                SecureZeroMemory(chars, this->Script.length() * sizeof(QChar));
                this->Script.clear();
            }
            DeleteScriptLater = false;
        }
        this->Script = Script;

    }

    QString ScriptWorker::GetScript()
    {
        return Script;
    }

    void ScriptWorker::SetSolverFactory(ISolverFactory* FactorySolver)
    {
        this->FactorySolver = FactorySolver;

    }

    ISolverFactory* ScriptWorker::GetSolverFactory()
    {
        return FactorySolver;
    }

    void ScriptWorker::SetEngineResources(IEngineResources* EngineRes)
    {
        this->EngineRes = EngineRes;
    }

    IEngineResources* ScriptWorker::GetEngineResources()
    {
        return EngineRes;
    }

    void ScriptWorker::SetScriptSuspender(IScriptSuspender* ScriptSuspender)
    {
        this->ScriptSuspender = ScriptSuspender;

    }

    IScriptSuspender* ScriptWorker::GetScriptSuspender()
    {
        return ScriptSuspender;
    }


    void ScriptWorker::ProgressValueSlot(int value)
    {
        emit ProgressValue(value);
    }

    void ScriptWorker::ProgressMaximumSlot(int max)
    {
        emit ProgressMaximum(max);
    }

    void ScriptWorker::SetFailFunction(const QString& FailFunction)
    {
        this->FailFunction = FailFunction;
    }

    void ScriptWorker::SetFailExceedFunction(const QString& FailExceedFunction)
    {
        this->FailExceedFunction = FailExceedFunction;
    }

    void ScriptWorker::SetSuccessExceedFunction(const QString& SuccessExceedFunction)
    {
        this->SuccessExceedFunction = SuccessExceedFunction;
    }

    void ScriptWorker::SetSuccessFunction(const QString& SuccessFunction)
    {
        this->SuccessFunction = SuccessFunction;
    }

    void ScriptWorker::SetAbortFunction(const QString& AbortFunction)
    {
        this->AbortFunction = AbortFunction;
    }

    void ScriptWorker::AddModule(QObject *Module, const QString& Name)
    {
        Modules.insert(Name, Module);
    }

    QString ScriptWorker::PickResources()
    {
        QJsonObject Obj;

        for(QString& Name: EngineRes->GetNames())
        {
            //QString Type = EngineRes->GetInfo(Name,"Type");
            Obj[Name] = EngineRes->GetDescription(Name);
        }

        QJsonDocument doc(Obj);
        return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    }

    void ScriptWorker::SetPop3ClientFactory(IPop3ClientFactory* Pop3ClientFactory)
    {
        this->Pop3ClientFactory = Pop3ClientFactory;
    }
    IPop3ClientFactory* ScriptWorker::GetPop3ClientFactory()
    {
        return Pop3ClientFactory;
    }

    void ScriptWorker::SetImapClientFactory(IImapClientFactory* ImapClientFactory)
    {
        this->ImapClientFactory = ImapClientFactory;
    }

    IImapClientFactory* ScriptWorker::GetImapClientFactory()
    {
        return ImapClientFactory;
    }

    void ScriptWorker::SetDoTrace(bool DoTrace)
    {
        this->DoTrace = DoTrace;
    }

    bool ScriptWorker::GetDoTrace()
    {
        return DoTrace;
    }

    void ScriptWorker::SetIsRecord(bool IsRecord)
    {
        this->IsRecord = IsRecord;
    }

    bool ScriptWorker::GetIsRecord()
    {
        return IsRecord;
    }

    void ScriptWorker::SetEmptyProject()
    {
        EmptyProject = true;
    }

    void ScriptWorker::SetAdditionEngineScripts(QList<QString>* AdditionalScripts)
    {
        this->AdditionalScripts = AdditionalScripts;
    }
    QList<QString>* ScriptWorker::GetAdditionEngineScripts()
    {
        return AdditionalScripts;
    }

    void ScriptWorker::InterruptAction()
    {
        if(IsRecord)
        {
            if(Waiter)
                Waiter->Stop();

            Fail(tr("Aborted By User"),false);
        }
    }

    void ScriptWorker::RequireRequestDone(QString Id, QString Filename, bool IsFailure)
    {
        if(Id == CurrentRequireRequestId)
        {
            CurrentRequireRequestId.clear();
            QString Result;
            QJsonObject Obj;
            Obj["success"] = !IsFailure;
            Obj["data"] = Filename;
            QJsonDocument doc(Obj);
            Result = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
            SetAsyncResult(QScriptValue(Result));
            emit BrowserExtensioRequireFinishedSignal();
        }
    }


    void ScriptWorker::Run()
    {
        ResultMessage = "Ok";
        ResultStatus = IWorker::SuccessStatus;

        engine = new QScriptEngine(this);
        NeedToSetAsyncResult = false;

        HtmlParser = HtmlParserFactory->GetHtmlParser();
        HtmlParser->setParent(this);

        Helper = HelperFactory->GetHelper();
        Helper->setParent(this);

        QScriptValue HelperValue = engine->newQObject(Helper);
        engine->globalObject().setProperty("Helper", HelperValue);

        QScriptValue CsvHelperValue = engine->newQObject(new CsvHelperWrapper(engine,CsvHelper,this));
        engine->globalObject().setProperty("CsvHelper", CsvHelperValue);

        QScriptValue HtmlParserValue = engine->newQObject(HtmlParser);
        engine->globalObject().setProperty("HtmlParser", HtmlParserValue);

        QScriptValue BrowserValue = engine->newQObject(Browser);
        engine->globalObject().setProperty("Browser", BrowserValue);

        QScriptValue ScriptWorkerValue = engine->newQObject(this);
        engine->globalObject().setProperty("ScriptWorker", ScriptWorkerValue);

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

        QScriptValue LoggerValue = engine->newQObject(Logger);
        engine->globalObject().setProperty("Logger", LoggerValue);

        QScriptValue FactorySolverValue = engine->newQObject(FactorySolver);
        engine->globalObject().setProperty("FactorySolver", FactorySolverValue);

        QScriptValue EngineResValue = engine->newQObject(EngineRes);
        engine->globalObject().setProperty("EngineRes", EngineResValue);

        QScriptValue ResourceHandlersValue = engine->newQObject(ResourceHandlers);
        engine->globalObject().setProperty("ResourceHandlers", ResourceHandlersValue);

        QScriptValue PropertiesValue = engine->newQObject(Properties);
        engine->globalObject().setProperty("Properties", PropertiesValue);

        engine->globalObject().setProperty("_K", qApp->property("lang").toString());

        QHash<QString,QObject*>::iterator i = Modules.begin();
        while (i != Modules.end())
        {
            QScriptValue Value = engine->newQObject(i.value());
            engine->globalObject().setProperty(i.key(), Value);
            i++;
        }
        Browser->SetScriptResources(ScriptResources);

        foreach(QString script, ScriptResources->GetEngineScripts(IsRecord))
            engine->evaluate(script);


        int ScriptLength = AdditionalScripts->size();
        for(int i = 0;i<ScriptLength;i++)
        {
            QString Script = AdditionalScripts->at(i);
            engine->evaluate(Script);
        }

        if(EmptyProject)
        {
            SetScript("sleep(2147483647, function(){})");
        }
        RunSubScript();
    }

    void ScriptWorker::AttachNetworkAccessManager()
    {
        if(Browser->GetNetworkAccessManager())
        {
            Browser->GetNetworkAccessManager()->SetWorker(this);
            QScriptValue NetworkAccessManagerValue = engine->newQObject(Browser->GetNetworkAccessManager());
            engine->globalObject().setProperty("NetworkAccessManager", NetworkAccessManagerValue);
        }else
        {
            engine->globalObject().setProperty("NetworkAccessManager", QScriptValue());
        }


        if(Browser->GetNetworkAccessManagerVirtual())
        {
            Browser->GetNetworkAccessManagerVirtual()->SetWorker(this);
            QScriptValue NetworkAccessManagerVirtualValue = engine->newQObject(Browser->GetNetworkAccessManagerVirtual());
            engine->globalObject().setProperty("NetworkAccessManagerVirtual", NetworkAccessManagerVirtualValue);
        }else
        {
            engine->globalObject().setProperty("NetworkAccessManagerVirtual", QScriptValue());
        }
    }

    void ScriptWorker::RunSubScript()
    {
        Browser->SetWorker(this);
        RunSubScriptNoWorkerUpdate();
    }

    void ScriptWorker::RunSubScriptNoWorkerUpdate()
    {
        QDateTime Now = QDateTime::currentDateTime();
        if(!GarbrageCollectorWasRun)
        {
            GarbrageCollectorWasRun = true;
            GarbrageCollectorNextDate = Now.addSecs(30);
        }
        if(Now > GarbrageCollectorNextDate)
        {
            GarbrageCollectorNextDate = Now.addSecs(30);
            engine->collectGarbage();
        }

        if(engine->globalObject().property("Browser").isNull())
        {
            QScriptValue BrowserValue = engine->newQObject(Browser);
            engine->globalObject().setProperty("Browser", BrowserValue);
        }


        ResultStatus = IWorker::SuccessStatus;

        if(HttpClient1)
        {
            HttpClient1->Disconnect();
        }
        if(HttpClient2)
        {
            HttpClient2->Disconnect();
        }
        if(Pop3Client)
        {
            Pop3Client->disconnect();
            Pop3Client->Reset();

        }

        if(engine->isEvaluating())
        {
            OnFinishScript += ";" + Script;
            return;
        }
        while(true)
        {

            if(!AsyncDieMessage.isEmpty())
            {
                engine->evaluate("die(ScriptWorker.GetAndClearAsyncDieMessage());");
            }else if(!AsyncFailMessage.isEmpty())
            {
                engine->evaluate("fail(ScriptWorker.GetAndClearAsyncFailMessage());");
            }else
            {
                if(NeedToSetAsyncResult)
                {
                    engine->evaluate("_set_result(ScriptWorker.GetAsyncResult())");
                    NeedToSetAsyncResult = false;
                }

                engine->evaluate(Script);
            }

            if(ResultStatus == IWorker::SuspendStatus)
            {
                break;
            }
            if(engine->hasUncaughtException())
            {
                QString Message = engine->uncaughtException().toString();

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
                Message += tr(" during execution of action ") + ActionId;
                if(Properties && Properties->Get("_","_s") == "1")
                {
                    //OnApplication start executing, need to stop all
                    if(FailFunction.isEmpty())
                        Die(Message, true);
                    else
                        Fail(Message, false);
                }else
                {
                    if(AvoidEndlessLoopForUnhandledExceptions.Run())
                    {
                        FailMessage = Message;
                        Waiter->Sleep(1,this,SLOT(FailDelayed()));
                    }else
                    {
                        Fail(Message, false);
                    }
                }
                break;

            }else
            {


                if(!IsAborted && !OnFinishScript.isEmpty())
                {
                    SetScript(OnFinishScript);
                    OnFinishScript.clear();
                }else if(!IsAborted && Waiter && !Waiter->IsActive())
                {

                    if(IsFailExceedRunning)
                    {
                        ResultMessage = tr("Fail number exceed");
                        ResultStatus = IWorker::FailStatus;
                        Abort(true);
                        break;
                    }
                    if(IsSuccessExceedRunning)
                    {
                        ResultMessage = tr("Success number exceed");
                        ResultStatus = IWorker::SuccessStatus;
                        Abort(true);
                        break;
                    }
                    SetScript("_next()");
                    continue;

                }else
                {


                    break;
                }
            }
        }
    }

    void ScriptWorker::Decrypt(const QString& Data)
    {
        SetScript(Preprocessor->Decrypt(Data));
        //Next time decrypted data will be deleted
        DeleteScriptLater = true;
        RunSubScript();
    }


    void ScriptWorker::AbortSubscript()
    {
        engine->abortEvaluation();
    }




    void ScriptWorker::Abort(bool SignalResourceHandlers)
    {
        EmbeddedTaskKill();
        ScriptFinished(SignalResourceHandlers);
        if(engine)
            engine->abortEvaluation();
        if(Waiter)
            Waiter->Abort();

    }


    QString ScriptWorker::Preprocess(const QString& script)
    {
        return GetPreprocessor()->Preprocess(script, 0 , false);
    }

    void ScriptWorker::FailUser(const QString& message, bool dont_create_more)
    {
        FailInternal(message, dont_create_more, LogUser);
    }

    void ScriptWorker::Info(const QString& message)
    {
        Logger->WriteInfo(PrepareMessage(message), LogInfo);
    }

    void ScriptWorker::Fail(const QString& message, bool dont_create_more)
    {
        FailInternal(message, dont_create_more, LogFail);
    }

    QString ScriptWorker::GetEmbeddedApiErrorString()
    {
        return EmbeddedApiErrorString;
    }

    QString ScriptWorker::GetEmbeddedVariablesList()
    {
        return EmbeddedVariablesList;
    }

    bool ScriptWorker::GetEmbeddedExecutingApiCode()
    {
        return EmbeddedExecutingApiCode;
    }

    void ScriptWorker::WebInterfaceEval(const QString& Script)
    {
        emit WebInterfaceEvalSignal(Script);
    }


    void ScriptWorker::FailInternal(const QString& message, bool dont_create_more, LogLevel level)
    {
        if(!EmbeddedIsFunctionCall)
        {
            if(EmbeddedExecutingApiCode)
            {
                EmbeddedApiFail(message);
                return;
            }
            EmbeddedTaskKill();
        }
        if(!FailFunction.isEmpty() && MaxFail <= 0 && !FailExceedFunction.isEmpty())
        {
            SetAsyncResult(QScriptValue(message));
            IsFailExceedRunning = true;
            SetScript(FailExceedFunction);
            FailExceedFunction.clear();
            AbortSubscript();
            RunSubScript();
            return;
        }
        if(!FailFunction.isEmpty() && MaxFail > 0)
        {
            MaxFail--;
            emit FailedButRescued(message);
            SetAsyncResult(QScriptValue(message));
            SetScript(FailFunction);
            FailFunction.clear();
            AbortSubscript();
            RunSubScript();
        }else
        {
            if(EmbeddedIsFunctionCall)
            {
                if(EmbeddedExecutingApiCode)
                {
                    EmbeddedApiFail(message);
                    return;
                }
                EmbeddedTaskKill();
            }

            QString Message;
            if(FailFunction.isEmpty())
            {
                Message = message;
            }else
            {
                Message = tr("Fail number exceed");
            }

            bool TaskRunningCopy = TaskRunning;
            SendTaskResultFail(Message);

            if(TaskRunningCopy)
            {
                SetScript("_kill_call_stack();_call(_BAS_WEBINTERFACE_WAIT, null, function(){success(\"\")})");
                AbortSubscript();
                RunSubScriptNoWorkerUpdate();
                return;
            }


            if(IsRecord)
            {
                Logger->WriteFail(PrepareMessage(Message), level);
                SetScript("section_start(\"test\",1,function(){})");
                AbortSubscript();
                RunSubScript();
            }else
            {
                DontCreateMore = dont_create_more;
                ResultMessage = Message;
                ResultStatus = IWorker::FailStatus;
                Abort(true);
            }
        }

    }

    void ScriptWorker::SetMaxFail(int MaxFail)
    {
        this->MaxFail = MaxFail;
    }

    void ScriptWorker::Die(const QString& message, bool instant)
    {
        DieInternal(message, instant, LogUser);
    }

    void ScriptWorker::DieInternal(const QString& message, bool instant, LogLevel level)
    {
        EmbeddedTaskKill();
        if(IsRecord)
        {
            Logger->WriteFail(PrepareMessage(message), level);
            SetScript("section_start(\"test\",1,function(){})");
            AbortSubscript();
            RunSubScript();
        }else
        {
            DieInstant = instant;
            ResultMessage = message;
            ResultStatus = IWorker::DieStatus;
            Abort(true);
        }
    }
    void ScriptWorker::Success(const QString& message)
    {
        EmbeddedTaskKill();

        if(!SuccessFunction.isEmpty() && MaxSuccess <= 0 && !SuccessExceedFunction.isEmpty())
        {
            SetAsyncResult(QScriptValue(message));
            IsSuccessExceedRunning = true;
            SetScript(SuccessExceedFunction);
            SuccessExceedFunction.clear();
            AbortSubscript();
            RunSubScript();
            return;
        }
        if(!SuccessFunction.isEmpty() && MaxSuccess > 0)
        {
            MaxSuccess--;
            emit SuccessedButRescued(message);
            //ResourceHandlers->Success();
            SetAsyncResult(QScriptValue(message));
            SetScript(SuccessFunction);
            SuccessFunction.clear();
            AbortSubscript();
            RunSubScript();
        }else
        {
            bool TaskRunningCopy = TaskRunning;
            SendTaskResultNullSuccess();

            if(TaskRunningCopy)
            {
                SetScript("_kill_call_stack();_call(_BAS_WEBINTERFACE_WAIT, null, function(){success(\"\")})");
                AbortSubscript();
                RunSubScriptNoWorkerUpdate();
                return;
            }

            if(SuccessFunction.isEmpty())
            {
                ResultMessage = message;
            }else
            {
                ResultMessage = tr("Success number exceed");
            }

            if(IsRecord)
            {
                Logger->WriteSuccess(PrepareMessage(ResultMessage));
                SetScript("section_start(\"test\",1,function(){})");
                AbortSubscript();
                RunSubScript();
            }else
            {
                ResultStatus = IWorker::SuccessStatus;
                Abort(true);
            }

        }
    }
    void ScriptWorker::SetMaxSuccess(int MaxSuccess)
    {
        this->MaxSuccess = MaxSuccess;
    }



    void ScriptWorker::ScriptFinished(bool SignalResourceHandlers)
    {
        if(IsAborted)
        {
            return;
        }
        if(IsRecord && ProcessComunicator)
            ProcessComunicator->Send("<ScriptFinished/>");
        if(engine)
            engine->evaluate(AbortFunction);
        AbortFunction.clear();
        IsAborted = true;
        if(SignalResourceHandlers)
        {
            switch(ResultStatus)
            {
                case IWorker::FailStatus: ResourceHandlers->Fail(); break;
                case IWorker::DieStatus:
                {
                    if(SubstageParentId)
                    {
                        //If this thread is created as async function call, or from web interface, don't delete resource
                        ResourceHandlers->Success();
                    }else
                    {
                        ResourceHandlers->Die();
                    }
                }break;

                case IWorker::SuccessStatus: ResourceHandlers->Success(); break;
            }
        }
        if(ProcessComunicator)
            disconnect(ProcessComunicator,SIGNAL(Received(QString)),0,0);
        if(ProcessComunicatorVirtual)
            disconnect(ProcessComunicatorVirtual,SIGNAL(Received(QString)),0,0);

        emit Finished();
    }

    void ScriptWorker::SetFailMessage(const QString& message)
    {
        this->FailMessage = message;
    }

    void ScriptWorker::FailProcessFinished()
    {
        Fail(tr("Failed because process is stopped"), false);
    }

    void ScriptWorker::FailBecauseOfTimeout()
    {
        if(_DNSLookup)
        {
            _DNSLookup->deleteLater();
            _DNSLookup = 0;
        }
        if(HttpClient1)
            HttpClient1->Stop();
        if(HttpClient2)
            HttpClient2->Stop();

        Fail(FailMessage, false);
    }

    void ScriptWorker::FailDelayed()
    {
        Fail(FailMessage, false);
    }

    void ScriptWorker::Sleep(int msec, const QString& callback)
    {
        SetScript(callback);
        Waiter->Sleep(msec,this,SLOT(RunSubScript()));
    }

    void ScriptWorker::WaitNextTask(const QString& callback)
    {
        SetScript(callback);
        Waiter->WaitInfinity(this,SIGNAL(TaskReceivedSignal()),this,SLOT(StartNextTask()));
        if(WaitForNextTaskRunning)
        {
            emit TaskReceivedSignal();
        }
    }

    void ScriptWorker::StartNextTask()
    {
        WaitForNextTaskRunning = false;
        TaskRunning = true;
        RunSubScript();
    }



    QString ScriptWorker::Spintax(const QString& Text)
    {
        return StringBuilder->Expand(Text);
    }

    void ScriptWorker::Suspend(int msec, const QString& callback)
    {
        SetScript(callback);
        IWorker::WorkerStatus State = this->ResultStatus;
        ResultStatus = IWorker::SuspendStatus;
        if(!ScriptSuspender->Suspend(msec, this))
        {
            ResultStatus = State;
            Waiter->Sleep(msec,this,SLOT(RunSubScript()));
        }
    }


    void ScriptWorker::ReloadResource(const QString& name)
    {
        EngineRes->Reload(name);
    }

    void ScriptWorker::ClearResource(const QString& name)
    {
        EngineRes->Clear(name);
    }


    QScriptValue ScriptWorker::GetSolver(const QString& name)
    {
        ISolver *isolver = FactorySolver->GetSolver(name);
        if(!isolver)
        {
            return engine->undefinedValue();
        }
        return engine->newQObject(isolver);
    }




    static QScriptValue prepare(QScriptEngine *engine, IWebElement* web);

    static QScriptValue prototype_nowait(QScriptContext *ctx, QScriptEngine *engine)
    {
        IWebElement *web = qobject_cast<IWebElement*>(ctx->thisObject().toQObject());
        if(!web)
        {
            return engine->undefinedValue();
        }
        if(ctx->argumentCount()!=0)
        {
            return engine->undefinedValue();
        }
        return prepare(engine,web->nowait());
    }

    static QScriptValue prototype_css(QScriptContext *ctx, QScriptEngine *engine)
    {
        IWebElement *web = qobject_cast<IWebElement*>(ctx->thisObject().toQObject());
        if(!web)
        {
            return engine->undefinedValue();
        }
        if(ctx->argumentCount()!=1)
        {
            return engine->undefinedValue();
        }
        return prepare(engine,web->css(ctx->argument(0).toString()));
    }

    static QScriptValue prototype_xpath(QScriptContext *ctx, QScriptEngine *engine)
    {
        IWebElement *web = qobject_cast<IWebElement*>(ctx->thisObject().toQObject());
        if(!web)
        {
            return engine->undefinedValue();
        }
        if(ctx->argumentCount()!=1)
        {
            return engine->undefinedValue();
        }
        return prepare(engine,web->xpath(ctx->argument(0).toString()));
    }

    static QScriptValue prototype_frame(QScriptContext *ctx, QScriptEngine *engine)
    {
        IWebElement *web = qobject_cast<IWebElement*>(ctx->thisObject().toQObject());
        if(!web)
        {
            return engine->undefinedValue();
        }
        if(ctx->argumentCount()!=1)
        {
            return engine->undefinedValue();
        }
        return prepare(engine,web->frame(ctx->argument(0).toString()));
    }

    static QScriptValue prototype_frame_css(QScriptContext *ctx, QScriptEngine *engine)
    {
        IWebElement *web = qobject_cast<IWebElement*>(ctx->thisObject().toQObject());
        if(!web)
        {
            return engine->undefinedValue();
        }
        if(ctx->argumentCount()!=1)
        {
            return engine->undefinedValue();
        }
        return prepare(engine,web->frame_css(ctx->argument(0).toString()));
    }

    static QScriptValue prototype_frame_element(QScriptContext *ctx, QScriptEngine *engine)
    {
        IWebElement *web = qobject_cast<IWebElement*>(ctx->thisObject().toQObject());
        if(!web)
        {
            return engine->undefinedValue();
        }
        if(ctx->argumentCount()!=0)
        {
            return engine->undefinedValue();
        }
        return prepare(engine,web->frame_element());
    }

    static QScriptValue prototype_frame_match(QScriptContext *ctx, QScriptEngine *engine)
    {
        IWebElement *web = qobject_cast<IWebElement*>(ctx->thisObject().toQObject());
        if(!web)
        {
            return engine->undefinedValue();
        }
        if(ctx->argumentCount()!=1)
        {
            return engine->undefinedValue();
        }
        return prepare(engine,web->frame_match(ctx->argument(0).toString()));
    }

    static QScriptValue prototype_position(QScriptContext *ctx, QScriptEngine *engine)
    {
        IWebElement *web = qobject_cast<IWebElement*>(ctx->thisObject().toQObject());
        if(!web)
        {
            return engine->undefinedValue();
        }
        if(ctx->argumentCount()!=2)
        {
            return engine->undefinedValue();
        }
        return prepare(engine,web->position(ctx->argument(0).toInt32(),ctx->argument(1).toInt32()));
    }

    static QScriptValue prototype_at(QScriptContext *ctx, QScriptEngine *engine)
    {
        IWebElement *web = qobject_cast<IWebElement*>(ctx->thisObject().toQObject());
        if(!web)
        {
            return engine->undefinedValue();
        }
        if(ctx->argumentCount()!=1)
        {
            return engine->undefinedValue();
        }
        return prepare(engine,web->at(ctx->argument(0).toInt32()));
    }

    static QScriptValue prototype_match(QScriptContext *ctx, QScriptEngine *engine)
    {
        IWebElement *web = qobject_cast<IWebElement*>(ctx->thisObject().toQObject());
        if(!web)
        {
            return engine->undefinedValue();
        }
        if(ctx->argumentCount()!=1)
        {
            return engine->undefinedValue();
        }
        return prepare(engine,web->match(ctx->argument(0).toString()));
    }

    static QScriptValue prototype_all(QScriptContext *ctx, QScriptEngine *engine)
    {
        IWebElement *web = qobject_cast<IWebElement*>(ctx->thisObject().toQObject());
        if(!web)
        {
            return engine->undefinedValue();
        }
        if(ctx->argumentCount()!=1)
        {
            return engine->undefinedValue();
        }

        return prepare(engine,web->all(ctx->argument(0).toString()));
    }

    static QScriptValue prototype_xpath_all(QScriptContext *ctx, QScriptEngine *engine)
    {
        IWebElement *web = qobject_cast<IWebElement*>(ctx->thisObject().toQObject());
        if(!web)
        {
            return engine->undefinedValue();
        }
        if(ctx->argumentCount()!=1)
        {
            return engine->undefinedValue();
        }

        return prepare(engine,web->xpath_all(ctx->argument(0).toString()));
    }

    static QScriptValue prototype_match_all(QScriptContext *ctx, QScriptEngine *engine)
    {
        IWebElement *web = qobject_cast<IWebElement*>(ctx->thisObject().toQObject());
        if(!web)
        {
            return engine->undefinedValue();
        }
        if(ctx->argumentCount()!=1)
        {
            return engine->undefinedValue();
        }
        return prepare(engine,web->match_all(ctx->argument(0).toString()));
    }

    static QScriptValue prototype_shadow(QScriptContext *ctx, QScriptEngine *engine)
    {
        IWebElement *web = qobject_cast<IWebElement*>(ctx->thisObject().toQObject());
        if(!web)
        {
            return engine->undefinedValue();
        }
        if(ctx->argumentCount()!=0)
        {
            return engine->undefinedValue();
        }
        return prepare(engine,web->shadow());
    }


    static QScriptValue prepare(QScriptEngine *engine, IWebElement* web)
    {
        ScriptWorker* Worker = qobject_cast<ScriptWorker*>(engine->parent());
        web->setParent(Worker);
        if(Worker->CurrentWebElement)
        {
            Worker->CurrentWebElement->deleteLater();
            Worker->CurrentWebElement = 0;
        }

        Worker->CurrentWebElement = web;

        QScriptValue res = engine->newQObject(web);
        res.setProperty("css", engine->newFunction(prototype_css));
        res.setProperty("nowait", engine->newFunction(prototype_nowait));
        res.setProperty("xpath", engine->newFunction(prototype_xpath));
        res.setProperty("frame", engine->newFunction(prototype_frame));
        res.setProperty("frame_css", engine->newFunction(prototype_frame_css));
        res.setProperty("frame_element", engine->newFunction(prototype_frame_element));
        res.setProperty("frame_match", engine->newFunction(prototype_frame_match));
        res.setProperty("position", engine->newFunction(prototype_position));
        res.setProperty("all", engine->newFunction(prototype_all));
        res.setProperty("xpath_all", engine->newFunction(prototype_xpath_all));
        res.setProperty("match", engine->newFunction(prototype_match));
        res.setProperty("match_all", engine->newFunction(prototype_match_all));
        res.setProperty("shadow", engine->newFunction(prototype_shadow));
        res.setProperty("at", engine->newFunction(prototype_at));

        return res;
    }



    QScriptValue ScriptWorker::PrepareWebElement(IWebElement* web)
    {
        web->SetWorker(this);
        return prepare(engine,web);
    }

    QScriptValue ScriptWorker::GetRootElement()
    {
        return PrepareWebElement(Browser->GetRootElement());
    }

    void ScriptWorker::GetHandler(const QString& name, bool die_on_fail, const QString& callback)
    {
        LastHandlerName = name;
        DieOnFailHandler = die_on_fail;
        SetScript(callback);
        Waiter->WaitForHandler(EngineRes,name,RefuseData[name],this,SLOT(HandlerWaitFinishedSuccess()),this,SLOT(HandlerWaitFinishedFailed()));

    }

    void ScriptWorker::Refuse(const QString & name,const QString & value)
    {
        RefuseData[name].insert(value);
    }

    QScriptValue ScriptWorker::GetAllData(const QString& name)
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

    QScriptValue ScriptWorker::GetRandomSubarrayData(const QString& name, int size)
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

    QScriptValue ScriptWorker::GetAtIndex(const QString& name, int index)
    {
        return EngineRes->GetAtIndex(name,index);
    }

    void ScriptWorker::SetAtIndex(const QString& name, int index, const QString& val)
    {
        EngineRes->SetAtIndex(name,index,val);
    }

    QScriptValue ScriptWorker::GetTotalLength(const QString& name)
    {
        return QScriptValue(EngineRes->GetTotalLength(name));
    }

    void ScriptWorker::NeedToWaitForResource(const QString& ResourceName)
    {
        Logger->WriteInfo(PrepareMessage(tr("Waiting for resource %1").arg(ResourceName)), LogInfo);
    }

    void ScriptWorker::HandlerWaitFinishedSuccess()
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

    void ScriptWorker::HandlerWaitFinishedFailed()
    {
        if(Waiter->IsResourceRefused())
        {
            engine->evaluate("IS_REFUSED=true;");
            RunSubScript();
            return;
        }

        if(DieOnFailHandler)
        {

            DieInternal(tr("All data have been processed for ") + LastHandlerName, false, LogFail);
        }else
        {
            engine->evaluate("_set_result(null)");
            RunSubScript();
        }
    }


    IWorker::WorkerStatus ScriptWorker::GetResultStatus()
    {
        return ResultStatus;
    }

    QString ScriptWorker::GetResultMessage()
    {
        QString status;
        if(ResultStatus == IWorker::SuspendStatus)
        {
            return PrepareMessage(tr("Thread suspended"));
        }

        bool ShowActionId = false;

        switch(ResultStatus)
        {
            case IWorker::FailStatus: ShowActionId = true; status = tr("Thread ended"); break;
            case IWorker::DieStatus: status = tr("Thread ended"); break;
            case IWorker::SuccessStatus: status = tr("Thread succeeded"); break;
        }
        QString text = status + tr(" with message \"") + ResultMessage + "\"";
        if(ShowActionId)
        {
            text = PrepareMessage(text);
        }else
        {
            text = PrepareMessageNoId(text);
        }
        return text;
    }

    QString ScriptWorker::GetResultMessageRawWithId()
    {
        QString result;
        if(CurrentAction >= 0)
        {
            result += QString("[") + QString::number(CurrentAction) + QString("] ");
        }
        result += ResultMessage;
        return result;
    }

    QString ScriptWorker::GetResultMessageRaw()
    {
        return ResultMessage;
    }

    void ScriptWorker::SetCurrentAction(qint64 CurrentAction)
    {
        this->CurrentAction = CurrentAction;
        if(ProfilerData && !IsRecord)
        {
            ProfilerData->RunAction(ThreadNumber,CurrentAction);
        }
    }

    qint64 ScriptWorker::GetCurrentAction()
    {
        return CurrentAction;
    }

    void ScriptWorker::OnWaiterStarted()
    {
        if(ProfilerData && !IsRecord)
            ProfilerData->SwitchCurrentActionToAsync(ThreadNumber);
    }

    void ScriptWorker::OnWaiterEnded()
    {
        if(ProfilerData && !IsRecord)
            ProfilerData->SwitchCurrentActionToSync(ThreadNumber);
    }

    QString ScriptWorker::PrepareMessage(const QString &message)
    {
        QString status;
        QString datestring = QTime::currentTime().toString("hh:mm:ss");
        if(CurrentAction >= 0)
        {
            status += QString("[") + QString::number(CurrentAction) + QString("] ");
        }
        status  += "[" + datestring + "]";

        status += QString(" ") + tr("Thread #") + QString::number(ThreadNumber) + " : " + message;
        return status;
    }

    QString ScriptWorker::PrepareMessageNoId(const QString &message)
    {
        QString status;
        QString datestring = QTime::currentTime().toString("hh:mm:ss");

        status  += "[" + datestring + "]";

        status += QString(" ") + tr("Thread #") + QString::number(ThreadNumber) + " : " + message;
        return status;
    }

    void ScriptWorker::SetRecaptchaV3SolverProperties(const QString& method, const QString& key, const QString & serverurl, double score, bool use_proxy)
    {
        RecaptchaV3Method = method;
        RecaptchaV3Key = key;
        if(!serverurl.isEmpty())
        {
            RecaptchaV3ServerUrl = serverurl;

            if(!RecaptchaV3ServerUrl.endsWith("/"))
                RecaptchaV3ServerUrl += QString("/");
        }else if(method == "antigate")
        {
            RecaptchaV3ServerUrl = "http://api.anti-captcha.com/";
        }
        RecaptchaV3Score = score;
        RecaptchaV3UseProxy = use_proxy;
    }

    void ScriptWorker::SolveRecaptchaV3(const QString& id, const QString& action, const QString & sitekey, const QString & url)
    {
        ISolver* solver = GetSolverFactory()->GetSolver(RecaptchaV3Method);
        disconnect(solver,SIGNAL(DoneRecatpchaV3(QString,QString,bool,QString)),this,SLOT(RecaptchaV3Solved(QString,QString,bool,QString)));
        connect(solver,SIGNAL(DoneRecatpchaV3(QString,QString,bool,QString)),this,SLOT(RecaptchaV3Solved(QString,QString,bool,QString)));
        if(!solver)
        {
            Fail(tr("CAPTCHA_FAIL") + " : " + tr("Failed to get solver"), false);
            return;
        }
        QStringList Params;

        bool IsJsonInterface = RecaptchaV3Method == "antigate";

        if(IsJsonInterface)
        {
            Params.append("is_json_interface");
            Params.append("1");

            Params.append("pageAction");
            Params.append(action);

            Params.append("type");
            Params.append("RecaptchaV3TaskProxyless");

            Params.append("websiteKey");
            Params.append(sitekey);

            Params.append("websiteURL");
            Params.append(url);

            Params.append("minScore");
            Params.append(QString::number(RecaptchaV3Score));
        }else
        {

            if(RecaptchaV3UseProxy)
            {
                QString Server = engine->evaluate("_PROXY.server").toString();
                if(!Server.isEmpty())
                {
                    int Port = engine->evaluate("_PROXY.Port").toInt32();
                    bool IsHttp = engine->evaluate("_PROXY.IsHttp").toBool();
                    QString Name = engine->evaluate("_PROXY.name").toString();
                    QString Password = engine->evaluate("_PROXY.password").toString();

                    QString Proxy = Server + QString(":") + QString::number(Port);
                    if(!Name.isEmpty() && !Password.isEmpty())
                    {
                        Proxy = Name + QString(":") + Password + QString("@") + Proxy;
                    }
                    QString ProxyType = (IsHttp) ? "HTTPS" : "SOCKS5";

                    Params.append("proxy");
                    Params.append(Proxy);

                    Params.append("proxytype");
                    Params.append(ProxyType);
                }
            }


            Params.append("action");
            Params.append(action);

            Params.append("method");
            Params.append("userrecaptcha");

            Params.append("googlekey");
            Params.append(sitekey);

            Params.append("pageurl");
            Params.append(url);


            Params.append("version");
            Params.append("v3");

            Params.append("min_score");
            Params.append(QString::number(RecaptchaV3Score));

        }




        Params.append("key");
        Params.append(RecaptchaV3Key);

        Params.append("bas_disable_image_convert");
        Params.append("1");

        Params.append("recatpchav3");
        Params.append("true");

        Params.append("id_override");
        Params.append(id);


        if(!RecaptchaV3ServerUrl.isEmpty())
        {
            Params.append("serverurl");
            Params.append(RecaptchaV3ServerUrl);
        }

        solver->Solve(QString(),Params);
    }

    void ScriptWorker::RecaptchaV3Solved(const QString& val, const QString& id, bool res,const QString& solver_id)
    {
        if(!res)
            return;

        if(!Browser)
            return;

        Browser->SendRecaptchaV3Result(id,val);
    }

    void ScriptWorker::SolveInternal(const QString& method, const QString& base64, const QStringList & params,const QString& callback)
    {
        engine->globalObject().setProperty("LAST_CAPTCHA_ID", "");
        ISolver* solver = GetSolverFactory()->GetSolver(method);
        if(!solver)
        {
            Fail(tr("CAPTCHA_FAIL") + " : " + tr("Failed to get solver"), false);
            return;
        }
        SetScript(callback);

        QString id = solver->Solve(base64,params);
        GetWaiter()->WaitForSolver(solver,id,this,SLOT(SolverSuccess()),this,SLOT(SolverFailed()));
    }

    void ScriptWorker::Solve(const QString& method, const QString& base64, const QStringList & params,const QString& callback)
    {
        SolverNotFailNextTime = false;
        SolveInternal(method, base64,params, callback);
    }
    void ScriptWorker::SolveNoFail(const QString& method, const QString& base64, const QStringList & params,const QString& callback)
    {
        SolverNotFailNextTime = true;
        SolveInternal(method, base64,params, callback);
    }
    void ScriptWorker::SolveCaptcha(const QString& method, const QString& base64, const QStringList & params, const bool fail_on_error, const QString& callback)
    {
        if(!fail_on_error)
        {
            SolveNoFail(method, base64, params, callback);
        }
        else
        {
            Solve(method, base64, params, callback);
        }
    }

    void ScriptWorker::SolverSuccess()
    {
        QString res = GetWaiter()->GetLastSolverResult();
        QString id = GetWaiter()->GetLastSolverId();
        engine->globalObject().setProperty("LAST_CAPTCHA_ID", id);

        if(!SolverNotFailNextTime)
        {
            if(res.startsWith("CAPTCHA_FAIL"))
            {
                Fail(res.replace("CAPTCHA_FAIL",tr("CAPTCHA_FAIL")), false);
                return;
            }
        }
        SetAsyncResult(QScriptValue(res));


        RunSubScript();
    }

    void ScriptWorker::SolverFailed()
    {
        engine->globalObject().setProperty("LAST_CAPTCHA_ID", "");
        Fail(tr("Captcha wait timeout"), false);
    }

    QString ScriptWorker::ExecuteNativeModuleCodeSync(const QString& DllName, const QString& FunctionName, const QString& InputParam)
    {
        std::shared_ptr<FunctionRunData> FunctionDataInternal(ModuleManager->PrepareExecuteFunction(DllName,FunctionName,InputParam,GetThreadNumber()));
        if(FunctionDataInternal->IsError)
        {
            Fail(QString::fromStdString(FunctionDataInternal->ErrorString), false);
            return QString();
        }
        if(FunctionDataInternal->IsAync)
        {
            Fail(tr("Async function is called in sync mode"), false);
            return QString();
        }

        FunctionDataInternal->Execute();

        if(FunctionDataInternal->ExecuteError)
        {
            Fail(tr("Failed to run function ") + DllName + QString(".") + FunctionName, false);
            return QString();
        }

        return QString::fromUtf8(FunctionDataInternal->OutputJson.data(),FunctionDataInternal->OutputJson.size());

    }

    void ScriptWorker::ExecuteNativeModuleCodeAsync(const QString& DllName, const QString& FunctionName, const QString& InputParam, const QString& Callback)
    {
        FunctionData = ModuleManager->PrepareExecuteFunction(DllName,FunctionName,InputParam,GetThreadNumber());
        if(FunctionData->IsError)
        {
            Fail(QString::fromStdString(FunctionData->ErrorString), false);
            delete FunctionData;
            return;
        }
        if(!FunctionData->IsAync)
        {
            Fail(tr("Sync function is called in async mode"), false);
            delete FunctionData;
            return;
        }

        QThread* thread = new QThread;

        FunctionDataList.append(FunctionData);

        SetScript(Callback);
        SetFailMessage(tr("Failed to execute module function ") + DllName + QString(".") + FunctionName);
        if(FunctionData->WaitInfinite)
            Waiter->WaitInfinity(FunctionData,SIGNAL(ReadyResult()),this,SLOT(DllResult()));
        else
            Waiter->WaitForSignal(FunctionData,SIGNAL(ReadyResult()),this,SLOT(DllResult()),this, SLOT(FailBecauseOfTimeout()));

        FunctionData->moveToThread(thread);


        connect(thread, SIGNAL(started()), FunctionData, SLOT(Execute()));
        connect(FunctionData, SIGNAL(Finished()), this, SLOT(RemoveFromFunctionDataList()));
        connect(FunctionData, SIGNAL(Finished()), thread, SLOT(quit()));

        connect(thread, SIGNAL(finished()), FunctionData, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));


        thread->start();
    }

    void ScriptWorker::RemoveFromFunctionDataList()
    {
        if(qobject_cast<FunctionRunData *>(sender()) == FunctionData)
        {
            FunctionData = 0;
        }
        FunctionDataList.removeAll(qobject_cast<FunctionRunData *>(sender()));
    }

    void ScriptWorker::DllResult()
    {
        if(FunctionData == 0)
            return;
        if(FunctionData->ExecuteError)
        {
            Fail(tr("Failed to run function ") + FunctionData->DllName + QString(".") + FunctionData->FunctionName, false);
        }else
        {
            QString Result = QString::fromUtf8(FunctionData->OutputJson.data(),FunctionData->OutputJson.size());
            SetAsyncResult(QScriptValue(Result));
            RunSubScript();
        }
    }

    /* Pop3CLient */
    void ScriptWorker::NewPop3Client()
    {
        if(Pop3Client)
            Pop3Client->deleteLater();
        Pop3Client = Pop3ClientFactory->GetPop3Client();
        Pop3Client->setParent(this);


        QScriptValue Pop3ClientValue = engine->newQObject(Pop3Client);
        engine->globalObject().setProperty("Pop3Client", Pop3ClientValue);
    }

    void ScriptWorker::Pop3ClientLoadMessageNumber(const QString& callback)
    {
        SetScript(callback);
        SetFailMessage(tr("Failed to get mail number with Pop3CLient"));
        Waiter->WaitForSignal(Pop3Client,SIGNAL(Result()),this,SLOT(Pop3ClientResult()),this, SLOT(FailBecauseOfTimeout()));
        Pop3Client->PullNumberOfMessages();
    }

    void ScriptWorker::Pop3ClientLoadMessage(int index, const QString& callback)
    {
        SetScript(callback);
        SetFailMessage(tr("Failed to get message with Pop3CLient"));
        Waiter->WaitForSignal(Pop3Client,SIGNAL(Result()),this,SLOT(Pop3ClientResult()),this, SLOT(FailBecauseOfTimeout()));
        Pop3Client->PullMessage(index);
    }


    void ScriptWorker::Pop3ClientResult()
    {
        Pop3Client->disconnect();
        Pop3Client->Reset();
        if(Pop3Client->GetStatus())
        {
            RunSubScript();
        }else
        {
            SetFailMessage(Pop3Client->GetLastError());
            FailBecauseOfTimeout();
        }

    }

    /* Timeout */

    void ScriptWorker::SetGeneralWaitTimeout(int timeout)
    {
        if(Waiter)
            Waiter->SetGeneralWaitTimeout(timeout);
    }

    void ScriptWorker::SetGeneralWaitTimeoutNext(int timeout)
    {
        if(Waiter)
            Waiter->SetGeneralWaitTimeoutNext(timeout);
    }

    void ScriptWorker::SetSolverWaitTimeout(int timeout)
    {
        if(Waiter)
            Waiter->SetSolverWaitTimeout(timeout);
    }

    void ScriptWorker::SetSolverWaitTimeoutNext(int timeout)
    {
        if(Waiter)
            Waiter->SetSolverWaitTimeoutNext(timeout);
    }

    /* ImapClient */
    void ScriptWorker::NewImapClient()
    {
        if(ImapClient)
            ImapClient->deleteLater();
        ImapClient = ImapClientFactory->GetImapClient();
        ImapClient->setParent(this);


        QScriptValue ImapClientValue = engine->newQObject(ImapClient);
        engine->globalObject().setProperty("ImapClient", ImapClientValue);
    }

    void ScriptWorker::ImapClientPullNumberOfMessages(const QString& callback)
    {
        SetScript(callback);
        SetFailMessage(tr("Failed to get mail number with ImapCLient"));
        Waiter->WaitForSignal(ImapClient,SIGNAL(Result()),this,SLOT(ImapClientResult()),this, SLOT(FailBecauseOfTimeout()));
        ImapClient->PullNumberOfMessages();
    }

    void ScriptWorker::ImapClientSearchMessages(const QString& Sender,const QString& Subject,const QString& Body,const QString& To,const QString& callback)
    {
        SetScript(callback);
        SetFailMessage(tr("Failed to search mail with ImapCLient"));
        Waiter->WaitForSignal(ImapClient,SIGNAL(Result()),this,SLOT(ImapClientResult()),this, SLOT(FailBecauseOfTimeout()));
        ImapClient->SearchMessages(Sender,Subject,Body,To);
    }

    void ScriptWorker::ImapClientCustomSearchMessages(const QString& Query,const QString& callback)
    {
        SetScript(callback);
        SetFailMessage(tr("Failed to custom search mail with ImapCLient"));
        Waiter->WaitForSignal(ImapClient,SIGNAL(Result()),this,SLOT(ImapClientResult()),this, SLOT(FailBecauseOfTimeout()));
        ImapClient->SearchCustomMessages(Query);
    }

    void ScriptWorker::ImapClientPullMessage(const QString& Uid,const QString& callback)
    {
        SetScript(callback);
        SetFailMessage(tr("Failed to fetch mail with ImapCLient"));
        Waiter->WaitForSignal(ImapClient,SIGNAL(Result()),this,SLOT(ImapClientResult()),this, SLOT(FailBecauseOfTimeout()));
        ImapClient->PullMessage(Uid);
    }

    void ScriptWorker::ImapClientCustomQuery(const QString& Url,const QString& Command,const QString& Filter,const QString& callback)
    {
        SetScript(callback);
        SetFailMessage(tr("Failed to execute custom query with ImapCLient"));
        Waiter->WaitForSignal(ImapClient,SIGNAL(Result()),this,SLOT(ImapClientResult()),this, SLOT(FailBecauseOfTimeout()));
        ImapClient->CustomQuery(Url,Command, Filter);
    }

    void ScriptWorker::ImapClientResult()
    {
        ImapClient->disconnect();
        if(ImapClient->GetStatus())
        {
            RunSubScript();
        }else
        {
            SetFailMessage(ImapClient->GetLastError());
            FailBecauseOfTimeout();
        }
    }

    /* HttpClient */
    void ScriptWorker::NewHttpClient()
    {
        if(GetActualHttpClient())
            GetActualHttpClient()->deleteLater();
        if(HttpClientIndex == 1)
            HttpClient1 = HttpClientFactory->GetHttpClient();
        else
            HttpClient2 = HttpClientFactory->GetHttpClient();
        GetActualHttpClient()->setParent(this);

        QScriptValue HttpClientValue = engine->newQObject(GetActualHttpClient());
        engine->globalObject().setProperty("HttpClient" + QString::number(HttpClientIndex), HttpClientValue);
    }

    void ScriptWorker::FollowRedirectInternal(bool IsGet)
    {
        bool CopyLastRedirectIsPost = LastRedirectIsPost;
        QHash<QString,QString> CopyLastRedirectPostData = LastRedirectPostData;
        PostOptions CopyLastRedirectPostSettings = LastRedirectPostSettings;
        GetOptions CopyLastRedirectGetSettings = LastRedirectGetSettings;

        LastRedirectIsPost = false;
        LastRedirectPostData.clear();
        LastRedirectPostSettings.Headers.clear();
        LastRedirectGetSettings.Headers.clear();

        QString Location = GetActualHttpClient()->GetHeader("Location");


        bool IsAbsolute = false;
        {
            QUrl url(Location);
            IsAbsolute = url.isValid() && !url.isEmpty() && !url.isRelative();
        }

        if(IsAbsolute)
        {
            //Use Location without modifications
        }else if(Location.startsWith("//"))
        {
            QUrl url = QUrl(GetActualHttpClient()->GetLastUrl());

            QUrl urllocation = QUrl(Location);
            urllocation.setScheme(url.scheme());
            Location = urllocation.toString();

        }else if(Location.startsWith("/"))
        {
            QUrl url = QUrl(GetActualHttpClient()->GetLastUrl());
            QUrl urllocation = QUrl(Location);
            url.setPath(urllocation.path());
            url.setQuery(urllocation.query());
            url.setFragment(urllocation.fragment());

            Location = url.toString();
        }else if(!Location.isEmpty())
        {
            QUrl url = QUrl(GetActualHttpClient()->GetLastUrl());
            QUrl urllocation = QUrl(Location);

            Location = url.resolved(urllocation).toString();
        }
        GetActualHttpClient()->Disconnect();
        if(!Location.isEmpty())
        {
            SetFailMessage(tr("Failed to get page ") + Location + tr(" with HttpClient"));
            if(IsGet)
            {
                if(HttpClientNextTimeout >= 0)
                    Waiter->SetGeneralWaitTimeoutNext(HttpClientNextTimeout);
                Waiter->WaitForSignal(GetActualHttpClient(),SIGNAL(Finished()),this,SLOT(FollowRedirect()),this,SLOT(FailBecauseOfTimeout()));
                if((GetActualHttpClient()->GetStatus() == 307 || GetActualHttpClient()->GetStatus() == 308) && CopyLastRedirectIsPost)
                {
                    GetActualHttpClient()->Post(Location,CopyLastRedirectPostData, CopyLastRedirectPostSettings);
                }else
                {
                    GetActualHttpClient()->Get(Location, CopyLastRedirectGetSettings);
                }
            }else
            {
                if(HttpClientNextTimeout >= 0)
                    Waiter->SetGeneralWaitTimeoutNext(HttpClientNextTimeout);
                Waiter->WaitForSignal(GetActualHttpClient(),SIGNAL(Finished()),this,SLOT(FollowRedirectDownload()),this,SLOT(FailBecauseOfTimeout()));
                GetActualHttpClient()->Download(Location, CurrentFileDownload);
            }
        }else
        {
            RunSubScript();
        }
    }


    void ScriptWorker::FollowRedirect()
    {
        FollowRedirectInternal(true);
    }

    void ScriptWorker::FollowRedirectDownload()
    {
        FollowRedirectInternal(false);
    }


    void ScriptWorker::DNSLookup(const QString& Domain, const QString& Callback)
    {
        SetScript(Callback);
        SetFailMessage(tr("Failed to resolve domain ") + Domain);

        if(_DNSLookup)
            _DNSLookup->deleteLater();

        _DNSLookup = new QDnsLookup(this);
        Waiter->WaitForSignal(_DNSLookup,SIGNAL(finished()),this,SLOT(DNSLookupResult()),this,SLOT(FailBecauseOfTimeout()));
        _DNSLookup->setType(QDnsLookup::A);
        _DNSLookup->setName(Domain);
        _DNSLookup->lookup();
    }

    void ScriptWorker::DNSLookupResult()
    {
        if(!_DNSLookup)
            return;

        QStringList Result;

        if (_DNSLookup->error() != QDnsLookup::NoError)
        {
            SetAsyncResult(qScriptValueFromSequence(engine,Result));
            _DNSLookup->deleteLater();
            _DNSLookup = 0;
            RunSubScript();
            return;
        }

        for(const QDnsHostAddressRecord &record : _DNSLookup->hostAddressRecords())
        {
            Result.append(record.value().toString());
        }

        SetAsyncResult(qScriptValueFromSequence(engine,Result));
        _DNSLookup->deleteLater();
        _DNSLookup = 0;
        RunSubScript();
    }



    void ScriptWorker::HttpClientPostRedirect(const QString& url, const QStringList & params, const QStringList & params_glob, const QString& callback)
    {
        SetScript(callback);
        SetFailMessage(tr("Failed to post page ") + url + tr(" with HttpClient"));
        HttpClientNextTimeout = Waiter->GetGeneralWaitTimeoutNext();
        Waiter->WaitForSignal(GetActualHttpClient(),SIGNAL(Finished()),this,SLOT(FollowRedirect()),this,SLOT(FailBecauseOfTimeout()));
        QHash<QString,QString> p;
        bool isname = true;
        QString name = "";
        foreach(QString str, params)
        {
            if(isname)
            {
                name = str;
            }else
            {
                p.insert(name,str);
            }
            isname = !isname;
        }

        QHash<QString,QString> p1;
        isname = true;
        name = "";
        foreach(QString str, params_glob)
        {
            if(isname)
            {
                name = str;
            }else
            {
                p1.insert(name,str);
            }
            isname = !isname;
        }
        PostOptions Options;
        if(p1.contains("content-type"))
        {
            Options.PrepareStrategy = p1["content-type"];
        }
        if(p1.contains("encoding"))
        {
            Options.ContentTypeEncoding = p1["encoding"];
        }
        if(p1.contains("method"))
        {
            Options.Method = p1["method"];
        }
        Options.Headers = p1["headers"].split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

        LastRedirectIsPost = true;
        LastRedirectPostData = p;
        LastRedirectPostSettings = Options;
        LastRedirectGetSettings.Method = "GET";
        LastRedirectGetSettings.Headers = Options.Headers;
        GetActualHttpClient()->Post(url,p,Options);
    }

    void ScriptWorker::HttpClientPostNoRedirect(const QString& url, const QStringList & params, const QStringList & params_glob, const QString& callback)
    {
        SetScript(callback);
        SetFailMessage(tr("Failed to post page ") + url + tr(" with HttpClient"));
        Waiter->WaitForSignal(GetActualHttpClient(),SIGNAL(Finished()),this,SLOT(RunSubScript()),this,SLOT(FailBecauseOfTimeout()));
        QHash<QString,QString> p;
        bool isname = true;
        QString name = "";
        foreach(QString str, params)
        {
            if(isname)
            {
                name = str;
            }else
            {
                p.insert(name,str);
            }
            isname = !isname;
        }

        QHash<QString,QString> p1;
        isname = true;
        name = "";
        foreach(QString str, params_glob)
        {
            if(isname)
            {
                name = str;
            }else
            {
                p1.insert(name,str);
            }
            isname = !isname;
        }
        PostOptions Options;
        if(p1.contains("content-type"))
        {
            Options.PrepareStrategy = p1["content-type"];
        }
        if(p1.contains("encoding"))
        {
            Options.ContentTypeEncoding = p1["encoding"];
        }
        if(p1.contains("method"))
        {
            Options.Method = p1["method"];
        }
        Options.Headers = p1["headers"].split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
        GetActualHttpClient()->Post(url,p,Options);
    }

    void ScriptWorker::RequireExtensions(const QString& extensions, const QString& callback)
    {
        SetScript(callback);

        bool IsInstant = false;
        bool IsInstantFailure = false;

        QString IdOrResult = BrowserExtensionManager->Require(extensions,IsInstant,IsInstantFailure);
        if(IsInstant)
        {
            QJsonObject Obj;
            Obj["success"] = !IsInstantFailure;
            Obj["data"] = IdOrResult;
            QJsonDocument doc(Obj);
            QString Result = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
            SetAsyncResult(QScriptValue(Result));
            RunSubScript();
        }else
        {
            CurrentRequireRequestId = IdOrResult;
            Waiter->WaitInfinity(this,SIGNAL(BrowserExtensioRequireFinishedSignal()),this,SLOT(RunSubScript()));
        }
    }

    void ScriptWorker::HttpClientGetNoRedirect(const QString& url, const QString& callback)
    {
        SetScript(callback);
        SetFailMessage(tr("Failed to get page ") + url + tr(" with HttpClient"));
        Waiter->WaitForSignal(GetActualHttpClient(),SIGNAL(Finished()),this,SLOT(RunSubScript()),this,SLOT(FailBecauseOfTimeout()));
        GetActualHttpClient()->Get(url);
    }

    void ScriptWorker::HttpClientGetRedirect(const QString& url, const QString& callback)
    {
        SetScript(callback);
        SetFailMessage(tr("Failed to get page ") + url + tr(" with HttpClient"));
        HttpClientNextTimeout = Waiter->GetGeneralWaitTimeoutNext();
        Waiter->WaitForSignal(GetActualHttpClient(),SIGNAL(Finished()),this,SLOT(FollowRedirect()),this,SLOT(FailBecauseOfTimeout()));
        LastRedirectIsPost = false;
        GetActualHttpClient()->Get(url);
    }

    void ScriptWorker::HttpClientGetNoRedirect2(const QString& url, const QStringList & params_glob, const QString& callback)
    {
        SetScript(callback);
        SetFailMessage(tr("Failed to get page ") + url + tr(" with HttpClient"));
        QHash<QString,QString> p1;
        bool isname = true;
        QString name = "";
        foreach(QString str, params_glob)
        {
            if(isname)
            {
                name = str;
            }else
            {
                p1.insert(name,str);
            }
            isname = !isname;
        }
        GetOptions Options;
        if(p1.contains("method"))
        {
            Options.Method = p1["method"];
            Options.Headers = p1["headers"].split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
        }

        Waiter->WaitForSignal(GetActualHttpClient(),SIGNAL(Finished()),this,SLOT(RunSubScript()),this,SLOT(FailBecauseOfTimeout()));
        GetActualHttpClient()->Get(url,Options);
    }

    void ScriptWorker::HttpClientGetRedirect2(const QString& url, const QStringList & params_glob, const QString& callback)
    {
        SetScript(callback);
        SetFailMessage(tr("Failed to get page ") + url + tr(" with HttpClient"));
        QHash<QString,QString> p1;
        bool isname = true;
        QString name = "";
        foreach(QString str, params_glob)
        {
            if(isname)
            {
                name = str;
            }else
            {
                p1.insert(name,str);
            }
            isname = !isname;
        }
        GetOptions Options;
        if(p1.contains("method"))
        {
            Options.Method = p1["method"];
            Options.Headers = p1["headers"].split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
        }
        HttpClientNextTimeout = Waiter->GetGeneralWaitTimeoutNext();
        Waiter->WaitForSignal(GetActualHttpClient(),SIGNAL(Finished()),this,SLOT(FollowRedirect()),this,SLOT(FailBecauseOfTimeout()));
        LastRedirectIsPost = false;
        LastRedirectGetSettings = Options;
        GetActualHttpClient()->Get(url,Options);
    }

    void ScriptWorker::HttpClientDownload(const QString& url, const QString& file, const QString& callback)
    {
        SetScript(callback);
        SetFailMessage(tr("Failed to download page ") + url + tr(" with HttpClient"));
        HttpClientNextTimeout = Waiter->GetGeneralWaitTimeoutNext();
        Waiter->WaitForSignal(GetActualHttpClient(),SIGNAL(Finished()),this,SLOT(FollowRedirectDownload()),this,SLOT(FailBecauseOfTimeout()));
        CurrentFileDownload = file;
        GetActualHttpClient()->Download(url, file);
    }

    void ScriptWorker::DebugTerminate(int how)
    {
        Terminate().DoTerminate(how);
    }

    QString ScriptWorker::DatabaseAddGroup(const QString& GroupName,const QString& GroupDescription, int TableId)
    {
        DatabaseGroup Group;
        Group.IsNull = false;
        Group.Name = GroupName;
        Group.Description = GroupDescription;
        return DatabaseConnector->InsertGroup(Group,TableId);
    }

    QStringList ScriptWorker::DatabaseSelectGroups(int TableId)
    {
        QList<DatabaseGroup> ResData = DatabaseConnector->GetGroups(TableId);
        QStringList res;

        for(DatabaseGroup &group: ResData)
        {
            if(group.Id == "-1" || group.Id == "-2")
                continue;
            QStringList line;

            line.append(group.Id);
            line.append(group.Name);
            line.append(group.Description);

            QString Line = CsvHelper->Generate(line,':');
            res.append(Line);
        }
        return res;
    }

    void ScriptWorker::DatabaseDeleteGroup(const QString& GroupId, bool DeleteData, int TableId)
    {
        DatabaseGroups Groups;
        Groups.IsNull = false;
        Groups.GroupIdList = QStringList()<<GroupId;
        if(DeleteData)
        {
            DatabaseConnector->DeleteGroupWithData(Groups, TableId);
        }else
        {
            DatabaseConnector->DeleteGroupNoData(Groups, TableId);
        }
    }


    void ScriptWorker::DatabaseDeleteOneRecord(const QString& RecordId, bool DeleteResources, int TableId)
    {
        DatabaseMassSelector Selector;
        Selector.Filters.IsNull = true;
        Selector.Groups.IsNull = true;
        Selector.Sort.IsNull = true;
        Selector.TableId = TableId;
        DatabaseItems Items;
        Items.IsNull = false;
        Items.IsInverseSelection = false;
        Items.SelectedRows.insert(RecordId);
        Selector.Items = Items;


        if(DeleteResources)
        {
            QString ResourceName;
            QString FormattedString;

            DatabaseItem ItemOriginal = DatabaseConnector->Select(RecordId,TableId);
            if(!ItemOriginal.IsNull)
            {

                FormatRecord(ItemOriginal,TableId,ResourceName,FormattedString);

                if(!ResourceName.isEmpty())
                {
                    QList<int> ResIndexes = EngineRes->FindByKeys(ResourceName,QStringList()<<RecordId);

                    for(int index: ResIndexes)
                    {
                        if(index>=0)
                        {
                            EngineRes->RemoveIndex(ResourceName, index);
                        }
                    }
                }
            }
        }

        DatabaseConnector->Delete(Selector);

    }


    void ScriptWorker::DatabaseChangeGroup(const QString& Name, const QString& Description, const QString& GroupId, int TableId)
    {
        DatabaseGroup Group;
        Group.IsNull = false;
        Group.Id = GroupId;
        Group.Name = Name;
        Group.Description = Description;

        if(Name.isEmpty() || Description.isEmpty())
        {

            QList<DatabaseGroup> ResData = DatabaseConnector->GetGroups(TableId);

            for(DatabaseGroup &group: ResData)
            {
                if(GroupId == group.Id)
                {
                    if(Name.isEmpty())
                    {
                        Group.Name = group.Name;
                    }
                    if(Description.isEmpty())
                    {
                        Group.Description = group.Description;
                    }
                    break;
                }

            }

        }


        DatabaseConnector->UpdateGroup(Group, TableId);
    }

    QString ScriptWorker::DatabaseCopyGroup(const QString& GroupId, int TableId)
    {
        DatabaseGroup Group;
        Group.IsNull = false;

        QList<DatabaseGroup> ResData = DatabaseConnector->GetGroups(TableId);

        for(DatabaseGroup &group: ResData)
        {
            if(GroupId == group.Id)
            {
                Group.Name = group.Name;
                Group.Description = group.Description;
                break;
            }

        }

        return DatabaseConnector->CopyGroup(Group, TableId, GroupId);
    }

    QString ScriptWorker::DatabaseMoveRecordToGroup(const QString& GroupId, const QString& RecordId, const QString& MoveType, int TableId)
    {
        DatabaseMassSelector Selector;
        Selector.TableId = TableId;
        Selector.Filters.IsNull = true;
        Selector.Groups.IsNull = true;
        Selector.Sort.IsNull = true;
        DatabaseItems Items;
        Items.IsNull = false;
        Items.IsInverseSelection = false;
        Items.SelectedRows.insert(RecordId);
        Selector.Items = Items;

        if(GroupId.isEmpty())
        {

            DatabaseConnector->DeleteFromAllGroups(Selector);
            return RecordId;
        }else
        {
            if(MoveType == "move")
            {
                DatabaseConnector->DeleteFromAllGroups(Selector);
                DatabaseConnector->MoveToGroup(Selector, GroupId);
                return RecordId;
            }
            if(MoveType == "copy")
            {
                DatabaseItem Item = DatabaseConnector->Select(RecordId,TableId);
                if(!Item.IsNull)
                {
                    DatabaseGroups Groups;
                    Groups.IsNull = false;
                    Groups.GroupIdList.append(GroupId);
                    return DatabaseConnector->Insert(Groups, Item, TableId);
                }
                return RecordId;
            }
            if(MoveType == "reference")
            {
                DatabaseConnector->CopyToGroup(Selector, GroupId);
                return RecordId;
            }
        }
        return RecordId;
    }


    QStringList ScriptWorker::DatabaseSelectRecords(const QString& FilterJson,int PageNumber, int PageSize, int TableId, const QString& SortingType, const QString& GroupId, int SortColumn)
    {
        DatabaseSelector Selector;

        DatabaseSort Sort;
        if(SortingType == "no sorting")
        {
            Sort.IsNull = true;
        }else if(SortingType == "ascending")
        {
            Sort.IsNull = false;
            Sort.OrderIsAsc = true;
            Sort.ColumnId = SortColumn;
        }else if(SortingType == "descending")
        {
            Sort.IsNull = false;
            Sort.OrderIsAsc = false;
            Sort.ColumnId = SortColumn;
        }

        Selector.Sort = Sort;

        Selector.TableId = TableId;
        Selector.Filter = DatabaseConnector->ParseFilter(FilterJson);

        DatabaseGroups DbGroups;
        if(GroupId.isEmpty())
        {
            DbGroups.IsNull = true;
        }else
        {
            DbGroups.IsNull = false;
            DbGroups.GroupIdList.append(GroupId);

        }
        Selector.Groups = DbGroups;

        DatabasePage Page;
        Page.IsNull = false;
        Page.PageNumber = PageNumber;
        Page.PageSize = PageSize;
        Selector.Page = Page;

        QList<DatabaseItem> ResData = DatabaseConnector->Select(Selector);

        QList<DatabaseColumn> Columns = DatabaseConnector->GetColumns(TableId);
        QStringList res;
        for(DatabaseItem &item: ResData)
        {
            QStringList line;

            for(DatabaseColumn &Column: Columns)
            {
                if(Column.Type == DatabaseColumn::Date)
                    line.append(QString::number(item.Data[Column.Id].toDateTime().toMSecsSinceEpoch()));
                else
                    line.append(item.Data[Column.Id].toString());

            }
            line.append(item.Id);

            QString Line = CsvHelper->Generate(line,':');
            res.append(Line);
        }
        return res;
    }

    QString ScriptWorker::DatabaseSelectOneRecord(const QString& RecordId, int TableId)
    {
        DatabaseItem item = DatabaseConnector->Select(RecordId,TableId);
        if(item.IsNull)
            return QString();

        QList<DatabaseColumn> Columns = DatabaseConnector->GetColumns(TableId);
        QStringList line;

        for(DatabaseColumn &Column: Columns)
        {
            if(Column.Type == DatabaseColumn::Date)
                line.append(QString::number(item.Data[Column.Id].toDateTime().toMSecsSinceEpoch()));
            else
                line.append(item.Data[Column.Id].toString());

        }
        line.append(item.Id);

        QString Line = CsvHelper->Generate(line,':');

        return Line;
    }

    int ScriptWorker::DatabaseCountRecords(const QString& FilterJson,int TableId, const QString& GroupId)
    {
        DatabaseSelector Selector;
        Selector.Page.IsNull = true;
        Selector.Sort.IsNull = true;
        Selector.TableId = TableId;
        Selector.Filter = DatabaseConnector->ParseFilter(FilterJson);

        DatabaseGroups DbGroups;
        if(GroupId.isEmpty())
        {
            DbGroups.IsNull = true;
        }else
        {
            DbGroups.IsNull = false;
            DbGroups.GroupIdList.append(GroupId);

        }
        Selector.Groups = DbGroups;

        return DatabaseConnector->Count(Selector);
    }

    void ScriptWorker::DatabaseDeleteRecords(const QString& FilterJson,int TableId, const QString& GroupId, bool DeleteResources)
    {
        DatabaseMassSelector Selector;

        Selector.TableId = TableId;
        Selector.Filters = DatabaseConnector->ParseFilter(FilterJson);
        Selector.Items.IsNull = true;

        DatabaseGroups DbGroups;
        if(GroupId.isEmpty())
        {
            DbGroups.IsNull = true;
        }else
        {
            DbGroups.IsNull = false;
            DbGroups.GroupIdList.append(GroupId);

        }
        Selector.Groups = DbGroups;


        if(DeleteResources)
        {
            QString ResourceName;
            QString FormattedString;

            DatabaseItem item;
            item.IsNull = false;
            item.Id = TableId;

            FormatRecord(item,TableId,ResourceName,FormattedString);
            if(!ResourceName.isEmpty())
            {

                DatabaseSelector DSelector;
                DSelector.Filter = Selector.Filters;
                DSelector.Groups = Selector.Groups;
                DSelector.Page.IsNull = true;
                DSelector.Sort.IsNull = true;
                DSelector.TableId = Selector.TableId;

                QStringList Ids;
                QList<DatabaseItem> SelectResult = DatabaseConnector->Select(DSelector);
                for(DatabaseItem &ItemOriginal: SelectResult)
                {
                    Ids.append(ItemOriginal.Id);
                }
                QList<int> ResIndexes = EngineRes->FindByKeys(ResourceName,Ids);
                QList<int> ResIndexesFinal;


                for(int ResIndex :ResIndexes)
                {
                    if(ResIndex >= 0)
                    {
                        ResIndexesFinal.append(ResIndex);
                    }
                }

                std::sort(ResIndexesFinal.begin(), ResIndexesFinal.end(), std::greater<int>());
                for(int ResIndex :ResIndexesFinal)
                {
                    EngineRes->RemoveIndex(ResourceName, ResIndex);
                }

            }
        }

        DatabaseConnector->Delete(Selector);

    }

    void ScriptWorker::FormatRecord(const DatabaseItem& Record, int TableId, QString& ResourceName, QString& FormattedString)
    {
        ResourceName.clear();
        FormattedString.clear();
        for(QString &CurrentResourceName: EngineRes->GetNames())
        {
            if(EngineRes->GetInfo(CurrentResourceName,"Type") == "Database")
            {
                int ResTableId = EngineRes->GetInfo(CurrentResourceName,"TableId").toInt();
                if(ResTableId == TableId)
                {
                    ResourceName = CurrentResourceName;
                    QStringList ColumnsString = EngineRes->GetInfo(ResourceName,"ColumnIds").split(",");
                    QList<int> Columns;
                    for(const QString& ColumnString: ColumnsString)
                    {
                        Columns.append(ColumnString.toInt());
                    }
                    QStringList line;


                    if(Columns.empty() || Columns.size() == 1 && Columns[0] == 0)
                    {
                        Columns.clear();

                        for(DatabaseColumn &c: DatabaseConnector->GetColumns(TableId))
                        {
                            Columns.append(c.Id);
                        }
                    }

                    QList<bool> IsDateList;

                    for(DatabaseColumn &c: DatabaseConnector->GetColumns(TableId))
                    {
                        IsDateList.push_back(c.Type == DatabaseColumn::Date);
                    }


                    int index = 0;
                    foreach(int ColumnId, Columns)
                    {
                        if(IsDateList[index])
                        {
                            line.append(QString::number(Record.Data[ColumnId].toDateTime().toMSecsSinceEpoch()));
                        }else
                        {
                            line.append(Record.Data[ColumnId].toString());
                        }
                        index++;
                    }
                    line.append(Record.Id);

                    QString Line = CsvHelper->Generate(line,':');
                    FormattedString = Line;



                    break;
                }
            }
        }
    }

    QString ScriptWorker::DatabaseAddRecord(const QString& GroupId,const QStringList& Record, int TableId, bool AddToResources)
    {

        DatabaseItem Item;
        Item.IsNull = false;

        QList<DatabaseColumn> Columns = DatabaseConnector->GetColumns(TableId);

        for(int i = 0;i<Record.length() - 1;i+=2)
        {
            int ColumnId = Record.at(i).toInt();
            QString StringParam = Record.at(i+1);
            QVariant VariantParam;
            foreach(DatabaseColumn Column, Columns)
            {
                if(ColumnId == Column.Id)
                {
                    switch(Column.Type)
                    {
                        case DatabaseColumn::Int: VariantParam = QVariant(StringParam.toInt());break;
                        case DatabaseColumn::String: VariantParam = QVariant(StringParam);break;
                        case DatabaseColumn::Bool: VariantParam = QVariant(StringParam == "true");break;
                        case DatabaseColumn::Date:
                        {
                            QDateTime timestamp;
                            if(StringParam.length() > 0)
                            {
                                timestamp.setTime_t(StringParam.toLongLong() / 1000);
                            }
                            else
                                timestamp = QDateTime::currentDateTime();
                            VariantParam = QVariant(timestamp);
                        }break;
                    }

                    break;
                }
            }
            Item.Data[ColumnId] = VariantParam;
        }

        DatabaseGroups Groups;
        Groups.IsNull = false;
        if(GroupId.isEmpty())
            Groups.GroupIdList.append("-1");
        else
            Groups.GroupIdList.append(GroupId);




        QString InsertedId =  DatabaseConnector->Insert(Groups,Item,TableId);
        if(AddToResources)
        {
            Item.Id = InsertedId;
            QString ResourceName;
            QString FormattedString;
            FormatRecord(Item,TableId,ResourceName,FormattedString);

            if(!ResourceName.isEmpty())
            {
                EngineRes->Insert(ResourceName,FormattedString,false);
            }
        }
        return InsertedId;
    }

    void ScriptWorker::DatabaseUpdateRecordsByCriteria(const QStringList& Record, int TableId, bool UpdateResources, const QString& FilterJson, const QString& GroupId)
    {
        //Generate selector
        DatabaseMassSelector Selector;
        Selector.Items.IsNull = true;
        Selector.TableId = TableId;

        Selector.Filters = DatabaseConnector->ParseFilter(FilterJson);

        DatabaseGroups DbGroups;

        if(GroupId.isEmpty())
        {
            DbGroups.IsNull = true;
        }else
        {
            DbGroups.IsNull = false;
            DbGroups.GroupIdList.append(GroupId);

        }
        Selector.Groups = DbGroups;


        //Generate item
        DatabaseItem item;
        item.IsNull = false;

        QList<DatabaseColumn> Columns = DatabaseConnector->GetColumns(TableId);

        for(int i = 0;i<Record.length() - 1;i+=2)
        {
            int ColumnId = Record.at(i).toInt();
            QString StringParam = Record.at(i+1);
            QVariant VariantParam;
            foreach(DatabaseColumn Column, Columns)
            {
                if(ColumnId == Column.Id)
                {
                    switch(Column.Type)
                    {
                        case DatabaseColumn::Int: VariantParam = QVariant(StringParam.toInt());break;
                        case DatabaseColumn::String: VariantParam = QVariant(StringParam);break;
                        case DatabaseColumn::Bool: VariantParam = QVariant(StringParam == "true");break;
                        case DatabaseColumn::Date:
                        {
                            QDateTime timestamp;
                            if(StringParam.length() > 0)
                            {
                                timestamp.setTime_t(StringParam.toLongLong() / 1000);
                            }
                            else
                                timestamp = QDateTime::currentDateTime();
                            VariantParam = QVariant(timestamp);
                        }break;
                    }

                    break;
                }
            }
            item.Data[ColumnId] = VariantParam;
        }

        //Update database
        DatabaseConnector->Update(item,Selector);

        //Update resources
        if(UpdateResources)
        {
            QString ResourceName;
            QString FormattedString;

            FormatRecord(item,TableId,ResourceName,FormattedString);
            if(!ResourceName.isEmpty())
            {

                DatabaseSelector DSelector;
                DSelector.Filter = Selector.Filters;
                DSelector.Groups = Selector.Groups;
                DSelector.Page.IsNull = true;
                DSelector.Sort.IsNull = true;
                DSelector.TableId = Selector.TableId;

                QStringList Ids;
                QList<DatabaseItem> SelectResult = DatabaseConnector->Select(DSelector);
                for(DatabaseItem &ItemOriginal: SelectResult)
                {
                    Ids.append(ItemOriginal.Id);
                }
                QList<int> ResIndexes = EngineRes->FindByKeys(ResourceName,Ids);


                int index = 0;
                for(DatabaseItem &ItemOriginal: SelectResult)
                {
                    int ResIndex = ResIndexes[index];

                    if(ResIndex < 0)
                    {
                        index++;
                        continue;
                    }

                    QHash<int,QVariant>::iterator i;
                    for (i = item.Data.begin(); i != item.Data.end(); ++i)
                    {
                        ItemOriginal.Data[i.key()] = i.value();
                    }

                    FormatRecord(ItemOriginal,TableId,ResourceName,FormattedString);

                    if(!ResourceName.isEmpty())
                    {

                        EngineRes->SetAtIndex(ResourceName, ResIndex, FormattedString);
                    }

                    index++;
                }
            }
        }

    }


    void ScriptWorker::DatabaseUpdateRecord(const QString& RecordId,const QStringList& Record, int TableId, bool UpdateResources)
    {
        DatabaseItem item;
        item.Id = RecordId;
        item.IsNull = false;

        QList<DatabaseColumn> Columns = DatabaseConnector->GetColumns(TableId);

        for(int i = 0;i<Record.length() - 1;i+=2)
        {
            int ColumnId = Record.at(i).toInt();
            QString StringParam = Record.at(i+1);
            QVariant VariantParam;
            foreach(DatabaseColumn Column, Columns)
            {
                if(ColumnId == Column.Id)
                {
                    switch(Column.Type)
                    {
                        case DatabaseColumn::Int: VariantParam = QVariant(StringParam.toInt());break;
                        case DatabaseColumn::String: VariantParam = QVariant(StringParam);break;
                        case DatabaseColumn::Bool: VariantParam = QVariant(StringParam == "true");break;
                        case DatabaseColumn::Date:
                        {
                            QDateTime timestamp;
                            if(StringParam.length() > 0)
                            {
                                timestamp.setTime_t(StringParam.toLongLong() / 1000);
                            }
                            else
                                timestamp = QDateTime::currentDateTime();
                            VariantParam = QVariant(timestamp);
                        }break;
                    }

                    break;
                }
            }
            item.Data[ColumnId] = VariantParam;
        }

        DatabaseConnector->Update(item,TableId);

        if(UpdateResources)
        {
            QString ResourceName;
            QString FormattedString;

            DatabaseItem ItemOriginal = DatabaseConnector->Select(RecordId,TableId);
            if(!ItemOriginal.IsNull)
            {

                QHash<int,QVariant>::iterator i;
                for (i = item.Data.begin(); i != item.Data.end(); ++i)
                {
                    ItemOriginal.Data[i.key()] = i.value();
                }


                FormatRecord(ItemOriginal,TableId,ResourceName,FormattedString);

                if(!ResourceName.isEmpty())
                {

                    QList<int> ResIndexes = EngineRes->FindByKeys(ResourceName,QStringList()<<RecordId);

                    for(int index: ResIndexes)
                    {
                        if(index>=0)
                        {
                            EngineRes->SetAtIndex(ResourceName, index, FormattedString);
                        }
                    }
                }
            }
        }
    }


    void ScriptWorker::SubstageSetStartingFunction(const QString& StartingFunction)
    {
        this->SubstageStartingFunction = StartingFunction;
    }

    QString ScriptWorker::SubstageGetStartingFunction()
    {
        return SubstageStartingFunction;
    }

    int ScriptWorker::SubstageGetId()
    {
        return SubstageId;
    }

    void ScriptWorker::SubstageSetId(int Id)
    {
        this->SubstageId = Id;
    }

    int ScriptWorker::SubstageGetParentId()
    {
        return SubstageParentId;
    }

    void ScriptWorker::SubstageSetParentId(int Id)
    {
        this->SubstageParentId = Id;
    }

    void ScriptWorker::SubstageFinished(int Id)
    {
        if(Id == SubstageId)
        {
            emit SubstageFinishedSignal();
        }
    }

    void ScriptWorker::TaskDataReceived(int Id, const QString& FunctionName, const QString& FunctionParams, int ResultId)
    {
        if(Id == SubstageParentId)
        {
            if(TaskRunning)
            {
                if(TaskResultId == -2)
                {
                    emit CustomThreadResult(SubstageParentId,false,false,"null","Other task is executing, can't start new task one");
                    return;
                }else
                {
                    //Task is already runnning, send error
                    QJsonObject Obj;
                    Obj["Message"] = "Other task is executing, can't start new task one";
                    Obj["Result"] = QJsonValue::Null;
                    Obj["Success"] = false;
                    QJsonDocument doc(Obj);
                    emit SendTaskResultRunSignal(ResultId, QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
                    return;
                }
            }
            TaskFunctionName = FunctionName;
            TaskFunctionParams = FunctionParams;
            TaskResultId = ResultId;
            WaitForNextTaskRunning = true;
            emit TaskReceivedSignal();
        }
    }

    QString ScriptWorker::GetTaskFunctionName()
    {
        QString Res = TaskFunctionName;
        TaskFunctionName.clear();
        return Res;
    }

    QString ScriptWorker::GetTaskFunctionParams()
    {
        QString Res = TaskFunctionParams;
        TaskFunctionParams.clear();
        return Res;
    }

    bool ScriptWorker::IsTaskRunning()
    {
        return TaskRunning;
    }
    int ScriptWorker::RunningTaskId()
    {
        return TaskResultId;
    }

    void ScriptWorker::SubstageCall(const QString& StartingFunction, qint64 ThreadsNumber, qint64 MaximumSuccess, qint64 MaximumFailure, const QString& Callback)
    {
        SubstageId = qrand() % 1000000 + 1;
        SetScript(Callback);
        if(MaximumSuccess < 0)
            MaximumSuccess = std::numeric_limits<qint64>::max();
        if(MaximumFailure < 0)
            MaximumFailure = std::numeric_limits<qint64>::max();
        Waiter->WaitInfinity(this,SIGNAL(SubstageFinishedSignal()),this,SLOT(SubstageFinishAndRunNext()));
        emit SubstageBeginSignal(StartingFunction, ThreadsNumber, MaximumSuccess, MaximumFailure, SubstageId);
    }

    void ScriptWorker::SubstageFinishAndRunNext()
    {

        if(SubstageId)
        {
            SubstageId = 0;
            RunSubScript();
        }
    }

    void ScriptWorker::SendTaskResultFail(const QString& Message)
    {
        QJsonObject Obj;
        Obj["Message"] = Message;
        Obj["Result"] = QJsonValue::Null;
        Obj["Success"] = false;
        QJsonDocument doc(Obj);
        SendTaskResult(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    }

    void ScriptWorker::SendTaskResultNullSuccess()
    {
        QJsonObject Obj;
        Obj["Message"] = "";
        Obj["Result"] = QJsonValue::Null;
        Obj["Success"] = true;
        QJsonDocument doc(Obj);
        SendTaskResult(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    }

    void ScriptWorker::SendTaskResult(const QString& ResultData)
    {
        if(TaskResultId == -2)
        {
            //If custom thread and run mode, parse json send result
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


            if(TaskRunning)
            {
                TaskRunning = false;
                WaitForNextTaskRunning = false;
                emit CustomThreadResult(SubstageParentId,Success,false,ResultString,Message);
            }
        }else
        {
            if(IsRecord)
            {
                //Notify web interface about return value in record mode
                emit SendTaskResultRecordSignal(ResultData);
            }else
            {
                if(TaskRunning)
                {
                    TaskRunning = false;
                    WaitForNextTaskRunning = false;
                    emit SendTaskResultRunSignal(TaskResultId, ResultData);
                }
            }
        }
    }

    /* Custom threads */

    void ScriptWorker::CustomThreadStartSlot(int ThreadId)
    {
        emit CustomThreadStart(ThreadId);
    }
    void ScriptWorker::CustomThreadRunFunctionSlot(int ThreadId, QString FunctionName, QString Parameters, bool StopThreadAfterTaskFinish, bool StartPostponedOnIdle)
    {
        emit CustomThreadRunFunction(ThreadId, FunctionName, Parameters, StopThreadAfterTaskFinish, StartPostponedOnIdle);
    }

    void ScriptWorker::CustomThreadQueueFunctionSlot(int ThreadId, QString FunctionName, QString Parameters, bool StopThreadAfterTaskFinish, bool StartPostponedOnIdle, int MaxThreads)
    {
        emit CustomThreadQueueFunction(ThreadId, FunctionName, Parameters, StopThreadAfterTaskFinish, StartPostponedOnIdle, MaxThreads);
    }

    void ScriptWorker::CustomThreadStopSlot(int ThreadId)
    {
        emit CustomThreadStop(ThreadId);
    }

    void ScriptWorker::CustomThreadStopListSlot(QString ThreadList)
    {
        emit CustomThreadStopList(ThreadList);
    }


    bool ScriptWorker::CustomThreadGetIsRunning(int ThreadId)
    {
        return GetSubstageManager()->CustomThreadGetIsRunning(ThreadId);
    }

    bool ScriptWorker::CustomThreadGetWasRunningAtLeastOnce(int ThreadId)
    {
        return GetSubstageManager()->CustomThreadGetWasRunningAtLeastOnce(ThreadId);
    }

    bool ScriptWorker::CustomThreadGetIsSuccess(int ThreadId)
    {
        return GetSubstageManager()->CustomThreadGetIsSuccess(ThreadId);
    }

    QString ScriptWorker::CustomThreadGetResult(int ThreadId)
    {
        return GetSubstageManager()->CustomThreadGetResult(ThreadId);
    }

    QString ScriptWorker::CustomThreadGetError(int ThreadId)
    {
        return GetSubstageManager()->CustomThreadGetError(ThreadId);
    }

    int ScriptWorker::CustomThreadsTotal()
    {
        return GetSubstageManager()->CustomThreadsTotal();
    }

    bool ScriptWorker::CustomThreadExists(int ThreadId)
    {
        return GetSubstageManager()->HasStage(ThreadId);
    }

    bool ScriptWorker::CustomThreadCanStartInIddleThread()
    {
        return GetSubstageManager()->CustomThreadCanStartInIddleThread();
    }
    int ScriptWorker::CustomThreadGetIddleThread()
    {
        return GetSubstageManager()->CustomThreadGetIddleThread();
    }


    /* Embedded */
    void ScriptWorker::ReceivedResultData(quint64 Id, const QString& Variables, bool IsSuccess, const QString& ErrorMessage)
    {

        if(Id == -1)
        {
            EmbeddedActionId = 0;
            EmbeddedIsSuccess = IsSuccess;
            EmbeddedIsApiCall = false;
            EmbeddedErrorString = ErrorMessage;
            emit EmbeddedCallFinished();
            return;
        }
        if(Id == EmbeddedActionId)
        {
            QJsonObject Obj;
            Obj["data"] = Variables;
            QJsonDocument doc(Obj);
            QString s = QString("_embedded_parse_variables(%1)").arg(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
            engine->evaluate(s);
            EmbeddedActionId = 0;
            EmbeddedIsSuccess = IsSuccess;
            EmbeddedIsApiCall = false;
            EmbeddedErrorString = ErrorMessage;
            emit EmbeddedCallFinished();

        }
    }

    void ScriptWorker::ReceivedApiData(quint64 Id, const QString& Variables,const QString& ApiString)
    {
        if(Id == EmbeddedActionId)
        {
            QJsonObject Obj;
            Obj["data"] = Variables;
            QJsonDocument doc(Obj);
            QString EmbeddedVariables = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
            EmbeddedIsApiCall = true;
            EmbeddedExecutingApiCode = true;
            EmbeddedIsFunctionCall = ApiString.startsWith("_prepare_function_and_call");
            QString s = QString("_embedded_parse_variables(%1)").arg(EmbeddedVariables);
            engine->evaluate(s);
            s = Preprocess(ApiString + QString("\n;ScriptWorker.EmbeddedApiResponce(_embedded_prepare_variables(\"%1\"))").arg(EmbeddedVariablesList));
            SetScript(s);
            emit EmbeddedCallFinished();

        }
    }

    void ScriptWorker::EmbeddedApiResponce(const QString& Variables)
    {
        EmbeddedExecutingApiCode = false;

        EmbeddedLanguageManager->SendApiResponce(Variables,EmbeddedActionId,EmbeddedLanguageName,EmbeddedLanguageVersion);

        SetGeneralWaitTimeoutNext(EmbeddedTimeout);

        Waiter->WaitForSignal(this,SIGNAL(EmbeddedCallFinished()),this,SLOT(EmbeddedCallFinal()),this, SLOT(EmbeddedCallTimeout()));
    }

    void ScriptWorker::PrepareFunction(const QString& FunctionName)
    {
        this->CurrentPrepareFunction = FunctionName;
        emit PrepareFunctionSignal(FunctionName);
        Waiter->WaitInfinity(this,SIGNAL(PrepareFunctionResultSignal()),this,SLOT(RunSubScript()));

    }

    void ScriptWorker::PrepareFunctionResult(const QString& FunctionName, const QString& FunctionData)
    {
        if(FunctionName == CurrentPrepareFunction)
        {
            CurrentPrepareFunction.clear();
            engine->evaluate(Preprocess(FunctionData));
            SetScript("_prepare_function_and_call_end();");
            emit PrepareFunctionResultSignal();
        }
    }

    void ScriptWorker::StopAllFunctions()
    {
        for(FunctionRunData* func:FunctionDataList)
            func->Stop();
    }

    void ScriptWorker::GetNumberOfRunningFunctions(int * FunctionsTotal)
    {
        *FunctionsTotal += FunctionDataList.size();
    }


    void ScriptWorker::EmbeddedCallFinal()
    {
        if(EmbeddedIsApiCall)
        {
            EmbeddedIsApiCall = false;
            RunSubScript();
        }else if(EmbeddedIsSuccess)
        {
            EmbeddedErrorString.clear();
            SetScript(EmbeddedScriptFinal);
            EmbeddedScriptFinal.clear();
            RunSubScript();
        }else
        {
            EmbeddedScriptFinal.clear();
            Fail(EmbeddedErrorString, false);
            EmbeddedErrorString.clear();
        }
    }

    void ScriptWorker::EmbeddedTaskKill()
    {
        if(EmbeddedLanguageManager && EmbeddedActionId)
        {
            EmbeddedLanguageManager->KillTask(EmbeddedActionId,EmbeddedLanguageName,EmbeddedLanguageVersion);
            EmbeddedActionId = 0;
        }
    }


    void ScriptWorker::EmbeddedApiFail(const QString& Message)
    {
        EmbeddedExecutingApiCode = false;
        EmbeddedApiErrorString = Message;
        SetScript(Preprocess(QString("ScriptWorker.EmbeddedApiResponce(_embedded_prepare_variables(\"%1\", ScriptWorker.GetEmbeddedApiErrorString()))").arg(EmbeddedVariablesList)));
        AbortSubscript();
        RunSubScript();
    }

    void ScriptWorker::EmbeddedLog(const QString& Text,quint64 Id)
    {
        if(Id == EmbeddedActionId)
        {
            Logger->Write(PrepareMessage(Text), LogUser);
        }
    }


    void ScriptWorker::EmbeddedCallTimeout()
    {
        EmbeddedTaskKill();
        Fail(tr("Embedded language execution timeout"), false);
    }

    void ScriptWorker::Embedded(const QString& Variables, const QString& VariablesList, const QString& Function, const QString& LanguageName, const QString& LanguageVersion, int Timeout, const QString& Callback)
    {
        if(EmbeddedActionId)
        {
            Fail(tr("Embedded call already running"), false);
            return;
        }

        QString Error;

        EmbeddedActionId = EmbeddedLanguageManager->SendFunction(Variables,Function,LanguageName,LanguageVersion,Error);

        if(EmbeddedActionId == 0)
        {
            Fail(Error, false);
            return;
        }

        EmbeddedLanguageName = LanguageName;
        EmbeddedLanguageVersion = LanguageVersion;
        EmbeddedVariablesList = VariablesList;

        EmbeddedScriptFinal = Callback;

        EmbeddedTimeout = Timeout;

        SetGeneralWaitTimeoutNext(Timeout);

        Waiter->WaitForSignal(this,SIGNAL(EmbeddedCallFinished()),this,SLOT(EmbeddedCallFinal()),this, SLOT(EmbeddedCallTimeout()));
    }


    void ScriptWorker::SetClipboard(bool IsBase64, const QString& Mime, const QString& DataStr)
    {
        QClipboard *clipboard = QApplication::clipboard();
        QMimeData *mimeData = new QMimeData();

        QByteArray Data;
        if(IsBase64)
        {
            Data = QByteArray::fromBase64(DataStr.toUtf8());
        }else
        {
            Data = DataStr.toUtf8();
        }

        mimeData->setData(Mime,Data);
        clipboard->setMimeData(mimeData);
    }

    QString ScriptWorker::GetClipboard(bool IsBase64, const QString& Mime)
    {
        QClipboard *clipboard = QApplication::clipboard();
        const QMimeData *mimeData = clipboard->mimeData();

        QByteArray ResArray = mimeData->data(Mime);
        if(IsBase64)
            ResArray = ResArray.toBase64();
        
        return QString::fromUtf8(ResArray);
    }

    QString ScriptWorker::PreparePerfectCanvasRequest(const QString& PerfectCanvasRequest)
    {
        if(PerfectCanvasRequest.startsWith("Ids:"))
            return PerfectCanvasRequest;

        if(PerfectCanvasRequest.size() <= 64)
            return PerfectCanvasRequest;

        QString Compressed = PerfectCanvasRequest.mid(64);

        QByteArray ResultData = QByteArray::fromHex(Compressed.toUtf8());
        //Append data size
        QByteArray DataSizeBytes;
        DataSizeBytes.resize(4);
        unsigned int DataSize = ResultData.size();
        qToBigEndian<unsigned int>(DataSize,(unsigned char *)DataSizeBytes.data());

        ResultData.prepend(DataSizeBytes);
        ResultData = qUncompress(ResultData);
        QString ResultString = QString::fromUtf8(ResultData);

        QJsonParseError err;
        QJsonDocument Document = QJsonDocument::fromJson(ResultString.toUtf8(), &err);
        if(err.error || !Document.isArray())
        {
            return PerfectCanvasRequest;
        }

        QStringList ResultList;

        QJsonArray ResultJsonData = Document.array();

        for(QJsonValue Value: ResultJsonData)
        {
            if(Value.isObject())
            {
                QJsonObject ItemObject = Value.toObject();

                if(ItemObject.contains("id"))
                {
                    QJsonValue IdObject = ItemObject["id"];

                    if(IdObject.isString())
                    {
                        ResultList.append(IdObject.toString());
                    }
                }
            }
        }

        return QString("Ids:") + ResultList.join(",");
    }

}
