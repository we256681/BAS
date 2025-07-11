var Value = GetInputConstructorValue("Value",loader)
var Mime = GetInputConstructorValue("MimeType",loader)
var Base64 = $("#Check").is(':checked')


if(Value["original"].length <= 0)
{
  Invalid("Data is empty");
  return;
}

if(Mime["original"].length <= 0)
{
  Invalid("MimeType is empty");
  return;
}

 try{
  var code = loader.GetAdditionalData() + _.template($('#setclipboard_code').html())({data:Value["updated"],mime:Mime["updated"],base64:Base64})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{
}