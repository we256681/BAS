#include "devtoolsactionfactory.h"
#include "devtoolsactiongettabs.h"
#include "DevToolsActionSwitchToTab.h"
#include "devtoolsactionload.h"
#include "devtoolsactionreload.h"
#include "devtoolsactiongetbrowsersize.h"
#include "devtoolsactionresizebrowser.h"
#include "devtoolsactionsetstartupscript.h"
#include "devtoolsactionexecutejavascript.h"
#include "DevToolsActionCreateTab.h"
#include "DevToolsActionCloseTab.h"
#include "DevToolsActionStartScreenCast.h"
#include "DevToolsActionStopScreenCast.h"
#include "DevToolsActionGetCurrentUrl.h"
#include "DevToolsActionNavigateBack.h"
#include "DevToolsActionNavigateForward.h"
#include "DevToolsActionSetProxy.h"
#include "devtoolsactioninspect.h"
#include "devtoolsactiongetnavigationhistory.h"
#include "devtoolsactionresizewithcorrection.h"
#include "devtoolsactionopenfile.h"
#include "devtoolsactiondialogresult.h"
#include "DevToolsActionSetHeaders.h"
#include "devtoolsactionsavecookies.h"
#include "devtoolsactionrestorecookies.h"
#include "DevToolsActionSetRequestsRestrictions.h"
#include "devtoolsactionstartdragfile.h"
#include "devtoolsactionsleep.h"
#include "devtoolsactiontouch.h"
#include "devtoolsactionresetdevicescalefactor.h"
#include "devtoolsactionempty.h"
#include <random>

int DevToolsActionFactory::Rand()
{
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0xF, 0x7FFFFFFF);
    return distribution(generator);
}

IDevToolsAction* DevToolsActionFactory::Create(const std::string& Name, DevToolsGlobalState* GlobalState)
{

    IDevToolsAction* Result = 0;
    if(Name == "GetTabsList")
    {
        Result = new DevToolsActionGetTabs();
    }else if(Name == "Load")
    {
        Result = new DevToolsActionLoad();
    }else if(Name == "Reload")
    {
        Result = new DevToolsActionReload();
    }else if(Name == "GetBrowserSize")
    {
        Result = new DevToolsActionGetBrowserSize();
    }else if(Name == "ResizeBrowser")
    {
        Result = new DevToolsActionResizeBrowser();
    }else if (Name == "SetStartupScript")
    {
        Result = new DevToolsActionSetStartupScript();
    }else if (Name == "ExecuteJavascript")
    {
        Result = new DevToolsActionExecuteJavascript();
    }else if(Name == "CreateTab")
    {
        Result = new DevToolsActionCreateTab();
    } else if(Name == "SwitchToTab")
    {
        Result = new DevToolsActionSwitchToTab();
    } else if(Name == "CloseTab")
    {
        Result = new DevToolsActionCloseTab();
    } else if(Name == "StartScreencast")
    {
        Result = new DevToolsActionStartScreenCast();
    } else if(Name == "StopScreencast")
    {
        Result = new DevToolsActionStopScreenCast();
    } else if(Name == "SetRequestsRestrictions")
    {
        Result = new DevToolsActionSetRequestsRestrictions();
    } else if(Name == "GetCurrentUrl")
    {
        Result = new DevToolsActionGetCurrentUrl();
    } else if(Name == "NavigateBack")
    {
        Result = new DevToolsActionNavigateBack();
    } else if(Name == "NavigateForward")
    {
        Result = new DevToolsActionNavigateForward();
    } else if(Name == "SetProxy")
    {
        Result = new DevToolsActionSetProxy();
    } else if(Name == "Inspect")
    {
        Result = new DevToolsActionInspect();
    } else if(Name == "GetHistory")
    {
        Result = new DevToolsActionGetNavigationHistory();
    }else if(Name == "ResizeBrowserWithCorrection")
    {
        Result = new DevToolsActionResizeWithCorrection();
    }else if(Name == "ResizeBrowserWithCorrection")
    {
        Result = new DevToolsActionResizeWithCorrection();
    }else if(Name == "OpenFile")
    {
        Result = new DevToolsActionOpenFile();
    }else if(Name == "DialogResult")
    {
        Result = new DevToolsActionDialogResult();
    }else if(Name == "SetHeaders")
    {
        Result = new DevToolsActionSetHeaders();
    }else if(Name == "SaveCookies")
    {
        Result = new DevToolsActionSaveCookies();
    }else if(Name == "RestoreCookies")
    {
        Result = new DevToolsActionRestoreCookies();
    }else if(Name == "ResetDeviceScaleFactor")
    {
        Result = new DevToolsActionResetDeviceScaleFactor();
    }else if(Name == "StartDragFile")
    {
        Result = new DevToolsActionStartDragFile();
    }else if(Name == "Sleep")
    {
        Result = new DevToolsActionSleep();
    }else if(Name == "Touch")
    {
        Result = new DevToolsActionTouch();
    }else if(Name == "Empty")
    {
        Result = new DevToolsActionEmpty();
    }



    if(Result)
    {
        Result->Initialize(Name);
        int UniqueId = Rand();
        Result->SetId(Rand());
        Result->SetUniqueId(UniqueId);
        Result->GetResult()->SetActionUniqueId(UniqueId);
        Result->SetGroupId(Rand());
        Result->SetGlobalState(GlobalState);
        if (Name == "Empty") Result->Run();
    }

    return Result;
}

IDevToolsAction* DevToolsActionFactory::CreateWebsocketQuery(const std::string& WebSocketMethod, DevToolsGlobalState* GlobalState, const std::string& ReturnPath, DevToolsActionWebsocketQuery::WebsocketQueryReturnType ReturnType)
{
    DevToolsActionWebsocketQuery* Result = new DevToolsActionWebsocketQuery();

    Result->Initialize("WebsocketQuery");

    int UniqueId = Rand();
    Result->SetUniqueId(UniqueId);
    Result->GetResult()->SetActionUniqueId(UniqueId);

    Result->WebSocketMethod = WebSocketMethod;
    Result->ReturnPath = ReturnPath;
    if(ReturnType == DevToolsActionWebsocketQuery::None)
    {
        Result->ReturnTypeString = "None";
    }else if(ReturnType == DevToolsActionWebsocketQuery::String)
    {
        Result->ReturnTypeString = "String";
    }else if(ReturnType == DevToolsActionWebsocketQuery::Bool)
    {
        Result->ReturnTypeString = "Bool";
    }else if(ReturnType == DevToolsActionWebsocketQuery::Number)
    {
        Result->ReturnTypeString = "Number";
    }

    Result->SetGlobalState(GlobalState);
    Result->SetId(Rand());
    Result->SetGroupId(Rand());

    return Result;
}
