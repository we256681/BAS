#ifndef PREPARESTARTUPSCRIPT_H
#define PREPARESTARTUPSCRIPT_H

#include <string>
#include "browserdata.h"


std::string PrepareConstantStartupScript(BrowserData* Data);
std::string PrepareMutableStartupScript(BrowserData* Data);
std::vector<std::pair<std::string,std::string> > PrepareHeaders(BrowserData* Data);
void UpdateBrowserData(BrowserData* Data);

#endif // PREPARESTARTUPSCRIPT_H
