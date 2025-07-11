_call_function(SQL_CountRecords,{"table":(<%= table %>), "where":(<%= where %>), "where_parameterize":(<%= parameterize %>), "timeout":(<%= timeout_value() || 60000 %>)})!
<%= variable %> = _result_function()
