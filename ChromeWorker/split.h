#ifndef SPLIT_H
#define SPLIT_H
#include <string>
#include <sstream>
#include <vector>

std::vector<std::string> split(const std::string &s, char delim);
std::vector<std::wstring> split(const std::wstring &s, wchar_t delim);


#endif // SPLIT_H
