var GroupId = GetInputConstructorValue("GroupId", loader);
var DeleteResources = GetInputConstructorValue("DeleteResources", loader);

var Table = $("#Table").val();

if(DeleteResources["original"] == 0)
{
  Invalid("DeleteResources is empty");
  return;
}

var filters = _Filter.GetFilters()
var Filters = " [ " 
filters.forEach(function(filter, item_index_all){
	Filters += " [ " 
	filter.forEach(function(item, item_index){
		if(item_index == 0)
			loader.SetIsString(false)
		if(item_index == 1)
			loader.SetIsString(true)
		if(item_index == 2)
			loader.SetIsString(false)
		
    	var add = loader.Resolve(item);
    	Filters += add
    	
		if(item_index != filter.length - 1)
			Filters += ","
	})
	
	Filters += " ] " 

	if(item_index_all != filters.length - 1)
		Filters += ","	
})

Filters += " ]" 

try{
  var code = loader.GetAdditionalData() + _.template($("#database_delete_records_code").html())({filters: Filters,table: Table, group_id: GroupId["updated"], delete_resources: DeleteResources["updated"]})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}