#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "iengineresources.h"
#include "imultiworker.h"
#include "iscriptresources.h"
#include "ihttpclientfactory.h"
#include "ipop3clientfactory.h"
#include <QSettings>
#include <QMovie>
#include "outputsection.h"
#include "resources.h"
#include "qtresourcecontroller.h"
#include "engineresources.h"
#include "engineresourcecontroller.h"
#include "stringbuilder.h"
#include "isystemtraynotifier.h"
#include <QPushButton>
#include "askuserforresourcesdialog.h"
#include "additionalsettingswidgets.h"
#include "scriptmultiworkerreportdata.h"
#include "reportdatavisualizer.h"
#include "resourcesvisualizer.h"
#include "ilanguagemodel.h"
#include "translator.h"
#include "iimapclientfactory.h"
#include "ihtmlparserfactory.h"
#include "ihelperfactory.h"
#include "multitimer.h"
#include "databaseadmin.h"
#include "mongodatabaseconnector.h"
#include "csvhelper.h"
#include "browserversionselector.h"
#include "databasestate.h"
#include "modulemanager.h"
#include "mainwindowtabblink.h"
#include "embeddedlanguagemanager.h"
#include "logfilter.h"
#include "complexlogger.h"
#include "schedulerconnector.h"


namespace Ui {
class MainWindow;
}

using namespace BrowserAutomationStudioFramework;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    SchedulerConnector *_SchedulerConnector;
    ComplexLogger *ComplexLoggerLog;
    LogFilter *_LogFilter;
    IEmbeddedLanguageManager *_EmbeddedLanguageManager;
    DatabaseAdmin *_DatabaseAdmin;
    bool NeedRestart;
    QString LogFileName;
    OutputSection *Output;
    MultiTimer *_MultiTimer;
    IBrowserFactory *BrowserFactory;
    ISolverFactory *FactorySolver;
    Resources *Res;
    QSettings *Settings;
    EngineResources *EngineRes;
    EngineResourceController *EngineResController;
    BrowserAutomationStudioFramework::StringBuilder *StringBuild;
    IMultiWorker * Worker;
    QString CurrentFileName;
    IScriptResources* ScriptResources;
    IHttpClientFactory * _HttpClientFactory;
    IPop3ClientFactory * _Pop3ClientFactory;
    IImapClientFactory *_ImapClientFactory;
    IHtmlParserFactory *_HtmlParserFactory;
    IHelperFactory *_HelperFactory;
    QMovie *movie;
    ISystemTrayNotifier *TrayNotifier;
    QPushButton *LogMenuButton;
    QPushButton * LogFilterButton;
    Translator TranslateEngine;
    Translator TranslateFastExecuteScript;
    QString LastLoaderResult;
    QString LogLocation;
    MongoDatabaseConnector *_DataBaseConnector;
    MongoDatabaseConnector *_DataBaseConnector2;
    MongoDatabaseConnector *_DataBaseConnector3;
    DatabaseState *_DataBaseState;
    MainWindowTabBlink *_TabBlink;


    AskUserForResourcesDialog *Ask;
    AdditionalSettingsWidgets *AdditionalSettings;
    QtResourceController *UserWidgetControllerPointer;
    bool CaptchaWidgetPresent;
    ILanguageModel * LangModel;
    CsvHelper *_CsvHelper;
    BrowserVersionSelector *_BrowserVersionSelector;

    ModuleManager *_ModuleManager;

    QString Language;

    //Reports
    ScriptMultiWorkerReportData *ReportData;
    ReportDataVisualizer *DataVisualizer;
    ResourcesVisualizer *ResVisualizer;
    bool IsClosingWindow;
    bool AlreadyShowedBrowserPanel;
    bool AlreadyShowsMessage;
    QDateTime LastShowWindow;

    void SetDatabaseConnector(MongoDatabaseConnector * _DatabaseConnector,MongoDatabaseConnector * _DatabaseConnector2,MongoDatabaseConnector * _DatabaseConnector3, DatabaseState* _DataBaseState, CsvHelper* _CsvHelper, SchedulerConnector* _SchedulerConnector);

    bool IsSilent;
    QString Remote;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void SetIsSilent();
    void SetRemote(const QString& Remote);

    void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *ui;
    void changeEvent(QEvent *e);
    bool IsRDP;

private slots:
    void LogAnchorClicked(QUrl url);
    void FilterChanged(QList<LogLevel>,int,QString,bool);
    void InitBrowserList(const QString& WrokerType);
    void UpdateCaptchaSize(int size);
    void InitRecources();
    void InitWidgets();
    void AddedOutput1();
    void AddedOutput2();
    void AddedOutput3();
    void AddedOutput4();
    void AddedOutput5();
    void AddedOutput6();
    void AddedOutput7();
    void AddedOutput8();
    void AddedOutput9();

    void Close();
    void Restart();

    void Finished();
    void LogMenu_Click();
    void LogFilter_Click();
    void SetDefault();
    void ShowData();
    void SaveActual(const QString& filename);
    void LoadActual(const QString& filename);
    void ChooseLanguage();
    void ChooseLogLocation();
    void ShowDatabase();
    void ShowBrowserPanel();
    void ResourcesReport();
    void ResourcesReportStateChanged(bool IsEmpty);
    void OnEmbeddedLanguageStarted();
    void RemoveOldTunnels();
    bool IsProcessRunning(int pid);

public slots:
    void ToggleVisibility();
    void Start();
    void Abort();

signals:
    void SyncResources();

};

#endif // MAINWINDOW_H

