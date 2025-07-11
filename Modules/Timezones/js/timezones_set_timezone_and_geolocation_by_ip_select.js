var Value = GetInputConstructorValue("Value", loader);

var IpInfoMethod = GetInputConstructorValue("IpInfoMethod", loader);
var IpApiKey = GetInputConstructorValue("IpApiKey", loader);
var FunctionName = this.$el.find("#FunctionName").val();
var ChangeTimezone = GetInputConstructorValue("ChangeTimezone",loader)
var ChangeGeolocation = GetInputConstructorValue("ChangeGeolocation",loader)
var ChangeWebrtcIp = GetInputConstructorValue("ChangeWebrtcIp",loader)
var ChangeBrowserLanguage = GetInputConstructorValue("ChangeBrowserLanguage",loader)
var CallsFunctionCode = ""

if(FunctionName.length == 0)
{
  FunctionName = "null"
}else
{
  CallsFunctionCode = "\/*CallsFunction:" + FunctionName + "*\/\n"
}

if(Value["original"].length == 0)
{
  Invalid("Ip is empty");
  return;
}
 try{
  var code = loader.GetAdditionalData() + _.template($("#timezones_set_timezone_and_geolocation_by_ip_code").html())(
    {
      value: Value["updated"],
      ip_info_method: IpInfoMethod["updated"],
      ip_api_key: IpApiKey["updated"],
      function_name: FunctionName,
      change_timezone: ChangeTimezone["updated"], 
      change_geolocation: ChangeGeolocation["updated"], 
      change_webrtc: ChangeWebrtcIp["updated"],
      change_browserlanguage: ChangeBrowserLanguage["updated"]
    })
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + CallsFunctionCode + code, action, DisableIfAdd);
}catch(e)
{}