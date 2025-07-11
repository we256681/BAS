var number = GetInputConstructorValue("number", loader);
if(number["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Number") + tr("\" is not specified"));
    return;
};
var status = GetInputConstructorValue("status", loader);
if(status["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Stаtus") + tr("\" is not specified"));
    return;
};
var deleteInfo = $("#Check").is(':checked');
try{
    var code = loader.GetAdditionalData() + _.template($("#SMS_SetStatus_code").html())({
        "number": number["updated"],
        "status": status["updated"],
		"deleteInfo": deleteInfo
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}