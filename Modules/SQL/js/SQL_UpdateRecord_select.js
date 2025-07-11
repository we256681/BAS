var table = GetInputConstructorValue("table", loader);
if (table["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Database table") + tr("\" is not specified"));
    return;
};
var where;
if($("#Check").is(':checked')){
	where = "\"" + je($("#where").val()) + "\"";
}else{
	where = GetInputConstructorValue("where", loader)["updated"];
};
var parameterize = $("#Check").is(':checked');
var values = "\"" + je($("#values").val()) + "\"";
if($("#values").val().length == 0){
	Invalid(tr("The parameter \"") + tr("Values") + tr("\" is not specified"));
    return;
};
var convert = $("#Check2").is(':checked');
var fields = GetInputConstructorValue("fields", loader);
try {
    var code = loader.GetAdditionalData() + _.template($("#SQL_UpdateRecord_code").html())({
        "table": table["updated"],
        "where": where,
        "parameterize": parameterize,
        "values": values,
        "convert": convert,
        "fields": fields["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch (e){}