var RecordId = GetInputConstructorValue("RecordId", loader);
var UpdateResources = GetInputConstructorValue("UpdateResources", loader);

var Table = $("#Table").val();

if(UpdateResources["original"] == 0)
{
  Invalid("UpdateResources is empty");
  return;
}

var params = []

var els = $("*[data-table-id=" + Table + "]")
for(var i = 0;i<els.length;i++)
{
	var el = els[i]
	var id = el.attributes["data-id"]["value"]
	
	var p = GetInputConstructorValue("Table_" + Table + "_Column_" + id, loader);
	if(p["original"].length > 0)
	{
		params.push(id)
		params.push(p["updated"])
	}
}


try{
  var code = loader.GetAdditionalData() + _.template($("#database_update_record_code").html())({update_resources: UpdateResources["updated"], record:RecordId["updated"],table: Table, params: params})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}