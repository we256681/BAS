var protocol = GetInputConstructorValue("protocol", loader);
var username = GetInputConstructorValue("username", loader);
var password = GetInputConstructorValue("password", loader);
var host = GetInputConstructorValue("host", loader);
var port = GetInputConstructorValue("port", loader);
var path = GetInputConstructorValue("path", loader);
var query = GetInputConstructorValue("query", loader);
var hash = GetInputConstructorValue("hash", loader);
var Save = this.$el.find("#Save").val().toUpperCase();
if(Save.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#generate_url_code").html())({
        "protocol": protocol["updated"],
        "username": username["updated"],
        "password": password["updated"],
        "host": host["updated"],
        "port": port["updated"],
        "path": path["updated"],
        "query": query["updated"],
        "hash": hash["updated"],
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}