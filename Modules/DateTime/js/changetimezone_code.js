{
	<%= variable %> = _parse_date(<%= value %>, "auto")
	<%= variable %> = new Date(<%= variable %>.getTime())
	<%= variable %>.getTimezoneOffset = function(){return parseInt(<%= timezone %>)}
}
