var path = GetInputConstructorValue("path", loader);
if(path["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Path") + tr("\" is not specified"));
    return;
};
var ext = GetInputConstructorValue("ext", loader);
var Save = this.$el.find("#Save").val().toUpperCase();
if(Save.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#path_base_name_code").html())({
        "path": path["updated"],
		"ext": ext["updated"],
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}