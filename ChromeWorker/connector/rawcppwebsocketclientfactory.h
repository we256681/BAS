#ifndef RAWCPPWEBSOCKETCLIENTFACTORY_H
#define RAWCPPWEBSOCKETCLIENTFACTORY_H


#include "IWebSocketClientFactory.h"

class RawCppWebSocketClientFactory: public IWebSocketClientFactory
{
    std::wstring LogPath;
    public:
    virtual void SetLogPath(const std::wstring& LogPath);
    virtual IWebSocketClient* Create();
};

#endif // RAWCPPWEBSOCKETCLIENTFACTORY_H
