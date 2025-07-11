var val = GetInputConstructorValue("Code",loader)
if(val["original"].length > 0)
{
   try{
    var code = loader.GetAdditionalData() + _.template($("#user_notification_web_interface_eval_code").html())({code:val["updated"]})
    code = Normalize(code,0)
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
  }catch(e)
  {}
}else
{
  Invalid("Code is empty");
  return;
}