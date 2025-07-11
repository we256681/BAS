var from = GetInputConstructorValue("from", loader);
var notFrom = GetInputConstructorValue("notFrom", loader);
var to = GetInputConstructorValue("to", loader);
var notTo = GetInputConstructorValue("notTo", loader);
var subject = GetInputConstructorValue("subject", loader);
var notSubject = GetInputConstructorValue("notSubject", loader);
var text = GetInputConstructorValue("text", loader);
var notText = GetInputConstructorValue("notText", loader);
var flags = GetInputConstructorValue("flags", loader);
var notFlags = GetInputConstructorValue("notFlags", loader);
var since = GetInputConstructorValue("since", loader);
var before = GetInputConstructorValue("before", loader);
var sortType = GetInputConstructorValue("sortType", loader);
if(sortType["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Sorting type") + tr("\" is not specified"));
    return;
};
var sortField = GetInputConstructorValue("sortField", loader);
var wait = $("#wait").is(':checked');
var minResults = GetInputConstructorValue("minResults", loader);
if(wait && minResults["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Number of letters") + tr("\" is not specified"));
    return;
};
var interval = GetInputConstructorValue("interval", loader);
if(wait && interval["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Interval") + tr("\" is not specified"));
    return;
};
var timeout = GetInputConstructorValue("timeout", loader);
if(wait && timeout["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Timeout") + tr("\" is not specified"));
    return;
};
var box = GetInputConstructorValue("box", loader);
var Save = this.$el.find("#Save").val().toUpperCase();
if(Save.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#InMail_SearchOne_code").html())({
		"from": from["updated"],
		"notFrom": notFrom["updated"],
		"to": to["updated"],
		"notTo": notTo["updated"],
		"subject": subject["updated"],
		"notSubject": notSubject["updated"],
		"text": text["updated"],
		"notText": notText["updated"],
		"flags": flags["updated"],
		"notFlags": notFlags["updated"],
		"since": since["updated"],
		"before": before["updated"],
		"sortType": sortType["updated"],
		"sortField": sortField["updated"],
		"wait": wait,
		"minResults": minResults["updated"],
        "interval": interval["updated"],
        "timeout": timeout["updated"],
        "box": box["updated"],
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}