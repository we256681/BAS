var proxyString = GetInputConstructorValue("proxyString", loader);
var proxyType = GetInputConstructorValue("proxyType", loader);
var proxyUsername = GetInputConstructorValue("proxyUsername", loader);
var proxyPassword = GetInputConstructorValue("proxyPassword", loader);
try{
    var code = loader.GetAdditionalData() + _.template($("#InMail_Proxy_code").html())({
        "proxyString": proxyString["updated"],
        "proxyType": proxyType["updated"],
        "proxyUsername": proxyUsername["updated"],
        "proxyPassword": proxyPassword["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}