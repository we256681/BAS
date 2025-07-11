var AllowedChars = GetInputConstructorValue("AllowedChars", loader);
var Length = GetInputConstructorValue("Length", loader);
var Save = this.$el.find("#Save").val().toUpperCase();
if(Save.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + tr("\" is not specified"));
    return;
};
if(AllowedChars["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Allowed Chars") + tr("\" is not specified"));
    return;
};
if(Length["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Result String Length") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($('#' + State + "_code").html())({
        "variable": "VAR_" + Save,
        "chars": AllowedChars["updated"],
        "length": Length["updated"]
    });
	code = Normalize(code, 0);
	BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}