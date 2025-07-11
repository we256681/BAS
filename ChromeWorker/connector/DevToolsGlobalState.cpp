#include "DevToolsGlobalState.h"

void DevToolsGlobalState::Reset()
{
    TabId.clear();
    SwitchToTabId.clear();
    SwitchToTabFrameId.clear();
    SwitchToTabResetSavedActions = false;
    StartupScriptIds.clear();
    ExecutionContexts.clear();
    Tabs.clear();
    ScreenCastTabId.clear();
    OpenFileDialogResult.clear();
    OpenFileDialogIsManual = false;
    PromptResult.clear();
    CurrentDownloadFileName.clear();
    IsDownloading = false;
    IsDownloadsAllowed = true;
    IsPopupsAllowed = true;
    LoadNextTargetId.clear();
    LoadNextUrl.clear();
    LoadNext = false;
    LoadNextData.clear();
    BlockRequests.clear();
    CacheCapture.clear();
    CachedData.clear();
    CachedRequests.clear();
    LoadedUrls.clear();
    IsProxySet = false;
    IsUserAgentChanged = false;
    UserAgentData.clear();
    ExtensionList.clear();
    IsDeviceScaleFactorModified = false;
    HttpAuthEnabled = false;
    HttpAuthUserName.clear();
    HttpAuthPassword.clear();
    DragAndDropIsEnabled = false;
    DragAndDropData.clear();
}
