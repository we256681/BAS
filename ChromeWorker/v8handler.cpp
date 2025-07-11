#include "v8handler.h"
#include "log.h"
#include <thread>
#include "split.h"

V8Handler::V8Handler()
{
    Changed = false;
    ChangedInspect = false;
    ChangedHighlight = false;
    ChangedLocalStorage = false;
    ChangedFrameFind = false;
    RecaptchaV3 = false;
    NewLocalStorage.clear();
    ChangedMultiSelectPositions = false;
    ChangedMultiSelectReport = false;
    ChangedFrameStructure = false;
}

bool V8Handler::GetResultReady()
{
    return Changed;
}

std::pair<std::string,bool> V8Handler::GetResult()
{
    std::pair<std::string,bool> r;
    r.first = Result;
    r.second = Changed;
    //Changed = false;
    return r;
}

std::pair<std::vector<LocalStorageDataItem>,bool> V8Handler::GetLocalStorage()
{
    std::pair<std::vector<LocalStorageDataItem>,bool> r;
    r.first = NewLocalStorage;
    r.second = ChangedLocalStorage;
    ChangedLocalStorage = false;
    NewLocalStorage.clear();
    return r;
}

std::pair<V8Handler::FrameStructure,bool> V8Handler::GetFrameStructure()
{
    std::pair<FrameStructure,bool> r;
    r.first = _FrameStructure;
    r.second = ChangedFrameStructure;
    ChangedFrameStructure = false;
    return r;
}

bool V8Handler::IsChangedFrameStructure()
{
    return ChangedFrameStructure;
}

std::pair<std::string,bool> V8Handler::GetMultiSelectPositions()
{
    std::pair<std::string,bool> r;
    r.first = MultiSelectPositions;
    r.second = ChangedMultiSelectPositions;
    ChangedMultiSelectPositions = false;
    MultiSelectPositions.clear();
    return r;
}

std::pair<std::string,bool> V8Handler::GetMultiSelectReport()
{
    std::pair<std::string,bool> r;
    r.first = MultiSelectReport;
    r.second = ChangedMultiSelectReport;
    ChangedMultiSelectReport = false;
    MultiSelectReport.clear();
    return r;
}

void V8Handler::SetResultProcessed()
{
    Changed = false;
}

bool V8Handler::GetInspectResultReady()
{
    return ChangedInspect;
}

bool V8Handler::IsChangedMultiSelectPositions()
{
    return ChangedMultiSelectPositions;
}

bool V8Handler::IsChangedMultiSelectReport()
{
    return ChangedMultiSelectReport;
}

std::pair<InspectResult,bool> V8Handler::GetInspectResult()
{
    std::pair<InspectResult,bool> r;
    r.first = _InspectResult;
    r.second = ChangedInspect;
    ChangedInspect = false;
    return r;
}

void V8Handler::ResetInspectResult()
{
    ChangedInspect = true;
}

bool V8Handler::GetHighlightResultReady()
{
    return ChangedHighlight;
}

std::pair<V8Handler::RecaptchaV3Request,bool> V8Handler::GetRecaptchaV3Request()
{
    std::pair<V8Handler::RecaptchaV3Request,bool> r;
    r.first = _RecaptchaV3Request;
    r.second = RecaptchaV3;
    _RecaptchaV3Request.ActionName.clear();
    _RecaptchaV3Request.InspectId.clear();
    _RecaptchaV3Request.SiteKey.clear();
    _RecaptchaV3Request.Url.clear();
    RecaptchaV3 = false;
    return r;
}

bool V8Handler::GetRecaptchaV3RequestReady()
{
    return RecaptchaV3;
}


std::pair<HighlightResult,bool> V8Handler::GetHighlightResult()
{
    std::pair<HighlightResult,bool> r;
    r.first = _HighlightResult;
    r.second = ChangedHighlight;
    _HighlightResult.highlights.clear();
    ChangedHighlight = false;
    return r;
}

bool V8Handler::GetFrameFindResultReady()
{
    return ChangedFrameFind;
}


std::pair<InspectResult,bool> V8Handler::GetFrameFindResult()
{
    std::pair<InspectResult,bool> r;
    r.first = _FrameFindResult;
    r.second = ChangedFrameFind;
    /*_FrameFindResult.css.clear();
    _FrameFindResult.match.clear();
    _FrameFindResult.label.clear();*/
    ChangedFrameFind = false;
    return r;
}

void V8Handler::ResetFrameFindResult()
{
    ChangedFrameFind = true;
}


void V8Handler::ClearHighlightLast()
{
    HighlightLast.clear();
}


bool V8Handler::Execute(const CefString& name, CefRefPtr<CefListValue> arguments)
{
    if(name == std::string("browser_automation_studio_result"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            Result = arguments->GetString(0);
            Changed = true;
        }else if (arguments->GetSize() == 1 && (arguments->GetType(0) == VTYPE_NULL))
        {
            Result = "";
            Changed = true;
        } if (arguments->GetSize() == 1 && (arguments->GetType(0) == VTYPE_INT))
        {
            Result = std::to_string(arguments->GetInt(0));
            Changed = true;
        }
    }else if(name == std::string("browser_automation_studio_frame_find_result"))
    {
        _FrameFindResult.x = arguments->GetInt(0);
        _FrameFindResult.y = arguments->GetInt(1);

        _FrameFindResult.FrameData.frame_name = arguments->GetString(2);
        _FrameFindResult.FrameData.frame_url = arguments->GetString(3);
        _FrameFindResult.FrameData.frame_tag_html = arguments->GetString(4);
        _FrameFindResult.FrameData.frame_index = arguments->GetInt(5);

        _FrameFindResult.FrameData.x_with_padding = arguments->GetInt(6);
        _FrameFindResult.FrameData.y_with_padding = arguments->GetInt(7);

        _FrameFindResult.FrameData.is_frame = true;

        _FrameFindResult.active = arguments->GetBool(8);

        ChangedFrameFind = true;
    }else if(name == std::string("browser_automation_studio_inspect_result"))
    {
        _InspectResult.x = arguments->GetInt(0);
        _InspectResult.y = arguments->GetInt(1);
        _InspectResult.width = arguments->GetInt(2);
        _InspectResult.height = arguments->GetInt(3);

        _InspectResult.label = arguments->GetString(4);
        _InspectResult.css = arguments->GetString(5);
        _InspectResult.css2 = arguments->GetString(6);
        _InspectResult.css3 = arguments->GetString(7);
        _InspectResult.match = arguments->GetString(8);
        _InspectResult.xpath = arguments->GetString(9);
        _InspectResult.mousex = arguments->GetInt(10);
        _InspectResult.mousey = arguments->GetInt(11);
        _InspectResult.active = arguments->GetBool(12);

        _InspectResult.FrameData.is_frame = arguments->GetBool(13);
        _InspectResult.FrameData.frame_name = arguments->GetString(14);
        _InspectResult.FrameData.frame_url = arguments->GetString(15);
        _InspectResult.FrameData.frame_tag_html = arguments->GetString(16);
        _InspectResult.FrameData.frame_index = arguments->GetInt(17);

        _InspectResult.FrameData.x_with_padding = arguments->GetInt(18);
        _InspectResult.FrameData.y_with_padding = arguments->GetInt(19);

        _InspectResult.position = arguments->GetInt(20);

        ChangedInspect = true;
    }else if(name == std::string("BrowserAutomationStudio_RecaptchaV3"))
    {
        if(arguments->GetSize() == 4 && (arguments->GetType(0) == VTYPE_STRING) && (arguments->GetType(1) == VTYPE_STRING) && (arguments->GetType(2) == VTYPE_STRING) && (arguments->GetType(3) == VTYPE_STRING))
        {
            RecaptchaV3 = true;

            _RecaptchaV3Request.ActionName = arguments->GetString(0);
            _RecaptchaV3Request.Url = arguments->GetString(1);
            _RecaptchaV3Request.SiteKey = arguments->GetString(2);
            _RecaptchaV3Request.InspectId = arguments->GetString(3);
        }

    }
    else if(name == std::string("browser_automation_studio_highlight_result"))
    {
        bool DoUpdate = false;
        std::string HighlightNow = arguments->GetString(0);

        {

            if(HighlightLast != HighlightNow)
            {
                HighlightLast = HighlightNow;
                DoUpdate = true;
            }
        }

        if(DoUpdate)
        {
            _HighlightResult.highlights.clear();

            std::vector<std::string> s = split(HighlightNow,';');

            HighlightResult::rect r;

            for(int i = 0;i<s.size();i++)
            {
                switch(i%6)
                {
                    case 0: r.x = std::stoi(s[i]);r.y = 0;r.height = -1;r.width = -1; break;
                    case 1: r.y = std::stoi(s[i]); break;
                    case 2: r.width = std::stoi(s[i]); break;
                    case 3: r.height = std::stoi(s[i]);  break;
                    case 4: r.index = std::stoi(s[i]);  break;
                    case 5: r.is_alternative = s[i] == std::string("true"); _HighlightResult.highlights.push_back(r);  break;
                }
            }

            ChangedHighlight = true;
        }
    }else if(name == std::string("browser_automation_studio_frame_structure_query_result"))
    {
        if (arguments->GetSize() == 2 && arguments->GetType(0) == VTYPE_STRING && arguments->GetType(1) == VTYPE_STRING)
        {
            _FrameStructure.Result = std::stoll(arguments->GetString(1).ToString());
            _FrameStructure.Id = arguments->GetString(0).ToString();
            ChangedFrameStructure = true;
        }

    }else if(name == std::string("browser_automation_studio_multiselect_result"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            MultiSelectPositions = arguments->GetString(0);
            ChangedMultiSelectPositions = true;
        }

    }else if(name == std::string("browser_automation_studio_multiselect_report"))
    {
        if (arguments->GetSize() == 1 && arguments->GetType(0) == VTYPE_STRING)
        {
            MultiSelectReport = arguments->GetString(0);
            ChangedMultiSelectReport = true;
        }

    }

    return true;
}

