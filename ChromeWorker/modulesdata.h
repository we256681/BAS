#ifndef MODULESDATA_H
#define MODULESDATA_H

#include <string>
#include <vector>
#include <memory>
#include <map>


struct CodeDataClass
{
    std::string Code;
    std::string Name;
};

using CodeData = std::shared_ptr<CodeDataClass>;

struct ActionDataClass
{
    std::string Name;
    std::string Description;
    std::string DescriptionNoTranslation;
    std::string SuggestionEn;
    std::string SuggestionRu;
    std::string Template;
    bool IsElement;
    std::string InterfaceScript;
    std::string SelectScript;
    std::vector<CodeData> CodeScript;
};

using ActionData = std::shared_ptr<ActionDataClass>;

struct LocalizeDataClass
{
    std::map<std::string,std::string> Items;
    std::string Key;
};

using LocalizeData = std::shared_ptr<LocalizeDataClass>;

struct ModulesDataClass
{
    std::string Name;
    std::string Description;
    std::string Icon;
    std::string Info;
    std::string Folder;
    bool OnlyWithDatabase = false;
    std::vector<std::string> BrowserScripts;
    std::vector<ActionData> Actions;
    std::vector<LocalizeData> Localization;
};

using ModulesData = std::shared_ptr<ModulesDataClass>;
using ModulesDataList = std::vector<ModulesData>;

void EnableModule(const std::string& ModuleName);
ModulesDataList LoadModulesData(const std::string& Locale, const std::string& BASPid, ModulesDataList& UnusedModules);
std::string GetAllBrowserDataCode(const ModulesDataList& Modules);
bool IsModuleEnabled(const std::string& ModuleName);

#endif // MODULESDATA_H
