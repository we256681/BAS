#ifndef ISIMPLEHTTPCLIENTFACTORY_H
#define ISIMPLEHTTPCLIENTFACTORY_H

#include "ISimpleHttpClient.h"

class ISimpleHttpClientFactory
{
public:
    virtual ISimpleHttpClient* Create() = 0;
};


#endif // ISIMPLEHTTPCLIENTFACTORY_H
