var Value = GetInputConstructorValue("Value", loader);
var TimezoneName = GetInputConstructorValue("TimezoneName", loader);

 try{
  var code = loader.GetAdditionalData() + _.template($("#timezones_set_timezone_code").html())({
    value: Value["updated"], 
    timezonename: TimezoneName["updated"],
  })
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}