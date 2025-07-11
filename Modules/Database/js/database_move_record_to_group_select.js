var MovedRecordId = this.$el.find("#MovedRecordId").val().toUpperCase();
var GroupId = GetInputConstructorValue("GroupId", loader);
var MoveType = GetInputConstructorValue("MoveType", loader);
var RecordId = GetInputConstructorValue("RecordId", loader);


var Table = $("#Table").val();


if(MovedRecordId.length == 0)
{
  Invalid("MovedRecordId is empty");
  return;
}

if(MoveType["original"] == 0)
{
  Invalid("MoveType is empty");
  return;
}

if(RecordId["original"] == 0)
{
  Invalid("RecordId is empty");
  return;
}



try{
  var code = loader.GetAdditionalData() + _.template($("#database_move_record_to_group_code").html())(
  	{
  		group_id: GroupId["updated"], 
  		move_type: MoveType["updated"], 
  		record_id: RecordId["updated"], 
  		variable: "VAR_" + MovedRecordId,
  		table: Table
  	})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}