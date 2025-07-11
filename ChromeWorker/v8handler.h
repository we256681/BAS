#ifndef V8HANDLER_H
#define V8HANDLER_H

#include "include/cef_app.h"
#include <functional>
#include <mutex>
#include "inspectresult.h"
#include "highlightresult.h"
#include "localstoragedata.h"
#include "multithreading.h"

class V8Handler : public CefBaseRefCounted
{
    public:
        struct RecaptchaV3Request
        {
            std::string ActionName;
            std::string Url;
            std::string SiteKey;
            std::string InspectId;
        };
        struct FrameStructure
        {
            int64 Result;
            std::string Id;
        };
    private:

    RecaptchaV3Request _RecaptchaV3Request;
    std::string HighlightLast;
    std::string Result;
    std::string MultiSelectPositions;
    std::string MultiSelectReport;
    std::vector<LocalStorageDataItem> NewLocalStorage;
    InspectResult _InspectResult;
    HighlightResult _HighlightResult;
    InspectResult _FrameFindResult;
    bool Changed;
    bool ChangedInspect;
    bool ChangedHighlight;
    bool ChangedLocalStorage;
    bool ChangedFrameFind;
    bool RecaptchaV3;
    bool ChangedMultiSelectPositions;
    bool ChangedMultiSelectReport;

    FrameStructure _FrameStructure;
    bool ChangedFrameStructure;
public:
    V8Handler();

    bool Execute(const CefString& name, CefRefPtr<CefListValue> arguments);

    bool GetResultReady();
    std::pair<std::string,bool> GetResult();

    std::pair<std::vector<LocalStorageDataItem>,bool> GetLocalStorage();

    std::pair<FrameStructure,bool> GetFrameStructure();
    bool IsChangedFrameStructure();

    void SetResultProcessed();

    std::pair<RecaptchaV3Request,bool> GetRecaptchaV3Request();
    bool GetRecaptchaV3RequestReady();

    std::pair<InspectResult,bool> GetInspectResult();
    void ResetInspectResult();
    bool GetInspectResultReady();

    std::pair<HighlightResult,bool> GetHighlightResult();
    bool GetHighlightResultReady();
    void ClearHighlightLast();

    std::pair<InspectResult,bool> GetFrameFindResult();
    void ResetFrameFindResult();
    bool GetFrameFindResultReady();

    bool IsChangedMultiSelectPositions();
    std::pair<std::string,bool> GetMultiSelectPositions();

    bool IsChangedMultiSelectReport();
    std::pair<std::string,bool> GetMultiSelectReport();



private:
    IMPLEMENT_REFCOUNTING(V8Handler);
};

#endif // V8HANDLER_H

