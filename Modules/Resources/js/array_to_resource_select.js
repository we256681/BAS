var ResourceName = $("#ResourceName").val();
var Variable = this.$el.find("#Variable").val().toUpperCase();


if(ResourceName.length <= 0)
{
  Invalid("Resource name is empty");
  return;
}

if(Variable.length <= 0)
{
  Invalid("Variable is empty");
  return;
}

try{
  var code = loader.GetAdditionalData() + _.template($("#array_to_resource_code").html())({key: "\"" + je(ResourceName) + "\"",variable:"VAR_" + Variable})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}