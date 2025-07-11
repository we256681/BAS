#include "newtabschemehandlerfactory.h"
#include "include/wrapper/cef_stream_resource_handler.h"

CefRefPtr<CefResourceHandler> NewTabSchemeHandlerFactory::Create(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& scheme_name, CefRefPtr<CefRequest> request)
{
    const std::string& html_content = "<html><body></body></html>";

    CefRefPtr<CefStreamReader> stream =
        CefStreamReader::CreateForData(
            static_cast<void*>(const_cast<char*>(html_content.c_str())),
            html_content.size());

    return new CefStreamResourceHandler("text/html", stream);
}
