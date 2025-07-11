<% if(isspintax && istemplate){ %>
	_template(<%= template %>)!
	var tmp_res = _avoid_nil(_result());
	tmp_res = _is_not_empty_string(tmp_res) ? tmp_res.replace("BASASYNC","\)\!") : tmp_res;
	<%= variable %> = _spintax(tmp_res);
<% } %>
<% if(!isspintax && istemplate){ %>
	_template(<%= template %>)!
	var tmp_res = _avoid_nil(_result());
	tmp_res = _is_not_empty_string(tmp_res) ? tmp_res.replace("BASASYNC","\)\!") : tmp_res;
	<%= variable %> = tmp_res;
<% } %>

<% if(isspintax && !istemplate){ %>
	<%= variable %> = _spintax(<%= template %>)
<% } %>

<% if(!isspintax && !istemplate){ %>
	<%= variable %> = <%= template %>
<% } %>