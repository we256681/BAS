#include "fingerprintdetector.h"
#include <windows.h>
#include <fstream>
#include <stdio.h>
#include "picojson.h"
#include "converter.h"


FingerprintDetector::FingerprintDetector()
{
    LastGroupCount = 0;
}

void FingerprintDetector::Init(const std::string& UniqueId)
{
    IsInitialized = true;
    this->UniqueId = UniqueId;
}

int FingerprintDetector::GetLastGroupCount()
{
    return LastGroupCount;
}


int FingerprintDetector::Timer(CefRefPtr<CefBrowser> DetectorBrowser, bool Initialized)
{
    int Res = 0;
    if(!IsInitialized)
        return Res;

    time_t now = time(0);
    if(LastTimer != 0 && now - LastTimer < 5)
        return Res;

    LastTimer = now;

    std::string ToSend;

    std::string MutexName = std::string("FINGERPRINTDETECTOR_") + UniqueId;
    HANDLE MutexHandler = CreateMutexA(0,false,MutexName.c_str());
    WaitForSingleObject(MutexHandler,INFINITE);

        std::vector<std::string> Data = DataPending;
        DataPending.clear();

        try{
            std::string Filename = std::string("s/") + UniqueId + std::string(".detect");
            std::ifstream Detect(Filename);
            std::string Line;
            while(std::getline(Detect, Line))
            {
                int Level = 0;
                if(!Line.empty())
                {
                    Level = std::stoi(Line.substr(0,1));
                    Line.erase(0,1);
                }

                Data.push_back(Line);

                LastGroupCount ++;

                if(Level >= 5)
                    Res += 1;
            }
            Detect.close();
            remove(Filename.c_str());
        }catch(...)
        {
        }

        if(!Data.empty())
        {
            if(DetectorBrowser && Initialized)
            {
                std::string DataJoinded;
                int Index = Data.size() - 5000;
                if(Index > 0)
                    Data.erase(Data.begin(),Data.begin()+Index);
                for(std::string& d:Data)
                {
                    if(!DataJoinded.empty())
                        DataJoinded += ",";
                    DataJoinded += d;
                }
                ToSend = std::string("BrowserAutomationStudio_NewFingerprintData('") + DataJoinded + std::string("');");
            }
            else
            {
                int Index = Data.size() - 5000;
                if(Index > 0)
                    Data.erase(Data.begin(),Data.begin()+Index);
                DataPending = Data;
            }
        }

        if(DetectorBrowser && Initialized)
        {
            ToSend += DataPending2;
            DataPending2.clear();
        }

    ReleaseMutex(MutexHandler);
    CloseHandle(MutexHandler);

    if(!ToSend.empty())
        DetectorBrowser->GetMainFrame()->ExecuteJavaScript(ToSend,"",0);

    return Res;
}

void FingerprintDetector::BrowserData(CefRefPtr<CefBrowser> DetectorBrowser, const std::string& Data, bool Initialized)
{
    if(!IsInitialized)
        return;

    LastGroupCount = 0;

    std::string script = std::string("BrowserAutomationStudio_NewBrowserData(" + picojson::value(Data).serialize() + std::string(");"));

    DataPending2 += script;
}
