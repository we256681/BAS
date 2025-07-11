var url = GetInputConstructorValue("url", loader);
if(url["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("URL") + tr("\" is not specified"));
    return;
};
var protocol = GetInputConstructorValue("protocol", loader);
var username = GetInputConstructorValue("username", loader);
var password = GetInputConstructorValue("password", loader);
var host = GetInputConstructorValue("host", loader);
var port = GetInputConstructorValue("port", loader);
var path = GetInputConstructorValue("path", loader);
var query = GetInputConstructorValue("query", loader);
var hash = GetInputConstructorValue("hash", loader);
var clear_query = $("#Check3").is(':checked');
var normalize = $("#Check").is(':checked');
var rfail = $("#Check2").is(':checked');
var base_url = GetInputConstructorValue("base_url", loader);
var Save = this.$el.find("#Save").val().toUpperCase();
if(Save.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#change_url_code").html())({
		"url": url["updated"],
        "protocol": protocol["updated"],
        "username": username["updated"],
        "password": password["updated"],
        "host": host["updated"],
        "port": port["updated"],
        "path": path["updated"],
        "query": query["updated"],
        "hash": hash["updated"],
		"clear_query": clear_query,
		"normalize": normalize,
		"rfail": rfail,		
        "base_url": base_url["updated"],
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}