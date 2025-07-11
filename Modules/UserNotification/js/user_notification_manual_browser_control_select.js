var Label = GetInputConstructorValue("Label", loader);


try{
  var code = loader.GetAdditionalData() + _.template($('#user_notification_manual_browser_control_code').html())({label: Label["updated"]})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{
}