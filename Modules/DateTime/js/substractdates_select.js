var Save = this.$el.find("#Save").val().toUpperCase();
var DecreasingString = GetInputConstructorValue("DecreasingString", loader);
var SubtractedString = GetInputConstructorValue("SubtractedString", loader);



if(Save.length == 0)
{
  Invalid("Variable is empty");
  return;
}

if(DecreasingString["original"].length == 0)
{
  Invalid("Decreasing string is empty");
  return;
}

if(SubtractedString["original"].length == 0)
{
  Invalid("Subtracted string is empty");
  return;
}

 try{
  var code = loader.GetAdditionalData() + _.template($("#substractdates_code").html())({variable:"VAR_" + Save,decreasing: DecreasingString["updated"],subtracted: SubtractedString["updated"]})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}