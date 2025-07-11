#ifndef ISCHEDULERINSTALLER_H
#define ISCHEDULERINSTALLER_H

#include "engine_global.h"

#include <QObject>
#include <QFile>

namespace BrowserAutomationStudioFramework
{

    class ENGINESHARED_EXPORT ISchedulerInstaller : public QObject
    {
        Q_OBJECT
        public:

            explicit ISchedulerInstaller(QObject *parent = 0);

            virtual void Install() = 0;
            virtual void Uninstall() = 0;
            virtual void Restart(QFile * LockFile) = 0;
            virtual void Run(const QString& StartType,
                             const QString& IniFileLocation = QString(),
                             const QString& ScriptData = QString(),
                             const QString& ScriptName = QString(),
                             const QString& ScriptLogin = QString(),
                             const QString& ScriptPass = QString()
                            ) = 0;
            virtual QString GetDirectory() = 0;
            virtual QString GetVersion() = 0;

    };
}


#endif // ISCHEDULERINSTALLER_H
