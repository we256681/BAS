<%= variable %> = new Date(Date.UTC(<%= year %>, <%= month %> - 1, <%= day %>, <%= hour %>, <%= minute %>, <%= second %>, <%= millisecond %>));
<%= variable %>.getTimezoneOffset = function(){return parseInt(<%= timezone %>)}
