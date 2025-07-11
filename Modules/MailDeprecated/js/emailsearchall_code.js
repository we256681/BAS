<%= timeout_general() %>
imap_client_search(<%= sender %>,<%= subject %>,<%= body %>,<%= to %>)!
<%= variable %> = imap_client_search_result()