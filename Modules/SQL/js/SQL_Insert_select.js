var table = GetInputConstructorValue("table", loader);
if(table["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Database table") + tr("\" is not specified"));
    return;
};
var fields = GetInputConstructorValue("fields", loader);
var data = GetInputConstructorValue("data", loader);
if(data["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Data") + tr("\" is not specified"));
    return;
};
var convert = $("#Check").is(':checked');
try{
    var code = loader.GetAdditionalData() + _.template($("#SQL_Insert_code").html())({
        "table": table["updated"],
        "fields": fields["updated"],
        "data": data["updated"],
        "convert": convert
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}