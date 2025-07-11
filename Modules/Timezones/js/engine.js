function _get_ip_info()
{
    _ARG = _arguments()
    
    _if_else(_ARG[1] == "database", function(){
        _set_result(JSON.parse(native("timezones", "ipinfo", _ARG[0])))
    },function(){
        _if_else(_ARG[1] == "ip-api.com", function(){
            _switch_http_client_internal()
	        http_client_set_fail_on_error(false)
            
            _do(function(){

                var Now = Date.now()

                /* Parse request list */
                var RequestList = P("ipapi", "requestlist")
                if(!RequestList)
                    RequestList = []
                else
                {
                    RequestList = JSON.parse(RequestList)
                }

                /* Remove old requests */
                RequestList = RequestList.filter(function(Item){return (Now - Item.date < 3 * 60 * 1000)});

                /* Parse ban time */
                var EndTime = P("ipapi", "endtime")
                if(!EndTime)
                    EndTime = 0
                else
                {
                    EndTime = parseInt(EndTime)
                }

                /* If ban time finished, reset block */
                if(EndTime > 0 && Date.now() > EndTime)
                {
                    EndTime = 0;
                    PSet("ipapi", "endtime", "0")
                }

                /* Create request id for this thread or get old */
                if(typeof(IPAPI_REQEUSTID) == "undefined")
                {
                    IPAPI_REQEUSTID = rand()
                }

                /* Check if possible to do request */
                var DoRequest = (RequestList.length < 5 || _ARG[2].length > 0) && EndTime == 0 && RequestList.indexOf(function(Item){return Item.id == IPAPI_REQEUSTID}) < 0



                /* Add request to data structure */
                if(DoRequest)
                {
                    RequestList.push({id: IPAPI_REQEUSTID, date: Now})
                    PSet("ipapi", "requestlist", JSON.stringify(RequestList))
                }


                _if(DoRequest, function(){

                    http_client_get2("http://" + (_ARG[2].length > 0 ? "pro." : "") + "ip-api.com/json/" + _ARG[0].replace(/\[/g,"").replace(/\]/g,"") + "?fields=status,message,countryCode,city,lat,lon,timezone,offset" + (_ARG[2].length > 0 ? ("&key=" + _ARG[2]) : ""),{method:("GET"),headers:("Accept-Encoding: gzip, deflate")})!

                    /* Delete request id */
                    var RequestList = P("ipapi", "requestlist")
                    RequestList = JSON.parse(RequestList)
                    RequestList = RequestList.filter(function(Item){return (Item.id != IPAPI_REQEUSTID)});
                    PSet("ipapi", "requestlist", JSON.stringify(RequestList))

                    var EndTime = http_client_header("X-Ttl")
                    var RequestLeft = http_client_header("X-Rl")

                    if(EndTime && RequestLeft)
                    {
                        /* Temporary ban new requests */
                        RequestLeft = parseInt(RequestLeft)
                        if(RequestLeft < 20)
                        {
                            EndTime = Date.now() + (parseInt(EndTime) + 15) * 1000
                            PSet("ipapi", "endtime", EndTime.toString())
                        }
                    }
                    
                    
                    if(http_client_status() == 200 || http_client_status() == 403)
                    {
                        var json = http_client_content()
                        try
                        {
                            var json_parsed = JSON.parse(json)
                            if(json_parsed["status"] == "success")
                            {
                                delete json_parsed["status"]
                                json_parsed["country"] = json_parsed["countryCode"]
                                delete json_parsed["countryCode"]
                                json_parsed["latitude"] = json_parsed["lat"]
                                delete json_parsed["lat"]
                                json_parsed["longitude"] = json_parsed["lon"]
                                var timezone_json = JSON.parse(native("timezones", "timezone_name_to_offset", json_parsed["timezone"]))
                                json_parsed["offset"] = timezone_json["offset"] || 0
                                json_parsed["dstoffset"] = timezone_json["dstoffset"] || 0

                                json_parsed["valid"] = true

                                _set_result(json_parsed)
                                _break(2)
                            }else
                            {
                                _set_result({valid:false})
                                _break(2)
                            }
                        }catch(e){}
                    }
                })!
                
        
                sleep(3000)!
            })!  

            http_client_set_fail_on_error(true)
	        _switch_http_client_main()
        }, function(){
            if(typeof(_ARG[3]) != "function")
            {
                fail("\"Custom IP information function\" parameter is not a function.")
            }
            _call_function(_ARG[3],{ "Ip" : (_ARG[0])  })!
            var FunctionResult = _result_function()
            if(typeof(FunctionResult) != "object")
            {
                fail("IP information function result is not an object")
            }
            if(typeof(FunctionResult.valid) != "boolean")
            {
                fail("IP information function result doesn't contain \"valid\" property.")
            }
            if(FunctionResult.valid)
            {
                if(typeof(FunctionResult.country) == "undefined")
                {
                    fail("IP information function result doesn't contain \"country\" property.")
                }   
                if(typeof(FunctionResult.latitude) == "undefined")
                {
                    fail("IP information function result doesn't contain \"latitude\" property.")
                }   
                if(typeof(FunctionResult.longitude) == "undefined")
                {
                    fail("IP information function result doesn't contain \"longitude\" property.")
                }   
                if(typeof(FunctionResult.timezone) == "undefined")
                {
                    fail("IP information function result doesn't contain \"timezone\" property.")
                }   
                if(typeof(FunctionResult.offset) == "undefined")
                {
                    fail("IP information function result doesn't contain \"offset\" property.")
                }   
                if(typeof(FunctionResult.dstoffset) == "undefined")
                {
                    fail("IP information function result doesn't contain \"dstoffset\" property.")
                }   
            }
            _set_result(FunctionResult)
        })!
    })!
    
}
