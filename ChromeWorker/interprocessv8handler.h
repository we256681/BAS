#ifndef INTERPROCESSV8HANDLER_H
#define INTERPROCESSV8HANDLER_H

#include "include/cef_app.h"


class InterprocessV8Handler : public CefV8Handler
{
    std::string prefix;

public:
    InterprocessV8Handler(const std::string& prefix);

    virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) OVERRIDE;
private:
    IMPLEMENT_REFCOUNTING(InterprocessV8Handler);
};

#endif // INTERPROCESSV8HANDLER_H
