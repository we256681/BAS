var number = GetInputConstructorValue("number", loader);
if(number["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Number") + tr("\" is not specified"));
    return;
};
var interval = GetInputConstructorValue("interval", loader);
if(interval["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Interval") + tr("\" is not specified"));
    return;
};
var timeout = GetInputConstructorValue("timeout", loader);
if(timeout["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Timeout") + tr("\" is not specified"));
    return;
};
var Save = this.$el.find("#Save").val().toUpperCase();
try{
    var code = loader.GetAdditionalData() + _.template($("#SMS_GetCode_code").html())({
        "number": number["updated"],
        "interval": interval["updated"],
        "timeout": timeout["updated"],
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}