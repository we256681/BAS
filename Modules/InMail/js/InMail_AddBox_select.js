var name = GetInputConstructorValue("name", loader);
if(name["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Folder name") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#InMail_AddBox_code").html())({
        "name": name["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}