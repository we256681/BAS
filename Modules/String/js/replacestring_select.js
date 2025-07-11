var Value = GetInputConstructorValue("Value", loader);
var ReplaceFrom = GetInputConstructorValue("ReplaceFrom", loader);
var ReplaceTo = GetInputConstructorValue("ReplaceTo", loader);
var Save = this.$el.find("#Save").val().toUpperCase();
if(Save.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + tr("\" is not specified"));
    return;
};
if(Value["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("String") + tr("\" is not specified"));
    return;
};
if(ReplaceFrom["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Replace From") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#replacestring_code").html())({
        "variable": "VAR_" + Save,
        "value": Value["updated"],
        "from": ReplaceFrom["updated"],
        "to": ReplaceTo["updated"]
    });
	code = Normalize(code, 0);
	BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch (e){}