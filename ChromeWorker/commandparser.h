#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <string>
#include <vector>
#include <functional>
#include "elementcommand.h"

class CommandParser
{
    std::string AllXml;
public:
    CommandParser();
    void Parse(const std::string& Xml);

    //Events
    std::vector<std::function<void(const std::string&)> > EventLoad;
    std::vector<std::function<void(const std::string&, const std::string&, bool)> > EventLoad2;
    std::vector<std::function<void(const std::string&)> > EventView;
    std::vector<std::function<void(const std::string&)> > EventGetTabs;
    std::vector<std::function<void(const std::string&)> > EventManualBrowserControl;

    std::vector<std::function<void()> > EventDisableBorwser;

    std::vector<std::function<void(bool)> > EventVisible;
    std::vector<std::function<void()> > EventFlush;
    std::vector<std::function<void(const std::string&)> > EventSetNextAction;
    std::vector<std::function<void(const std::string&, int, bool, const std::string&, const std::string&, const std::string&)> > EventSetProxy;
    std::vector<std::function<void(const std::string&, const std::string&, const std::string&)> > EventAddHeader;
    std::vector<std::function<void(const std::string&)> > EventSetHeaderList;
    std::vector<std::function<void(const std::string&)> > EventSetAcceptLanguagePattern;
    std::vector<std::function<void(const std::string&)> > EventSetUserAgentData;
    std::vector<std::function<void(const std::string&)> > EventSetUserAgent;
    std::vector<std::function<void(const std::string&)> > EventPrepareFunction;
    std::vector<std::function<void(const std::string&)> > EventBackupDone;
    std::vector<std::function<void(const std::string&)> > EventRecaptchaV3List;
    std::vector<std::function<void(const std::string&)> > EventClickExtensionButton;
    std::vector<std::function<void(const std::string&,const std::string&)> > EventRecaptchaV3Result;
    std::vector<std::function<void(const std::string&)> > EventSetOpenFileName;
    std::vector<std::function<void(const std::string&)> > EventDragFile;
    std::vector<std::function<void(const std::string&,const std::string&, const std::string&)> > EventSetStartupScript;
    std::vector<std::function<void(const std::string&,const std::string&, const std::string&)> > EventRunTask;
    std::vector<std::function<void(const std::string&,bool, const std::string&)> > EventCheckResult;

    std::vector<std::function<void(bool,bool,const std::string&,int,bool, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&)> > EventSendWorkerSettings;

    std::vector<std::function<void(const std::string&)> > EventSetPromptResult;
    std::vector<std::function<void(const std::string&,const std::string&)> > EventSetHttpAuthResult;

    std::vector<std::function<void()> > EventGetUrl;
    std::vector<std::function<void()> > EventGetBrowserScreenSettings;
    std::vector<std::function<void(int, int)> > EventResize;
    std::vector<std::function<void(const std::string&, bool)> > EventSetWindow;
    std::vector<std::function<void(const std::string&)> > EventHighlightAction;
    std::vector<std::function<void(int, int)> > EventMouseClick;
    std::vector<std::function<void(int, int)> > EventMouseClickUp;
    std::vector<std::function<void(int, int)> > EventMouseClickDown;
    std::vector<std::function<void(int, int)> > EventScroll;
    std::vector<std::function<void(int, int, int, int)> > EventRender;
    std::vector<std::function<void(int, int, double, double, double, bool,bool,float,bool,bool)> > EventMouseMove;
    std::vector<std::function<void(int)> > EventPopupClose;
    std::vector<std::function<void(int)> > EventPopupSelect;
    std::vector<std::function<void(bool, std::string)> > EventPopupCreate;
    std::vector<std::function<void(double)> > EventSetDeviceScaleFactor;
    std::vector<std::function<void(bool, std::string, std::string, bool)> > EventPopupCreate2;
    std::vector<std::function<void(int)> > EventSetComboboxIndex;
    std::vector<std::function<void()> > EventPopupInfo;
    std::vector<std::function<void(int)> > EventTimezone;
    std::vector<std::function<void()> > EventCleanHeader;
    std::vector<std::function<void(const std::string&)> > EventGetCookiesForUrl;
    std::vector<std::function<void()> > EventSaveCookies;
    std::vector<std::function<void(const std::string&)> > EventRestoreCookies;
    std::vector<std::function<void(const std::string&)> > EventRestoreLocalStorage;
    std::vector<std::function<void()> > EventWaitCode;
    std::vector<std::function<void(int)> > EventStartSection;
    std::vector<std::function<void(std::string)> > EventSetFontList;

    std::vector<std::function<void()> > EventClearImageData;
    std::vector<std::function<void(std::string)> > EventSetImageData;
    std::vector<std::function<void()> > EventFindImage;

    std::vector<std::function<void()> > EventScriptFinished;
    std::vector<std::function<void()> > EventIsChanged;
    std::vector<std::function<void()> > EventCrush;

    std::vector<std::function<void(const std::string&)> > EventAddCacheMaskAllow;
    std::vector<std::function<void(const std::string&)> > EventAddCacheMaskDeny;
    std::vector<std::function<void(const std::string&)> > EventAddRequestMaskAllow;
    std::vector<std::function<void(const std::string&)> > EventAddRequestMaskDeny;
    std::vector<std::function<void()> > EventClearCacheMask;
    std::vector<std::function<void()> > EventClearRequestMask;
    std::vector<std::function<void()> > EventAllowPopups;
    std::vector<std::function<void()> > EventAllowDownloads;
    std::vector<std::function<void()> > EventRestrictPopups;
    std::vector<std::function<void()> > EventRestrictDownloads;
    std::vector<std::function<void()> > EventClearLoadedUrl;
    std::vector<std::function<void()> > EventClearCachedData;
    std::vector<std::function<void()> > EventClearAll;
    std::vector<std::function<void()> > EventClearMasks;
    std::vector<std::function<void()> > EventClearData;
    std::vector<std::function<void(const std::string&)> > EventFindCacheByMaskBase64;
    std::vector<std::function<void(const std::string&)> > EventFindStatusByMask;
    std::vector<std::function<void(const std::string&)> > EventFindUrlByMask;
    std::vector<std::function<void(const std::string&)> > EventFindCacheByMaskString;
    std::vector<std::function<void(const std::string&)> > EventFindAllCache;
    std::vector<std::function<void(const std::string&)> > EventIsUrlLoadedByMask;
    std::vector<std::function<void(const std::string&)> > EventRequestVariablesResult;
    std::vector<std::function<void(const std::string&)> > EventDebugVariablesResult;
    std::vector<std::function<void(const std::string&)> > EventDebugCallstackResult;
    std::vector<std::function<void()> > EventGetLoadStats;
    std::vector<std::function<void()> > EventRestoreOriginalStage;
    std::vector<std::function<void(const std::string&)> > EventSetMode;


    std::vector<std::function<void(const std::string&,const std::string&,const std::string&,bool,const std::string&,const std::string&,const std::string&)> > EventSetCode;
    std::vector<std::function<void(const std::string&)> > EventSetResources;
    std::vector<std::function<void()> > EventReset;
    std::vector<std::function<void(bool)> > EventNavigateBack;
    std::vector<std::function<void()> > EventResetNoCookies;

    std::vector<std::function<void(const ElementCommand &)> > EventElementCommand;

};

#endif // COMMANDPARSER_H
