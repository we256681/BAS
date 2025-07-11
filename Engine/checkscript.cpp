#include "checkscript.h"
#include <QRegularExpression>
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    CheckScript::CheckScript(QObject *parent) :
        ICheckScript(parent)

    {
    }

    bool CheckScript::Check(const QString& Script)
    {

        if(Script.contains("_if(") || Script.contains("sleep(") || Script.contains("log("))
        {
            if(!Script.contains("/*Dat:"))
            {
                return false;
            }
        }

        if(Script.contains("_BAS_DEC("))
        {
            return false;
        }

        if(!Script.contains(")!"))
        {
            return false;
        }

        if(Script.contains("_sa("))
        {
            return false;
        }

        {
            QRegularExpression re("sleep[\\n\\r\\s]*\\([\\n\\r\\s]*[\\dA-Z_]+[\\n\\r\\s]*,[\\n\\r\\s]*function[\\n\\r\\s]*\\([\\n\\r\\s]*\\)[\\n\\r\\s]*\\{");
            re.setPatternOptions(QRegularExpression::MultilineOption);
            QRegularExpressionMatch match = re.match(Script);
            if (match.hasMatch())
            {
                return false;
            }
        }

        {
            QRegularExpression re("mouse[\\n\\r\\s]*\\([\\n\\r\\s]*[\\dA-Z_]+[\\n\\r\\s]*,[\\n\\r\\s]*[\\dA-Z_]+[\\n\\r\\s]*,[\\n\\r\\s]*function[\\n\\r\\s]*\\([\\n\\r\\s]*\\)[\\n\\r\\s]*\\{");
            re.setPatternOptions(QRegularExpression::MultilineOption);
            QRegularExpressionMatch match = re.match(Script);
            if (match.hasMatch())
            {
                return false;
            }
        }




        return true;
    }

}
