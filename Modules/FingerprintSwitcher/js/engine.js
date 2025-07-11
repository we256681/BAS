function BrowserAutomationStudio_PerformanceFingerprint()
{
	FINGERPRINT_JSON = _arguments()
	if(!FINGERPRINT_JSON.time_deformation)
	{
		FINGERPRINT_JSON.time_deformation = Math.random() * (FINGERPRINT_JSON.time_deformation_to - FINGERPRINT_JSON.time_deformation_from) + FINGERPRINT_JSON.time_deformation_from
	}

	delete FINGERPRINT_JSON.time_deformation_to
	delete FINGERPRINT_JSON.time_deformation_from

	if(_get_profile().length > 0)
	{
	    native("filesystem", "writefile", JSON.stringify({path: _get_profile() + "/performance.json",value: JSON.stringify(FINGERPRINT_JSON),base64:false,append:false}))
	}

	_settings(
		{
			"Fingerprints.Performance.Precision": (FINGERPRINT_JSON.disable_performance_precision ? "Enable": "Disable"),
			"Fingerprints.Performance.TimeDeformation": (FINGERPRINT_JSON.time_deformation).toString(),
			"Fingerprints.Performance.MaxMeasurmentTime": (FINGERPRINT_JSON.max_measurement_time).toString()
		})!
}

function BrowserAutomationStudio_GetFingerprint()
{
	if(_arguments().length != 1)
		fail("Wrong number of input params")

	FINGERPRINT_JSON = _arguments()[0]

	if(typeof(FINGERPRINT_JSON.min_browser_version) == "string" && FINGERPRINT_JSON.min_browser_version == "current")
	{
		FINGERPRINT_JSON.min_browser_version = _get_current_browser_version_info("minimal")
	}

	if(typeof(FINGERPRINT_JSON.max_browser_version) == "string" && FINGERPRINT_JSON.max_browser_version == "current")
	{
		FINGERPRINT_JSON.max_browser_version = _get_current_browser_version_info("minimal")
	}

	var q = (FINGERPRINT_JSON.tags).split(",").map(function(el){return el.trim()})
	if(q.length == 0 || q.length == 1 && q[0] == "*")
	{
		q = ((FINGERPRINT_JSON.key).length > 0) ? ("?version=5&&key=" + encodeURIComponent(FINGERPRINT_JSON.key)) : "?version=5"
		if(FINGERPRINT_JSON.perfectcanvas_request.length > 0)
		{
			q += "&tags=*"
		}
	}else
	{
    	q = "?version=5&tags=" + encodeURIComponent(q.join(",")) + (((FINGERPRINT_JSON.key).length > 0) ? ("&key=" + encodeURIComponent(FINGERPRINT_JSON.key)) : "")
	}

	if(FINGERPRINT_JSON.min_browser_version != "*")
		q += "&min_browser_version=" + parseInt(FINGERPRINT_JSON.min_browser_version)

	if(FINGERPRINT_JSON.max_browser_version != "*")
		q += "&max_browser_version=" + parseInt(FINGERPRINT_JSON.max_browser_version)

	if(FINGERPRINT_JSON.min_width != "*")
		q += "&min_width=" + parseInt(FINGERPRINT_JSON.min_width)	

	if(FINGERPRINT_JSON.min_height != "*")
		q += "&min_height=" + parseInt(FINGERPRINT_JSON.min_height)	

	if(FINGERPRINT_JSON.max_width != "*")
		q += "&max_width=" + parseInt(FINGERPRINT_JSON.max_width)	

	if(FINGERPRINT_JSON.max_height != "*")
		q += "&max_height=" + parseInt(FINGERPRINT_JSON.max_height)

	if(FINGERPRINT_JSON.time_limit != "*")
		q += "&time_limit=" + encodeURIComponent(FINGERPRINT_JSON.time_limit)	

	FINGERPRINT_JSON.additional = {}
	FINGERPRINT_JSON.additional.was_simplified_perfect_canvas_request = false
	FINGERPRINT_JSON.additional.api_url = null
	FINGERPRINT_JSON.additional.server_type_is_perfect_canvas = false
	FINGERPRINT_JSON.additional.can_use_simplified_perfect_canvas_request = false
	FINGERPRINT_JSON.additional.server_type_is_post_data = false
	FINGERPRINT_JSON.additional.final_fingerprint_valid = false
	FINGERPRINT_JSON.additional.dynamic_perfect_canvas = typeof(FINGERPRINT_JSON.dynamic_perfect_canvas) == "string" && FINGERPRINT_JSON.dynamic_perfect_canvas == "true"
	FINGERPRINT_JSON.additional.enable_precomputed_fingerprints = true
	if(typeof(FINGERPRINT_JSON.enable_precomputed_fingerprints) == "string" && FINGERPRINT_JSON.enable_precomputed_fingerprints == "false")
	{
		FINGERPRINT_JSON.additional.enable_precomputed_fingerprints = false
	}
	FINGERPRINT_JSON.additional.is_custom_server = typeof(FINGERPRINT_JSON.enable_custom_server) == "string" && FINGERPRINT_JSON.enable_custom_server == "true"


	if(FINGERPRINT_JSON.additional.is_custom_server)
	{
		if(typeof(FINGERPRINT_JSON.is_custom_server_retry) == "boolean" && FINGERPRINT_JSON.is_custom_server_retry)
		{
			FINGERPRINT_JSON.additional.server_type_is_perfect_canvas = true;
			FINGERPRINT_JSON.additional.server_type_is_post_data = true;
			FINGERPRINT_JSON.additional.status_url = "https://customcanvas.bablosoft.com/status"
			FINGERPRINT_JSON.additional.api_url = "https://customcanvas.bablosoft.com/prepare"
			FINGERPRINT_JSON.additional.can_use_simplified_perfect_canvas_request = false;
		}else if(FINGERPRINT_JSON.perfectcanvas_request.length > 0)
		{
			FINGERPRINT_JSON.additional.server_type_is_perfect_canvas = false;
			FINGERPRINT_JSON.additional.server_type_is_post_data = true;
			FINGERPRINT_JSON.additional.status_url = "https://customcanvas.bablosoft.com/status"
			FINGERPRINT_JSON.additional.api_url = "https://customfingerprints.bablosoft.com/prepare"
			FINGERPRINT_JSON.additional.can_use_simplified_perfect_canvas_request = false;
		}else
		{
			FINGERPRINT_JSON.additional.server_type_is_perfect_canvas = false;
			FINGERPRINT_JSON.additional.server_type_is_post_data = false;
			FINGERPRINT_JSON.additional.status_url = "https://customcanvas.bablosoft.com/status"
			FINGERPRINT_JSON.additional.api_url = "https://customfingerprints.bablosoft.com/prepare"
			FINGERPRINT_JSON.additional.can_use_simplified_perfect_canvas_request = false;
		}
	}else if(FINGERPRINT_JSON.perfectcanvas_request.length > 0)
	{
		if((typeof(FINGERPRINT_JSON.is_main_server_retry) == "boolean" && FINGERPRINT_JSON.is_main_server_retry) || !FINGERPRINT_JSON.additional.enable_precomputed_fingerprints)
		{
			FINGERPRINT_JSON.additional.server_type_is_perfect_canvas = true;
			FINGERPRINT_JSON.additional.server_type_is_post_data = true;
			FINGERPRINT_JSON.additional.status_url = "https://canvas.bablosoft.com:4443/status"
			FINGERPRINT_JSON.additional.api_url = "https://canvas.bablosoft.com:4443/prepare"
			FINGERPRINT_JSON.additional.can_use_simplified_perfect_canvas_request = false;
		}else
		{
			FINGERPRINT_JSON.additional.server_type_is_perfect_canvas = false;
			FINGERPRINT_JSON.additional.server_type_is_post_data = true;
			FINGERPRINT_JSON.additional.status_url = "https://canvas.bablosoft.com:4443/status"
			FINGERPRINT_JSON.additional.api_url = "https://fingerprints.bablosoft.com/prepare"
			q += "&returnpc=true"
			FINGERPRINT_JSON.additional.can_use_simplified_perfect_canvas_request = true;
		}
	}else
	{
		FINGERPRINT_JSON.additional.server_type_is_perfect_canvas = false;
		FINGERPRINT_JSON.additional.server_type_is_post_data = false;
		FINGERPRINT_JSON.additional.status_url = "https://canvas.bablosoft.com:4443/status"
		FINGERPRINT_JSON.additional.api_url = "https://fingerprints.bablosoft.com/prepare"
		q += "&returnpc=true"
		FINGERPRINT_JSON.additional.can_use_simplified_perfect_canvas_request = false;
	}

	FINGERPRINT_JSON.additional.api_url += q

	FINGERPRINT_JSON.perfectcanvas_logs = (typeof(FINGERPRINT_JSON.perfectcanvas_logs) == "string" && FINGERPRINT_JSON.perfectcanvas_logs == "true") || (typeof(FINGERPRINT_JSON.perfectcanvas_logs) == "boolean" && FINGERPRINT_JSON.perfectcanvas_logs)


	_switch_http_client_internal()
	http_client_set_fail_on_error(false)

	_if_else(FINGERPRINT_JSON.additional.server_type_is_perfect_canvas, function(){
		if(FINGERPRINT_JSON.perfectcanvas_logs)
		{
			log("(PerfectCanvas) Start obtaining fingerprint")
		}
		_do(function(){
			http_client_post(FINGERPRINT_JSON.additional.api_url, ["data", FINGERPRINT_JSON.perfectcanvas_request], {"content-type":"custom/" + ("application/octet-stream"), "encoding":("UTF-8"), "method":("POST"),headers:("Accept-Encoding: gzip, deflate")})!

			var json = http_client_content()
	
			try
			{
				var json_parsed = JSON.parse(json)
				if(json_parsed.Status == "success")
				{
					FINGERPRINT_JSON.request_id = json_parsed.Data
					_break()
				}else if(json_parsed.Status == "error")
				{
					log_fail("(PerfectCanvas) " + json_parsed.Message)
					fail(json_parsed.Message)
				}else
				{
					if(FINGERPRINT_JSON.perfectcanvas_logs)
					{
						log("(PerfectCanvas) " + json_parsed.Message)
					}
				}
			}catch(e){}
	
			sleep(15000)!
		})!

		if(FINGERPRINT_JSON.perfectcanvas_logs)
		{
			log("(PerfectCanvas) Request id is " + FINGERPRINT_JSON.request_id)
		}

		sleep(5000)!

		_do(function(){
			http_client_get2(FINGERPRINT_JSON.additional.status_url + "/" + FINGERPRINT_JSON.request_id,{method:("GET"),headers:("Accept-Encoding: gzip, deflate")})!

			var json = http_client_content()
	
			try
			{
				var json_parsed = JSON.parse(json)
				if(json_parsed.Status == "success")
				{
					_set_result(json_parsed.Data)
					_break()
				}else if(json_parsed.Status == "error")
				{
					log_fail("(PerfectCanvas) " + json_parsed.Message)
					fail(json_parsed.Message)
				}else
				{
					if(FINGERPRINT_JSON.perfectcanvas_logs)
					{
						log("(PerfectCanvas) " + json_parsed.Message)
					}
				}
			}catch(e){}
	
			sleep(5000)!
		})!

		if(FINGERPRINT_JSON.perfectcanvas_logs)
		{
			log_success("(PerfectCanvas) Fingerprint has been obtained")
		}

	},function(){
		_do(function(){
			if(_iterator()>15)
				fail("Query limit reached")

			_if_else(FINGERPRINT_JSON.additional.server_type_is_post_data, function(){
				var post_data = FINGERPRINT_JSON.perfectcanvas_request
				if(FINGERPRINT_JSON.additional.can_use_simplified_perfect_canvas_request && !FINGERPRINT_JSON.additional.was_simplified_perfect_canvas_request)
				{
					FINGERPRINT_JSON.additional.was_simplified_perfect_canvas_request = true
					post_data = ScriptWorker.PreparePerfectCanvasRequest(FINGERPRINT_JSON.perfectcanvas_request)
				}
				http_client_post(FINGERPRINT_JSON.additional.api_url, ["data", post_data], {"content-type":"custom/" + ("application/octet-stream"), "encoding":("UTF-8"), "method":("POST"),headers:("Accept-Encoding: gzip, deflate")})!
			}, function(){
				http_client_get2(FINGERPRINT_JSON.additional.api_url,{method:("GET"),headers:("Accept-Encoding: gzip, deflate")})!
			})!

			var json = http_client_content()
			var instantretry = false
	
			try
			{
				var json_parsed = JSON.parse(json)
				if(typeof(json_parsed["instantretry"]) == "boolean")
				{
					instantretry = json_parsed["instantretry"]
				}
				if(!json_parsed["trylater"])
				{
					FINGERPRINT_JSON.additional.final_fingerprint_valid = json_parsed.valid
					_set_result(json)
					_break()
				}
			}catch(e){}

			_if(!instantretry, function(){
				sleep(20000)!
			})!
			
		})!  
	})!


	http_client_set_fail_on_error(true)
	_switch_http_client_main()

	_if(typeof(FINGERPRINT_JSON) == "object" && FINGERPRINT_JSON.additional.is_custom_server && !FINGERPRINT_JSON.additional.server_type_is_perfect_canvas && FINGERPRINT_JSON.additional.server_type_is_post_data && !FINGERPRINT_JSON.additional.final_fingerprint_valid && FINGERPRINT_JSON.additional.dynamic_perfect_canvas, function(){
		FINGERPRINT_JSON.is_custom_server_retry = true
		_call(BrowserAutomationStudio_GetFingerprint,[FINGERPRINT_JSON])!
	})!

	_if(typeof(FINGERPRINT_JSON) == "object" && !FINGERPRINT_JSON.additional.is_custom_server && !FINGERPRINT_JSON.additional.server_type_is_perfect_canvas && FINGERPRINT_JSON.additional.server_type_is_post_data && !FINGERPRINT_JSON.additional.final_fingerprint_valid, function(){
		FINGERPRINT_JSON.is_main_server_retry = true
		_call(BrowserAutomationStudio_GetFingerprint,[FINGERPRINT_JSON])!
	})!

	if(typeof(FINGERPRINT_JSON) == "object")
	{
		delete FINGERPRINT_JSON
	}
}

function BrowserAutomationStudio_ApplyFingerprint()
{
	if(!_is_bas_browser_real())
    {
    	fail(tr("Fingerprint switcher is not integrated with Multilogin yet."))
        return
    }
	if(_arguments().length == 0)
		fail("fingerprint is empty")

	FINGERPRINT_JSON = null
	FINGERPRINT_CANVAS = false
	FINGERPRINT_WEBGL = false
	FINGERPRINT_AUDIO = false
	FINGERPRINT_BATTERY = false
	FINGERPRINT_RECTANGLES = false
	FINGERPRINT_PERFECTCANVAS = true
	FINGERPRINT_SENSOR = false
	FINGERPRINT_FONT_DATA = false
	FINGERPRINT_DEVICE_SCALE = false

	if(typeof(_arguments()) == "object")
	{
		try{
			FINGERPRINT_JSON = JSON.parse(_arguments()[0])
		}catch(e)
		{
			fail(e.message)
		}
		if(_arguments().length > 1 && FINGERPRINT_JSON["canvas"])
			FINGERPRINT_CANVAS = _arguments()[1]
		if(_arguments().length > 2 && FINGERPRINT_JSON["webgl"])
			FINGERPRINT_WEBGL = _arguments()[2]
		if(_arguments().length > 3 && FINGERPRINT_JSON["audio"])
			FINGERPRINT_AUDIO = _arguments()[3]
		if(_arguments().length > 4 && FINGERPRINT_JSON["battery"])
			FINGERPRINT_BATTERY = _arguments()[4]
		if(_arguments().length > 5 && FINGERPRINT_JSON["rectangles"])
			FINGERPRINT_RECTANGLES = _arguments()[5]
		if(_arguments().length > 6 && typeof(_arguments()[6]) == "boolean" && FINGERPRINT_JSON["perfectcanvas"])
			FINGERPRINT_PERFECTCANVAS = _arguments()[6]
		if(_arguments().length > 7 && FINGERPRINT_JSON["sensor"])
			FINGERPRINT_SENSOR = _arguments()[7]
		if(_arguments().length > 8 && FINGERPRINT_JSON["font_data2"])
			FINGERPRINT_FONT_DATA = _arguments()[8]
		if(_arguments().length > 9)
			FINGERPRINT_DEVICE_SCALE = _arguments()[9]
			
	}else
	{
		try{
			FINGERPRINT_JSON = JSON.parse(_arguments())
		}catch(e)
		{
			fail(e.message)
		}
	}

	//Check fingerprint is valid
    if(FINGERPRINT_JSON["valid"] == false)
	{
		var error = "Unknown error"
		if(FINGERPRINT_JSON["message"])
			error = FINGERPRINT_JSON["message"]
		fail("Fingerprint error: \""+ error + "\"") 
	}

	//Save fingerprint to profile
	if(_get_profile().length > 0)
	{
		var value = {
	   		"canvas": FINGERPRINT_CANVAS,
	   		"webgl": FINGERPRINT_WEBGL,
	   		"audio": FINGERPRINT_AUDIO,
	   		"battery": FINGERPRINT_BATTERY,
	   		"rectangles": FINGERPRINT_RECTANGLES,
			"perfectcanvas": FINGERPRINT_PERFECTCANVAS,
			"sensor": FINGERPRINT_SENSOR,
			"font_data": FINGERPRINT_FONT_DATA,
			"device_scale": FINGERPRINT_DEVICE_SCALE
	   	}
		if(typeof(_arguments()) == "object")
		{
			value["fingerprint"] = _arguments()[0]
		}else
		{
			value["fingerprint"] = _arguments()
		}

	   	value = JSON.stringify(value)
	    native("filesystem", "writefile", JSON.stringify({path: _get_profile() + "/fingerprint.json",value: value,base64:false,append:false}))
	}

	_if(typeof(FINGERPRINT_JSON["features"]) == "object", function(){
		var Settings = {}
		var Keys = Object.keys(FINGERPRINT_JSON["features"])
		for(var i = 0;i<Keys.length;i++)
		{
			var Key = Keys[i]
			if(typeof(FINGERPRINT_JSON["features"][Key]) == "boolean")
			{
				Settings["Fingerprints.Feature." + Key] = (FINGERPRINT_JSON["features"][Key]) ? "Enable" : "Disable"
			}
		}
		_settings(Settings)!
	})!

	_if(FINGERPRINT_CANVAS || FINGERPRINT_WEBGL || FINGERPRINT_AUDIO || FINGERPRINT_BATTERY || FINGERPRINT_RECTANGLES || FINGERPRINT_SENSOR, function(){
		var Settings = {}

		if(FINGERPRINT_RECTANGLES)
		{
			Settings["Fingerprints.RectanglesReplace"] = "true"
			Settings["Fingerprints.RectanglesFingerprint"] = FINGERPRINT_JSON["rectangles"]
		}else
		{
			Settings["Fingerprints.RectanglesReplace"] = "false"
			Settings["Fingerprints.RectanglesFingerprint"] = ""
		}

		if(FINGERPRINT_CANVAS)
		{
			Settings["Canvas"] = "noise"
			Settings["CanvasNoise"] = FINGERPRINT_JSON["canvas"]
			Settings["Fingerprints.PerfectCanvasSkipZeroEntropyCode"] = "Enable"
		}

		if(FINGERPRINT_WEBGL)
		{
			Settings["Webgl"] = "noise"
			Settings["WebglNoise"] = FINGERPRINT_JSON["webgl"]
			var Keys = Object.keys(FINGERPRINT_JSON["webgl_properties"])
			for(var i = 0;i<Keys.length;i++)
			{
				var Key = Keys[i]
				var Value = FINGERPRINT_JSON["webgl_properties"][Key]
				if(typeof(Value) == "number")
				{
					Value = Value.toString()
				}
				if(!Value)
					Value = ""
				if(typeof(Value) == "object")
				{
					var ValueKeys = Object.keys(Value)
					for(var j = 0;j<ValueKeys.length;j++)
					{
						var Key2 = ValueKeys[j]
						Settings["Webgl." + Key + "." + Key2] = Value[Key2].toString()
					}
				}else
				{
					Settings["Webgl." + Key] = Value
				}
				
				
			}

			if(typeof(FINGERPRINT_JSON["webgl_properties"]["version2"]) == "undefined")
			{
				Settings["Fingerprints.Webgl2Type"] = "Disable"
			}
		}

		if(FINGERPRINT_SENSOR)
		{
			var Keys = Object.keys(FINGERPRINT_JSON["sensor"])
			for(var i = 0;i<Keys.length;i++)
			{
				var Key = Keys[i]
				var Value = FINGERPRINT_JSON["sensor"][Key]
				Settings["Fingerprints." + Key] = Value.toString()
			}
		}

		if(FINGERPRINT_FONT_DATA)
		{
			var FontDataRootPath = native("fontpack", "getfontpackpath", "")
			if(FontDataRootPath.length > 0)
			{
				var Keys = Object.keys(FINGERPRINT_JSON["font_data2"])
				var FontPathList = []
				for(var i = 0;i<Keys.length;i++)
				{
					var Key = Keys[i]

					var Values = FINGERPRINT_JSON["font_data2"][Key]
					if(Values.length > 0)
					{
						Values = Values.split(",")
						var MaxSize = 0
						var MaxPath = ""
						for(var j = 0;j<Values.length;j++)
						{
							var Value = Values[j]
							var Path = FontDataRootPath + Value + ".ttf"
							
							if(Values.length > 1)
							{
								var FileInfo = JSON.parse(native("filesystem", "fileinfo", Path))
								var Size = FileInfo["size"]
								var Exists = FileInfo["exists"]
								
								if(Exists && Size > MaxSize)
								{
									MaxPath = Path
									MaxSize = Size
								}
							}else
							{
								MaxPath = Path
							}
							
						}

						if(MaxPath.length > 0 && FontPathList.indexOf(MaxPath) < 0)
						{
							FontPathList.push(MaxPath)
						}
					}
				}
				Settings["Fingerprints.FontFilesList"] = base64_encode(JSON.stringify(FontPathList))
			}
		}

		if(FINGERPRINT_BATTERY)
		{
			Settings["Fingerprints.BatteryEnabled"] = "true"
			Settings["Fingerprints.HasBatteryAPI"] = (FINGERPRINT_JSON["has_battery_api"] == true) ? "true" : "false"
			Settings["Fingerprints.HasBatteryDevice"] = (FINGERPRINT_JSON["has_battery_device"] == true) ? "true" : "false"
			Settings["Fingerprints.BatteryFingerprint"] = FINGERPRINT_JSON["battery"]
		}else
		{
			Settings["Fingerprints.BatteryEnabled"] = "false"
		}

		if(FINGERPRINT_AUDIO)
		{
			Settings["Audio"] = "noise"
			Settings["AudioNoise"] = FINGERPRINT_JSON["audio"]
			if(FINGERPRINT_JSON["audio_properties"])
			{
				var Keys = Object.keys(FINGERPRINT_JSON["audio_properties"])
				for(var i = 0;i<Keys.length;i++)
				{
					var Key = Keys[i]
					var Value = FINGERPRINT_JSON["audio_properties"][Key]
					try
					{
						Settings["Fingerprints.Audio." + Key] = Value.toString()
					}catch(e){FINGERPRINT_USERAGENT = ""}
					if(Value)
					{
						if(Key == "BaseAudioContextSampleRate")
							Key = "sampleRate"
						else if(Key == "AudioDestinationNodeMaxChannelCount")
							Key = "maxChannelCount"
						else if(Key == "AudioContextBaseLatency")
							Key = "baseLatency"
						else if(Key == "AudioContextOutputLatency")
							Key = "outputLatency"
						else
							Key = ""
						if(Key.length > 0)
							Settings["Attribute." + Key] = Value.toString()
					}
				}
			}
		}
		_settings(Settings)!
	})!

	//Accept language pattern
	_if(FINGERPRINT_JSON["lang"], function(){
		_set_accept_language_pattern(FINGERPRINT_JSON["lang"])!
	})!
	
	//User agent
	FINGEPRINT_SETTINGS = {}

	if(typeof(FINGERPRINT_JSON["ChromeApp"]) == "string")
	{
		FINGEPRINT_SETTINGS["Fingerprints.ChromeApp"] = FINGERPRINT_JSON["ChromeApp"]
	}

	if(typeof(FINGERPRINT_JSON["ChromeRuntime"]) == "string")
	{
		FINGEPRINT_SETTINGS["Fingerprints.ChromeRuntime"] = FINGERPRINT_JSON["ChromeRuntime"]
	}

	_if(typeof(FINGERPRINT_JSON["useragentdata"]) == "string", function(){
		_if_else(FINGERPRINT_JSON["useragentdata"].length == 0, function(){
			_set_user_agent_data("eyJicmFuZHMiOltdLCJtb2JpbGUiOmZhbHNlLCJmdWxsVmVyc2lvbiI6IiIsInBsYXRmb3JtIjoiIiwicGxhdGZvcm1WZXJzaW9uIjoiIiwiYXJjaGl0ZWN0dXJlIjoiIiwibW9kZWwiOiIifQ==")!
		}, function(){
			_set_user_agent_data(FINGERPRINT_JSON["useragentdata"])!
		})!
	})!


	try
	{
		FINGERPRINT_USERAGENT = FINGERPRINT_JSON["ua"] || ""
	}catch(e){FINGERPRINT_USERAGENT = ""}

	_if(FINGERPRINT_USERAGENT.length > 0, function(){
		header("User-Agent", FINGERPRINT_JSON["ua"])!
	})!
	

	// Screen size
	try
	{
		FINGERPRINT_WIDTH = parseInt(FINGERPRINT_JSON["width"])
		FINGERPRINT_HEIGHT = parseInt(FINGERPRINT_JSON["height"])
	}catch(e){FINGERPRINT_HEIGHT = -1;FINGERPRINT_WIDTH = -1}


	_if(FINGERPRINT_DEVICE_SCALE, function(){
		FINGERPRINT_DEVICE_SCALE_FACTOR = 1.0
		try
		{
			FINGERPRINT_DEVICE_SCALE_FACTOR = parseFloat(FINGERPRINT_JSON.attr["window.devicePixelRatio"])
		}catch(e){FINGERPRINT_DEVICE_SCALE_FACTOR = 1.0}
	
		_if(FINGERPRINT_DEVICE_SCALE_FACTOR >= 1.01 || FINGERPRINT_DEVICE_SCALE_FACTOR <= 0.99, function(){
			_set_device_scale_factor_no_resize(FINGERPRINT_DEVICE_SCALE_FACTOR)!
		})!
	})!

	_if(FINGERPRINT_WIDTH > 0, function(){
		resize(FINGERPRINT_WIDTH, FINGERPRINT_HEIGHT)!
	})!

	
	_if(FINGERPRINT_JSON["dnt"], function(){
		header("DNT","1")!
	})!

	//Native code
	if(FINGERPRINT_JSON["native_code"])
	{
		var split = FINGERPRINT_JSON["native_code"].split("Object")
		FINGEPRINT_SETTINGS["Fingerprints.NativeCodeAfter"] = base64_encode(split[split.length - 1])
		FINGEPRINT_SETTINGS["Fingerprints.NativeCodeBefore"] = base64_encode(split[0])
	}

	if(FINGERPRINT_JSON["plugins"] && FINGERPRINT_JSON["mimes"])
	{
		FINGEPRINT_SETTINGS["Fingerprints.Plugins"] = base64_encode(JSON.stringify(FINGERPRINT_JSON["plugins"]))
		FINGEPRINT_SETTINGS["Fingerprints.Mimes"] = base64_encode(JSON.stringify(FINGERPRINT_JSON["mimes"]))
	}

	_if(FINGERPRINT_JSON["attr"], function(){
		var Keys = Object.keys(FINGERPRINT_JSON["attr"])
		for(var i = 0;i<Keys.length;i++)
		{
			var Key = Keys[i]
			var Split = Key.split(".")
			var KeySettings = "Attribute." + Split[Split.length - 1]
			try
			{
				FINGEPRINT_SETTINGS[KeySettings] = FINGERPRINT_JSON["attr"][Key].toString()
			}catch(e)
			{
				FINGEPRINT_SETTINGS[KeySettings] = ""
			}
		}
	})!

	var BrowserMode = "desktop"
	try
	{
		if(parseInt(FINGERPRINT_JSON["attr"]["maxTouchPoints"]) > 0)
		{
			BrowserMode = "mobile"
		}
	}catch(e)
	{

	}

	_browser_mode(BrowserMode)!
	

	if(FINGERPRINT_JSON["connection"])
	{
		var NetInfoDownlinkMax = false
		var Keys = Object.keys(FINGERPRINT_JSON["connection"])
		var HasConnection = Keys.length > 0

		for(var i = 0;i<Keys.length;i++)
		{
			var Key = Keys[i]

			if(Key == "downlinkMax")
			{
				NetInfoDownlinkMax = true;
			}

			var KeySettings = "Attribute.Connection." + Key
			try
			{
				if(typeof(FINGERPRINT_JSON["connection"][Key]) == "boolean")
				{
					FINGEPRINT_SETTINGS[KeySettings] = (FINGERPRINT_JSON["connection"][Key]) ? "1" : "0";
				}else if(typeof(FINGERPRINT_JSON["connection"][Key]) == "object" && FINGERPRINT_JSON["connection"][Key] == null)
				{
					if(Key == "downlinkMax")
					{
						FINGEPRINT_SETTINGS[KeySettings] = "INFINITY";
					}else
					{
					FINGEPRINT_SETTINGS[KeySettings] = "0";
					}
				}else
				{
					FINGEPRINT_SETTINGS[KeySettings] = FINGERPRINT_JSON["connection"][Key].toString()
				}
			}catch(e)
			{
				FINGEPRINT_SETTINGS[KeySettings] = ""
			}
		}
		if(!HasConnection)
		{
			FINGEPRINT_SETTINGS["Fingerprints.Feature.FingerprintsConnection"] = "Disable"
		}

		if(NetInfoDownlinkMax)
		{
			FINGEPRINT_SETTINGS["Fingerprints.Feature.NetInfoDownlinkMax"] = "Enable"
		}
	}

	if(FINGERPRINT_JSON["orientation"])
	{
		var Keys = Object.keys(FINGERPRINT_JSON["orientation"])

		for(var i = 0;i<Keys.length;i++)
		{
			var Key = Keys[i]

			var KeySettings = "Attribute.Orientation." + Key
			try
			{
				FINGEPRINT_SETTINGS[KeySettings] = FINGERPRINT_JSON["orientation"][Key].toString()
			}catch(e)
			{
				FINGEPRINT_SETTINGS[KeySettings] = ""
			}
		}
	}

	if(typeof(FINGERPRINT_JSON["doNotTrack"]) == "string")
	{
		try
		{
			FINGEPRINT_SETTINGS["Attribute.doNotTrack"] = FINGERPRINT_JSON["doNotTrack"]
		}catch(e)
		{
			
		}
	}

	if(FINGERPRINT_JSON["css"])
	{
		var Keys = Object.keys(FINGERPRINT_JSON["css"])
		for(var i = 0;i<Keys.length;i++)
		{
			var Key = "css-" + Keys[i]
			Key = Key.split("-").map(function(word){
				if(!word)
					return word;
				return word.charAt(0).toUpperCase() + word.slice(1)
			}).join("")
			FINGEPRINT_SETTINGS["Fingerprints." + Key] = FINGERPRINT_JSON["css"][Keys[i]].toString()
		}
	}

	if(typeof(FINGERPRINT_JSON["keyboard"]) == "object" && typeof(FINGERPRINT_JSON["keyboard"].length) == "number")
	{
		FINGEPRINT_SETTINGS["Fingerprints.KeyboardLayout"] = base64_encode(JSON.stringify(FINGERPRINT_JSON["keyboard"]))
	}

	if(typeof(FINGERPRINT_JSON["media"]) == "object" && typeof(FINGERPRINT_JSON["media"]["devices"]) == "object" && typeof(FINGERPRINT_JSON["media"]["constraints"]) == "object")
	{
		try
		{
			var AllDevices = []
			for(var i = 0;i<FINGERPRINT_JSON["media"]["devices"].length;i++)
			{
				var Device = FINGERPRINT_JSON["media"]["devices"][i]
				if(typeof(Device) == "object" && typeof(Device.deviceId) == "string" && typeof(Device.kind) == "string" && typeof(Device.label) == "string" && typeof(Device.groupId) == "string")
				{
					AllDevices.push(Device.deviceId)
					AllDevices.push(Device.kind)
					AllDevices.push(Device.label)
					AllDevices.push(Device.groupId)
				}
			}
			FINGEPRINT_SETTINGS["Fingerprints.MediaDevices"] = base64_encode(JSON.stringify(AllDevices))

		}catch(e)
		{
		}

		try
		{
			var AllConstrains = Object.keys(FINGERPRINT_JSON["media"]["constraints"])
			var ActualConstrains = []
			for(var i = 0;i<AllConstrains.length;i++)
			{
				var Constrain = AllConstrains[i]
				if(typeof(FINGERPRINT_JSON["media"]["constraints"][Constrain]) == "boolean" && FINGERPRINT_JSON["media"]["constraints"][Constrain] === true)
				{
					ActualConstrains.push(Constrain)
				}
			}
			FINGEPRINT_SETTINGS["Fingerprints.MediaConstrains"] = base64_encode(JSON.stringify(ActualConstrains))
		}catch(e)
		{
		}
	}

	{
		var PerfectCanvasReplaceType = "Disable"
		if(FINGERPRINT_JSON["perfectcanvas"] && FINGERPRINT_PERFECTCANVAS)
		{
			
			var Keys = Object.keys(FINGERPRINT_JSON["perfectcanvas"])
			for(var i = 0;i<Keys.length;i++)
			{
				var Key = Keys[i]
				
				var Value = FINGERPRINT_JSON["perfectcanvas"][Key]
				FINGEPRINT_SETTINGS["Fingerprints.PerfectCanvasReplace." + Key] = Value
			}
			if(Keys.length > 0)
			{
				PerfectCanvasReplaceType = "Enable"
			}
		}

		FINGEPRINT_SETTINGS["Fingerprints.PerfectCanvasCapture"] = PerfectCanvasReplaceType
		FINGEPRINT_SETTINGS["Fingerprints.PerfectCanvasDoReplace"] = PerfectCanvasReplaceType
	}

	FINGEPRINT_SETTINGS["Fingerprints.FontList"] = FINGERPRINT_JSON["fonts"].join(";")
	
	try
	{
		FINGEPRINT_SETTINGS["Fingerprints.Tags"] = FINGERPRINT_JSON["tags"].join(",")
	}catch(e)
	{

	}

	try
	{
		if(FINGERPRINT_JSON["tags"].indexOf("Desktop") < 0)
		{
			FINGEPRINT_SETTINGS["Fingerprints.Extensions"] = "Disable"
		}
	}catch(e)
	{

	}

	try
	{
		if(FINGERPRINT_JSON["systemcolors"])
			FINGEPRINT_SETTINGS["Fingerprints.SystemColors"] = base64_encode(JSON.stringify(FINGERPRINT_JSON["systemcolors"]))
		else
			FINGEPRINT_SETTINGS["Fingerprints.SystemColors"] = base64_encode("{}")
	}catch(e)
	{
		FINGEPRINT_SETTINGS["Fingerprints.SystemColors"] = base64_encode("{}")
	}

	try
	{
		if(FINGERPRINT_JSON["systemfonts"])
			FINGEPRINT_SETTINGS["Fingerprints.SystemFonts"] = base64_encode(JSON.stringify(FINGERPRINT_JSON["systemfonts"]))
		else
			FINGEPRINT_SETTINGS["Fingerprints.SystemFonts"] = base64_encode("{}")
	}catch(e)
	{
		FINGEPRINT_SETTINGS["Fingerprints.SystemFonts"] = base64_encode("{}")
	}

	try
	{
		if(FINGERPRINT_JSON["speech"])
			FINGEPRINT_SETTINGS["Fingerprints.Speech"] = base64_encode(JSON.stringify(FINGERPRINT_JSON["speech"]))
		else
			FINGEPRINT_SETTINGS["Fingerprints.Speech"] = base64_encode("[]")
	}catch(e)
	{
		FINGEPRINT_SETTINGS["Fingerprints.Speech"] = base64_encode("[]")
	}

	try
	{
		if(typeof(FINGERPRINT_JSON["codecs"]) == "object" && FINGERPRINT_JSON["codecs"].length > 0)
		{
			var TheoraData = null
			for(var i = 0;i < FINGERPRINT_JSON["codecs"].length;i++)
			{
				var DataCandidate = FINGERPRINT_JSON["codecs"][i]
				if(DataCandidate.contentType == 'video/ogg; codecs="theora"')
				{
					TheoraData = DataCandidate
					break;
				}
			}
			if(typeof(TheoraData) == "object" && TheoraData != null && !TheoraData.supported)
			{
				FINGEPRINT_SETTINGS["Fingerprints.IsTheoraEnabled"] = "Disable"
			}
		}
			
	}catch(e)
	{
		
	}

	try
	{
		if(typeof(FINGERPRINT_JSON["bluetooth"]) == "boolean")
		{
			if(FINGERPRINT_JSON["bluetooth"])
			{
				FINGEPRINT_SETTINGS["Fingerprints.Bluetooth"] = "Enable"
			}else
			{
				FINGEPRINT_SETTINGS["Fingerprints.Bluetooth"] = "Disable"
			}
		}

	}catch(e)
	{
		
	}

	try
	{
		if(FINGERPRINT_JSON["heap"])
		{
			FINGEPRINT_SETTINGS["Fingerprints.Feature.FingerprintsMemory"] = "Enable"
			FINGEPRINT_SETTINGS["Fingerprints.Heap"] = FINGERPRINT_JSON["heap"]
		}
		else
			{
			FINGEPRINT_SETTINGS["Fingerprints.Feature.FingerprintsMemory"] = "Disable"
		}

	}catch(e)
	{
		FINGEPRINT_SETTINGS["Fingerprints.Feature.FingerprintsMemory"] = "Disable"
	}
		

	try
	{
		if(typeof(FINGERPRINT_JSON["storage"]) == "string")
		{
			FINGEPRINT_SETTINGS["Fingerprints.Storage"] = FINGERPRINT_JSON["storage"]
		}
	}catch(e)
		{

	}

	try
	{
		if(typeof(FINGERPRINT_JSON["hls"]) == "boolean")
		{
			if(FINGERPRINT_JSON["hls"])
			{
				FINGEPRINT_SETTINGS["Fingerprints.IsHLSEnabled"] = "Enable"
			}else
			{
				FINGEPRINT_SETTINGS["Fingerprints.IsHLSEnabled"] = "Disable"
			}
		}

	}catch(e)
	{
		
	}

	_settings(FINGEPRINT_SETTINGS)!

	
	_if(ScriptWorker.GetIsRecord(), function(){
		url()!
		_if(_result() == "about:blank", function(){
			_load("data:text/plain,", "", false)!
		})!
	})!

}


