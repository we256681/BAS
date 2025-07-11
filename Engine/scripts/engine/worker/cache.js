_BROWSERAUTOMATIONSTUDIO_TARGET = ""
_PROXY = {server: "", Port:0, IsHttp: true, name: "", password: ""}

function _set_target(target)
{
    _BROWSERAUTOMATIONSTUDIO_TARGET = target
}

function _get_target()
{
    var res = "";

    if(typeof(_BROWSERAUTOMATIONSTUDIO_TARGET) != "string")
    {
        try{
            _BROWSERAUTOMATIONSTUDIO_TARGET["tab"] = _BROWSERAUTOMATIONSTUDIO_TARGET["tab"].toString()
        }catch(e){}
        res = JSON.stringify(_BROWSERAUTOMATIONSTUDIO_TARGET)
    }

    _BROWSERAUTOMATIONSTUDIO_TARGET = ""
    return res
}

function _get_network_access_manager()
{
    if(typeof(NetworkAccessManagerVirtual)!='undefined')
    {
        return NetworkAccessManagerVirtual;
    }else if(typeof(NetworkAccessManager)!='undefined')
    {
        return NetworkAccessManager;
    }else
    {
        die("Manual mode is enabled but browser is not created. Either remove all 'Open Browser' and 'Close Browser' actions or add 'Open Browser' befor this one.")
    }

}

function header(name, value, callback)
{
    if(!_is_bas_browser_real() && name == "User-Agent")
    {
        _settings({"multilogin.fingerprintGeneration.userAgent":JSON.stringify(value)}, callback)
        return
    }
    if(!_is_bas_browser_real() && name == "Accept-Language")
    {
        _settings({"multilogin.fingerprintGeneration.langHdr":JSON.stringify(value)}, callback)
        return
    }
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().AddHeader(_ARG[0], _ARG[1], _get_target(), _get_function_body(_ARG[2]));
    })
}

function _set_accept_language_pattern(pattern, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().SetAcceptLanguagePattern(_ARG[0], _get_function_body(_ARG[1]));
    })
}

function _set_user_agent_data(data, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().SetUserAgentData(_ARG[0], _get_function_body(_ARG[1]));
    })
}

function header_order(json, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().SetHeaderList(JSON.stringify(_ARG[0]), _get_function_body(_ARG[1]));
    })
}

function clear_header(callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().CleanHeader(_get_function_body(_ARG[0]));
    })
}

function _reset_proxy()
{
    Browser.ResetProxy();
    _PROXY = {server: "", Port:0, IsHttp: true, name: "", password: ""}
}

function proxy(proxy_string, callback)
{
    var proxy_parsed = proxy_parse(proxy_string);
    set_proxy(proxy_parsed["server"], proxy_parsed["Port"], proxy_parsed["IsHttp"], proxy_parsed["name"], proxy_parsed["password"], callback)
}

function _is_ip(server)
{
    return (typeof(server) == "string" && (server.length == 0 || server.match(/^\d+\.\d+\.\d+\.\d+$/) != null));
}

function set_proxy(server, Port, IsHttp, name, password, callback)
{
    _PROXY["Port"] = Port
    _PROXY["IsHttp"] = IsHttp
    _PROXY["name"] = name
    _PROXY["password"] = password
    _PROXY["target"] = _get_target()
    _PROXY["server"] = server

    /*if(Browser.HasBrowserProcess() && Browser.GetBrowserEngine().indexOf("MLA") == 0)
        _info(tr("Proxy should be set before any other action that uses browser. This rule applies only if you are using Multilogin."))*/

    if(_is_ip(server))
    {
        _settings(_PROXY,callback)
    }else
    {
        _ARG = arguments

        _dns_lookup(server, function(){
            if(_result().length == 0)
                fail("Failed to resolve " + _ARG[0])
            else
            {
                _PROXY["server"] = _result()[0]
                _settings(_PROXY,_ARG[5])
            }

        })


    }
}

function _set_proxy_for_next_profile(proxy_string, callback)
{
    if(proxy_string == "block" || proxy_string == "direct")
    {
        _settings({"ProxyForNextProfile": proxy_string},callback)
    }else
    {
        _PROXY_NEXT_PROFILE = proxy_parse(proxy_string)

        if(_is_ip(_PROXY_NEXT_PROFILE["server"]))
        {
            var ProxyForNextProfile = JSON.stringify(_PROXY_NEXT_PROFILE)
            ProxyForNextProfile = base64_encode(ProxyForNextProfile)

            delete _PROXY_NEXT_PROFILE

            _settings({"ProxyForNextProfile": ProxyForNextProfile},callback)
        }else
        {
            _ARG = arguments

            _dns_lookup(_PROXY_NEXT_PROFILE.server, function(){
                if(_result().length == 0)
                {
                    delete _PROXY_NEXT_PROFILE

                    _settings({"ProxyForNextProfile": "eyJzZXJ2ZXIiOiIxMjcuMC4wLjEiLCJQb3J0IjowLCJJc0h0dHAiOnRydWUsIm5hbWUiOiIiLCJwYXNzd29yZCI6IiJ9"},_ARG[1])
                }
                else
                {
                    _PROXY_NEXT_PROFILE["server"] = _result()[0]
                    var ProxyForNextProfile = JSON.stringify(_PROXY_NEXT_PROFILE)
                    ProxyForNextProfile = base64_encode(ProxyForNextProfile)

                    delete _PROXY_NEXT_PROFILE

                    _settings({"ProxyForNextProfile": ProxyForNextProfile},_ARG[1])
                }

            })
        }
    }
}

function set_proxy_extended()
{
    var DetectExternalIp = arguments[0]
    var ChangeGeolocation = arguments[1]
    var ChangeWebrtcIp = arguments[2]
    var ChangeTimezone = arguments[3]

    var ChangeBrowserLanguage = false
    var callback = null

    var IpInfoMethod = "database"
    var IpInfoKey = ""
    var IpInfoFunctionName = null

    if(arguments.length == 5)
    {
        callback = arguments[4]
    }

    if(arguments.length == 6)
    {
        ChangeBrowserLanguage = arguments[4]
        callback = arguments[5]
    }


    if(arguments.length == 9)
    {
        ChangeBrowserLanguage = arguments[4]
        IpInfoMethod = arguments[5]
        IpInfoKey = arguments[6]
        IpInfoFunctionName = arguments[7]
        callback = arguments[8]
    }



    _if_else(DetectExternalIp || _PROXY["server"].length == 0, function(){
        browser_ip(function(){
            if(typeof(_result()) != "string" || _result().length == 0)
            {
                browser_ip_https(function(){
                    if(typeof(_result()) != "string" || _result().length == 0)
                    {
                        fail("Failed to get proxy ip")
                    }else
                    {
                        _PROXY["ip"] = _result()
                    }
                })

            }else
            {
                _PROXY["ip"] = _result()
            }
        })
    }, function(){
        _PROXY["ip"] = _PROXY["server"]
    }, function(){

        _if_else(_PROXY["ip"].length == 0, function(){
            _if(ChangeGeolocation, function(){
                geolocation(100000,100000, function(){})
            }, function(){
                _if(ChangeWebrtcIp, function(){
                    _settings({"Webrtc":"disable"}, function(){})
                }, function(){
                    _if(ChangeTimezone, function(){
                        _settings({"Timezone":"100000","TimezoneName":"BAS_NOT_SET"}, function(){})
                    }, function(){
                        _if(ChangeBrowserLanguage, function(){
                            header("Accept-Language", "en", function(){})
                        }, function(){})
                    })
                })
            })
        }, function(){

            _call(_get_ip_info, [_PROXY["ip"], IpInfoMethod, IpInfoKey, IpInfoFunctionName], function(){

                JSON_TEMP = _result()

                _if(ChangeGeolocation, function(){
                    _if_else(JSON_TEMP["valid"], function(){
                        geolocation(JSON_TEMP["latitude"],JSON_TEMP["longitude"],function(){})
                    }, function(){
                        geolocation(100000,100000, function(){})
                    }, function(){})
                }, function(){
                    _if(ChangeWebrtcIp, function(){
                        _if_else(JSON_TEMP["valid"], function(){
                            _settings({"Webrtc":"replace","WebrtcIps":_PROXY["ip"]},function(){})
                        }, function(){
                            _settings({"Webrtc":"disable"},function(){})
                        }, function(){})
                    }, function(){
                        _if(ChangeTimezone, function(){
                            _if_else(JSON_TEMP["valid"], function(){
                                _settings({"Timezone":(-JSON_TEMP["offset"]).toString(),"TimezoneName":JSON_TEMP["timezone"]}, function(){})
                            }, function(){
                                _settings({"Timezone":"0","TimezoneName":""}, function(){})
                            }, function(){})
                        }, function(){
                            _if(ChangeBrowserLanguage, function(){
                                _if_else(JSON_TEMP["valid"], function(){
                                    var country = JSON_TEMP["country"].toUpperCase()
                                    var language = native("timezones", "country_to_language", country)
                                    header("Accept-Language", language + "-" + country, function(){
                                        _settings({"Fingerprints.Locale":JSON_TEMP["country"].toLowerCase()}, function(){})
                                    })

                                }, function(){
                                    header("Accept-Language", "en", function(){})
                                }, function(){})
                            }, function(){delete JSON_TEMP})
                        })
                    })
                })

            })



        }, callback)
    })






}

function browser_ip(callback)
{
    _ARG = arguments
    _set_result("")
    _switch_http_client_internal()
    http_client_set_fail_on_error(false)
    http_client_set_proxy(_PROXY["server"], _PROXY["Port"], _PROXY["IsHttp"], _PROXY["name"], _PROXY["password"])
    http_client_get2("http://ip.bablosoft.com/?requestid=" + rand(0,100000),{method:("GET"),headers:("")}, function(){
        if(http_client_was_error() || http_client_status() != 200)
            _set_result("")
        else
            _set_result(http_client_encoded_content("auto"))

        new_http_client()
        _switch_http_client_main()
        _ARG[0]()
    })

}

function browser_ip_https(callback)
{
    _ARG = arguments
    _set_result("")
    _switch_http_client_internal()
    http_client_set_fail_on_error(false)
    http_client_set_proxy(_PROXY["server"], _PROXY["Port"], _PROXY["IsHttp"], _PROXY["name"], _PROXY["password"])
    http_client_get2("https://ip" + rand(1,3) + ".bablosoft.com?requestid=" + rand(0,100000),{method:("GET"),headers:("")}, function(){
        if(http_client_was_error() || http_client_status() != 200)
            _set_result("")
        else
            _set_result(http_client_encoded_content("auto"))

        new_http_client()
        _switch_http_client_main()
        _ARG[0]()
    })

}

function cache_allow(match, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().AddCacheMaskAllow(_ARG[0],_get_function_body(_ARG[1]))
    })
}

function cache_deny(match, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().AddCacheMaskDeny(_ARG[0],_get_function_body(_ARG[1]))
    })
}

function request_allow(match, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().AddRequestMaskAllow(_ARG[0],_get_function_body(_ARG[1]))
    })
}

function request_deny(match, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().AddRequestMaskDeny(_ARG[0],_get_function_body(_ARG[1]))
    })
}

function cache_get_base64(match, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().FindCacheByMaskBase64(_ARG[0],_get_function_body(_ARG[1]))
    })
}

function cache_get_string(match, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().FindCacheByMaskString(_ARG[0],_get_function_body(_ARG[1]))
    })
}

function _cache_get_all(match, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().FindAllCache(_ARG[0],_get_function_body(_ARG[1]))
    })
}

function cache_get_status(match, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().FindStatusByMask(_ARG[0],_get_function_body(_ARG[1]))
    })
}

function cache_get_url(match, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().FindUrlByMask(_ARG[0],_get_function_body(_ARG[1]))
    })
}

function cache_clear(callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().ClearAll(_get_function_body(_ARG[0]))
    })
}

function cache_data_clear(callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().ClearData(_get_function_body(_ARG[0]))
    })
}

function cache_masks_clear(callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().ClearMasks(_get_function_body(_ARG[0]))
    })
}

function is_load(match, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().IsUrlLoadedByMask(_ARG[0],_get_function_body(_ARG[1]))
    })
}

function get_load_stats(callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().GetLoadStats(_get_function_body(_ARG[0]))
    })
}

function _restrict_popups(callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().RestrictPopups(_get_function_body(_ARG[0]))
    })
}

function _allow_popups(callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().AllowPopups(_get_function_body(_ARG[0]))
    })
}

function _enable_notifications(enable, callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _settings({"Fingerprints.Notifications":((_ARG[0]) ? "Enable" : "Disable")}, _ARG[1])
    })
}

function _restrict_downloads(callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().RestrictDownloads(_get_function_body(_ARG[0]))
    })
}

function _allow_downloads(callback)
{
    _ARG = arguments
    _create_browser_if_needed(function(){
        _get_network_access_manager().AllowDownloads(_get_function_body(_ARG[0]))
    })
}

function _dns_lookup(domain, callback)
{
    ScriptWorker.DNSLookup(domain, _get_function_body(callback))
}
