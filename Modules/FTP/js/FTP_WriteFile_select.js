var FilePath = GetInputConstructorValue("FilePath", loader);
if(FilePath["original"].length == 0){
	Invalid(tr("Remote file path") + " " + tr("is empty"));
    return;
};
var Value = GetInputConstructorValue("Value", loader);
var EndingSymbol = $("#Check").is(':checked');
var Append = $("#Check2").is(':checked');
var base64 = $("#Check3").is(':checked');
try{
    var code = loader.GetAdditionalData() + _.template($("#FTP_WriteFile_code").html())({
        "FilePath": FilePath["updated"],
        "Value": Value["updated"],
        "EndingSymbol": EndingSymbol,
		"Append": Append,
        "base64": base64
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}
