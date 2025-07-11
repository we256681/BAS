#ifndef NONEENCRYPTOR_H
#define NONEENCRYPTOR_H
#include <QObject>

#include "engine_global.h"
#include "iencryptor.h"


namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT NoneEncryptor : public IEncryptor
    {
        Q_OBJECT
    public:
        explicit NoneEncryptor(QObject *parent = 0);

        virtual void GenerateAsymmetricKeys(QByteArray& PublicKeyData,QByteArray & PrivateKeyData);

        virtual QByteArray AsymmetricEncrypt(const QByteArray& Data,const QByteArray& PublicKeyData);

        virtual QByteArray AsymmetricDecrypt(const QByteArray& Data,const QByteArray& PrivateKeyData);

        virtual QByteArray GenerateSymmetricKey();

        virtual QByteArray SymmetricEncrypt(const QByteArray& Data,const QByteArray& KeyData);

        virtual QByteArray SymmetricDecrypt(const QByteArray& Data,const QByteArray& KeyData);

        virtual QByteArray HybridEncrypt(const QByteArray& Data,const QByteArray& PublicKeyData);

        virtual QByteArray HybridDecrypt(const QByteArray& Data,const QByteArray& PrivateKeyData);

        virtual QByteArray AsymmetricSign(const QByteArray& Data,const QByteArray& PrivateKeyData);

        virtual bool AsymmetricVerify(const QByteArray& Message,const QByteArray& Signature,const QByteArray& PublicKeyData);

    signals:

    public slots:

    };
}

#endif // NONEENCRYPTOR_H
