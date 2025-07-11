#ifndef RAWCPPHTTPCLIENT_H
#define RAWCPPHTTPCLIENT_H

#include "ISimpleHttpClient.h"
#include "ixwebsocket/IXHttpClient.h"

class RawCppHttpClient : public ISimpleHttpClient
{
    std::shared_ptr<ix::HttpClient> HttpClient = 0;

    enum MessageType
    {
        Result
    };

    struct MessageClass
    {
        MessageType Type;
        std::string Data;
        bool IsSuccess;
        int Status;
    };

    std::mutex DataMutex;
    std::vector<MessageClass> Messages;
public:
    ~RawCppHttpClient();

    virtual void Get(const std::string& Url);
    virtual void Stop();

    virtual void Timer();

    virtual void GlobalActivate();
    virtual void GlobalDeactivate();

private:

    void OnMessageFunction(const ix::HttpResponsePtr& Message);


};

#endif // RAWCPPHTTPCLIENT_H
