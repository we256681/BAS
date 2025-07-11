var RecordId = GetInputConstructorValue("RecordId", loader);
var RecordString = this.$el.find("#RecordString").val().toUpperCase();
var Table = $("#Table").val();

if(RecordString.length == 0)
{
  Invalid("Record is empty");
  return;
}

if(RecordId["original"] == 0)
{
  Invalid("RecordId is empty");
  return;
}


try{

  var code = loader.GetAdditionalData() + _.template($("#database_select_one_record_code").html())
  (
  	{
  		record_id:RecordId["updated"],
  		table: Table,
  		variable:"VAR_" + RecordString
  	}
  )

  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{
	alert(e)
}