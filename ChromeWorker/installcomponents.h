#ifndef INSTALLCOMPONENTS_H
#define INSTALLCOMPONENTS_H

#include <string>

void InstallWidevine(const std::wstring& ProfilePathAbsolute, int BrowserVersionId);
void InstallSafeBrowsing(const std::wstring& ProfilePathAbsolute, int BrowserVersionId);
void InstallComponents(const std::wstring& ProfilePathAbsolute, int BrowserVersionId);
void DeinstallWidevine(const std::wstring& ProfilePathAbsolute);
void DeinstallSafeBrowsing(const std::wstring& ProfilePathAbsolute);
void DeinstallComponents(const std::wstring& ProfilePathAbsolute);

#endif // INSTALLCOMPONENTS_H
