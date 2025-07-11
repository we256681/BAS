var string = GetInputConstructorValue("string", loader);
if(string["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("String") + tr("\" is not specified"));
    return;
};
var decimals = GetInputConstructorValue("decimals", loader);
var dec_point = GetInputConstructorValue("dec_point", loader);
var thousands_sep = GetInputConstructorValue("thousands_sep", loader);
var Save = this.$el.find("#Save").val().toUpperCase();
if(Save.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#to_number_code").html())({
        "string": string["updated"],
		"decimals": decimals["updated"],
		"dec_point": dec_point["updated"],
		"thousands_sep": thousands_sep["updated"],
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}