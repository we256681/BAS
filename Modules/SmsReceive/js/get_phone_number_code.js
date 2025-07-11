_if(<%= service %> == "sms-reg.com", function(){
	var country = _BAS_GETSMSCOUTRYCODE(<%= service %>, <%= country %>);
	if(country.length == 0)
		country = "all"

	var url = "http://api.sms-reg.com/getNum.php?service=" + _BAS_GETSMSSITECODE(<%= service %>, <%= site %>) + "&apikey=" + (<%= apikey %>) + "&country=" + country + "&appid=RUBMC9BX6OIRJG3S"
	_call(_BAS_SMSREGAPIREQUEST,{method: "getNum", url: url})!
    var json = _result()
    _SMS_CONFIRM_DATA = {id: json["tzid"], api: (<%= apikey %>), method: "sms-reg.com", originalid: json["tzid"]}

    _SMS_MAX_WAIT = Date.now() + 600000
	_do(function(){
        if(Date.now() > _SMS_MAX_WAIT)
			fail("Sms Reg Error: Timeout for getState");

        var url = "http://api.sms-reg.com/getState.php?tzid=" + _SMS_CONFIRM_DATA["id"] + "&apikey=" + _SMS_CONFIRM_DATA["api"]
        _call(_BAS_SMSREGAPIREQUEST,{method: "getState", url: url, dontcheckresp: true})!
        var json = _result()


        if(json["response"] === "TZ_NUM_PREPARE")
        {
            _SMS_CONFIRM_DATA["number"] = json["number"]
        	_BAS_SMSCONFIRMDATA[json["number"]] = _SMS_CONFIRM_DATA
            _break()
        }

        if(json["response"] !== "TZ_INPOOL")
        {
            fail("Sms Reg Error: " + json["response"] + " for getNum");
        }

    })!

    <%= variable %> = _SMS_CONFIRM_DATA["number"]
})!


_if(<%= service %> == "sms-activate.ru" || <%= service %> == "getsms.online", function(){

	var country = _BAS_GETSMSCOUTRYCODE(<%= service %>, <%= country %>);
	if(country.length == 0)
		country = "0"

	var ref = ""
	if(<%= service %> == "sms-activate.ru")
	{
		ref = "browserAutomationStudio"
	}else if(<%= service %> == "getsms.online")
	{
		ref = "20111"
	}

    _call(_BAS_SMSACTIVATEPIREQUEST,{domain: <%= service %>, api_key: (<%= apikey %>), action: "getNumber", service: _BAS_GETSMSSITECODE(<%= service %>, <%= site %>), country: country, ref: ref})!
    var arr = _result()
    if(arr[0] == "NO_NUMBERS")
    {
        _sms_ban_service(60)
    }

    if(arr[0] != "ACCESS_NUMBER")
    {
        fail("Error during sms-activate getNumber " + arr.join(":"))
    }
    _SMS_CONFIRM_DATA = {id: arr[1], api: (<%= apikey %>), domain: <%= service %>, method: <%= service %>, originalid: arr[1], number: arr[2]}
    _BAS_SMSCONFIRMDATA[arr[2] ] = _SMS_CONFIRM_DATA

    <%= variable %> = _SMS_CONFIRM_DATA["number"]
})!

_if(<%= service %> == "smspva.com", function(){

	var country = _BAS_GETSMSCOUTRYCODE(<%= service %>, <%= country %>);
	if(country.length == 0)
		country = "RU"
    _call(_BAS_SMSPVAREQUEST,{apikey: (<%= apikey %>), id: 1, metod: "get_number", service: _BAS_GETSMSSITECODE(<%= service %>, <%= site %>), country: country})!
    var json = _result()
    if(json["response"] != "1")
    {
        fail("failed to get number from smspva " + JSON.stringify(json))
    }

		var prefix = json["CountryCode"].replace("+","")

    _SMS_CONFIRM_DATA = {id: json["id"], api: (<%= apikey %>), method: "smspva.com", originalid: json["id"], number: json["number"], service: _BAS_GETSMSSITECODE(<%= service %>, <%= site %>),prefix: prefix, country: country}
    _BAS_SMSCONFIRMDATA[ prefix + json["number"] ] = _SMS_CONFIRM_DATA

    <%= variable %> = prefix + _SMS_CONFIRM_DATA["number"]
})!
