#ifndef DEVTOOLSACTIONNAVIGATEBACK_H
#define DEVTOOLSACTIONNAVIGATEBACK_H

#include "idevtoolsaction.h"

class DevToolsActionNavigateBack : public IDevToolsAction
{
    bool IsGettingHistoryEntries = false;
    bool IsGettingFrameTree = false;
    bool IsInstant = false;
    int EntryId;
    std::string FrameId;
public:
    virtual void Run();
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
    virtual void OnWebSocketEvent(const std::string& Method, const std::string& Message);
};

#endif // DEVTOOLSACTIONNAVIGATEBACK_H