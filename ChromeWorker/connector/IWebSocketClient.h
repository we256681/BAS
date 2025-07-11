#ifndef IWEBSOCKETCLIENT_H
#define IWEBSOCKETCLIENT_H

#include <vector>
#include <functional>
#include <string>

class IWebSocketClient
{
    public:

    virtual ~IWebSocketClient(){};

    virtual void Connect(const std::string& Url) = 0;
    virtual void Disconnect() = 0;
    virtual void Send(const std::string& Data) = 0;

    virtual void Timer() = 0;

    virtual void GlobalActivate() = 0;
    virtual void GlobalDeactivate() = 0;


    std::vector<std::function<void(std::string&)> > OnMessage;
    std::vector<std::function<void()> > OnDisconnected;
    std::vector<std::function<void(bool)> > OnConnected;

    bool DeleteLater = false;

};

#endif // IWEBSOCKETCLIENT_H
