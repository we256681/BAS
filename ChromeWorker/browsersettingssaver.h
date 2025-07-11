#ifndef BROWSERSETTINGSSAVER_H
#define BROWSERSETTINGSSAVER_H

#include <string>
#include <vector>

struct BrowserSettingsSaver
{
    //Setting to save files
    std::string FilePath;

    bool Detector;
    bool TemporaryDisableDetector;
    bool IsProxySet;

    //Languages
    std::vector<std::string> Languages;
    std::string LanguagesHeader;
    std::string UserAgent;
    std::string UserAgentDataBase64;

    std::vector<std::pair<bool, std::string> > RequestMask;
    std::string StartupScript;

    //Record mode
    bool IsRecord;

    double DeviceScaleFactor = 1.0;

    //Headers
    std::vector<std::pair<std::string, std::string> > Headers;

    void Save();
};

#endif // BROWSERSETTINGSSAVER_H
