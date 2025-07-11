#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMovie>
#include "isolverfactory.h"
#include "imultiworker.h"
#include "resources.h"
#include "qtresourcecontroller.h"
#include "iscriptresources.h"
#include "engineresources.h"
#include "engineresourcecontroller.h"
#include "stringbuilder.h"
#include <QSettings>
#include "compiler.h"
#include "ihttpclientfactory.h"
#include "ipop3clientfactory.h"
#include "outputsection.h"
#include "isystemtraynotifier.h"
#include <QPushButton>
#include "askuserforresourcesdialog.h"
#include "additionalsettingswidgets.h"
#include "scriptmultiworkerreportdata.h"
#include "reportdatavisualizer.h"
#include "resourcesvisualizer.h"
#include "ilanguagemodel.h"
#include "translator.h"
#include "imapclientfactory.h"
#include "icodeeditor.h"
#include "textfinderwidget.h"
#include "ihtmlparserfactory.h"
#include "ihelperfactory.h"
#include "multitimer.h"
#include "databaseadmin.h"
#include "mongodatabaseconnector.h"
#include "csvhelper.h"
#include "browserversionselector.h"
#include "databasestate.h"
#include "recordprocesscommunication.h"
#include "modulemanager.h"
#include "modulecreator.h"
#include "diffpatcher.h"
#include "mainwindowtabblink.h"
#include "profilerdata.h"
#include "profilerdatavisualizer.h"
#include "httpsniffer.h"
#include "embeddedlanguagemanager.h"
#include "logfilter.h"
#include "webdriverlogfilter.h"
#include "complexlogger.h"
#include "webinterface.h"
#include "webinterfaceapi.h"
#include "iprocesscomunicatorfactory.h"



namespace Ui {
    class MainWindow;
}
using namespace BrowserAutomationStudioFramework;
class MainWindow : public QMainWindow {
    Q_OBJECT
    ICodeEditor *TextEditor;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    bool IsRDP;
    WebInterface *_WebInterface;
    WebInterfaceApi *_WebInterfaceApi;
    bool WebInterfaceIsRememberChoice;
    bool WebInterfaceRememberChoiceUseCustom;
    QString PremiumUsername;
    QString PremiumPassword;
    QString AutorunPath;
    bool IsAutorun;
    ComplexLogger *ComplexLoggerLog;
    ComplexLogger *ComplexLoggerWebDriver;

    LogFilter *_LogFilter;
    WebDriverLogFilter *_WebDriverLogFilter;
    EmbeddedLanguageManager * _EmbeddedLanguageManager;
    DiffPatcher _DiffPatcher;
    DiffPatcher _DiffPatcherEmbeddedData;
    MultiTimer *_MultiTimer;
    QString LogFileName;
    OutputSection *Output;
    IBrowserFactory *BrowserFactory;
    ISolverFactory *FactorySolver;
    Resources *Res;
    QSettings *Settings;
    QtResourceController *WidgetController;
    EngineResources *EngineRes;
    EngineResourceController *EngineResController;
    BrowserAutomationStudioFramework::StringBuilder *StringBuild;
    BrowserAutomationStudioFramework::ProfilerData *_ProfilerData;
    IMultiWorker * Worker;
    QString CurrentFileName;
    IScriptResources* ScriptResources;
    TextFinderWidget* _TextFinderWidget;
    DatabaseAdmin *_DatabaseAdmin;
    MongoDatabaseConnector *_DataBaseConnector;
    MongoDatabaseConnector *_DataBaseConnector2;
    MongoDatabaseConnector *_DataBaseConnector3;
    DatabaseState *_DataBaseState;
    RecordProcessCommunication *_RecordProcessCommunication;
    MainWindowTabBlink *_TabBlink;
    QString Schema;
    QString DatabaseId;
    bool ConnectionIsRemote;
    QString ConnectionServer;
    QString ConnectionPort;
    QString ConnectionLogin;
    QString ConnectionPassword;
    QString InterfaceState;
    QString ScriptEngineVersion;
    bool IsDatabaseDirty;
    HttpSniffer *_HttpSniffer = 0;

    QList<IRecordProcessCommunication::ResourceDescription> LastResourceList;

    CsvHelper *_CsvHelper;
    BrowserVersionSelector *_BrowserVersionSelector;
    void closeEvent(QCloseEvent *event);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

    Compiler *compiler;
    ModuleCreator *_ModuleCreator;
    IHttpClientFactory * _HttpClientFactory;
    IPop3ClientFactory * _Pop3ClientFactory;
    IImapClientFactory * _ImapClientFactory;
    IHtmlParserFactory *_HtmlParserFactory;
    IHelperFactory *_HelperFactory;
    QMovie *movie;
    ISystemTrayNotifier *TrayNotifier;
    QPushButton * LogMenuButton;
    QPushButton * LogWebDriverMenuButton;
    QPushButton * LogFilterButton;
    QPushButton * WebDriverLogFilterButton;
    QString LogLocation;


    AskUserForResourcesDialog *Ask;
    AdditionalSettingsWidgets *AdditionalSettings;
    QtResourceController *UserWidgetControllerPointer;
    QDockWidget *dockWidgetCaptcha;
    QDockWidget *dockWidgetWebDriverLog;

    ILanguageModel *LangModel;
    QString LanguageScript, LanguageInterface;
    Translator TranslateEngine;
    Translator TranslateStudio;

    ModuleManager *_ModuleManager;


    //Reports
    bool AlreadyShowsMessage;
    ScriptMultiWorkerReportData *ReportData;
    ReportDataVisualizer *DataVisualizer;
    ProfilerDataVisualizer *ProfilerVisualizer;
    ResourcesVisualizer *ResVisualizer;
    bool IsClosingWindow;

    bool IsRecordLast;
    bool NeedRestart;
    bool IsModified;
    QDateTime LastShowWindow;

private:
    Ui::MainWindow *ui;


private slots:
    void RunScheduler();
    void InstallScheduler();
    void FilterChanged(QList<LogLevel>,int,QString,bool);
    void WebDriverFilterChanged(QList<LogLevel>,int,QString,bool);
    void InitBrowserList(const QString& WrokerType, bool IsRecord);
    void AddedOutput1();
    void AddedOutput2();
    void AddedOutput3();
    void AddedOutput4();
    void AddedOutput5();
    void AddedOutput6();
    void AddedOutput7();
    void AddedOutput8();
    void AddedOutput9();
    void Run();
    void Record();
    void RunInternal();
    void Compile();
    void CreateArchive();
    void New();
    void Open();
    void Show();
    void ShowInternal();
    bool SavePrevious();
    void Close();
    void CloseNoSave();
    void ResourcesReport();
    void OpenUrl();
    void UploadToGoogleDrive();
    void ShowBackupPath();
    QString OpenFromFile(const QString& fileName);
    void OpenFromFileOrDisplayMessageBox(const QString& fileName);
    void ResourcesNotEmpty();
    void ResourcesEmpty();
    void HighlightAction(QUrl url);
    void AbortNotInstantInternal();
    void AbortInstantInternal();
    void WebDriverSetupLog(IProcessComunicatorFactory *Factory);
    void ShowScriptCode();
    void WriteInfoOnStart();

    void SetIsDirty(bool IsDirty);
    void OpenDefault();
    void LoadSchema();
    void Save();
    void AbortInstant();
    void Abort();
    void SaveAs();
    QPair<bool,QString> SaveToFileSilent(const QString& file);
    void SaveToFile(const QString& file);
    void InitAction();
    void InitRecources();
    void InitWidgets();
    void StartAction();
    void StopAction();
    void UpdateCaptchaSize(int size);
    void ClearState();
    void RestoreState();
    void ResourcesReportStateChanged(bool IsEmpty);
    void RestoreMaxHeight();
    void SetCurrentFileName(const QString& CurrentFileName);
    void UpdateTitle();
    void SetModified();
    void SetNotModified();
    void RecordWindowAttached();

    void LogMenu_Click();
    void LogWebDriverMenu_Click();
    void LogFilter_Click();
    void LogWebDriverFilter_Click();
    void SetDefault();
    void ShowData();
    void ShowDataBaseDialogNoAsk();
    void ShowDataBaseManager();
    void ShowDataBase();
    void ShowDataBaseDialog();
    void SaveActual(const QString& filename);
    void LoadActual(const QString& filename);
    void Documentation();
    void ShowDebugVersion();
    void Localization();
    void ChangeScriptLanguage();
    void ChangeInterfaceLanguage();
    void ChooseLogLocation();

    void AboutEngine();
    void RegexpConstructor();
    void FingerprintSwitcher();
    void ShowModuleManager();
    void SetDefaultText();

    void SendCode();
    void ReceiveCode(const QString& Code);
    void ReceiveInterface(const QString& Json);
    void SetNeedRestart();
    void RemoveOldTunnels();
    bool IsProcessRunning(int pid);

    void on_ButtonAddNewResource_clicked();
    QString GetRandomString();
signals:
    void SyncResources();
    void CurrentFileNameHasChanged(QString);
};

#endif // MAINWINDOW_H
