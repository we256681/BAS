var Method = GetInputConstructorValue("Method", loader);
var Rucaptcha = GetInputConstructorValue("Rucaptcha", loader);
var TimesToSolve = GetInputConstructorValue("TimesToSolve", loader);
var Server = GetInputConstructorValue("Server", loader);
var Tasks = GetInputConstructorValue("Tasks", loader);
var SendProxy = GetInputConstructorValue("SendProxy", loader);
var Check = $("#Check").is(':checked')
if(Method["original"].length == 0)
{
  Invalid("Method is empty");
  return;
}
if(TimesToSolve["original"].length == 0)
{
  Invalid("If captcha appear is empty");
  return;
}

try{
  var path = GetPath(loader)
  var code = loader.GetAdditionalData() + _.template($("#ReCaptcha2_code").html())({path: path["path"],waiter: path["waiter"], disableemulation: Check,method: Method["updated"], rucaptcha: Rucaptcha["updated"], timestosolve:TimesToSolve["updated"], serverurl: Server["updated"], tasklist: Tasks["updated"], query: path["query"], sendproxy: SendProxy["updated"]})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}