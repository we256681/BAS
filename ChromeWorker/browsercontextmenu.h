#ifndef BROWSERCONTEXTMENU_H
#define BROWSERCONTEXTMENU_H

#include <windows.h>
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "idevtoolsconnector.h"


class SourceSaver: public CefStringVisitor
{
    virtual void Visit(const CefString& string) OVERRIDE;
private:
    IMPLEMENT_REFCOUNTING(SourceSaver);
};

class BrowserContextMenu
{
public:
    const static int IdBackward = 2000;
    const static int IdForward = IdBackward + 1;
    const static int IdReload = IdBackward + 2;
    const static int IdFind = IdBackward + 3;
    const static int IdGetPageSource = IdBackward + 4;
    const static int IdOpenDeveloperTools = IdBackward + 5;
    const static int IdInspectElement = IdBackward + 6;
    const static int IdCopyLinkLocation = IdBackward + 7;
    const static int IdOpenLinkInANewTab = IdBackward + 8;

    const static int IdCopyUrl = IdBackward + 9;
    const static int IdSaveAs = IdBackward + 10;
    const static int IdCopyText = IdBackward + 11;
    const static int IdFindInGoogle = IdBackward + 12;
    const static int IdCutEditable = IdBackward + 13;
    const static int IdCopyEditable = IdBackward + 14;
    const static int IdPasteEditable = IdBackward + 15;
    const static int IdSelectAllEditable = IdBackward + 16;
    const static int IdSavePageAs = IdBackward + 17;

    const static int IdExtensions = IdBackward + 99;

    CefRefPtr<SourceSaver> _SourceSaver;


    HMENU hMenu = 0;
    int LastClickX = 0;
    int LastClickY = 0;
    std::string Url;
    std::string UrlMedia;
    std::string LastSelectText;
    std::string LastCurrentUrl;

    FINDREPLACE find_state_;
    WCHAR find_buff_[3000] = { 0 };
    std::wstring find_what_last_;
    bool find_next_;
    bool find_match_case_last_;
    HWND find_hwnd_ = 0;

    void Show(HWND hwnd, int X, int Y, bool IsLink, bool IsMedia, bool IsEdit, const std::string& LinkUrl, const std::string& MediaUrl, const std::string& CurrentUrl, const std::string& SelectedText, bool CanGoBack, bool CanGoForward);
    void ShowMenu(HWND hwnd, POINT& p, bool IsRecord, bool CanGoBack, bool CanGoForward, const std::vector<std::pair<std::string, std::string> >& Extensions);
    void Process(HWND hwnd, int Command, IDevToolsConnector* Connector, const std::string& UniqueProcessId);
    void OnFind(IDevToolsConnector* Connector, LPFINDREPLACE Data);
    void ShowFindDialog(HWND hwnd);

private:
    void Input(IDevToolsConnector* Connector, const std::string Text);
    void SetClipboard(const std::string& Text);


};

#endif // BROWSERCONTEXTMENU_H
