var Save = this.$el.find("#Save").val().toUpperCase();
var Mime = GetInputConstructorValue("MimeType", loader);
var Base64 = $("#Check").is(':checked')

if(Mime.length == 0)
{
  Invalid("MimeType is empty");
  return;
}
 try{
  var code = loader.GetAdditionalData() + _.template($("#getclipboard_code").html())({variable:"VAR_" + Save,base64:Base64,mime:Mime["updated"]})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}