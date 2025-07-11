var FilePath = GetInputConstructorValue("FilePath", loader);
if(FilePath["original"].length == 0){
	Invalid(tr("Remote path to file or folder") + " " + tr("is empty"));
    return;
};
var DestinationPath = GetInputConstructorValueFilename("DestinationPath", loader);
if(DestinationPath["original"].length == 0){
    Invalid(tr("Destination path") + " " + tr("is empty"));
    return;
};
var VerifyChecksums = $("#Check").is(':checked');
try{
    var code = loader.GetAdditionalData() + _.template($("#FTP_Download_code").html())({
        "FilePath": FilePath["updated"],
        "DestinationPath": DestinationPath["updated"],
		"VerifyChecksums": VerifyChecksums
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}