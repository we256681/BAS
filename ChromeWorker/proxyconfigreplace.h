#ifndef PROXYCONFIGREPLACE_H
#define PROXYCONFIGREPLACE_H
#include <string>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <map>

class ProxyConfigReplace
{


public:
    static ProxyConfigReplace& GetInstance()
    {
        static ProxyConfigReplace instance;
        return instance;
    }
private:
    ProxyConfigReplace(){}
    ProxyConfigReplace(ProxyConfigReplace const&);
    void operator=(ProxyConfigReplace const&);
public:
    void Replace();
    void Disable();
    void SetPid(const std::wstring& Pid);

};

#endif // PROXYCONFIGREPLACE_H
