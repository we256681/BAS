var service = GetInputConstructorValue("service", loader);
if(service["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Service") + tr("\" is not specified"));
    return;
};
var apiKey = GetInputConstructorValue("apiKey", loader);
if(apiKey["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("API key") + tr("\" is not specified"));
    return;
};
var site = $("#site").val();
var country = GetInputConstructorValue("country", loader);
var operator = GetInputConstructorValue("operator", loader);
var customSite = GetInputConstructorValue("customSite", loader);
var customCountry = GetInputConstructorValue("customCountry", loader);
var serverUrl = GetInputConstructorValue("serverUrl", loader);
var Save = this.$el.find("#Save").val().toUpperCase();
try{
    var code = loader.GetAdditionalData() + _.template($("#SMS_GetNumber_code").html())({
        "service": service["updated"],
        "apiKey": apiKey["updated"],
		"site": "\"" + site + "\"",
        "country": country["updated"],
        "operator": operator["updated"],
		"customSite": customSite["updated"],
		"customCountry": customCountry["updated"],
        "serverUrl": serverUrl["updated"],
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}