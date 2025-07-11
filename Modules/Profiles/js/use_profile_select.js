var Path = GetInputConstructorValue("Path", loader);
var LoadFPFromFolder = GetInputConstructorValue("LoadFPFromFolder", loader);
var LoadProxyFromFolder = GetInputConstructorValue("LoadProxyFromFolder", loader);


if(Path["original"].length == 0)
{
  Invalid("Path is empty");
  return;
}

if(LoadFPFromFolder["original"].length == 0)
{
  Invalid("LoadFPFromFolder is empty");
  return;
}

if(LoadProxyFromFolder["original"].length == 0)
{
  Invalid("LoadProxyFromFolder is empty");
  return;
}



try{
  var code = loader.GetAdditionalData() + _.template($("#use_profile_code").html())({profile:Path["updated"], load_fp: LoadFPFromFolder["updated"], load_proxy: LoadProxyFromFolder["updated"]});
  code = Normalize(code,0);
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd, "", true);
}catch(e)
{}