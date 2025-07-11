var FilePath = GetInputConstructorValueFilename("FilePath", loader);
if(FilePath["original"].length == 0){
    Invalid(tr("The parameter \"") + tr("File path") + tr("\" is not specified"));
    return;
};
var SheetList = GetInputConstructorValue("SheetList", loader);
var SuccessNumber = GetInputConstructorValue("SuccessNumber", loader);
if(SuccessNumber["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Success Usage") + tr("\" is not specified"));
    return;
};
var FailNumber = GetInputConstructorValue("FailNumber", loader);
if(FailNumber["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Fail Usage") + tr("\" is not specified"));
    return;
};
var SimultaneousUsage = GetInputConstructorValue("SimultaneousUsage", loader);
if(SimultaneousUsage["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Simultaneous Usage") + tr("\" is not specified"));
    return;
};
var Interval = GetInputConstructorValue("Interval", loader);
if(Interval["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Interval Between Usage") + tr("\" is not specified"));
    return;
};
var Greedy = $("#Check").is(':checked');
var DontGiveUp = $("#Check2").is(':checked');
try{
    var code = loader.GetAdditionalData() + _.template($("#Excel_ImportToResources_code").html())({
        "FilePath": FilePath["updated"],
		"SheetList": SheetList["updated"],
        "SuccessNumber": SuccessNumber["updated"],
        "FailNumber": FailNumber["updated"],
        "SimultaneousUsage": SimultaneousUsage["updated"],
        "Interval": Interval["updated"],
        "Greedy": Greedy,
        "DontGiveUp": DontGiveUp
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}