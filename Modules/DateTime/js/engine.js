function _parse_date(value, format)
{
	if(typeof(value) == "object")
		return value
	var split = (native("datetime", "stringtodate", JSON.stringify({value:value,format:format}))).split(",");
	var res = new Date(parseInt(split[0]));
	res.getTimezoneOffset = function(){return parseInt(split[1])}
	return res;
}

function _format_date(value, format)
{
	var date = _parse_date(value, "auto")
	return native("datetime", "datetostring", JSON.stringify({time:date.toISOString(),offset:date.getTimezoneOffset(),format:format}));
}