var Path = GetInputConstructorValue("Path", loader);

if(Path["original"].length == 0)
{
  Invalid("Path is empty");
  return;
}


try{
  var code = loader.GetAdditionalData() + _.template($("#save_profile_code").html())({profile:Path["updated"]});
  code = Normalize(code,0);
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd, "", true);
}catch(e)
{}