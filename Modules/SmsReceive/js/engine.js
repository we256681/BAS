function _BAS_GETSMSSITECODE(service,site)
{
	if(service == "sms-reg.com")
	{
		if(site == "4game")
			return "4game"
		if(site == "GMail")
			return "gmail"
		if(site == "Facebook")
			return "facebook"
		if(site == "Mail.RU")
			return "mailru"
		if(site == "VK")
			return "vk"
		if(site == "OD")
			return "classmates"
		if(site == "Twitter")
			return "twitter"
		if(site == "mamba")
			return "mamba"
		if(site == "Uber")
			return "uber"
		if(site == "Telegram")
			return "telegram"
		if(site == "Badoo")
			return "badoo"
		if(site == "Drug Vokrug")
			return "drugvokrug"
		if(site == "Avito")
			return "avito"
		if(site == "OLX")
			return "olx"
		if(site == "Steam")
			return "steam"
		if(site == "Fotostrana")
			return "fotostrana"
		if(site == "HotMail")
			return "microsoft"
		if(site == "Viber")
			return "viber"
		if(site == "WhatsAPP")
			return "whatsapp"
		if(site == "WeChat")
			return "wechat"
		if(site == "SEOsprint.net")
			return "seosprint"
		if(site == "Instagram")
			return "instagram"
		if(site == "Yahoo")
			return "yahoo"

		return "other"
	}

	if(service == "sms-activate.ru")
	{
		if(site == "VK")
			return "vk"
		if(site == "OD")
			return "ok"
		if(site == "WhatsAPP")
			return "wa"
		if(site == "Viber")
			return "vi"
		if(site == "Telegram")
			return "tg"
		if(site == "WeChat")
			return "wb"
		if(site == "GMail")
			return "go"
		if(site == "Avito")
			return "av"
		if(site == "Facebook")
			return "fb"
		if(site == "Twitter")
			return "tw"
		if(site == "Uber")
			return "ub"
		if(site == "Qiwi")
			return "qw"
		if(site == "GetTaxi")
			return "gt"
		if(site == "OLX")
			return "sn"
		if(site == "Instagram")
			return "ig"
		if(site == "SEOsprint.net")
			return "ss"
		if(site == "Yandex")
			return "ya"
		if(site == "Mail.RU")
			return "ma"
		if(site == "HotMail")
			return "mm"
		if(site == "Yahoo")
			return "mb"
		if(site == "AOL")
			return "we"
		if(site == "Open I Messenger")
			return "oi"

		return "ot"
	}

	if(service == "getsms.online")
	{
		if(site == "VK")
			return "vk"
		if(site == "OD")
			return "ok"
		if(site == "WhatsAPP")
			return "wa"
		if(site == "Viber")
			return "vi"
		if(site == "Telegram")
			return "tg"
		if(site == "WeChat")
			return "wb"
		if(site == "GMail")
			return "go"
		if(site == "Avito")
			return "av"
		if(site == "Facebook")
			return "fb"
		if(site == "Twitter")
			return "tw"
		if(site == "Uber")
			return "ub"
		if(site == "Instagram")
			return "ig"
		if(site == "Yandex")
			return "ya"
		if(site == "Mail.RU")
			return "ma"
		if(site == "HotMail")
			return "mm"
		if(site == "Yahoo")
			return "mb"
		if(site == "AOL")
			return "we"
		return "ot"
	}

	if(service == "smspva.com")
	{
		var services = {
		"GMail" : "opt1",
		"Facebook" : "opt2",
		"VK" : "opt4",
		"OD" : "opt5",
		"Fiverr" : "opt6",
		"mamba" : "opt100",
		"LinkedIn" : "opt8",
		"Tinder" : "opt9",
		"AOL" : "opt10",
		"Viber" : "opt11",
		"Fotostrana" : "opt13",
		"HotMail" : "opt15",
		"Instagram" : "opt16",
		"MeetMe" : "opt17",
		"Qiwi" : "opt18",
		"*" : "opt19",
		"WhatsAPP" : "opt20",
		"SEOsprint.net" : "opt22",
		"Yandex" : "opt23",
		"WebMoney" : "opt24",
		"dodopizza.ru" : "opt27",
		"Telegram" : "opt29",
		"Drug Vokrug" : "opt31",
		"Drom.RU" : "opt32",
		"Mail.RU" : "opt33",
		"GetTaxi" : "opt35",
		"Auto.RU" : "opt38",
		"Twitter" : "opt41",
		"Sipnet.ru" : "opt51",
		"Ubank.ru" : "opt52",
		"Weebly" : "opt54",
		"Badoo" : "opt56",
		"Steam" : "opt58",
		"Avito" : "opt59",
		"Yahoo" : "opt65",
		"Twilio" : "opt66",
		"WeChat" : "opt67",
		"Naver" : "opt73",
		"Taxi Maksim" : "opt74",
		"Discord" : "opt45",
		"Ebay": "opt83",
		"G2A.COM" : "opt68",
		"Proton Mail" : "opt57",
		"Snapchat" : "opt90",
		"Spotify" : "opt98",
		"Uber" : "opt72",
		"Zoho" : "opt93"
	}




		if(services[site])
		{
			return services[site]
		}else
		{
			return "opt19"
		}

	}

	return "other"
}



function _BAS_GETSMSCOUTRYCODE(service,country)
{
	if(service == "sms-activate.ru")
	{
		if(country == "RU")
			return "0";
		if(country == "KZ")
			return "2";
		if(country == "UA")
			return "1";
		if(country == "ID")
			return "6";
		if(country == "IL")
			return "13";
		if(country == "KG")
			return "11";
		if(country == "PH")
			return "4";
		if(country == "PL")
			return "15";
		if(country == "VN")
			return "10";
		if(country == "CN")
			return "3";
		if(country == "US")
			return "12";
		if(country == "HK")
			return "14";
		return "0"
	}

	if(service == "getsms.online")
	{
		if(country == "RU")
			return "ru";
		if(country == "KZ")
			return "kz";
		if(country == "UA")
			return "ua";
		if(country == "BY")
			return "by";
		if(country == "PL")
			return "pl";
		if(country == "UK")
			return "uk";
		if(country == "DE")
			return "de";

		return "or"
	}

	if(service == "smspva.com")
	{
		if(country == "CN")
			return "CN2";
		if(country == "US")
			return "RU";
		if(country == "HK")
			return "RU";
		if(country == "BY")
			return "RU";
		if(country == "DE")
			return "RU";
		return country
	}

	if(service == "sms-reg.com")
	{
		if(country == "RU")
			return "ru";
		if(country == "KZ")
			return "kz";
		if(country == "UA")
			return "ua";
		return "all"
	}


}

function _BAS_PARSEJSONFROMHTTPCLIENT(message)
{
	var json;
	try{
		json = JSON.parse(http_client_content())
	}catch(e)
	{
		fail(message)
	}
	return json
}

_BAS_SMSCONFIRMDATA = {}
_SMS_BAN_THREAD = 0
_SMS_DEBUG = false

function _sms_ban_thread(seconds)
{
	_SMS_BAN_THREAD = Date.now() + seconds * 1000
}

function _sms_ban_service(seconds)
{
	PSet("sms", "_SMS_BAN_THREAD", (Date.now() + seconds * 1000).toString())
}

function _sms_before_request()
{
	_do(function(){
		_SMS_BAN_THREAD_SLEEP = false

		_if(_SMS_BAN_THREAD > 0 && _SMS_BAN_THREAD - Date.now() > 0, function(){
			if(_SMS_DEBUG)
				log("Wait before api request " + (_SMS_BAN_THREAD - Date.now()).toString())
			sleep(_SMS_BAN_THREAD - Date.now())!
			_SMS_BAN_THREAD_SLEEP = true
		})!

		_if(P("sms", "_SMS_BAN_THREAD").length > 0 && parseInt(P("sms", "_SMS_BAN_THREAD")) - Date.now() > 0, function(){
			var sleep_time = parseInt(P("sms", "_SMS_BAN_THREAD")) - Date.now() + rand(0,30) * 1000
			if(_SMS_DEBUG)
				log("Wait before api request " + sleep_time)
			sleep(sleep_time)!
			_SMS_BAN_THREAD_SLEEP = true
		})!
		if(!_SMS_BAN_THREAD_SLEEP)
			_break();

	})!

}


function _BAS_SMSREGAPIREQUEST()
{
	var url = _arguments()["url"]
	var method = _arguments()["method"]
	var number = _arguments()["number"]
	var dontcheckresp = _arguments()["dontcheckresp"]

	if(!url)
	{
		_SMS_CONFIRM_DATA = _BAS_SMSCONFIRMDATA[number]
		url = "http://api.sms-reg.com/" + method + ".php?tzid=" + _SMS_CONFIRM_DATA["id"] + "&apikey=" + _SMS_CONFIRM_DATA["api"]
	}

	_call(_sms_before_request, null)!

	_switch_http_client_internal()

	if(_SMS_DEBUG)
		log("smsreg api request " + url)

	_ensure_http_client()
	_BAS_FAIL_ON_ERROR = FAIL_ON_ERROR
	http_client_set_fail_on_error(false)
	_do(function(){
		if(_iterator() > 10)
		{
			_switch_http_client_main()
			FAIL_ON_ERROR = _BAS_FAIL_ON_ERROR
			fail("Failed sms api request")
		}
		http_client_get2(url,{method:("GET")})!
		if(!http_client_was_error())
			_break()

		sleep(1000)!
	})!
	FAIL_ON_ERROR = _BAS_FAIL_ON_ERROR


	_sms_ban_thread(5)

	if(_SMS_DEBUG)
		log("smsreg api responce " + http_client_content())

	var url = _arguments()["url"]
	var method = _arguments()["method"]
	var number = _arguments()["number"]
	var dontcheckresp = _arguments()["dontcheckresp"]

	var json = _BAS_PARSEJSONFROMHTTPCLIENT("Failed to parse sms-reg " + method + " responce");

	_switch_http_client_main()


	if(!dontcheckresp)
	{
		if(json["response"].toString() !== "1")
		{
			var msg = json["error_msg"]
			if(!msg)
				msg = json["response"].toString()
			fail("Sms Reg Error: during " + method + " " + msg)
		}
	}

	_set_result(json)
}

function _BAS_SMSACTIVATEPIREQUEST()
{
	var domain = "sms-activate.ru"
	var arg = _arguments()

	if("domain" in arg)
	{
		domain = arg["domain"]
		if(domain == "getsms.online")
			domain = "smsactivateapi.getsms.online"
		delete arg["domain"]
	}

	var url = "http://" + domain + "/stubs/handler_api.php?" + Object.keys(arg).map(function(e){return e + "=" + encodeURIComponent(arg[e])}).join("&")

	_call(_sms_before_request, null)!

	_switch_http_client_internal()

	if(_SMS_DEBUG)
		log("sms-activate api request " + url)

	_ensure_http_client()
	_BAS_FAIL_ON_ERROR = FAIL_ON_ERROR
	http_client_set_fail_on_error(false)
	_do(function(){
		if(_iterator() > 10)
		{
			_switch_http_client_main()
			FAIL_ON_ERROR = _BAS_FAIL_ON_ERROR
			fail("Failed sms api request")
		}
		http_client_get2(url,{method:("GET")})!
		if(!http_client_was_error())
			_break()

		sleep(1000)!
	})!
	FAIL_ON_ERROR = _BAS_FAIL_ON_ERROR

	_sms_ban_thread(5)

	if(_SMS_DEBUG)
		log("sms-activate api responce " + http_client_content())

	var res = http_client_content().split(":")

	_switch_http_client_main()

	_set_result(res)
}

function _BAS_SMSPVAREQUEST()
{
	var url = "http://smspva.com/priemnik.php?" + Object.keys(_arguments()).map(function(e){return e + "=" + encodeURIComponent(_arguments()[e])}).join("&")

	_call(_sms_before_request, null)!

	_switch_http_client_internal()

	if(_SMS_DEBUG)
		log("smspva api request " + url)

	_ensure_http_client()
	_BAS_FAIL_ON_ERROR = FAIL_ON_ERROR
	http_client_set_fail_on_error(false)
	_do(function(){
		if(_iterator() > 10)
		{
			_switch_http_client_main()
			FAIL_ON_ERROR = _BAS_FAIL_ON_ERROR
			fail("Failed sms api request")
		}
		http_client_get2(url,{method:("GET")})!
		if(!http_client_was_error())
			_break()

		sleep(1000)!
	})!
	FAIL_ON_ERROR = _BAS_FAIL_ON_ERROR

	_sms_ban_thread(20)

	if(_SMS_DEBUG)
		log("smspva api responce " + http_client_content())

	var res = _BAS_PARSEJSONFROMHTTPCLIENT("Failed to parse smspva " + _arguments()["method"] + " responce")

	if(res["response"] == "5")
		_sms_ban_service(60)

	if(res["response"] == "6")
		_sms_ban_service(600)

	_switch_http_client_main()

	_set_result(res)
}
