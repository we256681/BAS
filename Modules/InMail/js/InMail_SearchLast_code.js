_call_function(_InMail.searchLast, {timeout: (<%= timeout_value() || 60000 %>)<%if(box!=='""'){%>, box: (<%= box %>)<%}%>})!
<%= variable %> = _result_function();
