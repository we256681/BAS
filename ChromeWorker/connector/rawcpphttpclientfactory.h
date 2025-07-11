#ifndef RAWCPPHTTPCLIENTFACTORY_H
#define RAWCPPHTTPCLIENTFACTORY_H

#include "ISimpleHttpClientFactory.h"

class RawCppHttpClientFactory: public ISimpleHttpClientFactory
{
    public:
    virtual ISimpleHttpClient* Create();
};

#endif // RAWCPPHTTPCLIENTFACTORY_H
