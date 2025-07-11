<%= variable %> = _join_strings(<%
	var u1 = string1!=='""' || string2!=='""' || string2!=='""'
	var u2 = strings_list!=='""'
	if(u1){
		%>[<%= [string1, string2, string3].filter(function(el){return el !== '""'}).map(function(el){return '_avoid_nil(' + el + ')'}).join(', ') %>]<%
		if(u2){
			%>.concat(<%
		}
	}
	if(u2){
		%>_to_arr(_avoid_nilb(<%= strings_list %>, []))<%
		if(u1){
			%>)<%
		}
	}
%>, <%= separator %>);
