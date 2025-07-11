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
var FromCell = GetInputConstructorValue("FromCell", loader);
if(FromCell["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("From cell") + tr("\" is not specified"));
    return;
};
var ToCell = GetInputConstructorValue("ToCell", loader);
if(ToCell["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("To cell") + tr("\" is not specified"));
    return;
};
var StyleName = GetInputConstructorValue("StyleName", loader);
if(StyleName["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Style name") + tr("\" is not specified"));
    return;
};
var StyleValue = GetInputConstructorValue("StyleValue", loader);
if(StyleValue["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Style value") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#Excel_SetStyleToCellsRange_code").html())({
        "FilePath": FilePath["updated"],
        "SheetIndexOrName": SheetIndexOrName["updated"],
        "FromCell": FromCell["updated"],
        "ToCell": ToCell["updated"],
        "StyleName": StyleName["updated"],
        "StyleValue": StyleValue["updated"],
		"Sync": BrowserAutomationStudio_UsesWaitCode()
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}