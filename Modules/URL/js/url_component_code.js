<% if(encode) { %>
	<%= variable %> = _encode_url_component(<%= string %>)
<% }else{ %>
	<%= variable %> = _decode_url_component(<%= string %>)
<% } %>
