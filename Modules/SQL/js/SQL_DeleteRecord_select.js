var table = GetInputConstructorValue("table", loader);
if (table["original"].length == 0) {
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
try{
    var code = loader.GetAdditionalData() + _.template($("#SQL_DeleteRecord_code").html())({
        "table": table["updated"],
        "where": where,
        "parameterize": parameterize
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}