var FilePath = GetInputConstructorValueFilename("FilePath", loader);
if(FilePath["original"].length == 0){
    Invalid(tr("The parameter \"") + tr("File path") + tr("\" is not specified"));
    return;
};
var ResourceList = GetInputConstructorValue("ResourceList", loader);
if(ResourceList["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("List of resource names") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#Excel_ExportFromResources_code").html())({
        "FilePath": FilePath["updated"],
        "ResourceList": ResourceList["updated"],
		"Sync": BrowserAutomationStudio_UsesWaitCode()
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}