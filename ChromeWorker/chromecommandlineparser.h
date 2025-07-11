#ifndef CHROMECOMMANDLINEPARSER_H
#define CHROMECOMMANDLINEPARSER_H

#include <vector>
#include <string>

std::vector<std::pair<std::string, std::string>> ParseCommandLine(const std::string& File, const std::vector<std::wstring>& AdditionalParams = std::vector<std::wstring>());
std::vector<std::pair<std::string, std::string>> ParseChromeCommandLine(const std::vector<std::wstring>& AdditionalParams = std::vector<std::wstring>());
std::vector<std::pair<std::string, std::string>> ParseWorkerCommandLine(const std::vector<std::wstring>& AdditionalParams = std::vector<std::wstring>());
bool HasWorkerArgument(const std::string& Argument);

#endif // CHROMECOMMANDLINEPARSER_H
