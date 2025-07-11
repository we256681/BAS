#ifndef EMBEDDEDDEFINITIONS_H
#define EMBEDDEDDEFINITIONS_H

#include <QString>

namespace BrowserAutomationStudioFramework
{
    struct EmbeddedLanguage
    {
        QString Name;
        QString Version;
    };

    struct EmbeddedModule
    {
        QString ModuleName;
        QString ModuleVersion;
        QString LanguageName;
        QString LanguageVersion;
    };

    struct EmbeddedCodeItem
    {
        QString Data;
        QString DataName;
        int DataType;
        QString LanguageName;
        QString LanguageVersion;
    };
}


#endif // EMBEDDEDDEFINITIONS_H
