var FilePath = GetInputConstructorValueFilename("FilePath", loader);
if(FilePath["original"].length == 0){
    Invalid(tr("The parameter \"") + tr("File path") + tr("\" is not specified"));
    return;
};
var SheetIndexOrName = GetInputConstructorValue("SheetIndexOrName", loader);
if(SheetIndexOrName["original"].length == 0){
    Invalid(tr("The parameter \"") + tr("Sheet index or name") + tr("\" is not specified"));
    return;
};
var FromRow = GetInputConstructorValue("FromRow", loader);
var Data = GetInputConstructorValue("Data", loader);
if(Data["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Data") + tr("\" is not specified"));
    return;
};
var Convert = $("#Check").is(':checked');
var SetFormat = $("#Check2").is(':checked');
try {
    var code = loader.GetAdditionalData() + _.template($("#Excel_InsertRows_code").html())({
        "FilePath": FilePath["updated"],
        "SheetIndexOrName": SheetIndexOrName["updated"],
        "FromRow": FromRow["updated"],
        "Data": Data["updated"],
        "Convert": Convert,
        "SetFormat": SetFormat,
		"Sync": BrowserAutomationStudio_UsesWaitCode()
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}