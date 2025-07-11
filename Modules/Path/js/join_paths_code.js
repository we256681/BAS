<%= variable %> = _path.join(<%
	var u1 = path1!=='""' || path2!=='""' || path2!=='""'
	var u2 = paths_list!=='""'
	if(u1){
		%>[<%= [path1, path2, path3].filter(function(el){return el !== '""'}).map(function(el){return '_avoid_nil(' + el + ')'}).join(', ') %>]<%
		if(u2){
			%>.concat(<%
		}
	}
	if(u2){
		%>_to_arr(_avoid_nilb(<%= paths_list %>, []))<%
		if(u1){
			%>)<%
		}
	}
%>);
