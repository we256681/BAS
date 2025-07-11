<% if(encode) { %>
	<%= variable %> = _escape_html(<%= string %>)
<% }else{ %>
	<%= variable %> = _unescape_html(<%= string %>)
<% } %>
