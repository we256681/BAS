#include "rawcpphttpclientfactory.h"
#include "rawcpphttpclient.h"

ISimpleHttpClient* RawCppHttpClientFactory::Create()
{
    return new RawCppHttpClient();
}
