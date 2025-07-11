#ifndef GENERATEJSONMENU_H
#define GENERATEJSONMENU_H

#include <string>
#include <vector>
#include "modulesdata.h"

std::string GenerateJsonMenu(bool IsImageSelect, int MousePositionX, int MousePositionY, const std::vector<std::string>& Tabs, const ModulesDataList& Modules, const ModulesDataList& UnusedModules);

#endif // GENERATEJSONMENU_H
