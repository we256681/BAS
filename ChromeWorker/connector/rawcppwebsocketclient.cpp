#include "rawcppwebsocketclient.h"
#include "ixwebsocket/IXNetSystem.h"
#include <fstream>
#include "converter.h"


using namespace std::placeholders;
using namespace std::chrono;


RawCppWebSocketClient::~RawCppWebSocketClient()
{
    Disconnect();
}

void RawCppWebSocketClient::SetLogPath(const std::wstring& LogPath)
{
    this->LogPath = LogPath;
}

long long RawCppWebSocketClient::Now()
{
    return duration_cast< milliseconds >( system_clock::now().time_since_epoch() ).count();
}

void RawCppWebSocketClient::Connect(const std::string& Url)
{
    if(WebSocket)
        return;

    WebSocket = std::make_shared<ix::WebSocket>();

    WebSocket->setUrl(Url);

    WebSocket->disablePerMessageDeflate();

    WebSocket->setOnMessageCallback(std::bind(&RawCppWebSocketClient::OnMessageFunction,this,_1));

    ConnectionStart = Now();

    WebSocket->start();
}

void RawCppWebSocketClient::Timer()
{
    std::vector<MessageClass> MessagesCopy;
    {
        std::lock_guard<std::mutex> lock(DataMutex);
        MessagesCopy = Messages;
        Messages.clear();
    }

    for(MessageClass& _Message:MessagesCopy)
    {
        if(_Message.Type == ReceivedMessage)
        {
            if(!LogPath.empty())
            {
                std::wofstream outfile(LogPath, std::ios::binary | std::ios::app);
                if(outfile.is_open())
                {
                    outfile << L" --- " << s2ws(_Message.Data) << std::endl;;
                }
                outfile.flush();
                outfile.close();
            }

            for(auto f:OnMessage)
                f(_Message.Data);
        }else if(_Message.Type == Connection)
        {
            for(auto f:OnConnected)
                f(_Message.IsSuccess);
        }else if(_Message.Type == Disconnected)
        {
            for(auto f:OnDisconnected)
                f();
        }
    }

    if(ConnectionStart > 0 && Now() - ConnectionStart > 10000)
    {
        ConnectionStart = 0;
        for(auto f:OnConnected)
            f(false);
    }

}

void RawCppWebSocketClient::OnMessageFunction(const ix::WebSocketMessagePtr& Message)
{
    std::lock_guard<std::mutex> lock(DataMutex);

    if(Message->type == ix::WebSocketMessageType::Message)
    {
        MessageClass _Message;
        _Message.IsSuccess = true;
        _Message.Type = ReceivedMessage;
        _Message.Data = Message->str;

        Messages.push_back(_Message);
    }
    if(Message->type == ix::WebSocketMessageType::Open)
    {
        MessageClass _Message;
        _Message.IsSuccess = true;
        _Message.Type = Connection;

        Messages.push_back(_Message);

        ConnectionStart = 0;
    }
    if(Message->type == ix::WebSocketMessageType::Close)
    {
        MessageClass _Message;
        _Message.IsSuccess = true;
        _Message.Type = Disconnected;

        ConnectionStart = 0;

        Messages.push_back(_Message);
    }
}

void RawCppWebSocketClient::Disconnect()
{
    ConnectionStart = 0;

    if(!WebSocket)
        return;

    WebSocket->stop();
    WebSocket.reset();
}

void RawCppWebSocketClient::Send(const std::string& Data)
{

    if(!LogPath.empty())
    {
        std::wofstream outfile(LogPath, std::ios::binary | std::ios::app);
        if(outfile.is_open())
        {
            outfile << L" +++ " << s2ws(Data) << std::endl;;
        }
        outfile.flush();
        outfile.close();
    }

    if(!WebSocket)
        return;

    WebSocket->send(Data);
}

void RawCppWebSocketClient::GlobalActivate()
{
    ix::initNetSystem();
}

void RawCppWebSocketClient::GlobalDeactivate()
{
    ix::uninitNetSystem();
}
