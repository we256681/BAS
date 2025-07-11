var uid = GetInputConstructorValue("uid", loader);
if(uid["original"].length == 0){
    Invalid(tr("The parameter \"") + tr("Message id") + tr("\" is not specified"));
    return;
};
var box = GetInputConstructorValue("box", loader);
var Save = this.$el.find("#Save").val().toUpperCase();
if(Save.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#InMail_GetFlags_code").html())({
        "uid": uid["updated"],
        "box": box["updated"],
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}