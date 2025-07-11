var string = GetInputConstructorValue("string", loader);
if(string["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("String") + tr("\" is not specified"));
    return;
};
var separators = GetInputConstructorValue("separators", loader);
var convert_types = $("#Check").is(':checked');
var list = $("#VariablesList").val().toUpperCase();
if(list.length <= 0){
	Invalid(tr("The parameter \"") + tr("Variables list") + tr("\" is not specified"));
	return;
};
list = list.replace(/\s/g,"");
list = list.split(",");
list = _.compact(list);
if(list.length <= 0){
	Invalid(tr("The parameter \"") + tr("Variables list") + tr("\" is not specified"));
	return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#parse_csv_string_code").html())({
        "string": string["updated"],
		"separators": separators["updated"],
        "convert_types": convert_types,
        "list":list
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}