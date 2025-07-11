var oldName = GetInputConstructorValue("oldName", loader);
if(oldName["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Old folder name") + tr("\" is not specified"));
    return;
};
var newName = GetInputConstructorValue("newName", loader);
if(newName["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("New folder name") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#InMail_RenameBox_code").html())({
        "oldName": oldName["updated"],
        "newName": newName["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}