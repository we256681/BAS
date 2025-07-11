#include "noneconnector.h"
#include "json/picojson.h"
#include <fstream>

using namespace std::placeholders;

void NoneConnector::ResetProxy(const std::string& ParentProcessId)
{
    // Generate proxy data
    GlobalState.SaveProxy->WriteDirectConnectionConfig();
}

std::wstring NoneConnector::GetProxyConfigFolder()
{
    return GlobalState.SaveProxy->GetConfigFilePath(std::string());
}

void NoneConnector::Initialize(
    std::shared_ptr<ISimpleHttpClientFactory> SimpleHttpClientFactory,
    std::shared_ptr<IWebSocketClientFactory> WebSocketClientFactory,
    int Port, const std::string& UniqueProcessId, const std::string& ParentProcessId,
    const std::string& ConstantStartupScript,
    const std::vector<std::pair<std::string,std::string> >& CommandLineAdditional,
    const std::string& InitalProxy, int PcapPort
)
{
    GlobalState.SaveProxy.reset(new ProxySaver());
    GlobalState.SaveProxy->Initialize(ParentProcessId, PcapPort);

    GlobalState.Port = Port;
    GlobalState.UniqueProcessId = UniqueProcessId;
    GlobalState.ParentProcessId = ParentProcessId;
    GlobalState.ConstantStartupScript = ConstantStartupScript;
}

char* NoneConnector::GetPaintData()
{
    return 0;
}

int NoneConnector::GetPaintHeight()
{
    return 0;
}

int NoneConnector::GetPaintWidth()
{
    return 0;
}

char* NoneConnector::GetPaintDataScaled()
{
    return 0;
}

int NoneConnector::GetPaintHeightScaled()
{
    return 0;
}

int NoneConnector::GetPaintWidthScaled()
{
    return 0;
}

int NoneConnector::GetScrollX()
{
    return 0;
}

int NoneConnector::GetScrollY()
{
    return 0;
}

int NoneConnector::GetHeight()
{
    return 0;
}

int NoneConnector::GetWidth()
{
    return 0;
}

void NoneConnector::Timer()
{
    if (ConnectionState == Connected) return;

    for (auto f : OnBrowserCreated)
    {
        f();
    }

    ConnectionState = Connected;
}

void NoneConnector::SetProfilePath(const std::wstring& Path)
{
}

void NoneConnector::SetExtensionList(const std::vector<std::wstring>& Extensions)
{
}

bool NoneConnector::InterruptAction(int ActionUniqueId)
{
    return true;
}

void NoneConnector::StartProcess()
{
}

void NoneConnector::OpenDevTools()
{
}

void NoneConnector::InspectAt(int X, int Y)
{
}

Async NoneConnector::Reset(int Timeout)
{
    return ActionStub(Timeout);
}

bool NoneConnector::IsLoading()
{
    return false;
}

void NoneConnector::CloseBrowser()
{
}

Async NoneConnector::CreateTab(const std::string& Url, bool IsInstant, bool IsDelayed, const std::string& Referrer, int Timeout)
{
    if(!GlobalState.IsProxySet)
    {
        ResetProxy(GlobalState.ParentProcessId);
        GlobalState.IsProxySet = true;
    }
    return ActionStub(Timeout);
}

Async NoneConnector::SwitchToTab(int Index, int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::CloseTab(int Index, int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::GetTabsList(int Timeout)
{
    return ActionStub(Timeout);
}

int NoneConnector::GetCurrentTabIndex()
{
    return -1;
}

int NoneConnector::GetTabNumber()
{
    return 0;
}

Async NoneConnector::ResetDeviceScaleFactor(float DeviceScaleFactor, int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::Screenshot(int X, int Y, int Width, int Height, int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::StartScreenCast(int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::StopScreenCast(int Timeout)
{
    return ActionStub(Timeout);
}

void NoneConnector::DisableBackgroundMode()
{
}

void NoneConnector::EnableBackgroundMode()
{
}

Async NoneConnector::Load(const std::string& Url, bool IsInstant, const std::string& Referrer, int Timeout)
{
    if(!GlobalState.IsProxySet)
    {
        ResetProxy(GlobalState.ParentProcessId);
        GlobalState.IsProxySet = true;
    }
    return ActionStub(Timeout);
}

Async NoneConnector::Reload(bool IsInstant, int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::NavigateBack(bool IsInstant, int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::NavigateForward(bool IsInstant, int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::GetCurrentUrl(int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::SetRequestsRestrictions(const std::vector<std::pair<bool, std::string> >& Rules, int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::SetHttpAuth(const std::string& UserName, const std::string& Password, int Timeout)
{
    return ActionStub(Timeout);
}

void NoneConnector::SetCacheMasks(const std::vector<std::pair<bool, std::string> >& Rules)
{
}

std::string NoneConnector::GetSingleCacheData(const std::string& Mask, bool IsBase64)
{
    return std::string();
}

std::string NoneConnector::GetAllCacheData(const std::string& Mask)
{
    return std::string();
}

void NoneConnector::ClearNetworkData()
{
}

int NoneConnector::GetStatusForURL(const std::string& UrlPattern)
{
    return 0;
}

std::string NoneConnector::FindLoadedURL(const std::string& UrlPattern)
{
    return std::string();
}

bool NoneConnector::IsURLLoaded(const std::string& UrlPattern)
{
    return false;
}

Async NoneConnector::SetProxy(const std::string Server, int Port, bool IsHttp, const std::string Login, const std::string Password, int Timeout)
{
    GlobalState.IsProxySet = true;

    GlobalState.SaveProxy->WriteProxyConfig(Server, Port, IsHttp, Login, Password);

    return ActionStub(Timeout);
}

Async NoneConnector::GetHistory(int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::Sleep(int TimeMilliseconds, int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::SetHeaders(const std::vector<std::pair<std::string, std::string>>& Headers, int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::SetUserAgentData(const std::string& Data, const std::vector<std::pair<std::string, std::string>>& Headers, int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::GetBrowserSize(int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::ResizeBrowser(int Width, int Height, int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::SetStartupScript(const std::string& Script, int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::ExecuteJavascript(const std::string& Script, const std::string& Variables, const std::string& ElementPath, bool ScrollToElement, int Timeout)
{
    return ActionStub(Timeout);
}

void NoneConnector::Mouse(MouseEvent Event, int X, int Y, MouseButton Button, int MousePressed, int KeyboardPresses, int ClickCount)
{
}

void NoneConnector::Wheel(int X, int Y, bool IsUp, int Delta, int MousePressed, int KeyboardPresses)
{
}

Async NoneConnector::Touch(TouchEvent Event, int X, int Y, int Id, double RadiusX, double RadiusY, double RotationAngle, double Pressure, int Timeout)
{
    return ActionStub(Timeout);
}

void NoneConnector::Key(KeyEvent Event, const std::string& Char, int KeyboardPresses)
{
}

void NoneConnector::KeyRaw(KeyEvent Event, WPARAM WindowsVirtualKeyCode, LPARAM NativeVirtualKeyCode, int KeyboardPresses)
{
}

void NoneConnector::Focus()
{
}

Async NoneConnector::Inspect(int X, int Y, int Position, int Timeout)
{
    return ActionStub(Timeout);
}

void NoneConnector::SetOpenFileDialogResult(const std::string& Result)
{
}

void NoneConnector::SetOpenFileDialogManualMode(bool IsManual)
{
}

void NoneConnector::SetPromptResult(const std::string& PromptResult)
{
}

Async NoneConnector::RestrictDownloads(int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::AllowDownloads(int Timeout)
{
    return ActionStub(Timeout);
}

std::string NoneConnector::GetDownloadedFilePath()
{
    return std::string();
}

bool NoneConnector::IsFileDownloadReady()
{
    return true;
}

void NoneConnector::RestrictPopups()
{
}

void NoneConnector::AllowPopups()
{
}

Async NoneConnector::RestoreCookies(const std::string& Cookies, int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::SaveCookies(int Timeout)
{
    return ActionStub(Timeout);
}

void NoneConnector::TriggerExtensionButton(const std::string ExtensionIdOrNamePart)
{
}

std::vector<std::pair<std::string, std::string>> NoneConnector::GetExtensionList()
{
    return std::vector<std::pair<std::string, std::string>>{};
}

Async NoneConnector::StartDragFile(const std::string& Path, int Timeout)
{
    return ActionStub(Timeout);
}

Async NoneConnector::ActionStub(int Timeout)
{
    std::shared_ptr<IDevToolsAction> NewAction(ActionsFactory.Create("Empty", &GlobalState));
    NewAction->SetTimeout(Timeout);
    return NewAction->GetResult();
}
