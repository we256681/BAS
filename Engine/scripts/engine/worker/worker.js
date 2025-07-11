VAR_WAS_ERROR = false;
VAR_LAST_ERROR = "";
VAR_ERROR_ID = 0;
_BAS_SOLVER_PROPERTIES = {}
GLOBAL = this
_BROWSER_TYPE = 1;
_IS_MOBILE = false;

//Manual browser control
function _manual_browser_control(text, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.StartManualBrowserControl(_ARG[0],_get_function_body(_ARG[1]));
    })
}

function _create_browser_if_needed(callback, arg)
{

    if(typeof(NetworkAccessManager)=='undefined' || NetworkAccessManager == null)
    {
        browser(callback)
    }else
    {
        if(Browser.NeedToCreateVirtualProcessCommunicator())
        {
            Browser.CreateNewVirtualBrowser(true, "ScriptWorker.AttachNetworkAccessManager();" +_get_function_body(callback))
        }else
            callback()
    }
}

/* Hard reset */
function new_browser(callback)
{
    Browser.ResetSettings();

    if(!Browser.IsBASBrowserVirtual())
    {
        //Reset only virtual browser
        Browser.CreateNewVirtualBrowser(true, "ScriptWorker.AttachNetworkAccessManager();" +_get_function_body(callback))

    }else
    {
        //Reset real browser
        Browser.CreateNewBrowser(true, "ScriptWorker.AttachNetworkAccessManager();_reset_proxy();" + _get_function_body(callback))
    }
}

/* Create browser caus used action which needs browser */
function browser(callback)
{
    Browser.CreateNewBrowser(false, "_mbr();ScriptWorker.AttachNetworkAccessManager();_init_browser(function(){_mar();" + _get_function_body(callback) + "})")
}

/* Init browser after created */
function _init_browser(callback)
{
    _DEFAULT_MOVE_PARAMS = {}

    if(Browser.IsBASBrowser())
        Browser.ResetProxy();

    _settings(_PROXY, callback)


}

/* Check if bas browser is set through settings and it actually used */
function _is_bas_browser_real()
{
    return Browser.IsBASBrowser() && Browser.IsBASBrowserVirtual();
}

/* Check if in record mode */
function _is_record()
{
    return ScriptWorker.GetIsRecord();
}

function _get_browser_process_id()
{
    return Browser.GetBrowserProcessId();
}

function _get_browser_unique_id()
{
    return Browser.GetBrowserUniqueId();
}

/* Manual control */
function open_browser(callback)
{
    Browser.SetManualBrowserControl();

    if(!Browser.IsBASBrowserVirtual())
    {
        Browser.CreateNewVirtualBrowser(false, "ScriptWorker.AttachNetworkAccessManager();" +_get_function_body(callback))
    }else
    {
        Browser.CreateNewBrowser(false, "_mbr();ScriptWorker.AttachNetworkAccessManager();_init_browser(function(){_mar();" + _get_function_body(callback) + "})")
    }
}

function close_browser()
{
    if(!Browser.IsBASBrowserVirtual())
    {
        Browser.CloseVirtualBrowser();
        ScriptWorker.AttachNetworkAccessManager();
    }else
    {
        if(!ScriptWorker.GetIsRecord())
        {
            Browser.CloseBrowser();
            ScriptWorker.AttachNetworkAccessManager();
        }
    }
}

function _ensure_browser_created()
{
    if(typeof(NetworkAccessManager)=='undefined' || NetworkAccessManager == null)
        die("NetworkAccessManager is not accessible, try to create new browser with open browser command")
}

function _simulate_crush(callback)
{
    _ensure_browser_created();
    Browser.SimulateCrush(_get_function_body(callback));
}

function _settings(json, callback)
{
    Browser.SendWorkerSettings(JSON.stringify(json), "if(_result() == 2)NetworkAccessManager=null;if(_result() == 1)ScriptWorker.AttachNetworkAccessManager();" + _get_function_body(callback))
}

function _require_extensions(extensions, callback)
{
    ScriptWorker.RequireExtensions(extensions, "(function(){var Data = JSON.parse(_result());if(!Data.success){fail(Data.data);}else{_set_result(Data.data)}})();" + _get_function_body(callback))
}

function _click_extension_button(id, callback)
{
    Browser.ClickExtensionButton(id, _get_function_body(callback))
}



function _mbr()
{
    Browser.MarkBeforeReset();
}

function _mar()
{
    Browser.MarkAfterReset();
}

function _browser_mode(mode, callback)
{
    _ARG = arguments

    _create_browser_if_needed(function(){

        var current_settings = {}

        if(_ARG[0] == "mobile")
        {
            _IS_MOBILE = true;
            current_settings = {
                "Fingerprints.Setting.forceAndroidOverlayScrollbar":"Enable",
                "Fingerprints.Setting.passwordEchoEnabled":"Enable",
                "Fingerprints.Setting.textAreasAreResizable":"Disable",
                "Fingerprints.Setting.useSolidColorScrollbars":"Enable",
                "Fingerprints.Setting.availablePointerTypes":"2",
                "Fingerprints.Setting.availableHoverTypes":"1",
                "Fingerprints.Setting.primaryPointerType":"2",
                "Fingerprints.Setting.primaryHoverType":"1",
                "Fingerprints.BrowserMode":"Mobile",
                "Fingerprints.Feature.TouchEventFeatureDetection":"Enable"

            }
        }

        if(_ARG[0] == "desktop")
        {
            _IS_MOBILE = false;
            current_settings = {
                "Fingerprints.Setting.forceAndroidOverlayScrollbar":"Disable",
                "Fingerprints.Setting.passwordEchoEnabled":"Disable",
                "Fingerprints.Setting.textAreasAreResizable":"Enable",
                "Fingerprints.Setting.useSolidColorScrollbars":"Disable",
                "Fingerprints.Setting.availablePointerTypes":"4",
                "Fingerprints.Setting.availableHoverTypes":"2",
                "Fingerprints.Setting.primaryPointerType":"4",
                "Fingerprints.Setting.primaryHoverType":"2",
                "Fingerprints.BrowserMode":"Desktop",
                "Fingerprints.Feature.TouchEventFeatureDetection":"Disable"
            }
        }

        _settings(current_settings, function(){
             Browser.SetMode(_ARG[0],_get_function_body(_ARG[1]))
        })
    })

}

function _get_profile()
{
    return Browser.GetProfilePath()
}

function _get_all_browser_version_info(format)
{
    return JSON.parse(Browser.GetAllBrowserVersionInfo(format))
}

function _get_current_browser_version_info(format)
{
    return JSON.parse(Browser.GetCurrentBrowserVersionInfo(format))
}

function _find_browser_version_id(version_string)
{
    var Result = Browser.FindBrowserVersionId(version_string)
    if(Result == -1)
    {
        fail("Failed to find browser version")
    }
    return Result
}

function mouse(x, y, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.MouseClick(_ARG[0],_ARG[1],_get_function_body(_ARG[2]))
    })
}

function mouse_up(x, y, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.MouseClickUp(_ARG[0],_ARG[1],_get_function_body(_ARG[2]))
    })
}

function mouse_down(x, y, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.MouseClickDown(_ARG[0],_ARG[1],_get_function_body(_ARG[2]))
    })
}

function timezone(offset, callback)
{
    if(!_is_bas_browser_real())
    {
        _settings({
                      "multilogin.fingerprintGeneration.timeZone":JSON.stringify(native("timezones", "timezone_offset_to_name", (-offset).toString()))
                  }, callback)
        return
    }
    _settings({
                  "Timezone": (-parseInt(offset)).toString()
              }, callback)

}

function _timezone_name(name, callback)
{
    _settings({
                  "TimezoneName": name
              }, callback)

}


function geolocation(latitude,longitude,callback)
{
    _ARG = arguments

    _create_browser_if_needed(function(){

        var SettingsObject = {}
        if(_ARG[0] > 99999)
        {
            SettingsObject = {"Fingerprints.Geolocation": "Disable"};
        }else
        {
            SettingsObject = {"Fingerprints.Geolocation": "Enable", "Fingerprints.Geolocation.latitude": _ARG[0].toString(), "Fingerprints.Geolocation.longitude": _ARG[1].toString()};
        }

        _settings(SettingsObject,_ARG[2])
    })
}

function geolocation_object(object_string,callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        var SettingsObject = {}
        var properties = ["altitude", "altitudeAccuracy", "heading", "speed" ]
        for(var i = 0;i < properties.length;i++)
        {
            if(typeof(_ARG[0][ properties[i] ]) == "object" && _ARG[0][ properties[i] ] == null)
            {
                SettingsObject[ "Fingerprints.Geolocation." + properties[i] ] = "-100000.0";
            }else if(typeof(_ARG[0][ properties[i] ]) == "number")
            {
                SettingsObject[ "Fingerprints.Geolocation." + properties[i] ] = _ARG[0][ properties[i] ].toString();
            }
        }
        if(typeof(_ARG[0].accuracy) == "number")
        {
            SettingsObject["Fingerprints.Geolocation.accuracy"] = _ARG[0].accuracy.toString();
        }
        _settings(SettingsObject,_ARG[1])
    })
}


function popupclose(index, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.PopupClose(_ARG[0],_get_function_body(_ARG[1]))
    })
}

function popupselect(index, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.PopupSelect(_ARG[0],_get_function_body(_ARG[1]))
    })
}

function _popupinfo(callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.PopupInfo(_get_function_body(_ARG[0]))
    })
}

function _popupcreate(is_silent, url, callback)
{
    _ARG_POPUP = arguments
    _create_browser_if_needed(function(){
        _popupinfo(function(){
            _if(JSON.parse(_result())["urls"].length == 0, function(){
                load("data:text/html,", function(){})
            },function(){
                Browser.PopupCreate(_ARG_POPUP[0],_ARG_POPUP[1],_get_function_body(_ARG_POPUP[2]))
                delete _ARG_POPUP
            })


        })

    })
}





function render(x, y, width, height, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.Render(_ARG[0],_ARG[1],_ARG[2],_ARG[3],_get_function_body(_ARG[4]))
    })
}

function scroll(x, y, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.Scroll(_ARG[0],_ARG[1],_get_function_body(_ARG[2]))
    })
}

_DEFAULT_MOVE_PARAMS = {}

function _default_move_params(params)
{
    if(params.hasOwnProperty("speed"))
    {
        params.speed = parseFloat(params.speed)
    }
    if(params.hasOwnProperty("gravity"))
    {
        params.gravity = parseFloat(params.gravity)
    }
    if(params.hasOwnProperty("deviation"))
    {
        params.deviation = parseFloat(params.deviation)
    }

    _DEFAULT_MOVE_PARAMS = params
}

function _get_browser_screen_settings()
{
    _ARG = arguments

    _create_browser_if_needed(function(){
        Browser.GetBrowserScreenSettings(_get_function_body(_ARG[0]));
    })
}

function _type()
{
    _ARG3 = arguments
    _if_else(_IS_MOBILE,function(){
        var Result = []
        var CurrentString = _ARG3[0].toString()

        while(true)
        {
            var IndexNext = 999999;
            var LengthNext = -1;
            var ReplacementNext = "";

            var WasReplacement = false

            var AllReplacementStrings =
                    [
                        "<MOUSESCROLLUP>","<MOUSESCROLLDOWN>",
                        "<MOUSESCROLLUP><MOUSESCROLLUP>","<MOUSESCROLLDOWN><MOUSESCROLLDOWN>",
                        "<MOUSESCROLLUP><MOUSESCROLLUP><MOUSESCROLLUP>","<MOUSESCROLLDOWN><MOUSESCROLLDOWN><MOUSESCROLLDOWN>"
                    ]
            for(var i = 0;i<AllReplacementStrings.length;i++)
            {
                var IndexCandidate = CurrentString.indexOf(AllReplacementStrings[i])
                var LengthCandidate = AllReplacementStrings[i].length
                if(IndexCandidate <= IndexNext && IndexCandidate >= 0)
                {
                    IndexNext = IndexCandidate
                    LengthNext = LengthCandidate
                    ReplacementNext = AllReplacementStrings[i]
                    WasReplacement = true
                }
            }

            if(!WasReplacement)
            {
                break;
            }else
            {
                if(IndexNext > 0)
                {
                    Result.push({
                                    is_string: true,
                                    offset: 0,
                                    track_scroll: true,
                                    text: CurrentString.substring(0,IndexNext)
                                })
                }

                var AddItem = {
                    is_string: false,
                    offset: 0,
                    track_scroll: true,
                    text: ""
                }
                if(ReplacementNext == "<MOUSESCROLLUP>")
                {
                    AddItem.offset = -100;
                }else if(ReplacementNext == "<MOUSESCROLLDOWN>")
                {
                    AddItem.offset = 100;
                }else if(ReplacementNext == "<MOUSESCROLLUP><MOUSESCROLLUP>")
                {
                    if(rand(0,100) > 45)
                        AddItem.offset = -200;
                    else
                    {
                        Result.push({
                                        is_string: false,
                                        offset: -100,
                                        track_scroll: true,
                                        text: ""
                                    })
                        AddItem.offset = -100;
                    }
                }else if(ReplacementNext == "<MOUSESCROLLDOWN><MOUSESCROLLDOWN>")
                {
                    if(rand(0,100) > 45)
                        AddItem.offset = 200;
                    else
                    {
                        Result.push({
                                        is_string: false,
                                        offset: 100,
                                        track_scroll: true,
                                        text: ""
                                    })
                        AddItem.offset = 100;
                    }
                }else if(ReplacementNext == "<MOUSESCROLLUP><MOUSESCROLLUP><MOUSESCROLLUP>")
                {
                    var Random = rand(0,100)
                    if(Random > 66)
                    {
                        Result.push({
                                        is_string: false,
                                        offset: -100,
                                        track_scroll: true,
                                        text: ""
                                    })
                        AddItem.offset = -200;
                    }
                    else if(Random > 33)
                    {
                        Result.push({
                                        is_string: false,
                                        offset: -200,
                                        track_scroll: false,
                                        text: ""
                                    })
                        AddItem.offset = -100;
                    }else
                    {
                        AddItem.offset = -300;
                    }
                }else if(ReplacementNext == "<MOUSESCROLLDOWN><MOUSESCROLLDOWN><MOUSESCROLLDOWN>")
                {
                    var Random = rand(0,100)
                    if(Random > 66)
                    {
                        Result.push({
                                        is_string: false,
                                        offset: 100,
                                        track_scroll: true,
                                        text: ""
                                    })
                        AddItem.offset = 200;
                    }
                    else if(Random > 33)
                    {
                        Result.push({
                                        is_string: false,
                                        offset: 200,
                                        track_scroll: false,
                                        text: ""
                                    })
                        AddItem.offset = 100;
                    }else
                    {
                        AddItem.offset = 300;
                    }
                }
                Result.push(AddItem)
                CurrentString = CurrentString.substring(IndexNext + LengthNext)
            }
        }



        if(CurrentString.length > 0)
        {
            Result.push({
                            is_string: true,
                            offset: 0,
                            track_scroll: true,
                            text: CurrentString
                        })
        }



        _ARG3[0] = Result

        _do(function(){
            if(_ARG3[0].length == 0)
                _break();

            _if_else(_ARG3[0][0].is_string, function(){
                page().type(_ARG3[0][0].text,_ARG3[1], function(){})
            }, function(){
               _slide(_ARG3[0][0].offset,_ARG3[0][0].track_scroll, function(){})
            },function(){
                _ARG3[0].shift()
            })

        }, function(){
        })
    }, function(){
        page().type(_ARG3[0],_ARG3[1], function(){
        })
    }, _ARG3[2])
}

function _scroll_to(target, callback)
{
    _SELECTOR = target
    _call(_random_point, {}, callback)
}

function _slide()
{
    _ARG2 = arguments
    _get_browser_screen_settings(function(){
        var screen_settings = JSON.parse(_result())
        var X1 = 0
        if(rand(0,100) >= 96)
        {
            X1 = (rand(0,400) + rand(0,400) + 100) * 0.001
        }else
        {
            X1 = (rand(0,100) + rand(0,100) + 700) * 0.001
        }
        var X2 = X1 + rand(-30,80) * 0.001
        var IsSlideDown = _ARG2[0] > 0
        _ARG2[0] = Math.abs(_ARG2[0])
        if(_ARG2[0] <= 100)
        {
            X2 = X1 + rand(-15,40) * 0.001
        }else if(_ARG2[0] <= 200)
        {
            X2 = X1 + rand(-20,60) * 0.001
        }

        X2 = X2 * screen_settings["Width"]

        var Y1 = 0
        if(rand(0,100) >= 96)
        {
            Y1 = (rand(0,400) + rand(0,400) + 100) * 0.001
        }else
        {
            Y1 = (rand(0,100) + rand(0,100) + 700) * 0.001
        }

        var SpeedMultiplier = 1.0

        if(_ARG2[0] >= 400 && rand(0,100) > 5)
        {
            SpeedMultiplier = 1.0 + (_ARG2[0] - 250) / 250
            if(SpeedMultiplier > 3.0)
                SpeedMultiplier = 3.0
            _ARG2[0] = 300
        }

        var Y2 = (Y1) * screen_settings["Height"] - _ARG2[0] * (1 + rand(-30,30)*0.01)

        X1 = (X1) * screen_settings["Width"];
        Y1 = (Y1) * screen_settings["Height"];

        if(X1 < screen_settings["Width"] * 0.05)
            X1 = screen_settings["Width"] * 0.05

        if(X1 > screen_settings["Width"] * 0.95)
            X1 = screen_settings["Width"] * 0.95

        if(X2 < screen_settings["Width"] * 0.05)
            X2 = screen_settings["Width"] * 0.05

        if(X2 > screen_settings["Width"] * 0.95)
            X2 = screen_settings["Width"] * 0.95


        if(Y1 < screen_settings["Height"] * 0.05)
            Y1 = screen_settings["Height"] * 0.05

        if(Y1 > screen_settings["Height"] * 0.95)
            Y1 = screen_settings["Height"] * 0.95

        if(Y2 < screen_settings["Height"] * 0.05)
            Y2 = screen_settings["Height"] * 0.05

        if(Y2 > screen_settings["Height"] * 0.95)
            Y2 = screen_settings["Height"] * 0.95


        var ReleaseRadius = rand(30,50)

        if(_ARG2[0] <= 100)
        {
            ReleaseRadius = rand(0,10)
        }else if(_ARG2[0] <= 200)
        {
            ReleaseRadius = rand(0,20)
        }

        var DoMouseUp = "true"

        if(rand(0,100) > 85)
        {
            ReleaseRadius = 0;
        }

        if(SpeedMultiplier > 1.0)
        {
            ReleaseRadius = rand(80,120)
        }

        if(ReleaseRadius == 0)
        {
            DoMouseUp = "false"
        }

        var Speed = rand(110,150)

        if(_ARG2[0] <= 100)
        {
            if(ReleaseRadius == 0)
            {
                Speed *= 0.8
            }else
            {
                Speed *= 1.7
            }
        }else if(_ARG2[0] <= 200)
        {
            if(ReleaseRadius == 0)
            {
                Speed *= 0.9
            }else
            {
                Speed *= 1.1
            }
        }

        Speed *= SpeedMultiplier;

        var Deviation = 2.3

        if(_ARG2[0] <= 100)
        {
            Deviation = 3.0
        }else if(_ARG2[0] <= 200)
        {
            Deviation = 2.5
        }

        var TrackScroll = (_ARG2[1]) ? "true" : "false"

        _ARG2 = [_ARG2[2]]

        if(IsSlideDown)
        {
            _ARG2.push(X1)
            _ARG2.push(Y1)
            _ARG2.push(X2)
            _ARG2.push(Y2)
        }else
        {
            _ARG2.push(X2)
            _ARG2.push(Y2)
            _ARG2.push(X1)
            _ARG2.push(Y1)
        }
        _ARG2.push({
                       "speed": Speed,
                       "gravity": rand(10,15),
                       "deviation": Deviation,
                       "do_mouse_up": DoMouseUp,
                       "release_radius": ReleaseRadius,
                       "relative_coordinates": "true",
                       "track_scroll": TrackScroll
                   })

        if(ReleaseRadius == 0)
        {
            //sleep time
            if(rand(0,10) > 7)
            {
                _ARG2.push(rand(100,600))
            }else
            {
                _ARG2.push(0)
            }
            X2 += rand(-5,15)


            if(IsSlideDown)
                Y2 += rand(-45,5)
            else
                Y2 = Y1 + rand(-5,45)

            if(X2 < screen_settings["Width"] * 0.05)
                X2 = screen_settings["Width"] * 0.05

            if(X2 > screen_settings["Width"] * 0.95)
                X2 = screen_settings["Width"] * 0.95


            if(Y2 < screen_settings["Height"] * 0.05)
                Y2 = screen_settings["Height"] * 0.05

            if(Y2 > screen_settings["Height"] * 0.95)
                Y2 = screen_settings["Height"] * 0.95

            _ARG2.push(X2)
            _ARG2.push(Y2)

            _ARG2.push({
                           "speed": rand(50,80),
                           "gravity": rand(8,10),
                           "deviation": 1.0,
                           "do_mouse_up": "true",
                           "release_radius": 0,
                           "relative_coordinates": "true",
                           "track_scroll": TrackScroll
                       })
        }
        move(_ARG2[1], _ARG2[2], {"speed": 300,"gravity": 10,"deviation": 0,"do_mouse_up": "false","release_radius": 0,"relative_coordinates": "true"}, function(){
            mouse_down(_ARG2[1], _ARG2[2], function(){
                move(_ARG2[3], _ARG2[4], _ARG2[5], function(){
                    _if(_ARG2.length > 6, function(){
                        _if(_ARG2[6] > 0, function(){
                            sleep(_ARG2[6], function(){})
                        },function(){
                            move(_ARG2[7], _ARG2[8], _ARG2[9], function(){})
                        })
                    },_ARG2[0])
                })
            })
        })
    })
}

function move()
{
    var length = arguments.length

    var x = arguments[0]
    var y = arguments[1]
    var params = arguments[2]
    var callback = arguments[3]
    var iscoordinates = false


    if(length == 4)
    {
        x = arguments[0]
        y = arguments[1]
        params = arguments[2]
        callback = arguments[3]
        iscoordinates = true
    }else if(length == 3)
    {
        x = arguments[0]
        y = arguments[1]
        params = {}
        callback = arguments[2]
        iscoordinates = true
    }else if(length == 2)
    {
        X = parseInt(_result().split(",")[0])
        Y = parseInt(_result().split(",")[1])
        x = X
        y = Y
        params = arguments[0]
        callback = arguments[1]
    }else if(length == 1)
    {
        X = parseInt(_result().split(",")[0])
        Y = parseInt(_result().split(",")[1])
        x = X
        y = Y
        callback = arguments[0]
        params = {}
    }else
    {
        fail("move, wrong number of arguments")
    }

    if(Object.keys(params).length == 0)
    {
        params = _DEFAULT_MOVE_PARAMS
    }

    params["iscoordinates"] = (iscoordinates) ? "true" : "false"

    _ARG = [x,y,params,callback]
    _create_browser_if_needed(function(){
        Browser.MouseMove(_ARG[0],_ARG[1],JSON.stringify(_ARG[2]),_get_function_body(_ARG[3]))
    })
}

function _clarify()
{
    _MOUSE_SETTINGS = _arguments
    _if(_result().length > 0, function(){
        X = parseInt(_result().split(",")[0])
        Y = parseInt(_result().split(",")[1])
        move(X,Y, _MOUSE_SETTINGS, function(){
            delete _MOUSE_SETTINGS
        })
    }, function(){delete _MOUSE_SETTINGS})
}

function _random_point()
{
    _SELECTOR_DELTA = null
    _SELECTOR_DELTA_PREV = null
    _SELECTOR_JUMP_NUMBER = null
    _SELECTOR_FOUND_ELEMENT = true
    _do(function(){
        if(_iterator() > 20)
            _break();
        var Selector;
        var Code;
        if(typeof(_SELECTOR) == "string")
        {
            Selector = _SELECTOR.split(">FRAME>")[0];
            Code = "(function(){if(!_BAS_HIDE(BrowserAutomationStudio_IsVisible)(self))return '';var rect = _BAS_HIDE(BrowserAutomationStudio_GetBoundingClientRect)(self);var top = rect.top;var bottom = rect.bottom;var height = _BAS_SAFE(Window.innerHeight); var center_element = _BAS_SAFE(Math.floor)((top + bottom) * 0.5); var center_viewport = _BAS_SAFE(Math.floor)((height) * 0.5); if((top < 0 && bottom > height) || (top >=0 && bottom <= height)) return '0'; return _BAS_HIDE(BrowserAutomationStudio_ToString)(_BAS_SAFE(Math.floor)(center_element - center_viewport));})()";
        }else
        {
            Selector = "";
            Code = "(function(){var height = _BAS_SAFE(Window.innerHeight);var current_position = _BAS_SAFE($Element.scrollTop)(_BAS_HIDE(BrowserAutomationStudio_GetScrollingNode)());var target_position = " + _SELECTOR.toString() + ";if ((target_position >= current_position) && (target_position <= height + current_position))return '0';return _BAS_HIDE(BrowserAutomationStudio_ToString)(_BAS_SAFE(Math.floor)(target_position - (current_position + height * 0.5)));})()";
        }
        get_element_selector(Selector, false).script(Code,function(){
            if(_result() == "")
            {
                _SELECTOR_FOUND_ELEMENT = false
                _break(1)
                return;
            }
            _SELECTOR_DELTA = parseInt(_result())
            if(_SELECTOR_DELTA == 0)
            {
                _break(1)
                return;
            }
            if(_SELECTOR_DELTA_PREV != null && (Math.abs(_SELECTOR_DELTA_PREV) - Math.abs(_SELECTOR_DELTA) < 10))
            {
                _break(1)
                return;
            }

            _if_else(_IS_MOBILE,function(){
                _SELECTOR_DELTA_PREV = _SELECTOR_DELTA

                var _ARG3 = []

                var SizeLeft = Math.abs(_SELECTOR_DELTA)

                var ScrollDirection = _SELECTOR_DELTA >= 0 ? 1.0 : -1.0

                for(var i = 0;i<5;i++)
                {
                    if(SizeLeft < 100)
                    {
                        break
                    }
                    if(
                            (i == 0) ||
                            (i == 1 && rand(0,100) < 80) ||
                            (i == 2 && rand(0,100) < 70) ||
                            (i == 3 && rand(0,100) < 60) ||
                            (i == 4 && rand(0,100) < 50)
                        )
                    {
                        var ScrollSize = rand(100, SizeLeft)
                        if(rand(0, 100) < 10)
                        {
                            var MaxSize = SizeLeft
                            if(MaxSize > 500)
                                MaxSize = 500
                            ScrollSize = rand(100, MaxSize)
                        }
                        var Track = false
                        ScrollSize *= 0.8
                        if(ScrollSize < 100)
                        {
                            ScrollSize = 100
                            SizeLeft -= 100
                        }
                        else if(ScrollSize > 1000)
                        {
                            Track = rand(0,100) < 10
                            ScrollSize = rand(250,500)
                        }else
                        {
                            ScrollSize = rand(200,500)
                        }

                        if(!Track)
                        {
                            if(SizeLeft - ScrollSize * 2.0 < 0 && SizeLeft - ScrollSize > 0)
                            {
                                SizeLeft -= ScrollSize
                                Track = true
                            }else
                            {
                                SizeLeft -= ScrollSize * 2.0
                            }
                        }
                        else
                            SizeLeft -= ScrollSize


                        if(SizeLeft>=0)
                        {
                            _ARG3.push([ScrollSize * ScrollDirection, Track])
                        }
                    }else
                    {
                        break;
                    }
                }

                if(_ARG3.length == 0)
                {
                    _ARG3.push([100 * ScrollDirection, false])
                }

                _ARG3[_ARG3.length - 1][1] = true

                _do(function(){
                    if(_iterator() - 1 >= _ARG3.length)
                        _break();

                    _slide(_ARG3[_iterator() - 1][0], _ARG3[_iterator() - 1][1], function(){
                        _if(rand(0,100) > 70, function(){
                            sleep(rand(100,1000), function(){})
                        }, function(){})

                    })
                }, function(){delete _ARG3})

            }, function(){

                var Speed = 100
                if(_SELECTOR_JUMP_NUMBER != null)
                    Speed = Math.round((Math.abs(_SELECTOR_DELTA_PREV) - Math.abs(_SELECTOR_DELTA))/_SELECTOR_JUMP_NUMBER)

                _SELECTOR_DELTA_PREV = _SELECTOR_DELTA
                var SleepTimeout = 100;


                var TypeItem = (_SELECTOR_DELTA > 0) ? "\u003cMOUSESCROLLDOWN\u003e" : "\u003cMOUSESCROLLUP\u003e";
                if(_SELECTOR_JUMP_NUMBER == null)
                    _SELECTOR_JUMP_NUMBER = 1;
                else
                {
                    _SELECTOR_JUMP_NUMBER = Math.round(Math.abs(_SELECTOR_DELTA)/Speed);

                    var RandJump = 20;

                    if(_SELECTOR_JUMP_NUMBER > 50)
                    {
                        SleepTimeout = 30;
                        RandJump = rand(40,49)
                    }else if(_SELECTOR_JUMP_NUMBER > 20)
                    {
                        SleepTimeout = 30;
                        RandJump = rand(15,19)
                    }
                    else
                    {
                        SleepTimeout = 100;
                        RandJump = rand(6,8)
                    }

                    if(_SELECTOR_JUMP_NUMBER > RandJump)
                        _SELECTOR_JUMP_NUMBER = RandJump;
                }
                var TypeString = "";
                for(var i = 0;i<_SELECTOR_JUMP_NUMBER;i++)
                {
                    TypeString += TypeItem
                }

                _type(TypeString,SleepTimeout,function(){sleep(rand(100,500), function(){})})

            }, function(){

            })




        })
    },function(){
        _if_else(_SELECTOR_FOUND_ELEMENT && typeof(_SELECTOR) == "string",function(){
            get_element_selector(_SELECTOR, false).random_point(function(){})
        },function(){
            _set_result("")
        }, function(){})
    })
}

function wait_code(callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.WaitCode(_get_function_body(_ARG[0]))
    })
}

function section_end(callback)
{
    Browser.WaitCode(_get_function_body(callback))
}

function load(text, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        if(_ARG[0] == "about:blank")
        {
            Browser.LoadPageInstant(_ARG[0],_get_function_body(_ARG[1]));
        }else
        {
            Browser.LoadPage(_ARG[0],"if(_result() == false){fail(tr('Failed to load page ') + _ARG[0])};" + _get_function_body(_ARG[1]));
        }
    })
}

function _load(text, referrer, is_instant, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.LoadPage2(_ARG[0],_ARG[1],_ARG[2],"if(_result().length > 0){fail(tr('Failed to load page ') + _ARG[0] + ' : ' + _result())};" + _get_function_body(_ARG[3]));
    })
}

function _set_combobox_index(index, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.SetComboboxIndex(_ARG[0], _get_function_body(_ARG[1]));
    })
}

function _popupcreate2(is_silent, url, referrer, is_instant, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.PopupCreate2(_ARG[0],_ARG[1],_ARG[2],_ARG[3],"if(_result().length > 0){fail(tr('Failed to load page ') + _ARG[1] + ' : ' + _result())};" + _get_function_body(_ARG[4]));
    })
}

function navigate_back(is_instant, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.NavigateBack(_ARG[0], _get_function_body(_ARG[1]));
    })
}

function load_instant(text, callback)
{
    _ARG = arguments

    _create_browser_if_needed(function(){
        Browser.LoadPageInstant(_ARG[0],_get_function_body(_ARG[1]));
    })
}

function open_file_dialog(text, callback)
{
    _ARG = arguments

    _create_browser_if_needed(function(){
        var sendtext = _ARG[0]
        if(_ARG[0] instanceof Array)
            sendtext = JSON.stringify(_ARG[0])
        Browser.SetOpenFileName(sendtext,_get_function_body(_ARG[1]));
    })
}

function drag_file(text, callback)
{
    _ARG = arguments

    _create_browser_if_needed(function(){
        Browser.DragFile(_ARG[0],_get_function_body(_ARG[1]));
    })
}

function prompt_result(text, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.SetPromptResult(_ARG[0],_get_function_body(_ARG[1]));
    })
}

function http_auth_result(login, pass, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.SetHttpAuthResult(_ARG[0],_ARG[1],_get_function_body(_ARG[2]));
    })
}

function screenshot(path, callback)
{
    _ARG = arguments

    _create_browser_if_needed(function(){
        page().render_file(_ARG[0], _ARG[1]);
    })
}

function url(callback)
{
    _ARG = arguments

    _create_browser_if_needed(function(){
        Browser.GetUrl(_get_function_body(_ARG[0]));
    })
}

function get_cookies(url, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.GetCookiesForUrl(_ARG[0],_get_function_body(_ARG[1]));
    })
}

function resize(x, y, callback)
{
    _ARG = arguments

    _create_browser_if_needed(function(){
        Browser.Resize(_ARG[0], _ARG[1],_get_function_body(_ARG[2]));
    })
}

function _set_device_scale_factor(scale, callback)
{
    _ARG_SCALE = []
    _ARG_SCALE.push(arguments[0])
    _ARG_SCALE.push(arguments[1])

    _create_browser_if_needed(function(){
        _get_browser_screen_settings(function(){
            var result = JSON.parse(_result())
            _ARG_SCALE.push(result["Width"])
            _ARG_SCALE.push(result["Height"])
            Browser.SetDeviceScaleFactor(_ARG_SCALE[0],_get_function_body(function(){
                resize(_ARG_SCALE[2],_ARG_SCALE[3], _ARG_SCALE[1])
            }));
        })
    })
}

function _set_device_scale_factor_no_resize(scale, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.SetDeviceScaleFactor(_ARG[0], _get_function_body(_ARG[1]));
    })
}

function save_cookies(callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.SaveCookies(_get_function_body(_ARG[0]));
    })
}

function restore_cookies(cookies, callback)
{
    _ARG_RESTORE_COOKIES = arguments
    _create_browser_if_needed(function(){
        //Load about:blank if browser is not yet created
        _popupinfo(function(){
            var json = JSON.parse(_result())
            _if_else(json["urls"].length == 0, function(){
                load("about:blank",function(){
                    Browser.RestoreCookies(_ARG_RESTORE_COOKIES[0], _get_function_body(_ARG_RESTORE_COOKIES[1]));
                    delete _ARG_RESTORE_COOKIES
                })
            }, function(){
                Browser.RestoreCookies(_ARG_RESTORE_COOKIES[0], _get_function_body(_ARG_RESTORE_COOKIES[1]));
                delete _ARG_RESTORE_COOKIES
            }, function(){

            })
        })
    })
}

function restore_localstorage(localstorage, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.RestoreLocalStorage(_ARG[0], _get_function_body(_ARG[1]));
    })
}

function page()
{
    _ensure_browser_created();
    return ScriptWorker.GetRootElement();
}

function clear_log()
{
    Logger.Clear();
}

function log(text)
{
    Logger.Write(ScriptWorker.PrepareMessage(text));
}

function log_html(html, text)
{
    Logger.WriteHtml(html, text);
}

function log_success(text)
{
    Logger.WriteSuccess(ScriptWorker.PrepareMessage(text));
}

function log_fail(text)
{
    Logger.WriteFail(ScriptWorker.PrepareMessage(text));
}

function ResultResolve(number)
{
    if(number === 0)
    {
        return Results1;
    }else if(number === 1)
    {
        return Results2;
    }else if(number === 2)
    {
        return Results3;
    }
    else if(number === 3)
        {
            return Results4;
        }

    else if(number === 4)
        {
            return Results5;
        }

    else if(number === 5)
        {
            return Results6;
        }

    else if(number === 6)
        {
            return Results7;
        }

    else if(number === 7)
        {
            return Results8;
        }

    else if(number === 8)
        {
            return Results9;
        }

}

function result(text, number)
{
    if(typeof(number) === "undefined")
    {
        number = 0;
    }
    ResultResolve(number).Write(text);
}

function result_html(html, text, number)
{
    if(typeof(number) === "undefined")
    {
        number = 0;
    }
    ResultResolve(number).WriteHtml(html, text);
}

function result_file(number)
{
    if(typeof(number) === "undefined")
    {
        number = 0;
    }
    return ResultResolve(number).GetFileName(number);
}

function css(text)
{
    return page().css(text);
}

function frame(name)
{
    return page().frame(name);
}

function frame_css(name)
{
    return page().frame_css(name);
}

function xpath(name)
{
    return page().xpath(name);
}

function xpath_all(name)
{
    return page().xpath_all(name);
}

function frame_match(name)
{
    return page().frame_match(name);
}


function position(x, y)
{
    return page().position(x,y);
}

function match(text)
{
    return page().match(text);
}

function match_all(text)
{
    return page().match_all(text);
}

function all(text)
{
    return page().all(text);
}

function thread_number()
{
    return ScriptWorker.GetThreadNumber();
}

function success_number()
{
    return ScriptWorker.GetSuccessNumber();
}

function project_path()
{
    return ScriptWorker.GetProjectPath();
}

function fail_number()
{
    return ScriptWorker.GetFailNumber();
}

function sleep(milliseconds, callback)
{
    ScriptWorker.Sleep(milliseconds,_get_function_body(callback));
}

function script(text, callback)
{
    page().script(text,callback);
}

function font_list(fonts, callback)
{
    _ARG = arguments

    _create_browser_if_needed(function(){
        Browser.SetFontList(_ARG[0],_get_function_body(_ARG[1]));
    })
}


function onloadjavascript(text, callback)
{
    _ARG = arguments

    _create_browser_if_needed(function(){
        Browser.SetStartupScript(_ARG[0],"Main",_get_target(),_get_function_body(_ARG[1]));
    })
}

function onloadjavascriptinternal(text, script_id, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.SetStartupScript(_ARG[0],_ARG[1],_get_target(),_get_function_body(_ARG[2]));
    })
}

function agent(text, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        Browser.SetUserAgent(_ARG[0],_get_function_body(_ARG[1]));
    })
}


function antigate(key)
{
    solver_property("antigate","key",key)
}

function rucaptcha(key)
{
    solver_property("rucaptcha","key",key)
}

function twocaptcha(key)
{
    solver_property("2captcha","key",key)
}

function capmonster(key)
{
    solver_property("capmonster","key",key)
}

function xevil(key)
{
    solver_property("xevil","key",key)
}

function solver_properties_clear(solver)
{

    if(solver == "antigate")
    {
        _BAS_SOLVER_PROPERTIES[solver] = {serverurl: "http://antigate.com/"}
    }else if(solver == "dbc")
    {
        _BAS_SOLVER_PROPERTIES[solver] = {serverurl: "http://api.dbcapi.me/"}
    }else if(solver == "rucaptcha")
    {
        _BAS_SOLVER_PROPERTIES[solver] = {serverurl: "http://rucaptcha.com/"}
    }else if(solver == "2captcha")
    {
        _BAS_SOLVER_PROPERTIES[solver] = {serverurl: "http://2captcha.com/"}
    }else
    {
        _BAS_SOLVER_PROPERTIES[solver] = {}
    }
}

function solver_property(solver,key,value)
{
    var h;
    if(!(solver in _BAS_SOLVER_PROPERTIES && _BAS_SOLVER_PROPERTIES[solver]))
    {
        _BAS_SOLVER_PROPERTIES[solver] = {}
    }
    h = _BAS_SOLVER_PROPERTIES[solver]

    h[key] = value

    _BAS_SOLVER_PROPERTIES[solver] = h
}

function dbc(key)
{
    solver_property("dbc","key",key)
}

function _solver_properties_list(solver)
{
    var h;
    if(!(solver in _BAS_SOLVER_PROPERTIES && _BAS_SOLVER_PROPERTIES[solver]))
        _BAS_SOLVER_PROPERTIES[solver] = {}

    h = _BAS_SOLVER_PROPERTIES[solver]
    return Object.keys(h).reduce(function(a,k){a.push(k);a.push(h[k]);return a},[])
}

function solve(match, url, callback)
{
    LAST_CAPTCHA_METHOD = match
    _ENGINE_CALLBACK = [match,url,callback];

    is_load(_ENGINE_CALLBACK[1], function(){
        if(_result() === false)
        {
            fail("CAPTCHA_FAIL : No image in cache");
        }
        cache_get_base64(_ENGINE_CALLBACK[1],function(){
            ScriptWorker.Solve(_ENGINE_CALLBACK[0], _result(),_solver_properties_list(_ENGINE_CALLBACK[0]),_get_function_body(_ENGINE_CALLBACK[2]));
        })
    })
}


function _recaptchav3(action, method, rucaptcha, serverurl, score, use_proxy, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        ScriptWorker.SetRecaptchaV3SolverProperties(_ARG[1], _ARG[2], _ARG[3], _ARG[4], _ARG[5]);
        if(_ARG[1] == "disable")
        {
            _ARG[0] = ""
        }
        Browser.SendRecaptchaV3List(_ARG[0],_get_function_body(_ARG[6]))
    })
}

function _solve_captcha(method, data_base64, params, fail_on_error, callback) {
    LAST_CAPTCHA_METHOD = method;
    const list = Object.keys(params).reduce(function (a, k) { return a.concat(k, params[k]) }, []);
    ScriptWorker.SolveCaptcha(method, data_base64, list, fail_on_error, _get_function_body(callback));
}

function solve_base64(match, data_base64, callback)
{
    LAST_CAPTCHA_METHOD = match
    ScriptWorker.Solve(match, data_base64,_solver_properties_list(match),_get_function_body(callback));
}

function solve_base64_no_fail(match, data_base64, callback)
{
    LAST_CAPTCHA_METHOD = match
    ScriptWorker.SolveNoFail(match, data_base64,_solver_properties_list(match),_get_function_body(callback));
}

function solver_failed()
{
    if(typeof(LAST_CAPTCHA_ID) != "undefined" && LAST_CAPTCHA_ID != "")
        ScriptWorker.GetSolver(LAST_CAPTCHA_METHOD).ReportBad(LAST_CAPTCHA_ID);
}



function progress()
{
    progress_value(-1);
}

function progress_value(val)
{
    ScriptWorker.ProgressValueSlot(val);
}

function progress_maximum(val)
{
    ScriptWorker.ProgressMaximumSlot(val);
}

function on_fail(callback)
{
    ScriptWorker.SetFailFunction(_get_function_body(callback));
}

function clear_on_fail()
{
    ScriptWorker.SetFailFunction("");
}

function on_success(callback)
{
    ScriptWorker.SetSuccessFunction(_get_function_body(callback));
}

function clear_on_success()
{
    ScriptWorker.SetSuccessFunction("");
}

function _on_fail(callback)
{
    var label = rand()
    _set_label(label)
    var c = CYCLES.Current()
    if(c)
        c.OnFail = "_rewind('" + label + "');" + _get_function_body(callback)
    ScriptWorker.SetFailFunction("_rewind('" + label + "');" + _get_function_body(callback));
}

function _on_fail_exceed(callback)
{
    var label = rand()
    _set_label(label)
    ScriptWorker.SetFailExceedFunction("_rewind('" + label + "');" + _get_function_body(callback));
}

function _on_success_exceed(callback)
{
    var label = rand()
    _set_label(label)
    ScriptWorker.SetSuccessExceedFunction("_rewind('" + label + "');" + _get_function_body(callback));
}

function _finnaly(callback)
{
    ScriptWorker.SetAbortFunction(_get_function_body(callback));
}

function _clear_on_fail()
{
    var c = CYCLES.Current()
    if(c)
        c.OnFail = ""

    ScriptWorker.SetFailFunction("");
}

function _on_success(callback)
{
    var label = rand()
    _set_label(label)
    var c = CYCLES.Current()
    if(c)
        c.OnSuccess = "_rewind('" + label + "');" + _get_function_body(callback)
    ScriptWorker.SetSuccessFunction("_rewind('" + label + "');" + _get_function_body(callback));
}

function _clear_on_success()
{
    var c = CYCLES.Current()
    if(c)
        c.OnSuccess = ""

    ScriptWorker.SetSuccessFunction("");
}

function _set_max_fail(max_fail_number)
{
    ScriptWorker.SetMaxFail(max_fail_number);
}

function _set_max_success(max_success_number)
{
    ScriptWorker.SetMaxSuccess(max_success_number);
}


function _BAS_DEC(callback)
{
    ScriptWorker.Decrypt(callback);
}

function _db_add_record(group_id, data_list, table_id, add_to_resources)
{
    if(!add_to_resources)
    {
        add_to_resources = false
    }
    return ScriptWorker.DatabaseAddRecord(group_id, data_list, table_id, add_to_resources);
}
function _db_select_records(selector, page_number,page_size, table_id,sorting_type,group_id,sort_column)
{
    if(!sorting_type)
    {
        sorting_type = "no sorting"
    }
    if(!group_id)
    {
        group_id = ""
    }
    if(!sort_column)
    {
        sort_column = -1
    }
    return ScriptWorker.DatabaseSelectRecords(JSON.stringify(selector),page_number,page_size, table_id,sorting_type,group_id,sort_column);
}

function _db_select_one_record(record_id, table_id)
{
    return ScriptWorker.DatabaseSelectOneRecord(record_id, table_id);
}

function _db_delete_group(group_id, delete_data, table_id)
{
    ScriptWorker.DatabaseDeleteGroup(group_id, delete_data, table_id);
}
function _db_delete_one_record(record_id, delete_resources, table_id)
{
    ScriptWorker.DatabaseDeleteOneRecord(record_id, delete_resources, table_id);
}
function _db_change_group(name, description, group, table_id)
{
    ScriptWorker.DatabaseChangeGroup(name, description, group, table_id);
}
function _db_copy_group(group_id, table_id)
{
    return ScriptWorker.DatabaseCopyGroup(group_id, table_id);
}

function _db_move_record_to_group(group_id, record_id, move_type, table_id)
{
    return ScriptWorker.DatabaseMoveRecordToGroup(group_id, record_id, move_type, table_id);
}

function _db_select_groups(table_id)
{
    return ScriptWorker.DatabaseSelectGroups(table_id);
}


function _db_delete_records(selector, table_id, group_id, delete_resources)
{
    if(!group_id)
    {
        group_id = ""
    }
    if(!delete_resources)
    {
        delete_resources = false
    }
    ScriptWorker.DatabaseDeleteRecords(JSON.stringify(selector),table_id,group_id,delete_resources);
}

function _db_count_records(selector, table_id, group_id)
{
    return ScriptWorker.DatabaseCountRecords(JSON.stringify(selector),table_id,group_id);
}


function _db_update_record(record_id, data_list, table_id, update_resources)
{
    if(!update_resources)
    {
        update_resources = false
    }
    return ScriptWorker.DatabaseUpdateRecord(record_id, data_list, table_id, update_resources);
}

function _db_update_records_by_criteria(data_list, table_id, update_resources, filters, group_id)
{
    return ScriptWorker.DatabaseUpdateRecordsByCriteria(data_list, table_id, update_resources, JSON.stringify(filters), group_id);
}

function _db_add_group(group_name, group_description, table_id)
{
    return ScriptWorker.DatabaseAddGroup(group_name, group_description, table_id);
}

function _BAS_WEBINTERFACE_WAIT()
{
    _do(function(){
        ScriptWorker.WaitNextTask(_get_function_body(function(){
            var Name = ScriptWorker.GetTaskFunctionName()
            var Params = JSON.parse(ScriptWorker.GetTaskFunctionParams())
            _call_task(eval(Name),Params,function(){
                ScriptWorker.SendTaskResult(JSON.stringify({Message: "", Result: null,Success:true}))
            })
        }))
    }, function(){
        success("")
    })
}

function _on_start()
{
    if(ScriptWorker.SubstageGetParentId() > 0)
    {
        _call(eval(ScriptWorker.SubstageGetStartingFunction()), null, function(){
            success("")
        })
        return
    }

    if(typeof(OnApplicationStart) == "undefined")
    {
        _break(1)
        return
    }

    _do(function(){
        APP_START = P("_","_s")
        if(APP_START == "2")
        {
            _break(2)
            return
        }
        if(APP_START == "")
        {
            PSet("_","_s","1")
            _break(1)
            return
        }
        sleep(1000, function(){})
    }, function(){
        _call(OnApplicationStart,null, function(){
            PSet("_","_s","2")
            success("none")
        })
    })
}

function native(dll,func,data)
{
    return ScriptWorker.ExecuteNativeModuleCodeSync(dll,func,data);
}

function native_async(dll,func,data,callback)
{
    ScriptWorker.ExecuteNativeModuleCodeAsync(dll,func,data,_get_function_body(callback));
}

function general_timeout_next(timeout)
{
    ScriptWorker.SetGeneralWaitTimeoutNext(timeout);
}

function general_timeout(timeout)
{
    ScriptWorker.SetGeneralWaitTimeout(timeout);
    BROWSERAUTOMATIONSTUDIO_WAIT_TIMEOUT = Math.floor(timeout/1000);
}

function async_load_timeout(timeout)
{
    BROWSERAUTOMATIONSTUDIO_FULL_LOAD_TIMEOUT = Math.floor(timeout/1000);
}

function solver_timeout_next(timeout)
{
    ScriptWorker.SetSolverWaitTimeoutNext(timeout);
}

function solver_timeout(timeout)
{
    ScriptWorker.SetSolverWaitTimeout(timeout);
}

function _preprocess(script)
{
    return ScriptWorker.Preprocess(script);
}

function _embedded_parse_variables(data)
{
    var variables = JSON.parse(data["data"])
    var keys = Object.keys(variables)
    var length = keys.length
    for(var i = 0;i<length;i++)
    {
        var v = keys[i]
        GLOBAL["VAR_" + v] = variables[v]
    }
}

function _embedded_prepare_variables(variables, error)
{

    var split = variables.split(",")
    var variables_object = {}
    for(var i = 0;i<split.length;i++)
    {
        var v = split[i]
        try
        {
            variables_object[v] = GLOBAL["VAR_" + v]
        }catch(e)
        {
        }
    }
    if(GLOBAL.hasOwnProperty("VAR__BAS_FUNCTION_RESULT_"))
    {
        try
        {
            variables_object["_BAS_FUNCTION_RESULT_"] = GLOBAL["VAR__BAS_FUNCTION_RESULT_"]
        }catch(e)
        {
        }
    }
    if(error)
    {
        variables_object["-BAS-API-ERROR-"] = error;

    }
    var variables_string = JSON.stringify(variables_object);
    delete GLOBAL["VAR__BAS_FUNCTION_RESULT_"]
    return variables_string
}


function _embedded(function_name, language_name, language_version, variables, timeout, callback)
{
    var variables_string = _embedded_prepare_variables(variables);
    ScriptWorker.Embedded(variables_string,variables,function_name,language_name,language_version,timeout,_get_function_body(callback));
}

/* Custom threads */


function _thread_start(id, is_new_thread, name, params, stop_thread_after_finish, max_threads, start_postponed_on_idle, callback)
{
    if(ScriptWorker.GetIsRecord())
    {
        //Is record, fallback to task call
        ScriptWorker.CustomThreadStartSlot(id);
        ScriptWorker.CustomThreadRunFunctionSlot(id, name, JSON.stringify(params), stop_thread_after_finish, start_postponed_on_idle);
        _call_task(eval(name),params,callback)
    }else
    {
        if(ScriptWorker.CustomThreadExists(id))
        {
            //Start in existing thread
            ScriptWorker.CustomThreadRunFunctionSlot(id, name, JSON.stringify(params), stop_thread_after_finish, start_postponed_on_idle);
        }else if(is_new_thread && ScriptWorker.CustomThreadCanStartInIddleThread())
        {
            //Start in other thread, that is iddle
            var iddle_thread_id = ScriptWorker.CustomThreadGetIddleThread();
            if(iddle_thread_id != 0)
                ScriptWorker.CustomThreadRunFunctionSlot(iddle_thread_id, name, JSON.stringify(params), stop_thread_after_finish, start_postponed_on_idle);
        }else if(max_threads > ScriptWorker.CustomThreadsTotal())
        {
            //Start new thread
            ScriptWorker.CustomThreadStartSlot(id);
            ScriptWorker.CustomThreadRunFunctionSlot(id, name, JSON.stringify(params), stop_thread_after_finish, start_postponed_on_idle);
        }else
        {
            //Queue thread to start later
            ScriptWorker.CustomThreadQueueFunctionSlot(id, name, JSON.stringify(params), stop_thread_after_finish, start_postponed_on_idle, max_threads);
        }

        ScriptWorker.SetScript(_get_function_body(callback))
        ScriptWorker.RunSubScript()
    }

}

function _thread_stop(id)
{
    if(typeof(id) == "number")
    {
        ScriptWorker.CustomThreadStopSlot(id);
    }else
    {
        ScriptWorker.CustomThreadStopListSlot(id.join(","));
    }
}

function _thread_get_status(id)
{
    var result = null
    try
    {
        result = JSON.parse(ScriptWorker.CustomThreadGetResult(id))
    }catch(e)
    {
        result = null
    }

    return {
        "is_running": ScriptWorker.CustomThreadGetIsRunning(id),
        "is_success": ScriptWorker.CustomThreadGetIsSuccess(id),
        "result": result,
        "error": ScriptWorker.CustomThreadGetError(id),
        "was_running_at_least_once": ScriptWorker.CustomThreadGetWasRunningAtLeastOnce(id),
    }
}

function _thread_wait(id, func)
{
    _ARG = [id,false]
    waiter_nofail_next()
    wait("",function(){
        var result = true
        var id = _arguments()

        if(typeof(id) == "number")
        {
            var status = _thread_get_status(id)
            if(!status.was_running_at_least_once || status.is_running)
            {
                result = false
            }
        }else
        {
            for(var i = 0;i<id.length;i++)
            {
                var status = _thread_get_status(id[i])
                if(!status.was_running_at_least_once || status.is_running)
                {
                    result = false
                    break
                }
            }
        }

        if(result)
        {
            _ARG[1] = true
            _set_result(true);
        }

    },id,function(){
        if(!_ARG[1])
        {
            _thread_stop(id);
            fail(tr("Failed to wait for async function call"));
        }
        func();
    });

}
