var regexp_result = native("regexp", "first", JSON.stringify({text: <%= string %>,regexp:<%= regexp %>}))
if(regexp_result.length == 0)
	regexp_result = []
else
	regexp_result = JSON.parse(regexp_result)

<%= resultall %> = regexp_result.pop()
if(typeof(<%= resultall %>) == 'undefined' || !<%= resultall %>)
    <%= resultall %> = ""

<% var list = variable.split(",") %>

<% for(var i = 0;i < list.length; i++){ %>
  VAR_<%= list[i] %> = regexp_result[<%= i %>]
  if(typeof(VAR_<%= list[i] %>) == 'undefined' || !VAR_<%= list[i] %>)
    VAR_<%= list[i] %> = ""
<% } %>

if(regexp_result.length == 0)
{
	VAR_<%= list[0] %> = <%= resultall %>
}