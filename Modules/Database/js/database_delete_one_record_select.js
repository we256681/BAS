var RecordId = GetInputConstructorValue("RecordId", loader);
var DeleteResources = GetInputConstructorValue("DeleteResources", loader);

var Table = $("#Table").val();

if(DeleteResources["original"] == 0)
{
  Invalid("DeleteResources is empty");
  return;
}



try{

  var code = loader.GetAdditionalData() + _.template($("#database_delete_one_record_code").html())({delete_resources: DeleteResources["updated"], record_id:RecordId["updated"],table: Table})

  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{
	alert(e)
}