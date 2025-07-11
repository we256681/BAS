<%= timeout_general() %>
imap_client_search(<%= sender %>,<%= subject %>,<%= body %>,<%= to %>)!
<%= variable %> = imap_client_search_result()
if(<%= variable %>.length > 0)
<%= variable %> = <%= variable %>[<%= variable %>.length - 1]
else
<%= variable %> = ""
<%= mailid %> = <%= variable %>
<%= timeout_general() %>
imap_client_pull_message(<%= variable %>)!
<%= variable %> = imap_client_message()

var csv_parse_result = <%= variable %>.match(LINK_REGEXP) || []
<% for(var i = 0;i < link_list.length; i++){ %>
VAR_<%= link_list[i] %> = csv_parse_result[<%= i %>]
if(typeof(VAR_<%= link_list[i] %>) == 'undefined' || !VAR_<%= link_list[i] %>)
{
    VAR_<%= link_list[i] %> = ""
}
<% } %>