var group_id = (<%= group_id %>)
if(group_id.length == 0)
	group_id = P("_bas_table_" + (<%= table %>), "_last_group")

<%= variable %> = _db_copy_group(group_id, <%= table %>)