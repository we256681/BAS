var url = GetInputConstructorValue("url", loader);
if(url["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("URL") + tr("\" is not specified"));
    return;
};
var base_url = GetInputConstructorValue("base_url", loader);
var default_protocol = GetInputConstructorValue("default_protocol", loader);
var remove_query_parameters = GetInputConstructorValue("remove_query_parameters", loader);
var remove_directory_index = GetInputConstructorValue("remove_directory_index", loader);
var normalize_protocol = $("#Check").is(':checked');
var force_http = $("#Check2").is(':checked');
var force_https = $("#Check3").is(':checked');
var strip_authentication = $("#Check4").is(':checked');
var strip_query = $("#Check5").is(':checked');
var strip_hash = $("#Check6").is(':checked');
var strip_protocol = $("#Check7").is(':checked');
var strip_www = $("#Check8").is(':checked');
var remove_trailing_slash = $("#Check9").is(':checked');
var sort_query_parameters = $("#Check10").is(':checked');
var Save = this.$el.find("#Save").val().toUpperCase();
if(Save.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#normalize_url_code").html())({
        "url": url["updated"],
		"base_url": base_url["updated"],
		"default_protocol": default_protocol["updated"],
		"remove_query_parameters": remove_query_parameters["updated"],
		"remove_directory_index": remove_directory_index["updated"],
		"normalize_protocol": normalize_protocol,
		"force_http": force_http,
		"force_https": force_https,
		"strip_authentication": strip_authentication,
		"strip_query": strip_query,
		"strip_hash": strip_hash,
		"strip_protocol": strip_protocol,
		"strip_www": strip_www,
		"remove_trailing_slash": remove_trailing_slash,
		"sort_query_parameters": sort_query_parameters,
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}