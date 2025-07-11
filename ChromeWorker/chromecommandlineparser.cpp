#include "chromecommandlineparser.h"
#include "trim.h"
#include "split.h"
#include "converter.h"
#include <fstream>
#include <map>

std::vector<std::pair<std::string, std::string>> ParseChromeCommandLine(const std::vector<std::wstring>& AdditionalParams)
{
    return ParseCommandLine("chrome_command_line.txt", AdditionalParams);
}

std::vector<std::pair<std::string, std::string>> ParseWorkerCommandLine(const std::vector<std::wstring>& AdditionalParams)
{
    return ParseCommandLine("worker_command_line.txt", AdditionalParams);
}

std::vector<std::pair<std::string, std::string>> ParseCommandLine(const std::string& File, const std::vector<std::wstring>& AdditionalParams)
{
    std::vector<std::string> lines;

    std::ifstream fin(File);
    if(fin.is_open())
    {
        std::string line;
        while(std::getline(fin, line))
        {
            line = trim(line);
            if(!line.empty())
            {
               lines.push_back(line);
            }
        }
    }
    fin.close();

    for(const std::wstring& Item:AdditionalParams)
    {
        std::string line = ws2s(Item);
        lines.push_back(line);
    }

    std::map<std::string, std::string> res;

    for(std::string& line:lines)
    {
        if(!line.empty())
        {
           std::vector<std::string> s = split(line,'=');
           if(s.size() == 1)
           {
               std::pair<std::string, std::string> p;
               p.first = s[0];
               res[s[0]] = std::string();
           }
           if(s.size() == 2)
           {
               std::pair<std::string, std::string> p;
               p.first = s[0];
               p.second = s[1];
               res[s[0]] = s[1];
           }
        }
    }

    std::vector<std::pair<std::string, std::string>> Res;

    for(std::pair<std::string, std::string> pair : res)
    {
        Res.push_back(pair);
    }

    return Res;
}

bool HasWorkerArgument(const std::string& Argument)
{
    for(std::pair<std::string, std::string> pair : ParseWorkerCommandLine())
    {
        if(pair.first == Argument)
        {
            return true;
        }
    }
    return false;
}
