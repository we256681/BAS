var GroupId = GetInputConstructorValue("GroupId", loader);
var DeleteData = GetInputConstructorValue("DeleteData", loader);

var Table = $("#Table").val();


if(GroupId["original"].length == 0)
{
  Invalid("GroupId is empty");
  return;
}

if(DeleteData["original"].length == 0)
{
  Invalid("DeleteData is empty");
  return;
}


try{
  var code = loader.GetAdditionalData() + _.template($("#database_delete_group_code").html())({group_id: GroupId["updated"], delete_data: DeleteData["updated"],table: Table})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}