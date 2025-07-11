#ifndef DONOTHINGCALLBACK_H
#define DONOTHINGCALLBACK_H

#include "include/cef_client.h"


class DoNothingCallback: public CefCompletionCallback
{
public:
    virtual void OnComplete() OVERRIDE;
private:
    IMPLEMENT_REFCOUNTING(DoNothingCallback);
};

#endif // DONOTHINGCALLBACK_H
