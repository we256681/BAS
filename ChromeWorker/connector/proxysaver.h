#ifndef PROXYSAVER_H
#define PROXYSAVER_H

#include <string>

class ProxySaver
{
private:

    std::string ParentProcessId;
    int PcapPort = 29815;

    std::string GenerateProxyConfig(const std::string& Server, int Port, bool IsHttp, const std::string& Login, const std::string& Password);
    void WriteConfigFile(const std::string& Filename, const std::string& Data);

public:
    void Initialize(const std::string &ParentProcessId, int PcapPort);

    /*Proxy*/
    void WriteProxyConfig(const std::string& Server, int Port, bool IsHttp, const std::string& Login, const std::string& Password);
    void WriteDirectConnectionConfig();
    bool IsCurrentProxyConfigEquals(const std::string& Server, int Port, bool IsHttp, const std::string& Login, const std::string& Password);

    /*Other config files*/
    void ResetAllConnections();
    void ResetDPI();
    void SetMinCapturePeriod(int MinCapturePeriod);
    void TriggerExtensionButton(const std::string& ExtensionId);

    std::wstring GetConfigFilePath(const std::string& Filename);

};

#endif // PROXYSAVER_H
