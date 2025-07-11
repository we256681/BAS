<%= result_thread_id %> = <%= desired_thread_id %>
if(<%= result_thread_id %> <= 0)
{
    <%= result_thread_id %> = Math.floor(Math.random() * 1000000) + 1;
}

if(typeof(<%= thread_list %>) == "undefined")
{
    <%= thread_list %> = [];
}
if((<%= thread_list %>).indexOf(<%= result_thread_id %>) < 0)
    (<%= thread_list %>).push(<%= result_thread_id %>)

<% var keys = Object.keys(args) %>

_thread_start(<%= result_thread_id %>, <%= desired_thread_id %> == 0, <%= JSON.stringify(name) %>, {<% for(var i = 0;i<keys.length; i++){  %> <%= JSON.stringify(keys[i]) %> : (<%= args[ keys[i] ] %>) <% if(i < keys.length -1){ %>,<% } %> <% } %>}, (<%= close_thread_after_function_ends %>) == "true", <%= max_threads %>, <%= on_iddle %> != "wait")!
