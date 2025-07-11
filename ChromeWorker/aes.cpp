#include "aes.h"
#include <openssl/aes.h>
#include <vector>

std::string encrypt_aes_cfb_128(const std::string& text, const std::string& key, const std::string& iv)
{
    AES_KEY EncryptKey;
    AES_set_encrypt_key(reinterpret_cast<const unsigned char *>(key.data()), 128, &EncryptKey);

    std::vector<unsigned char> ivc(iv.begin(), iv.end());

    int num = 0;
    std::vector<unsigned char> out(text.length() + 128);
    AES_cfb128_encrypt(reinterpret_cast<const unsigned char *>(text.data()), out.data(), text.length(), &EncryptKey, ivc.data(), &num, AES_ENCRYPT);

    std::string res((const char*)out.data(),text.length());
    return res;
}
