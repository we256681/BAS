#include "moduledll.h"
#include <stdio.h>
#include <QString>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QDateTime>
#include <QTextStream>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDirIterator>
#include <QProcess>


extern "C" {

    void* StartDll()
    {

        QDir dir(".");
        dir.setNameFilters(QStringList() << "temp_*.bat");
        dir.setFilter(QDir::Files);
        foreach(QString dirFile, dir.entryList())
        {
            dir.remove(dirFile);
        }
        return 0;
    }



    void EndDll(void * DllData)
    {

    }

    void* StartThread()
    {
        return 0;
    }

    void EndThread(void * DllData)
    {

    }


    void ProcessManagerRunExe(char *InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool *NeedToStop, bool* WasError)
    {

        QString location;
        QString arguments;
        QString directory;
        int version = 1;
        bool waitfinish;
        {
            QJsonDocument InputObject;
            QJsonParseError err;
            InputObject = QJsonDocument::fromJson(QByteArray(InputJson),&err);
            if(err.error)
                return;
            location = InputObject.object()["location"].toString();
            arguments = InputObject.object()["arguments"].toString();

            if(InputObject.object().contains("version"))
                version = InputObject.object()["version"].toInt();

            if(InputObject.object().contains("working_folder"))
                directory = InputObject.object()["working_folder"].toString();

            waitfinish = InputObject.object()["waitfinish"].toBool();
        }



        if(!waitfinish)
        {
            QProcess *Process = new QProcess();
            QObject::connect(Process,SIGNAL(finished(int)),Process,SLOT(deleteLater()));

            if(arguments.isEmpty())
            {
                Process->setArguments(QStringList());
            }else
            {
                Process->setArguments(arguments.split(" "));
            }

            if(directory.isEmpty())
            {
                Process->setWorkingDirectory(QFileInfo(location).absoluteDir().absolutePath());
            }else
            {
                Process->setWorkingDirectory(directory);
            }
            Process->start(location);

            return;
        }


        QProcess p;
        if(directory.isEmpty())
        {
            p.setWorkingDirectory(QFileInfo(location).absoluteDir().absolutePath());
        }else
        {
            p.setWorkingDirectory(directory);
        }

        if(arguments.isEmpty())
        {
            p.start(location);
        }else
            p.start(location,arguments.split(" "));



        while(p.state() != QProcess::NotRunning)
        {
            if(*NeedToStop)
            {
                p.kill();
                return;
            }
            QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
        }

        QByteArray ResArray;
        if(version == 1)
        {
            QString res = p.readAllStandardOutput();
            ResArray = res.toUtf8();
        }else if(version == 2)
        {
            QString res = p.readAllStandardOutput().toBase64() + "," + p.readAllStandardError().toBase64();
            ResArray = res.toUtf8();
        }

        char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
        memcpy(ResMemory, ResArray.data(), ResArray.size() );

    }

}
