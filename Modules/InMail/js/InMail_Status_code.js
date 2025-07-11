_call_function(_InMail.status, {name: (<%= name %>), timeout: (<%= timeout_value() || 60000 %>)})!
var boxInfo = _result_function();
<%= total %> = boxInfo["messages"]["total"];
<%= recent %> = boxInfo["messages"]["new"];
<%= unseen %> = boxInfo["messages"]["unseen"];
