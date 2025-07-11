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
var Save = this.$el.find("#Save").val().toUpperCase();
try{
    var code = loader.GetAdditionalData() + _.template($("#SQL_CountRecords_code").html())({
        "table": table["updated"],
        "where": where,
        "parameterize": parameterize,
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch (e){}