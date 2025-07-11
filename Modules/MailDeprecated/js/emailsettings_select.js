var Host = GetInputConstructorValue("Host",loader)
var Port = GetInputConstructorValue("Port",loader)
var Username = GetInputConstructorValue("Username",loader)
var Password = GetInputConstructorValue("Password",loader)
var Inbox = GetInputConstructorValue("Inbox",loader)
var Secure = $("#Select").val()

if(Host["original"].length <= 0)
{
    Invalid("Host is empty");
    return;
}
if(Port["original"].length <= 0)
{
    Invalid("Port is empty");
    return;
}
if(Inbox["original"].length <= 0)
{
    Invalid("Inbox is empty");
    return;
}
if(Username["original"].length <= 0)
{
    Username["updated"] = '""';
}
if(Password["original"].length <= 0)
{
    Password["updated"] = '""';
}
    try{
    var code = loader.GetAdditionalData() + _.template($("#emailsettings_code").html())({host:Host["updated"], username:Username["updated"], password: Password["updated"], port:Port["updated"], encryption:"\"" + je(Secure) + "\"", inbox:Inbox["updated"]})
    code = Normalize(code,0)
    BrowserAutomationStudio_Append("" , BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}