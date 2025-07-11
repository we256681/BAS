native_async("curlwrapper", "easyperform", JSON.stringify(smtp_client_send_mail(<%= mailfrom %>, <%= mailto %>, <%= mailcc %>, <%= mailsubject %>, <%= mailbody %>, <%= is_html %>, <%= attachments %>)))!
{
	var json = JSON.parse(_result())
	if(typeof(_SMTP_DEBUG) != "undefined" && _SMTP_DEBUG)
		log(json["trace"]);
	if(json["code"] != "CURLE_OK")
	{
		fail(json["code"] + ": " + json["error"])
	}
}
