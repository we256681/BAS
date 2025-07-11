var FolderPath = GetInputConstructorValue("FolderPath", loader);
if(FolderPath["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Folder path") + tr("\" is not specified"));
    return;
};
var ArchiveType = GetInputConstructorValue("ArchiveType", loader);
if(ArchiveType["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Archive type") + tr("\" is not specified"));
    return;
};
var DestinationPath = GetInputConstructorValue("DestinationPath", loader);
try{
    var code = loader.GetAdditionalData() + _.template($("#Archive_ArchiveFolder_code").html())({
        "FolderPath": FolderPath["updated"],
		"ArchiveType": ArchiveType["updated"],
        "DestinationPath": DestinationPath["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}