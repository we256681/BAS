var DuplicatedGroupId = this.$el.find("#DuplicatedGroupId").val().toUpperCase();
var GroupId = GetInputConstructorValue("GroupId", loader);

var Table = $("#Table").val();


if(DuplicatedGroupId.length == 0)
{
  Invalid("DuplicatedGroupId is empty");
  return;
}



try{
  var code = loader.GetAdditionalData() + _.template($("#database_copy_group_code").html())({group_id: GroupId["updated"], variable: "VAR_" + DuplicatedGroupId,table: Table})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}