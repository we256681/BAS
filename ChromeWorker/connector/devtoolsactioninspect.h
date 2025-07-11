#ifndef DEVTOOLSACTIONINSPECT_H
#define DEVTOOLSACTIONINSPECT_H

#include "idevtoolsaction.h"
class DevToolsActionInspect :	public IDevToolsAction
{
    //Inputs
    int X;
    int Y;
    int Position;

    //Result
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    std::string label;
    std::string css;
    std::string css2;
    std::string css3;
    std::string xpath;
    std::string match;
    int position = 0;
    int mousex = -1;
    int mousey = -1;
    bool active = false;

    bool is_frame;
    int x_with_padding;
    int y_with_padding;

    std::string LastMessage;
    std::string CurrentLoaderId;
    std::vector<std::pair<std::string, std::string> > CurrentPrefix;
    std::string CurrentContextId;
    std::string CurrentFrameSessionId = "CurrentTab";
    bool ScrollDataWasObtained = false;
    bool IsDoingScrollRequest = false;
    int PositionX = 0;
    int PositionY = 0;
    enum {
        Initial,
        InspectPosition,
        FrameSearchEvaluate,
        FrameSearchGetNodeId,
        FrameSearchReleaseObject,
        FrameSearchGetPosition,
        JavascriptExecution
    }RequestType = Initial;
    void Next();
    std::string Javascript(const std::string& Script);
    std::string PrepareResult();
public:
    virtual void Run();
    virtual void OnWebSocketMessage(const std::string& Message, const std::string& Error);
};

#endif // DEVTOOLSACTIONINSPECT_H
