var Path = GetInputConstructorValue("Path", loader);


try{
  var code = loader.GetAdditionalData() + _.template($("#remove_local_profile_code").html())({profile:Path["updated"]});
  code = Normalize(code,0);
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd, "", true);
}catch(e)
{}