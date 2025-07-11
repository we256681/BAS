var group = (<%= group %>)
if(group.length == 0)
	group = P("_bas_table_" + (<%= table %>), "_last_group")

_db_change_group(<%= name %>, <%= description %>, group, <%= table %>)
