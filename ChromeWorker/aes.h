#ifndef AES_H
#define AES_H

#include <string>

std::string encrypt_aes_cfb_128(const std::string& text, const std::string& key, const std::string& iv);


#endif // AES_H
