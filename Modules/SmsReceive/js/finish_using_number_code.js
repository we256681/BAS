_SMS_CONFIRM_DATA = _BAS_SMSCONFIRMDATA[<%= number %>]
    if(!_SMS_CONFIRM_DATA)
        fail("No information about this number")


_if(_SMS_CONFIRM_DATA["method"] == "sms-reg.com", function(){
	delete _BAS_SMSCONFIRMDATA[_SMS_CONFIRM_DATA["number"]];
})!

_if(_SMS_CONFIRM_DATA["method"] == "sms-activate.ru" || _SMS_CONFIRM_DATA["method"] == "getsms.online", function(){
	_call(_BAS_SMSACTIVATEPIREQUEST,{domain: _SMS_CONFIRM_DATA["domain"], api_key: _SMS_CONFIRM_DATA["api"], action: "setStatus", status: "6", id: _SMS_CONFIRM_DATA["id"]})!
	var arr = _result()
    if(arr[0].indexOf("ACCESS_") != 0)
    {
        fail("Error during sms-activate setStatus(6) " + arr.join(":"))
    }
	delete _BAS_SMSCONFIRMDATA[_SMS_CONFIRM_DATA["number"]];
})!

_if(_SMS_CONFIRM_DATA["method"] == "smspva.com", function(){
	delete _BAS_SMSCONFIRMDATA[ _SMS_CONFIRM_DATA["prefix"] + _SMS_CONFIRM_DATA["number"]];
})!
