<% if(encode) { %>
	<%= variable %> = base64_encode(<%= value %>)
<% }else{ %>
	<%= variable %> = base64_decode(<%= value %>)
<% } %>
