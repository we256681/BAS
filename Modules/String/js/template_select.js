var template = '';
var isspintax = $("#Check2").is(':checked');
var istemplate = $("#Check3").is(':checked');
if($("#Check").is(':checked')){
    template = GetInputConstructorValue("Template", loader)["updated"];
}else{
	template = "\"" + je($("#Template").val()) + "\"";
};
if(istemplate){
	template = template.replace("\\)\\!", "BASASYNC")
};
var Save = this.$el.find("#Save").val().toUpperCase();
if(Save.length==0){
	Invalid(tr("The parameter \"") + tr("Variable") + tr("\" is not specified"));
	return;
};
if($("#Template").val().length==0){
	Invalid(tr("The parameter \"") + tr("Template") + tr("\" is not specified"));
	return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#template_code").html())({
        "variable": "VAR_" + Save,
        "template": template,
        "isspintax": isspintax,
        "istemplate": istemplate
    });
	code = Normalize(code, 0);
	BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}