#include "noneencryptor.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    NoneEncryptor::NoneEncryptor(QObject *parent) :
        IEncryptor(parent)
    {
    }

    void NoneEncryptor::GenerateAsymmetricKeys(QByteArray& PublicKeyData,QByteArray & PrivateKeyData)
    {

    }

    QByteArray NoneEncryptor::AsymmetricEncrypt(const QByteArray& Data,const QByteArray& PublicKeyData)
    {
        return Data;
    }

    QByteArray NoneEncryptor::AsymmetricDecrypt(const QByteArray& Data,const QByteArray& PrivateKeyData)
    {
        return Data;
    }

    QByteArray NoneEncryptor::GenerateSymmetricKey()
    {
        return QByteArray();
    }

    QByteArray NoneEncryptor::SymmetricEncrypt(const QByteArray& Data,const QByteArray& KeyData)
    {
        return Data;
    }

    QByteArray NoneEncryptor::SymmetricDecrypt(const QByteArray& Data,const QByteArray& KeyData)
    {
        return Data;
    }

    QByteArray NoneEncryptor::HybridEncrypt(const QByteArray& Data,const QByteArray& PublicKeyData)
    {
        return Data;
    }

    QByteArray NoneEncryptor::HybridDecrypt(const QByteArray& Data,const QByteArray& PrivateKeyData)
    {
        return Data;
    }

    QByteArray NoneEncryptor::AsymmetricSign(const QByteArray& Data,const QByteArray& PrivateKeyData)
    {
        return QByteArray();
    }

    bool NoneEncryptor::AsymmetricVerify(const QByteArray& Message,const QByteArray& Signature,const QByteArray& PublicKeyData)
    {
        return true;
    }
}
