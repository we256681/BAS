var FilePath = GetInputConstructorValue("FilePath", loader);
if(FilePath["original"].length == 0){
	Invalid(tr("Remote path to file or folder") + " " + tr("is empty"));
    return;
};
var NewFilePath = GetInputConstructorValue("NewFilePath", loader);
if(NewFilePath["original"].length == 0){
	Invalid(tr("New remote path to file or folder") + " " + tr("is empty"))
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#FTP_Move_code").html())({
        "FilePath": FilePath["updated"],
        "NewFilePath": NewFilePath["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}