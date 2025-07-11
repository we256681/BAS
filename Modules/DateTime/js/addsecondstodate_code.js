{
	<%= variable %> = _parse_date(<%= value %>,"auto")
	var offset = <%= variable %>.getTimezoneOffset()
	<%= variable %> = new Date(<%= variable %>.getTime() + (<%= seconds %>) * 1000 + (<%= minutes %>) * 1000 * 60 + (<%= hours %>) * 1000 * 60 * 60 + (<%= days %>) * 1000 * 60 * 60 * 24 )
	<%= variable %>.getTimezoneOffset = function(){return offset}
}