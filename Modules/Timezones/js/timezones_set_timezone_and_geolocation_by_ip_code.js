_call(_get_ip_info, [<%= value %>, <%= ip_info_method %>, <%= ip_api_key %>, <%= function_name %>])!
IP_INFO = _result()
if(!IP_INFO["valid"])
	fail("Failed to get ip info for " + (<%= value %>));

_if((<%= change_geolocation %>) == "true", function(){
	geolocation(IP_INFO["latitude"],IP_INFO["longitude"])!
})!

_if((<%= change_timezone %>) == "true", function(){
	_settings({"Timezone":(-IP_INFO["offset"]).toString(),"TimezoneName":IP_INFO["timezone"]})!
})!

_if((<%= change_webrtc %>) == "true", function(){
	_settings({"Webrtc":"replace","WebrtcIps":(<%= value %>)})!
})!

_if((<%= change_browserlanguage %>) == "true", function(){
	var country = IP_INFO["country"].toUpperCase()
	var language = native("timezones", "country_to_language", country)
	header("Accept-Language", language + "-" + country)!
	_settings({"Fingerprints.Locale":IP_INFO["country"].toLowerCase()})!
})!
sleep(1000)!