var protocol = GetInputConstructorValue("protocol", loader);
if(protocol["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Protocol") + tr("\" is not specified"));
    return;
};
var autoConfig = GetInputConstructorValue("autoConfig", loader);
if(autoConfig["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Auto configuration") + tr("\" is not specified"));
    return;
};
var host = GetInputConstructorValue("host", loader);
var port = GetInputConstructorValue("port", loader);
var encrypt = GetInputConstructorValue("encrypt", loader);
if($('#autoConfig').val() != "true"){
	if(host["original"].length == 0){
		Invalid(tr("The parameter \"") + tr("Server address") + tr("\" is not specified"));
		return;
	};
	if(port["original"].length == 0){
		Invalid(tr("The parameter \"") + tr("Port") + tr("\" is not specified"));
		return;
	};
	if(encrypt["original"].length == 0){
		Invalid(tr("The parameter \"") + tr("Encryption") + tr("\" is not specified"));
		return;
	};
};
var username = GetInputConstructorValue("username", loader);
if($('#autoConfig').val() == "true" && username["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Username") + tr("\" is not specified"));
	return;
};
var password = GetInputConstructorValue("password", loader);
var box = GetInputConstructorValue("box", loader);
if(box["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Folder name") + tr("\" is not specified"));
    return;
};
var connectTimeout = GetInputConstructorValue("connectTimeout", loader);
if(connectTimeout["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Connection timeout") + tr("\" is not specified"));
    return;
};
var resetTimeout = GetInputConstructorValue("resetTimeout", loader);
if(resetTimeout["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Reset timeout") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#InMail_Configure_code").html())({
        "protocol": protocol["updated"],
        "autoConfig": autoConfig["updated"],
        "host": host["updated"],
        "port": port["updated"],
        "encrypt": encrypt["updated"],
        "username": username["updated"],
        "password": password["updated"],
        "box": box["updated"],
        "connectTimeout": connectTimeout["updated"],
        "resetTimeout": resetTimeout["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}