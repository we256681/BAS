var Save = this.$el.find("#Save").val().toUpperCase();
var Value = GetInputConstructorValue("Value", loader);
var encode = $("#Select").val()=="encode"
if(Save.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + tr("\" is not specified"));
	return;
};
if(Value["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Data") + tr("\" is not specified"));
	return;
};
try{
	var code = loader.GetAdditionalData() + _.template($("#base64_code").html())({
		"variable": "VAR_" + Save,
		"value": Value["updated"],
		"encode": encode
	});
	code = Normalize(code, 0);
	BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}