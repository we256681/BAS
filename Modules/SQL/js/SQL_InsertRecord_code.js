_call_function(SQL_InsertRecord,{"table":(<%= table %>), "fields":(<%= fields %>), "data": (<%= data %>), "convert":(<%= convert %>),<% if(idFieldName!=='""'){ %> "idFieldName":(<%= idFieldName %>),<% } %> "timeout":(<%= timeout_value() || 60000 %>)})!
<% if(idFieldName!=='""'){ %>if(<%= idFieldName %>){
	<%= variable %> = _result_function();
};<% } %>
