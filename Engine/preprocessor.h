#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include "engine_global.h"

#include "ipreprocessor.h"
namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT Preprocessor : public IPreprocessor
    {
        Q_OBJECT
        bool IsInsideComment(const QString& str,int start);

        struct GotoLabelData
        {
        bool IsGotoLabel;
        int Index;
        QString Label;
        };
        GotoLabelData ParseSetGotoLabel(const QString& str,int start);
        QString Encrypt(const QString& Script,int ParanoicLevel);
        IEncryptor* Encryptor;
        QByteArray Key;
        int EncryptIterator;
        bool IsRecord;

        struct ParsedMainScript
        {
            QString MainScript;
            QStringList Functions;
        };

        ParsedMainScript ParseMainScript(const QString& Script);
        void PreprocessInternal(QString& Res,int ParanoicLevel,QMap<QString,QString>& GotoData);
        void PreprocessSections(QString& Res);

    public:
        explicit Preprocessor(QObject *parent = 0);
        virtual void SetEncryptor(IEncryptor* Encryptor);
        virtual IEncryptor* GetEncryptor();
        virtual void SetKey(const QByteArray& Key);

        void SetIsRecord(bool IsRecord);
        bool GetIsRecord();

        virtual QString Preprocess(const QString& Script,int ParanoicLevel,bool IsMainScript);

        virtual QString Decrypt(const QString& Script);
        virtual QString Encrypt(const QString& Script);

    signals:

    public slots:

    };
}

#endif // PREPROCESSOR_H
