#include "webdriverchromewindowhide.h"
#include <QDebug>
#include <QTimer>

#include "every_cpp.h"
#include <windows.h>

namespace BrowserAutomationStudioFramework
{

    BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
    {
        QList<QString>* PIDS = ((QList<QString>*)lParam);

        unsigned long ProcessIdActualInt;
        GetWindowThreadProcessId(handle, &ProcessIdActualInt);
        QString ProcessIdActualString = QString::number(ProcessIdActualInt);
        if(!PIDS->contains(ProcessIdActualString))
        {
            return true;
        }

        std::string ClassNameStdString;
        ClassNameStdString.resize(1000);
        int size = GetClassNameA(handle,(char *)ClassNameStdString.data(),1000);
        ClassNameStdString = std::string(ClassNameStdString.data(),size);

        if(ClassNameStdString == "Chrome_WidgetWin_1")
        {
            ShowWindow(handle, SW_HIDE);
        }

        return true;
    }


    WebDriverChromeWindowHide::WebDriverChromeWindowHide(QObject *parent) : QObject(parent)
    {

    }

    void WebDriverChromeWindowHide::Hide()
    {
        EnumWindows(EnumWindowsCallback, (LPARAM)(&PIDS));
    }

    void WebDriverChromeWindowHide::HideAndStart()
    {
        Hide();
        Start();
    }

    void WebDriverChromeWindowHide::Start()
    {
        Hide();
        QTimer::singleShot(500,this,SLOT(HideAndStart()));
    }

    void WebDriverChromeWindowHide::AddPID(const QString& PID)
    {
        PIDS.append(PID);
    }
    void WebDriverChromeWindowHide::RemovePID(const QString& PID)
    {
        PIDS.removeAll(PID);
    }

}
