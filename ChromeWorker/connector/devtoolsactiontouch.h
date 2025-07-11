#ifndef DEVTOOLSACTIONTOUCH_H
#define DEVTOOLSACTIONTOUCH_H

#include "DevToolsMultiAction.h"

class DevToolsActionTouch : public DevToolsMultiAction
{
    bool IsSendingTouchEvent = false;
    bool IsMouseMoveSearchingFrame = false;
    bool IsWaitingForFindFrame = false;

    std::string FrameFinderId;
    std::string FrameFinderType;
    int X = 0;
    int Y = 0;
    bool IsTimerEventSubscribed = false;
    bool IsInIsolatedFrame = false;

    bool NeedToSetSessionId = false;
    std::string SessionIdToSave;


    void FindFrame();
    public:
    virtual void Run();
    virtual void OnActionFinished();
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
    virtual void OnWebSocketEvent(const std::string& Method, const std::string& Message);

};

#endif // DEVTOOLSACTIONTOUCH_H
