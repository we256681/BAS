 <%= variable %> = native("regexp", "scan", JSON.stringify({text: <%= string %>,regexp:<%= regexp %>}))
if(<%= variable %>.length == 0)
	<%= variable %> = []
else
	<%= variable %> = JSON.parse(<%= variable %>)