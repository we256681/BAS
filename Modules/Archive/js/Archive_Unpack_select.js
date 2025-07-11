var ArchivePath = GetInputConstructorValue("ArchivePath", loader);
if(ArchivePath["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Archive path") + tr("\" is not specified"));
    return;
};
var DestinationPath = GetInputConstructorValue("DestinationPath", loader);
var ArchiveType = GetInputConstructorValue("ArchiveType", loader);
if(ArchiveType["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Archive type") + tr("\" is not specified"));
    return;
};
var ListOfFiles = GetInputConstructorValue("ListOfFiles", loader);
var password = GetInputConstructorValue("password", loader);
try{
    var code = loader.GetAdditionalData() + _.template($("#Archive_Unpack_code").html())({
        "ArchivePath": ArchivePath["updated"],
        "DestinationPath": DestinationPath["updated"],
        "ArchiveType": ArchiveType["updated"],
        "ListOfFiles": ListOfFiles["updated"],
        "password": password["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}
