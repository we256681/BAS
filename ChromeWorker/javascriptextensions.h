#ifndef JAVASCRIPTEXTENSIONS_H
#define JAVASCRIPTEXTENSIONS_H

#include <string>

class JavaScriptExtensions
{
public:
    JavaScriptExtensions();

    std::string GetBasicExtension(bool IsRecord);

    std::string GetHideExtension(const std::string& UniqueProcessId);
    std::string ProcessJs(const std::string& Script, const std::string& UniqueProcessId);
};

#endif // JAVASCRIPTEXTENSIONS_H
