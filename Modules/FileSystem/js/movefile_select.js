var Value = GetInputConstructorValueFilename("Value", loader);
var ValueDest = GetInputConstructorValueFilename("ValueDest", loader);

if(Value["original"].length == 0)
{
  Invalid("File path is empty");
  return;
}

if(ValueDest["original"].length == 0)
{
  Invalid("File path is empty");
  return;
}

 try{
  var code = loader.GetAdditionalData() + _.template($("#movefile_code").html())({value: Value["updated"],dest: ValueDest["updated"]})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}