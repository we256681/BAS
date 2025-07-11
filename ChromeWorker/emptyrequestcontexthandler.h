#ifndef EMPTYREQUESTCONTEXTHANDLER_H
#define EMPTYREQUESTCONTEXTHANDLER_H

#include "include/cef_app.h"
#include "include/cef_request_context_handler.h"

class EmptyRequestContextHandler: public CefRequestContextHandler
{
    private:
        IMPLEMENT_REFCOUNTING(EmptyRequestContextHandler);
};

#endif // EMPTYREQUESTCONTEXTHANDLER_H
