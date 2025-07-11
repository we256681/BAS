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
var box = GetInputConstructorValue("box", loader);
var Save = this.$el.find("#Save").val().toUpperCase();
if(Save.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#InMail_Count_code").html())({
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
        "box": box["updated"],
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}