var url = GetInputConstructorValue("url", loader);
if(url["original"].length == 0){
	Invalid("URL" + " " + tr("is empty"));
    return;
};
var protocol = this.$el.find("#protocol").val().toUpperCase();
if(protocol.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Protocol/Scheme") + tr("\" is not specified"));
    return;
};
var username = this.$el.find("#username").val().toUpperCase();
if(username.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Username") + tr("\" is not specified"));
    return;
};
var password = this.$el.find("#password").val().toUpperCase();
if(password.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Password") + tr("\" is not specified"));
    return;
};
var host = this.$el.find("#host").val().toUpperCase();
if(host.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Host") + tr("\" is not specified"));
    return;
};
var port = this.$el.find("#port").val().toUpperCase();
if(port.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Port") + tr("\" is not specified"));
    return;
};
var path = this.$el.find("#path").val().toUpperCase();
if(path.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Path") + tr("\" is not specified"));
    return;
};
var query = this.$el.find("#query").val().toUpperCase();
if(query.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Query") + tr("\" is not specified"));
    return;
};
var hash = this.$el.find("#hash").val().toUpperCase();
if(hash.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Fragment") + tr("\" is not specified"));
    return;
};
var normalize = $("#Check").is(':checked');
var rfail = $("#Check2").is(':checked');
var base_url = GetInputConstructorValue("base_url", loader);
try{
    var code = loader.GetAdditionalData() + _.template($("#parse_url_code").html())({
        "url": url["updated"],
        "protocol": "VAR_" + protocol,
        "username": "VAR_" + username,
        "password": "VAR_" + password,
        "host": "VAR_" + host,
        "port": "VAR_" + port,
        "path": "VAR_" + path,
        "query": "VAR_" + query,
        "hash": "VAR_" + hash,
		"normalize": normalize,
		"rfail": rfail,		
        "base_url": base_url["updated"],
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}