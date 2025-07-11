<%= variable %> = _csv_generate(<%
	var u1 = item1!=='""' || item2!=='""' || item2!=='""'
	var u2 = items_list!=='""'
	if(u1){
		%>[<%= [item1, item2, item3].filter(function(el){return el !== '""'}).map(function(el){return '_avoid_nil(' + el + ')'}).join(', ') %>]<%
		if(u2){
			%>.concat(<%
		}
	}
	if(u2){
		%>_to_arr(_avoid_nilb(<%= items_list %>, []))<%
		if(u1){
			%>)<%
		}
	}
%>, <%= separator %>);
