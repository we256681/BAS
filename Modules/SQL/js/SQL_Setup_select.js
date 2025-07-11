var dialect = GetInputConstructorValue("dialect", loader);
if(dialect["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Database dialect") + tr("\" is not specified"));
    return;
};
var host = GetInputConstructorValue("host", loader);
var port = GetInputConstructorValue("port", loader);
var username = GetInputConstructorValue("username", loader);
var password = GetInputConstructorValue("password", loader);
var database = GetInputConstructorValue("database", loader);
var storage = GetInputConstructorValueFilename("storage", loader);
var connect_timeout = GetInputConstructorValue("connect_timeout", loader);
var timeout = GetInputConstructorValue("timeout", loader);
if(timeout["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Timeout") + tr("\" is not specified"));
    return;
};
var dialect_options = GetInputConstructorValue("dialect_options", loader);
try{
    var code = loader.GetAdditionalData() + _.template($("#SQL_Setup_code").html())({
        "dialect": dialect["updated"],
        "host": host["updated"],
        "port": port["updated"],
        "username": username["updated"],
        "password": password["updated"],
        "database": database["updated"],
        "storage": storage["updated"],
		"connect_timeout": connect_timeout["updated"],
        "timeout": timeout["updated"],
        "dialect_options": dialect_options["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}