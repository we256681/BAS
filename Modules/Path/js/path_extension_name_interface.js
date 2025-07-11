<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"path", description: tr("Path"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The path from which to get the extension."),examples:[{code:"/home/user/dir/file.txt"},{code:"D:/dev/sql/sqlite/your.db"},{code:"C:/Users/Admin/Desktop/site/index.html"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "PATH_FILE_EXTENSION", help: {description: tr("Variable in which, after successful execution of the action, the extension will be written."), examples:[{code:".txt",description:tr("Path") + ": <code style=\"font-size:85%\">/home/user/dir/file.txt</code>"},{code:".db",description:tr("Path") + ": <code style=\"font-size:85%\">D:/dev/sql/sqlite/your.db</code>"},{code:".html",description:tr("Path") + ": <code style=\"font-size:85%\">C:/Users/Admin/Desktop/site/index.html</code>"},{code:tr("Empty string"),description:tr("Path") + ": <code style=\"font-size:85%\">/foo/bar/baz/asdf/quux</code>"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get the file extension of the specified path.</div>
	<div class="tr tooltip-paragraph-fold">This will return the expansion of the last part of the path, from the last period to the end of the string, trailing directory separators are ignored.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the path <code>"C:/foo/bar/baz/asdf/quux.html/"</code> is specified, then the action will return <code>".html"</code>.</div>
	<div class="tr tooltip-paragraph-fold">If there is no period in the last part or there are no characters before the last period, then the action will return an empty string.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the path is <code>"C:/foo/bar/baz/asdf/quux/"</code> or <code>"C:/foo/bar/baz/asdf/.index"</code>, then the action will return <code>""</code>.</div>
	<div class="tr tooltip-paragraph-fold">You can parse a path into items using the "Parse path" action.</div>
	<div class="tr tooltip-paragraph-last-fold">If the "Path" parameter is not a string, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
