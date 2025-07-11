var query;
if($("#Check").is(':checked')){
	query = "\"" + je($("#query").val()) + "\"";
}else{
	query = GetInputConstructorValue("query", loader)["updated"];
};
if($("#query").val().length == 0){
	Invalid(tr("The parameter \"") + tr("Query") + tr("\" is not specified"));
    return;
};
var parameterize = $("#Check").is(':checked');
var Save = this.$el.find("#Save").val().toUpperCase();
var data_format = GetInputConstructorValue("data_format", loader);
if(data_format["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Data format") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#SQL_Query_code").html())({
        "query": query,
		"parameterize": parameterize,
        "variable": "VAR_" + Save,
		"data_format": data_format["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}