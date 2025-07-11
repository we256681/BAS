#include "moduledll.h"
#include <QByteArray>
#include <QString>
#include <QSettings>
#include <QFile>
#include <QCryptographicHash>
#include <QDir>
#include <QRegularExpression>

extern "C" {

    struct FontPackPathResult
    {
        QString Path;
        bool IsCalculated = false;
    };

    void CalculatePackPath(FontPackPathResult *ResultPath)
    {
        //Avoid repeated calculations
        if(ResultPath->IsCalculated)
        {
            return;
        }
        ResultPath->IsCalculated = true;


        QRegularExpression RegexpFolderVersion("^\\d+\\.\\d+\\.\\d+$");

        //Get root path from registry

        QSettings Settings("HKEY_CURRENT_USER\\Software\\FontPack\\FontPack", QSettings::NativeFormat);

        QString PathRoot = Settings.value("Path", QString()).toString();


        //Check if empty or not dir
        if(PathRoot.isEmpty())
        {
            return;
        }


        {
            QFileInfo FileInfo(PathRoot);
            if(!FileInfo.exists() || !FileInfo.isDir())
            {
                return;

            }
        }

        //Get all entries which matches version regexp
        QDir RootDir = QDir(PathRoot);
        QStringList FolderList = RootDir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
        QStringList FilteredList;
        for(QString CurrentDir: FolderList)
        {
            if(RegexpFolderVersion.match(CurrentDir).hasMatch())
            {
                FilteredList.append(CurrentDir);
                ResultPath->Path = CurrentDir;

            }
        }

        if(FilteredList.isEmpty())
        {
            return;
        }


        //Get highest version
        QString Result;
        for(QString CurrentDir: FilteredList)
        {
            if(Result.isEmpty())
            {
                Result = CurrentDir;
                continue;
            }

            QStringList ds = CurrentDir.split(".");
            QStringList rs = Result.split(".");

            int l = qMin(ds.length(),rs.length());
            for(int i = 0;i<l;i++)
            {
                int di = ds.at(i).toInt();
                int ri = rs.at(i).toInt();
                if(di>ri)
                {
                    Result = CurrentDir;
                    break;
                }else if(di<ri)
                {
                    break;
                }

            }

        }

        if(Result.isEmpty())
        {
            return;
        }

        //Add root path to folder version and return
        Result = PathRoot + QString("/") + Result + QString("/");
        Result = QDir::cleanPath(Result);
        if(!Result.endsWith("/"))
        {
            Result.append("/");
        }
        ResultPath->Path = Result;

    }

    void* StartDll()
    {
        FontPackPathResult *ResultPath = new FontPackPathResult();
        return ResultPath;
    }


    void EndDll(void * DllData)
    {
        FontPackPathResult * Result = (FontPackPathResult *)DllData;
        delete Result;
    }

    void* StartThread()
    {
        return 0;
    }

    void EndThread(void * DllData)
    {
    }

    void GetFontPackPath(char *InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool *NeedToStop, bool* WasError)
    {
        FontPackPathResult * Result = (FontPackPathResult *)DllData;

        CalculatePackPath(Result);

        QString ResString = Result->Path;

        QByteArray ResArray = ResString.toUtf8();

        char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
        memcpy(ResMemory, ResArray.data(), ResArray.size() );
    }

}
