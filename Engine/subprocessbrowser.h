#ifndef SUBPROCESSBROWSER_H
#define SUBPROCESSBROWSER_H
#include "engine_global.h"

#include "ibrowser.h"
#include "iprocesscomunicatorfactory.h"

namespace BrowserAutomationStudioFramework
{

    class ENGINESHARED_EXPORT SubprocessBrowser : public IBrowser
    {
        Q_OBJECT
        IWorker *Worker;
        IWorker *LastInjectedWorker;
        INetworkAccessManagerFactory *NetworkAccessManagerFactory;
        INetworkAccessManager *NetworkAccessManager;
        INetworkAccessManager *NetworkAccessManagerVirtual;
        IProcessComunicatorFactory *ProcessComunicatorFactory;
        IProcessComunicator *ProcessComunicator;
        IProcessComunicator *ProcessComunicatorVirtual;
        IPCResourcesSmoothUsage *PCResourcesSmoothUsage;
        QString NextAction;
        QString Language;
        QString RecordId;
        IWorkerSettings *WorkerSettings;
        bool ManualBrowserControl;
        qint64 LastPID;
        int BrowserId;
        qint64 ExecutionPointWhenAddingAction;

    public:

        explicit SubprocessBrowser(QObject *parent = 0);
        ~SubprocessBrowser();

    public slots:
        virtual void SetNetworkAccessManagerFactory(INetworkAccessManagerFactory *NetworkAccessManagerFactory);
        void SetProcessComunicatorFactory(IProcessComunicatorFactory *ProcessComunicatorFactory);
        void SetLanguage(const QString& Language);
        void SetRecordId(const QString& RecordId);
        virtual void LoadPage(const QString& url, const QString& callback);
        virtual void LoadPageInstant(const QString& url, const QString& callback);
        virtual void LoadPage2(const QString& url, const QString& referrer, bool IsInstant, const QString& callback);
        virtual void SetUserAgent(const QString& agent, const QString& callback);
        virtual void GetCookiesForUrl(const QString& url, const QString& callback);
        virtual void SaveCookies(const QString& callback);
        virtual void RestoreCookies(const QString& cookies, const QString& callback);
        virtual void RestoreLocalStorage(const QString& localstorage, const QString& callback);

        virtual IWebElement* GetRootElement();
        virtual void GetUrl(const QString& callback);
        virtual void GetBrowserScreenSettings(const QString& callback);
        virtual void CreateNewBrowser(bool ForseNewBrowserCreation, const QString& callback);
        void CreateNewVirtualBrowser(bool ForseNewBrowserCreation, const QString& callback);

        virtual void CloseBrowser();
        virtual void CloseVirtualBrowser();
        virtual void SetWorkerSettings(IWorkerSettings *WorkerSettings);
        virtual IWorkerSettings * GetWorkerSettings();

        virtual INetworkAccessManager* GetNetworkAccessManager();
        virtual INetworkAccessManager* GetNetworkAccessManagerVirtual();
        virtual void SetScriptResources(IScriptResources* ScriptResources);
        virtual IScriptResources* GetScriptResources();
        virtual void SetWorker(IWorker* Worker);
        virtual IWorker* GetWorker();
        virtual void DisableBrowser(const QString& callback);
        virtual void SetOpenFileName(const QString & OpenFileName, const QString& callback);
        virtual void DragFile(const QString & FileName, const QString& callback);
        virtual void SetStartupScript(const QString& script,const QString& script_id,const QString& target, const QString& callback);
        virtual void NavigateBack(bool IsInstant, const QString& callback);
        virtual void SetFontList(const QString& fonts, const QString& callback);
        virtual void SetPromptResult(const QString & Text, const QString& callback);
        virtual void SetHttpAuthResult(const QString & Login, const QString & Password, const QString& callback);
        virtual void MouseClickInstant(int x, int y);
        virtual void SetMode(const QString& mode, const QString& callback);
        virtual void MouseClick(int x, int y, const QString& callback);
        virtual void MouseClickUp(int x, int y, const QString& callback);
        virtual void MouseClickDown(int x, int y, const QString& callback);
        virtual void PopupClose(int index, const QString& callback);
        virtual void PopupSelect(int index, const QString& callback);
        virtual void PopupCreate(bool is_silent, const QString& url, const QString& callback);
        virtual void PopupCreate2(bool is_silent, const QString& url, const QString& referrer, bool is_instant, const QString& callback);
        virtual void SetComboboxIndex(int index, const QString& callback);
        virtual void PopupInfo(const QString& callback);
        virtual void Timezone(int offset, const QString& callback);
        virtual void MouseMove(int x, int y,const QString& params, const QString& callback);
        virtual void Resize(int x, int y, const QString& callback);
        virtual void SetDeviceScaleFactor(double scale,const QString& callback);
        virtual void StartManualBrowserControl(const QString& message, const QString& callback);
        virtual void Jquery(const QString& callback);
        virtual void OptimizeMemory(const QString& callback);
        virtual void WaitCode(const QString& callback);
        virtual void StartSection(const QString& Name, int Id, const QString& callback);
        virtual void Scroll(int x, int y, const QString& callback);
        virtual void Render(int x, int y, int width, int height, const QString& callback);
        virtual void RequestVariablesResult(const QString& data, const QString& callback);
        virtual void DebugVariablesResult(const QString& data, const QString& callback);
        virtual void DebugCallstackResult(const QString& data, const QString& callback);
        virtual void SendRecaptchaV3List(const QString& action, const QString& callback);
        virtual void SendRecaptchaV3Result(const QString& id,const QString& result);
        virtual void SendWorkerSettings(const QString& json, const QString& callback);
        virtual void ClickExtensionButton(const QString& id, const QString& callback);
        virtual void SetPCResourcesSmoothUsage(IPCResourcesSmoothUsage *PCResourcesSmoothUsage);
        virtual bool HasBrowserProcess();
        virtual QString GetBrowserEngine();

        virtual void ClearImageData(const QString& callback);
        virtual void SetImageData(const QString& base64, const QString& callback);
        virtual void FindImage(const QString& callback);

        void MarkBeforeReset();
        void MarkAfterReset();

        void SetManualBrowserControl();
        bool GetManualBrowserControl();

        void ResetSettings();
        QString GetBrowserProcessId();
        QString GetBrowserUniqueId();

        void ResetProxy();
        bool IsBASBrowser();
        bool IsBASBrowserVirtual();
        bool NeedToCreateVirtualProcessCommunicator();
        QString GetProfilePath();
        QString GetCurrentBrowserVersionInfo(const QString& Format);
        QString GetAllBrowserVersionInfo(const QString& Format);
        int FindBrowserVersionId(const QString& VersionString);
        bool IsTemporaryProfile();

        virtual void OnSupend();
        virtual void OnSuspendVirtual();
        virtual void SimulateCrush(const QString& callback);

        //Online profiles
        void RemoveOnlineProfile(const QString& ProfileId, const QString& callback);
        void RemoveLocalProfile(const QString& ProfileId, const QString& callback);
        void FindAllOnlineProfiles(const QString& SearchPattern, const QString& callback);

        qint64 GetCurrentExecutionPointWhenAddingAction();
    signals:
        void ProcessCreated(IProcessComunicator *Communicator);
        void Loaded();
        void Loaded2();
        void LoadedInstant();
        void GetUrl();
        void GetBrowserScreenSettings();
        void SetUserAgent();
        void GetCookiesForUrl();
        void SaveCookies();
        void RestoreLocalStorage();
        void RestoreCookies();
        void SetOpenFileName();
        void NavigateBack();
        void DisableBrowser();
        void SetStartupScript();
        void SetFontList();
        void SetHttpAuthResult();
        void SetPromptResult();
        void SetMode();
        void MouseClick();
        void MouseClickUp();
        void MouseClickDown();
        void PopupSelect();
        void PopupCreate();
        void PopupCreate2();
        void SetComboboxIndex();
        void PopupClose();
        void PopupInfo();
        void MouseMove();
        void Resize();
        void SetDeviceScaleFactor();
        void Reset();
        void Scroll();
        void Render();
        void Jquery();
        void OptimizeMemory();
        void WaitCode();
        void RequestVariablesResult();
        void DebugVariablesResult();
        void DebugCallstackResult();
        void RecaptchaV3List();
        void Timezone();
        void Crush();
        void ClearImageData();
        void SetImageData();
        void FindImage();
        void SendWorkerSettings();
        void ClickExtensionButton();
        void DragFile();
        void StartManualBrowserControl();
        void CheckSystemResourcesBeforeBrowserCreate(int BrowserId);
        void ResourcesCheckedThis();
        void BrowserStarted(int BrowserId);
        void RemoveOnlineProfile();
        void RemoveLocalProfile();
        void FindAllOnlineProfiles();
        void SwitchToOnlineProfile();


    private slots:
        void Received(const QString& value);
        void WorkerStopped();
        void StartTunnel();
        void AfterVirtualBrowserCreated();
        void ClearLastTunnelFolder();
        void ResourcesCheckedCommon(int BrowserId);
        void AfterResourcesChecked();
    };
}
#endif // SUBPROCESSBROWSER_H

