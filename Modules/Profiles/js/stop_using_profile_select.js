try{
  var code = loader.GetAdditionalData() + _.template($("#stop_using_profile_code").html())({});
  code = Normalize(code,0);
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd, "", true);
}catch(e)
{}