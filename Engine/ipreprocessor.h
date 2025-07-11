#ifndef IPREPROCESSOR_H
#define IPREPROCESSOR_H
#include "engine_global.h"

#include <QObject>
#include "iencryptor.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT IPreprocessor : public QObject
    {
        Q_OBJECT
    public:
        explicit IPreprocessor(QObject *parent = 0);
        virtual void SetEncryptor(IEncryptor* Encryptor) = 0;
        virtual IEncryptor* GetEncryptor() = 0;
        virtual void SetKey(const QByteArray& Key) = 0;

        virtual QString Preprocess(const QString& Script,int ParanoicLevel,bool IsMainScript) = 0;
        virtual QString Decrypt(const QString& Script) = 0;
        virtual QString Encrypt(const QString& Script) = 0;
    signals:

    public slots:

    };
}

#endif // IPREPROCESSOR_H
