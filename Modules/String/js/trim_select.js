var string = GetInputConstructorValue("string", loader);
if(string["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("String") + tr("\" is not specified"));
    return;
};
var spaces = $("#Check").is(':checked');
var breaks = $("#Check2").is(':checked');
var tabs = $("#Check3").is(':checked');
var characters = GetInputConstructorValue("characters", loader);
var all = $("#Check4").is(':checked');
var left = $("#Check5").is(':checked');
var right = $("#Check6").is(':checked');
var Save = this.$el.find("#Save").val().toUpperCase();
if(Save.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#trim_code").html())({
        "string": string["updated"],
		"spaces": spaces,
		"breaks": breaks,
		"tabs": tabs,
		"characters": characters["updated"],
		"all": all,
		"left": left,
		"right": right,
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}
