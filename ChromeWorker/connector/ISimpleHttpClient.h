#ifndef ISIMPLEHTTPCLIENT_H
#define ISIMPLEHTTPCLIENT_H

#include <vector>
#include <functional>
#include <string>

class ISimpleHttpClient
{
    public:

    virtual ~ISimpleHttpClient(){};

    virtual void Get(const std::string& Url) = 0;
    virtual void Stop() = 0;

    virtual void Timer() = 0;

    virtual void GlobalActivate() = 0;
    virtual void GlobalDeactivate() = 0;


    //is success, status, content
    std::vector<std::function<void(bool,int,std::string&)> > OnResult;

    bool DeleteLater = false;

};

#endif // ISIMPLEHTTPCLIENT_H
