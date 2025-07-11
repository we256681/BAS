#include "multithreading.h"

multithreading mthreading;

std::string GetCurrentThreadString()
{
    if(CefCurrentlyOn(TID_UI))
        return "TID_UI";

    if(CefCurrentlyOn(TID_FILE_BACKGROUND))
        return "TID_FILE_BACKGROUND";

    if(CefCurrentlyOn(TID_FILE_USER_VISIBLE))
        return "TID_FILE_USER_VISIBLE";

    if(CefCurrentlyOn(TID_FILE_USER_BLOCKING))
        return "TID_FILE_USER_BLOCKING";

    if(CefCurrentlyOn(TID_PROCESS_LAUNCHER))
        return "TID_PROCESS_LAUNCHER";

    if(CefCurrentlyOn(TID_IO))
        return "TID_IO";

    if(CefCurrentlyOn(TID_RENDERER))
        return "TID_RENDERER";

    return "TID_UNKNOWN";
}
