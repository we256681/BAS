var Protocol = GetInputConstructorValue("Protocol", loader);
if(Protocol["original"].length == 0){
	Invalid(tr("Protocol") + " " + tr("is empty"));
    return;
};
var Host = GetInputConstructorValue("Host", loader);
if(Host["original"].length == 0){
	Invalid(tr("Host name") + " " + tr("is empty"));
    return;
};
var Port = GetInputConstructorValue("Port", loader);
if(Port["original"].length == 0){
	Invalid(tr("Port") + " " + tr("is empty"));
    return;
};
var Username = GetInputConstructorValue("Username", loader);
if(Username["original"].length == 0){
	Invalid(tr("Username") + " " + tr("is empty"));
    return;
};
var Password = GetInputConstructorValue("Password", loader);
if(Password["original"].length == 0){
	Invalid(tr("Password") + " " + tr("is empty"));
    return;
};
var Timeout = GetInputConstructorValue("Timeout", loader);
if(Timeout["original"].length == 0){
	Invalid(tr("Timeout") + " " + tr("is empty"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#FTP_Config_code").html())({
        "Protocol": Protocol["updated"],
        "Host": Host["updated"],
        "Port": Port["updated"],
        "Username": Username["updated"],
        "Password": Password["updated"],
        "Timeout": Timeout["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}