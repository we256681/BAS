<%= variable %> = _parse_date(Date.now(),"auto")
<%= variable %>.getTimezoneOffset = function(){return parseInt(native("datetime", "systemtimezone", "")) }