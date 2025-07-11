<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"path", description: tr("Path"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The path from which to get the directory."),examples:[{code:"/foo/bar/baz/asdf/quux"},{code:"D:/modules/res/panels/panel3.jpg"},{code:"C:/Users/Admin/Desktop/projects/project.xml"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "PATH_DIRECTORY", help: {description: tr("Variable in which, after successful execution of the action, the directory will be written."), examples:[{code:"/foo/bar/baz/asdf",description:tr("Path") + ": <code style=\"font-size:85%\">/foo/bar/baz/asdf/quux</code>"},{code:"D:/modules/res/panels",description:tr("Path") + ": <code style=\"font-size:85%\">D:/modules/res/panels/panel3.jpg</code>"},{code:"C:/Users/Admin/Desktop/projects",description:tr("Path") + ": <code style=\"font-size:85%\">C:/Users/Admin/Desktop/projects/project.xml</code>"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get the directory of the specified path.</div>
	<div class="tr tooltip-paragraph-fold">This action will return the directory of the specified path, trailing directory separators are ignored.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the path <code>"C:/foo/bar/baz/asdf/quux.html/"</code> is specified, then the action will return <code>"C:/foo/bar/baz/asdf"</code>.</div>
	<div class="tr tooltip-paragraph-fold">You can parse a path into items using the "Parse path" action.</div>
	<div class="tr tooltip-paragraph-last-fold">If the "Path" parameter is not a string, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
