var Save = this.$el.find("#Save").val().toUpperCase();
var StringValue = GetInputConstructorValue("String", loader);
var Seconds = GetInputConstructorValue("Seconds", loader);
var Minutes = GetInputConstructorValue("Minutes", loader);
var Hours = GetInputConstructorValue("Hours", loader);
var Days = GetInputConstructorValue("Days", loader);


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

if(Seconds["original"].length == 0)
{
  Invalid("Seconds is empty");
  return;
}

if(Minutes["original"].length == 0)
{
  Invalid("Minutes is empty");
  return;
}


if(Hours["original"].length == 0)
{
  Invalid("Hours is empty");
  return;
}

if(Days["original"].length == 0)
{
  Invalid("Days is empty");
  return;
}

 try{
  var code = loader.GetAdditionalData() + _.template($("#addsecondstodate_code").html())({variable:"VAR_" + Save,value: StringValue["updated"],seconds: Seconds["updated"],minutes: Minutes["updated"],hours: Hours["updated"],days: Days["updated"]})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}