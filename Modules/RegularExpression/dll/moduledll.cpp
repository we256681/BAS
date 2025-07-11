#include "moduledll.h"
#include <QVariantMap>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QRegularExpression>

extern "C" {

    void* StartDll()
    {
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

    void RegexpFirstMatch(char *InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool *NeedToStop, bool* WasError)
    {
        QJsonDocument InputObject;
        QJsonParseError err;
        InputObject = QJsonDocument::fromJson(QByteArray(InputJson),&err);
        if(err.error)
            return;

        if(!InputObject.object().contains("text"))
            return;

        if(!InputObject.object().contains("regexp"))
            return;

        QString Text = InputObject.object()["text"].toString();
        QString RegexpString = InputObject.object()["regexp"].toString();

        QRegularExpression Regexp(RegexpString);
        Regexp.setPatternOptions(QRegularExpression::MultilineOption);
        Regexp.setPatternOptions(QRegularExpression::UseUnicodePropertiesOption);
        QRegularExpressionMatch match = Regexp.match(Text);
        if (match.hasMatch())
        {
            QStringList list = match.capturedTexts();
            if(list.size() >= 2)
            {
                QString first = list.first();
                list.removeFirst();
                list.append(first);
            }
            QByteArray ResArray = QJsonDocument::fromVariant(QVariant(list)).toJson(QJsonDocument::Compact);
            char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
            memcpy(ResMemory, ResArray.data(), ResArray.size());
        }

    }

    void RegexpIsMatch(char *InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool *NeedToStop, bool* WasError)
    {
        QJsonDocument InputObject;
        QJsonParseError err;
        InputObject = QJsonDocument::fromJson(QByteArray(InputJson),&err);
        if(err.error)
            return;

        if(!InputObject.object().contains("text"))
            return;

        if(!InputObject.object().contains("regexp"))
            return;

        QString Text = InputObject.object()["text"].toString();
        QString RegexpString = InputObject.object()["regexp"].toString();

        QRegularExpression Regexp(RegexpString);
        Regexp.setPatternOptions(QRegularExpression::MultilineOption);
        Regexp.setPatternOptions(QRegularExpression::UseUnicodePropertiesOption);
        QRegularExpressionMatch match = Regexp.match(Text);
        if(match.hasMatch())
        {
            QByteArray ResArray = QString("true").toUtf8();
            char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
            memcpy(ResMemory, ResArray.data(), ResArray.size());
        }

    }

    void RegexpSplit(char *InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool *NeedToStop, bool* WasError)
    {
        QJsonDocument InputObject;
        QJsonParseError err;
        InputObject = QJsonDocument::fromJson(QByteArray(InputJson),&err);
        if(err.error)
            return;

        if(!InputObject.object().contains("text"))
            return;

        if(!InputObject.object().contains("regexp"))
            return;

        QString Text = InputObject.object()["text"].toString();
        QString RegexpString = InputObject.object()["regexp"].toString();

        QRegularExpression Regexp(RegexpString);
        Regexp.setPatternOptions(QRegularExpression::MultilineOption);
        Regexp.setPatternOptions(QRegularExpression::UseUnicodePropertiesOption);
        QByteArray ResArray = QJsonDocument::fromVariant(QVariant(Text.split(Regexp, QString::SkipEmptyParts))).toJson(QJsonDocument::Compact);
        char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
        memcpy(ResMemory, ResArray.data(), ResArray.size());


    }

    void RegexpScan(char *InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool *NeedToStop, bool* WasError)
    {
        QJsonDocument InputObject;
        QJsonParseError err;
        InputObject = QJsonDocument::fromJson(QByteArray(InputJson),&err);
        if(err.error)
            return;

        if(!InputObject.object().contains("text"))
            return;

        if(!InputObject.object().contains("regexp"))
            return;

        QString Text = InputObject.object()["text"].toString();
        QString RegexpString = InputObject.object()["regexp"].toString();

        QRegularExpression Regexp(RegexpString);
        Regexp.setPatternOptions(QRegularExpression::MultilineOption);
        Regexp.setPatternOptions(QRegularExpression::UseUnicodePropertiesOption);

        QRegularExpressionMatchIterator i = Regexp.globalMatch(Text);
        QStringList Result;
        while (i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            QString res = match.captured("result");
            if(res.isNull())
            {
                if(match.lastCapturedIndex() >= 1)
                {
                    Result.append(match.captured(1));
                }else
                {
                    Result.append(match.captured());
                }
            }else
            {
                Result.append(res);
            }

        }

        QByteArray ResArray = QJsonDocument::fromVariant(QVariant(Result)).toJson(QJsonDocument::Compact);
        char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
        memcpy(ResMemory, ResArray.data(), ResArray.size());
    }

    void RegexpReplace(char *InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool *NeedToStop, bool* WasError)
    {
        QJsonDocument InputObject;
        QJsonParseError err;
        InputObject = QJsonDocument::fromJson(QByteArray(InputJson),&err);
        if(err.error)
            return;

        if(!InputObject.object().contains("text"))
            return;

        if(!InputObject.object().contains("regexp"))
            return;

        if(!InputObject.object().contains("replace"))
            return;

        QString Text = InputObject.object()["text"].toString();
        QString RegexpString = InputObject.object()["regexp"].toString();
        QString Replace = InputObject.object()["replace"].toString();

        QRegularExpression Regexp(RegexpString);
        Regexp.setPatternOptions(QRegularExpression::MultilineOption);
        Regexp.setPatternOptions(QRegularExpression::UseUnicodePropertiesOption);

        QByteArray ResArray = Text.replace(Regexp,Replace).toUtf8();
        char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
        memcpy(ResMemory, ResArray.data(), ResArray.size());
    }


}
