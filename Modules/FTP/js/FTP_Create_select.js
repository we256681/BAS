var FilePath = GetInputConstructorValue("FilePath", loader);
if(FilePath["original"].length == 0){
	Invalid(tr("Remote path to new file/folder") + " " + tr("is empty"));
    return;
};
var isFile = $("#Check").is(':checked');
var isDirectory = $("#Check2").is(':checked');
try{
    var code = loader.GetAdditionalData() + _.template($("#FTP_Create_code").html())({
        "FilePath": FilePath["updated"],
        "isFile": isFile,
		"isDirectory": isDirectory,
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}