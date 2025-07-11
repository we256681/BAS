var FilePath = GetInputConstructorValueFilename("FilePath", loader);
if(FilePath["original"].length == 0){
    Invalid(tr("The parameter \"") + tr("File path") + tr("\" is not specified"));
    return;
};
var SheetName = GetInputConstructorValue("SheetName", loader);
if(SheetName["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Sheet name") + tr("\" is not specified"));
    return;
};
var SheetIndex = GetInputConstructorValue("SheetIndex", loader);
try{
    var code = loader.GetAdditionalData() + _.template($("#Excel_AddSheet_code").html())({
        "FilePath": FilePath["updated"],
        "SheetName": SheetName["updated"],
        "SheetIndex": SheetIndex["updated"],
		"Sync": BrowserAutomationStudio_UsesWaitCode()
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}