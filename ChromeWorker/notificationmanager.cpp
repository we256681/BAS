#include "notificationmanager.h"
#include "readallfile.h"

void NotificationManager::Init(const std::string& UniqueId)
{
    this->UniqueId = UniqueId;
}

void NotificationManager::Timer(CefRefPtr<CefBrowser> BrowserToolbox)
{
    std::string Filename = std::string("s/") + UniqueId + std::string(".notification");
    try
    {
        std::string NotificationLabel = ReadAllString(Filename);
        if(!NotificationLabel.empty())
        {
            BrowserToolbox->GetMainFrame()->ExecuteJavaScript(std::string("BrowserAutomationStudio_Notify('notification')"),BrowserToolbox->GetMainFrame()->GetURL(), 0);
            remove(Filename.c_str());
        }
    }catch(...)
    {

    }

}
