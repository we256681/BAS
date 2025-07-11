#ifndef NONECONNECTOR_H
#define NONECONNECTOR_H

#include "idevtoolsconnector.h"
#include "IWebSocketClientFactory.h"
#include "ISimpleHttpClientFactory.h"
#include "InputEventsEnumerations.h"
#include "KeyboardEmulation.h"
#include <windows.h>

class NoneConnector : public IDevToolsConnector
{
    private:
        // State
        DevToolsGlobalState GlobalState;

        // Factory
        DevToolsActionFactory ActionsFactory;

        Async ActionStub(int Timeout = -1);

        void ResetProxy(const std::string& ParentProcessId);
    public:
        // Paint
        char* GetPaintData();
        int GetPaintHeight();
        int GetPaintWidth();

        char* GetPaintDataScaled();
        int GetPaintHeightScaled();
        int GetPaintWidthScaled();

        // Scroll
        int GetScrollX();
        int GetScrollY();

        // Viewport
        int GetHeight();
        int GetWidth();

        void Timer();

        void Initialize
        (
                std::shared_ptr<ISimpleHttpClientFactory> SimpleHttpClientFactory,
                std::shared_ptr<IWebSocketClientFactory> WebSocketClientFactory,
                int Port, const std::string& UniqueProcessId, const std::string& ParentProcessId,
                const std::string& ConstantStartupScript,
                const std::vector<std::pair<std::string,std::string> >& CommandLineAdditional,
                const std::string& InitalProxy, int PcapPort
        );
        void SetProfilePath(const std::wstring& Path);
        void SetExtensionList(const std::vector<std::wstring>& Extensions);
        bool InterruptAction(int ActionUniqueId);
        void StartProcess();
        void OpenDevTools();
        void InspectAt(int X, int Y);

        std::wstring GetProxyConfigFolder();

        // Browser life cycle
        Async Reset(int Timeout = -1);
        bool IsLoading();
        void CloseBrowser();

        // Tabs
        Async CreateTab(const std::string& Url, bool IsInstant = false, bool IsDelayed = false, const std::string& Referrer = std::string(), int Timeout = -1);
        Async SwitchToTab(int Index, int Timeout = -1);
        Async CloseTab(int Index, int Timeout = -1);
        Async GetTabsList(int Timeout = -1);
        int GetCurrentTabIndex();
        int GetTabNumber();

        // Rendering
        Async ResetDeviceScaleFactor(float DeviceScaleFactor, int Timeout = -1);
        Async Screenshot(int X, int Y, int Width, int Height, int Timeout = -1);
        Async StartScreenCast(int Timeout = -1);
        Async StopScreenCast(int Timeout = -1);
        void DisableBackgroundMode();
        void EnableBackgroundMode();

        // Network
        Async Load(const std::string& Url, bool IsInstant = false, const std::string& Referrer = std::string(), int Timeout = -1);
        Async Reload(bool IsInstant = false, int Timeout = -1);
        Async NavigateBack(bool IsInstant = false, int Timeout = -1);
        Async NavigateForward(bool IsInstant = false, int Timeout = -1);
        Async GetCurrentUrl(int Timeout = -1);
        Async SetRequestsRestrictions(const std::vector<std::pair<bool, std::string> >& Rules, int Timeout = -1);
        Async SetHttpAuth(const std::string& UserName, const std::string& Password, int Timeout = -1);

        // Cache
        void SetCacheMasks(const std::vector<std::pair<bool, std::string> >& Rules);
        std::string GetSingleCacheData(const std::string& Mask, bool IsBase64);
        std::string GetAllCacheData(const std::string& Mask);
        void ClearNetworkData();

        int GetStatusForURL(const std::string& UrlPattern);
        std::string FindLoadedURL(const std::string& UrlPattern);
        bool IsURLLoaded(const std::string& UrlPattern);
        Async SetProxy(const std::string Server, int Port, bool IsHttp = true, const std::string Login = std::string(), const std::string Password = std::string(), int Timeout = -1);
        Async GetHistory(int Timeout = -1);
        Async SetHeaders(const std::vector<std::pair<std::string, std::string>>& Headers, int Timeout = -1);
        Async SetUserAgentData(const std::string& Data, const std::vector<std::pair<std::string, std::string>>& Headers, int Timeout = -1);

        // Size
        Async GetBrowserSize(int Timeout = -1);
        Async ResizeBrowser(int Width, int Height, int Timeout = -1);

        // Javascript
        Async SetStartupScript(const std::string& Script, int Timeout = -1);
        Async ExecuteJavascript(const std::string& Script, const std::string& Variables = std::string(), const std::string& ElementPath = std::string(), bool ScrollToElement = false, int Timeout = -1);

        // Inputs
        void Mouse(MouseEvent Event, int X, int Y, MouseButton Button = MouseButtonLeft, int MousePressed = MouseButtonNone, int KeyboardPresses = KeyboardModifiersNone, int ClickCount = 1);
        void Wheel(int X, int Y, bool IsUp, int Delta = 100, int MousePressed = MouseButtonNone, int KeyboardPresses = KeyboardModifiersNone);
        Async Touch(TouchEvent Event, int X, int Y, int Id, double RadiusX = 11.5, double RadiusY = 11.5, double RotationAngle = 0.0, double Pressure = 1.0, int Timeout = -1);
        void Key(KeyEvent Event, const std::string& Char, int KeyboardPresses = KeyboardModifiersNone);
        void KeyRaw(KeyEvent Event, WPARAM WindowsVirtualKeyCode, LPARAM NativeVirtualKeyCode, int KeyboardPresses = KeyboardModifiersNone);
        void Focus();

        // Interacting with elements
        Async Inspect(int X, int Y, int Position = 0, int Timeout = -1);

        // Dialogs
        void SetOpenFileDialogResult(const std::string& Result);
        void SetOpenFileDialogManualMode(bool IsManual);
        void SetPromptResult(const std::string& PromptResult);

        // Downloads
        Async RestrictDownloads(int Timeout = -1);
        Async AllowDownloads(int Timeout = -1);
        std::string GetDownloadedFilePath();
        bool IsFileDownloadReady();

        // Popups
        void RestrictPopups();
        void AllowPopups();

        // Cookies
        Async RestoreCookies(const std::string& Cookies, int Timeout = -1);
        Async SaveCookies(int Timeout = -1);

        // Extensions
        void TriggerExtensionButton(const std::string ExtensionIdOrNamePart);
        std::vector<std::pair<std::string, std::string> > GetExtensionList();

        // Drag and drop
        Async StartDragFile(const std::string& Path, int Timeout = -1);

        // Helpers
        Async Sleep(int TimeMilliseconds, int Timeout = -1);
};

#endif // NONECONNECTOR_H
