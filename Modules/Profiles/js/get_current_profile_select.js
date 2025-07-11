var Save = this.$el.find("#Save").val().toUpperCase();
var Save2 = this.$el.find("#Save2").val().toUpperCase();
var Save3 = this.$el.find("#Save3").val().toUpperCase();


if(Save.length == 0)
{
  Invalid("Variable is empty");
  return;
}

if(Save2.length == 0)
{
  Invalid("Variable2 is empty");
  return;
}

if(Save3.length == 0)
{
  Invalid("Variable3 is empty");
  return;
}


 try{
  var code = loader.GetAdditionalData() + _.template($('#get_current_profile_code').html())(
      {
        variable:"VAR_" + Save,
        variable2:"VAR_" + Save2,
        variable3:"VAR_" + Save3
      }
    )
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{
}