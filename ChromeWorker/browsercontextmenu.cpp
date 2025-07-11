#include "browsercontextmenu.h"
#include "translate.h"
#include "writefile.h"
#include "browsereventsemulator.h"
#include "log.h"
#include "picojson.h"
#include "converter.h"
#include "javascriptextensions.h"

void BrowserContextMenu::ShowMenu(HWND hwnd, POINT& p, bool IsRecord, bool CanGoBack, bool CanGoForward, const std::vector<std::pair<std::string, std::string> >& Extensions)
{
    if(hMenu)
    {
        DestroyMenu(hMenu);
        hMenu = 0;
    }

    hMenu = CreatePopupMenu();
    UINT Enabled = MF_BYPOSITION | MF_STRING;
    UINT Disabled = Enabled | MF_DISABLED;


    AppendMenu(hMenu, ( CanGoBack ) ? Enabled : Disabled, IdBackward, Translate::Tr(L"Backward").c_str());
    AppendMenu(hMenu, ( CanGoForward ) ? Enabled : Disabled, IdForward, Translate::Tr(L"Forward").c_str());
    AppendMenu(hMenu, Enabled, IdReload, Translate::Tr(L"Reload").c_str());
    AppendMenu(hMenu, Enabled, IdFind, Translate::Tr(L"Find").c_str());
    if(!Extensions.empty())
    {
        HMENU hSubMenu = CreatePopupMenu();
        int Id = IdExtensions;
        for(const std::pair<std::string, std::string>& Extension: Extensions)
        {
            AppendMenu(hSubMenu, Enabled, Id, (s2ws(Extension.second) + std::wstring(L"(") + s2ws(Extension.first) + std::wstring(L")")).c_str());
            Id++;
        }
        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, Translate::Tr(L"Extensions").c_str());
    }
    AppendMenu(hMenu, Enabled, IdGetPageSource, Translate::Tr(L"Get page source").c_str());
    /*if(!IsRecord)
    {
        AppendMenu(hMenu, Enabled, IdSavePageAs, Translate::Tr(L"Save page as").c_str());
    }*/
    AppendMenu(hMenu, Enabled, IdOpenDeveloperTools, Translate::Tr(L"Open developer tools").c_str());


    TrackPopupMenu(hMenu, TPM_TOPALIGN | TPM_LEFTALIGN, p.x, p.y, 0, hwnd, NULL);
    if(hMenu)
    {
        DestroyMenu(hMenu);
        hMenu = 0;
    }
}

void BrowserContextMenu::Show(HWND hwnd, int X, int Y, bool IsLink, bool IsMedia, bool IsEdit, const std::string& LinkUrl, const std::string& MediaUrl, const std::string& CurrentUrl, const std::string& SelectedText, bool CanGoBack, bool CanGoForward)
{

    LastClickX = X;
    LastClickY = Y;
    Url = LinkUrl;
    UrlMedia = MediaUrl;
    LastSelectText = SelectedText;
    LastCurrentUrl = CurrentUrl;

    if(hMenu)
    {
        DestroyMenu(hMenu);
        hMenu = 0;
    }

    hMenu = CreatePopupMenu();
    UINT Enabled = MF_BYPOSITION | MF_STRING;
    UINT Disabled = Enabled | MF_DISABLED;


    AppendMenu(hMenu, ( CanGoBack ) ? Enabled : Disabled, IdBackward, Translate::Tr(L"Backward").c_str());
    AppendMenu(hMenu, ( CanGoForward ) ? Enabled : Disabled, IdForward, Translate::Tr(L"Forward").c_str());
    AppendMenu(hMenu, Enabled, IdReload, Translate::Tr(L"Reload").c_str());
    AppendMenu(hMenu, Enabled, IdFind, Translate::Tr(L"Find").c_str());
    AppendMenu(hMenu, Enabled, IdGetPageSource, Translate::Tr(L"Get page source").c_str());
    AppendMenu(hMenu, Enabled, IdSavePageAs, Translate::Tr(L"Save page as").c_str());
    AppendMenu(hMenu, Enabled, IdOpenDeveloperTools, Translate::Tr(L"Open developer tools").c_str());
    AppendMenu(hMenu, Enabled, IdInspectElement, Translate::Tr(L"Inspect element").c_str());


    if(IsLink)
    {
        AppendMenu(hMenu,MF_SEPARATOR,NULL,L"Separator");
        AppendMenu(hMenu, Enabled, IdCopyLinkLocation, Translate::Tr(L"Copy link location").c_str());
        AppendMenu(hMenu, Enabled, IdOpenLinkInANewTab, Translate::Tr(L"Open link in a new tab").c_str());
    }
    if(IsMedia)
    {
        AppendMenu(hMenu,MF_SEPARATOR,NULL,L"Separator");
        AppendMenu(hMenu, Enabled, IdCopyUrl, Translate::Tr(L"Copy media url").c_str());
        AppendMenu(hMenu, Enabled, IdSaveAs, Translate::Tr(L"Save media as").c_str());
    }
    if(!SelectedText.empty())
    {
        AppendMenu(hMenu,MF_SEPARATOR,NULL,L"Separator");
        AppendMenu(hMenu, Enabled, IdCopyText, Translate::Tr(L"Copy selected text").c_str());
        std::wstring Text = Translate::Tr(L"Find ");
        std::wstring TextAdd = s2ws(SelectedText);
        if(TextAdd.length() > 20)
            TextAdd = TextAdd.substr(0,20) + std::wstring(L" ... ");
        Text += std::wstring(L"\"");
        Text += TextAdd;
        Text += std::wstring(L"\"");
        Text += Translate::Tr(L" in Google");
        AppendMenu(hMenu, Enabled, IdFindInGoogle, Text .c_str());
    }
    if(IsEdit)
    {
        AppendMenu(hMenu,MF_SEPARATOR,NULL,L"Separator");
        AppendMenu(hMenu, Enabled, IdCutEditable, Translate::Tr(L"Cut").c_str());
        AppendMenu(hMenu, Enabled, IdCopyEditable, Translate::Tr(L"Copy").c_str());
        AppendMenu(hMenu, Enabled, IdPasteEditable, Translate::Tr(L"Paste").c_str());
        AppendMenu(hMenu, Enabled, IdSelectAllEditable, Translate::Tr(L"Select all").c_str());
    }


    POINT p;
    p.x = 0;
    p.y = 0;

    GetCursorPos(&p);

    TrackPopupMenu(hMenu, TPM_TOPALIGN | TPM_LEFTALIGN, p.x, p.y, 0, hwnd, NULL);
    if(hMenu)
    {
        DestroyMenu(hMenu);
        hMenu = 0;
    }

}

void SourceSaver::Visit(const CefString& string)
{
    //Getting page source
    WriteStringToFile("source.txt",string.ToString());
    ShellExecute(0, 0, L"source.txt", 0, 0 , SW_SHOW );
}

void BrowserContextMenu::Input(IDevToolsConnector* Connector, const std::string Text)
{
    std::string TextCurrent = Text;
    KeyState State;
    while(true)
    {
        BrowserEventsEmulator::Key(Connector, TextCurrent, State, LastClickX, LastClickY, false);
        if(TextCurrent.length() == 0 && State.IsClear() && !State.IsPresingCharacter())
        {
            return;
        }
    }
}

void BrowserContextMenu::SetClipboard(const std::string& Text)
{
    std::string data = Text;
    if (OpenClipboard(0))
    {
        HGLOBAL clipbuffer;
        char * buffer;
        EmptyClipboard();
        clipbuffer = GlobalAlloc(GMEM_DDESHARE, data.length() + 1);
        buffer = (char*)GlobalLock(clipbuffer);
        memcpy(buffer, data.data(),data.size());
        buffer[data.length()] = 0;
        GlobalUnlock(clipbuffer);
        SetClipboardData(CF_TEXT,clipbuffer);
        CloseClipboard();
    }
}

void BrowserContextMenu::OnFind(IDevToolsConnector* Connector, LPFINDREPLACE Data)
{
    if(Data->Flags & FR_DIALOGTERM)
    {
        //Browser->GetHost()->StopFinding(true);
        find_what_last_.clear();
        find_next_ = false;
        find_hwnd_ = 0;
    }else if (Data->Flags & FR_FINDNEXT)
    {
        bool match_case = (Data->Flags & FR_MATCHCASE) ? true : false;
        const std::wstring& find_what = find_buff_;
        if (match_case != find_match_case_last_ || find_what != find_what_last_)
        {
            if(!find_what.empty())
            {
                //Browser->GetHost()->StopFinding(true);
                find_next_ = false;
            }
            find_match_case_last_ = match_case;
            find_what_last_ = find_buff_;
        }
        //Browser->GetHost()->Find(0, find_what, (find_state_.Flags & FR_DOWN) ? true : false, match_case, find_next_);
        std::string Script = std::string("_BAS_SAFE(Window.find)(") + picojson::value(ws2s(find_what)).serialize() + std::string(", ")
        + ((match_case) ? std::string("true") : std::string("false"))
        + std::string(", ")
        + ((find_state_.Flags & FR_DOWN) ? std::string("false") : std::string("true"))
        + std::string(", true, false, true);");

        Connector->ExecuteJavascript(Script, std::string(), std::string("[]"));
        if(!find_next_)
            find_next_ = true;
    }

}

void BrowserContextMenu::ShowFindDialog(HWND hwnd)
{
    if(find_hwnd_)
    {
        // Give focus to the existing find dialog.
        ::SetFocus(find_hwnd_);
        return;
    }

    ZeroMemory(&find_state_, sizeof(find_state_));
    find_state_.lStructSize = sizeof(find_state_);
    find_state_.hwndOwner = hwnd;
    find_state_.lpstrFindWhat = find_buff_;
    find_state_.wFindWhatLen = sizeof(find_buff_);
    find_state_.Flags = FR_HIDEWHOLEWORD | FR_DOWN;

    find_hwnd_ = FindText(&find_state_);
}

void BrowserContextMenu::Process(HWND hwnd, int Command, IDevToolsConnector* Connector, const std::string& UniqueProcessId)
{
    
    if(Command == IdBackward)
    {
        Connector->NavigateBack(true);
    }else if(Command == IdForward)
    {
        Connector->NavigateForward(true);
    }else if(Command == IdReload)
    {
        Connector->Reload(true);
    }else if(Command == IdGetPageSource)
    {
        Connector->ExecuteJavascript("[[RESULT]] = _BAS_HIDE(BrowserAutomationStudio_GetPageContent)();", std::string(), std::string("[]"))->Then([this](AsyncResult* Result)
        {
            JsonParser Parser;
            std::string TextResult = Parser.GetStringFromJson(Result->GetString(),"RESULT");
            WriteStringToFile("source.txt",TextResult);
            ShellExecute(0, 0, L"source.txt", 0, 0 , SW_SHOW );
        });

    }else if(Command == IdOpenDeveloperTools)
    {
        Connector->OpenDevTools();
    }else if(Command == IdInspectElement)
    {
        Connector->InspectAt(LastClickX, LastClickY);
    }else if(Command == IdCopyLinkLocation)
    {
        SetClipboard(Url);

    }else if(Command == IdOpenLinkInANewTab)
    {
        Input(Connector, "<CONTROL><SHIFT><MOUSELEFT>");

    }else if(Command == IdCopyUrl)
    {
        SetClipboard(UrlMedia);

    }else if(Command == IdCopyText)
    {
        Input(Connector, "<CONTROL>c");
    }else if(Command == IdFindInGoogle)
    {
        Connector->CreateTab(std::string("https://www.google.com/search?q=") + LastSelectText,true);
    }
    else if(Command == IdCutEditable)
    {
        Input(Connector, "<CONTROL>x");
    }else if(Command == IdCopyEditable)
    {
        Input(Connector, "<CONTROL>c");
    }else if(Command == IdPasteEditable)
    {
        Input(Connector, "<CONTROL>v");
    }else if(Command == IdSelectAllEditable)
    {
        Input(Connector, "<CONTROL>a");
    }else if(Command == IdFind)
    {
        ShowFindDialog(hwnd);

    }else if(Command == IdSaveAs)
    {
        std::string UrlEscaped = picojson::value(UrlMedia).serialize();
        std::string Script = std::string("_BAS_HIDE(BrowserAutomationStudio_DownloadUrl)(") + UrlEscaped + std::string(");");
        JavaScriptExtensions Extensions;
        Script = Extensions.ProcessJs(Script,UniqueProcessId);
        Connector->ExecuteJavascript(Script, std::string(), std::string("[]"));
    }else if(Command == IdSavePageAs)
    {
        std::string UrlEscaped = picojson::value(LastCurrentUrl).serialize();
        std::string Script = std::string("_BAS_HIDE(BrowserAutomationStudio_DownloadUrl)(") + UrlEscaped + std::string(");");
        JavaScriptExtensions Extensions;
        Script = Extensions.ProcessJs(Script,UniqueProcessId);
        Connector->ExecuteJavascript(Script, std::string(), std::string("[]"));
    }else if(Command >= IdExtensions)
    {
        std::vector<std::pair<std::string, std::string> > Extensions = Connector->GetExtensionList();
        int Index = Command - IdExtensions;
        if(Index < Extensions.size())
        {
            Connector->TriggerExtensionButton(Extensions[Index].first);
        }
    }



}
