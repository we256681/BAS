#ifndef WORKER_H
#define WORKER_H
#include "engine_global.h"

#include "iworker.h"

#include "iwaiter.h"
#include "ihttpclient.h"
#include "ihtmlparser.h"
#include "ihelper.h"
#include "ibrowserextensionmanager.h"
#include <QDnsLookup>


#include <QtScript>
#include <QHash>
#include <QSet>
#include "avoidendlessloop.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT ScriptWorker : public IWorker
    {
    Q_OBJECT
        AvoidEndlessLoop AvoidEndlessLoopForUnhandledExceptions;
        QDnsLookup *_DNSLookup;
        IWebElement * CurrentWebElement;
        IPreprocessor* Preprocessor;
        IBrowser *Browser;
        ILogger *Logger;
        ISolverFactory *FactorySolver;
        ILogger *Results1;
        ILogger *Results2;
        ILogger *Results3;
        ILogger *Results4;
        ILogger *Results5;
        ILogger *Results6;
        ILogger *Results7;
        ILogger *Results8;
        ILogger *Results9;
        QDateTime GarbrageCollectorNextDate;
        bool GarbrageCollectorWasRun;
        IScriptResources *ScriptResources;
        IWaiter *Waiter;
        QString Script;
        QScriptEngine* engine;
        IEngineResources * EngineRes;
        IPop3Client * Pop3Client;
        IImapClient * ImapClient;

        IResourceHandlers * ResourceHandlers;
        IWorker::WorkerStatus ResultStatus;
        bool IsAborted;
        QString ResultMessage;
        qint64 ThreadNumber;
        qint64 CurrentAction;
        IProcessComunicator *ProcessComunicator;
        IProcessComunicator *ProcessComunicatorVirtual;
        IHttpClientFactory* HttpClientFactory;
        IHelperFactory* HelperFactory;
        IHelper* Helper;
        ICsvHelper *CsvHelper;
        IHtmlParserFactory* HtmlParserFactory;
        IHtmlParser* HtmlParser;
        IProperties* Properties;
        IModuleManager *ModuleManager;
        IBrowserExtensionManager *BrowserExtensionManager;
        IStringBuilder *StringBuilder;
        IProfilerData *ProfilerData;
        IEmbeddedLanguageManager *EmbeddedLanguageManager;


        IHttpClient* HttpClient1;
        IHttpClient* HttpClient2;
        QScriptValue AsyncResult;
        QString AsyncDieMessage;
        QString AsyncFailMessage;
        bool NeedToSetAsyncResult;
        QString OnFinishScript;
        bool DieOnFailHandler;
        QString LastHandlerName;
        IScriptSuspender *ScriptSuspender;
        QString FailFunction;
        QString SuccessFunction;
        QString FailExceedFunction;
        QString SuccessExceedFunction;
        bool IsFailExceedRunning;
        bool IsSuccessExceedRunning;
        QString AbortFunction;

        QString CurrentRequireRequestId;

        QString FailMessage;
        QHash<QString,QObject*> Modules;
        IPop3ClientFactory* Pop3ClientFactory;
        IImapClientFactory* ImapClientFactory;
        QHash<QString, QSet<QString> >RefuseData;
        bool DoTrace;
        int MaxFail;
        int MaxSuccess;

        QString CurrentFileDownload;
        IDatabaseConnector *DatabaseConnector;
        bool IsRecord;

        QList<QString>* AdditionalScripts;
        FunctionRunData * FunctionData;
        QList<FunctionRunData *> FunctionDataList;

        int HttpClientIndex;
        IHttpClient* GetActualHttpClient();

        bool DieInstant;
        bool DontCreateMore;

        qint64 *SuccessNumber;
        qint64 *FailNumber;
        QString ProjectPath;

        int HttpClientNextTimeout;
        bool SolverNotFailNextTime;

        bool EmptyProject = false;

        bool DeleteScriptLater = false;

        /* Substage */
        QString SubstageStartingFunction;
        int SubstageId;
        int SubstageParentId;
        SubstageManager* _SubstageManager;

        /* Webinterface task */
        QString TaskFunctionName;
        QString TaskFunctionParams;
        int TaskResultId;
        bool TaskRunning;
        bool WaitForNextTaskRunning;

        /* Embedded */
        QString EmbeddedLanguageName;
        QString EmbeddedLanguageVersion;
        quint64 EmbeddedActionId;
        bool EmbeddedIsSuccess;
        bool EmbeddedIsApiCall;
        bool EmbeddedExecutingApiCode;
        QString EmbeddedErrorString;
        QString EmbeddedApiErrorString;
        QString EmbeddedVariablesList;
        QString EmbeddedScriptFinal;
        int EmbeddedTimeout;
        QString CurrentPrepareFunction;
        bool EmbeddedIsFunctionCall;

        /* RecaptchaV3 Settings */
        QString RecaptchaV3Method;
        QString RecaptchaV3Key;
        QString RecaptchaV3ServerUrl;
        double RecaptchaV3Score = 0.6;
        bool RecaptchaV3UseProxy = false;

        /*Http redirect*/
        bool LastRedirectIsPost = false;
        QHash<QString,QString> LastRedirectPostData;
        PostOptions LastRedirectPostSettings;
        GetOptions LastRedirectGetSettings;

    public:
        explicit ScriptWorker(QObject *parent = 0);
        ~ScriptWorker();

        virtual void SetSuccessNumber(qint64* SuccessNumber);

        virtual void SetFailNumber(qint64* FailNumber);

        virtual void SetProjectPath(const QString& Path);

        virtual void SetEmbeddedLanguageManager(IEmbeddedLanguageManager *EmbeddedLanguageManager);
        virtual IEmbeddedLanguageManager * GetEmbeddedLanguageManager();

        virtual void SetBrowserExtensionManager(IBrowserExtensionManager *BrowserExtensionManager);
        virtual IBrowserExtensionManager * GetBrowserExtensionManager();

        virtual void SetStringBuilder(IStringBuilder *StringBuilder);
        virtual IStringBuilder * GetStringBuilder();

        virtual void SetProfilerData(IProfilerData *ProfilerData);
        virtual IProfilerData * GetProfilerData();

        virtual void SetProcessComunicator(IProcessComunicator *ProcessComunicator);
        virtual IProcessComunicator * GetProcessComunicator();

        virtual void SetProcessComunicatorVirtual(IProcessComunicator *ProcessComunicatorVirtual);
        virtual IProcessComunicator * GetProcessComunicatorVirtual();

        virtual IProcessComunicator * GetProcessComunicatorActual();

        virtual void SetDatabaseConnector(IDatabaseConnector *DatabaseConnector);
        virtual IDatabaseConnector * GetDatabaseConnector();

        virtual void SetCsvHelper(ICsvHelper *CsvHelper);
        virtual ICsvHelper * GetCsvHelper();

        virtual void AddModule(QObject *Module, const QString& Name);

        virtual void SetBrowser(IBrowser *Browser);
        virtual IBrowser * GetBrowser();

        virtual void SetLogger(ILogger *Logger);
        virtual ILogger * GetLogger();

        virtual void SetResults1(ILogger *Results1);
        virtual ILogger * GetResults1();

        virtual void SetResults2(ILogger *Results2);
        virtual ILogger * GetResults2();

        virtual void SetResults3(ILogger *Results3);
        virtual ILogger * GetResults3();

        virtual void SetResults4(ILogger *Results4);
        virtual ILogger * GetResults4();

        virtual void SetResults5(ILogger *Results5);
        virtual ILogger * GetResults5();

        virtual void SetResults6(ILogger *Results6);
        virtual ILogger * GetResults6();

        virtual void SetResults7(ILogger *Results7);
        virtual ILogger * GetResults7();

        virtual void SetResults8(ILogger *Results8);
        virtual ILogger * GetResults8();

        virtual void SetResults9(ILogger *Results9);
        virtual ILogger * GetResults9();


        virtual void SetPreprocessor(IPreprocessor* Preprocessor);
        virtual IPreprocessor* GetPreprocessor();

        virtual void SetScriptResources(IScriptResources* ScriptResources);
        virtual IScriptResources* GetScriptResources();

        virtual void SetResourceHandlers(IResourceHandlers* ResourceHandlers);
        virtual IResourceHandlers* GetResourceHandlers();

        virtual void SetWaiter(IWaiter* Waiter);
        virtual IWaiter* GetWaiter();

        virtual void SetSolverFactory(ISolverFactory* FactorySolver);
        virtual ISolverFactory* GetSolverFactory();

        virtual void SetEngineResources(IEngineResources* EngineRes);
        virtual IEngineResources* GetEngineResources();

        virtual void SetThreadNumber(qint64 ThreadNumber);

        virtual void SetHttpClientFactory(IHttpClientFactory* HttpClientFactory);
        virtual IHttpClientFactory* GetHttpClientFactory();

        virtual void SetScriptSuspender(IScriptSuspender* ScriptSuspender);
        virtual IScriptSuspender* GetScriptSuspender();

        virtual void SetPop3ClientFactory(IPop3ClientFactory* Pop3ClientFactory);
        virtual IPop3ClientFactory* GetPop3ClientFactory();

        virtual void SetImapClientFactory(IImapClientFactory* ImapClientFactory);
        virtual IImapClientFactory* GetImapClientFactory();

        virtual void SetHelperFactory(IHelperFactory* HelperFactory);
        virtual IHelperFactory* GetHelperFactory();

        virtual void SetHtmlParserFactory(IHtmlParserFactory* HtmlParserFactory);
        virtual IHtmlParserFactory* GetHtmlParserFactory();

        virtual void SetProperties(IProperties* Properties);
        virtual IProperties* GetProperties();

        virtual void SetSubstageManager(SubstageManager* _SubstageManager);
        virtual SubstageManager* GetSubstageManager();

        virtual void SetDoTrace(bool DoTrace);
        virtual bool GetDoTrace();


        virtual void SetAdditionEngineScripts(QList<QString>* AdditionalScripts);
        virtual QList<QString>* GetAdditionEngineScripts();

        void SetModuleManager(IModuleManager *ModuleManager);
        IModuleManager* GetModuleManager();

        virtual void SetEmptyProject();

    signals:
        void SubstageFinishedSignal();
        void TaskReceivedSignal();
        void EmbeddedCallFinished();
        void PrepareFunctionResultSignal();
        void BrowserExtensioRequireFinishedSignal();
    public slots:
        QString PreparePerfectCanvasRequest(const QString& PerfectCanvasRequest);
        QString PickResources();
        QString GetEmbeddedApiErrorString();
        QString GetEmbeddedVariablesList();
        bool GetEmbeddedExecutingApiCode();
        void RequireRequestDone(QString Id, QString Filename, bool IsFailure);

        void WebInterfaceEval(const QString& Script);
        void OnWaiterStarted();
        void OnWaiterEnded();
        QString Preprocess(const QString& script);
        virtual void Fail(const QString& message, bool dont_create_more);
        void FailInternal(const QString& message, bool dont_create_more, LogLevel level);
        void FailUser(const QString& message, bool dont_create_more);
        void Info(const QString& message);
        void SetMaxFail(int MaxFail);
        virtual void Die(const QString& message, bool instant);
        virtual void DieInternal(const QString& message, bool instant, LogLevel level);
        virtual void Success(const QString& message);
        void SetMaxSuccess(int MaxSuccess);
        virtual QString GetResultMessage();
        virtual QString GetResultMessageRaw();
        virtual QString GetResultMessageRawWithId();
        virtual WorkerStatus GetResultStatus();
        virtual void Abort(bool SignalResourceHandlers);
        virtual void Run();
        virtual void InterruptAction();
        virtual void SetFailMessage(const QString& message);
        virtual qint64 GetThreadNumber();
        virtual qint64 GetSuccessNumber();
        virtual qint64 GetFailNumber();
        virtual QString GetProjectPath();

        void Decrypt(const QString& Data);

        virtual void FailBecauseOfTimeout();
        virtual void FailDelayed();
        virtual void FailProcessFinished();
        virtual void RunSubScript();
        virtual void RunSubScriptNoWorkerUpdate();


        virtual void SetScript(const QString& Script);
        virtual QString GetScript();

        virtual void SetAsyncResult(const QScriptValue & AsyncResult);
        virtual QScriptValue GetAsyncResult();

        virtual void SetAsyncFailMessage(const QString & AsyncFailMessage);
        virtual QString GetAndClearAsyncFailMessage();

        virtual void SetAsyncDieMessage(const QString & AsyncDieMessage);
        virtual QString GetAndClearAsyncDieMessage();

        virtual bool IsDieInstant();
        virtual bool IsDontCreateMore();
        void ReloadResource(const QString& name);
        void ClearResource(const QString& name);

        //Recatpcha V3
        void SetRecaptchaV3SolverProperties(const QString& method, const QString& key, const QString & serverurl, double score, bool use_proxy);
        virtual void SolveRecaptchaV3(const QString& id, const QString& action, const QString & sitekey, const QString & url);
        void RecaptchaV3Solved(const QString& val, const QString& id, bool res,const QString& solver_id);


        void AbortSubscript();
        void SolveInternal(const QString& method, const QString& base64, const QStringList & params, const QString& callback);

        void Solve(const QString& method, const QString& base64, const QStringList & params,const QString& callback);
        void SolveNoFail(const QString& method, const QString& base64, const QStringList & params,const QString& callback);
        void SolveCaptcha(const QString& method, const QString& base64, const QStringList & params,  const bool fail_on_error, const QString& callback);

        QScriptValue GetRootElement();
        QScriptValue GetSolver(const QString& name);
        void GetHandler(const QString& name, bool die_on_fail, const QString& callback);
        void Refuse(const QString & name,const QString & value);
        QScriptValue GetAllData(const QString& name);
        QScriptValue GetRandomSubarrayData(const QString& name, int size);
        QScriptValue GetAtIndex(const QString& name, int index);
        void SetAtIndex(const QString& name, int index, const QString& val);

        QScriptValue GetTotalLength(const QString& name);
        void DebugTerminate(int how);


        QScriptValue PrepareWebElement(IWebElement* web);
        void SetCurrentAction(qint64 CurrentAction);
        qint64 GetCurrentAction();
        QString PrepareMessage(const QString &message);
        QString PrepareMessageNoId(const QString &message);

        void Sleep(int msec, const QString& callback);
        void WaitNextTask(const QString& callback);
        void StartNextTask();
        void Suspend(int msec, const QString& callback);

        void ScriptFinished(bool SignalResourceHandlers);
        void AttachNetworkAccessManager();

        void ProgressValueSlot(int);
        void ProgressMaximumSlot(int);

        /* Clipboard */
        void SetClipboard(bool IsBase64, const QString& Mime, const QString& DataStr);
        QString GetClipboard(bool IsBase64, const QString& Mime);

        void NewPop3Client();
        void Pop3ClientLoadMessageNumber(const QString& callback);
        void Pop3ClientLoadMessage(int index, const QString& callback);
        void NewImapClient();
        void ImapClientPullNumberOfMessages(const QString& callback);
        void ImapClientSearchMessages(const QString& Sender,const QString& Subject,const QString& Body,const QString& To,const QString& callback);
        void ImapClientCustomSearchMessages(const QString& Query,const QString& callback);
        void ImapClientPullMessage(const QString& Uid,const QString& callback);
        void ImapClientCustomQuery(const QString& Url,const QString& Command,const QString& Filter,const QString& callback);

        void RequireExtensions(const QString& extensions, const QString& callback);

        void NewHttpClient();
        void FollowRedirect();
        void FollowRedirectDownload();

        void HttpClientGetNoRedirect(const QString& url, const QString& callback);
        void HttpClientGetRedirect(const QString& url, const QString& callback);

        void HttpClientGetNoRedirect2(const QString& url, const QStringList & params_glob, const QString& callback);
        void HttpClientGetRedirect2(const QString& url, const QStringList & params_glob, const QString& callback);

        void HttpClientDownload(const QString& url, const QString& file, const QString& callback);

        void HttpClientPostNoRedirect(const QString& url, const QStringList & params, const QStringList & params_glob, const QString& callback);
        void HttpClientPostRedirect(const QString& url, const QStringList & params, const QStringList & params_glob, const QString& callback);

        void SetFailFunction(const QString& FailFunction);
        void SetSuccessFunction(const QString& SuccessFunction);
        void SetFailExceedFunction(const QString& FailExceedFunction);
        void SetSuccessExceedFunction(const QString& SuccessExceedFunction);
        void SetAbortFunction(const QString& AbortFunction);
        void FormatRecord(const DatabaseItem& Record, int TableId, QString& GroupName, QString& FormattedString);
        QString DatabaseAddRecord(const QString& GroupId,const QStringList& Record, int TableId, bool AddToResources);
        QStringList DatabaseSelectRecords(const QString& FilterJson,int PageNumber, int PageSize, int TableId, const QString& SortingType, const QString& GroupId, int SortColumn);
        QString DatabaseSelectOneRecord(const QString& RecordId, int TableId);
        void DatabaseDeleteGroup(const QString& GroupId, bool DeleteData, int TableId);
        void DatabaseDeleteOneRecord(const QString& RecordId, bool DeleteResources, int TableId);
        void DatabaseChangeGroup(const QString& Name, const QString& Description, const QString& GroupId, int TableId);
        QString DatabaseCopyGroup(const QString& GroupId, int TableId);
        QString DatabaseMoveRecordToGroup(const QString& GroupId, const QString& RecordId, const QString& MoveType, int TableId);
        QStringList DatabaseSelectGroups(int TableId);
        void DatabaseDeleteRecords(const QString& FilterJson,int TableId, const QString& GroupId, bool DeleteResources);
        int DatabaseCountRecords(const QString& FilterJson,int TableId, const QString& GroupId);
        void DatabaseUpdateRecord(const QString& RecordId,const QStringList& Record, int TableId, bool UpdateResources);
        void DatabaseUpdateRecordsByCriteria(const QStringList& Record, int TableId, bool UpdateResources, const QString& FilterJson, const QString& GroupId);
        QString DatabaseAddGroup(const QString& GroupName,const QString& GroupDescription, int TableId);

        QString Spintax(const QString& Text);

        //CallDll functions
        QString ExecuteNativeModuleCodeSync(const QString& DllName, const QString& FunctionName, const QString& InputParam);
        void ExecuteNativeModuleCodeAsync(const QString& DllName, const QString& FunctionName, const QString& InputParam, const QString& Callback);
        void RemoveFromFunctionDataList();
        void DllResult();

        //Timeouts
        void SetGeneralWaitTimeout(int timeout);
        void SetGeneralWaitTimeoutNext(int timeout);
        void SetSolverWaitTimeout(int timeout);
        void SetSolverWaitTimeoutNext(int timeout);

        virtual void SetIsRecord(bool IsRecord);
        virtual bool GetIsRecord();

        void SwitchHttpClient(int index);
        void XpathHttpClient();


        /* Substage */
        virtual void SubstageSetStartingFunction(const QString& StartingFunction);
        virtual QString SubstageGetStartingFunction();
        virtual int SubstageGetId();
        virtual void SubstageSetId(int Id);
        virtual int SubstageGetParentId();
        virtual void SubstageSetParentId(int Id);
        virtual void SubstageFinished(int Id);
        void SubstageCall(const QString& StartingFunction, qint64 ThreadsNumber, qint64 MaximumSuccess, qint64 MaximumFailure, const QString& Callback);
        void SendTaskResult(const QString& ResultData);
        void SendTaskResultFail(const QString& Message);
        void SendTaskResultNullSuccess();
        void SubstageFinishAndRunNext();

        /* Web interface task */
        virtual void TaskDataReceived(int Id, const QString& FunctionName, const QString& FunctionParams, int ResultId);
        QString GetTaskFunctionName();
        QString GetTaskFunctionParams();
        virtual bool IsTaskRunning();
        virtual int RunningTaskId();

        void Embedded(const QString& Variables,const QString& VariablesList, const QString& Function, const QString& LanguageName, const QString& LanguageVersion, int Timeout, const QString& Callback);
        void EmbeddedApiResponce(const QString& Variables);
        void PrepareFunction(const QString& FunctionName);
        virtual void PrepareFunctionResult(const QString& FunctionName, const QString& FunctionData);
        virtual void StopAllFunctions();
        virtual void GetNumberOfRunningFunctions(int * FunctionsTotal);

        void DNSLookup(const QString& Domain, const QString& Callback);

        /* Custom threads */
        void CustomThreadStartSlot(int ThreadId);
        void CustomThreadRunFunctionSlot(int ThreadId, QString FunctionName, QString Parameters, bool StopThreadAfterTaskFinish, bool StartPostponedOnIdle);
        void CustomThreadStopSlot(int ThreadId);
        void CustomThreadStopListSlot(QString ThreadList);
        void CustomThreadQueueFunctionSlot(int ThreadId, QString FunctionName, QString Parameters, bool StopThreadAfterTaskFinish, bool StartPostponedOnIdle, int MaxThreads);

        bool CustomThreadGetIsRunning(int ThreadId);
        bool CustomThreadGetWasRunningAtLeastOnce(int ThreadId);
        bool CustomThreadGetIsSuccess(int ThreadId);
        QString CustomThreadGetResult(int ThreadId);
        QString CustomThreadGetError(int ThreadId);
        int CustomThreadsTotal();
        bool CustomThreadExists(int ThreadId);
        bool CustomThreadCanStartInIddleThread();
        int CustomThreadGetIddleThread();

    private slots:

        void DNSLookupResult();
        void HandlerWaitFinishedSuccess();
        void HandlerWaitFinishedFailed();
        void SolverSuccess();
        void SolverFailed();
        void Pop3ClientResult();
        void ImapClientResult();
        void NeedToWaitForResource(const QString& ResourceName);


        void ReceivedResultData(quint64 Id, const QString& Variables, bool IsSuccess, const QString& ErrorMessage);
        void ReceivedApiData(quint64 Id, const QString& Variables,const QString& ApiString);
        void EmbeddedCallFinal();
        void EmbeddedCallTimeout();
        void EmbeddedTaskKill();
        void EmbeddedApiFail(const QString& Message);
        void EmbeddedLog(const QString& Text,quint64 Id);


    private:
        void FollowRedirectInternal(bool IsGet);


        friend static QScriptValue prepare(QScriptEngine *engine, IWebElement* web);


    };
}
#endif // WORKER_H
