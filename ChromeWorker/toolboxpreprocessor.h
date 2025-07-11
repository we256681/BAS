#ifndef TOOLBOXPREPROCESSOR_H
#define TOOLBOXPREPROCESSOR_H

#include "modulesdata.h"

void ToolboxPreprocess(const ModulesDataList & Modules, const ModulesDataList & UnusedModules, std::string& OriginalScript);
void ScenarioPreprocess(const ModulesDataList & Modules, std::string& OriginalScript);


#endif // TOOLBOXPREPROCESSOR_H
