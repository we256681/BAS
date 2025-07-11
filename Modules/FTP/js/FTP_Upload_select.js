var FilePath = GetInputConstructorValueFilename("FilePath", loader);
if(FilePath["original"].length == 0){
	Invalid(tr("Remote path") + " " + tr("is empty"));
    return;
};
var DestinationPath = GetInputConstructorValue("DestinationPath", loader);
if(DestinationPath["original"].length == 0){
    Invalid(tr("Remote destination path") + " " + tr("is empty"));
    return;
};
var VerifyChecksums = $("#Check").is(':checked');
try{
    var code = loader.GetAdditionalData() + _.template($("#FTP_Upload_code").html())({
        "FilePath": FilePath["updated"],
        "DestinationPath": DestinationPath["updated"],
		"VerifyChecksums": VerifyChecksums
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}