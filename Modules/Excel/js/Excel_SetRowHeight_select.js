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
var RowIndex = GetInputConstructorValue("RowIndex", loader);
if(RowIndex["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Row index") + tr("\" is not specified"));
    return;
};
var Height = GetInputConstructorValue("Height", loader);
if(Height["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Height") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#Excel_SetRowHeight_code").html())({
        "FilePath": FilePath["updated"],
        "SheetIndexOrName": SheetIndexOrName["updated"],
        "RowIndex": RowIndex["updated"],
        "Height": Height["updated"],
		"Sync": BrowserAutomationStudio_UsesWaitCode()
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}
