var File1 = GetInputConstructorValue("File1", loader);
var File2 = GetInputConstructorValue("File2", loader);
var File3 = GetInputConstructorValue("File3", loader);
var ListOfFiles = GetInputConstructorValue("ListOfFiles", loader);
if(File1["original"].length==0 && File2["original"].length==0 && File3["original"].length==0 && ListOfFiles["original"].length==0){
	Invalid(tr("The parameter \"") + tr("List of files") + tr("\" is not specified"));
    return;
};
var ArchiveType = GetInputConstructorValue("ArchiveType", loader);
if(ArchiveType["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Archive type") + tr("\" is not specified"));
    return;
};
var DestinationPath = GetInputConstructorValue("DestinationPath", loader);
if (DestinationPath["original"].length == 0) {
	Invalid(tr("The parameter \"") + tr("Destination path") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#Archive_ArchiveFiles_code").html())({
        "File1": File1["updated"],
        "File2": File2["updated"],
        "File3": File3["updated"],
        "ListOfFiles": ListOfFiles["updated"],
		"ArchiveType": ArchiveType["updated"],
        "DestinationPath": DestinationPath["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}