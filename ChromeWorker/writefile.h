#ifndef WRITEFILE_H
#define WRITEFILE_H

#include <string>

void WriteStringToFile(const std::string& filename,const std::string& data);
void WriteStringToFile(const std::wstring& filename,const std::string& data);

#endif // WRITEFILE_H
