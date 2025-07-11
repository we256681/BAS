var GroupList = this.$el.find("#GroupList").val().toUpperCase();

var Table = $("#Table").val();

if(GroupList.length == 0)
{
  Invalid("GroupList is empty");
  return;
}


try{
  var code = loader.GetAdditionalData() + _.template($("#database_get_group_list_code").html())({variable:"VAR_" + GroupList,table: Table})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}