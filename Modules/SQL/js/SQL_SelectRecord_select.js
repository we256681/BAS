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
var included_columns = GetInputConstructorValue("included_columns", loader);
var excluded_columns = GetInputConstructorValue("excluded_columns", loader);
var order_column = GetInputConstructorValue("order_column", loader);
var order_direction = GetInputConstructorValue("order_direction", loader);
var offset = GetInputConstructorValue("offset", loader);
var Save = this.$el.find("#Save").val().toUpperCase();
var data_format = GetInputConstructorValue("data_format", loader);
if(data_format["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Data format") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#SQL_SelectRecord_code").html())({
		"table": table["updated"],
        "where": where,
        "parameterize": parameterize,
        "included_columns": included_columns["updated"],
        "excluded_columns": excluded_columns["updated"],
        "offset": offset["updated"],
        "order_column": order_column["updated"],
        "order_direction": order_direction["updated"],
        "variable": "VAR_" + Save,
		"data_format": data_format["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}