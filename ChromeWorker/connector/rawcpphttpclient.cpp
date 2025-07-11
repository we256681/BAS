#include "rawcpphttpclient.h"
#include "ixwebsocket/IXNetSystem.h"

using namespace std::placeholders;


RawCppHttpClient::~RawCppHttpClient()
{
    Stop();
}


void RawCppHttpClient::Get(const std::string& Url)
{
    if(HttpClient)
        return;

    HttpClient = std::make_shared<ix::HttpClient>(true);

    ix::HttpRequestArgsPtr Args = HttpClient->createRequest(Url, ix::HttpClient::kGet);

    Args->connectTimeout = 10;
    Args->transferTimeout = 60;
    Args->compress = false;

    HttpClient->performRequest(Args, std::bind(&RawCppHttpClient::OnMessageFunction,this,_1));

}

void RawCppHttpClient::OnMessageFunction(const ix::HttpResponsePtr& Message)
{
    std::lock_guard<std::mutex> lock(DataMutex);

    MessageClass _Message;
    _Message.Type = Result;
    _Message.Data = Message->body;
    _Message.IsSuccess = ix::HttpErrorCode::Ok == Message->errorCode;
    _Message.Status = Message->statusCode;


    Messages.push_back(_Message);
}


void RawCppHttpClient::Stop()
{
    if(!HttpClient)
        return;

    HttpClient.reset();
}

void RawCppHttpClient::Timer()
{
    std::vector<MessageClass> MessagesCopy;
    {
        std::lock_guard<std::mutex> lock(DataMutex);
        MessagesCopy = Messages;
        Messages.clear();
    }


    for(MessageClass& _Message:MessagesCopy)
    {
        if(_Message.Type == Result)
        {
            for(auto f:OnResult)
            {
                f(_Message.IsSuccess,_Message.Status,_Message.Data);
            }
        }
    }
}

void RawCppHttpClient::GlobalActivate()
{
    ix::initNetSystem();
}

void RawCppHttpClient::GlobalDeactivate()
{
    ix::uninitNetSystem();
}
