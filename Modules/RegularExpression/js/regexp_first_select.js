var Value = GetInputConstructorValue("Value",loader)
var Regexp = GetInputConstructorValue("Regexp",loader)
var Result = this.$el.find("#Result").val().toUpperCase();
var ResultAll = this.$el.find("#ResultAll").val().toUpperCase();


if(Value["original"] == 0)
{
  Invalid("String is empty");
  return;
}

if(Regexp["original"] == 0)
{
  Invalid("Regexp is empty");
  return;
}

if(Result.length == 0)
{
  Invalid("Result is empty");
  return;
}

if(ResultAll.length == 0)
{
  Invalid("Entire match is empty");
  return;
}


try{
  var code = loader.GetAdditionalData() + _.template($("#regexp_first_code").html())({variable:Result,resultall:"VAR_" + ResultAll,string:Value["updated"],regexp:Regexp["updated"]})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}