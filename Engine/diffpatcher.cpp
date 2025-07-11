#include "diffpatcher.h"
#include "diff_match_patch.h"
#include <QList>
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    DiffPatcher::DiffPatcher(QObject *parent) :  IDiffPatcher(parent)
    {

    }

    void DiffPatcher::Init(const QString& Text)
    {
        LastText = Text;

    }

    QString DiffPatcher::CurrentText()
    {
        return LastText;
    }


    QString DiffPatcher::ApplyPatch(const QString& PatchString)
    {
        QString PatchStringCopy = PatchString;

        int PrefixLengthIndex = PatchStringCopy.indexOf("@@@");
        int PrefixLength = PatchStringCopy.mid(0,PrefixLengthIndex).toInt();
        PatchStringCopy = PatchStringCopy.remove(0,PrefixLengthIndex + 3);


        int SuffixLengthIndex = PatchStringCopy.indexOf("@@@");
        int SuffixLength = PatchStringCopy.mid(0,SuffixLengthIndex).toInt();
        PatchStringCopy = PatchStringCopy.remove(0,SuffixLengthIndex + 3);

        int TypeIndex = PatchStringCopy.indexOf("@@@");
        int Type = PatchStringCopy.mid(0,TypeIndex).toInt();
        PatchStringCopy = PatchStringCopy.remove(0,TypeIndex + 3);

        if(Type == 0)
        {
            // Diff match patch used
            QString LastTextCopy = LastText.mid(PrefixLength,LastText.length() - PrefixLength - SuffixLength);

            diff_match_patch diff;
            QList<Patch> PatchObject = diff.patch_fromText(PatchStringCopy);
            LastTextCopy = diff.patch_apply(PatchObject, LastTextCopy).first;
            LastText.replace(PrefixLength,LastText.length() - PrefixLength - SuffixLength,LastTextCopy);
        }else
        {
            // Native replace used
            LastText.replace(PrefixLength,LastText.length() - PrefixLength - SuffixLength,PatchStringCopy);
        }


        return LastText;
    }


}
