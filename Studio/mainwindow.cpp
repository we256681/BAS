#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plaintextlogger.h"
#include "scriptmultiworker.h"
#include "httpclientfactory.h"
#include "pop3clientfactory.h"
#include "scriptresourcesfromqt.h"
#include "timeoutwaiter.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
#include <QScrollBar>
#include <QInputDialog>
#include <QTextStream>
#include "manualcaptchasolver.h"
#include "solverfactory.h"
#include "qtresourcecontroller.h"
#include "resourcedesignwidgetfactory.h"
#include "resourceuserwidgetfactory.h"
#include "resources.h"
#include "resourcemodelfixedinteger.h"
#include "resourcemodelfixedstring.h"
#include "xmlresourcecontroller.h"
#include "askuserforresourcesdialog.h"
#include "skincontroller.h"
#include "resourcehandlers.h"
#include "timeoutwaiterfactory.h"
#include "resourcehandlersfactory.h"
#include "scriptworkerfactory.h"
#include "preprocessor.h"
#include "networkaccessmanagerfactory.h"
#include "subprocessbrowserfactory.h"
#include "subprocessnetworkaccessmanagerfactory.h"
#include "processcomunicatorfactory.h"
#include "additionalsettingswidgets.h"
#include "compilewindow.h"
#include "compileresults.h"
#include <QDesktopServices>
#include "complexlogger.h"
#include "filelogger.h"
#include "systemtraynotifier.h"
#include "nonetraynotifier.h"
#include "toprightpositioner.h"
#include "copyresourcecontroller.h"
#include "versioninfo.h"
#include "scriptsuspender.h"
#include "helper.h"
#include "languagemodel.h"
#include "languagechooserdevelopmentwidget.h"
#include "scriptlanguagechooserdialog.h"
#include "periodicalhttprequest.h"
#include "htmlparser.h"
#include "imapclientfactory.h"
#include "scintillacodeeditor.h"
#include "plaincodeeditor.h"
#include "advancedhider.h"
#include "htmlparserfactory.h"
#include "helperfactory.h"
#include "memoryinfo.h"
#include "loglocationchooser.h"
#include "noneencryptor.h"
#include "properties.h"
#include <QDirIterator>
#include "modulemanagerwindow.h"
#include "projectbackup.h"
#include "databaseconnectionwindow.h"
#include "databasestatedialog.h"
#include "constructresourcewizard.h"
#include "workersettings.h"
#include "pcresourcessmoothusage.h"
#include "webinterface.h"
#include "webinterfaceresourcecontroller.h"
#include "httpclient.h"
#include "webinterfacelogger.h"
#include "webinterfacesolver.h"
#include "webdriverlogsyntax.h"
#include "downloadprojectdialog.h"
#include "uploadproject.h"
#include "schedulerinstaller.h"
#include "checkscript.h"
#include "every_cpp.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), Worker(0), UserWidgetControllerPointer(0), dockWidgetCaptcha(0), dockWidgetWebDriverLog(0), IsClosingWindow(false), Output(0),_DatabaseAdmin(0), Ask(0), WidgetController(0)
{

    qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));

    {
        QString ClientName = QProcessEnvironment::systemEnvironment().value("ClientName", "").toLower();
        IsRDP = ClientName != "" && ClientName != "console";
    }


    LogWebDriverMenuButton = 0;
    WebDriverLogFilterButton = 0;

    _WebInterface = 0;
    _WebInterfaceApi = 0;
    WebInterfaceIsRememberChoice = false;
    WebInterfaceRememberChoiceUseCustom = false;

    _EmbeddedLanguageManager = new EmbeddedLanguageManager(this);
    _EmbeddedLanguageManager->SetDiffPatcher(&_DiffPatcherEmbeddedData);
    connect(_EmbeddedLanguageManager,SIGNAL(Started()),this,SLOT(RunInternal()));

    _TabBlink = new MainWindowTabBlink(this);
    _TabBlink->SetWidget(this);

    AlreadyShowsMessage = false;

    ui->setupUi(this);

    ComplexLoggerLog = 0;
    ComplexLoggerWebDriver = 0;
    _WebDriverLogFilter = 0;

    ui->EditOutput->setOpenExternalLinks(false);
    ui->EditOutput->setOpenLinks(false);
    connect(ui->EditOutput,SIGNAL(anchorClicked(QUrl)),this,SLOT(HighlightAction(QUrl)));

    _ModuleManager = new ModuleManager(this);

    _ModuleManager->TransferModulesFromOldVersion();

    _ProfilerData = new ProfilerData(this);

    SetCurrentFileName(QDir::current().absoluteFilePath("project.xml"));

    IsModified = false;


    connect(ui->Recents,SIGNAL(FileSelected(QString)),this,SLOT(SavePrevious()));
    connect(ui->Recents,SIGNAL(FileSelected(QString)),this,SLOT(OpenFromFileOrDisplayMessageBox(QString)));

    _RecordProcessCommunication = new RecordProcessCommunication(this);
    connect(_RecordProcessCommunication,SIGNAL(ReceiveCode(QString)),this,SLOT(ReceiveCode(QString)));
    connect(_RecordProcessCommunication,SIGNAL(EmbeddedData(QString)),_EmbeddedLanguageManager,SLOT(DeserializeData(QString)));
    connect(_RecordProcessCommunication,SIGNAL(LoadScript(QString)),this,SLOT(OpenFromFileOrDisplayMessageBox(QString)));
    connect(_RecordProcessCommunication,SIGNAL(NeedRestart()),this,SLOT(SetNeedRestart()));
    connect(_RecordProcessCommunication,SIGNAL(Stop()),this,SLOT(AbortInstant()));

    connect(_EmbeddedLanguageManager,SIGNAL(CheckResult(QString,bool,QString)),_RecordProcessCommunication,SLOT(CheckResult(QString,bool,QString)));


    _MultiTimer = new MultiTimer(this);
    //_MultiTimer = 0;

    _DataBaseConnector = new MongoDatabaseConnector(this);
    {
        DatabaseConnectionWindow * _DatabaseConnectionWindow = new DatabaseConnectionWindow(_DataBaseConnector);
        _DatabaseConnectionWindow->SetIsSilent(false);
        _DataBaseConnector->SetDatabaseConnectionWindow(_DatabaseConnectionWindow);
    }

    _BrowserVersionSelector = new BrowserVersionSelector(this);

    _CsvHelper = new CsvHelper(_DataBaseConnector);
    _DataBaseConnector->SetCsvHelper(_CsvHelper);

    _DataBaseState = new DatabaseState(this);
    _DataBaseConnector2 = new MongoDatabaseConnector(_DataBaseState);
    {
        DatabaseConnectionWindow * _DatabaseConnectionWindow = new DatabaseConnectionWindow(_DataBaseConnector2);
        _DatabaseConnectionWindow->SetIsSilent(false);
        _DataBaseConnector2->SetDatabaseConnectionWindow(_DatabaseConnectionWindow);
    }
    _DataBaseState->SetDatabaseConnector(_DataBaseConnector2);

    _DataBaseConnector3 = new MongoDatabaseConnector(this);
    {
        DatabaseConnectionWindow * _DatabaseConnectionWindow = new DatabaseConnectionWindow(_DataBaseConnector3);
        _DatabaseConnectionWindow->SetIsSilent(false);
        _DataBaseConnector3->SetDatabaseConnectionWindow(_DatabaseConnectionWindow);
    }

    connect(_DataBaseConnector,SIGNAL(GroupManipulated()),_DataBaseState,SLOT(Reload()));

    #ifdef QT_DEBUG
        TextEditor = new PlainCodeEditor(this);
    #else
        TextEditor = new ScintillaCodeEditor(this);
    #endif

    connect(ui->ScriptLabel,SIGNAL(linkActivated(QString)),this,SLOT(ShowScriptCode()));

    TextEditor->GetWidget()->setParent(ui->dockWidgetScript);
    ui->ScriptLayout->addWidget(TextEditor->GetWidget());

    _TextFinderWidget = new TextFinderWidget(this);
    ui->ScriptLayout->addWidget(_TextFinderWidget);

    TextEditor->GetWidget()->setVisible(false);
    _TextFinderWidget->setVisible(false);



    connect(TextEditor,SIGNAL(ShowFinder()),_TextFinderWidget,SLOT(Show()));
    connect(TextEditor,SIGNAL(Changed()),_RecordProcessCommunication,SLOT(SendIsChanged()));
    connect(TextEditor,SIGNAL(Changed()),this,SLOT(SetModified()));

    connect(_TextFinderWidget,SIGNAL(TermChanged(QString)),TextEditor,SIGNAL(SetSearchTerm(QString)));
    connect(_TextFinderWidget,SIGNAL(FindNext(bool)),TextEditor,SIGNAL(FindNext(bool)));

    TranslateEngine.SetDirectory(":/engine/translate");
    TranslateStudio.SetDirectory(":/studio/translate");

    LangModel = new LanguageModel(this);

    if(qApp->arguments().indexOf("--hide-tray-icon") < 0)
    {
        TrayNotifier = new SystemTrayNotifier(this);
    }else
    {
        TrayNotifier = new NoneTrayNotifier(this);
    }

    TrayNotifier->Init();


    BrowserFactory = 0;

    compiler = new Compiler(this);
    _ModuleCreator = new ModuleCreator(this);
    Settings = new QSettings("settings.ini",QSettings::IniFormat,this);
    LogLocation = Settings->value("LogLocation","../../logs").toString();
    SetCurrentFileName(Settings->value("CurrentFileName",QDir::current().absoluteFilePath("project.xml")).toString());
    SkinController * skin = new SkinController();
    skin->ApplyTheme(Settings->value("Theme","DarkFusion").toString());
    skin->deleteLater();

    LanguageInterface = Settings->value("DefaultLanguageInterface","").toString().toLower();


    if(LanguageInterface.isEmpty())
    {
        ScriptLanguageChooserDialog LangDialog;
        LangDialog.SetLanguageModel(LangModel);
        if(!LangDialog.exec())
        {
            QTimer::singleShot(50,this,SLOT(CloseNoSave()));
            return;
        }
        LanguageInterface = LangDialog.GetLang();
        Settings->setValue("DefaultLanguageInterface",LanguageInterface);
    }

    LangModel->SetInterfaceLanguage(LanguageInterface);
    LanguageScript = Settings->value("DefaultLanguageScript",LanguageInterface).toString().toLower();
    _DataBaseConnector->Init(LanguageScript);
    _DataBaseConnector2->Init(LanguageScript);

    LoadSchema();


    if(!_DataBaseConnector->Start(Schema, DatabaseId, ConnectionIsRemote, ConnectionServer, ConnectionPort, ConnectionLogin, ConnectionPassword))
    {
        QEventLoop loop;
        connect(_DataBaseConnector, SIGNAL(Started()), &loop, SLOT(quit()));
        loop.exec();
    }
    if(_DataBaseConnector->HasDatabase())
    {
        if(_DataBaseConnector->WasError())
        {
            QTimer::singleShot(50,this,SLOT(CloseNoSave()));
            return;
        }

        if(!_DataBaseConnector2->Start(Schema, DatabaseId, ConnectionIsRemote, ConnectionServer, ConnectionPort, ConnectionLogin, ConnectionPassword))
        {
            QEventLoop loop;
            connect(_DataBaseConnector2, SIGNAL(Started()), &loop, SLOT(quit()));
            loop.exec();
        }
        if(_DataBaseConnector2->WasError())
        {
            QTimer::singleShot(50,this,SLOT(CloseNoSave()));
            return;
        }

        if(!_DataBaseConnector3->Start(Schema, DatabaseId, ConnectionIsRemote, ConnectionServer, ConnectionPort, ConnectionLogin, ConnectionPassword))
        {
            QEventLoop loop;
            connect(_DataBaseConnector3, SIGNAL(Started()), &loop, SLOT(quit()));
            loop.exec();
        }
        if(_DataBaseConnector3->WasError())
        {
            QTimer::singleShot(50,this,SLOT(CloseNoSave()));
            return;
        }

        _DataBaseState->Reload();
    }


    TranslateEngine.Translate(LanguageInterface);
    TranslateStudio.Translate(LanguageInterface);

    _HttpSniffer = new HttpSniffer();

    _LogFilter = new LogFilter(this);
    _LogFilter->SetAvailableLevels(QList<LogLevel>()<<LogFail<<LogSuccess<<LogInfo<<LogUser);
    _LogFilter->SetThreadsNumber(0);

    QWidget *LogButtonsContainer = new QWidget(ui->DockLog);

    LogButtonsContainer->setMaximumWidth(70);

    LogMenuButton = new QPushButton(LogButtonsContainer);
    LogMenuButton->setIcon(QIcon(":/images/menu2.png"));
    LogMenuButton->setMaximumWidth(30);
    LogMenuButton->setMinimumHeight(30);
    LogMenuButton->setVisible(false);

    LogFilterButton = new QPushButton(LogButtonsContainer);
    LogFilterButton->setIcon(QIcon(":/images/search.png"));
    LogFilterButton->setMaximumWidth(30);
    LogFilterButton->setMinimumHeight(30);
    LogFilterButton->setVisible(false);

    QHBoxLayout *LogButtonsLayout = new QHBoxLayout(LogButtonsContainer);
    LogButtonsLayout->setMargin(0);
    LogButtonsLayout->setSpacing(10);
    LogButtonsLayout->setContentsMargins(0,0,0,0);
    LogButtonsContainer->setLayout(LogButtonsLayout);
    LogButtonsLayout->addWidget(LogFilterButton);
    LogButtonsLayout->addWidget(LogMenuButton);

    TopRightPositioner * AllButtonPositioner = new TopRightPositioner(this);
    AllButtonPositioner->SetChild(LogButtonsContainer);
    AllButtonPositioner->SetParent(ui->DockLog);
    AllButtonPositioner->SetMarginRight(20);
    AllButtonPositioner->SetMarginTop(10);
    AllButtonPositioner->Start();



    connect(LogMenuButton,SIGNAL(clicked()),this,SLOT(LogMenu_Click()));
    connect(LogFilterButton,SIGNAL(clicked()),this,SLOT(LogFilter_Click()));
    connect(_LogFilter,SIGNAL(FilterChanged(QList<LogLevel>,int,QString,bool)),this,SLOT(FilterChanged(QList<LogLevel>,int,QString,bool)));



    compiler->SetReleaseFolder(Settings->value("ReleaseFolder","../../release").toString());
    compiler->SetBuildFolder(Settings->value("BuildFolder",".").toString());
    /* Settings->setValue("Build","../build");*/


    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(Close()));
    connect(ui->actionOpenUrl,SIGNAL(triggered()),this,SLOT(OpenUrl()));
    connect(ui->actionDebug_Version,SIGNAL(triggered()),this,SLOT(ShowDebugVersion()));
    //connect(ui->actionUploadToGoogleDrive,SIGNAL(triggered()),this,SLOT(UploadToGoogleDrive()));
    connect(ui->actionBackup_Path,SIGNAL(triggered()),this,SLOT(ShowBackupPath()));
    connect(ui->actionRestoreOriginal,SIGNAL(triggered()),this,SLOT(ClearState()));
    connect(ui->actionRestoreOriginal,SIGNAL(triggered()),this,SLOT(RestoreState()));
    connect(ui->actionRestoreOriginal,SIGNAL(triggered()),_RecordProcessCommunication,SLOT(RestoreOriginalStage()));

    connect(ui->actionShow,SIGNAL(triggered()),this,SLOT(Show()));
    connect(ui->actionRun,SIGNAL(triggered()),this,SLOT(Run()));
    connect(ui->actionRecord,SIGNAL(triggered()),this,SLOT(Record()));
    connect(ui->RecordButton,SIGNAL(clicked()),this,SLOT(Record()));
    connect(ui->RunButton,SIGNAL(clicked()),this,SLOT(Run()));
    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(New()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(Open()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(Save()));
    connect(ui->actionSave_As,SIGNAL(triggered()),this,SLOT(SaveAs()));
    connect(ui->actionCompile,SIGNAL(triggered()),this,SLOT(Compile()));
    connect(ui->actionDocumentation,SIGNAL(triggered()),this,SLOT(Documentation()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionAbout_Engine, SIGNAL(triggered()), this, SLOT(AboutEngine()));
    connect(ui->actionLocalization, SIGNAL(triggered()), this, SLOT(Localization()));
    connect(ui->actionInterface_language, SIGNAL(triggered()), this, SLOT(ChangeInterfaceLanguage()));
    connect(ui->actionLog_Location_Chooser,SIGNAL(triggered()),this,SLOT(ChooseLogLocation()));
    connect(ui->actionResourcesReport,SIGNAL(triggered()),this,SLOT(ResourcesReport()));
    connect(ui->actionShow_Database,SIGNAL(triggered()),this,SLOT(ShowDataBaseManager()));
    connect(ui->actionModules_Manager,SIGNAL(triggered()),this,SLOT(ShowModuleManager()));
    connect(ui->actionRegexp_Constructor,SIGNAL(triggered()),this,SLOT(RegexpConstructor()));
    connect(ui->actionFingerprint_Switcher,SIGNAL(triggered()),this,SLOT(FingerprintSwitcher()));
    connect(ui->actionStop,SIGNAL(triggered()),this,SLOT(Abort()));
    connect(ui->actionHTTP_Sniffer,SIGNAL(triggered()),_HttpSniffer,SLOT(Show()));
    connect(ui->actionScheduler,SIGNAL(triggered()),this,SLOT(RunScheduler()));



    connect(TrayNotifier,SIGNAL(Show()),this,SLOT(Show()));
    QMenu *Menu = new QMenu(this);
    Menu->addAction(ui->actionShow);
    Menu->addSeparator();
    Menu->addAction(ui->actionExit);
    TrayNotifier->SetContextmenu(Menu);

    tabifyDockWidget(ui->dockWidgetResult,ui->dockWidgetOutput);
    tabifyDockWidget(ui->dockWidgetResult,ui->dockWidgetScript);


    RestoreState();
    InitRecources();
    InitWidgets();
    InitAction();
    OpenDefault();
    //connect(ui->EditScript,SIGNAL(textChanged()),SLOT(Documentation()));
    SetNotModified();


    SetIsDirty(false);

    if(DatabaseId.isEmpty())
        DatabaseId = QString("Database.") + QString::number(qrand() % 100000);


    ui->Recents->Reload(CurrentFileName);
    ui->Recents->setVisible(true);

    {
        ProjectBackup *backup = new ProjectBackup(this);
        backup->SetPeriod(Settings->value("ProjectBackupPeriod",5 * 60000).toInt());
        backup->SetDestFolder(Settings->value("ProjectBackupDestFolder","../../projectbackups").toString());
        connect(backup,SIGNAL(Backup(QString)),this,SLOT(SaveToFileSilent(QString)));
        connect(_RecordProcessCommunication,SIGNAL(StartBackup()),backup,SLOT(StartBackup()));
        connect(backup,SIGNAL(BackupDone(QString)),_RecordProcessCommunication,SLOT(BackupDone(QString)));
        connect(this,SIGNAL(CurrentFileNameHasChanged(QString)),backup,SLOT(CurrentFileNameHasChanged(QString)));
        backup->CurrentFileNameHasChanged(CurrentFileName);
        backup->Start();
    }

    IsAutorun = false;
    int index = qApp->arguments().indexOf("--testing");
    if(index > 0 && qApp->arguments().length() - 1 >= index + 1)
    {
        AutorunPath = qApp->arguments()[index + 1];
        IsAutorun = true;

        OpenFromFileOrDisplayMessageBox(AutorunPath);
        Record();
    }

    if(qApp->arguments().indexOf("--autostart-record") >= 0)
    {
        Record();
    }

    WriteInfoOnStart();

    ui->dockWidgetResult->widget()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->dockWidgetResult->widget()->setMinimumHeight(160);

}

void MainWindow::RunScheduler()
{
    ui->actionScheduler->setIconText(tr("Scheduler ..."));
    ui->actionScheduler->setEnabled(false);

    SchedulerInstaller Installer;
    Installer.Run("AnyAndRecent",QFileInfo("settings.ini").absoluteFilePath());

    ui->actionScheduler->setIconText(tr("Scheduler"));
    ui->actionScheduler->setEnabled(true);
}


void MainWindow::InstallScheduler()
{
    ui->actionScheduler->setIconText(tr("Scheduler ..."));
    ui->actionScheduler->setEnabled(false);

    SchedulerInstaller Installer;
    Installer.Install();

    ui->actionScheduler->setIconText(tr("Scheduler"));
    ui->actionScheduler->setEnabled(true);
}

void MainWindow::UploadToGoogleDrive()
{
    if(!SavePrevious())
        return;
    UploadProject Dialog;
    connect(&Dialog,SIGNAL(Show()),this,SLOT(ShowInternal()));
    Dialog.SetCurrentProject(CurrentFileName);
    Dialog.Start();
    Dialog.exec();

}

void MainWindow::ShowBackupPath()
{
    QString Folder("../../projectbackups");
    QDir(Folder).mkpath(".");
    QDesktopServices::openUrl(QUrl::fromLocalFile(Folder));
}

void MainWindow::OpenUrl()
{
    if(!SavePrevious())
        return;

    DownloadProjectDialog Dialog;
    if(Dialog.exec() && Dialog.GetIsSuccess())
    {
        QString Project = Dialog.GetProject();
        OpenFromFileOrDisplayMessageBox(Project);
    }
}

void MainWindow::WriteInfoOnStart()
{
    QString PremiumString;
    VersionInfo info;
    #ifdef PREMIUM
        PremiumString = tr("premium");
    #else
        PremiumString = tr("free");
    #endif

    ui->EditOutput->setHtml(
                tr("<span style='color:gray'>You are using BrowserAutomationStudio version <span style='color:white'>%1</span> (<span style='color:white'>%2</span>).<br>Installation dir : <span style='color:white'>%3</span>.<br>This panel contains information about script execution, use 'Log' action to write data here.</span>")
                .arg(info.VersionString())
                .arg(PremiumString)
                .arg(QDir::current().absolutePath())
        );
}

void MainWindow::ShowScriptCode()
{
    TextEditor->GetWidget()->setVisible(true);
    TextEditor->Show();
    ui->ScriptLabel->setVisible(false);
}

void MainWindow::SetCurrentFileName(const QString& CurrentFileName)
{

    if(CurrentFileName != QDir::current().absoluteFilePath("project.xml"))
    {
        ui->Recents->OpenFile(CurrentFileName);
        ui->Recents->Reload(CurrentFileName);
    }
    this->CurrentFileName = CurrentFileName;
    emit CurrentFileNameHasChanged(CurrentFileName);
    UpdateTitle();
}

void MainWindow::SetModified()
{
    IsModified = true;
    UpdateTitle();
}

void MainWindow::SetNotModified()
{
    IsModified = false;
    UpdateTitle();
}

void MainWindow::UpdateTitle()
{
    QString IsModifiedString = QString( (IsModified) ? "*" : "");
    setWindowTitle(QFileInfo(CurrentFileName).fileName() + IsModifiedString + QString(" - Browser Automation Studio"));
}

void MainWindow::SetNeedRestart()
{
    NeedRestart = true;
}

void MainWindow::ShowDataBaseManager()
{
    DatabaseStateDialog dialog;
    connect(&dialog,SIGNAL(EditDatabase()),this,SLOT(ShowDataBaseDialogNoAsk()));
    dialog.SetHasDabase(_DataBaseConnector->HasDatabase());
    dialog.SetIsRemote(ConnectionIsRemote);
    dialog.SetConnectionServer(ConnectionServer);
    dialog.SetConnectionPort(ConnectionPort);
    dialog.SetConnectionLogin(ConnectionLogin);
    dialog.SetConnectionPassword(ConnectionPassword);
    dialog.SetSchema(Schema);
    dialog.SetIsDirty(IsDatabaseDirty);
    dialog.SetDatabaseId(DatabaseId);
    dialog.SetDatabaseBaseLocation(_DataBaseConnector->GetDatabaseBaseLocation());
    dialog.exec();
    if(dialog.GetIsSuccess())
    {
        Schema = dialog.GetSchema();
        DatabaseId = dialog.GetDatabaseId();
        ConnectionIsRemote = dialog.GetIsRemote();
        ConnectionServer = dialog.GetConnectionServer();
        ConnectionPort = dialog.GetConnectionPort();
        ConnectionLogin = dialog.GetConnectionLogin();
        ConnectionPassword = dialog.GetConnectionPassword();

        SetIsDirty(dialog.GetIsDirty());
        IsModified = true;
        UpdateTitle();
        if(dialog.GetNeedRestart())
        {
            SaveToFile(CurrentFileName);
            qApp->quit();
            QProcess::startDetached(QFileInfo( QCoreApplication::applicationFilePath() ).fileName(), QStringList()<<"--notasksingleinstance");
        }
    }
}

void MainWindow::ShowDataBase()
{
    if(!_DatabaseAdmin)
    {
        _DatabaseAdmin = new DatabaseAdmin();
        _DatabaseAdmin->SetDatabaseConnector(_DataBaseConnector);
    }
    _DatabaseAdmin->hide();
    _DatabaseAdmin->Show();

}

void MainWindow::ShowDataBaseDialogNoAsk()
{
    if(_DatabaseAdmin)
    {
        delete _DatabaseAdmin;
        _DatabaseAdmin = 0;
    }
    if(!_DatabaseAdmin)
    {
        _DatabaseAdmin = new DatabaseAdmin();
        _DatabaseAdmin->SetDatabaseConnector(_DataBaseConnector);
    }


    QDialog d;
    QHBoxLayout *l = new QHBoxLayout(&d);
    l->setMargin(0);
    d.setLayout(l);
    d.resize(_DatabaseAdmin->width(),_DatabaseAdmin->height());
    d.layout()->addWidget(_DatabaseAdmin);
    d.setWindowTitle(_DatabaseAdmin->windowTitle());
    d.setModal(true);

    _DatabaseAdmin->Show();
    d.exec();
    _DatabaseAdmin->setParent(0);

    _DatabaseAdmin->deleteLater();
    _DatabaseAdmin = 0;
}

void MainWindow::ShowDataBaseDialog()
{
    ShowDataBaseDialogNoAsk();
    if(Ask)
        Ask->show();

}

void MainWindow::ResourcesReport()
{
    ResVisualizer->ShowReport();
}

void MainWindow::ChangeInterfaceLanguage()
{
    ScriptLanguageChooserDialog LangDialog;

    LangDialog.SetLang(LanguageInterface);
    LangDialog.SetLanguageModel(LangModel);
    LangDialog.ChangesWillTakeEffectAfterRestart();
    if(LangDialog.exec())
    {
        LanguageInterface = LangDialog.GetLang();
        Settings->setValue("DefaultLanguageInterface",LanguageInterface);
        //TranslateEngine.Translate(LanguageInterface);
        //TranslateStudio.Translate(LanguageInterface);
        LangModel->ChangeDefaultLanguage(LanguageInterface);
    }
}

void MainWindow::ChooseLogLocation()
{
    LogLocationChooser Dialog;

    Dialog.SetLogLocation(LogLocation);

    if(Dialog.exec())
    {
        LogLocation = Dialog.GetLogLocation();
        Settings->setValue("LogLocation",LogLocation);
    }
}

void MainWindow::SetDefaultText()
{
    QFile f(":/studio/data/new.txt");
    if(f.open(QFile::ReadOnly | QFile::Text))
    {
        QString text = QString::fromUtf8(f.readAll());
        TextEditor->SetText(text);
        _DiffPatcher.Init(text);
    }
}

void MainWindow::LoadSchema()
{
    XmlResourceController loader;
    loader.SetFileName(CurrentFileName);
    Resources r;
    loader.FromViewToModel(&r);

    Schema = loader.GetSchema();
    DatabaseId = loader.GetDatabaseId();
    ConnectionIsRemote = loader.GetConnectionIsRemote();
    ConnectionServer = loader.GetConnectionServer();
    ConnectionPort = loader.GetConnectionPort();
    ConnectionLogin = loader.GetConnectionLogin();
    ConnectionPassword = loader.GetConnectionPassword();

    SetIsDirty(true);
}

void MainWindow::OpenDefault()
{
    QString filename = CurrentFileName;
    OpenFromFile(filename);
    if(!QFileInfo(filename).exists())
    {
        SetDefaultText();
    }

}

void MainWindow::ShowDebugVersion()
{
    QString Id;
    if(LanguageInterface != "en")
    {
        Id = LanguageInterface + QString(":");
    }
    QDesktopServices::openUrl(QUrl(QString("https://wiki.bablosoft.com/doku.php?id=") + Id + QString("debug_version")));
}

void MainWindow::Documentation()
{

    QString Server = Settings->value("Server","https://bablosoft.com/").toString();
    QDesktopServices::openUrl(QUrl(Server + "help"));
}

void MainWindow::Localization()
{
    LanguageChooserDevelopmentWidget dialog;
    dialog.SetLanguageModel(LangModel);
    dialog.exec();
}

void MainWindow::AboutEngine()
{
    QString Server = Settings->value("Server","https://bablosoft.com/").toString();
    VersionInfo info;
    #ifdef PREMIUM
        info.SetIsPremium();
    #endif
    info.SetServerName(Server);
    info.ShowAboutWindow(_BrowserVersionSelector->GetAllBrowserFullVersions());
}

void MainWindow::RegexpConstructor()
{
    QString Regexp = Settings->value("RegexpConstructor","https://bablosoft.github.io/RegexpConstructor/").toString();
    QDesktopServices::openUrl(QUrl(Regexp));
}

void MainWindow::FingerprintSwitcher()
{
    QString Regexp = Settings->value("FingerprintSwitcher","https://fingerprints.bablosoft.com/").toString();
    QDesktopServices::openUrl(QUrl(Regexp));
}

void MainWindow::ShowModuleManager()
{
    QString Script = TextEditor->GetText();
    _ModuleCreator->SetScript(Script);

    ModuleManagerWindow Window;
    Window.SetModuleManager(_ModuleManager);
    Window.SetModuleCreator(_ModuleCreator);
    Window.SetEmbeddedLanguageManager(_EmbeddedLanguageManager);
    Window.Reload();
    Window.exec();

    if(Window.IsProjectChnaged() || Window.GetIsModuleListChanged())
    {
        IsModified = true;
        UpdateTitle();
    }
}

void MainWindow::Compile()
{
    {
        CheckScript Check;
        if(!Check.Check(TextEditor->GetText()))
        {
            return;
        }
    }
    CompileWindow *compile = new CompileWindow(compiler);
    
    if(compile->exec())
    {
        CompileResults results;
        connect(&results,SIGNAL(CreateArchive()),this,SLOT(CreateArchive()));
        connect(&results,SIGNAL(SaveCurrentProject()),this,SLOT(SavePrevious()));
        results.Init(compiler,CurrentFileName,compile->GetIsOverrideRemote(),PremiumUsername,PremiumPassword);
        results.exec();
        PremiumUsername = results.GetPremiumUsername();
        PremiumPassword = results.GetPremiumPassword();
    }
    compile->deleteLater();
}

void MainWindow::CreateArchive()
{
    if(compiler->Compile())
    {
        XmlResourceController saver;
        QDir d(compiler->GetEngineFolder());
        /*if(QFileInfo("schema.xml").exists())
            QFile("schema.xml").copy(d.absoluteFilePath("schema.xml"));*/
        saver.SetFileName(d.absoluteFilePath("data/project.xml"));
        saver.SetScript(TextEditor->GetText());
        saver.SetEmbeddedData(_EmbeddedLanguageManager->SerializeData());
        saver.SetSchema(Schema);

        saver.SetConnectionIsRemote(ConnectionIsRemote);
        saver.SetConnectionServer(ConnectionServer);
        saver.SetConnectionPort(ConnectionPort);
        saver.SetConnectionLogin(ConnectionLogin);
        saver.SetConnectionPassword(ConnectionPassword);

        saver.SetScriptName(compiler->GetName());
        saver.SetProtectionStrength(compiler->GetProtectionStrength());
        saver.SetScriptIcon(compiler->GetIcon());
        saver.SetIsCustomIcon(compiler->GetIsCustomIcon());
        saver.SetHideBrowsers(compiler->GetHideBrowsers());
        saver.SetIntegrateScheduler(compiler->GetIntegrateScheduler());
        saver.SetSingleInstance(compiler->GetSingleInstance());
        saver.SetCompileType(compiler->GetTypeString());
        saver.SetIsEnginesInAppData(compiler->GetIsEnginesInAppData());
        saver.SetCopySilent(compiler->GetCopySilent());
        saver.SetDatabaseId(DatabaseId);
        saver.SetAvailableLanguages(LangModel->GetScriptAvailableLanguagesString());
        saver.SetScriptVersion(compiler->GetVersion());
        saver.SetModuleInfo(_ModuleCreator->GetModuleInfo());
        saver.SetOutputTitle1(Output->GetOutputTitle1());
        saver.SetOutputTitle2(Output->GetOutputTitle2());
        saver.SetOutputTitle3(Output->GetOutputTitle3());
        saver.SetOutputTitle4(Output->GetOutputTitle4());
        saver.SetOutputTitle5(Output->GetOutputTitle5());
        saver.SetOutputTitle6(Output->GetOutputTitle6());
        saver.SetOutputTitle7(Output->GetOutputTitle7());
        saver.SetOutputTitle8(Output->GetOutputTitle8());
        saver.SetOutputTitle9(Output->GetOutputTitle9());
        saver.SetOutputVisible1(Output->GetOutputVisible1());
        saver.SetOutputVisible2(Output->GetOutputVisible2());
        saver.SetOutputVisible3(Output->GetOutputVisible3());
        saver.SetOutputVisible4(Output->GetOutputVisible4());
        saver.SetOutputVisible5(Output->GetOutputVisible5());
        saver.SetOutputVisible6(Output->GetOutputVisible6());
        saver.SetOutputVisible7(Output->GetOutputVisible7());
        saver.SetOutputVisible8(Output->GetOutputVisible8());
        saver.SetOutputVisible9(Output->GetOutputVisible9());
        saver.SetUnusedModules(_ModuleManager->GetStandartModulesNotUsedInProject(TextEditor->GetText()));

        QList<IModuleManager::ModulePreserve> ModulesPreserve = _ModuleManager->GetModulesUsedInProject(TextEditor->GetText());
        _ModuleManager->PackModules(ModulesPreserve);

        saver.SetModulesPreserve(ModulesPreserve);


        {
            VersionInfo info;
            saver.SetEngineVersion(info.VersionString());
        }
        {
            QFile f("chrome_command_line.txt");
            if(f.exists() && f.open(QFile::ReadOnly | QFile::Text))
            {
                QTextStream in(&f);
                saver.SetChromeCommandLine(in.readAll());
            }
            f.close();
        }
        {
            QFile f("settings_worker.ini");
            if(f.exists() && f.open(QFile::ReadOnly | QFile::Text))
            {
                QTextStream in(&f);
                saver.SetSettingsWorker(in.readAll());
            }
            f.close();
        }
        {
            QFile f("modules/meta.json");
            if(f.exists() && f.open(QFile::ReadOnly | QFile::Text))
            {
                QTextStream in(&f);
                saver.SetModulesMetaJson(in.readAll());
            }
            f.close();
        }
        Res->FromViewToModel(WidgetController);
        Res->FromModelToView(&saver);
        if(!saver.GetResult())
        {
            QMessageBox::warning(0, tr("Error saving file"), QString(tr("Error saving file : %1")).arg(saver.GetErrorString()));
            return;
        }
    }
}


void MainWindow::Close()
{
    IsClosingWindow = true;
    SaveToFile(CurrentFileName);
    Settings->setValue("State",QString::fromUtf8(saveState().toBase64()));

    if(_WebInterface)
    {
        _WebInterface->Close();
    }
    if(Worker)
        Worker->Abort();
    if(_DatabaseAdmin)
        _DatabaseAdmin->close();
    close();
}

void MainWindow::CloseNoSave()
{
    IsClosingWindow = true;
    if(Worker)
        Worker->Abort();
    if(_DatabaseAdmin)
        _DatabaseAdmin->close();
    close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!IsClosingWindow)
    {
        hide();
        if(!AlreadyShowsMessage)
        {
            TrayNotifier->ShowMessage(tr("Application is still running"),tr("Use exit menu item to shut it down"));
            AlreadyShowsMessage = true;
        }
        event->ignore();
    }else
    {
        qApp->setQuitOnLastWindowClosed(true);
        qApp->closeAllWindows();
    }


}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG *msg = static_cast<MSG*>(message);
    if(msg->message == WM_ACTIVATE && msg->wParam == WA_ACTIVE)
    {
        //qDebug()<<"WM_ACTIVATE";
        Show();
        //QTimer::singleShot(1,this,SLOT(Show()));
    }
    return false;
}

void MainWindow::ClearState()
{
    Settings->remove("State");
    Settings->sync();
}

void MainWindow::RestoreState()
{
    if(Settings->contains("State"))
    {
        restoreState(QByteArray::fromBase64(Settings->value("State","").toString().toUtf8()));
    }else
    {
        QFile f(":/studio/data/state.txt");
        if (!f.open(QFile::ReadOnly | QFile::Text)) return;
        QTextStream in(&f);
        QByteArray b;
        b.append(in.readAll());
        restoreState(QByteArray::fromBase64(b));
    }
}

void MainWindow::Open()
{
    //qDebug()<<QString::fromUtf8(saveState().toBase64());
    if(!SavePrevious())
        return;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), "", tr("Project Files (*.xml);;All Files (*.*)"));

    if(fileName.length()>0)
    {
        OpenFromFileOrDisplayMessageBox(fileName);
    }
}

void MainWindow::Show()
{
    if(qApp->activeModalWidget())
        return;

    ShowInternal();
}

void MainWindow::ShowInternal()
{
    if(qApp->arguments().indexOf("--silent") >= 0)
    {
        hide();
        return;
    }
    if(!isVisible())
        show();

    QDateTime now = QDateTime::currentDateTime();
    if(LastShowWindow.isValid() && LastShowWindow.msecsTo(now) < 100)
        return;
    LastShowWindow = now;
    //qDebug()<<"Show";
    //BringWindowToTop((HWND)this->winId());
    SwitchToThisWindow((HWND)this->winId(),true);
    HWND hCurWnd = ::GetForegroundWindow();
    DWORD dwMyID = ::GetCurrentThreadId();
    DWORD dwCurID = ::GetWindowThreadProcessId(hCurWnd, NULL);
    ::AttachThreadInput(dwCurID, dwMyID, TRUE);
    ::SetWindowPos((HWND)this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    ::SetWindowPos((HWND)this->winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    ::SetForegroundWindow((HWND)this->winId());
    ::AttachThreadInput(dwCurID, dwMyID, FALSE);
    ::SetFocus((HWND)this->winId());
    ::SetActiveWindow((HWND)this->winId());
}

void MainWindow::OpenFromFileOrDisplayMessageBox(const QString& fileName)
{
    QString Result = OpenFromFile(fileName);
    if(!Result.isEmpty())
    {
        QMessageBox::warning(0, tr("Error loading file"), QString(tr("Error loading file : %1")).arg(Result));
    }else
    {
        if(_RecordProcessCommunication)
            _RecordProcessCommunication->StopRecorder();
        SetCurrentFileName(fileName);
        Settings->setValue("CurrentFileName",CurrentFileName);
    }
}

QString MainWindow::OpenFromFile(const QString& fileName)
{
    XmlResourceController loader;
    loader.SetFileName(fileName);

    Res->FromViewToModel(&loader);
    if(!loader.GetResult())
    {
        return loader.GetErrorString();
    }
    Res->FromModelToView(WidgetController);

    {
        QFile f("settings_worker.ini");
        if(f.open(QFile::WriteOnly | QFile::Text))
        {
            QTextStream out(&f);
            out<<loader.GetSettingsWorker();
        }
        f.close();
    }

    _EmbeddedLanguageManager->ReadLanguageList(true, false, false, _ModuleManager->GetAllEmbeddedLanguages(), _ModuleManager->GetAllEmbeddedModules());
    _EmbeddedLanguageManager->SetModuleCode(_ModuleManager->GetAllEmbeddedCodeItems());


    compiler->SetVersion(loader.GetScriptVersion());
    compiler->SetName(loader.GetScriptName());
    compiler->SetProtectionStrength(loader.GetProtectionStrength());
    compiler->SetIcon(loader.GetScriptIcon());
    compiler->SetIsCustomIcon(loader.GetIsCustomIcon());
    compiler->SetHideBrowsers(loader.GetHideBrowsers());
    compiler->SetIntegrateScheduler(loader.GetIntegrateScheduler());
    compiler->SetSingleInstance(loader.GetSingleInstance());
    compiler->SetCopySilent(loader.GetCopySilent());
    compiler->SetIsEnginesInAppData(loader.GetIsEnginesInAppData());
    compiler->SetTypeString(loader.GetCompileType());
    _ModuleCreator->SetModuleInfo(loader.GetModuleInfo());

    {
        QList<IModuleManager::ModulePreserve> Modules = loader.GetModulesPreserve();
        _ModuleManager->UnpackModules(Modules);
    }

    {
        _ModuleManager->ClearPreserveCache();
        QList<IModuleManager::ModulePreserve> Modules = _ModuleManager->GetModulesUsedInProject(loader.GetScript());
        _ModuleManager->PackModules(Modules);
        _ModuleManager->UnpackModules(Modules);
    }


    LangModel->SetScriptAvailableLanguagesString(loader.GetAvailableLanguages());

    Output->SetOutputTitle1(loader.GetOutputTitle1());
    Output->SetOutputTitle2(loader.GetOutputTitle2());
    Output->SetOutputTitle3(loader.GetOutputTitle3());
    Output->SetOutputTitle4(loader.GetOutputTitle4());
    Output->SetOutputTitle5(loader.GetOutputTitle5());
    Output->SetOutputTitle6(loader.GetOutputTitle6());
    Output->SetOutputTitle7(loader.GetOutputTitle7());
    Output->SetOutputTitle8(loader.GetOutputTitle8());
    Output->SetOutputTitle9(loader.GetOutputTitle9());

    Output->SetOutputVisible1(loader.GetOutputVisible1());
    Output->SetOutputVisible2(loader.GetOutputVisible2());
    Output->SetOutputVisible3(loader.GetOutputVisible3());
    Output->SetOutputVisible4(loader.GetOutputVisible4());
    Output->SetOutputVisible5(loader.GetOutputVisible5());
    Output->SetOutputVisible6(loader.GetOutputVisible6());
    Output->SetOutputVisible7(loader.GetOutputVisible7());
    Output->SetOutputVisible8(loader.GetOutputVisible8());
    Output->SetOutputVisible9(loader.GetOutputVisible9());

    Schema = loader.GetSchema();
    DatabaseId = loader.GetDatabaseId();
    ConnectionIsRemote = loader.GetConnectionIsRemote();
    ConnectionServer = loader.GetConnectionServer();
    ConnectionPort = loader.GetConnectionPort();
    ConnectionLogin = loader.GetConnectionLogin();
    ConnectionPassword = loader.GetConnectionPassword();
    ScriptEngineVersion = loader.GetEngineVersion();
    InterfaceState = loader.GetInterfaceState();

    SetIsDirty(!loader.GetSchema().isEmpty() || _DataBaseConnector->HasDatabase());

    TextEditor->SetText(loader.GetScript());
    _EmbeddedLanguageManager->ReadData(loader.GetEmbeddedData());
    _DiffPatcher.Init(loader.GetScript());
    if(!loader.GetResult())
    {
        return loader.GetErrorString();
    }
    SetNotModified();

    return QString();
}

void MainWindow::Save()
{

    if(CurrentFileName.size()>0)
    {
        SaveToFile(CurrentFileName);
    }else
    {
        SaveAs();
    }
}


void MainWindow::AbortInstant()
{
    if(Worker)
    {
        Worker->Abort();
    }
}

void MainWindow::Abort()
{
    if(Worker)
    {
        if(IsRecordLast)
        {
            Worker->Abort();
            return;
        }

        QStringList items;
        items << tr("Stop instant") << tr("Wait each thread");

        bool ok;
        QString item = QInputDialog::getItem(this, tr("Stop"), tr("Stop type"), items, 0, false, &ok);

        if(Worker)
        {
            if(ok && !item.isEmpty())
            {
                if(item == items[0])
                    Worker->Abort();
                else
                    Worker->AbortNotInstant();
            }
        }
    }
}

void MainWindow::AbortNotInstantInternal()
{
    if(Worker)
        Worker->AbortNotInstant();
}
void MainWindow::AbortInstantInternal()
{
    if(Worker)
        Worker->Abort();
}

QPair<bool,QString> MainWindow::SaveToFileSilent(const QString& file)
{
    QPair<bool,QString> res;
    res.first = false;

    if(!Output)
        return res;

    QList<IModuleManager::ModulePreserve> ModulesPreserve = _ModuleManager->GetModulesUsedInProject(TextEditor->GetText());
    if(_ModuleManager->PackModules(ModulesPreserve) && !_ModuleManager->GetIsRunning())
    {
        _ModuleManager->UnpackModules(ModulesPreserve);
    }


    XmlResourceController saver;
    saver.SetAvailableLanguages(LangModel->GetScriptAvailableLanguagesString());
    saver.SetFileName(file);
    saver.SetSchema(Schema);
    saver.SetDatabaseId(DatabaseId);
    saver.SetScript(TextEditor->GetText());
    saver.SetEmbeddedData(_EmbeddedLanguageManager->SerializeData());

    saver.SetScriptName(compiler->GetName());
    saver.SetProtectionStrength(compiler->GetProtectionStrength());
    saver.SetScriptIcon(compiler->GetIcon());
    saver.SetIsCustomIcon(compiler->GetIsCustomIcon());
    saver.SetHideBrowsers(compiler->GetHideBrowsers());
    saver.SetIntegrateScheduler(compiler->GetIntegrateScheduler());
    saver.SetSingleInstance(compiler->GetSingleInstance());
    saver.SetCompileType(compiler->GetTypeString());
    saver.SetIsEnginesInAppData(compiler->GetIsEnginesInAppData());
    saver.SetCopySilent(compiler->GetCopySilent());
    saver.SetScriptVersion(compiler->GetVersion());
    saver.SetModuleInfo(_ModuleCreator->GetModuleInfo());
    saver.SetConnectionIsRemote(ConnectionIsRemote);
    saver.SetConnectionServer(ConnectionServer);
    saver.SetConnectionPort(ConnectionPort);
    saver.SetConnectionLogin(ConnectionLogin);
    saver.SetConnectionPassword(ConnectionPassword);
    saver.SetInterfaceState(InterfaceState);
    saver.SetModulesPreserve(ModulesPreserve);
    saver.SetUnusedModules(_ModuleManager->GetStandartModulesNotUsedInProject(TextEditor->GetText()));


    saver.SetOutputTitle1(Output->GetOutputTitle1());
    saver.SetOutputTitle2(Output->GetOutputTitle2());
    saver.SetOutputTitle3(Output->GetOutputTitle3());
    saver.SetOutputTitle4(Output->GetOutputTitle4());
    saver.SetOutputTitle5(Output->GetOutputTitle5());
    saver.SetOutputTitle6(Output->GetOutputTitle6());
    saver.SetOutputTitle7(Output->GetOutputTitle7());
    saver.SetOutputTitle8(Output->GetOutputTitle8());
    saver.SetOutputTitle9(Output->GetOutputTitle9());
    saver.SetOutputVisible1(Output->GetOutputVisible1());
    saver.SetOutputVisible2(Output->GetOutputVisible2());
    saver.SetOutputVisible3(Output->GetOutputVisible3());
    saver.SetOutputVisible4(Output->GetOutputVisible4());
    saver.SetOutputVisible5(Output->GetOutputVisible5());
    saver.SetOutputVisible6(Output->GetOutputVisible6());
    saver.SetOutputVisible7(Output->GetOutputVisible7());
    saver.SetOutputVisible8(Output->GetOutputVisible8());
    saver.SetOutputVisible9(Output->GetOutputVisible9());

    {
        VersionInfo info;
        saver.SetEngineVersion(info.VersionString());
    }
    {
        QFile f("chrome_command_line.txt");
        if(f.exists() && f.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream in(&f);
            saver.SetChromeCommandLine(in.readAll());
        }
        f.close();
    }
    {
        QFile f("settings_worker.ini");
        if(f.exists() && f.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream in(&f);
            saver.SetSettingsWorker(in.readAll());
        }
        f.close();
    }
    {
        QFile f("modules/meta.json");
        if(f.exists() && f.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream in(&f);
            saver.SetModulesMetaJson(in.readAll());
        }
        f.close();
    }


    Res->FromViewToModel(WidgetController);
    Res->FromModelToView(&saver);
    res.first = saver.GetResult();
    res.second = saver.GetErrorString();

    return res;
}

void MainWindow::SaveToFile(const QString& file)
{
    QPair<bool,QString> res = SaveToFileSilent(file);
    if(!res.first)
    {
        QMessageBox::warning(0, tr("Error saving file"), QString(tr("Error saving file : %1")).arg(res.second));
    }else
    {
        SetCurrentFileName(file);
        Settings->setValue("CurrentFileName",CurrentFileName);
        SetNotModified();
    }
}

void MainWindow::SaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project"), "", tr("Project Files (*.xml);;All Files (*.*)"));
    if(fileName.length()>0)
        SaveToFile(fileName);
}


bool MainWindow::SavePrevious()
{
    SaveToFile(CurrentFileName);
    return true;
}



void MainWindow::New()
{
    if(!SavePrevious())
        return;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Create Project"), "", tr("Project Files (*.xml);;All Files (*.*)"));
    if(fileName.length()>0)
    {
        if(_RecordProcessCommunication)
            _RecordProcessCommunication->StopRecorder();
        {
            QFile f("settings_worker.ini");
            f.remove();
        }

        _EmbeddedLanguageManager->ReadLanguageList(true, false, false, _ModuleManager->GetAllEmbeddedLanguages(), _ModuleManager->GetAllEmbeddedModules());
        _EmbeddedLanguageManager->SetModuleCode(QList<EmbeddedCodeItem>());
        _EmbeddedLanguageManager->ReadData("[]");


        Output->SetDefaults();
        TextEditor->Clear();
        SetDefaultText();
        WidgetController->DeleteAllView();
        SetCurrentFileName(fileName);
        Settings->setValue("CurrentFileName",CurrentFileName);
        InterfaceState.clear();
        Schema.clear();
        ConnectionIsRemote = false;
        ConnectionServer.clear();
        ConnectionPort.clear();
        ConnectionLogin.clear();
        ConnectionPassword.clear();
        SetIsDirty(_DataBaseConnector->HasDatabase());
        DatabaseId = QString("Database.") + QString::number(qrand() % 100000);
        compiler->Reset();
        _ModuleCreator->SetModuleInfo(QString("{}"));

        {
            QList<IModuleManager::ModulePreserve> Modules;
            _ModuleManager->UnpackModules(Modules);
        }

        {
            VersionInfo info;
            ScriptEngineVersion = info.VersionString();
        }

        SavePrevious();
    }
}

void MainWindow::InitRecources()
{
    HttpClientFactory * _HttpClientFactoryTemp = new HttpClientFactory(this);
    connect(_HttpSniffer,SIGNAL(StateChanged(bool)),_HttpClientFactoryTemp,SLOT(SnifferStateChanged(bool)));
    connect(_HttpClientFactoryTemp,SIGNAL(NewSnifferReport(SnifferReport)),_HttpSniffer,SLOT(NewSnifferReport(SnifferReport)));
    _HttpClientFactory = _HttpClientFactoryTemp;
    _Pop3ClientFactory = new Pop3ClientFactory(this);
    _ImapClientFactory = new ImapClientFactory(this);
    _HtmlParserFactory = new HtmlParserFactory(this);
    _HelperFactory = new HelperFactory(this);

    Res = new Resources(this);
    StringBuild = new BrowserAutomationStudioFramework::StringBuilder(this);
    StringBuild->AddFromSettings(":/engine/text/strings.ini",QStringList()<<"RFemName"<<"TFemName"<<"RMaleName"<<"TMaleName"<<"RSurname"<<"TSurname");

    ScriptResources = new ScriptResourcesFromQt(this);

    EngineResController = new EngineResourceController(Res);
    TimeoutWaiterFactory *WaiterFactory = new TimeoutWaiterFactory(Res);
    WaiterFactory->SetMultiWorker(_MultiTimer);
    EngineResController->SetWaiterFactory(WaiterFactory);
    EngineResController->SetHttpClientFactory(_HttpClientFactory);
    EngineResController->SetStringBuilder(StringBuild);
    EngineResController->SetDatabaseConnector(_DataBaseConnector3);
    EngineResController->SetCsvHelper(_CsvHelper);


    QtResourceController * wc = new QtResourceController(Res);
    wc->SetLanguage(LanguageInterface);
    ConstructResourceWizard * wiz = new ConstructResourceWizard(wc);
    wiz->SetStringBuilder(StringBuild);
    wc->SetConstructResource(wiz);
    wc->SetUseUIConstructor();
    wc->SetLanguageModel(LangModel);
    wc->SetIncludeSections(false);
    ResourceDesignWidgetFactory *f = new ResourceDesignWidgetFactory(wc);
    f->SetDatabaseState(_DataBaseState);
    f->SetStringBuilder(StringBuild);
    wc->SetResourceWidgetFactory(f);
    connect(ui->ButtonAddNewResource,SIGNAL(clicked()),wc,SLOT(AddWidgetToView()));
    connect(wc,SIGNAL(WidgetsEmpty()),this,SLOT(ResourcesEmpty()));
    connect(wc,SIGNAL(WidgetsNotEmpty()),this,SLOT(ResourcesNotEmpty()));

    wc->SetWidget(ui->DataWidget);
    Res->FromModelToView(wc);
    WidgetController = wc;
}

void MainWindow::ResourcesEmpty()
{
    ui->DataWidget->setVisible(false);
    ui->SpacerResourcesBottom->changeSize(1,1, QSizePolicy::Fixed, QSizePolicy::Expanding);
    ui->SpacerResourcesTop->changeSize(1,1, QSizePolicy::Fixed, QSizePolicy::Expanding);
}

void MainWindow::ResourcesNotEmpty()
{
    ui->DataWidget->setVisible(true);
    ui->SpacerResourcesBottom->changeSize(0,0, QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->SpacerResourcesTop->changeSize(0,0, QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void MainWindow::ResourcesReportStateChanged(bool IsEmpty)
{
    if(ui->actionResourcesReport->isEnabled())
    {
        if(IsEmpty)
            ui->actionResourcesReport->setIcon(QIcon(":/studio/images/resources_error.png"));
        else
            ui->actionResourcesReport->setIcon(QIcon(":/studio/images/resources_ok.png"));
    }
}



void MainWindow::InitWidgets()
{
    Output = new OutputSection(this);


    Output->SetConfiguratorVisible(true);
    Output->SetLanguageModel(LangModel);

    ui->dockWidgetResult->setWidget(Output->GetWidget());
    Output->SetTooltipVisible(true);


    SolverFactory *fs = new SolverFactory(this);
    fs->SetHttpClientFactory(_HttpClientFactory);

    ReportData = new ScriptMultiWorkerReportData(this);
    DataVisualizer = new ReportDataVisualizer(this);
    ResVisualizer = new ResourcesVisualizer(this);
    ProfilerVisualizer = new ProfilerDataVisualizer(this);
    ProfilerVisualizer->SetProfilerData(_ProfilerData);
    connect(ResVisualizer,SIGNAL(ChangedState(bool)),this,SLOT(ResourcesReportStateChanged(bool)));


    connect(ui->actionShow_Report,SIGNAL(triggered()),DataVisualizer,SLOT(ShowReport()));
    connect(ui->actionShow_Profiler,SIGNAL(triggered()),ProfilerVisualizer,SLOT(ShowReport()));
    DataVisualizer->SetReportData(ReportData);
    connect(fs,SIGNAL(UsedSolver(QString)),ReportData,SLOT(CaptchaUsed(QString)));
    connect(fs,SIGNAL(FailedSolver(QString)),ReportData,SLOT(CaptchaFailed(QString)));


    ManualCaptchaSolver *cw = qobject_cast<ManualCaptchaSolver *>(fs->GetSolver("manual"));
    FactorySolver = fs;
    connect(fs,SIGNAL(UsedSolver(QString)),Output,SLOT(UsedSolver(QString)));
    connect(cw,SIGNAL(ChangedSize(int)),this,SLOT(UpdateCaptchaSize(int)));



}

void MainWindow::UpdateCaptchaSize(int size)
{
    if(!dockWidgetCaptcha)
    {
        ManualCaptchaSolver *cw = qobject_cast<ManualCaptchaSolver *>(FactorySolver->GetSolver("manual"));
        dockWidgetCaptcha = new QDockWidget(this);
        dockWidgetCaptcha->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
        tabifyDockWidget(ui->dockWidgetResult,dockWidgetCaptcha);
        QWidget *w = new QWidget(dockWidgetCaptcha);
        dockWidgetCaptcha->setWidget(w);

        QVBoxLayout *l = new QVBoxLayout(w);
        w->setLayout(l);

        l->addWidget(cw->GetWidget());

    }

    QString text = QString(tr("Captcha (%1)")).arg(size);
    dockWidgetCaptcha->setWindowTitle(text);
    _TabBlink->SetText(text.split(" ").first());
    if(size > 0)
        _TabBlink->Start();
    else
        _TabBlink->Stop();


}

void MainWindow::InitAction()
{
    ui->actionStop->setEnabled(false);
    movie = new QMovie(":/studio/images/loading.gif");
    movie->setParent(this);
    ui->LabelStatus->setVisible(false);
    ui->LabelStatusText->setVisible(false);
    ui->StartRecord->setVisible(false);

}

void MainWindow::StartAction()
{
    ui->VerticalSpacerRecordButtonUp->changeSize(0,0, QSizePolicy::Fixed, QSizePolicy::Fixed);
    if(!IsRecordLast)
        ui->verticalSpacer->changeSize(0,0, QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->RecordContent->setVisible(false);
    ui->Recents->setVisible(false);

    ui->actionRun->setEnabled(false);
    ui->actionRecord->setEnabled(false);
    ui->RecordButton->setEnabled(false);
    ui->RecordButton->setVisible(false);
    ui->RunButton->setEnabled(false);
    ui->RunButton->setVisible(false);
    ui->actionStop->setEnabled(true);
    ui->actionCompile->setEnabled(false);
    ui->LabelStatus->setVisible(true);
    if(IsRecordLast)
    {
        ui->CenterStatus->changeSize(1,1, QSizePolicy::Fixed, QSizePolicy::Expanding);
        ui->StatusLeft->changeSize(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);
        ui->StatusRight->changeSize(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);
        ui->StartRecord->setVisible(true);
        ui->LabelStatusText->setVisible(false);
    }else
    {
        ui->CenterStatus->changeSize(0,0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        ui->StatusLeft->changeSize(0,0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        ui->StatusRight->changeSize(0,0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        ui->LabelStatusText->setText(tr("Running project ..."));
        ui->StartRecord->setVisible(false);
        ui->LabelStatusText->setVisible(true);
    }
    ui->LabelStatusText->setAlignment(Qt::AlignRight);

    ui->LabelStatus->setMovie(movie);

    ui->dockWidgetResult->widget()->setMinimumHeight(130);
    ui->dockWidgetResult->widget()->setMaximumHeight(130);
    ui->dockWidgetOutput->widget()->setMaximumHeight(130);
    ui->dockWidgetScript->widget()->setMaximumHeight(130);

    QTimer::singleShot(1,this,SLOT(RestoreMaxHeight()));

    movie->start();
}
void MainWindow::RestoreMaxHeight()
{
    ui->dockWidgetResult->widget()->setMaximumHeight(10000);
    ui->dockWidgetOutput->widget()->setMaximumHeight(10000);
    ui->dockWidgetScript->widget()->setMaximumHeight(10000);
}

void MainWindow::StopAction()
{
    Worker->WaitForAllModuleFunctionsToFinish();
    _ModuleManager->StopAllDlls();
    ui->verticalSpacer->changeSize(1,1, QSizePolicy::Fixed, QSizePolicy::Expanding);
    ui->VerticalSpacerRecordButtonUp->changeSize(1,1, QSizePolicy::Fixed, QSizePolicy::Expanding);
    ui->RecordContent->setVisible(true);
    ui->dockWidgetResult->widget()->setMinimumHeight(160);

    ui->CenterStatus->changeSize(0,0, QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->StatusLeft->changeSize(0,0, QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->StatusRight->changeSize(0,0, QSizePolicy::Fixed, QSizePolicy::Fixed);
    {
        QLayoutItem* item;
        if ( ( item = ui->LayoutBrowserInner->takeAt( 0 ) ) != NULL )
        {
            item->widget()->hide();
        }
    }
    ui->Recents->setVisible(true);

    _RecordProcessCommunication->Closed();
    ResVisualizer->SetDisabled();
    ui->actionResourcesReport->setEnabled(false);
    ui->actionResourcesReport->setIcon(QIcon(":/studio/images/resources_ok.png"));
    Output->SetConfiguratorVisible(true);
    Output->SetTooltipVisible(true);
    ui->actionRun->setEnabled(true);
    ui->actionRecord->setEnabled(true);
    ui->RecordButton->setEnabled(true);
    ui->RecordButton->setVisible(true);
    ui->LabelBrowserHelper->setVisible(true);
    ui->LabelRunHelper->setVisible(true);
    ui->RunButton->setEnabled(true);
    ui->RunButton->setVisible(true);
    ui->actionStop->setEnabled(false);
    ui->actionCompile->setEnabled(true);
    ui->LabelStatus->setVisible(false);
    ui->LabelStatusText->setVisible(false);
    ui->StartRecord->setVisible(false);
    TrayNotifier->Stop();
    sender()->deleteLater();
    Worker = 0;
    if(_WebInterfaceApi && _WebInterface)
    {
        _WebInterfaceApi->Reset();
    }
    if(_WebInterface)
    {
        _WebInterface->deleteLater();
        _WebInterface = 0;
        _WebInterfaceApi = 0;
    }
    movie->stop();
    emit SyncResources();
    if(!IsClosingWindow && !IsRecordLast)
    {
        QDateTime CurrentDateTime = QDateTime::currentDateTime();
        QString Datestringfile = CurrentDateTime.toString("yyyy.MM.dd.hh.mm.ss");
        QString FileName = QDir::cleanPath(LogLocation + QDir::separator() + QString("bas_reports/%2/%1.txt").arg(Datestringfile).arg(QFileInfo(CurrentFileName).fileName().replace(QRegExp("\\s"),".").replace(QRegExp("[^a-zA-Z1-9]"),"")));
        DataVisualizer->WriteToFile(FileName);
        DataVisualizer->ShowReport();
    }

    if(IsRecordLast && NeedRestart)
        QTimer::singleShot(1, ui->actionRecord, SLOT(trigger()));
    else
    {
        QList<IModuleManager::ModulePreserve> ModulesPreserve = _ModuleManager->GetModulesUsedInProject(TextEditor->GetText());
        _ModuleManager->PackModules(ModulesPreserve);
        _ModuleManager->UnpackModules(ModulesPreserve);
    }
    if(qApp->arguments().indexOf("--silent") < 0)
    {
        this->show();
    }

}

void MainWindow::SendCode()
{
    QString Code = TextEditor->GetText();
    VersionInfo info;
    if(Code.isEmpty())
        Code = " ";

    _RecordProcessCommunication->SendCode(Code,_DataBaseState->ToJson(),_EmbeddedLanguageManager->SerializeData(),IsAutorun, InterfaceState, ScriptEngineVersion, info.VersionString());
    _RecordProcessCommunication->SendResources(LastResourceList);
    _RecordProcessCommunication->SetWindow(QString::number(ui->centralWidget->winId()));


    ScriptEngineVersion = info.VersionString();
}

void MainWindow::RecordWindowAttached()
{
    if(IsRecordLast)
    {
        ui->LabelStatusText->setText(tr("Project recorder has started. It should be visible in a second. Try to search in task bar if you don't see it."));
        ui->LabelStatusText->setAlignment(Qt::AlignHCenter);
        ui->LabelStatusText->setVisible(true);
        ui->StartRecord->setVisible(false);
        ui->LabelStatus->setVisible(false);
    }
}

void MainWindow::ReceiveInterface(const QString& Json)
{
    InterfaceState = Json;
}

void MainWindow::ReceiveCode(const QString& Code)
{
    TextEditor->SetText(_DiffPatcher.ApplyPatch(Code));
}

QString MainWindow::GetRandomString()
{
   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
   const int randomStringLength = 12;

   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString;
}

void MainWindow::InitBrowserList(const QString& WrokerType, bool IsRecord)
{
    if(BrowserFactory)
    {
        delete BrowserFactory;
        BrowserFactory = 0;
        QLayoutItem* item;
        while ( ( item = ui->LayoutBrowserInner->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
    }

    ui->LabelBrowserHelper->setVisible(false);
    ui->LabelRunHelper->setVisible(false);

    QString RecordId;
    if(IsRecord)
        RecordId = GetRandomString();
    {
        SubprocessBrowserFactory *sf = new SubprocessBrowserFactory(this);
        if(_WebInterface->HasWebInterface())
        {
            _WebInterfaceApi->SetBrowserFactory(sf);
        }
        if(IsRecord)
        {
            sf->SetRecordId(RecordId);
        }
        PCResourcesSmoothUsage *_PCResourcesSmoothUsage = new PCResourcesSmoothUsage(sf);
        sf->SetPCResourcesSmoothUsage(_PCResourcesSmoothUsage);
        sf->SetLanguage(LanguageInterface);

        if(ComplexLoggerWebDriver)
            ComplexLoggerWebDriver->Clear();

        ProcessComunicatorFactory *cf = new ProcessComunicatorFactory(sf);
        if(IsRecord)
        {
            connect(cf,SIGNAL(ProcessComunicatorCreated(IProcessComunicator*)),_RecordProcessCommunication,SLOT(InstallProcessComunicator(IProcessComunicator*)));
            connect(cf,SIGNAL(ProcessComunicatorCreated(IProcessComunicator*)),this,SLOT(SendCode()));
            cf->SetIsRecord();
            cf->SetRecordId(RecordId);
        }

        connect(cf,SIGNAL(WebDriverStart(IProcessComunicatorFactory*)),this,SLOT(WebDriverSetupLog(IProcessComunicatorFactory*)));


        QStringList params = QStringList()<<"%keyin%"<<"%keyout%"<<"%pid%";
        if(IsRecord)
            params += "record";
        if(_EmbeddedLanguageManager && _EmbeddedLanguageManager->IsRunWithoutLanguages())
        {
            params += "--NoEmbeddedLanguages";
            params += "true";
        }

        cf->SetCommandLineParamsForPipes(params);
        cf->SetHttpClientFactoryForWebDriver(_HttpClientFactory);

        sf->SetProcessComunicatorFactory(cf);
        sf->SetNetworkAccessManager(new SubprocessNetworkAccessManagerFactory(this));

        QWidget * w = sf->GetWidget(IsRecord);
        w->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding));
        ui->LayoutBrowserInner->insertWidget(0,w);

        BrowserFactory = sf;
    }
}

void MainWindow::WebDriverSetupLog(IProcessComunicatorFactory *Factory)
{
    if(ComplexLoggerWebDriver)
    {
        ComplexLoggerWebDriver->deleteLater();
        ComplexLoggerWebDriver = 0;
    }
    if(dockWidgetWebDriverLog)
    {
        dockWidgetWebDriverLog->deleteLater();
        dockWidgetWebDriverLog = 0;
    }
    if(_WebDriverLogFilter)
    {
        _WebDriverLogFilter->deleteLater();
        _WebDriverLogFilter = 0;
    }

    ComplexLoggerWebDriver = new ComplexLogger();
    FileLogger *FileLoggerWebDriver = new FileLogger(ComplexLoggerWebDriver);
    FileLoggerWebDriver->AddSpecialLineEnding();
    PlainTextLogger * PlainTextLoggerWebDriver = 0;
    if(IsRecordLast)
    {
        PlainTextLoggerWebDriver = new PlainTextLogger(ComplexLoggerWebDriver);
        ComplexLoggerWebDriver->AddLogger(PlainTextLoggerWebDriver);
    }

    ComplexLoggerWebDriver->AddLogger(FileLoggerWebDriver);
    QString FileName = QDir::cleanPath(QString("multilogin_log.txt"));
    ComplexLoggerWebDriver->SetFileName(FileName);

    if(IsRecordLast)
    {
        dockWidgetWebDriverLog = new QDockWidget(this);
        dockWidgetWebDriverLog->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
        tabifyDockWidget(ui->dockWidgetResult,dockWidgetWebDriverLog);
        QWidget *w = new QWidget(dockWidgetWebDriverLog);
        dockWidgetWebDriverLog->setWidget(w);


        QVBoxLayout *l = new QVBoxLayout(w);
        l->setMargin(0);
        w->setLayout(l);

        QTextBrowser * TextBrowser = new QTextBrowser(w);
        //TextBrowser->setWordWrapMode(QTextOption::NoWrap);
        TextBrowser->setReadOnly(true);
        TextBrowser->setAcceptRichText(true);
        l->addWidget(TextBrowser);

        PlainTextLoggerWebDriver->SetPlainTextElement(TextBrowser);
        PlainTextLoggerWebDriver->SetLogSyntax(new WebDriverLogSyntax(PlainTextLoggerWebDriver));


        QString text = tr("Multilogin log");
        dockWidgetWebDriverLog->setWindowTitle(text);

        QWidget *WebDriverLogButtonsContainer = new QWidget(TextBrowser);

        WebDriverLogButtonsContainer->setMaximumWidth(70);

        WebDriverLogFilterButton = new QPushButton(WebDriverLogButtonsContainer);
        WebDriverLogFilterButton->setIcon(QIcon(":/images/search.png"));
        WebDriverLogFilterButton->setMaximumWidth(30);
        WebDriverLogFilterButton->setMinimumHeight(30);
        WebDriverLogFilterButton->setVisible(true);

        LogWebDriverMenuButton = new QPushButton(WebDriverLogButtonsContainer);
        LogWebDriverMenuButton->setIcon(QIcon(":/images/menu2.png"));
        LogWebDriverMenuButton->setMaximumWidth(30);
        LogWebDriverMenuButton->setMinimumHeight(30);
        LogWebDriverMenuButton->setVisible(true);


        QHBoxLayout *LogButtonsLayout = new QHBoxLayout(WebDriverLogButtonsContainer);
        LogButtonsLayout->setMargin(0);
        LogButtonsLayout->setSpacing(10);
        LogButtonsLayout->setContentsMargins(0,0,0,0);
        WebDriverLogButtonsContainer->setLayout(LogButtonsLayout);
        LogButtonsLayout->addWidget(WebDriverLogFilterButton);
        LogButtonsLayout->addWidget(LogWebDriverMenuButton);


        TopRightPositioner * AllButtonPositioner = new TopRightPositioner(this);
        AllButtonPositioner->SetChild(WebDriverLogButtonsContainer);
        AllButtonPositioner->SetParent(dockWidgetWebDriverLog);
        AllButtonPositioner->SetMarginRight(20);
        AllButtonPositioner->SetMarginTop(30);
        AllButtonPositioner->Start();


        _WebDriverLogFilter = new WebDriverLogFilter(this);
        _WebDriverLogFilter->SetAvailableLevels(QList<LogLevel>()<<LogFail<<LogSuccess<<LogInfo<<LogUser);
        _WebDriverLogFilter->SetThreadsNumber(0);

        connect(_WebDriverLogFilter,SIGNAL(FilterChanged(QList<LogLevel>,int,QString,bool)),this,SLOT(WebDriverFilterChanged(QList<LogLevel>,int,QString,bool)));
        connect(_WebDriverLogFilter,SIGNAL(FilterChanged(QList<LogLevel>,int,QString,bool)),ComplexLoggerWebDriver,SLOT(Filter(QList<LogLevel>,int,QString,bool)));


        connect(LogWebDriverMenuButton,SIGNAL(clicked()),this,SLOT(LogWebDriverMenu_Click()));
        connect(WebDriverLogFilterButton,SIGNAL(clicked()),this,SLOT(LogWebDriverFilter_Click()));

        ComplexLoggerWebDriver->Clear();
    }


    Factory->SetLogger(ComplexLoggerWebDriver);
}

void MainWindow::AddedOutput1()
{
    Output->AddedOutput(0);
}

void MainWindow::AddedOutput2()
{
    Output->AddedOutput(1);
}

void MainWindow::AddedOutput3()
{
    Output->AddedOutput(2);
}

void MainWindow::AddedOutput4()
{
    Output->AddedOutput(3);
}

void MainWindow::AddedOutput5()
{
    Output->AddedOutput(4);
}

void MainWindow::AddedOutput6()
{
    Output->AddedOutput(5);
}

void MainWindow::AddedOutput7()
{
    Output->AddedOutput(6);
}

void MainWindow::AddedOutput8()
{
    Output->AddedOutput(7);
}

void MainWindow::AddedOutput9()
{
    Output->AddedOutput(8);
}

void MainWindow::ShowData()
{
    Ask->ClearContentWidget();

    //Load current model to dialog
    if(UserWidgetControllerPointer)
        delete UserWidgetControllerPointer;
    UserWidgetControllerPointer = new QtResourceController();
    AdvancedHider * Hider = new AdvancedHider(UserWidgetControllerPointer);
    Hider->SetAskUserForResourcesDialog(Ask);
    connect(UserWidgetControllerPointer,SIGNAL(NewResourceWidget(IResourceWidget*)),Hider,SLOT(InsertResourceWidget(IResourceWidget*)));
    connect(UserWidgetControllerPointer,SIGNAL(WidgetGenerationDone()),Hider,SLOT(HideAdvanced()));

    Ask->SetValidator(UserWidgetControllerPointer->GetValidator());
    UserWidgetControllerPointer->SetLanguageModel(LangModel);
    UserWidgetControllerPointer->SetLanguage(LanguageScript);

    UserWidgetControllerPointer->SetIncludeSections(true);

    ResourceUserWidgetFactory *f = new ResourceUserWidgetFactory(UserWidgetControllerPointer);
    f->SetStringBuilder(StringBuild);
    f->SetDatabaseState(_DataBaseState);

    UserWidgetControllerPointer->SetResourceWidgetFactory(f);
    UserWidgetControllerPointer->SetWidget(Ask->GetContentWidget());


    Res->FromModelToView(UserWidgetControllerPointer);


    Ask->GetContentWidget()->layout()->addWidget(AdditionalSettings->GetWidget());

}


void MainWindow::SetDefault()
{
    Res->FromViewToModel(WidgetController);
    ShowData();
}

void MainWindow::SaveActual(const QString& filename)
{
    XmlResourceController XmlActualStatic;
    XmlResourceController *XmlActual = &XmlActualStatic;
    XmlActual->SetFileName(filename);
    Resources UserRes;
    UserRes.FromViewToModel(UserWidgetControllerPointer);
    UserRes.FromModelToView(XmlActual);



    Resources ResDefaults;
    ResDefaults.FromViewToModel(WidgetController);
    XmlResourceController XmlDefaults;
    XmlDefaults.SetFileName(filename + ".default.xml");
    ResDefaults.FromModelToView(&XmlDefaults);

}

void MainWindow::LoadActual(const QString& filename)
{
    XmlResourceController XmlActualStatic;
    XmlResourceController *XmlActual = &XmlActualStatic;

    XmlActual->SetFileName(filename);

    Resources ResActual;
    ResActual.FromViewToModel(XmlActual);

    Resources ResDefault;
    XmlResourceController XmlDefaults;
    XmlDefaults.SetFileName(filename + ".default.xml");
    ResDefault.FromViewToModel(&XmlDefaults);

    CopyResourceController Copy;
    Copy.SetResources(&ResActual);
    Copy.SetDefaults(&ResDefault);

    Res->FromViewToModel(&Copy);
    ShowData();

}

void MainWindow::Record()
{
    _RecordProcessCommunication->OnRecord();
    {
        CheckScript Check;
        if(!Check.Check(TextEditor->GetText()))
        {
            return;
        }
    }
    IsRecordLast = true;
    _EmbeddedLanguageManager->Stop();
    _EmbeddedLanguageManager->ReadLanguageList(true,false, false, _ModuleManager->GetAllEmbeddedLanguages(), _ModuleManager->GetAllEmbeddedModules());
    _EmbeddedLanguageManager->SetModuleCode(_ModuleManager->GetAllEmbeddedCodeItems());
    _EmbeddedLanguageManager->SetIsRecord(IsRecordLast || Settings->value("ForceEmbeddedLanguagesLog", false).toBool());
    _EmbeddedLanguageManager->Start();
}

void MainWindow::Run()
{
    _RecordProcessCommunication->OnRun();
    {
        CheckScript Check;
        if(!Check.Check(TextEditor->GetText()))
        {
            return;
        }
    }
    IsRecordLast = false;
    _EmbeddedLanguageManager->Stop();
    QStringList ActiveModules;
    for(IModuleManager::ModulePreserve &Module: _ModuleManager->GetModulesUsedInProject(TextEditor->GetText(), true))
    {
        ActiveModules.append(Module->Name);
    }

    _EmbeddedLanguageManager->ReadLanguageList(true,false, false, _ModuleManager->GetAllEmbeddedLanguages(ActiveModules), _ModuleManager->GetAllEmbeddedModules(ActiveModules));
    _EmbeddedLanguageManager->SetModuleCode(_ModuleManager->GetAllEmbeddedCodeItems(ActiveModules));
    _EmbeddedLanguageManager->SetIsRecord(IsRecordLast || Settings->value("ForceEmbeddedLanguagesLog", false).toBool());
    _EmbeddedLanguageManager->Start();
}

void MainWindow::HighlightAction(QUrl url)
{
    if(url.scheme() == "file")
    {
        QFileInfo info(url.toString().replace("file:///",""));
        QDesktopServices::openUrl(QUrl::fromLocalFile(info.absoluteFilePath()));
        return;
    }
    if(url.scheme() == "disablehcaptcha")
    {
        if(Worker)
            Worker->Abort();
        if(ComplexLoggerLog)
            ComplexLoggerLog->Clear();
        _ModuleManager->SetModuleEnabled("HCaptcha", false);

        QMessageBox::information(0, tr(""), QString(tr("HCaptcha module has been disabled")));

        return;
    }
    if(url.scheme() == "disablerecaptcha")
    {
        if(Worker)
            Worker->Abort();
        if(ComplexLoggerLog)
            ComplexLoggerLog->Clear();
        _ModuleManager->SetModuleEnabled("ReCaptcha", false);

        QMessageBox::information(0, tr(""), QString(tr("ReCaptcha module has been disabled")));

        return;
    }
    if(url.scheme() == "disablefuncaptcha")
    {
        if(Worker)
            Worker->Abort();
        if(ComplexLoggerLog)
            ComplexLoggerLog->Clear();
        _ModuleManager->SetModuleEnabled("FunCaptcha", false);

        QMessageBox::information(0, tr(""), QString(tr("FunCaptcha module has been disabled")));

        return;
    }
    if(!IsRecordLast)
        return;
    QString ActionId = url.host().replace("action","");
    _RecordProcessCommunication->HighlightAction(ActionId);
}

bool MainWindow::IsProcessRunning(int pid)
{
    HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    bool exists = (process != NULL);
    CloseHandle(process);
    return exists;
}

void MainWindow::RemoveOldTunnels()
{
    QDirIterator it("t", QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    while (it.hasNext())
    {
        QString dir = it.next();
        QString pid = dir.split( QRegExp("[/\\\\]") ).last();
        if(!IsProcessRunning(pid.toInt()))
        {
            QDir(dir).removeRecursively();
        }
    }
}

void MainWindow::RunInternal()
{
    if(_EmbeddedLanguageManager->WasError())
        return;
    _ModuleManager->StartAllDlls();
    NeedRestart = false;
    bool IsRecord = IsRecordLast;

    RemoveOldTunnels();

    //Delete actual if defaults not exists
    {
        QDir dir(QCoreApplication::applicationDirPath());

        if(!QFile::exists(dir.absoluteFilePath("./default.xml")) && QFile::exists(dir.absoluteFilePath("./actual.xml")))
        {
            QFile(dir.absoluteFilePath("./actual.xml")).remove();
        }
    }

    //Init web interface
    if(_WebInterface)
    {
        _WebInterfaceApi = 0;
        _WebInterface->deleteLater();
        _WebInterface = 0;

    }

    IHttpClient *ClientForWebInterface = _HttpClientFactory->GetHttpClient(false);
    _WebInterface = new WebInterface(this);
    _WebInterfaceApi = new WebInterfaceApi(_WebInterface);
    _WebInterfaceApi->Init(_WebInterface);
    _WebInterfaceApi->SetMainWindow(this);
    ClientForWebInterface->setParent(_WebInterface);
    _WebInterface->SetTesingMode();
    _WebInterface->SetScriptName(compiler->GetName());
    _WebInterface->SetHttpClient(ClientForWebInterface);
    connect(_WebInterfaceApi,SIGNAL(ShowDatabase()),this,SLOT(ShowDataBase()));
    connect(_WebInterfaceApi,SIGNAL(ShowScheduler()),this,SLOT(RunScheduler()));
    connect(_WebInterfaceApi,SIGNAL(InstallScheduler()),this,SLOT(InstallScheduler()));
    _WebInterface->SetLastChoice(WebInterfaceIsRememberChoice,WebInterfaceRememberChoiceUseCustom,PremiumUsername,PremiumPassword);

    if(_WebInterface->Init(false))
    {
        QEventLoop loop;
        connect(_WebInterface, SIGNAL(Initialized()), &loop, SLOT(quit()));
        loop.exec();
    }

    WebInterfaceIsRememberChoice = _WebInterface->GetWebInterfaceIsRememberChoice();
    WebInterfaceRememberChoiceUseCustom = _WebInterface->GetWebInterfaceRememberChoiceUseCustom();
    PremiumUsername = _WebInterface->GetPremiumUsername();
    PremiumPassword = _WebInterface->GetPremiumPassword();


    if(_WebInterface->HasWebInterface())
    {
        _WebInterfaceApi->SetDatabaseState(_DataBaseState);
        _WebInterfaceApi->SetDatabaseConnector(_DataBaseConnector);
        _WebInterfaceApi->SetResourcesVisualizer(ResVisualizer);
        _WebInterfaceApi->SetReportDataVisualizer(DataVisualizer);
        connect(_WebInterfaceApi,SIGNAL(AbortInstant()),this,SLOT(AbortInstantInternal()));
        connect(_WebInterfaceApi,SIGNAL(AbortNotInstant()),this,SLOT(AbortNotInstantInternal()));
        connect(_WebInterface,SIGNAL(Exit()),this,SLOT(AbortInstantInternal()));
        connect(_WebInterfaceApi,SIGNAL(Restart()),this,SLOT(AbortInstantInternal()));


        WebInterfaceSolver *Solver = new WebInterfaceSolver(_WebInterface);
        Solver->SetWebIntrface(_WebInterface);
        FactorySolver->ReplaceSolver("manual",Solver);
    }else
    {
        FactorySolver->ReplaceSolver("manual",0);
    }



    //Load data from user interface
    Res->FromViewToModel(WidgetController);
    AdditionalSettingsWidgets AdditionalSettingsStatic;
    AdditionalSettings = &AdditionalSettingsStatic;
    AdditionalSettings->SetDefault(Settings->value("DefaultWorker").toString());



    EngineRes = new EngineResources(Res);
    connect(this,SIGNAL(SyncResources()),EngineRes,SLOT(SyncNotForce()));
    EngineResController->SetEngineResources(EngineRes);
    EngineRes->SetEngineResourcesController(EngineResController);
    EngineRes->SetResources(Res);

    XmlResourceController XmlActualStatic;
    XmlResourceController *XmlActual = &XmlActualStatic;
    //Load From File
    QDir dir(QCoreApplication::applicationDirPath());
    XmlActual->SetFileName(dir.absoluteFilePath("./actual.xml"));

    bool EmptyScript = false;

    if(_WebInterface->HasWebInterface())
    {
        UserWidgetControllerPointer = 0;

        //Update with actual settings
        {
            Resources ResActual;
            ResActual.FromViewToModel(XmlActual);

            Resources ResDefault;
            XmlResourceController XmlDefaults;
            QDir dir(QCoreApplication::applicationDirPath());
            XmlDefaults.SetFileName(dir.absoluteFilePath("./default.xml"));
            ResDefault.FromViewToModel(&XmlDefaults);

            CopyResourceController Copy;
            Copy.SetDefaults(&ResDefault);
            Copy.SetResources(&ResActual);
            Res->FromViewToModel(&Copy);
        }


        WebInterfaceResourceController WebController;

        Res->FromModelToView(&WebController);

        _WebInterface->Send("initialize",false, 0, "data", _WebInterface->GetHtmlInterface(), "resources", WebController.GetData(), "database", _WebInterfaceApi->GetDatabaseStructure());
        this->setEnabled(false);
        QVariantMap DataSelectedByUser = _WebInterface->WaitForMessage("accept_resources");
        EmptyScript = DataSelectedByUser.contains("-bas-empty-script-") && DataSelectedByUser.value("-bas-empty-script-").toBool();

        this->setEnabled(true);

        if(!_WebInterface->IsConnected())
        {
            EngineRes->deleteLater();
            return;
        }

        //Save defaults
        {
            Resources ResDefaults;
            ResDefaults.FromViewToModel(WidgetController);
            XmlResourceController XmlDefaults;
            QDir dir(QCoreApplication::applicationDirPath());
            XmlDefaults.SetFileName(dir.absoluteFilePath("./default.xml"));
            ResDefaults.FromModelToView(&XmlDefaults);
        }


        Settings->setValue("DefaultWorker",AdditionalSettings->ParseWidget());
        InitBrowserList(AdditionalSettings->ParseWidget(), IsRecord);

        //Load data to new model and then to engine resources
        WebController.SetData(DataSelectedByUser);
        Res->FromViewToModel(&WebController);
        Res->FromModelToView(XmlActual);
        Res->FromModelToView(EngineResController);

        //Populate LastResourceList
        LastResourceList.clear();
        QList<IResourceModel * > * List = Res->GetModelList();
        for(IResourceModel * Model: *List)
        {
            IRecordProcessCommunication::ResourceDescription desc;
            desc.Name = Model->GetName();
            desc.Description = Model->GetDescription().GetTranslation(LangModel->GetInterfaceLanguage());
            desc.Type = Model->GetTypeId();
            LastResourceList.append(desc);
        }

    }else
    {
        if(Res->NeedToFillByUser())
        {
            //Create dialog
            AskUserForResourcesDialog ask(Settings->value("AskUserForResourcesWidth", 900).toInt(),Settings->value("AskUserForResourcesHeight", 600).toInt());

            Ask = &ask;

            connect(Ask,SIGNAL(ShowDatabase()),this,SLOT(ShowDataBaseDialog()));
            ask.setModal(true);

            //Update with actual settings
            {
                Resources ResActual;
                ResActual.FromViewToModel(XmlActual);

                Resources ResDefault;
                XmlResourceController XmlDefaults;
                QDir dir(QCoreApplication::applicationDirPath());
                XmlDefaults.SetFileName(dir.absoluteFilePath("./default.xml"));
                ResDefault.FromViewToModel(&XmlDefaults);

                CopyResourceController Copy;
                Copy.SetDefaults(&ResDefault);
                Copy.SetResources(&ResActual);
                Res->FromViewToModel(&Copy);
            }

            ShowData();

            connect(Ask,SIGNAL(Default()),this,SLOT(SetDefault()));
            connect(Ask,SIGNAL(Save(QString)),this,SLOT(SaveActual(QString)));
            connect(Ask,SIGNAL(Load(QString)),this,SLOT(LoadActual(QString)));
            connect(Ask,SIGNAL(ShowLanguage()),this,SLOT(ChangeScriptLanguage()));
            connect(Ask,SIGNAL(ShowLogLocation()),this,SLOT(ChooseLogLocation()));

            if(!_DataBaseConnector->HasDatabase())
            {
                Ask->HideDatabaseButton();
            }
            //Ask user for input
            bool ask_exec_res = ask.exec();

            Settings->setValue("AskUserForResourcesWidth", ask.width());
            Settings->setValue("AskUserForResourcesHeight", ask.height());
            Settings->sync();

            if(!ask_exec_res)
            {
                EngineRes->deleteLater();
                delete UserWidgetControllerPointer;
                UserWidgetControllerPointer = 0;
                return;
            }

            //Save defaults
            {
                Resources ResDefaults;
                ResDefaults.FromViewToModel(WidgetController);
                XmlResourceController XmlDefaults;
                QDir dir(QCoreApplication::applicationDirPath());
                XmlDefaults.SetFileName(dir.absoluteFilePath("./default.xml"));
                ResDefaults.FromModelToView(&XmlDefaults);

            }

            Settings->setValue("DefaultWorker",AdditionalSettings->ParseWidget());
            InitBrowserList(AdditionalSettings->ParseWidget(), IsRecord);

            //Load data to new model and then to engine resources
            Resources UserRes;
            UserRes.FromViewToModel(UserWidgetControllerPointer);
            UserRes.FromModelToView(XmlActual);
            UserRes.FromModelToView(EngineResController);

            //Populate LastResourceList
            LastResourceList.clear();
            QList<IResourceModel * > * List = UserRes.GetModelList();
            for(IResourceModel * Model: *List)
            {
                IRecordProcessCommunication::ResourceDescription desc;
                desc.Name = Model->GetName();
                desc.Description = Model->GetDescription().GetTranslation(LangModel->GetInterfaceLanguage());
                desc.Type = Model->GetTypeId();
                LastResourceList.append(desc);
            }

            //Clean memory
            delete UserWidgetControllerPointer;
            UserWidgetControllerPointer = 0;
        }else
        {
            //Clear LastResourceList
            LastResourceList.clear();

            InitBrowserList(Settings->value("DefaultWorker","MultiProcessQt5").toString(), IsRecord);
            Res->FromModelToView(EngineResController);
        }
    }
    ResVisualizer->SetReportData(EngineResController->GetWatcherList());
    SaveToFile(CurrentFileName);

    //Set Gui

    StartAction();


    //Prepare Worker
    ScriptMultiWorker* worker = new ScriptMultiWorker(this);
    if(EmptyScript && !IsRecord)
        worker->SetEmptyProject();

    WorkerSettings *_WorkerSettings = new WorkerSettings(worker);
    _WorkerSettings->SetWorkerPathSafe(Settings->value("Worker5Safe",QVariant("./Worker/Worker.exe")).toString());
    _WorkerSettings->SetWorkerPathNotSafe(Settings->value("Worker5NotSafe",QVariant("./WorkerNotSafe/Worker.exe")).toString());
    {
        QSettings SettingsWorker("settings_worker.ini",QSettings::IniFormat);
        _WorkerSettings->ParseFromSettings(SettingsWorker);
    }
    worker->SetWorkerSettings(_WorkerSettings);

    worker->SetProjectPath(CurrentFileName);
    connect(_RecordProcessCommunication,SIGNAL(Interrupt()),worker,SLOT(InterruptAction()));
    connect(_RecordProcessCommunication,SIGNAL(MaximizeWindow()),this,SLOT(Show()));
    connect(_RecordProcessCommunication,SIGNAL(WindowAttached()),this,SLOT(RecordWindowAttached()));
    connect(_RecordProcessCommunication,SIGNAL(StartBackup()),this,SLOT(RecordWindowAttached()));
    if(IsRecord)
    {
        connect(worker,SIGNAL(RunTaskInRecordMode(int,QString,QString)),_RecordProcessCommunication,SLOT(RunTaskInRecordMode(int,QString,QString)));
        connect(_RecordProcessCommunication,SIGNAL(RunTaskResult(int,QString)),worker,SLOT(RunTaskResultScenario(int,QString)));

        connect(worker,SIGNAL(PrepareFunctionSignal(QString)),_RecordProcessCommunication,SLOT(PrepareFunction(QString)));
        connect(_RecordProcessCommunication,SIGNAL(PrepareFunctionResult(QString,QString)),worker,SIGNAL(PrepareFunctionResult(QString,QString)));

        connect(_RecordProcessCommunication,SIGNAL(SaveInterface(QString)),this,SLOT(ReceiveInterface(QString)));
    }

    worker->SetModuleManager(_ModuleManager);
    worker->SetProfilerData(_ProfilerData);
    _ProfilerData->Start();
    worker->SetStringBuilder(StringBuild);
    worker->SetEmbeddedLanguageManager(_EmbeddedLanguageManager);



    EngineRes->setParent(worker);

    Preprocessor * _Preprocessor = new Preprocessor(worker);
    _Preprocessor->SetIsRecord(IsRecord);
    _Preprocessor->SetEncryptor(new NoneEncryptor(_Preprocessor));
    worker->SetPreprocessor(_Preprocessor);

    worker->SetBrowserFactory(BrowserFactory);


    Output->Clear();
    Output->disconnect();

    connect(worker,SIGNAL(Success()),Output,SLOT(Success()));
    connect(worker,SIGNAL(Failed()),Output,SLOT(Failed()));
    connect(worker,SIGNAL(ProgressMaximum(int)),Output,SLOT(ProgressMaximum(int)));
    connect(worker,SIGNAL(ProgressValue(int)),Output,SLOT(ProgressValue(int)));
    connect(worker,SIGNAL(Finished()),Output,SLOT(Finished()));

    //Set Loggers
    QDateTime CurrentDateTime = QDateTime::currentDateTime();
    QString datestringfile = CurrentDateTime.toString("yyyy.MM.dd.hh.mm.ss");

    ComplexLogger *ComplexLoggerResults1 = new ComplexLogger(worker);
    FileLogger *FileLoggerResults1 = new FileLogger(ComplexLoggerResults1);
    QString FileName1 = QDir::cleanPath(LogLocation + QDir::separator() + QString("%2/%1.txt").arg(datestringfile).arg(Output->GetOutputTitle1().GetTranslation("en").replace(QRegExp("\\s"),".").replace(QRegExp("[^a-zA-Z1-9]"),"")));
    Output->SetFileName1(FileName1);
    FileLoggerResults1->SetFileName(FileName1);
    PlainTextLogger * PlainTextLoggerResults1 = new PlainTextLogger(ComplexLoggerResults1);
    PlainTextLoggerResults1->SetPlainTextElement(Output->GetOutput(0));

    if(_WebInterface->HasWebInterface())
    {
        WebInterfaceLogger *WebInterfaceLoggerResults1 = new WebInterfaceLogger(ComplexLoggerResults1);
        WebInterfaceLoggerResults1->SetWebInterface(_WebInterface);
        WebInterfaceLoggerResults1->SetResultIndex(0);
        ComplexLoggerResults1->AddLogger(WebInterfaceLoggerResults1);
    }

    ComplexLoggerResults1->AddLogger(FileLoggerResults1);
    ComplexLoggerResults1->AddLogger(PlainTextLoggerResults1);
    connect(ComplexLoggerResults1, SIGNAL(NewLine()), this, SLOT(AddedOutput1()));

    ComplexLogger *ComplexLoggerResults2 = new ComplexLogger(worker);
    FileLogger *FileLoggerResults2 = new FileLogger(ComplexLoggerResults2);
    QString FileName2 = QDir::cleanPath(LogLocation + QDir::separator() + QString("%2/%1.txt").arg(datestringfile).arg(Output->GetOutputTitle2().GetTranslation("en").replace(QRegExp("\\s"),".").replace(QRegExp("[^a-zA-Z1-9]"),"")));
    Output->SetFileName2(FileName2);
    FileLoggerResults2->SetFileName(FileName2);
    PlainTextLogger * PlainTextLoggerResults2 = new PlainTextLogger(ComplexLoggerResults2);
    PlainTextLoggerResults2->SetPlainTextElement(Output->GetOutput(1));
    if(_WebInterface->HasWebInterface())
    {
        WebInterfaceLogger *WebInterfaceLoggerResults2 = new WebInterfaceLogger(ComplexLoggerResults2);
        WebInterfaceLoggerResults2->SetWebInterface(_WebInterface);
        WebInterfaceLoggerResults2->SetResultIndex(1);
        ComplexLoggerResults2->AddLogger(WebInterfaceLoggerResults2);
    }
    ComplexLoggerResults2->AddLogger(FileLoggerResults2);
    ComplexLoggerResults2->AddLogger(PlainTextLoggerResults2);
    connect(ComplexLoggerResults2, SIGNAL(NewLine()), this, SLOT(AddedOutput2()));

    ComplexLogger *ComplexLoggerResults3 = new ComplexLogger(worker);
    FileLogger *FileLoggerResults3 = new FileLogger(ComplexLoggerResults3);
    QString FileName3 = QDir::cleanPath(LogLocation + QDir::separator() + QString("%2/%1.txt").arg(datestringfile).arg(Output->GetOutputTitle3().GetTranslation("en").replace(QRegExp("\\s"),".").replace(QRegExp("[^a-zA-Z1-9]"),"")));
    Output->SetFileName3(FileName3);
    FileLoggerResults3->SetFileName(FileName3);
    PlainTextLogger * PlainTextLoggerResults3 = new PlainTextLogger(ComplexLoggerResults3);
    PlainTextLoggerResults3->SetPlainTextElement(Output->GetOutput(2));
    if(_WebInterface->HasWebInterface())
    {
        WebInterfaceLogger *WebInterfaceLoggerResults3 = new WebInterfaceLogger(ComplexLoggerResults3);
        WebInterfaceLoggerResults3->SetWebInterface(_WebInterface);
        WebInterfaceLoggerResults3->SetResultIndex(2);
        ComplexLoggerResults3->AddLogger(WebInterfaceLoggerResults3);
    }
    ComplexLoggerResults3->AddLogger(FileLoggerResults3);
    ComplexLoggerResults3->AddLogger(PlainTextLoggerResults3);
    connect(ComplexLoggerResults3, SIGNAL(NewLine()), this, SLOT(AddedOutput3()));

    ComplexLogger *ComplexLoggerResults4 = new ComplexLogger(worker);
    FileLogger *FileLoggerResults4 = new FileLogger(ComplexLoggerResults4);
    QString FileName4 = QDir::cleanPath(LogLocation + QDir::separator() + QString("%2/%1.txt").arg(datestringfile).arg(Output->GetOutputTitle4().GetTranslation("en").replace(QRegExp("\\s"),".").replace(QRegExp("[^a-zA-Z1-9]"),"")));
    Output->SetFileName4(FileName4);
    FileLoggerResults4->SetFileName(FileName4);
    PlainTextLogger * PlainTextLoggerResults4 = new PlainTextLogger(ComplexLoggerResults4);
    PlainTextLoggerResults4->SetPlainTextElement(Output->GetOutput(3));
    if(_WebInterface->HasWebInterface())
    {
        WebInterfaceLogger *WebInterfaceLoggerResults4 = new WebInterfaceLogger(ComplexLoggerResults4);
        WebInterfaceLoggerResults4->SetWebInterface(_WebInterface);
        WebInterfaceLoggerResults4->SetResultIndex(3);
        ComplexLoggerResults4->AddLogger(WebInterfaceLoggerResults4);
    }
    ComplexLoggerResults4->AddLogger(FileLoggerResults4);
    ComplexLoggerResults4->AddLogger(PlainTextLoggerResults4);
    connect(ComplexLoggerResults4, SIGNAL(NewLine()), this, SLOT(AddedOutput4()));

    ComplexLogger *ComplexLoggerResults5 = new ComplexLogger(worker);
    FileLogger *FileLoggerResults5 = new FileLogger(ComplexLoggerResults5);
    QString FileName5 = QDir::cleanPath(LogLocation + QDir::separator() + QString("%2/%1.txt").arg(datestringfile).arg(Output->GetOutputTitle5().GetTranslation("en").replace(QRegExp("\\s"),".").replace(QRegExp("[^a-zA-Z1-9]"),"")));
    Output->SetFileName5(FileName5);
    FileLoggerResults5->SetFileName(FileName5);
    PlainTextLogger * PlainTextLoggerResults5 = new PlainTextLogger(ComplexLoggerResults5);
    PlainTextLoggerResults5->SetPlainTextElement(Output->GetOutput(4));
    if(_WebInterface->HasWebInterface())
    {
        WebInterfaceLogger *WebInterfaceLoggerResults5 = new WebInterfaceLogger(ComplexLoggerResults5);
        WebInterfaceLoggerResults5->SetWebInterface(_WebInterface);
        WebInterfaceLoggerResults5->SetResultIndex(4);
        ComplexLoggerResults5->AddLogger(WebInterfaceLoggerResults5);
    }
    ComplexLoggerResults5->AddLogger(FileLoggerResults5);
    ComplexLoggerResults5->AddLogger(PlainTextLoggerResults5);
    connect(ComplexLoggerResults5, SIGNAL(NewLine()), this, SLOT(AddedOutput5()));

    ComplexLogger *ComplexLoggerResults6 = new ComplexLogger(worker);
    FileLogger *FileLoggerResults6 = new FileLogger(ComplexLoggerResults6);
    QString FileName6 = QDir::cleanPath(LogLocation + QDir::separator() + QString("%2/%1.txt").arg(datestringfile).arg(Output->GetOutputTitle6().GetTranslation("en").replace(QRegExp("\\s"),".").replace(QRegExp("[^a-zA-Z1-9]"),"")));
    Output->SetFileName6(FileName6);
    FileLoggerResults6->SetFileName(FileName6);
    PlainTextLogger * PlainTextLoggerResults6 = new PlainTextLogger(ComplexLoggerResults6);
    PlainTextLoggerResults6->SetPlainTextElement(Output->GetOutput(5));
    if(_WebInterface->HasWebInterface())
    {
        WebInterfaceLogger *WebInterfaceLoggerResults6 = new WebInterfaceLogger(ComplexLoggerResults6);
        WebInterfaceLoggerResults6->SetWebInterface(_WebInterface);
        WebInterfaceLoggerResults6->SetResultIndex(5);
        ComplexLoggerResults6->AddLogger(WebInterfaceLoggerResults6);
    }
    ComplexLoggerResults6->AddLogger(FileLoggerResults6);
    ComplexLoggerResults6->AddLogger(PlainTextLoggerResults6);
    connect(ComplexLoggerResults6, SIGNAL(NewLine()), this, SLOT(AddedOutput6()));

    ComplexLogger *ComplexLoggerResults7 = new ComplexLogger(worker);
    FileLogger *FileLoggerResults7 = new FileLogger(ComplexLoggerResults7);
    QString FileName7 = QDir::cleanPath(LogLocation + QDir::separator() + QString("%2/%1.txt").arg(datestringfile).arg(Output->GetOutputTitle7().GetTranslation("en").replace(QRegExp("\\s"),".").replace(QRegExp("[^a-zA-Z1-9]"),"")));
    Output->SetFileName7(FileName7);
    FileLoggerResults7->SetFileName(FileName7);
    PlainTextLogger * PlainTextLoggerResults7 = new PlainTextLogger(ComplexLoggerResults7);
    PlainTextLoggerResults7->SetPlainTextElement(Output->GetOutput(6));
    if(_WebInterface->HasWebInterface())
    {
        WebInterfaceLogger *WebInterfaceLoggerResults7 = new WebInterfaceLogger(ComplexLoggerResults7);
        WebInterfaceLoggerResults7->SetWebInterface(_WebInterface);
        WebInterfaceLoggerResults7->SetResultIndex(6);
        ComplexLoggerResults7->AddLogger(WebInterfaceLoggerResults7);
    }
    ComplexLoggerResults7->AddLogger(FileLoggerResults7);
    ComplexLoggerResults7->AddLogger(PlainTextLoggerResults7);
    connect(ComplexLoggerResults7, SIGNAL(NewLine()), this, SLOT(AddedOutput7()));

    ComplexLogger *ComplexLoggerResults8 = new ComplexLogger(worker);
    FileLogger *FileLoggerResults8 = new FileLogger(ComplexLoggerResults8);
    QString FileName8 = QDir::cleanPath(LogLocation + QDir::separator() + QString("%2/%1.txt").arg(datestringfile).arg(Output->GetOutputTitle8().GetTranslation("en").replace(QRegExp("\\s"),".").replace(QRegExp("[^a-zA-Z1-9]"),"")));
    Output->SetFileName8(FileName8);
    FileLoggerResults8->SetFileName(FileName8);
    PlainTextLogger * PlainTextLoggerResults8 = new PlainTextLogger(ComplexLoggerResults8);
    PlainTextLoggerResults8->SetPlainTextElement(Output->GetOutput(7));
    if(_WebInterface->HasWebInterface())
    {
        WebInterfaceLogger *WebInterfaceLoggerResults8 = new WebInterfaceLogger(ComplexLoggerResults8);
        WebInterfaceLoggerResults8->SetWebInterface(_WebInterface);
        WebInterfaceLoggerResults8->SetResultIndex(7);
        ComplexLoggerResults8->AddLogger(WebInterfaceLoggerResults8);
    }
    ComplexLoggerResults8->AddLogger(FileLoggerResults8);
    ComplexLoggerResults8->AddLogger(PlainTextLoggerResults8);
    connect(ComplexLoggerResults8, SIGNAL(NewLine()), this, SLOT(AddedOutput8()));

    ComplexLogger *ComplexLoggerResults9 = new ComplexLogger(worker);
    FileLogger *FileLoggerResults9 = new FileLogger(ComplexLoggerResults9);
    QString FileName9 = QDir::cleanPath(LogLocation + QDir::separator() + QString("%2/%1.txt").arg(datestringfile).arg(Output->GetOutputTitle9().GetTranslation("en").replace(QRegExp("\\s"),".").replace(QRegExp("[^a-zA-Z1-9]"),"")));
    Output->SetFileName9(FileName9);
    FileLoggerResults9->SetFileName(FileName9);
    PlainTextLogger * PlainTextLoggerResults9 = new PlainTextLogger(ComplexLoggerResults9);
    PlainTextLoggerResults9->SetPlainTextElement(Output->GetOutput(8));
    if(_WebInterface->HasWebInterface())
    {
        WebInterfaceLogger *WebInterfaceLoggerResults9 = new WebInterfaceLogger(ComplexLoggerResults9);
        WebInterfaceLoggerResults9->SetWebInterface(_WebInterface);
        WebInterfaceLoggerResults9->SetResultIndex(8);
        ComplexLoggerResults9->AddLogger(WebInterfaceLoggerResults9);
    }
    ComplexLoggerResults9->AddLogger(FileLoggerResults9);
    ComplexLoggerResults9->AddLogger(PlainTextLoggerResults9);
    connect(ComplexLoggerResults9, SIGNAL(NewLine()), this, SLOT(AddedOutput9()));


    worker->SetResults1(ComplexLoggerResults1);
    worker->SetResults2(ComplexLoggerResults2);
    worker->SetResults3(ComplexLoggerResults3);
    worker->SetResults4(ComplexLoggerResults4);
    worker->SetResults5(ComplexLoggerResults5);
    worker->SetResults6(ComplexLoggerResults6);
    worker->SetResults7(ComplexLoggerResults7);
    worker->SetResults8(ComplexLoggerResults8);
    worker->SetResults9(ComplexLoggerResults9);

    ui->EditOutput->setHtml("");

    if(ComplexLoggerLog)
    {
        disconnect(_LogFilter,0,ComplexLoggerLog,0);
        ComplexLoggerLog->deleteLater();
    }
    ComplexLoggerLog = new ComplexLogger(this);
    FileLogger *FileLoggerLog = new FileLogger(ComplexLoggerLog);
    FileLoggerLog->AddSpecialLineEnding();
    LogFileName = QDir::cleanPath(LogLocation + QDir::separator() + QString("log/%1.txt").arg(datestringfile));
    LogMenuButton->setVisible(true);
    LogFilterButton->setVisible(true);
    PlainTextLogger * PlainTextLoggerLog = new PlainTextLogger(ComplexLoggerLog);
    if(IsRecord)
    {
        PlainTextLoggerLog->SetReplaceActionIdWithLink();
    }else
    {
        PlainTextLoggerLog->SetReplaceActionIdWithColor();
    }
    PlainTextLoggerLog->SetPlainTextElement(ui->EditOutput);

    if(_WebInterface->HasWebInterface())
    {
        WebInterfaceLogger *WebInterfaceLoggerLog = new WebInterfaceLogger(ComplexLoggerLog);
        WebInterfaceLoggerLog->SetWebInterface(_WebInterface);
        ComplexLoggerLog->AddLogger(WebInterfaceLoggerLog);
    }
    ComplexLoggerLog->AddLogger(FileLoggerLog);
    ComplexLoggerLog->AddLogger(PlainTextLoggerLog);
    ComplexLoggerLog->SetFileName(LogFileName);
    worker->SetLogger(ComplexLoggerLog);


    if(IsRecord)
    {
        QFile f(":/studio/data/default.txt");
        if(f.open(QFile::ReadOnly | QFile::Text))
        {
            worker->SetScript(_Preprocessor->Preprocess(QString::fromUtf8(f.readAll()),0,true));
        }
        f.close();
    }else
        worker->SetScript(_Preprocessor->Preprocess(TextEditor->GetText(),0,true));


    if(IsRecord)
    {
        //Use all modules in record mode
        QStringList Exclude;
        QList<QString> List = _ModuleManager->GetModuleEngineCode(Exclude);
        _ModuleManager->CacheBrowserCode();
        worker->SetAdditionEngineScripts(List);
    }
    else
    {
        QStringList Exclude = _ModuleManager->GetStandartModulesNotUsedInProject(TextEditor->GetText());
        QList<IModuleManager::ModulePreserve> ActiveModules = _ModuleManager->GetModulesUsedInProject(TextEditor->GetText());
        QList<QString> List = _ModuleManager->GetModuleEngineCode(ActiveModules, Exclude);
        _ModuleManager->CacheBrowserCode();
        worker->SetAdditionEngineScripts(List);
    }

    worker->SetScriptResources(ScriptResources);

    TimeoutWaiterFactory *WaiterFactory = new TimeoutWaiterFactory(worker);
    WaiterFactory->SetMultiWorker(_MultiTimer);
    worker->SetWaiterFactory(WaiterFactory);

    worker->SetSolverFactory(FactorySolver);

    worker->SetEngineResources(EngineRes);

    worker->SetResourceHandlersFactory(new ResourceHandlersFactory(worker));
    _HttpClientFactory->Sniff(IsRecord);
    worker->SetHttpClientFactory(_HttpClientFactory);
    worker->SetPop3ClientFactory(_Pop3ClientFactory);
    worker->SetImapClientFactory(_ImapClientFactory);
    worker->SetCsvHelper(_CsvHelper);
    worker->SetBrowserVersionSelector(_BrowserVersionSelector);
    worker->SetDatabaseConnector(_DataBaseConnector3);

    worker->SetScriptSuspender(new ScriptSuspender(worker));
    worker->SetWorkerFactory(new ScriptWorkerFactory(worker));

    connect(worker,SIGNAL(Finished()),this,SLOT(StopAction()));
    Worker = worker;
    if(_WebInterface->HasWebInterface())
        _WebInterfaceApi->SetWorker(Worker);


    //Prepare abort button
    //disconnect(ui->actionStop,SIGNAL(triggered()),0,0);
    //connect(ui->actionStop,SIGNAL(triggered()),worker,SLOT(Abort()));

    TrayNotifier->Start();

    Output->SetConfiguratorVisible(false);
    Output->SetTooltipVisible(true);

    worker->SetReportData(ReportData);

    worker->SetDoTrace(true);

    //Modules
    Worker->SetHtmlParserFactory(_HtmlParserFactory);
    Worker->SetHelperFactory(_HelperFactory);

    worker->AddModule(new MemoryInfo(worker),"MemoryInfo",true,true);
    /*worker->AddModule(new Helper(worker),"Helper",true,true);
    worker->AddModule(new HtmlParser(worker),"HtmlParser",true,true);*/

    ui->actionResourcesReport->setEnabled(true);
    ui->actionResourcesReport->setIcon(QIcon(":/studio/images/resources_ok.png"));
    ResVisualizer->SetEnabled();

    worker->SetProperties(new Properties(worker));

    _LogFilter->Clear();
    connect(worker,SIGNAL(ThreadNumberChanged(int)),_LogFilter,SLOT(SetThreadsNumber(int)));
    connect(_LogFilter,SIGNAL(FilterChanged(QList<LogLevel>,int,QString,bool)),ComplexLoggerLog,SLOT(Filter(QList<LogLevel>,int,QString,bool)));



    //Run Worker
    worker->SetIsRecord(IsRecord);
    worker->SetIsStudio(true);

    if(_WebInterface->HasWebInterface())
    {
       _WebInterfaceApi->ScriptStarted();
    }

    worker->Run();

}

void MainWindow::FilterChanged(QList<LogLevel>,int,QString,bool IsActive)
{
    QString image;
    if(IsActive)
        image = "_active";
    LogFilterButton->setIcon(QIcon(QString(":/images/search%1.png").arg(image)));
}

void MainWindow::WebDriverFilterChanged(QList<LogLevel>,int,QString,bool IsActive)
{
    QString image;
    if(IsActive)
        image = "_active";
    WebDriverLogFilterButton->setIcon(QIcon(QString(":/images/search%1.png").arg(image)));
}

void MainWindow::ChangeScriptLanguage()
{
    ScriptLanguageChooserDialog LangDialog;

    LangDialog.SetLang(LanguageScript);
    LangDialog.SetLanguageModel(LangModel);
    if(LangDialog.exec())
    {
        LanguageScript = LangDialog.GetLang();
        Settings->setValue("DefaultLanguageScript",LanguageScript);
        ShowData();
    }
}


MainWindow::~MainWindow()
{
    if(_HttpSniffer)
    {
        delete _HttpSniffer;
        _HttpSniffer = 0;
    }
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
        break;
    }
    case QEvent::ActivationChange:
    {
        if(this->isActiveWindow() && IsRDP)
        {
            SkinController * skin = new SkinController();
            skin->ApplyTheme("DarkFusion");
            skin->deleteLater();
        }
        break;
    }
    default:
        break;
    }
}

void MainWindow::LogFilter_Click()
{
    QPoint p = LogFilterButton->mapToGlobal( QPoint( 0,0 ) );
    _LogFilter->move(p.x() - _LogFilter->width() - 20,p.y() + 20);
    _LogFilter->open();
}

void MainWindow::LogWebDriverFilter_Click()
{
    QPoint p = WebDriverLogFilterButton->mapToGlobal( QPoint( 0,0 ) );
    _WebDriverLogFilter->move(p.x() - _WebDriverLogFilter->width() - 40,p.y() + 20);
    _WebDriverLogFilter->open();
}

void MainWindow::LogMenu_Click()
{

    QPoint globalPos = LogMenuButton->mapToGlobal(QPoint(0,LogMenuButton->height()));

    QMenu myMenu;
    myMenu.setStyleSheet("QMenu{background-color:#353535} QMenu::item:selected{color:black;background-color:#c663ff} ");
    QAction * AllLogAction = myMenu.addAction(QString(tr("All log")));
    QAction * ClearLogAction = myMenu.addAction(QString(tr("Clear log")));

    QAction *res = myMenu.exec(globalPos);
    if(AllLogAction == res)
    {
        QFileInfo info(LogFileName);
        QDesktopServices::openUrl(QUrl::fromLocalFile(info.absoluteFilePath()));
    }
    if(ClearLogAction == res)
    {
        if(ComplexLoggerLog)
            ComplexLoggerLog->Clear();
    }

}


void MainWindow::LogWebDriverMenu_Click()
{

    QPoint globalPos = LogWebDriverMenuButton->mapToGlobal(QPoint(0,LogWebDriverMenuButton->height()));

    QMenu myMenu;
    myMenu.setStyleSheet("QMenu{background-color:#353535} QMenu::item:selected{color:black;background-color:#c663ff} ");
    QAction * AllLogAction = myMenu.addAction(QString(tr("All log")));
    QAction * ClearLogAction = myMenu.addAction(QString(tr("Clear log")));

    QAction *res = myMenu.exec(globalPos);
    if(AllLogAction == res)
    {
        QFileInfo info("multilogin_log.txt");
        QDesktopServices::openUrl(QUrl::fromLocalFile(info.absoluteFilePath()));
    }
    if(ClearLogAction == res)
    {
        if(ComplexLoggerWebDriver)
            ComplexLoggerWebDriver->Clear();
    }

}



void MainWindow::on_ButtonAddNewResource_clicked()
{
    if(_RecordProcessCommunication)
        _RecordProcessCommunication->SendIsChanged();
    SetModified();
}

void MainWindow::SetIsDirty(bool IsDirty)
{
    IsDatabaseDirty = IsDirty;
    if(WidgetController)
        WidgetController->SetIsDatabaseDirty(IsDatabaseDirty);
    QString ActionName = tr("Show Database");
    if(IsDirty)
        ActionName += tr(" (Need Restart)");
    ui->actionShow_Database->setText(ActionName);
    if(IsDirty)
        ui->actionShow_Database->setIcon(QIcon(":/engine/images/databasewarning.png"));
    else
        ui->actionShow_Database->setIcon(QIcon(":/engine/images/database.png"));
}

