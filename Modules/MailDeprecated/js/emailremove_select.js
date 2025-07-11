var Value = GetInputConstructorValue("Value", loader);
            
if(Value["original"].length == 0)
{
  Invalid("Eamil id is empty");
  return;
}
 try{
  var code = loader.GetAdditionalData() + _.template($("#emailremove_code").html())({mailid: Value["updated"]})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}