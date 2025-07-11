<% if(encode) { %>
	<%= variable %> = _punycode.urlToASCII(<%= string %>)
<% }else{ %>
	<%= variable %> = _punycode.urlToUnicode(<%= string %>)
<% } %>
