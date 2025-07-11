var string = GetInputConstructorValue("string", loader);
if(string["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("String") + tr("\" is not specified"));
    return;
};
var spaces = $("#Check").is(':checked');
var breaks = $("#Check2").is(':checked');
var tabs = $("#Check3").is(':checked');
var characters_to_delete = GetInputConstructorValue("characters_to_delete", loader);
var characters_to_space = GetInputConstructorValue("characters_to_space", loader);
var Save = this.$el.find("#Save").val().toUpperCase();
if(Save.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#clean_code").html())({
        "string": string["updated"],
		"spaces": spaces,
		"breaks": breaks,
		"tabs": tabs,
		"characters_to_delete": characters_to_delete["updated"],
		"characters_to_space": characters_to_space["updated"],
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}