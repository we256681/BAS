#ifndef FINGERPRINTDETECTOR_H
#define FINGERPRINTDETECTOR_H

#include <string>
#include "include/cef_app.h"
#include <atomic>
#include <vector>

class FingerprintDetector
{
    std::string UniqueId;
    bool IsInitialized = false;
    time_t LastTimer = 0;
    std::vector<std::string> DataPending;
    std::string DataPending2;
    std::atomic_int LastGroupCount;
public:
    FingerprintDetector();
    void Init(const std::string& UniqueId);

    int Timer(CefRefPtr<CefBrowser> DetectorBrowser, bool Initialized);
    void BrowserData(CefRefPtr<CefBrowser> DetectorBrowser, const std::string& Data, bool Initialized);

    int GetLastGroupCount();
};

#endif // FINGERPRINTDETECTOR_H
