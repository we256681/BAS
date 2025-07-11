var Name = GetInputConstructorValue("Name", loader);
var GroupId = GetInputConstructorValue("GroupId", loader);
var Description = GetInputConstructorValue("Description", loader);
var Table = $("#Table").val();


try{
  var code = loader.GetAdditionalData() + _.template($("#database_change_group_code").html())({group:GroupId["updated"],name: Name["updated"],description: Description["updated"],table: Table})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}