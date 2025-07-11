#include "browsersettingssaver.h"
#include <fstream>
#include "picojson.h"
#include "base64.h"
#include "replaceall.h"

void BrowserSettingsSaver::Save()
{
    try
    {
        std::ofstream outfile(FilePath);
        if(outfile.is_open())
        {
            std::string RequestMaskString;

            picojson::value::array RequestMaskJson;
            for (const std::pair<bool, std::string>& Mask: RequestMask)
            {
                picojson::value::object MaskJson;
                MaskJson["allow"] = picojson::value(Mask.first);
                MaskJson["data"] = picojson::value(Mask.second);
                RequestMaskJson.push_back(picojson::value(MaskJson));
            }

            RequestMaskString = picojson::value(RequestMaskJson).serialize();

            outfile<<"RequestMasks="<<base64_encode((unsigned char const *)RequestMaskString.data(),RequestMaskString.size())<<std::endl;


            std::string Language;
            if(!Languages.empty())
                Language = Languages[0];
            std::string LanguagesJsonString;

            picojson::value::array res;
            for (const std::string& Lang: Languages)
            {
                res.push_back(picojson::value(Lang));
            }

            LanguagesJsonString = picojson::value(res).serialize();

            outfile<<"Attribute.language="<<Language<<std::endl;
            outfile<<"Attribute.languages="<<LanguagesJsonString<<std::endl;
            if(!UserAgent.empty())
            {
                outfile<<"Attribute.userAgent="<<UserAgent<<std::endl;
                outfile<<"Attribute.vendor="<<"Google Inc."<<std::endl;
                outfile<<"Attribute.platform="<<"Win32"<<std::endl;
                outfile<<"Attribute.appVersion="<<ReplaceAll(UserAgent, "Mozilla/", "")<<std::endl;
            }

            if(!UserAgentDataBase64.empty())
                outfile<<"UserAgentData="<<UserAgentDataBase64<<std::endl;


            outfile<<"FingerprintDetectorEnabled="<<((IsRecord && Detector && !TemporaryDisableDetector) ? "true" : "false")<<std::endl;
            outfile<<"NotificationsInfo="<<((IsRecord) ? "Enable" : "Disable")<<std::endl;
            outfile<<"IsProxySet="<<((IsProxySet) ? "Enable" : "Disable")<<std::endl;

            std::string AdditionalHeaders;
            picojson::value::array AdditionalHeadersArray;

            for(std::pair<std::string, std::string>& Header:Headers)
            {
                AdditionalHeadersArray.push_back(picojson::value(Header.first));
                if(Header.first == "Accept-Language")
                {
                    AdditionalHeadersArray.push_back(picojson::value(LanguagesHeader));
                }else
                {
                    AdditionalHeadersArray.push_back(picojson::value(Header.second));
                }
            }

            AdditionalHeaders = picojson::value(AdditionalHeadersArray).serialize();


            outfile<<"AdditionalHeaders="<<base64_encode((unsigned char const *)AdditionalHeaders.data(),AdditionalHeaders.size())<<std::endl;

            outfile<<"StartupScript="<<base64_encode((unsigned char const *)StartupScript.data(),StartupScript.size())<<std::endl;

            outfile<<"LocaleName="<<Language<<std::endl;

            if(DeviceScaleFactor >= 1.01 || DeviceScaleFactor <= 0.99)
            {
                outfile<<"ForceDeviceScaleFactor="<<DeviceScaleFactor<<std::endl;
            }

        }
        outfile.close();
    }catch(...)
    {

    }
}
