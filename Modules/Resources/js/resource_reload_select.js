var ResourceName = $("#ResourceName").val();

if(ResourceName.length == 0)
{
  Invalid("Resource name is empty");
  return;
}

try{
  var code = loader.GetAdditionalData() + _.template($("#resource_reload_code").html())({key: "\"" + je(ResourceName) + "\""})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}