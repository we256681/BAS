var Save = this.$el.find("#Save").val().toUpperCase();
var StringValue = GetInputConstructorValue("String", loader);


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


 try{
  var code = loader.GetAdditionalData() + _.template($("#hour_code").html())({variable:"VAR_" + Save,value: StringValue["updated"]})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}