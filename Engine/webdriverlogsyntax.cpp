#include "webdriverlogsyntax.h"
#include <QRegularExpression>
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    WebDriverLogSyntax::WebDriverLogSyntax(QObject *parent) :
        ILogSyntax(parent)
    {
    }

    QString WebDriverLogSyntax::Process(const QString& Line)
    {
        QRegularExpression re("^\\s*(?<prefix>CommunicatorFactory|ProcessCommunicator\\s*\\[\\d+\\])?\\s*(?<target>bas received|mla received|mla sent|bas sent)?\\s*(?<brackets1>\\[[^\\]]+\\])?\\s*(?<brackets2>\\[[^\\]]+\\])?(?<main>.*)");
        re.setPatternOptions(QRegularExpression::MultilineOption);

        QRegularExpressionMatch match = re.match(Line);
        if(match.hasMatch())
        {
            QString Res = Line;
            if(match.capturedStart("main")>=0)
            {
                Res.replace(match.capturedStart("main"),match.capturedLength("main"),match.captured("main").replace("<","&lt;").replace(">","&gt;"));
            }
            if(match.capturedStart("brackets2")>=0)
            {
                Res.replace(match.capturedStart("brackets2"),match.capturedLength("brackets2"),QString("<span style='text-decoration: underline;'>") + match.captured("brackets2") + QString("</span>"));
            }
            if(match.capturedStart("brackets1")>=0)
            {
                Res.replace(match.capturedStart("brackets1"),match.capturedLength("brackets1"),QString("<span style='text-decoration: underline;'>") + match.captured("brackets1") + QString("</span>"));
            }
            if(match.capturedStart("target")>=0)
            {
                QString Target = match.captured("target");
                QString Color;
                if(Target == "bas received")
                {
                    Color = "#f9f474";
                }else if(Target == "bas sent")
                {
                    Color = "#f975e3";
                }else if(Target == "mla received")
                {
                    Color = "#41f48f";
                }else if(Target == "mla sent")
                {
                    Color = "#4292f4";
                }
                Res.replace(match.capturedStart("target"),match.capturedLength("target"),QString("<span style='color:") + Color + QString("'>") + match.captured("target") + QString("</span>"));
            }
            if(match.capturedStart("prefix")>=0)
            {
                Res.replace(match.capturedStart("prefix"),match.capturedLength("prefix"),QString("<span style='color:gray'>") + match.captured("prefix") + QString("</span>"));
            }
            return Res;
        }else
            return Line;
    }

}
