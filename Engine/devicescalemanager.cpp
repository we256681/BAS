#include "devicescalemanager.h"
#include <windows.h>
//#include <shellscalingapi.h>
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    DeviceScaleManager::DeviceScaleManager(QObject *parent) :
        IDeviceScaleManager(parent)
    {

    }

    //May return incorrect values
    //Uncomment shellscalingapi.h to test

    /*typedef HRESULT(__stdcall *GetScaleFactorForMonitorPointer)(HMONITOR, DEVICE_SCALE_FACTOR*);


    double DeviceScaleManager::GetScaleFactor()
    {
        HWND hWnd = GetDesktopWindow();

        HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);


        double Result = 1.0;

        HMODULE hLib = LoadLibrary(L"Shcore.dll");
        if(hLib)
        {
            GetScaleFactorForMonitorPointer getScaleFactorForMonitor = (GetScaleFactorForMonitorPointer)GetProcAddress(hLib, "GetScaleFactorForMonitor");

            DEVICE_SCALE_FACTOR devScaleFactor;

            HRESULT res = getScaleFactorForMonitor(hMonitor, &devScaleFactor);

            if(res != S_OK)
            {
                FreeLibrary(hLib);
                return 1.0;
            }

            Result = devScaleFactor * 0.01;

            if(Result <= 0.0)
                return 1.0;

            FreeLibrary(hLib);
        }

        return Result;
    }*/

    double DeviceScaleManager::GetScaleFactor()
    {
        QByteArray OldValue = qgetenv("QT_SCALE_FACTOR");

        if(!OldValue.isEmpty())
        {
            double CachedResult = QString::fromUtf8(OldValue).toDouble();
            return CachedResult;
        }

        HWND hWnd = GetDesktopWindow();

        HDC hDc = GetDC(hWnd);
        int LogicalScreenHeight = GetDeviceCaps(hDc, VERTRES);
        int PhysicalScreenHeight = GetDeviceCaps(hDc, DESKTOPVERTRES);

        if(LogicalScreenHeight <= 0)
            return 1.0;

        if(PhysicalScreenHeight <= 0)
            return 1.0;

        double Result = (double)PhysicalScreenHeight/(double)LogicalScreenHeight;

        if(Result <= 0.0)
            return 1.0;

        return Result;
    }

    void DeviceScaleManager::Autoscale()
    {
        //This doesn't may detect scale factor incorrectly because of https://bugreports.qt.io/browse/QTBUG-53022
        //QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

        QString Value = QString::number(GetScaleFactor(), 'f', 2);
        qputenv("QT_SCALE_FACTOR", Value.toUtf8());
    }

}
