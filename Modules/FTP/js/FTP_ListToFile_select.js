var FilePath = GetInputConstructorValue("FilePath", loader);
if(FilePath["original"].length == 0){
	Invalid(tr("Remote file path") + " " + tr("is empty"));
    return;
};
var Value = GetInputConstructorValue("Value", loader);
if(Value["original"].length==0){
	Invalid(tr("List") + " " + tr("is empty"));
    return;
};
var EndingSymbol = $("#Check").is(':checked');
var Append = $("#Check2").is(':checked');
try{
    var code = loader.GetAdditionalData() + _.template($("#FTP_ListToFile_code").html())({
        "FilePath": FilePath["updated"],
        "Value": Value["updated"],
        "EndingSymbol": EndingSymbol,
        "Append": Append
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}
