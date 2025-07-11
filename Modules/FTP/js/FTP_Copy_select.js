var FilePath = GetInputConstructorValue("FilePath", loader);
if(FilePath["original"].length == 0){
	Invalid(tr("Remote path to file or folder") + " " + tr("is empty"));
    return;
};
var CopyPath = GetInputConstructorValue("CopyPath", loader);
if(CopyPath["original"].length == 0){
	Invalid(tr("Remote destination path") + " " + tr("is empty"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#FTP_Copy_code").html())({
        "FilePath": FilePath["updated"],
		"CopyPath": CopyPath["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}