var ProxyText = GetInputConstructorValue("ProxyText",loader)
var ProxyType = GetInputConstructorValue("ProxyType", loader)
var ProxyLogin = GetInputConstructorValue("ProxyLogin",loader)
if(ProxyLogin["updated"] == "")
ProxyLogin["updated"] = '""'
var ProxyPassword = GetInputConstructorValue("ProxyPassword",loader)
if(ProxyPassword["updated"] == "")
ProxyPassword["updated"] = '""'
var IsEmptyProxy = false
if(ProxyText["updated"] == "")
{
ProxyText["updated"] = '""'
IsEmptyProxy = true
}
/*if(ProxyText["original"].length <= 0)
{
Invalid("Proxy is empty");
return;
}*/

try{
var code = loader.GetAdditionalData() + _.template($("#emailproxy_code").html())({ProxyText:ProxyText["updated"], ProxyType:ProxyType["updated"],ProxyLogin:ProxyLogin["updated"],ProxyPassword:ProxyPassword["updated"]})
code = Normalize(code,0)
BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}
