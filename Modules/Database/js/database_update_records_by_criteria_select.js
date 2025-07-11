var GroupId = GetInputConstructorValue("GroupId", loader);
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
  var code = loader.GetAdditionalData() + _.template($("#database_update_records_by_criteria_code").html())({update_resources: UpdateResources["updated"], table: Table, params: params, group_id: GroupId["updated"],filters: Filters})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}