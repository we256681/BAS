#include "rawcppwebsocketclientfactory.h"
#include "rawcppwebsocketclient.h"


void RawCppWebSocketClientFactory::SetLogPath(const std::wstring& LogPath)
{
    this->LogPath = LogPath;
}

IWebSocketClient* RawCppWebSocketClientFactory::Create()
{
    RawCppWebSocketClient* Result = new RawCppWebSocketClient();
    Result->SetLogPath(LogPath);
    return Result;
}

