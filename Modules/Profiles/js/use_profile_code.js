ProfilePath = (<%= profile %>);

_do(function(){
	if(ProfilePath == _get_profile())
		_break();
		
	if(_iterator() > 30)
		fail(tr("Timeout during switching to profile ") + ProfilePath);
	
	native("filesystem", "removefile", ProfilePath + "/lockfile");

	if(!JSON.parse(native("filesystem", "fileinfo", ProfilePath + "/lockfile"))["exists"])
		_break();

	sleep(1000)!
})!


_if(<%= load_proxy %> == "true", function(){
	var is_error = false;
	try
	{
		_ARG = JSON.parse(native("filesystem", "readfile", JSON.stringify({value: (<%= profile %>) + "/proxy.txt",base64:false,from:0,to:0})))
		_ARG = proxy_pack(_ARG)
	}catch(e)
	{
		is_error = true
	}
	_if(!is_error, function(){
		_set_proxy_for_next_profile(_ARG)!
	})!
})!

var Params = {};
Params["ProfilePath"] = ProfilePath
Params["LoadFingerprintFromProfileFolder"] = (<%= load_fp %>)
_settings(Params)!

_if(<%= load_proxy %> == "true", function(){
	var is_error = false;
	try
	{
		_ARG = JSON.parse(native("filesystem", "readfile", JSON.stringify({value: (<%= profile %>) + "/proxy.txt",base64:false,from:0,to:0})))
		_ARG["Port"] = parseInt(_ARG["Port"])
	}catch(e)
	{
		is_error = true
	}
	_if(!is_error, function(){
		set_proxy(_ARG["server"], _ARG["Port"], _ARG["IsHttp"], _ARG["name"], _ARG["password"])!
		sleep(1000)!
		set_proxy_extended(true, true, true, true, true)!
		sleep(1000)!
	})!
})!

_if(<%= load_fp %> == "true", function(){

	FINGERPRINT_JSON = native("filesystem", "readfile", JSON.stringify({value: (<%= profile %>) + "/fingerprint.json",base64:false,from:0,to:0}))
	_if(FINGERPRINT_JSON.length > 0, function(){
		FINGERPRINT_JSON = JSON.parse(FINGERPRINT_JSON)
		_call(BrowserAutomationStudio_ApplyFingerprint,[FINGERPRINT_JSON["fingerprint"],FINGERPRINT_JSON["canvas"],FINGERPRINT_JSON["webgl"],FINGERPRINT_JSON["audio"],FINGERPRINT_JSON["battery"],FINGERPRINT_JSON["rectangles"],FINGERPRINT_JSON["perfectcanvas"],FINGERPRINT_JSON["sensor"],FINGERPRINT_JSON["font_data"],FINGERPRINT_JSON["device_scale"]])!
		sleep(1000)!
	})!

	FINGERPRINT_JSON = native("filesystem", "readfile", JSON.stringify({value: (<%= profile %>) + "/performance.json",base64:false,from:0,to:0}))
	_if(FINGERPRINT_JSON.length > 0, function(){
		FINGERPRINT_JSON = JSON.parse(FINGERPRINT_JSON)
		_call(BrowserAutomationStudio_PerformanceFingerprint,FINGERPRINT_JSON)!
	})!
})!

