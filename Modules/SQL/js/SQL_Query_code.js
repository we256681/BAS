_call_function(SQL_Query,{"query":(<%= query %>), "query_parameterize":(<%= parameterize %>), "data_format":(<%= data_format %>), "timeout":(<%= timeout_value() || 60000 %>)})!
<%= variable %> = _result_function();
