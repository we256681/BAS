#include "md5.h"
#include <vector>
#include <openssl/md5.h>

std::string md5(const std::string& data)
{
    std::vector<unsigned char> res;
    res.resize(16);

    MD5((const unsigned char*)data.data(), data.size(), res.data());
    std::string s((const char *)res.data(),16);
    return s;
}
