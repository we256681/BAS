var Save = this.$el.find("#Save").val().toUpperCase();
var StringValue = GetInputConstructorValue("String", loader);
var Format = GetInputConstructorValue("Format", loader);


if(Save.length == 0)
{
  Invalid("Variable is empty");
  return;
}

if(StringValue["original"].length == 0)
{
  Invalid("String is empty");
  return;
}

if(Format["original"].length == 0)
{
  Invalid("Format is empty");
  return;
}

 try{
  var code = loader.GetAdditionalData() + _.template($("#datetostring_code").html())({variable:"VAR_" + Save,value: StringValue["updated"],format: Format["updated"]})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}