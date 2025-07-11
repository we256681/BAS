#ifndef IENCRYPTOR_H
#define IENCRYPTOR_H

#include <QObject>

#include "engine_global.h"


namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT IEncryptor : public QObject
    {
        Q_OBJECT
    public:
        explicit IEncryptor(QObject *parent = 0);

        virtual void GenerateAsymmetricKeys(QByteArray& PublicKeyData,QByteArray & PrivateKeyData) = 0;

        virtual QByteArray AsymmetricEncrypt(const QByteArray& Data,const QByteArray& PublicKeyData) = 0;

        virtual QByteArray AsymmetricDecrypt(const QByteArray& Data,const QByteArray& PrivateKeyData) = 0;

        virtual QByteArray GenerateSymmetricKey() = 0;

        virtual QByteArray SymmetricEncrypt(const QByteArray& Data,const QByteArray& KeyData) = 0;

        virtual QByteArray SymmetricDecrypt(const QByteArray& Data,const QByteArray& KeyData) = 0;

        virtual QByteArray HybridEncrypt(const QByteArray& Data,const QByteArray& PublicKeyData) = 0;

        virtual QByteArray HybridDecrypt(const QByteArray& Data,const QByteArray& PrivateKeyData) = 0;

        virtual QByteArray AsymmetricSign(const QByteArray& Data,const QByteArray& PrivateKeyData) = 0;

        virtual bool AsymmetricVerify(const QByteArray& Message,const QByteArray& Signature,const QByteArray& PublicKeyData) = 0;
    signals:

    public slots:

    };
}

#endif // IENCRYPTOR_H
