#ifndef IDEVTOOLSCONNECTOR_H
#define IDEVTOOLSCONNECTOR_H

#include "ISimpleHttpClientFactory.h"
#include "IWebSocketClientFactory.h"
#include "devtoolsactionfactory.h"
#include "KeyboardEmulation.h"
#include "idevtoolsaction.h"
#include <windows.h>

class IDevToolsConnector
{
    protected:
        // Connection
        enum
        {
            WaitingForAutoconnectEnable,
            WaitingForDownloadsEnable,
            WaitingForBrowserEndpoint,
            WaitingForBrowserClose,
            WaitingForWebsocket,
            WaitingFirstTab,
            NotStarted,
            Connected
        } ConnectionState = NotStarted;

        // Configuration
        void ResetProxy(const std::string& ParentProcessId);

    public:
        // Paint
        virtual char* GetPaintData() = 0;
        virtual int GetPaintHeight() = 0;
        virtual int GetPaintWidth() = 0;

        virtual char* GetPaintDataScaled() = 0;
        virtual int GetPaintHeightScaled() = 0;
        virtual int GetPaintWidthScaled() = 0;

        // Scroll
        virtual int GetScrollX() = 0;
        virtual int GetScrollY() = 0;

        // Viewport
        virtual int GetHeight() = 0;
        virtual int GetWidth() = 0;

        // Listeners
        std::vector<std::function<void()> > OnPaint;
        std::vector<std::function<void()> > OnResize;
        std::vector<std::function<void()> > OnScroll;
        std::vector<std::function<void()> > OnLoadStop;
        std::vector<std::function<void()> > OnLoadStart;
        std::vector<std::function<void()> > OnBrowserCreated;
        std::vector<std::function<void(std::string)> > OnRequestStop;
        std::vector<std::function<void(std::string)> > OnRequestStart;
        std::vector<std::function<void(std::string)> > OnNativeDialog;
        std::vector<std::function<void(std::string)> > OnAddressChanged;
        std::vector<std::function<void(std::wstring)> > OnDownloadStarted;
        std::vector<std::function<void(std::string)> > OnRequestDataMain;
        std::vector<std::function<void(std::string)> > OnRequestDataAdditional;
        std::vector<std::function<void(std::string&, std::string&)> > OnMessage;

        virtual void Timer() = 0;

        virtual void Initialize(
            std::shared_ptr<ISimpleHttpClientFactory> SimpleHttpClientFactory,
            std::shared_ptr<IWebSocketClientFactory> WebSocketClientFactory,
            int Port, const std::string& UniqueProcessId, const std::string& ParentProcessId,
            const std::string& ConstantStartupScript,
            const std::vector<std::pair<std::string,std::string> >& CommandLineAdditional,
            const std::string& InitalProxy, int PcapPort
        ) = 0;
        virtual void SetProfilePath(const std::wstring& Path) = 0;
        virtual void SetExtensionList(const std::vector<std::wstring>& Extensions) = 0;
        virtual bool InterruptAction(int ActionUniqueId) = 0;
        virtual void StartProcess() = 0;
        virtual void OpenDevTools() = 0;
        virtual void InspectAt(int X, int Y) = 0;

        virtual std::wstring GetProxyConfigFolder() = 0;

        // Browser life cycle
        virtual Async Reset(int Timeout = -1) = 0;
        virtual bool IsLoading() = 0;
        virtual void CloseBrowser() = 0;

        // Tabs
        virtual Async CreateTab(const std::string& Url, bool IsInstant = false, bool IsDelayed = false, const std::string& Referrer = std::string(), int Timeout = -1) = 0;
        virtual Async SwitchToTab(int Index, int Timeout = -1) = 0;
        virtual Async CloseTab(int Index, int Timeout = -1) = 0;
        virtual Async GetTabsList(int Timeout = -1) = 0;
        virtual int GetCurrentTabIndex() = 0;
        virtual int GetTabNumber() = 0;

        // Rendering
        virtual Async ResetDeviceScaleFactor(float DeviceScaleFactor, int Timeout = -1) = 0;
        virtual Async Screenshot(int X, int Y, int Width, int Height, int Timeout = -1) = 0;
        virtual Async StartScreenCast(int Timeout = -1) = 0;
        virtual Async StopScreenCast(int Timeout = -1) = 0;
        virtual void DisableBackgroundMode() = 0;
        virtual void EnableBackgroundMode() = 0;

        // Network
        virtual Async Load(const std::string& Url, bool IsInstant = false, const std::string& Referrer = std::string(), int Timeout = -1) = 0;
        virtual Async Reload(bool IsInstant = false, int Timeout = -1) = 0;
        virtual Async NavigateBack(bool IsInstant = false, int Timeout = -1) = 0;
        virtual Async NavigateForward(bool IsInstant = false, int Timeout = -1) = 0;
        virtual Async GetCurrentUrl(int Timeout = -1) = 0;
        virtual Async SetRequestsRestrictions(const std::vector<std::pair<bool, std::string> >& Rules, int Timeout = -1) = 0;
        virtual Async SetHttpAuth(const std::string& UserName, const std::string& Password, int Timeout = -1) = 0;

        // Cache
        virtual void SetCacheMasks(const std::vector<std::pair<bool, std::string> >& Rules) = 0;
        virtual std::string GetSingleCacheData(const std::string& Mask, bool IsBase64) = 0;
        virtual std::string GetAllCacheData(const std::string& Mask) = 0;
        virtual void ClearNetworkData() = 0;

        virtual int GetStatusForURL(const std::string& UrlPattern) = 0;
        virtual std::string FindLoadedURL(const std::string& UrlPattern) = 0;
        virtual bool IsURLLoaded(const std::string& UrlPattern) = 0;
        virtual Async SetProxy(const std::string Server, int Port, bool IsHttp = true, const std::string Login = std::string(), const std::string Password = std::string(), int Timeout = -1) = 0;
        virtual Async GetHistory(int Timeout = -1) = 0;
        virtual Async SetHeaders(const std::vector<std::pair<std::string, std::string>>& Headers, int Timeout = -1) = 0;
        virtual Async SetUserAgentData(const std::string& Data, const std::vector<std::pair<std::string, std::string>>& Headers, int Timeout = -1) = 0;

        // Size
        virtual Async GetBrowserSize(int Timeout = -1) = 0;
        virtual Async ResizeBrowser(int Width, int Height, int Timeout = -1) = 0;

        // Javascript
        virtual Async SetStartupScript(const std::string& Script, int Timeout = -1) = 0;
        virtual Async ExecuteJavascript(const std::string& Script, const std::string& Variables = std::string(), const std::string& ElementPath = std::string(), bool ScrollToElement = false, int Timeout = -1) = 0;

        // Inputs
        virtual void Mouse(MouseEvent Event, int X, int Y, MouseButton Button = MouseButtonLeft, int MousePressed = MouseButtonNone, int KeyboardPresses = KeyboardModifiersNone, int ClickCount = 1) = 0;
        virtual void Wheel(int X, int Y, bool IsUp, int Delta = 100, int MousePressed = MouseButtonNone, int KeyboardPresses = KeyboardModifiersNone) = 0;
        virtual Async Touch(TouchEvent Event, int X, int Y, int Id, double RadiusX = 11.5, double RadiusY = 11.5, double RotationAngle = 0.0, double Pressure = 1.0, int Timeout = -1) = 0;
        virtual void Key(KeyEvent Event, const std::string& Char, int KeyboardPresses = KeyboardModifiersNone) = 0;
        virtual void KeyRaw(KeyEvent Event, WPARAM WindowsVirtualKeyCode, LPARAM NativeVirtualKeyCode, int KeyboardPresses = KeyboardModifiersNone) = 0;
        virtual void Focus() = 0;

        // Interacting with elements
        virtual Async Inspect(int X, int Y, int Position = 0, int Timeout = -1) = 0;

        // Dialogs
        virtual void SetOpenFileDialogResult(const std::string& Result) = 0;
        virtual void SetOpenFileDialogManualMode(bool IsManual) = 0;
        virtual void SetPromptResult(const std::string& PromptResult) = 0;

        // Downloads
        virtual Async RestrictDownloads(int Timeout = -1) = 0;
        virtual Async AllowDownloads(int Timeout = -1) = 0;
        virtual std::string GetDownloadedFilePath() = 0;
        virtual bool IsFileDownloadReady() = 0;

        // Popups
        virtual void RestrictPopups() = 0;
        virtual void AllowPopups() = 0;

        // Cookies
        virtual Async RestoreCookies(const std::string& Cookies, int Timeout = -1) = 0;
        virtual Async SaveCookies(int Timeout = -1) = 0;

        // Extensions
        virtual void TriggerExtensionButton(const std::string ExtensionIdOrNamePart) = 0;
        virtual std::vector<std::pair<std::string, std::string> > GetExtensionList() = 0;

        // Drag and drop
        virtual Async StartDragFile(const std::string& Path, int Timeout = -1) = 0;

        // Helpers
        virtual Async Sleep(int TimeMilliseconds, int Timeout = -1) = 0;
};

#endif // IDEVTOOLSCONNECTOR_H
