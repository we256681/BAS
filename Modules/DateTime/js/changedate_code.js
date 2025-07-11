{
	<%= variable %> = _parse_date(<%= value %>, "auto")
	var offset = <%= variable %>.getTimezoneOffset()
	<%= variable %> = new Date(<%= variable %>.getTime())
	<%= variable %>.getTimezoneOffset = function(){return offset}
}
if(<%= year %> != -1)
	<%= variable %>.setFullYear(<%= year %>);

if(<%= month %> != -1)
	<%= variable %>.setMonth(<%= month %> - 1);

if(<%= day %> != -1)
	<%= variable %>.setDate(<%= day %>);

if(<%= hour %> != -1)
	<%= variable %>.setHours(<%= hour %>);

if(<%= minute %> != -1)
	<%= variable %>.setMinutes(<%= minute %>);

if(<%= second %> != -1)
	<%= variable %>.setSeconds(<%= second %>);

if(<%= millisecond %> != -1)
	<%= variable %>.setMilliseconds(<%= millisecond %>);